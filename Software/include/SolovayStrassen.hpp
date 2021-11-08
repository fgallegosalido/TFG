#pragma once

#include <gmpxx.h>

namespace tfg::solovay_strassen
{

/**
 * @brief This function checks if n is probably prime using the
 * Solovay-Strassen algorith with k rounds and with the specified PRNG
 * @param n Number to check for primality or probably composibility
 * @param k Number of rounds
 * @param prng Seed for the underlyined PRNG
 * @return True if n is probably prime. False otherwise
 * 
 * Complexity O(k*log(n)^3)
 */
[[nodiscard]] auto isProbablyPrime(mpz_class const &n, mpz_class const &k, gmp_randclass &prng) -> bool;

/**
 * @brief This function checks if n is prime or probably composite using the
 * Lucas primality test with k rounds and with a random seeded Mersenne-Twister
 * PRNG
 * @param n Number to check
 * @param k Number of rounds
 * @return False if n is probably composite. True otherwise
 * 
 * Complexity O(k*log(n)^3)
 */
[[nodiscard]] auto isProbablyPrime(mpz_class const &n, mpz_class const &k) -> bool;

} // namespace tfg::solovay_strassen
