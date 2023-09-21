#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <iostream>
#include <numeric>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

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


TEST_CASE("benchmark: Tensor-vector multiplication", "[SymmetricTensor]") {

    {

        SymmetricTensor2f<1> a{1, 2};
        glm::vec2 glm_a{1, 2};
        BENCHMARK("st2x2 = v2 * v2") { return SymmetricTensor2f<2>::CartesianPower(a); };
        BENCHMARK("st2x2 = v2 * v2 (handwritten)") {
                                                       return SymmetricTensor2f<2>{
                                                               a[0] * a[0], a[0] * a[1],
                                                               a[1] * a[1]
                                                       };
                                                   };
        BENCHMARK("t2x2 = v2 * v2 (glm)") { return glm::outerProduct(glm_a, glm_a); };
    }

    {

        SymmetricTensor3f<1> a{0, 1, 2};
        BENCHMARK("st3x3 = v3 * v3") { return SymmetricTensor3f<2>::CartesianPower(a); };
        BENCHMARK("st3x3 = v3 * v3 (handwritten)") {
                                                       return SymmetricTensor3f<2>{
                                                               a[0] * a[0], a[0] * a[1], a[0] * a[2],
                                                               a[1] * a[1], a[1] * a[2],
                                                               a[2] * a[2]
                                                       };
                                                   };

        glm::vec3 glm_a{0, 1, 2};
        BENCHMARK("t3x3 = v3 * v3 (glm)") { return (glm_a * glm_a); };
    }

    {

        SymmetricTensor3f<1> a{0, 1, 2};
        glm::vec3 glm_a{0, 1, 2};
        using glm_mat3x3x3 = std::array<glm::mat3x3, 3>;
        BENCHMARK("st3x3x3 = v3 * v3 * v3") { return SymmetricTensor3f<3>::CartesianPower(a); };
        BENCHMARK("t3x3x3 = v3 * v3 * v3 (glm)") {
                                                      glm::mat3x3 glm_b = glm::outerProduct(glm_a, glm_a);
                                                      return glm_mat3x3x3{
                                                              glm_b * glm_a[0],
                                                              glm_b * glm_a[1],
                                                              glm_b * glm_a[2]
                                                      };
                                                  };

        //        SymmetricTensor3f<2> b = a * a;
        //        BENCHMARK("st3x3x3 = st3x3 * v3") { return (b * a); };
        //
        //        using glm_mat3x3x3 = std::array<glm::mat3x3, 3>;
        //        glm::mat3x3 glm_b = glm::outerProduct(glm_a, glm_a);
        //        BENCHMARK("m3x3x3 = m3x3 * v3 (glm)") {
        //                                                return glm_mat3x3x3{
        //                                                        glm_b * glm_a[0],
        //                                                        glm_b * glm_a[1],
        //                                                        glm_b * glm_a[2]
        //                                                };
        //                                            };


    }

    //    {
    //
    //        auto multiply = [](const SymmetricTensor3f<3> &a, const SymmetricTensor3f<1> &b) {
    //            SymmetricTensor3f<4> result;
    //            using Index = SymmetricTensor3f<4>::Index;
    //            for (int i = 0; i < 3; ++i) {
    //                for (int j = 0; j < 3; ++j) {
    //                    for (int k = 0; k < 3; ++k) {
    //                        for (int l = 0; l < 3; ++l) {
    //                            result[{Index(i), Index(j), Index(k), Index(l)}] =
    //                                    (a[{Index(i), Index(j), Index(k)}] * b[l]);
    //                        }
    //
    //                    }
    //                }
    //            }
    //            return result;
    //        };
    //        SymmetricTensor3f<3> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    //        SymmetricTensor3f<1> b{3, 4, 5};
    //        SymmetricTensor3f<4> result;
    //        REQUIRE(a * b == multiply(a, b));
    //        BENCHMARK("st3x3x3x3 = vt3x3x3 * v3") { return result = (a * b); };
    //        BENCHMARK("st3x3x3x3 = multiply(vt3x3x3, v3)") { return result = multiply(a, b); };
    //    }
}
