#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include "symtensor/symtensor.h"
#include "symtensor/util.h"

namespace symtensor::gravity::direct {

    // An implementation of gravity derivatives in the style of GADGET-4
    template<std::size_t Order, indexable Vector>
    ALWAYS_INLINE auto derivative(const Vector &R) {

        auto r = glm::length(R);

        #if (__cpp_using_enum && !__clang__) || (__clang_major__ > 15)
        using
        enum SymmetricTensor3f<1>::Index;
        #else
        using Index<3>::X;
        using Index<3>::Y;
        using Index<3>::Z;
        #endif

        float f1 = -1.0f / pow<2>(r);
        float f2 = 3.0f / pow<3>(r);
        float f3 = -15.0f / pow<4>(r);
        float f4 = 105.0f / pow<5>(r);
        float f5 = -945.0f / pow<6>(r);

        float g1 = f1 / r;
        float g2 = f2 / pow<2>(r);
        float g3 = f3 / pow<3>(r);
        float g4 = f4 / pow<4>(r);
        float g5 = f5 / pow<5>(r);

        if constexpr (Order == 1) {

            return g1 * R;

        } else if constexpr (Order == 2) {

            return (g2 * SymmetricTensor3f < 2 > ::CartesianPower(R)) + (g1 * SymmetricTensor3f < 2 > ::Identity());

        } else if constexpr (Order == 3) {

            auto A = g3 * SymmetricTensor3f < 3 > ::CartesianPower(R);
            auto B = SymmetricTensor3f < 3 > {};
            B.at<X, Y, Y>() = R.x;
            B.at<X, Z, Z>() = R.x;
            B.at<Y, X, X>() = R.y;
            B.at<Y, Z, Z>() = R.y;
            B.at<Z, X, X>() = R.z;
            B.at<Z, Y, Y>() = R.z;
            B += 3 * SymmetricTensor3f < 3 > ::Diagonal(R);
            B *= g2;
            return A + B;

        } else if constexpr (Order == 4) {

            auto A = g4 * SymmetricTensor3f < 4 > ::CartesianPower(R);

            SymmetricTensor3f<4> B{};
            B.at<X, X, Y, Y>() = 1;
            B.at<X, X, Z, Z>() = 1;
            B.at<Y, Y, Z, Z>() = 1;
            B += SymmetricTensor3f < 4 > ::Diagonal(glm::vec3{3, 3, 3});
            B *= g2;

            SymmetricTensor3f<4> C{};
            SymmetricTensor3f<2> R2 = SymmetricTensor3f < 2 > ::CartesianPower(R);
            C += 6.0f * SymmetricTensor3f < 4 > ::Diagonal(R2.diagonal());
            C.at<X, X, X, Y>() = 3 * R2.at<X, Y>();
            C.at<X, Y, Y, Y>() = 3 * R2.at<X, Y>();
            C.at<X, X, X, Z>() = 3 * R2.at<X, Z>();
            C.at<X, Z, Z, Z>() = 3 * R2.at<X, Z>();
            C.at<Y, Y, Y, Z>() = 3 * R2.at<Y, Z>();
            C.at<Y, Z, Z, Z>() = 3 * R2.at<Y, Z>();
            C.at<X, X, Y, Y>() = R2.at<X, X>() + R2.at<Y, Y>();
            C.at<X, X, Z, Z>() = R2.at<X, X>() + R2.at<Z, Z>();
            C.at<Y, Y, Z, Z>() = R2.at<Y, Y>() + R2.at<Z, Z>();
            C.at<X, X, Y, Z>() = R2.at<Y, Z>();
            C.at<X, Y, Y, Z>() = R2.at<X, Z>();
            C.at<X, Y, Z, Z>() = R2.at<X, Y>();
            C *= g3;

            return A + B + C;

        } else if constexpr (Order == 5) {


            SymmetricTensor3f<2> R2 = SymmetricTensor3f < 2 > ::CartesianPower(R);
            SymmetricTensor3f<3> R3 = SymmetricTensor3f < 3 > ::CartesianPower(R);

            auto A = g5 * SymmetricTensor3f < 5 > ::CartesianPower(R);

            // ~~~

            SymmetricTensor3f<5> B{};
            B.at<X, X, X, X, X>() += 15 * g3 * R.x;
            B.at<Y, Y, Y, Y, Y>() += 15 * g3 * R.y;
            B.at<Z, Z, Z, Z, Z>() += 15 * g3 * R.z;

            B.at<X, X, X, X, Y>() += 3 * g3 * R.y;
            B.at<X, X, X, X, Z>() += 3 * g3 * R.z;
            B.at<X, Y, Y, Y, Y>() += 3 * g3 * R.x;
            B.at<X, Z, Z, Z, Z>() += 3 * g3 * R.x;
            B.at<Y, Y, Y, Y, Z>() += 3 * g3 * R.z;
            B.at<Y, Z, Z, Z, Z>() += 3 * g3 * R.y;

            B.at<X, X, X, Y, Y>() += 3 * g3 * R.x;
            B.at<X, X, X, Z, Z>() += 3 * g3 * R.x;
            B.at<X, X, Y, Y, Y>() += 3 * g3 * R.y;
            B.at<X, X, Z, Z, Z>() += 3 * g3 * R.z;
            B.at<Y, Y, Y, Z, Z>() += 3 * g3 * R.y;
            B.at<Y, Y, Z, Z, Z>() += 3 * g3 * R.z;

            B.at<X, X, Y, Z, Z>() += g3 * R.y;
            B.at<X, X, Y, Y, Z>() += g3 * R.z;
            B.at<X, Y, Y, Z, Z>() += g3 * R.x;

            B.at<X, X, X, Y, Z>() += 0;
            B.at<X, Y, Y, Y, Z>() += 0;
            B.at<X, Y, Z, Z, Z>() += 0;

            // ~~~

            SymmetricTensor3f<5> C{};
            C.at<X, X, X, X, X>() += 10 * g4 * R3.at<X, X, X>();
            C.at<Y, Y, Y, Y, Y>() += 10 * g4 * R3.at<Y, Y, Y>();
            C.at<Z, Z, Z, Z, Z>() += 10 * g4 * R3.at<Z, Z, Z>();

            C.at<X, X, X, X, Y>() += 6 * g4 * R3.at<X, X, Y>();
            C.at<X, X, X, X, Z>() += 6 * g4 * R3.at<X, X, Z>();
            C.at<X, Y, Y, Y, Y>() += 6 * g4 * R3.at<Y, Y, X>();
            C.at<X, Z, Z, Z, Z>() += 6 * g4 * R3.at<Z, Z, X>();
            C.at<Y, Y, Y, Y, Z>() += 6 * g4 * R3.at<Y, Y, Z>();
            C.at<Y, Z, Z, Z, Z>() += 6 * g4 * R3.at<Z, Z, Y>();

            C.at<X, X, X, Y, Y>() += g4 * R3.at<X, X, X>() + 3 * g4 * R3.at<X, Y, Y>();
            C.at<X, X, X, Z, Z>() += g4 * R3.at<X, X, X>() + 3 * g4 * R3.at<X, Z, Z>();
            C.at<X, X, Y, Y, Y>() += g4 * R3.at<Y, Y, Y>() + 3 * g4 * R3.at<Y, X, X>();
            C.at<X, X, Z, Z, Z>() += g4 * R3.at<Z, Z, Z>() + 3 * g4 * R3.at<Z, X, X>();
            C.at<Y, Y, Y, Z, Z>() += g4 * R3.at<Y, Y, Y>() + 3 * g4 * R3.at<Y, Z, Z>();
            C.at<Y, Y, Z, Z, Z>() += g4 * R3.at<Z, Z, Z>() + 3 * g4 * R3.at<Z, Y, Y>();

            C.at<X, X, Y, Z, Z>() += g4 * R3.at<Y, Z, Z>() + g4 * R3.at<X, X, Y>();
            C.at<X, X, Y, Y, Z>() += g4 * R3.at<Y, Y, Z>() + g4 * R3.at<X, X, Z>();
            C.at<X, Y, Y, Z, Z>() += g4 * R3.at<X, Z, Z>() + g4 * R3.at<X, Y, Y>();

            C.at<X, X, X, Y, Z>() += 3 * g4 * R3.at<X, Y, Z>();
            C.at<X, Y, Y, Y, Z>() += 3 * g4 * R3.at<X, Y, Z>();
            C.at<X, Y, Z, Z, Z>() += 3 * g4 * R3.at<X, Y, Z>();

            // todo: is this right?

            return A + B + C;
        }
    }

    template<std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivatives(const Vector &R) {
        return [&]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
            return std::make_tuple(derivative<i + 1>(R) ...);
        }(std::make_index_sequence<N>());
    };


}