#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <iostream>

#include <numeric>
#include <glm/vec2.hpp>

#include <symtensor/SymmetricTensorBase.h>

using namespace symtensor;

TEST_CASE("benchmark: Member access", "[SymmetricTensor]") {

    using Quadrupole = SymmetricTensor3f<2>;

    // Construct a quadrupole with known values
    constexpr Quadrupole q{0, 1, 2, 3, 4, 5};
    using
    enum Quadrupole::Index;

    //BENCHMARK("no indexing;") { return 1 + 2 + 3; };

    {
        SymmetricTensor2f<2> st2x2{1, 2, 3};
        using enum SymmetricTensor2f<2>::Index;

        BENCHMARK("2x2 tensor flat index") { return st2x2[0] + st2x2[1] + st2x2[3]; };
        BENCHMARK("2x2 tensor 2x2 index") { return st2x2[{X, X}] + st2x2[{X, Y}] + st2x2[{X, Y}]; };
    }
}
