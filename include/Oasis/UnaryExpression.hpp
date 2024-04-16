//
// Created by Matthew McCall on 3/29/24.
//

#ifndef UNARYEXPRESSION_HPP
#define UNARYEXPRESSION_HPP

#include "Expression.hpp"

namespace Oasis {

template <template <IExpression> class DerivedT, IExpression OperandT>
class UnaryExpression : public Expression {

    using DerivedSpecialized = DerivedT<OperandT>;
    using DerivedGeneralized = DerivedT<Expression>;

public:
    UnaryExpression() = default;

    UnaryExpression(const UnaryExpression& other)
        : Expression(other)
    {
        if (other.HasOperand()) {
            SetOperand(other.GetOperand());
        }
    }

    explicit UnaryExpression(const OperandT& operand)
    {
        SetOperand(operand);
    }

    [[nodiscard]] auto Copy() const -> std::unique_ptr<Expression> final
    {
        if (op == nullptr) {
            return std::make_unique<DerivedSpecialized>();
        }
        return std::make_unique<DerivedSpecialized>(*op);
    }

    auto Copy(tf::Subflow&) const -> std::unique_ptr<Expression> final
    {
        return std::make_unique<DerivedSpecialized>(*op);
    }

    [[nodiscard]] auto Equals(const Expression& other) const -> bool final
    {
        if (!other.Is<DerivedGeneralized>()) {
            return false;
        }

        // generalize
        const auto otherGeneralized = other.Generalize();
        const auto& otherUnaryGeneralized = dynamic_cast<const DerivedGeneralized&>(*otherGeneralized);
        if (this->HasOperand() != otherUnaryGeneralized.HasOperand()) {
            return false;
        }
        if (!this->HasOperand()) {
            return true;
        }
        return op->Equals(otherUnaryGeneralized.GetOperand());
    }

    [[nodiscard]] auto Generalize() const -> std::unique_ptr<Expression> final
    {
        if (op == nullptr) {
            return std::make_unique<DerivedGeneralized>();
        }
        return std::make_unique<DerivedGeneralized>(*op);
    }

    auto Generalize(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final
    {
        return DerivedGeneralized { *this }.Copy(subflow);
    }

    auto GetOperand() const -> const OperandT&
    {
        return *op;
    }

    auto HasOperand() const -> bool
    {
        return op != nullptr;
    }

    [[nodiscard]] auto StructurallyEquivalent(const Expression& other) const -> bool final
    {
        return this->GetType() == other.GetType();
    }

    auto StructurallyEquivalent(const Expression& other, tf::Subflow&) const -> bool final
    {
        return this->GetType() == other.GetType();
    }

    [[nodiscard]] virtual auto SubstituteVariable(const Expression& var, const Expression& exp) const -> std::unique_ptr<Expression> override
    {
        DerivedGeneralized generalized;

        if (op) {
            generalized.SetOperand(*op->SubstituteVariable(var, exp));
        }
        return std::make_unique<DerivedGeneralized>(generalized);
    }

    auto SetOperand(const OperandT& operand) -> void
    {
        if constexpr (std::same_as<OperandT, Expression>) {
            this->op = operand.Copy();
        } else {
            this->op = std::make_unique<OperandT>(operand);
        }
    }

protected:
    std::unique_ptr<OperandT> op;
};

#define IMPL_SPECIALIZE_UNARYEXPR(Derived, Operand)                                            \
    static auto Specialize(const Expression& other) -> std::unique_ptr<Derived<Operand>>       \
    {                                                                                          \
        if (!other.Is<Oasis::Derived>()) {                                                     \
            return nullptr;                                                                    \
        }                                                                                      \
                                                                                               \
        auto specialized = std::make_unique<Derived<Operand>>();                               \
        std::unique_ptr<Expression> otherGeneralized = other.Generalize();                     \
        const auto& otherUnary = dynamic_cast<const Derived<Expression>&>(*otherGeneralized);  \
                                                                                               \
        if (auto operand = Operand::Specialize(otherUnary.GetOperand()); operand != nullptr) { \
            specialized->op = std::move(operand);                                              \
            return specialized;                                                                \
        }                                                                                      \
                                                                                               \
        return nullptr;                                                                        \
    }                                                                                          \
                                                                                               \
    static auto Specialize(const Expression& other, tf::Subflow&) -> std::unique_ptr<Derived>  \
    {                                                                                          \
        /* TODO: Actually implement */                                                         \
        return Derived<Operand>::Specialize(other);                                            \
    }

} // Oasis

#endif // UNARYEXPRESSION_HPP
