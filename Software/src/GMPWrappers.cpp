#include "GMPWrappers.hpp"

namespace tfg::gmp
{

auto pow(mpz_class const& n, size_t exp) -> mpz_class
{
    mpz_class result;
    mpz_pow_ui(result.get_mpz_t(), n.get_mpz_t(), exp);

    return result;
}

auto powMod(mpz_class const& a, mpz_class const& k, mpz_class const& r) -> mpz_class
{
    mpz_class result;
    mpz_powm(result.get_mpz_t(), a.get_mpz_t(), k.get_mpz_t(), r.get_mpz_t());

    return result;
}

auto isPerfectPower(mpz_class const& n) -> bool
{
    return mpz_perfect_power_p(n.get_mpz_t()) != 0;
}

auto gcd(mpz_class const& a, std::size_t b) -> std::size_t
{
    return mpz_gcd_ui(nullptr, a.get_mpz_t(), b);
}
auto gcd(std::size_t a, mpz_class const& b) -> std::size_t
{
    return mpz_gcd_ui(nullptr, b.get_mpz_t(), a);
}

[[nodiscard]] auto jacobiSymbol(mpz_class const& n, mpz_class const& k) -> mpz_class
{
    return mpz_jacobi(n.get_mpz_t(), k.get_mpz_t());
}

} // namespace tfg::gmp