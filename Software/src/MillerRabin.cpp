#include "MillerRabin.hpp"

#include "GMPWrappers.hpp"

#include <memory>
#include <random>
#include <utility>

namespace tfg::miller_rabin
{

[[nodiscard]] auto isProbablyPrime(mpz_class const& n, mpz_class const& k, gmp_randclass& prng) -> bool
{
    if (n == 2_mpz || n == 3_mpz)
    {
        return true;
    }

    if (n < 2 || n % 2 == 0)
    {
        return false;
    }

    // Calculate n = d*2^r - 1, with d being odd
    auto const [r, d] = [&n]
    {
        auto dResult = mpz_class{n - 1};
        auto rResult = mpz_scan1(dResult.get_mpz_t(), 0);
        mpz_fdiv_q_2exp(dResult.get_mpz_t(), dResult.get_mpz_t(), rResult);

        return std::make_pair(rResult, dResult);
    }();

    for (auto i = 0_mpz; i < k; ++i)
    {
        // Get random number in the range [2, n-2]
        auto const a = mpz_class{prng.get_z_range(n - 3) + 2};
        auto x = gmp::powMod(a, d, n);

        if (x != 1 && x != n - 1)
        {
            for (auto j = mp_bitcnt_t{0}; j < r - 1; ++j)
            {
                x = gmp::powMod(x, 2, n);

                if (x == n - 1)
                {
                    break;
                }
            }

            if (x != n - 1)
            {
                return false;
            }
        }
    }

    return true;
}

[[nodiscard]] auto isProbablyPrime(mpz_class const& n, mpz_class const& k) -> bool
{
    // We use a unique_ptr to surpass the copy constructor being private
    static auto prng = []{
        auto result = std::make_unique<gmp_randclass>(gmp_randinit_mt);
        result->seed(std::random_device{}());

        return result;
    }();

    return tfg::miller_rabin::isProbablyPrime(n, k, *prng);
}

} // namespace tfg::miller_rabin