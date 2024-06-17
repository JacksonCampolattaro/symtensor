#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include <symtensor/symtensor.h>
#include <symtensor/util.h>

namespace symtensor::gravity {

    template<auto index, indexable Vector>
    inline constexpr auto product_of_elements(const Vector &v) {
        // todo: maybe don't use std::apply for this?
        return std::apply([&](auto ...i) {
            return (v[static_cast<std::size_t>(i)] * ...);
        }, index);
    }

    template<auto index, std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivative_at(const Vector &R) {

        auto r = glm::length(R);
        auto g1 = -1.0f / pow<3>(r);
        auto g2 = 3.0f / pow<5>(r);
        auto g3 = -15.0f / pow<7>(r);
        auto g4 = 105.0f / pow<9>(r);

        auto cartesian_product_term = product_of_elements<index>(R);
        if constexpr (N == 1) {
            return R[static_cast<std::size_t>(index[0])] * g1;
        } else if constexpr (N == 2) {
            return g1 * kronecker_delta<float>(index) + g2 * cartesian_product_term;
        } else if constexpr (N == 3) {
            constexpr auto partitions = binomial_partitions<1>(index);
            constexpr auto r_indices = std::get<0>(unzip(partitions));
            constexpr auto kronecker_indices = std::get<1>(unzip(partitions));
            constexpr auto repeats = repeats_table(kronecker_indices);
            auto kronecker_product_term = [&]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
                return ((
                        force_consteval<kronecker_delta<kronecker_indices[i]>() && (repeats[i] > 0)> ?
                        product_of_elements<r_indices[i]>(R) * repeats[i] : 0
                ) + ...);
            }(std::make_index_sequence<kronecker_indices.size()>());
            return g2 * kronecker_product_term + g3 * cartesian_product_term;
        } else if constexpr (N == 4) {
            auto result = g4 * cartesian_product_term;
            constexpr auto partitions = binomial_partitions<2>(index);
            constexpr auto kronecker_indices = std::get<0>(unzip(partitions));
            constexpr auto r_indices = std::get<1>(unzip(partitions));
            {
                constexpr auto kronecker_term = [&]<auto... i>(std::index_sequence<i...>) constexpr {
                    return ((kronecker_delta(kronecker_indices[i]) & kronecker_delta(r_indices[i])) + ...);
                }(std::make_index_sequence<kronecker_indices.size() / 2>());
                result += g2 * kronecker_term;
            }
            {
                constexpr auto repeats = repeats_table(kronecker_indices);
                auto kronecker_product_term = [&]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
                    return ((
                            force_consteval<kronecker_delta<kronecker_indices[i]>() && (repeats[i] > 0)> ?
                            product_of_elements<r_indices[i]>(R) * repeats[i] : 0
                    ) + ...);
                }(std::make_index_sequence<kronecker_indices.size()>());
                result += g3 * kronecker_product_term;
            }

            return result;
        }
        return 0.0f;
    }

    template<std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivative(const Vector &R) {
        return SymmetricTensor3f<N>::NullaryExpression([&]<auto index>()  LAMBDA_ALWAYS_INLINE {
            return derivative_at<index, N>(R);
        });
    };


    // todo: this needs to be regularized -- there must be a pattern!
    template<std::size_t Order, indexable Vector>
    ALWAYS_INLINE auto D(const Vector &R) {

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

        }
    }


    auto derivative4(const glm::vec3 &R) {
        return derivative<4>(R);
    }

    auto D4(const glm::vec3 &R) {
        return D<4>(R);
    }

}
