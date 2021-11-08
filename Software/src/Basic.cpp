#include "Basic.hpp"

namespace tfg::basic
{

auto isPrime(mpz_class const &n) -> bool
{
    auto const top = sqrt(n);

    for (mpz_class i = 2; i <= top; ++i)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}

} // namespace tfg::basic
