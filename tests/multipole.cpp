#include <catch2/catch_test_macros.hpp>

#include <symtensor/symtensor.h>

#include <iostream>

using namespace symtensor;

// Workaround for compiler defect DR2621 in Clang 15
// https://reviews.llvm.org/D134283
#if (__cpp_using_enum && !__clang__) || (__clang_major__ > 15)
using enum SymmetricTensor3f<1>::Index;
#else
using Index<3>::X;
using Index<3>::Y;
using Index<3>::Z;
#endif

TEST_CASE("Access to tensors", "[Multipole]") {

    Octupole3f a{
            {1, 2, 3},
            {1, 2, 3, 4, 5, 6},
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    };

    // Access indexed by rank
    REQUIRE(a.tensor<1>() == SymmetricTensor3f<1>{1, 2, 3});
    REQUIRE(a.tensor<2>() == SymmetricTensor3f<2>{1, 2, 3, 4, 5, 6});
    REQUIRE(a.tensor<3>() == SymmetricTensor3f<3>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // Access using structured bindings
    const auto &[a1, a2, a3] = a;
    REQUIRE(a1 == SymmetricTensor3f<1>{1, 2, 3});
    REQUIRE(a2 == SymmetricTensor3f<2>{1, 2, 3, 4, 5, 6});
    REQUIRE(a3 == SymmetricTensor3f<3>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // Direct access to tensor members
    REQUIRE(a.at<Z>() == 3);
    REQUIRE(a.at<X, X>() == 1);
    REQUIRE(a.at<X, Y, X>() == 2);
    REQUIRE(a.at<Z, Z, Z>() == 10);

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
