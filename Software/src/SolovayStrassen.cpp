#include "SolovayStrassen.hpp"

#include "GMPWrappers.hpp"

#include <cassert>
#include <memory>
#include <random>
#include <utility>

namespace tfg::solovay_strassen
{

[[nodiscard]] auto isProbablyPrime(mpz_class const &n, mpz_class const &k, gmp_randclass &prng) -> bool
{
    if (n == 2)
    {
        return true;
    }

    if (n < 2 || n % 2 == 0)
    {
        return false;
    }

    for (auto i = 0_mpz; i < k; ++i)
    {
        // Get random number in the range [2, n-1]
        auto const a = mpz_class{prng.get_z_range(n - 2) + 2};
        auto const x = [&a, &n]
        {
            auto result = gmp::jacobiSymbol(a, n);
            return (result < 0) ? n + result : result;
        }();

        if (x == 0 || gmp::powMod(a, (n - 1)/2, n) != x)
        {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto isProbablyPrime(mpz_class const &n, mpz_class const &k) -> bool
{
    // We use a unique_ptr to surpass the copy constructor being private
    static auto prng = []{
        auto result = std::make_unique<gmp_randclass>(gmp_randinit_mt);
        result->seed(std::random_device{}());

        return result;
    }();

    return tfg::solovay_strassen::isProbablyPrime(n, k, *prng);
}

} // namespace tfg::solovay_strassen
