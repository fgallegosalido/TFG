#pragma once

#include <gmpxx.h>

namespace tfg::miller_rabin
{

[[nodiscard]]

/**
 * @brief This function checks if n is probably prime using the Miller-Rabin
 * algorithm with k rounds and with the specified PRNG
 * @param n Number to check for probably primality
 * @param k Number of rounds
 * @param prng Seed for the underlyined PRNG
 * @return True if n is probably prime. False otherwise
 * 
 * Complexity O(k*log(n)^3)
 */
[[nodiscard]] auto isProbablyPrime(mpz_class const &n, mpz_class const &k, gmp_randclass &prng) -> bool;

/**
 * @brief This function checks if n is probably prime using the Miller-Rabin
 * algorithm with k rounds and with a random seeded Mersenne-Twister PRNG
 * @param n Number to check
 * @param k Number of rounds
 * @return True if n is probably prime. False otherwise
 * 
 * Complexity O(k*log(n)^3)
 */
[[nodiscard]] auto isProbablyPrime(mpz_class const &n, mpz_class const &k) -> bool;

} // namespace tfg::miller_rabin
