#pragma once

#include <gmpxx.h>

namespace tfg::basic
{

/**
 * @brief This function checks if n is prime using the basic algorithm
 * @param n Number to check
 * @return True if n is prime. False otherwise
 * 
 * Complexity O(sqrt(n))
 */
auto isPrime(mpz_class const &n) -> bool;

} // namespace tfg::basic
