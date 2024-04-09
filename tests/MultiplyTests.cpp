//
// Created by Matthew McCall on 8/10/23.
//

#include "catch2/catch_test_macros.hpp"

#include "Oasis/Add.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Imaginary.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
#include "Oasis/Subtract.hpp"
#include "Oasis/Util.hpp"

TEST_CASE("Multiplication", "[Multiply]")
{
    Oasis::Multiply subtract {
        Oasis::Multiply {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = subtract.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Complex Multiplication", "[Multiply][Complex]")
{
    const double epsilon = std::pow(10, -10);
    Oasis::Real real(3.0);
    Oasis::Imaginary img;
    Oasis::Multiply<Oasis::Real, Oasis::Imaginary> imgT(Oasis::Real(13.0), img);
    Oasis::Add<Oasis::Real, Oasis::Imaginary> realPI(Oasis::Real(5.0), img);
    Oasis::Add<Oasis::Real, Oasis::Multiply<Oasis::Real, Oasis::Imaginary>> complex = Oasis::Util::pairToComp(11.0, 7.0);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(imgT, imgT), Oasis::Util::pairToComp(-169.0, 0.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(imgT, real), Oasis::Util::pairToComp(0, 39.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(imgT, img), Oasis::Util::pairToComp(-13.0, 0.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(imgT, realPI), Oasis::Util::pairToComp(-13.0, 65.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(imgT, complex), Oasis::Util::pairToComp(-91, 143.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(real, real), Oasis::Util::pairToComp(9.0, 0.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(real, img), Oasis::Util::pairToComp(0.0, 3.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(real, realPI), Oasis::Util::pairToComp(15.0, 3.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(real, complex), Oasis::Util::pairToComp(33.0, 21.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(img, img), Oasis::Util::pairToComp(-1.0, 0.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(img, realPI), Oasis::Util::pairToComp(-1.0, 5.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(img, complex), Oasis::Util::pairToComp(-7.0, 11.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(realPI, realPI), Oasis::Util::pairToComp(24.0, 10.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(realPI, complex), Oasis::Util::pairToComp(48.0, 46.0))).GetValue() < epsilon);
    REQUIRE(Oasis::Util::abs(Oasis::Subtract(Oasis::Multiply(complex, complex), Oasis::Util::pairToComp(72.0, 154.0))).GetValue() < epsilon);
}

TEST_CASE("Generalized Multiplication", "[Multiply][Generalized]")
{
    Oasis::Multiply<Oasis::Expression> subtract {
        Oasis::Multiply<Oasis::Expression> {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    auto simplified = subtract.Simplify();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Multiplication Async", "[Multiply][Async]")
{
    Oasis::Multiply subtract {
        Oasis::Multiply {
            Oasis::Real { 1.0 },
            Oasis::Real { 2.0 } },
        Oasis::Real { 3.0 }
    };

    std::unique_ptr<Oasis::Expression> simplified = subtract.SimplifyAsync();
    REQUIRE(simplified->Is<Oasis::Real>());

    auto simplifiedReal = dynamic_cast<Oasis::Real&>(*simplified);
    REQUIRE(simplifiedReal.GetValue() == 6.0);
}

TEST_CASE("Imaginary Multiplication", "[Imaginary][Multiplication]")
{
    Oasis::Multiply i2 {
        Oasis::Imaginary {},
        Oasis::Imaginary {}
    };
    Oasis::Multiply i3 {
        Oasis::Exponent {
            Oasis::Imaginary {},
            Oasis::Real { 2.0 } },
        Oasis::Imaginary {}
    };
    Oasis::Multiply i4 {
        Oasis::Exponent {
            Oasis::Imaginary {},
            Oasis::Real { 2.0 } },
        Oasis::Exponent {
            Oasis::Imaginary {},
            Oasis::Real { 2.0 } }
    };

    auto simplified2 = i2.Simplify();
    auto simplified3 = i3.Simplify();
    auto simplified4 = i4.Simplify();

    REQUIRE(Oasis::Multiply { Oasis::Real { -1 }, Oasis::Imaginary {} }.Equals(*simplified3));
    REQUIRE(Oasis::Real { -1.0 }.Equals(*simplified2));
    REQUIRE(Oasis::Real { 1.0 }.Equals(*simplified4));
}

TEST_CASE("Imaginary Real Product", "[Imaginary][Real][Multiplication]")
{
    Oasis::Imaginary img;
    Oasis::Multiply a1 {
        Oasis::Multiply {
            Oasis::Real(7.0),
            img },
        Oasis::Multiply {
            Oasis::Real(13.0),
            img }
    };

    auto spec1 = Oasis::Real::Specialize(*a1.Simplify());
    REQUIRE(spec1 != nullptr);
    REQUIRE(spec1->GetValue() == -91.0);
}

TEST_CASE("Multiply Associativity", "[Multiply][Associativity]")
{
    Oasis::Multiply m1 {
        Oasis::Multiply {
            Oasis::Variable { "x" },
            Oasis::Real { 3.0 } },
        Oasis::Multiply {
            Oasis::Variable { "y" },
            Oasis::Exponent {
                Oasis::Variable { "x" },
                Oasis::Real { 4.0 } } }
    };

    auto simplified1 = m1.Simplify();

    REQUIRE(Oasis::Multiply {
        Oasis::Multiply {
            Oasis::Exponent {
                Oasis::Variable { "x" },
                Oasis::Real { 5.0 } },
            Oasis::Real { 3.0 } },
        Oasis::Variable { "y" } }
                .Equals(*simplified1));
}
TEST_CASE("Multiply Operator Overload", "[Multiply][Operator Overload]")
{
    const std::unique_ptr<Oasis::Expression> a = std::make_unique<Oasis::Real>(1.0);
    const std::unique_ptr<Oasis::Expression> b = std::make_unique<Oasis::Real>(2.0);

    const auto sum = a * b;
    auto realSum = Oasis::Real::Specialize(*sum);

    REQUIRE(realSum != nullptr);
    REQUIRE(realSum->GetValue() == 2.0);
}

TEST_CASE("Variadic Multiply Constructor", "[Multiply]")
{
    const Oasis::Multiply<> multiply {
        Oasis::Real { 1.0 },
        Oasis::Real { 2.0 },
        Oasis::Real { 3.0 },
        Oasis::Real { 4.0 },
        Oasis::Add<> {
            Oasis::Real { 5.0 },
            Oasis::Real { 6.0 },
                Oasis::Real { 7.0 }}
    };

    const Oasis::Real expected { 432.0 };

    const auto simplified = multiply.Simplify();
    REQUIRE(expected.Equals(*simplified));
}