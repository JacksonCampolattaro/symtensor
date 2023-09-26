#ifndef SYMTENSOR_GLM_H
#define SYMTENSOR_GLM_H

#include <symtensor/SymmetricTensorBase.h>

#include <glm/matrix.hpp>

namespace symtensor {

    template<class Implementation, typename Scalar, std::size_t D, typename I>
    constexpr static inline glm::mat<int(D), int(D), Scalar>
    to_glm(const symtensor::SymmetricTensorBase<Implementation, Scalar, D, 2, I> &symmetricMatrix) {

        using SymmetricMatrix = symtensor::SymmetricTensorBase<Implementation, Scalar, D, 2, I>;
        glm::mat<D, D, Scalar> result{};
        for (int i = 0; i < SymmetricMatrix::NumValues; ++i) {
            auto index2d = SymmetricMatrix::lexicographicalIndices(i);
            result[static_cast<size_t>(index2d[0])][static_cast<size_t>(index2d[1])] = symmetricMatrix[index2d];
        }
        return result;
    }

}

#endif //SYMTENSOR_GLM_H
