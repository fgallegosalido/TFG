#include "AKS.hpp"

#include "AKSCoefficient.hpp"
#include "AKSPolynomial.hpp"
#include "GMPWrappers.hpp"

#include <mpfr.h>
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pEX.h>

#include <cassert>
#include <cmath>

namespace tfg::aks
{
namespace steps
{
namespace
{

auto isOrderBiggerThan(mpz_class const& n, std::size_t r, std::size_t threshold) -> bool
{
    auto temp = 1_mpz;

    for (auto i = std::size_t{1}; i <= threshold; ++i)
    {
        temp *= n;
        temp %= r;

        if (temp == 1)
        {
            return false;
        }
    }

    return true;
}

auto phi(std::size_t n) -> std::size_t
{
    auto const top = static_cast<std::size_t>(std::sqrt(n));
    auto result = n;

    for (auto p = std::size_t{2}; p <= top; ++p)
    {
        if (n % p == 0)
        {
            while (n % p == 0)
            {
                n /= p;
            }

            result -= result / p;
        }
    }

    if (n > 1)
    {
        result -= result / n; // NOLINT
    }

    return result;
}

auto calculateUpperBound(mpz_class const& n, std::size_t r) -> std::size_t
{
    // log_2(n)
    mpfr_t result;
    mpfr_init_set_z(result, n.get_mpz_t(), MPFR_RNDU); // NOLINT
    mpfr_log2(result, result, MPFR_RNDU);              // NOLINT
    
    // sqrt(phi(r))
    mpfr_t sqrtPhiR;
    mpfr_init_set_ui(sqrtPhiR, phi(r), MPFR_RNDU); // NOLINT
    mpfr_sqrt(sqrtPhiR, sqrtPhiR, MPFR_RNDU);      // NOLINT

    // log_2(n) * sqrt(phi(r))
    mpfr_mul(result, result, sqrtPhiR, MPFR_RNDU); // NOLINT

    // Return floor(log_2(n) * sqrt(phi(r)))
    return mpfr_get_ui(result, MPFR_RNDD); // NOLINT
}

} // namespace unnamed

auto step1(mpz_class const& n) -> bool
{
    return gmp::isPerfectPower(n);
}

auto step2(mpz_class const& n) -> std::size_t
{
    auto const threshold = [&n]
    {
        // Use MPFR to calculate a more accurate threshold using floating point
        // computations. We round towards +infinity for a more conservative
        // threshold.
        mpfr_t thresholdMPFR;
        mpfr_init_set_z(thresholdMPFR, n.get_mpz_t(), MPFR_RNDU); // NOLINT
        mpfr_log2(thresholdMPFR, thresholdMPFR, MPFR_RNDU);       // NOLINT
        mpfr_sqr(thresholdMPFR, thresholdMPFR, MPFR_RNDU);        // NOLINT

        // Return the result from MPFR as an unsigned long (this threshold is
        // unlikely to be bigger that 32-bits long, let alone 64-bits).
        // Round towards -infinity to get the floored value
        return mpfr_get_ui(thresholdMPFR, MPFR_RNDD); // NOLINT
    }();

    // r = log^2(n) + 2 is the first r such that ord_r(n) can be higher than
    // log^2(n).
    //
    // If r <= log^2(n) + 1, then ord(r) <= phi(r) < r - 1 <= log^2(n)
    for (auto r = threshold + 2;; ++r)
    {
        if (isOrderBiggerThan(n, r, threshold))
        {
            return r;
        }
    }
}

auto step3(mpz_class const& n, std::size_t r) -> bool
{
    for (auto a = std::size_t{2}; a <= r; ++a)
    {
        auto const result = gmp::gcd(a, n);

        if (1 < result && result < n)
        {
            return true;
        }
    }

    return false;
}

auto step4(mpz_class const& n, std::size_t r) -> bool
{
    return n <= r;
}

namespace impl
{

auto step5Direct(mpz_class const& n, std::size_t r) -> bool
{
    // First we calculate the upper bound of the loop
    auto const top = calculateUpperBound(n, r);

    // Prepare the environment to work mod(X^r - 1, n)
    detail::AKSCoefficient::setModule(n);
    detail::AKSPolynomial::setModuleDegree(r);

    // We create both polynomials outside the loop to avoid reallocations.
    auto lhs = detail::AKSPolynomial{};
    auto rhs = detail::AKSPolynomial{};

    // We are also going to create a temporary polynomial to avoid even more
    // reallocations.
    auto temp = detail::AKSPolynomial{0_mpz, 1_mpz};

    // X^n mod(X^r - 1, n) instead of X^n + a mod(X^r - 1, n) so it doesn't
    // depend on a, therefore calculating it only once.
    temp.pow(detail::AKSCoefficient::getModule(), rhs);

    for (auto a = std::size_t{1}; a <= top; ++a)
    {
        // (X + a)^n - a mod(X^r - 1, n)
        temp.setCoefficient(0, mpz_class{a});
        temp.pow(detail::AKSCoefficient::getModule(), lhs);
        lhs -= mpz_class{a};

        if (lhs != rhs)
        {
            return true;
        }
    }

    return false;
}

auto step5NTL(mpz_class const& n, std::size_t r) -> bool
{
    // First we calculate the upper bound of the loop
    auto const top = calculateUpperBound(n, r);
    // Convert GMP's integer to NTL's integer and set ring to Z_n
    auto const nNTL = NTL::conv<NTL::ZZ>(n.get_str().c_str());
    NTL::ZZ_p::init(nNTL);

    // Define the polynomial module X^r - 1
    auto const module = NTL::ZZ_pXModulus{NTL::ZZ_pX{static_cast<long>(r), 1} - 1};
    // Define X^n mod X^r - 1
    auto const rhs = [&nNTL, &module]
    {
        auto result = NTL::ZZ_pX{1, 1};
        NTL::PowerMod(result, result, nNTL, module);

        return result;
    }();

    for (auto a = std::size_t{1}; a <= top; ++a)
    {
        // (X + a)^n - a mod(X^r - 1, n)
        auto lhs = NTL::ZZ_pX{1, 1};
        lhs += static_cast<long>(a);
        NTL::PowerMod(lhs, lhs, nNTL, module);
        lhs -= static_cast<long>(a);

        if ((lhs != rhs) != 0)
        {
            return true;
        }
    }

    return false;
}

} // namespace impl

auto step5(mpz_class const& n, std::size_t r) -> bool
{
    return impl::step5NTL(n, r);
}

auto step6() -> bool
{
    return true;
}

} // namespace steps

auto isPrime(mpz_class const &n) -> bool
{
    // Step 1: Check if n is a perfect power
    if (steps::step1(n))
    {
        return false;
    }

    // Step 2: Find smallest r such that ord_r(n) > log^2(n)
    auto const r = steps::step2(n);

    // Step 3: Check if 1 < (a, n) < n for some a <= r
    if (steps::step3(n, r))
    {
        return false;
    }

    // Step 4: Check if n <= r
    if (steps::step4(n, r))
    {
        return true;
    }

    // Step 5: Check if the polynomial identities are not satisfied
    if (steps::step5(n, r))
    {
        return false;
    }

    // Step 6: If we reach this point, the number is prime.
    return steps::step6();
}

} // namespace tfg::aks