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

TEST_CASE("Log addition", "[Add][Log]"){
    Oasis::Add addS(Oasis::Log(Oasis::Variable("x"),Oasis::Variable("y")),Oasis::Log(Oasis::Variable("x"),Oasis::Variable("z")));
    Oasis::Add addF(Oasis::Log(Oasis::Variable("y"),Oasis::Variable("x")),Oasis::Log(Oasis::Variable("z"),Oasis::Variable("x")));
    auto addSS = addS.Simplify();
    auto addFS = addF.Simplify();
    REQUIRE(addSS!=nullptr);
    auto addSSType = Oasis::Log<Oasis::Variable,Oasis::Add<Oasis::Variable>>::Specialize(*addSS);
    std::cout << addSS->ToString() << '\n';
    REQUIRE(addSSType!=nullptr);
    Oasis::Log goalS(Oasis::Variable("x"),Oasis::Add(Oasis::Variable("y"),Oasis::Variable("z")));
    REQUIRE(goalS.Equals(*addSSType));
    REQUIRE(addFS!=nullptr);
    REQUIRE(!goalS.Equals(*addFS));
}

TEST_CASE("Imaginary add large expression", "[Add][Imaginary]"){
    Oasis::Add add(Oasis::Add(Oasis::Imaginary(),Oasis::Real(4)),Oasis::Add(Oasis::Variable("z"),Oasis::Imaginary()));
    auto addSimp = add.Simplify();
    REQUIRE(addSimp!=nullptr);
    Oasis::Add goal(Oasis::Multiply(Oasis::Imaginary(),Oasis::Real(2)),Oasis::Add(Oasis::Variable("z"),Oasis::Real(4)));
    REQUIRE(addSimp->Equals(goal));
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