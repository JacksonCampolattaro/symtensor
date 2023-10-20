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
    class Multipole : public MultipoleBaseFromTuple<
            Multipole<Order, Tensors...>,
            TensorSequence<Order, Tensors...>
    > {

        using Base = MultipoleBaseFromTuple<
                Multipole<Order, Tensors...>,
                TensorSequence<Order, Tensors...>
        >;

    public:

        using Base::Base;
    };

    template<std::size_t Order>
    using Multipole2f = Multipole<Order, SymmetricTensor2f<1>>;

    template<std::size_t Order>
    using Multipole3f = Multipole<Order, SymmetricTensor3f<1>>;

    template<typename ...Tensors>
    class Quadrupole : public Multipole<2, Tensors...> {
        using Base = Multipole<2, Tensors...>;
    public:
        using Base::Base;
    };

    /// 2d quadrupole with floating point elements
    using Quadrupole2f = Multipole2f<2>;

    /// 3d quadrupole with floating point elements
    using Quadrupole3f = Multipole3f<2>;

    /// 2d octupole with floating point elements
    using Octupole2f = Multipole2f<3>;

    /// 3d octupole with floating point elements
    using Octupole3f = Multipole3f<3>;

    /// 2d hexadecupole with floating point elements
    using Hexadecupole2f = Multipole2f<4>;

    /// 3d hexadecupole with floating point elements
    using Hexadecupole3f = Multipole3f<4>;

    /// 2d triacontadyupole with floating point elements
    using Triacontadyupole2f = Multipole2f<5>;

    /// 3d triacontadyupole with floating point elements
    using Triacontadyupole3f = Multipole3f<5>;

}

#endif //SYMTENSOR_MULTIPOLE_H
