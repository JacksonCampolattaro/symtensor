/**
 * @file
 * @brief Provides the generic multipole type alongside typedefs for common uses.
 */
#ifndef SYMTENSOR_MULTIPOLEMOMENT_H
#define SYMTENSOR_MULTIPOLEMOMENT_H

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
    class MultipoleMoment : public MultipoleBaseFromTuple<
            MultipoleMoment<Order, Tensors...>,
            TensorSequence<Order, typename last_type<Tensors...>::Scalar, Tensors...>
    > {

        using Base = MultipoleBaseFromTuple<
                MultipoleMoment<Order, Tensors...>,
                TensorSequence<Order, typename last_type<Tensors...>::Scalar, Tensors...>
        >;

        using Self = MultipoleMoment;

        using Scalar = typename last_type<Tensors...>::Scalar;

    public:

        using Base::Base;

        template<typename S, indexable Vector>
        static inline constexpr MultipoleMoment FromPointMass(const S &mass, const Vector &position) {
            return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                return Self{
                        mass,
                        std::tuple_element_t<i + 1, typename Self::TensorTuple>::CartesianPower(position)...
                };
            }(std::make_index_sequence<Self::NumTensors - 1>());
        }

        inline constexpr const Scalar &scalar() const { return this->template tensor<0>(); }

        inline constexpr Scalar &scalar() { return this->template tensor<0>(); }

        template<typename T>
        inline constexpr Self &operator/=(const T &scalar) {
            [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                ((this->template tensor<i + 1>() /= scalar), ...);
            }(std::make_index_sequence<Self::NumTensors - 1>());
            return *this;
        }

        template<typename T>
        inline constexpr Self &operator*=(const T &scalar) {
            [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                ((this->template tensor<i + 1>() *= scalar), ...);
            }(std::make_index_sequence<Self::NumTensors - 1>());
            return *this;
        }


    };

    template<std::size_t Order>
    using MultipoleMoment2f = MultipoleMoment<Order, SymmetricTensor2f<1>>;

    template<std::size_t Order>
    using MultipoleMoment3f = MultipoleMoment<Order, SymmetricTensor3f<1>>;

    template<typename ...Tensors>
    class Quadrupole : public MultipoleMoment<2, Tensors...> {
        using Base = MultipoleMoment<2, Tensors...>;
    public:
        using Base::Base;
    };

    /// 2d quadrupole moment with floating point elements
    using QuadrupoleMoment2f = MultipoleMoment2f<2>;

    /// 3d quadrupole moment with floating point elements
    using QuadrupoleMoment3f = MultipoleMoment3f<2>;

    /// 2d octupole moment with floating point elements
    using OctupoleMoment2f = MultipoleMoment2f<3>;

    /// 3d octupole moment with floating point elements
    using OctupoleMoment3f = MultipoleMoment3f<3>;

    /// 2d hexadecupole moment with floating point elements
    using HexadecupoleMoment2f = MultipoleMoment2f<4>;

    /// 3d hexadecupole moment with floating point elements
    using HexadecupoleMoment3f = MultipoleMoment3f<4>;

    /// 2d triacontadyupole moment with floating point elements
    using TriacontadyupoleMoment2f = MultipoleMoment2f<5>;

    /// 3d triacontadyupole moment with floating point elements
    using TriacontadyupoleMoment3f = MultipoleMoment3f<5>;

}

#endif //SYMTENSOR_MULTIPOLEMOMENT_H
