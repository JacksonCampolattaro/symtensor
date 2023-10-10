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
            using type = std::tuple<PrefixTensors...>;
        };

    }

    template<std::size_t Order, typename ...PrefixTensors>
    using TensorSequence = typename TensorSequenceHelper<Order, PrefixTensors...>::type;

    /**
     * @brief Multipole base-type for use with CRTP implementations.
     *
     * In order to create a new type which behaves like a multipole,
     * you can subclass from this class so that operators will function correctly.
     *
     * @tparam Implementation CRTP subclass
     * @tparam TensorTuple sequence of tensors which make up the multipole.
     *   The order of the multipole is equivalent to the rank of the last tensor.
     *   The tensors should be provided in ascending order and without gaps.
     */
    template<class Implementation, typename TensorTuple>
    class MultipoleBase : public AsArithmeticTupleBase<Implementation, TensorTuple> {
    public:

        using Self = Implementation;

        static constexpr std::size_t Order = last_type_of_tuple<TensorTuple>::Rank;

        MultipoleBase() = default;

        template<std::size_t R>
        auto &tensor() {
            return get<indexForRank<R>()>(*this);
        }

        template<std::size_t R>
        const auto &tensor() const {
            return get<indexForRank<R>()>(*this);
        }

    private:

        template<std::size_t R>
        static consteval std::size_t indexForRank() {
            return (std::tuple_size_v<Self> - 1) - (Order - R);
        }
    };

}

#endif //SYMTENSOR_MULTIPOLEBASE_H
