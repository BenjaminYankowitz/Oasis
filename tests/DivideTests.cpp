//
// Created by Matthew McCall on 8/10/23.
//

#include "catch2/catch_test_macros.hpp"

#include "Oasis/Add.hpp"
#include "Oasis/Divide.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Imaginary.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
#include "Oasis/Variable.hpp"
#include "Oasis/Log.hpp"

TEST_CASE("Symbolic Change Of Base" "[Divide][Symbolic][Log]"){
    const Oasis::Variable x("x");
    const Oasis::Variable y("y");
    const Oasis::Variable z("z");
    const Oasis::Variable q("q");
    Oasis::Divide divG(Oasis::Log(z,x),Oasis::Log(z,y));
    Oasis::Divide divB(Oasis::Log(x,z),Oasis::Log(y,q));
    auto simplifiedG = divG.Simplify();
    auto simplifiedB = divB.Simplify();
    REQUIRE(simplifiedG != nullptr);
    REQUIRE(simplifiedB != nullptr);
    CAPTURE(simplifiedG->ToString());
    REQUIRE(simplifiedG->Equals(Oasis::Log(y,x)));
    REQUIRE(simplifiedB->Equals(divB));
}

// TEST_CASE("i over i Large Expression" "[Divide][Imaginary]"){
//     const Oasis::Variable x("x");
//     const Oasis::Variable y("y");
//     const Oasis::Variable z("z");
//     const Oasis::Variable q("q");
//     Oasis::Divide divG(Oasis::Log(x,z),Oasis::Log(y,z));
//     Oasis::Divide divB(Oasis::Log(x,z),Oasis::Log(y,q));
//     auto simplifiedG = divG.Simplify();
//     auto simplifiedB = divB.Simplify();
//     REQUIRE(simplifiedG != nullptr);
//     REQUIRE(simplifiedB != nullptr);
//     REQUIRE(simplifiedG->Equals(Oasis::Log(x,y)));
//     REQUIRE(simplifiedB->Equals(divB));
// }


TEST_CASE("Complex Division", "[Divide][Complex]")
{
    const double epsilon = std::pow(10, -5);
    Oasis::Divide div {
        Oasis::Real { 1.0 },
        Oasis::Add {
            Oasis::Real { 4.0 },
            Oasis::Multiply {
                Oasis::Real { 5.0 },
                Oasis::Imaginary() } }
    };

    auto simplified = div.Simplify();
    auto simplifiedComplex = Oasis::Add<Oasis::Real, Oasis::Multiply<Oasis::Real, Oasis::Imaginary>>::Specialize(*simplified);
    REQUIRE(simplifiedComplex != nullptr);
    REQUIRE(std::abs(simplifiedComplex->GetMostSigOp().GetValue() - 0.0975609756) < epsilon);
    REQUIRE(std::abs(simplifiedComplex->GetLeastSigOp().GetMostSigOp().GetValue() + 0.12195122) < epsilon);
}

TEST_CASE("Complex over imaginary Division", "[Divide][Complex]")
{
    const double epsilon = std::pow(10, -5);
    Oasis::Divide div {
        Oasis::Add {
            Oasis::Real { 1.0 },
            Oasis::Imaginary() },
        Oasis::Imaginary()
    };

    auto simplified = div.Simplify();
    auto simplifiedComplex = Oasis::Add<Oasis::Real, Oasis::Multiply<Oasis::Real, Oasis::Imaginary>>::Specialize(*simplified);
    REQUIRE(simplifiedComplex != nullptr);
    REQUIRE(std::abs(simplifiedComplex->GetMostSigOp().GetValue() - 1) < epsilon);
    REQUIRE(std::abs(simplifiedComplex->GetLeastSigOp().GetMostSigOp().GetValue() + 1) < epsilon);
}

TEST_CASE("Division", "[Divide]")
{
    Oasis::Divide subtract {
        Oasis::Divide {
            Oasis::Real { 2.0 },
            Oasis::Real { 1.0 } },
        Oasis::Real { 2.0 }
    };

    auto simplified = subtract.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 1.0);
}

TEST_CASE("Symbolic Division, equal variables", "[Division][Symbolic]")
{
    Oasis::Divide div {
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Variable { "x" } },
        Oasis::Multiply {
            Oasis::Real { 1.0 },
            Oasis::Variable { "x" } }
    };

    Oasis::Divide div2 {
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Multiply {
                Oasis::Variable { "z" },
                Oasis::Multiply {
                    Oasis::Variable { "y" },
                    Oasis::Variable { "x" } } } },
        Oasis::Multiply {
            Oasis::Real { 1.0 },
            Oasis::Multiply {
                Oasis::Variable { "y" },
                Oasis::Multiply {
                    Oasis::Variable { "x" },
                    Oasis::Variable { "z" } } } }
    };

    auto simplified = div.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());
    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 2.0);

    auto simplified2 = div2.Simplify();
    CAPTURE(simplified2->ToString());
    REQUIRE(simplified2->Is<Oasis::Real>());
    auto simplifiedReal2 = dynamic_cast<Oasis::Real&>(*simplified2);
    REQUIRE(simplifiedReal2.GetValue() == 2.0);
}

