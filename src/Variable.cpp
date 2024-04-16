//
// Created by Matthew McCall on 8/15/23.
//

#include "Oasis/Variable.hpp"
#include "Oasis/Add.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"

namespace Oasis {

Variable::Variable(std::string name)
    : name(std::move(name))
{
}

auto Variable::Equals(const Expression& other) const -> bool
{
    return other.Is<Variable>() && name == dynamic_cast<const Variable&>(other).name;
}

auto Variable::GetName() const -> std::string
{
    return name;
}

auto Variable::ToString() const -> std::string
{
    return name;
}

auto Variable::Specialize(const Expression& other) -> std::unique_ptr<Variable>
{
    return other.Is<Variable>() ? std::make_unique<Variable>(dynamic_cast<const Variable&>(other)) : nullptr;
}

auto Variable::Specialize(const Expression& other, tf::Subflow&) -> std::unique_ptr<Variable>
{
    return other.Is<Variable>() ? std::make_unique<Variable>(dynamic_cast<const Variable&>(other)) : nullptr;
}

auto Variable::Substitute(const Expression& var, const Expression& val) const -> std::unique_ptr<Expression>
{
    assert(var.Is<Variable>());
    if (Equals(var))
        return val.Copy();
    return Copy();
}

auto Variable::Differentiate(const Expression& differentiationVariable) -> std::unique_ptr<Expression>
{
    if (auto variable = Variable::Specialize(differentiationVariable); variable != nullptr) {

        // Power rule
        if (name == (*variable).GetName()) {
            return std::make_unique<Real>(Real { 1.0f })
                ->Simplify();
        }

        // Different variable, treat as constant
        return std::make_unique<Real>(Real { 0 })
            ->Simplify();
    }

    return Copy();
}

} // Oasis