/**
 * @file
 * @brief Provides the generic multipole type alongside typedefs for common uses.
 */
#ifndef SYMTENSOR_MULTIPOLE_H
#define SYMTENSOR_MULTIPOLE_H

#include <symtensor/MultipoleBase.h>

namespace symtensor {


    /**
     * @brief Generic multipole type
     *
     * @tparam Order the rank of the highest symmetric tensor
     * @tparam Tensors prefix of types which make up the lowest ranks of the multipole.
     *  Must end in a Symmetric tensor -- the final type is "promoted" to produce higher rank tensors
     *  until the requested Order is reached.
     */
    template<std::size_t Order, typename ...Tensors>
    class Multipole : public MultipoleBase<Multipole<Order, Tensors...>, TensorSequence<Order, Tensors...>> {
    };

    template<std::size_t Order>
    using Multipole2f = Multipole<Order, SymmetricTensor2f<1>>;

    template<std::size_t Order>
    using Multipole3f = Multipole<Order, SymmetricTensor3f<1>>;

    template<typename ...Tensors>
    class Quadrupole : public Multipole<2, Tensors...> {
    };

    using Quadrupole2f = Multipole2f<2>;
    using Quadrupole3f = Multipole3f<2>;

}

#endif //SYMTENSOR_MULTIPOLE_H
