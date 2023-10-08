#ifndef SYMTENSOR_MULTIPOLEBASE_H
#define SYMTENSOR_MULTIPOLEBASE_H

#include <symtensor/ArithmeticTuple.h>
#include <symtensor/SymmetricTensor.h>
#include <symtensor/Index.h>
#include <symtensor/util.h>

namespace symtensor {

    namespace {

        template<std::size_t Order, typename ...PrefixTensors>
        struct TensorSequenceHelper;

        template<std::size_t Order, typename ...PrefixTensors> requires (last_type<PrefixTensors...>::Rank < Order)
        struct TensorSequenceHelper<Order, PrefixTensors...> {
            using type = typename TensorSequenceHelper<
                    Order,
                    PrefixTensors...,
                    NextHigherRank<last_type<PrefixTensors...>>
            >::type;
        };

        template<std::size_t Order, typename ...PrefixTensors> requires (last_type<PrefixTensors...>::Rank == Order)
        struct TensorSequenceHelper<Order, PrefixTensors...> {
            using type = ArithmeticTuple<PrefixTensors...>;
        };

        template<std::size_t Order, typename ...PrefixTensors>
        using TensorSequence = typename TensorSequenceHelper<Order, PrefixTensors...>::type;

    }

    template<class Implementation, std::size_t O, typename ...Tensors>
    class MultipoleBase : public TensorSequence<O, Tensors...> {
    public:

        static constexpr std::size_t Order = O;
    };

}

#endif //SYMTENSOR_MULTIPOLEBASE_H
