#include "AKSCoefficient.hpp"

#include <cassert>
#include <utility>

namespace tfg::aks::detail
{

mpz_class AKSCoefficient::s_module{2}; // NOLINT

[[nodiscard]] auto AKSCoefficient::getModule() -> mpz_class const&
{
    return s_module;
}

auto AKSCoefficient::setModule(mpz_class const& module) -> void
{
    assert(module > 1); // NOLINT
    s_module = module;
}

AKSCoefficient::AKSCoefficient() :
    m_value{0}
{
}

AKSCoefficient::AKSCoefficient(mpz_class const& other) :
    m_value{other}
{
    m_value %= AKSCoefficient::getModule();
}

AKSCoefficient::AKSCoefficient(mpz_class&& other) :
    m_value{std::move(other)}
{
    m_value %= AKSCoefficient::getModule();
}

auto AKSCoefficient::operator+=(AKSCoefficient const& rhs) -> AKSCoefficient&
{
    m_value += rhs.m_value;
    m_value %= AKSCoefficient::getModule();
    return *this;
}

auto AKSCoefficient::operator-=(AKSCoefficient const& rhs) -> AKSCoefficient&
{
    m_value -= rhs.m_value;
    m_value += AKSCoefficient::getModule();
    m_value %= AKSCoefficient::getModule();
    return *this;
}

auto AKSCoefficient::operator*=(AKSCoefficient const& rhs) -> AKSCoefficient&
{
    m_value *= rhs.m_value;
    m_value %= AKSCoefficient::getModule();
    return *this;
}

AKSCoefficient::operator mpz_class() const
{
    return m_value;
}

} // namespace tfg::aks::detail
