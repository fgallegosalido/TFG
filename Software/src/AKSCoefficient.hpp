#pragma once

#include <gmpxx.h>

namespace tfg::aks::detail
{

class AKSCoefficient
{
public:
    [[nodiscard]] static auto getModule() -> mpz_class const&;
    static auto setModule(mpz_class const &module) -> void;

    AKSCoefficient();
    AKSCoefficient(mpz_class const& other); // NOLINT
    AKSCoefficient(mpz_class&& other); // NOLINT

    auto operator+=(AKSCoefficient const& rhs) -> AKSCoefficient&;
    auto operator-=(AKSCoefficient const& rhs) -> AKSCoefficient&;
    auto operator*=(AKSCoefficient const& rhs) -> AKSCoefficient&;

    [[nodiscard]] explicit operator mpz_class() const;

    [[nodiscard]] friend auto operator==(AKSCoefficient const& lhs, AKSCoefficient const& rhs) -> bool = default;

private:
    static mpz_class s_module; // NOLINT

    mpz_class m_value;
};

} // namespace tfg::aks::detail
