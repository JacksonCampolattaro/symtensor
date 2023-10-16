/**
 * @file
 * @brief Provides the generic symmetric tensor type alongside typedefs for common uses.
 */
#ifndef SYMTENSOR_SYMMETRICTENSOR_H
#define SYMTENSOR_SYMMETRICTENSOR_H

#include <symtensor/SymmetricTensorBase.h>

namespace symtensor {

    /**
     * @brief Generic symmetric tensor type
     *
     * @tparam S scalar type
     * @tparam D number of dimensions (2d, 3d, etc.)
     * @tparam R rank
     * @tparam I index type, defaults to the appropriate @ref symtensor::Index
     */
    template<typename S, std::size_t D, std::size_t R, typename I = Index<D>>
    class SymmetricTensor : public SymmetricTensorBase<SymmetricTensor<S, D, R, I>, S, D, R, I> {
        using Base = SymmetricTensorBase<SymmetricTensor<S, D, R, I>, S, D, R, I>;
    public:
        using Base::Base;
    };

    template<std::size_t R>
    using SymmetricTensor2f = SymmetricTensor<float, 2, R>;

    template<std::size_t R>
    using SymmetricTensor3f = SymmetricTensor<float, 3, R>;

}

#endif //SYMTENSOR_SYMMETRICTENSOR_H
