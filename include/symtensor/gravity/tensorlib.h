#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include "symtensor/symtensor.h"
#include "symtensor/util.h"

#include "tensorlib_autogen.h"
#include "symtensor/MultipoleMoment.h"

namespace symtensor::gravity::tensorlib {

    ALWAYS_INLINE auto derivatives5(const glm::vec3 &R) {
        // todo: Not a fan of this.
        float d0{};
        a_3 point{R.x, R.y, R.z};
        auto d = Multipole3f<5>{};
        auto &[st1, st2, st3, st4, st5] = d;
        get_multipole_derivatives(
                d0,
                reinterpret_cast<A_3 &>(st1),
                reinterpret_cast<AA_3 &>(st2),
                reinterpret_cast<AAA_3 &>(st3),
                reinterpret_cast<AAAA_3 &>(st4),
                reinterpret_cast<AAAAA_3 &>(st5),
                point
        );
        return d;
    }

    template<std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivative(const Vector &R) {
        return derivatives5(R).template tensor<N>();
    };

    template<std::size_t N, indexable Vector>
    ALWAYS_INLINE auto derivatives(const Vector &R) {
        auto m = Multipole3f<N>{};
        return [&]<auto... i>(std::index_sequence<i...>) LAMBDA_ALWAYS_INLINE {
            return Multipole3f<N>(derivative<i + 1>(R) ...);
        }(std::make_index_sequence<N>());
    };
}
