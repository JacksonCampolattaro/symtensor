#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <iostream>

#include <symtensor/SymmetricTensor3.h>
#include <symtensor/SymmetricTensorBase.h>

using namespace symtensor;

TEST_CASE("Member access to a 3x3 symmetric tensor", "[SymmetricTensor]") {

    using Quadrupole = SymmetricTensor3f<2>;

    // Construct a quadrupole with known values
    Quadrupole q{0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    using
    enum Quadrupole::Index;

    REQUIRE(q[{X, X}] == 0.0f);

    REQUIRE(q[{X, Y}] == 1.0f);
    REQUIRE(q[{Y, X}] == 1.0f);

    REQUIRE(q[{X, Z}] == 2.0f);
    REQUIRE(q[{Z, X}] == 2.0f);

    REQUIRE(q[{Y, Y}] == 3.0f);

    REQUIRE(q[{Y, Z}] == 4.0f);
    REQUIRE(q[{Z, Y}] == 4.0f);

    REQUIRE(q[{Z, Z}] == 5.0f);
}

TEST_CASE("Member access to a 3x3x3 symmetric tensor", "[SymmetricTensor]") {

    using Octupole = SymmetricTensor3f<3>;

    // Construct a quadrupole with known values
    Octupole o{0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    using
    enum Octupole::Index;

    REQUIRE(o[{X, X, X}] == 0.0f);

    REQUIRE(o[{X, X, Y}] == 1.0f);
    REQUIRE(o[{X, Y, X}] == 1.0f);

    REQUIRE(o[{X, X, Z}] == 2.0f);
    REQUIRE(o[{X, Z, X}] == 2.0f);

    REQUIRE(o[{X, Y, Y}] == 3.0f);
    REQUIRE(o[{Y, X, Y}] == 3.0f);

    REQUIRE(o[{X, Y, Z}] == 4.0f);
    REQUIRE(o[{X, Z, Y}] == 4.0f);

    REQUIRE(o[{X, Z, Z}] == 5.0f);
    REQUIRE(o[{Z, X, Z}] == 5.0f);

    REQUIRE(o[{Y, Y, Y}] == 6.0f);

    REQUIRE(o[{Y, Y, Z}] == 7.0f);
    REQUIRE(o[{Y, Z, Y}] == 7.0f);

    REQUIRE(o[{Y, Z, Z}] == 8.0f);
    REQUIRE(o[{Z, Y, Z}] == 8.0f);

    REQUIRE(o[{Z, Z, Z}] == 9.0f);

}

TEST_CASE("Member access to a 3x3x3x3 symmetric tensor", "[SymmetricTensor]") {

    using Hexadecupole = SymmetricTensor3f<4>;

    // Construct a quadrupole with known values
    Hexadecupole h{0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f};
    using
    enum Hexadecupole ::Index;

    REQUIRE(h[{X, X, X, X}] == 0.0f);

    REQUIRE(h[{X, X, X, Y}] == 1.0f);
    REQUIRE(h[{X, X, Y, X}] == 1.0f);

    REQUIRE(h[{X, X, X, Z}] == 2.0f);
    REQUIRE(h[{X, X, Z, X}] == 2.0f);

    REQUIRE(h[{X, X, Y, Y}] == 3.0f);
    REQUIRE(h[{X, Y, X, Y}] == 3.0f);

    REQUIRE(h[{X, X, Y, Z}] == 4.0f);
    REQUIRE(h[{X, X, Z, Y}] == 4.0f);

    REQUIRE(h[{X, X, Z, Z}] == 5.0f);
    REQUIRE(h[{X, Z, X, Z}] == 5.0f);

    REQUIRE(h[{X, Y, Y, Y}] == 6.0f);

    REQUIRE(h[{X, Y, Y, Z}] == 7.0f);
    REQUIRE(h[{X, Y, Z, Y}] == 7.0f);

    REQUIRE(h[{X, Y, Z, Z}] == 8.0f);
    REQUIRE(h[{X, Z, Y, Z}] == 8.0f);

    REQUIRE(h[{X, Z, Z, Z}] == 9.0f);

    REQUIRE(h[{Y, Y, Y, Y}] == 10.0f);

    REQUIRE(h[{Y, Y, Y, Z}] == 11.0f);
    REQUIRE(h[{Y, Y, Z, Y}] == 11.0f);

    REQUIRE(h[{Y, Y, Z, Z}] == 12.0f);
    REQUIRE(h[{Y, Z, Y, Z}] == 12.0f);

    REQUIRE(h[{Y, Z, Z, Z}] == 13.0f);
    REQUIRE(h[{Z, Y, Z, Z}] == 13.0f);

    REQUIRE(h[{Z, Z, Z, Z}] == 14.0f);

}
