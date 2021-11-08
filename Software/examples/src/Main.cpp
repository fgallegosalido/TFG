#include <TFG.hpp>

#include <fmt/core.h>
#include <magic_enum.hpp>

#include <chrono>
#include <iostream>
#include <utility>

enum class Algorithm
{
    Basic,
    AKS,
    MillerRabin,
    SolovayStrassen
};

auto measure(auto&& f, mpz_class const& n)
{
    using namespace std::chrono;

    auto const before = high_resolution_clock::now();
    auto const result = std::forward<decltype(f)>(f)(n);
    auto const after = high_resolution_clock::now();
    auto const elapsed = duration_cast<duration<double>>(after - before).count();

    fmt::print("Elapsed time: {}\n", elapsed);

    return result;
}

[[nodiscard]] auto typeRoundsMenu() -> mpz_class
{
    mpz_class roundsNumber;

    do
    {
        fmt::print("Type the number of rounds to perform: ");
        std::cin >> roundsNumber;
    } while (!std::cin.good() || roundsNumber < 1);

    return roundsNumber;
}

auto selectAlgorithmMenu(mpz_class const &selectedNumber) -> void
{
    static constexpr auto algorithms = magic_enum::enum_entries<Algorithm>();
    static constexpr auto algorithmsCount = magic_enum::enum_count<Algorithm>();
    std::size_t selectedAlgorithm{};

    do
    {
        fmt::print("Select algorithm:\n");
        for (auto const &algorithm : algorithms)
        {
            fmt::print("    {}. {}\n",
                       magic_enum::enum_index(algorithm.first).value(),
                       algorithm.second);
        }
        fmt::print("Enter selection number: ");

        std::cin >> selectedAlgorithm;
    } while (!std::cin.good() || algorithmsCount <= selectedAlgorithm);

    switch (magic_enum::enum_value<Algorithm>(selectedAlgorithm))
    {
    case Algorithm::Basic:
        fmt::print("\nRunning basic algorithm... \n");
        fmt::print("{}prime\n\n", measure(tfg::basic::isPrime, selectedNumber) ? "" : "Not ");
        break;
    case Algorithm::AKS:
        fmt::print("\nRunning AKS algorithm... \n");
        fmt::print("{}prime\n\n", measure(tfg::aks::isPrime, selectedNumber) ? "" : "Not ");
        break;
    case Algorithm::MillerRabin:
        {
            auto const roundsNumber = typeRoundsMenu();
            fmt::print("\nRunning Miller-Rabin algorithm... \n");
            fmt::print("{}prime\n\n",
                measure([&roundsNumber](mpz_class const& n) {
                    return tfg::miller_rabin::isProbablyPrime(n, roundsNumber);
                }, selectedNumber) ? "Probably " : "Not "
            );
        }
        break;
    case Algorithm::SolovayStrassen:
        {
            auto const roundsNumber = typeRoundsMenu();
            fmt::print("\nRunning Solovay-Strassen algorithm... \n");
            fmt::print("{}prime\n\n",
                measure([&roundsNumber](mpz_class const& n) {
                    return tfg::solovay_strassen::isProbablyPrime(n, roundsNumber);
                }, selectedNumber) ? "Probably " : "Not "
            );
        }
        break;
    default:
        fmt::print("Unknown algorithm\n");
        break;
    }
}

auto typeNumberMenu() -> void
{
    mpz_class selectedNumber;

    while (true)
    {
        do
        {
            fmt::print("Type a number to test for primality or -1 to exit: ");
            std::cin >> selectedNumber;
        } while (!std::cin.good());

        if (selectedNumber < 0)
        {
            break;
        }

        selectAlgorithmMenu(selectedNumber);
    }
}

auto main(int argc, char const* argv[]) -> int
{
    if (argc == 1)
    {
        typeNumberMenu();
    }
    else if (argc == 3)
    {
        switch (std::atoi(argv[1]))
        {
        case 0:
            fmt::print("\nRunning basic algorithm... \n");
            fmt::print("{}prime\n\n", measure(tfg::basic::isPrime, mpz_class{argv[2]}) ? "" : "Not ");
            break;
        case 1:
            fmt::print("\nRunning AKS algorithm... \n");
            fmt::print("{}prime\n\n", measure(tfg::aks::isPrime, mpz_class{argv[2]}) ? "" : "Not ");
            break;
        default:
            fmt::print(stderr, "Invalid algorithm (0=Basic,1=AKS,2=Miller-Rabin,3=Solovay-Strassen). Usage: {} [<number> <test> [<rounds>]]", argv[0]);
            break;
        }
    }
    else if (argc == 4)
    {
        switch (std::atoi(argv[1]))
        {
        case 2:
            {
                fmt::print("\nRunning Miller-Rabin algorithm... \n");
                fmt::print("{}prime\n\n",
                    measure([roundsNumber = mpz_class{argv[3]}](mpz_class const& n) {
                        return tfg::miller_rabin::isProbablyPrime(n, roundsNumber);
                    }, mpz_class{argv[2]}) ? "Probably " : "Not "
                );
            }
            break;
        case 3:
            {
                fmt::print("\nRunning Miller-Rabin algorithm... \n");
                fmt::print("{}prime\n\n",
                    measure([roundsNumber = mpz_class{argv[3]}](mpz_class const& n) {
                        return tfg::solovay_strassen::isProbablyPrime(n, roundsNumber);
                    }, mpz_class{argv[2]}) ? "Probably " : "Not "
                );
            }
            break;
        default:
            fmt::print(stderr, "Invalid algorithm (0=Basic,1=AKS,2=Miller-Rabin,3=Solovay-Strassen). Usage: {} [<number> <test> [<rounds>]]", argv[0]);
            break;
        }
    }
    else
    {
        fmt::print(stderr, "Wrong number of arguments. Usage: {} [<test> <number> [<rounds>]]", argv[0]);
    }
}
