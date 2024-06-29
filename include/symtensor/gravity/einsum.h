#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include "symtensor/symtensor.h"
#include "symtensor/util.h"

namespace symtensor::gravity::einsum {

    template<auto index, indexable Vector>
    inline constexpr auto product_of_elements(const Vector &v) {
        return std::apply([=](auto ...i) {
            return (v[static_cast<std::size_t>(i)] * ...);
        }, index);
    }

    template<auto index, std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivative_at(const Vector &R) {

        auto r2 = glm::length2(R);
        auto r = std::sqrt(r2);
        auto inv_r2 = 1.0f / r2;
        auto g1 = -inv_r2 / r;
        auto g2 = -3.0f * g1 * inv_r2;
        auto g3 = -5.0f * g2 * inv_r2;
        auto g4 = -7.0f * g3 * inv_r2;
        auto g5 = -9.0f * g4 * inv_r2;

        //        auto r = glm::length(R);
        //        auto g1 = -1.0f / pow<3>(r);
        //        auto g2 = 3.0f / pow<5>(r);
        //        auto g3 = -15.0f / pow<7>(r);
        //        auto g4 = 105.0f / pow<9>(r);
        //        auto g5 = -945.0f / pow<11>(r);

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
            auto kronecker_product_term = [=]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
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
                constexpr auto kronecker_term = [=]<auto... i>(std::index_sequence<i...>) constexpr {
                    return ((kronecker_delta(kronecker_indices[i]) & kronecker_delta(r_indices[i])) + ...);
                }(std::make_index_sequence<kronecker_indices.size() / 2>());
                result += g2 * kronecker_term;
            }
            {
                constexpr auto repeats = repeats_table(kronecker_indices);
                auto kronecker_product_term = [=]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
                        return ((
                        force_consteval<kronecker_delta<kronecker_indices[i]>() && (repeats[i] > 0)> ?
                        product_of_elements<r_indices[i]>(R) * repeats[i] : 0
                        ) + ...);
                }(std::make_index_sequence<kronecker_indices.size()>());
                result += g3 * kronecker_product_term;
            }

            return result;
        } else if constexpr (N == 5) {
            auto result = g5 * cartesian_product_term;
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

    template<std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivatives(const Vector &R) {
        return [&]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
                //                        return Multipole3f<N>(derivative<i + 1>(R) ...);
                return std::make_tuple(derivative<i + 1>(R) ...);
        }(std::make_index_sequence<N>());
    };


}
