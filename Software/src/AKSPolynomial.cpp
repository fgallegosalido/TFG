#include "AKSPolynomial.hpp"

#include <algorithm>
#include <cassert>

namespace tfg::aks::detail
{

std::size_t AKSPolynomial::s_moduleDegree{1}; // NOLINT

auto AKSPolynomial::getModuleDegree() -> std::size_t
{
    return s_moduleDegree; // NOLINT
}

auto AKSPolynomial::setModuleDegree(std::size_t moduleDegree) -> void
{
    assert(moduleDegree > 0); // NOLINT
    s_moduleDegree = moduleDegree;
}

AKSPolynomial::AKSPolynomial(AKSCoefficient const& firstCoeff, AKSCoefficient const& secondCoeff) :
    m_coeffs{std::make_unique<AKSCoefficient[]>(AKSPolynomial::getModuleDegree() * 2 + 1)},
    m_coeffsAux{std::make_unique<AKSCoefficient[]>(AKSPolynomial::getModuleDegree() * 2 + 1)},
    m_currentDegree{(secondCoeff == 0_mpz) ? 0U : 1U}
{
    setCoefficient(0, firstCoeff);
    setCoefficient(1, secondCoeff);
    dividePolMod();
}

auto AKSPolynomial::getDegree() const -> std::size_t
{
    return m_currentDegree;
}

auto AKSPolynomial::getCoefficient(std::size_t index) const -> AKSCoefficient const &
{
    return m_coeffs[index];
}
auto AKSPolynomial::setCoefficient(std::size_t index, AKSCoefficient const &coefficient) -> void
{
    m_coeffs[index] = coefficient;
}

auto AKSPolynomial::operator-=(AKSCoefficient const& rhs) -> AKSPolynomial&
{
    m_coeffs[0] -= rhs;
    return *this;
}

auto AKSPolynomial::operator*=(AKSPolynomial const& rhs) -> AKSPolynomial&
{
    auto const newDegree = getDegree() + rhs.getDegree() + 1;

    for (auto i = std::size_t{0}; i <= newDegree; ++i)
    {
        m_coeffsAux[i] = 0_mpz;
    }

    for (auto i = std::size_t{0}; i <= getDegree(); ++i)
    {
        for (auto j = std::size_t{0}; j <= rhs.getDegree(); ++j)
        {
            auto product = getCoefficient(i);
            product *= rhs.getCoefficient(j);
            m_coeffsAux[i + j] += product;
        }
    }

    m_currentDegree = newDegree;
    std::swap(m_coeffs, m_coeffsAux);

    dividePolMod();
    
    return *this;
}

auto AKSPolynomial::pow(mpz_class const &exp, AKSPolynomial& result) const -> void
{
    result.setCoefficient(0, 1_mpz);
    result.m_currentDegree = 0;

    for (auto const& bit : exp.get_str(2))
    {
        result *= result;
        if (bit == '1')
        {
            result *= *this;
        }
    }
}

auto AKSPolynomial::adjustDegree() -> void
{
    while (getCoefficient(getDegree()) == 0_mpz && getDegree() > 0)
    {
        --m_currentDegree;
    }
}

auto AKSPolynomial::dividePolMod() -> void
{
    if (getDegree() >= AKSPolynomial::getModuleDegree())
    {
        for (auto i = getDegree(); i >= AKSPolynomial::getModuleDegree(); --i)
        {
            m_coeffs[i - AKSPolynomial::getModuleDegree()] += getCoefficient(i);
        }

        m_currentDegree = AKSPolynomial::getModuleDegree() - 1;
    }
    adjustDegree();
}

[[nodiscard]] auto operator==(AKSPolynomial const &lhs, AKSPolynomial const &rhs) -> bool
{
    if (lhs.getDegree() != rhs.getDegree())
    {
        return false;
    }

    for (auto i = std::size_t{0}; i <= lhs.getDegree(); ++i)
    {
        if (lhs.getCoefficient(i) != rhs.getCoefficient(i))
        {
            return false;
        }
    }

    return true;
}

} // namespace tfg::aks::detail
