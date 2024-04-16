//
// Created by Matthew McCall on 8/10/23.
//

#include "catch2/catch_test_macros.hpp"

#include "Oasis/Imaginary.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
#include "Oasis/Subtract.hpp"
#include "Oasis/Variable.hpp"
#include <memory>

TEST_CASE("Subtraction", "[Subtract]")
{
    Oasis::Subtract subtract {
        Oasis::Subtract {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = subtract.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == -4.0);
}

TEST_CASE("Subtraction Print", "[Subtract][Pinrt]")
{
    Oasis::Subtract<Oasis::Expression> sub1(Oasis::Variable("x"), Oasis::Variable("y"));
    Oasis::Subtract<Oasis::Variable> sub2(Oasis::Variable("x"), Oasis::Variable("y"));
    std::string expected = "(x - y)";
    REQUIRE(sub1.ToString() == expected);
    REQUIRE(sub2.ToString() == expected);
    REQUIRE(sub1.ToString() == sub2.ToString());
}

TEST_CASE("Subtraction Specialization", "[Subtract][Specialization]")
{
    std::unique_ptr<Oasis::Expression> nSub = std::make_unique<Oasis::Real>(1.0);
    std::unique_ptr<Oasis::Expression> sub1 = std::make_unique<Oasis::Subtract<Oasis::Expression>>(Oasis::Real(1.0), Oasis::Real(2.0));
    std::unique_ptr<Oasis::Expression> sub2 = std::make_unique<Oasis::Subtract<Oasis::Expression>>(Oasis::Variable("x"), Oasis::Real(2.0));
    std::unique_ptr<Oasis::Expression> sub3 = std::make_unique<Oasis::Subtract<Oasis::Expression>>(Oasis::Real(1.0), Oasis::Variable("x"));
    REQUIRE(Oasis::Subtract<Oasis::Expression>::Specialize(*nSub) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real>::Specialize(*nSub) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real, Oasis::Variable>::Specialize(*nSub) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Real>::Specialize(*nSub) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Variable>::Specialize(*nSub) == nullptr);

    REQUIRE(Oasis::Subtract<Oasis::Expression>::Specialize(*sub1) != nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real>::Specialize(*sub1) != nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real, Oasis::Variable>::Specialize(*sub1) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Real>::Specialize(*sub1) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable>::Specialize(*sub1) == nullptr);

    REQUIRE(Oasis::Subtract<Oasis::Expression>::Specialize(*sub2) != nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real>::Specialize(*sub2) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real, Oasis::Variable>::Specialize(*sub2) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Real>::Specialize(*sub2) != nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Variable>::Specialize(*sub2) == nullptr);

    REQUIRE(Oasis::Subtract<Oasis::Expression>::Specialize(*sub3) != nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real>::Specialize(*sub3) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Real, Oasis::Variable>::Specialize(*sub3) != nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Real>::Specialize(*sub3) == nullptr);
    REQUIRE(Oasis::Subtract<Oasis::Variable, Oasis::Variable>::Specialize(*sub3) == nullptr);
}

TEST_CASE("Generalized Subtraction", "[Subtract][Generalized]")
{
    Oasis::Subtract<Oasis::Expression> subtract {
        Oasis::Subtract<Oasis::Expression> {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = subtract.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == -4.0);
}

TEST_CASE("Subtraction Async", "[Subtract][Async]")
{
    Oasis::Subtract subtract {
        Oasis::Subtract {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    std::unique_ptr<Oasis::Expression> simplified = subtract.SimplifyAsync();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == -4.0);
}

TEST_CASE("Imaginary Subtration", "[Subtract][Imaginary]")
{
    Oasis::Subtract s1 {
        Oasis::Multiply {
            Oasis::Real { 3.0 },
            Oasis::Imaginary {} },
        Oasis::Imaginary {}
    };

    auto spec1 = s1.Simplify();

    REQUIRE(Oasis::Multiply { Oasis::Real { 2.0 }, Oasis::Imaginary {} }.Equals(*spec1));
}

TEST_CASE("Subtract Operator Overload", "[Subtract][Operator Overload]")
{
    const std::unique_ptr<Oasis::Expression> a = std::make_unique<Oasis::Real>(1.0);
    const std::unique_ptr<Oasis::Expression> b = std::make_unique<Oasis::Real>(2.0);

    const auto sum = a - b;
    auto realSum = Oasis::Real::Specialize(*sum);

    REQUIRE(realSum != nullptr);
    REQUIRE(realSum->GetValue() == -1.0);
}