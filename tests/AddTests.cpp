//
// Created by Matthew McCall on 8/7/23.
//
#include "catch2/catch_test_macros.hpp"

#include "Oasis/Add.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Imaginary.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
#include "Oasis/Variable.hpp"

#include "Oasis/Log.hpp"
#include "Oasis/Util.hpp"
#include <functional>

TEST_CASE("Addition", "[Add]")
{
    Oasis::Add add {
        Oasis::Add {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = add.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Symbolic Addition", "[Add][Symbolic]")
{
    Oasis::Add add {
        Oasis::Multiply {
            Oasis::Real { 1.0 },
            Oasis::Variable { "x" } },
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Variable { "x" } }
    };

    auto simplified = add.Simplify();
    REQUIRE(simplified->Is<Oasis::Multiply>());

    REQUIRE(Oasis::Multiply {
        Oasis::Real { 3.0 },
        Oasis::Variable { "x" } }
                .Equals(*simplified));
}

TEST_CASE("Log Addition", "[Add][Log]")
{
    Oasis::Add addS(Oasis::Log(Oasis::Variable("x"), Oasis::Variable("y")), Oasis::Log(Oasis::Variable("x"), Oasis::Variable("z")));
    Oasis::Add addF(Oasis::Log(Oasis::Variable("y"), Oasis::Variable("x")), Oasis::Log(Oasis::Variable("z"), Oasis::Variable("x")));
    auto addSS = addS.Simplify();
    auto addFS = addF.Simplify();
    REQUIRE(addSS != nullptr);
    auto addSSType = Oasis::Log<Oasis::Variable, Oasis::Multiply<Oasis::Variable>>::Specialize(*addSS);
    REQUIRE(addSSType != nullptr);
    Oasis::Log goalS(Oasis::Variable("x"), Oasis::Multiply(Oasis::Variable("y"), Oasis::Variable("z")));
    REQUIRE(goalS.Equals(*addSSType));
    REQUIRE(addFS != nullptr);
    REQUIRE(!goalS.Equals(*addFS));
}

TEST_CASE("Complex Sum", "[Add][Complex]"){
    Oasis::Add<> addS{Oasis::Util::pairToComp(2,4),Oasis::Util::pairToComp(-1,5),Oasis::Util::pairToComp(9,2),Oasis::Util::pairToComp(3,2)};
    auto Simp = addS.Simplify();
    auto cast = Oasis::Add<Oasis::Real,Oasis::Multiply<Oasis::Real,Oasis::Imaginary>>::Specialize(*Simp);
    REQUIRE(cast!=nullptr);
    REQUIRE(cast->GetMostSigOp().GetValue()==13);
    REQUIRE(cast->GetLeastSigOp().GetMostSigOp().GetValue()==13);
}

TEST_CASE("Imaginary Add Large Expression", "[Add][Imaginary]")
{
    Oasis::Add add(Oasis::Add(Oasis::Imaginary(), Oasis::Real(4)), Oasis::Add(Oasis::Variable("z"), Oasis::Imaginary()));
    auto addSimp = add.Simplify();
    REQUIRE(addSimp != nullptr);
    Oasis::Add goal(Oasis::Multiply(Oasis::Imaginary(), Oasis::Real(2)), Oasis::Add(Oasis::Variable("z"), Oasis::Real(4)));
    REQUIRE(addSimp->Equals(goal));
}

TEST_CASE("Addition Of Like Terms Doesn't Multiply By 1", "[Add][Like Terms]")
{
    Oasis::Add add(Oasis::Multiply(Oasis::Imaginary(), Oasis::Real(0.5)), Oasis::Multiply(Oasis::Imaginary(), Oasis::Real(0.5)));
    auto addSimp = add.Simplify();
    REQUIRE(addSimp != nullptr);
    CAPTURE(addSimp->ToString());
    REQUIRE(addSimp->Is<Oasis::Imaginary>());
}

TEST_CASE("Symbolic Add Large Expression", "[Add][Symbolic]")
{
    Oasis::Add add(Oasis::Add(Oasis::Variable("y"), Oasis::Variable("y")), Oasis::Add(Oasis::Variable("x"), Oasis::Variable("y")));
    auto addSimp = add.Simplify();
    REQUIRE(addSimp != nullptr);
    Oasis::Add goal(Oasis::Variable("x"), Oasis::Multiply(Oasis::Variable("y"), Oasis::Real(3.0)));
    CAPTURE(addSimp->ToString());
    REQUIRE(addSimp->Equals(goal));
}

TEST_CASE("Symbolic Add Different Variables", "[Add][Symbolic]")
{
    const Oasis::Variable x("x");
    const Oasis::Variable y("y");
    const Oasis::Variable z("z");
    const Oasis::Variable q("q");
    Oasis::Add add(Oasis::Multiply(Oasis::Exponent(z, z), Oasis::Variable("y")), Oasis::Add(Oasis::Exponent(q, q), Oasis::Variable("x")));
    auto addSimp = add.Simplify();
    REQUIRE(addSimp != nullptr);
    CAPTURE(addSimp->ToString());
    REQUIRE(addSimp->Equals(add));
}

TEST_CASE("Symbolic Add Different Exponents", "[Add][Symbolic]")
{
    const Oasis::Variable x("x");
    const Oasis::Variable y("y");
    const Oasis::Variable z("z");
    Oasis::Add add {
        Oasis::Add {
            Oasis::Multiply(Oasis::Real(3.0), Oasis::Exponent(z, z)),
            Oasis::Exponent(y, y) },
        Oasis::Add {
            Oasis::Multiply(Oasis::Real(7.0), Oasis::Exponent(z, z)),
            Oasis::Multiply(Oasis::Real(8.0), Oasis::Exponent(x, x)),
        }
    };
    auto addSimp = add.Simplify();
    REQUIRE(addSimp != nullptr);
    CAPTURE(addSimp->ToString());
    Oasis::Add goal {
        Oasis::Add {
            Oasis::Multiply(Oasis::Real(10.0), Oasis::Exponent(z, z)),
            Oasis::Exponent(y, y) },
        Oasis::Multiply(Oasis::Real(8.0), Oasis::Exponent(x, x))
    };
    REQUIRE(addSimp->Equals(goal));
}

TEST_CASE("Adding x+ax Gives x For Very Small a", "[Add][Symbolic]")
{
    const Oasis::Variable x("x");
    Oasis::Add add(x, Oasis::Multiply(x, Oasis::Real(std::pow(2.0, -60.0))));
    auto addSimp = add.Simplify();
    REQUIRE(addSimp != nullptr);
    CAPTURE(addSimp->ToString());
    REQUIRE(addSimp->Equals(x));
}

/*
TEST_CASE("Symbolic Addition, variable case", "[Add][Symbolic]")
{
    Oasis::Add add {
        Oasis::Add {
            Oasis::Real { 1.0 },
            Oasis::Variable { "x" } },
        Oasis::Multiply {
                Oasis::Real { 2.0 },
                Oasis::Variable { "x" } }
        };

    auto simplified = add.Simplify();
    REQUIRE(simplified->Is<Oasis::Multiply<Oasis::Expression>>());

    REQUIRE(Oasis::Add {
        Oasis::Real { 1.0 },
        Oasis::Multiply {
            Oasis::Real { 3.0 },
            Oasis::Variable { "x" } }}.Equals(*simplified));
}
*/

TEST_CASE("Generalized Addition", "[Add][Generalized]")
{
    Oasis::Add<Oasis::Expression> add {
        Oasis::Add<Oasis::Expression> {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = add.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Addition Async", "[Add][Async]")
{
    Oasis::Add add {
        Oasis::Add {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    std::unique_ptr<Oasis::Expression> simplified = add.SimplifyAsync();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Generalized Addition Async", "[Add][Generalized][Async]")
{
    Oasis::Add<Oasis::Expression> add {
        Oasis::Add<Oasis::Expression> {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = add.SimplifyAsync();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Structurally Equivalent", "[StructurallyEquivalent]")
{
    REQUIRE(
        Oasis::Add {
            Oasis::Real {},
            Oasis::Real {} }
            .StructurallyEquivalent(
                Oasis::Add {
                    Oasis::Real {},
                    Oasis::Real {} }));
}

TEST_CASE("Generalized Structurally Equivalent", "[StructurallyEquivalent][Generalized]")
{
    REQUIRE(
        Oasis::Add<Oasis::Expression> {
            Oasis::Real {},
            Oasis::Real {} }
            .StructurallyEquivalent(
                Oasis::Add<Oasis::Expression> {
                    Oasis::Real {},
                    Oasis::Real {} }));
}

TEST_CASE("Structurally Equivalent Async", "[StructurallyEquivalent][Async]")
{
    REQUIRE(
        Oasis::Add {
            Oasis::Real {},
            Oasis::Real {} }
            .StructurallyEquivalentAsync(
                Oasis::Add {
                    Oasis::Real {},
                    Oasis::Real {} }));
}

TEST_CASE("Generalized Structurally Equivalent Async", "[StructurallyEquivalent][Generalized][Async]")
{
    REQUIRE(
        Oasis::Add<Oasis::Expression> {
            Oasis::Real {},
            Oasis::Real {} }
            .StructurallyEquivalentAsync(
                Oasis::Add<Oasis::Expression> {
                    Oasis::Real {},
                    Oasis::Real {} }));
}

TEST_CASE("Specialization", "[Specialization]")
{
    REQUIRE(Oasis::Add<Oasis::Real>::Specialize(
        Oasis::Add<Oasis::Expression> {
            Oasis::Real {},
            Oasis::Real {} }));
}

TEST_CASE("Imaginary Addition", "[Imaginary][Add]")
{
    Oasis::Add a1 {
        Oasis::Imaginary {},
        Oasis::Imaginary {}
    };

    auto spec1 = a1.Simplify();

    REQUIRE(Oasis::Multiply { Oasis::Real { 2.0 }, Oasis::Imaginary {} }.Equals(*spec1));
}

TEST_CASE("Addition Associativity", "[Add][Associative]")
{
    Oasis::Add assoc1 {
        Oasis::Real { 2.0 },
        Oasis::Add {
            Oasis::Variable { "x" },
            Oasis::Add {
                Oasis::Add {
                    Oasis::Real { 3.0 },
                    Oasis::Variable { "x" } },
                Oasis::Multiply {
                    Oasis::Real { 4.0 },
                    Oasis::Variable { "x" } } } }
    };

    auto simplified1 = assoc1.Simplify();

    REQUIRE(Oasis::Add {
        Oasis::Real { 5.0 },
        Oasis::Multiply {
            Oasis::Real { 6.0 },
            Oasis::Variable { "x" } } }
                .Equals(*simplified1));
}

TEST_CASE("Add Associativity with wider tree", "[Add][Associativity]")
{
    Oasis::Add assoc2 {
        Oasis::Multiply {
            Oasis::Real { 2.0 },
            Oasis::Exponent {
                Oasis::Variable { "x" },
                Oasis::Real { 2.0 } } },
        Oasis::Add {
            Oasis::Exponent {
                Oasis::Variable { "x" },
                Oasis::Real { 2.0 } },
            Oasis::Add {
                Oasis::Add {
                    Oasis::Real { 3.0 },
                    Oasis::Variable { "x" } },
                Oasis::Multiply {
                    Oasis::Real { 4.0 },
                    Oasis::Variable { "x" } } } }
    };

    auto simplified2 = assoc2.Simplify();

    REQUIRE(Oasis::Add {
        Oasis::Add {

            Oasis::Multiply {
                Oasis::Real { 3.0 },
                Oasis::Exponent {
                    Oasis::Variable { "x" },
                    Oasis::Real { 2.0 } } },
            Oasis::Real { 3.0 },
        },
        Oasis::Multiply {
            Oasis::Real { 5.0 },
            Oasis::Variable { "x" } } }
                .Equals(*simplified2));
}

TEST_CASE("Add Operator Overload", "[Add][Operator Overload]")
{
    const std::unique_ptr<Oasis::Expression> a = std::make_unique<Oasis::Real>(1.0);
    const std::unique_ptr<Oasis::Expression> b = std::make_unique<Oasis::Real>(2.0);

    const auto sum = a + b;
    auto realSum = Oasis::Real::Specialize(*sum);

    REQUIRE(realSum != nullptr);
    REQUIRE(realSum->GetValue() == 3.0);
}

TEST_CASE("Variadic Add Constructor", "[Add]")
{
    const Oasis::Add<> add {
        Oasis::Real { 1.0 },
        Oasis::Real { 2.0 },
        Oasis::Real { 3.0 },
        Oasis::Real { 4.0 },
        Oasis::Multiply {
            Oasis::Real { 5.0 },
            Oasis::Real { 6.0 } }
    };

    const Oasis::Real expected { 40.0 };

    const auto simplified = add.Simplify();
    REQUIRE(expected.Equals(*simplified));
}