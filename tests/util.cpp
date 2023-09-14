#include <symtensor/util.h>
#include <catch2/catch_test_macros.hpp>

using namespace symtensor;

TEST_CASE("Number of values in an non-symmetric tensor", "[numValuesInTensor]") {

    // Vectors of several sizes
    REQUIRE(numValuesInTensor(2, 1) == 2);
    REQUIRE(numValuesInTensor(3, 1) == 3);
    REQUIRE(numValuesInTensor(4, 1) == 4);

    // Matrices of several sizes
    REQUIRE(numValuesInTensor(2, 2) == 4);
    REQUIRE(numValuesInTensor(3, 2) == 9);
    REQUIRE(numValuesInTensor(4, 2) == 16);

    // 3-tensors of several sizes
    REQUIRE(numValuesInTensor(2, 3) == 8);
    REQUIRE(numValuesInTensor(3, 3) == 27);
    REQUIRE(numValuesInTensor(4, 3) == 64);
}

TEST_CASE("Number of unique values in a symmetric tensor", "[numUniqueValuesInSymmetricTensor]") {

    // Vectors of several sizes
    // (these should contain no redundant values)
    REQUIRE(numUniqueValuesInSymmetricTensor(2, 1) == 2);
    REQUIRE(numUniqueValuesInSymmetricTensor(3, 1) == 3);
    REQUIRE(numUniqueValuesInSymmetricTensor(4, 1) == 4);

    // Matrices of several sizes
    REQUIRE(numUniqueValuesInSymmetricTensor(2, 2) == 3);
    REQUIRE(numUniqueValuesInSymmetricTensor(3, 2) == 6);
    REQUIRE(numUniqueValuesInSymmetricTensor(4, 2) == 10);

    // 3-tensors of several sizes
    REQUIRE(numUniqueValuesInSymmetricTensor(2, 3) == 4);
    REQUIRE(numUniqueValuesInSymmetricTensor(3, 3) == 10);
    REQUIRE(numUniqueValuesInSymmetricTensor(4, 3) == 20);
}
