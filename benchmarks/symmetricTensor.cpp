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
        using
        enum SymmetricTensor2f<2>::Index;

        volatile SymmetricTensor2f<2>::Index vX = X, vY = Y;
        BENCHMARK("st2x2[volatile a, volatile b]") { return st2x2[{vX, vX}] + st2x2[{vX, vY}] + st2x2[{vY, vY}]; };
        BENCHMARK("st2x2[a, b]") { return st2x2[{X, X}] + st2x2[{X, Y}] + st2x2[{Y, Y}]; };
        BENCHMARK("st2x2.at<a, b>()") { return st2x2.at<X, X>() + st2x2.at<X, Y>() + st2x2.at<Y, Y>(); };
        BENCHMARK("st2x2[flat]") { return st2x2[0] + st2x2[1] + st2x2[2]; };
    }

    {
        SymmetricTensor3f<2> st3x3{1, 2, 3, 4, 5, 6};
        using
        enum SymmetricTensor3f<2>::Index;

        volatile SymmetricTensor3f<2>::Index vX = X, vY = Y, vZ = Z;
        BENCHMARK("st3x3[volatile a, ...]") { return st3x3[{vX, vX}] + st3x3[{vX, vY}] + st3x3[{vZ, vY}]; };
        BENCHMARK("st3x3[a, b]") { return st3x3[{X, X}] + st3x3[{X, Y}] + st3x3[{Z, Y}]; };
        BENCHMARK("st3x3.at<a, b>()") { return st3x3.at<X, X>() + st3x3.at<X, Y>() + st3x3.at<Z, Y>(); };
        BENCHMARK("st3x3[flat]") { return st3x3[0] + st3x3[3] + st3x3[4]; };
    }

    {
        SymmetricTensor3f<3> st3x3x3{1, 2, 3, 4, 5, 6, 7, 8, 9};
        using
        enum SymmetricTensor3f<3>::Index;

        volatile SymmetricTensor3f<3>::Index vX = X, vY = Y, vZ = Z;
        BENCHMARK("st3x3x3[volatile a, ...]")
                    { return st3x3x3[{vX, vX, vX}] + st3x3x3[{vX, vY, vZ}] + st3x3x3[{vZ, vY, vX}]; };
        BENCHMARK("st3x3x3[a, b]")
                    { return st3x3x3[{X, X, X}] + st3x3x3[{X, Y, Z}] + st3x3x3[{Z, Y, X}]; };
        BENCHMARK("st3x3x3.at<a, b>()")
                    { return st3x3x3.at<X, X, X>() + st3x3x3.at<X, Y, Z>() + st3x3x3.at<Z, Y, X>(); };
        BENCHMARK("st3x3x3[flat]")
                    { return st3x3x3[0] + st3x3x3[4] + st3x3x3[4]; };
    }

    {
        SymmetricTensor3f<5> st3x3x3x3x3{1, 2, 3, 4, 5, 6, 7, 8, 9};
        using
        enum SymmetricTensor3f<5>::Index;

        volatile SymmetricTensor3f<5>::Index vX = X, vY = Y, vZ = Z;
        BENCHMARK("st3x3x3x3x3[volatile a, ...]")
                    {
                        return st3x3x3x3x3[{vX, vX, vX, vX}] + st3x3x3x3x3[{vX, vY, vZ, vX}] +
                               st3x3x3x3x3[{vZ, vY, vX, vX}] + st3x3x3x3x3[{vZ, vY, vZ, vZ}];
                    };
        BENCHMARK("st3x3x3x3x3[a, ...]")
                    {
                        return st3x3x3x3x3[{X, X, X}] + st3x3x3x3x3[{X, Y, Z}] + st3x3x3x3x3[{Z, Y, X}] +
                               st3x3x3x3x3[{Z, Y, Z, Z}];
                    };
        BENCHMARK("st3x3x3x3x3.at<a, ...>()")
                    { return st3x3x3x3x3.at<X, X, X>() + st3x3x3x3x3.at<X, Y, Z>() + st3x3x3x3x3.at<Z, Y, X>(); };
        BENCHMARK("st3x3x3x3x3[flat]")
                    { return st3x3x3x3x3[0] + st3x3x3x3x3[4] + st3x3x3x3x3[4]; };
    }
}
