#pragma once

#include <gmpxx.h>

#include <cstddef>

namespace tfg::gmp
{

/**
 * @brief Calculates a^k mod(r).
 * @param a Base
 * @param k Exponent
 * @param r Module
 * @return a raised to the power of k modulo r.
 */
auto powMod(mpz_class const& a, mpz_class const& k, mpz_class const& r) -> mpz_class;

/**
 * @brief This function checks if n == a^b for some a, b > 1.
 * @param n Number to check
 * @return True if n == a^b for some a,b > 1. Otherwise false
 * 
 * Complexity: O(log^3(n))
 */
auto isPerfectPower(mpz_class const& n) -> bool;

/**
 * @brief Calculates gcd(a, b)
 * @param a First number
 * @param b Second number
 * @return Greatest Common Divisor between a and b
 */
auto gcd(mpz_class const& a, std::size_t b) -> std::size_t;
auto gcd(std::size_t a, mpz_class const& b) -> std::size_t;

/**
 * @brief Calculates the Jacobi symbol of a and b
 * @param a First number
 * @param b Second number. Must be odd
 * @return The Jacobi symbol (a/b)
 */
[[nodiscard]] auto jacobiSymbol(mpz_class const& n, mpz_class const& k) -> mpz_class;

} // namespace tfg::gmp