TEST_CASE("Symbolic Division, unequal variables", "[Division][Symbolic]")
{
    Oasis::Divide div {
        Oasis::Multiply {
            Oasis::Real { 4.0 },
            Oasis::Multiply {
                Oasis::Variable { "z" },
                Oasis::Variable { "x" } } },
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Multiply {
                Oasis::Variable { "y" },
                Oasis::Variable { "z" } } }
    };

    auto simplified = div.Simplify();

    CAPTURE(simplified->ToString());
    REQUIRE(Oasis::Divide {
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Variable { "x" } },
        Oasis::Variable { "y" } }
                .Equals(*simplified));
}

TEST_CASE("Symbolic Division of Expressions", "[Division][Symbolic]")
{
    // 4(z^2+1)(x+1)/2(x+1)=2(z^2+1)

    Oasis::Divide div {
        Oasis::Multiply {
            Oasis::Multiply {
                Oasis::Add {
                    Oasis::Variable { "x" },
                    Oasis::Real { 1 } },
                Oasis::Add {
                    Oasis::Exponent {
                        Oasis::Variable { "z" },
                        Oasis::Real { 2 } },
                    Oasis::Real { 1 } } },
            Oasis::Real { 4.0 } },
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Add {
                Oasis::Variable { "x" },
                Oasis::Real { 1 } } }
    };

    auto simplified = div.Simplify();

    CAPTURE(simplified->ToString());
    REQUIRE(Oasis::Multiply {
        Oasis::Real { 2 },
        Oasis::Add {
            Oasis::Exponent {
                Oasis::Variable { "z" },
                Oasis::Real { 2 } },
            Oasis::Real { 1 } },
    }
                .Equals(*simplified));
}

TEST_CASE("Symbolic Division, unequal exponents", "[Division][Symbolic]")
{
    Oasis::Divide div {
        Oasis::Multiply {
            Oasis::Real { 4.0 },
            Oasis::Exponent {
                Oasis::Variable { "z" },
                Oasis::Real { 2 } } },
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Multiply {
                Oasis::Variable { "y" },
                Oasis::Variable { "z" } } }
    };

    auto simplified = div.Simplify();

    CAPTURE(simplified->ToString());
    REQUIRE(Oasis::Divide {
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Variable { "z" } },
        Oasis::Variable { "y" } }
                .Equals(*simplified));
}

TEST_CASE("Symbolic Division, equal exponents", "[Division][Symbolic]")
{
    Oasis::Divide div {
        Oasis::Multiply {
            Oasis::Real { 4.0 },
            Oasis::Exponent {
                Oasis::Variable { "z" },
                Oasis::Real { 3 } } },
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Multiply {
                Oasis::Variable { "y" },
                Oasis::Variable { "z" } } }
    };

    auto simplified = div.Simplify();

    CAPTURE(simplified->ToString());
    REQUIRE(Oasis::Divide {
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Exponent {
                Oasis::Variable { "z" },
                Oasis::Real { 2 } } },
        Oasis::Variable { "y" } }
                .Equals(*simplified));
}

TEST_CASE("Generalized Division", "[Divide][Generalized]")
{
    Oasis::Divide<Oasis::Expression> subtract {
        Oasis::Divide<Oasis::Expression> {
            Oasis::Real { 2.0 },
            Oasis::Real { 1.0 } },
        Oasis::Real { 2.0 }
    };

    auto simplified = subtract.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 1.0);
}

TEST_CASE("Division Async", "[Divide][Async]")
{
    Oasis::Divide subtract {
        Oasis::Divide {
            Oasis::Real { 2.0 },
            Oasis::Real { 1.0 } },
        Oasis::Real { 2.0 }
    };

    std::unique_ptr<Oasis::Expression> simplified = subtract.SimplifyAsync();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 1.0);
}

TEST_CASE("Divide Operator Overload", "[Divide][Operator Overload]")
{
    const std::unique_ptr<Oasis::Expression> a = std::make_unique<Oasis::Real>(1.0);
    const std::unique_ptr<Oasis::Expression> b = std::make_unique<Oasis::Real>(2.0);

    const auto sum = b / a;
    auto realSum = Oasis::Real::Specialize(*sum);

    REQUIRE(realSum != nullptr);
    REQUIRE(realSum->GetValue() == 2.0);
}