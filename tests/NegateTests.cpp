//
// Created by Matthew McCall on 4/5/24.
//

#include "catch2/catch_test_macros.hpp"

#include "Oasis/Real.hpp"
#include "Oasis/Variable.hpp"
#include <Oasis/Negate.hpp>

TEST_CASE("Negate", "[Negate]")
{
    const Oasis::Negate negativeOne {
        Oasis::Real { 1.0 }
    };

    const auto simplified = negativeOne.Simplify();

    const Oasis::Real expected { -1.0 };
    REQUIRE(simplified->Equals(expected));
}
TEST_CASE("Negate Is", "[Negate], [Is]")
{
    Oasis::Negate u0(Oasis::Real(1.0));
    REQUIRE(u0.Is<Oasis::Negate>());
}

TEST_CASE("Negate Equals", "[Equals][Copy Constructor][assignment operator]")
{
    Oasis::Real rVal(1.0);
    Oasis::Negate u0(rVal);
    Oasis::Negate u1(rVal);
    Oasis::Negate u2(Oasis::Real(2.0));
    Oasis::Negate<Oasis::Real> u3(u1);
    REQUIRE(!u1.Equals(rVal));
    REQUIRE(!u1.Equals(u2));
    REQUIRE(u1.Equals(u0));
    REQUIRE(u1.Equals(u3));
}

TEST_CASE("Negate ToString", "[toString]")
{
    Oasis::Negate u1(Oasis::Variable("x"));
    REQUIRE(u1.ToString() == "-(x)");
}

TEST_CASE("Negate Specailize", "[Specailize]")
{
    Oasis::Real rVal(1.0);
    Oasis::Negate<Oasis::Real> u1(rVal);
    Oasis::Expression& realValue = rVal;
    Oasis::Expression& negateValue = u1;
    REQUIRE(Oasis::Negate<Oasis::Expression>::Specialize(realValue) == nullptr);
    REQUIRE(Oasis::Negate<Oasis::Expression>::Specialize(negateValue) != nullptr);
    REQUIRE(Oasis::Negate<Oasis::Real>::Specialize(realValue) == nullptr);
    REQUIRE(Oasis::Negate<Oasis::Real>::Specialize(negateValue) != nullptr);
    REQUIRE(Oasis::Negate<Oasis::Variable>::Specialize(negateValue) == nullptr);
}

TEST_CASE("Negate GetType and GetCategory", "[ExpressionType][ExpressionCategory]")
{
    Oasis::Negate u1(Oasis::Real(1.2));
    REQUIRE(u1.GetType() == Oasis::ExpressionType::Negate);
    REQUIRE(Oasis::Negate<Oasis::Expression>::GetStaticType() == Oasis::ExpressionType::Negate);
    REQUIRE(Oasis::Negate<Oasis::Real>::GetStaticType() == Oasis::ExpressionType::Negate);
    REQUIRE(u1.GetCategory() == Oasis::ExpressionCategory::UnExp);
    REQUIRE(Oasis::Negate<Oasis::Expression>::GetStaticCategory() == Oasis::ExpressionCategory::UnExp);
    REQUIRE(Oasis::Negate<Oasis::Real>::GetStaticCategory() == Oasis::ExpressionCategory::UnExp);
}

TEST_CASE("Negate Substitute Variable", "[Negate][Variable]")
{
    const Oasis::Variable x("x");
    const Oasis::Real rVal(2.0);
    Oasis::Negate u1(x);
    auto subbed = u1.Substitute(x, rVal);
    REQUIRE(subbed != nullptr);
    REQUIRE(subbed->Equals(Oasis::Negate(Oasis::Real(2.0))));
}