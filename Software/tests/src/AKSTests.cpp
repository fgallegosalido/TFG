#include <AKS.hpp>

#include <catch2/catch.hpp>

#include <array>

TEST_CASE("Perfect Power function")
{
    SECTION("Primes")
    {
        std::array testCases{
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
            67, 71, 73, 79, 83, 89, 97, 5519
        };

        for (auto const &n : testCases)
        {
            REQUIRE(tfg::aks::isPrime(n));
        }
    }
}
