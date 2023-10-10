#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <symtensor/ArithmeticTuple.h>

#include <iostream>
#include <numeric>

using namespace symtensor;

TEST_CASE("Arithmetic operations with a scalar", "[ArithmeticTuple]") {

    static_assert(std::is_same_v<std::tuple<int, float>, underlying_tuple<ArithmeticTuple<int, float>>>);

    ArithmeticTuple<int, float, double> numbers{1, 2.0f, 3.0};
    REQUIRE(numbers + 2 == ArithmeticTuple<int, float, double>{3, 4.0f, 5.0});
    REQUIRE(numbers - 2 == ArithmeticTuple<int, float, double>{-1, 0.0f, 1.0});
    REQUIRE(numbers * 2 == ArithmeticTuple<int, float, double>{2, 4.0f, 6.0});
    REQUIRE(numbers / 2 == ArithmeticTuple<int, float, double>{0, 1.0f, 1.5});

}

TEST_CASE("Arithmetic operations between tuples", "[ArithmeticTuple]") {

    ArithmeticTuple<int, float, double> a{1, 2.0f, 3.0}, b{2, 4.0f, 6.0};
    REQUIRE(a + b == ArithmeticTuple<int, float, double>{3, 6.0f, 9.0});
    REQUIRE(a - b == ArithmeticTuple<int, float, double>{-1, -2.0f, -3.0});
    REQUIRE(a * b == ArithmeticTuple<int, float, double>{2, 8.0f, 18.0});
    REQUIRE(a / b == ArithmeticTuple<int, float, double>{0, 0.5f, 0.5});

}
