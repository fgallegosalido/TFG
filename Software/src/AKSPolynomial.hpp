#pragma once

#include "AKSCoefficient.hpp"

#include <gmpxx.h>

#include <memory>
#include <utility>

namespace tfg::aks::detail
{

class AKSPolynomial
{
public:
    [[nodiscard]] static auto getModuleDegree() -> std::size_t;
    static auto setModuleDegree(std::size_t moduleDegree) -> void;

    explicit AKSPolynomial(
            AKSCoefficient const& firstCoeff = 0_mpz,
            AKSCoefficient const& secondCoeff = 0_mpz);
    
    AKSPolynomial(AKSPolynomial const& other) = delete;
    AKSPolynomial(AKSPolynomial&& other) = default;
    auto operator=(AKSPolynomial const& other) -> AKSPolynomial& = delete;
    auto operator=(AKSPolynomial&& other) -> AKSPolynomial& = default;
    ~AKSPolynomial() = default;
    
    [[nodiscard]] auto getDegree() const -> std::size_t;

    [[nodiscard]] auto getCoefficient(std::size_t index) const -> AKSCoefficient const&;
    auto setCoefficient(std::size_t index, AKSCoefficient const& coefficient) -> void;

    auto operator-=(AKSCoefficient const& rhs) -> AKSPolynomial&;
    auto operator*=(AKSPolynomial const& rhs) -> AKSPolynomial&;
    
    auto pow(mpz_class const& exp, AKSPolynomial& result) const -> void;

    friend auto operator==(AKSPolynomial const& lhs, AKSPolynomial const& rhs) -> bool;

private:
    auto adjustDegree() -> void;
    auto dividePolMod() -> void;

    static std::size_t s_moduleDegree; // NOLINT

    std::unique_ptr<AKSCoefficient[]> m_coeffs;
    std::unique_ptr<AKSCoefficient[]> m_coeffsAux;
    std::size_t                       m_currentDegree;
};

} // namespace tfg::aks::detail
