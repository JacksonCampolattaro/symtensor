#ifndef SYMTENSOR_MULTIPOLEBASE_H
#define SYMTENSOR_MULTIPOLEBASE_H

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
    class MultipoleBase;

    template<class Implementation, typename ...Tensors>
    class MultipoleBase<Implementation, std::tuple<Tensors...>> {

        using Self = MultipoleBase<Implementation, std::tuple<Tensors...>>;
        using TensorTuple = std::tuple<Tensors...>;

        TensorTuple _tuple;

        const Implementation &implementation() const { return *static_cast<const Implementation *>(this); }

        Implementation &implementation() { return *static_cast<Implementation *>(this); }

    public:

        static constexpr std::size_t Order = last_type_of_tuple<TensorTuple>::Rank;

        inline constexpr MultipoleBase() = default;

        inline constexpr MultipoleBase(const Implementation &other) : _tuple(other._tuple) {};

        inline constexpr MultipoleBase(Tensors &&...types) : _tuple(std::forward<Tensors>(types)...) {}

        inline constexpr Implementation &operator=(Implementation rhs) {
            std::swap(*this, rhs);
            return *this;
        }

        inline constexpr const TensorTuple &underlying_tuple() const { return _tuple; }

        inline constexpr TensorTuple &underlying_tuple() { return _tuple; }

        template<std::size_t R>
        auto &tensor() {
            return std::get<indexForRank<R>()>(this->underlying_tuple());
        }

        template<std::size_t R>
        const auto &tensor() const {
            return std::get<indexForRank<R>()>(this->underlying_tuple());
        }

    public: // Multipole-scalar operations

        template<typename T>
        inline constexpr Implementation &operator+=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements += scalar), ...);
            }, underlying_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator-=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements -= scalar), ...);
            }, underlying_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator*=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements *= scalar), ...);
            }, underlying_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator/=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements /= scalar), ...);
            }, underlying_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation operator+(const T &scalar) const {
            return Implementation{implementation()} += scalar;
        }

        template<typename T>
        inline constexpr Implementation operator-(const T &scalar) const {
            return Implementation{implementation()} -= scalar;
        }

        template<typename T>
        inline constexpr Implementation operator*(const T &scalar) const {
            return Implementation{implementation()} *= scalar;
        }

        template<typename T>
        inline constexpr Implementation operator/(const T &scalar) const {
            return Implementation{implementation()} /= scalar;
        }

    public: // Tuple-tuple operations

        inline constexpr Implementation &operator+=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(underlying_tuple()) += std::get<i>(other.underlying_tuple())), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator-=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(underlying_tuple()) -= std::get<i>(other.underlying_tuple())), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator*=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(underlying_tuple()) *= std::get<i>(other.underlying_tuple())), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator/=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(underlying_tuple()) /= std::get<i>(other.underlying_tuple())), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation operator+(const Implementation &other) const {
            return Implementation{implementation()} += other;
        }

        inline constexpr Implementation operator-(const Implementation &other) const {
            return Implementation{implementation()} -= other;
        }

        inline constexpr Implementation operator*(const Implementation &other) const {
            return Implementation{implementation()} *= other;
        }

        inline constexpr Implementation operator/(const Implementation &other) const {
            return Implementation{implementation()} /= other;
        }

    public:

        friend constexpr auto operator<=>(const Self &, const Self &) = default;

        friend constexpr auto operator<=>(const Implementation &a, const Implementation &b) {
            return operator<=>(static_cast<const Self &>(a), static_cast<const Self &>(b));
        };

        friend std::ostream &operator<<(std::ostream &out, const Self &self) {
            std::apply([&](const Tensors &... tupleElements) {
                ((out << tupleElements << " "), ...);
            }, self.underlying_tuple());
            return out;
        }

    private:

        template<std::size_t R>
        static consteval std::size_t indexForRank() {
            return (underlying_tuple_size<TensorTuple>() - 1) - (Order - R);
        }
    };

}

#endif //SYMTENSOR_MULTIPOLEBASE_H
