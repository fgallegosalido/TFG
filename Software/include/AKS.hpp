#pragma once

#include <gmpxx.h>

#include <cstddef>

namespace tfg::aks
{

namespace steps
{

/**
 * @brief Step 1 of the AKS test: check for perfect power
 * @param n Number to test for primality
 * @return True if n is a perfect power. False otherwise
 */
auto step1(mpz_class const& n) -> bool;

/**
 * @brief Step 2 of the AKS test: find lowest r such that ord_r(n) > log^2(n)
 * @param n Number to test for primality
 * @return Lowest r such that ord_r(n) > log^2(n)
 */
auto step2(mpz_class const& n) -> std::size_t;

/**
 * @brief Step 3 of the AKS test: Check if 1 < (a, n) < n for some a <= r
 * @param n Number to test for primality
 * @param r Number found in step 2
 * @return True if 1 < (a, n) < n for some a <= r. False otherwise
 */
auto step3(mpz_class const& n, std::size_t r) -> bool;

/**
 * @brief Step 4 of the AKS test: check if n <= r
 * @param n Number to test for primality
 * @param r Number found in step 2
 * @return True if n <= r. False otherwise
 */
auto step4(mpz_class const& n, std::size_t r) -> bool;

namespace impl
{

/**
 * @brief Step 5 of the AKS test without external libraries
 * @param n Number to test for primality
 * @param r Number found in step 2
 * @return False if all polynomial identities are satisfied. True otherwise
 */
auto step5Direct(mpz_class const &n, std::size_t r) -> bool;

/**
 * @brief Step 5 of the AKS test using the NTL library
 * @param n Number to test for primality
 * @param r Number found in step 2
 * @return False if all polynomial identities are satisfied. True otherwise
 */
auto step5NTL(mpz_class const &n, std::size_t r) -> bool;

} // namespace impl

/**
 * @brief Step 5 of the AKS test: check polynomial identities
 * @param n Number to test for primality
 * @param r Number found in step 2
 * @return False if all polynomial identities are satisfied. True otherwise
 */
auto step5(mpz_class const &n, std::size_t r) -> bool;

/**
 * @brief Step 6 of the AKS test: test passed
 * @return True always
 */
auto step6() -> bool;

} // namespace steps

/**
 * @brief This function checks if n is prime using the AKS algorithm
 * @param n Number to check
 * @return True if n is prime. False otherwise
 * 
 * Complexity O~(log(n)^21/2)
 */
auto isPrime(mpz_class const &n) -> bool;

} // namespace tfg::aks
