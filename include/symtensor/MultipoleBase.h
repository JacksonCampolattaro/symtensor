#ifndef SYMTENSOR_MULTIPOLEBASE_H
#define SYMTENSOR_MULTIPOLEBASE_H

#include <symtensor/ArithmeticTuple.h>
#include <symtensor/SymmetricTensor.h>
#include <symtensor/Index.h>
#include <symtensor/util.h>

namespace symtensor {

    namespace {

        template<std::size_t Order, typename ...PrefixTensors>
        consteval auto get_tensor_sequence() {
            using HighestRankTensor = last_type<PrefixTensors...>;
            if constexpr (HighestRankTensor::Rank < Order)
                return get_tensor_sequence<Order, PrefixTensors..., NextHigherRank<HighestRankTensor>>();
            else
                return std::declval<ArithmeticTuple<PrefixTensors...>>();
        }

        template<std::size_t Order, typename ...PrefixTensors>
        using TensorSequence = decltype(get_tensor_sequence<Order, PrefixTensors...>());

    }

    template<class Implementation, std::size_t O, typename ...Tensors>
    class MultipoleBase : public TensorSequence<O, Tensors...> {
    public:

        static constexpr std::size_t Order = O;
    };

}

#endif //SYMTENSOR_MULTIPOLEBASE_H
