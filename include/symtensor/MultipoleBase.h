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
     * you can derive from this class so that operators will function correctly.
     *
     * @tparam Implementation CRTP subclass
     * @tparam Tensors sequence of tensors which make up the multipole.
     *   The order of the multipole is equivalent to the rank of the last tensor.
     *   The tensors should be provided in ascending order and without gaps.
     */
    template<class Implementation, typename ...Tensors>
    class MultipoleBase {
    public:

        using TensorTuple = std::tuple<Tensors...>;

        static constexpr std::size_t Order = last_type_of_tuple<TensorTuple>::Rank;
        static constexpr std::size_t NumTensors = std::tuple_size_v<TensorTuple>;

    private:

        TensorTuple _tuple;

        using Self = MultipoleBase<Implementation, Tensors...>;

    public:
        /// @name Constructors
        /// @{

        /**
         * @brief Default constructor.
         *
         * Invokes the default constructor for all contained tensors.
         */
        inline constexpr MultipoleBase() = default;

        /**
         * @brief Constructor from a sequence of tensors
         *
         * @param types The values for each tensor contained in the multipole.
         *   So long as each tensor type has an implicit constructor, initializer-list syntax works.
         */
        inline constexpr MultipoleBase(Tensors &&...types) : _tuple(std::forward<Tensors>(types)...) {}

        /// @}
    public:
        /// @name Member access
        /// @{

        /**
         * @brief Tensor access by rank
         *
         * @tparam R Rank of the tensor to retrieve
         * @return reference to the tensor with the requested rank
         */
        template<std::size_t R>
        inline constexpr const auto &tensor() const {
            return std::get<indexForRank(R)>(_tuple);
        }

        /// @copydoc tensor() const
        template<std::size_t R>
        inline constexpr auto &tensor() {
            return std::get<indexForRank(R)>(_tuple);
        }

        /// @}
    public:
        /// @name Multipole-scalar operations
        /// @{

        /**
         * @brief Element-wise addition of a scalar
         *
         * @param scalar value to add to each tensor of the multipole
         * @return the modified multipole
         */
        template<typename T>
        inline constexpr Implementation &operator+=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements += scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise subtraction of a scalar
         *
         * @param scalar value to subtract from each tensor of the multipole
         * @return the modified multipole
         */
        template<typename T>
        inline constexpr Implementation &operator-=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements -= scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise multiplication by a scalar
         *
         * @param scalar value to multiply each tensor of the multipole by.
         * @return the modified multipole
         */
        template<typename T>
        inline constexpr Implementation &operator*=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements *= scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise division by a scalar
         *
         * @param scalar value to divide each element of the multipole by.
         * @return the modified multipole
         */
        template<typename T>
        inline constexpr Implementation &operator/=(const T &scalar) {
            std::apply([&](Tensors &... tupleElements) {
                ((tupleElements /= scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        /// @copydoc operator+=(const T &)
        template<typename T>
        inline constexpr Implementation operator+(const T &scalar) const {
            return Implementation{implementation()} += scalar;
        }

        /// @copydoc operator-=(const T &)
        template<typename T>
        inline constexpr Implementation operator-(const T &scalar) const {
            return Implementation{implementation()} -= scalar;
        }

        /// @copydoc operator*=(const T &)
        template<typename T>
        inline constexpr Implementation operator*(const T &scalar) const {
            return Implementation{implementation()} *= scalar;
        }

        /// @copydoc operator/=(const T &)
        template<typename T>
        inline constexpr Implementation operator/(const T &scalar) const {
            return Implementation{implementation()} /= scalar;
        }

        /// @}
    public:
        /// @name Tuple-tuple operations
        /// @{

        inline constexpr Implementation &operator+=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) += std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator-=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) -= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator*=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) *= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator/=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) /= std::get<i>(other._tuple)), ...);
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

        /// @}
    public:

        friend constexpr auto operator<=>(const Self &a, const Self &b) = default;

        friend std::ostream &operator<<(std::ostream &out, const Self &self) {
            std::apply([&](const Tensors &... tupleElements) {
                ((out << tupleElements << " "), ...);
            }, self._tuple);
            return out;
        }

    public:

        inline constexpr const TensorTuple &underlying_tuple() const { return _tuple; }

        inline constexpr TensorTuple &underlying_tuple() { return _tuple; }

        template<std::size_t I>
        inline constexpr auto &get() &{ return std::get<I>(_tuple); }

        template<std::size_t I>
        inline constexpr const auto &get() const &{ return std::get<I>(_tuple); }

        template<std::size_t I>
        inline constexpr auto &&get() &&{ return std::get<I>(_tuple); }


    private:

        static consteval std::size_t indexForRank(std::size_t R) {
            return (NumTensors - 1) - (Order - R);
        }

        const Implementation &implementation() const { return *static_cast<const Implementation *>(this); }

        Implementation &implementation() { return *static_cast<Implementation *>(this); }

    };

    template<typename Implementation, typename Tuple>
    using MultipoleBaseFromTuple = expand_tuple<MultipoleBase, Tuple, Implementation>;
}

namespace std {

    template<symtensor::derived_from_template<symtensor::MultipoleBase> T>
    struct tuple_size<T> : tuple_size<typename T::TensorTuple> {
    };

    template<std::size_t I, symtensor::derived_from_template<symtensor::MultipoleBase> T>
    struct tuple_element<I, T> : tuple_element<I, typename T::TensorTuple> {
    };
}

#endif //SYMTENSOR_MULTIPOLEBASE_H
