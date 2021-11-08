#include <TFG.hpp>

#include <benchmark/benchmark.h>
#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ranges>
#include <string>
#include <utility>

auto biggestNBitsPrime(std::size_t bits) -> mpz_class
{
    assert(bits >= 2);

    auto n = 2_mpz;
    mpz_pow_ui(n.get_mpz_t(), n.get_mpz_t(), bits);
    --n;

    while (true)
    {
        if (mpz_probab_prime_p(n.get_mpz_t(), 50) > 0)
        {
            return n;
        }
        n -= 2;
    }
}

auto generateTestPrimes(std::size_t n) -> std::vector<mpz_class>
{
    auto primes = std::vector<mpz_class>(n);

    std::ranges::generate(primes, [bits = std::size_t(1)]() mutable {
        return biggestNBitsPrime(++bits);
    });

    return primes;
}


auto generatePrimePowers(std::size_t exp, std::vector<mpz_class> const& primes) -> std::vector<mpz_class>
{
    assert(exp >= 2);

    auto primePowers = primes;

    for (auto& primePower : primePowers)
    {
        mpz_pow_ui(primePower.get_mpz_t(), primePower.get_mpz_t(), exp);
    }

    return primePowers;
}

struct MeanBestWorstResult
{
    double mean;
    double best;
    double worst;
};

struct ResultFiles
{
    std::ofstream meanResults;
    std::ofstream bestResults;
    std::ofstream worstResults;

    ResultFiles(std::string const& prefix) :
        meanResults{prefix + "-mean.txt", std::ios::out | std::ios::trunc},
        bestResults{prefix + "-best.txt", std::ios::out | std::ios::trunc},
        worstResults{prefix + "-worst.txt", std::ios::out | std::ios::trunc}
    {}

    auto save(mpz_class const& n, MeanBestWorstResult const& results) -> void   
    {
        meanResults << n << ' ' << results.mean << '\n';
        bestResults << n << ' ' << results.best << '\n';
        worstResults << n << ' ' << results.worst << '\n';
    }
};

template <std::size_t Size>
auto meanBestWorst(std::array<double, Size> const& times) -> MeanBestWorstResult
{
    auto mean = times[0];
    auto best = times[0];
    auto worst = times[0];

    for (auto i = std::size_t{1}; i < Size; ++i)
    {
        mean += times[i];
        best = std::ranges::min(best, times[i]);
        worst = std::ranges::max(worst, times[i]);
    }

    return MeanBestWorstResult{mean / Size, best, worst};
}

template <typename F>
requires std::invocable<F, mpz_class>
auto measure(F&& f, mpz_class const& n) -> MeanBestWorstResult
{
    using namespace std::chrono;

    auto times = std::array<double, 5>{};
    std::ranges::generate(times, [&f, &n] {
        auto const before = high_resolution_clock::now();
        benchmark::DoNotOptimize(std::invoke(std::forward<F>(f), n));
        auto const after = high_resolution_clock::now();

        return duration_cast<duration<double>>(after - before).count();
    });

    return meanBestWorst(times);
}

template <std::ranges::range R, std::invocable<mpz_class> F>
requires std::same_as<std::ranges::range_value_t<R>, mpz_class>
auto measure(F&& f, R&& testCases, std::string const& prefix) -> void
{
    auto results = ResultFiles{prefix};

    for (auto const& testCase : testCases)
    {
        fmt::print("{} {}...\n", prefix, testCase.get_str());
        results.save(testCase, measure(std::forward<F>(f), testCase));
    }
}

template <std::ranges::range R>
requires std::same_as<std::ranges::range_value_t<R>, mpz_class>
auto measureAKSVariants(R&& primes) -> void
{
    std::filesystem::create_directory("data/aks");

    measure([](mpz_class const& n) {
        auto const r = tfg::aks::steps::step2(n);
        return tfg::aks::steps::impl::step5Direct(n, r);
    }, primes, "data/aks/direct");

    measure([](mpz_class const& n) {
        auto const r = tfg::aks::steps::step2(n);
        return tfg::aks::steps::impl::step5NTL(n, r);
    }, primes, "data/aks/ntl");
}

template <std::ranges::range R>
requires std::same_as<std::ranges::range_value_t<R>, mpz_class>
auto measureProbabilistics(R&& primes) -> void
{
    auto const roundsCases = std::array
    {
        1_mpz, 2_mpz, 4_mpz, 8_mpz, 16_mpz, 32_mpz, 64_mpz, 128_mpz, 256_mpz,
        512_mpz, 1024_mpz, 2048_mpz, 4096_mpz, 8192_mpz, 16384_mpz, 32768_mpz,
        65536_mpz
    };

    std::filesystem::create_directory("data/probs");

    auto prng = gmp_randclass{gmp_randinit_mt};
    prng.seed(0);

    for (auto const& rounds : roundsCases)
    {
        measure([&rounds, &prng](mpz_class const& n) {
            return tfg::miller_rabin::isProbablyPrime(n, rounds, prng);
        }, primes, "data/probs/miller-rabin-" + rounds.get_str());

        measure([&rounds, &prng](mpz_class const& n) {
            return tfg::solovay_strassen::isProbablyPrime(n, rounds, prng);
        }, primes, "data/probs/solovay-strassen-" + rounds.get_str());
    }
}

