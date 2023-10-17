#include <catch2/catch_test_macros.hpp>

#include <symtensor/util.h>
#include <symtensor/Index.h>

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

TEST_CASE("Convert simple function to lookup table", "[as_lookup_table]") {
//
//    CHECK(
//            as_lookup_table<
//                    decltype([](int x) consteval { return 2 * x; }),
//                    int,
//                    1, 2, 3, 4, 5
//                                > (5) == 10
//    );
//
//    CHECK(
//            as_lookup_table<
//                    decltype([](auto x) consteval { return x.first + x.second; }),
//                    std::pair<int, int>,
//                    std::pair<int, int>{1, 1}, std::pair<int, int>{1, 2}, std::pair<int, int>{5, 2}
//            >({5, 2}) == 7
//    );
//
//    using I = std::size_t;
//    constexpr std::size_t R = 2;
//    constexpr std::size_t D = 2;
//    CHECK(
//            as_lookup_table<
//                    decltype([](std::array<I, R> ind) consteval { return symtensor::flatIndex(ind, D); }),
//                    std::array<I, R>,
//                    symtensor::lexicographicalIndices<R, I>(1, D)
//            >({0, 1}) == 1
//    );

}
