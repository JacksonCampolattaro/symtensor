#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <symtensor/symtensor.h>

#include <iostream>

using namespace symtensor;

TEST_CASE("Member access", "[Multipole]") {

    Octupole3f a{
            {1, 2, 3},
            {1, 2, 3, 4, 5, 6},
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    };

    REQUIRE(a.tensor<1>() == SymmetricTensor3f<1>{1, 2, 3});
    REQUIRE(a.tensor<2>() == SymmetricTensor3f<2>{1, 2, 3, 4, 5, 6});
    REQUIRE(a.tensor<3>() == SymmetricTensor3f<3>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // todo: implement structured bindings
    //auto &[a1, a2, a3] = a;

}

TEST_CASE("Arithmetic operations with a scalar", "[Multipole]") {

    Quadrupole2f a{{1, 2},
                   {1, 2, 3}};
    REQUIRE(a + 2 == Quadrupole2f{{3, 4},
                                  {3, 4, 5}});
    REQUIRE(a - 2 == Quadrupole2f{{-1, 0},
                                  {-1, 0, 1}});
    REQUIRE(a * 2 == Quadrupole2f{{2, 4},
                                  {2, 4, 6}});
    REQUIRE(a / 2 == Quadrupole2f{{0.5,   1.0},
                                  {0.5, 1.0, 1.5}});
}


TEST_CASE("Arithmetic operations with another multipole", "[Multipole]") {

    Quadrupole2f a{{1, 2},
                   {1, 2, 3}};

    Quadrupole2f b{{2, 2},
                   {2, 2, 2}};

    REQUIRE(a + b == Quadrupole2f{{3, 4},
                                  {3, 4, 5}});
    REQUIRE(a - b == Quadrupole2f{{-1, 0},
                                  {-1, 0, 1}});
}