template <std::ranges::range R>
requires std::same_as<std::ranges::range_value_t<R>, mpz_class>
auto measureAKSProbabilisticsPrimes(R&& primes) -> void
{
    auto const roundsMillerRabin = 40_mpz;
    auto const roundsSolovayStrassen = 80_mpz;
    auto prng = gmp_randclass{gmp_randinit_mt};
    prng.seed(0);

    std::filesystem::create_directory("data/aks-probs-primes");

    measure(tfg::aks::isPrime, primes, "data/aks-probs-primes/aks");

    measure([&roundsMillerRabin, &prng](mpz_class const& n) {
        return tfg::miller_rabin::isProbablyPrime(n, roundsMillerRabin, prng);
    }, primes, "data/aks-probs-primes/miller-rabin");

    measure([&roundsSolovayStrassen, &prng](mpz_class const& n) {
        return tfg::solovay_strassen::isProbablyPrime(n, roundsSolovayStrassen, prng);
    }, primes, "data/aks-probs-primes/solovay-strassen");
}

template <std::ranges::range R>
requires std::same_as<std::ranges::range_value_t<R>, mpz_class>
auto measureAKSProbabilisticsPrimePowers(R&& primes, std::size_t exp) -> void
{
    auto const roundsMillerRabin = 40_mpz;
    auto const roundsSolovayStrassen = 80_mpz;
    auto prng = gmp_randclass{gmp_randinit_mt};
    prng.seed(0);

    std::filesystem::create_directory("data/aks-probs-powers-" + std::to_string(exp));

    auto primePowers = primes | std::ranges::views::transform([&exp](mpz_class n) {
        mpz_pow_ui(n.get_mpz_t(), n.get_mpz_t(), exp);
        return n;
    });

    measure(tfg::aks::isPrime, primePowers, "data/aks-probs-powers-" + std::to_string(exp) + "/aks");

    measure([&roundsMillerRabin, &prng](mpz_class const& n) {
        return tfg::miller_rabin::isProbablyPrime(n, roundsMillerRabin, prng);
    }, primePowers, "data/aks-probs-powers-" + std::to_string(exp) + "/miller-rabin");

    measure([&roundsSolovayStrassen, &prng](mpz_class const& n) {
        return tfg::solovay_strassen::isProbablyPrime(n, roundsSolovayStrassen, prng);
    }, primePowers, "data/aks-probs-powers-" + std::to_string(exp) + "/solovay-strassen");
}

template <std::ranges::range R>
requires std::same_as<std::ranges::range_value_t<R>, mpz_class>
auto measureAKSProbabilisticsComposites(R&& primes, mpz_class const& prime) -> void
{
    auto const roundsMillerRabin = 40_mpz;
    auto const roundsSolovayStrassen = 80_mpz;
    auto prng = gmp_randclass{gmp_randinit_mt};
    prng.seed(0);

    auto const directoryName = "data/aks-probs-comps-" + std::to_string(mpz_sizeinbase(prime.get_mpz_t(), 2));
    std::filesystem::create_directory(directoryName);

    auto composites = primes | std::ranges::views::transform([&prime](mpz_class n) {
        return n *= prime;
    });

    measure(tfg::aks::isPrime, composites, directoryName + "/aks");

    measure([&roundsMillerRabin, &prng](mpz_class const& n) {
        return tfg::miller_rabin::isProbablyPrime(n, roundsMillerRabin, prng);
    }, composites, directoryName + "/miller-rabin");

    measure([&roundsSolovayStrassen, &prng](mpz_class const& n) {
        return tfg::solovay_strassen::isProbablyPrime(n, roundsSolovayStrassen, prng);
    }, composites, directoryName + "/solovay-strassen");
}

auto main(int argc, char const* argv[]) -> int
{
    namespace fs = std::filesystem;

    if (argc != 1)
    {
        fmt::print(stderr, "No arguments needed. Usage: {}", argv[0]);
    }

    auto const primes = generateTestPrimes(512);

    fs::remove_all("data");
    fs::create_directory("data");

    measureAKSVariants(primes | std::ranges::views::take(15));
    measureProbabilistics(primes | std::ranges::views::take(31));
    measureAKSProbabilisticsPrimes(primes | std::ranges::views::take(31));
    measureAKSProbabilisticsPrimePowers(primes | std::ranges::views::take(15), 100);
    measureAKSProbabilisticsPrimePowers(primes
        | std::ranges::views::take(256)
        | std::ranges::views::drop(192), 5);
    measureAKSProbabilisticsComposites(primes
        | std::ranges::views::take(41)
        | std::ranges::views::drop(31), primes[14]);
    measureAKSProbabilisticsComposites(primes
        | std::ranges::views::take(41)
        | std::ranges::views::drop(31), primes[30]);
}
