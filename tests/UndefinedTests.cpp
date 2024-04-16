//
//
//
#include "catch2/catch_test_macros.hpp"

#include "Oasis/Real.hpp"
#include "Oasis/Undefined.hpp"

TEST_CASE("Undefined Equals", "[Equals][Copy Constructor][assignment operator]")
{
    Oasis::Real rVal(1.0);
    Oasis::Undefined u0;
    Oasis::Undefined u1;
    u1 = u0;
    Oasis::Undefined u2(u1);
    REQUIRE(!u1.Equals(rVal));
    REQUIRE(!u1.Equals(u2));
}

TEST_CASE("Undefined ToString", "[toString]")
{
    Oasis::Undefined u1;
    REQUIRE(u1.ToString() == "Undefined");
}

TEST_CASE("Undefined Specailize", "[Specailize]")
{
    Oasis::Real rVal(1.0);
    Oasis::Undefined u1;
    Oasis::Expression& definedValue = rVal;
    Oasis::Expression& undefinedValue = u1;
    REQUIRE(Oasis::Undefined::Specialize(definedValue) == nullptr);
    REQUIRE(Oasis::Undefined::Specialize(undefinedValue) != nullptr);
}

TEST_CASE("Undefined GetType and GetCategory", "[ExpressionType][ExpressionCategory]")
{
    Oasis::Undefined u1;
    REQUIRE(u1.GetType() == Oasis::ExpressionType::None);
    REQUIRE(Oasis::Undefined::GetStaticType() == Oasis::ExpressionType::None);
    REQUIRE(u1.GetCategory() == Oasis::ExpressionCategory::UnExp);
    REQUIRE(Oasis::Undefined::GetStaticCategory() == Oasis::ExpressionCategory::UnExp);
}