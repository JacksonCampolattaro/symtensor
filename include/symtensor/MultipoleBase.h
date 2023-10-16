/**
 * @file
 * @brief Provides the base multipole template.
 */
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

    /**
     * @brief Produces an std::tuple of tensors based on a "prefix" and a desired order.
     *
     * The prefix determines the types of the tensors which make up the sequence.
     * For example, the following
     * @code{.cpp} TensorSequence<3, SymmetricTensor3f<1>> @endcode
     * produces the type:
     * @code{.cpp}
     * std::tuple<
     *  SymmetricTensor3f<1>,
     *  SymmetricTensor3f<2>,
     *  SymmetricTensor3f<3>
     * >
     * @endcode
     *
     * The prefix may also contain types which are not symmetric tensors,
     * so long as the final type of the prefix implements @ref SymmetricTensorBase.
     * For example,
     * @code{.cpp} TensorSequence<3, float, glm::vec3, SymmetricTensor3f<2>> @endcode
     * uses a `float` scalar component and a vector from GLM, this produces the type:
     * @code{.cpp}
     * std::tuple<
     *  float,
     *  glm::vec3,
     *  SymmetricTensor3f<2>,
     *  SymmetricTensor3f<3>
     * >
     * @endcode
     *
     * @tparam Order rank of the highest tensor of the sequence, which appears last in the std::tuple
     * @tparam PrefixTensors at least one low-order tensor which will be used to define higher order tensors.
     *   The last tensor of PrefixTensors is used to produce higher order tensors
     *   through repeated use of @ref NextHigherRank.
     */
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
        /// @name Multipole-multipole operations
        /// @{

        /**
         * @brief Element-wise addition with another multipole
         *
         * @param other multipole to add with this multipole
         * @return the modified multipole
         */
        inline constexpr Implementation &operator+=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) += std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise subtraction by another multipole
         *
         * @param other multipole to subtract from this multipole
         * @return the modified multipole
         */
        inline constexpr Implementation &operator-=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) -= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise multiplication by another multipole
         *
         * @param other multipole to multiply with this multipole
         * @return the modified multipole
         */
        inline constexpr Implementation &operator*=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) *= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise division by another multipole
         *
         * @param other multipole to divide this multipole by
         * @return the modified multipole
         */
        inline constexpr Implementation &operator/=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) /= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<TensorTuple>>{});
            return *static_cast<Implementation *>(this);
        }

        /// @copydoc operator+=(const Implementation &)
        inline constexpr Implementation operator+(const Implementation &other) const {
            return Implementation{implementation()} += other;
        }

        /// @copydoc operator-=(const Implementation &)
        inline constexpr Implementation operator-(const Implementation &other) const {
            return Implementation{implementation()} -= other;
        }

        /// @copydoc operator*=(const Implementation &)
        inline constexpr Implementation operator*(const Implementation &other) const {
            return Implementation{implementation()} *= other;
        }

        /// @copydoc operator/=(const Implementation &)
        inline constexpr Implementation operator/(const Implementation &other) const {
            return Implementation{implementation()} /= other;
        }

        /// @}
    public:
        /// @name Comparison operators
        /// @{

        /**
         * @brief Comparison with another multipole
         *
         * @param other multipole to compare with
         * @return true if all elements of the multipoles are equivalent, false otherwise
         */
        inline constexpr bool operator==(const Self &other) const = default;

        /// @}
    public:

        friend std::ostream &operator<<(std::ostream &out, const Self &self) {
            std::apply([&](const Tensors &... tupleElements) {
                ((out << tupleElements << " "), ...);
            }, self._tuple);
            return out;
        }

    public:
        /// @name std::tuple compatibility
        /// @{

        /**
         * @brief direct access to the tuple of tensors which comprises the multipole
         * @return reference to the std::tuple of tensors.
         */
        inline constexpr const TensorTuple &underlying_tuple() const { return _tuple; }

        /// @copydoc underlying_tuple()
        inline constexpr TensorTuple &underlying_tuple() { return _tuple; }

        /**
         * @brief element access for use with structured bindings
         *
         * @tparam I index of the element to retrieve (not necessarily the same as Rank)
         * @return reference to the tensor
         */
        template<std::size_t I>
        inline constexpr auto &get() &{ return std::get<I>(_tuple); }

        /// @copydoc get()
        template<std::size_t I>
        inline constexpr const auto &get() const &{ return std::get<I>(_tuple); }

        /// @copydoc get()
        template<std::size_t I>
        inline constexpr auto &&get() &&{ return std::get<I>(_tuple); }

        /// @}


    private:

        static consteval std::size_t indexForRank(std::size_t R) {
            return (NumTensors - 1) - (Order - R);
        }

        const Implementation &implementation() const { return *static_cast<const Implementation *>(this); }

        Implementation &implementation() { return *static_cast<Implementation *>(this); }

    };

    //    template<typename Implementation, typename Tuple>
    //    using MultipoleBaseFromTuple = expand_tuple<MultipoleBase, Tuple, Implementation>;

    /**
     * @brief Helper class for defining a multipole based on an std::tuple of tensors.
     *
     * @tparam Implementation CRTP subclass
     * @tparam Tuple the std::tuple of tensors.
     *   This is typically produced using @ref TensorSequence
     */
    template<typename Implementation, typename Tuple>
    class MultipoleBaseFromTuple : public MultipoleBase<Implementation, Tuple> {
    };

    template<typename Implementation, typename... Tensors>
    class MultipoleBaseFromTuple<Implementation, std::tuple<Tensors...>>
            : public MultipoleBase<Implementation, Tensors...> {
    public:
        using MultipoleBase<Implementation, Tensors...>::MultipoleBase;
    };

}

namespace std {

    /**
     * @brief tuple-size specialization to enable the use of structured bindings with multipoles
     *
     * @tparam T multipole type.
     *  must be derived from a specialization of @ref MultipoleBase, enforced by a concept
     */
    template<symtensor::derived_from_template<symtensor::MultipoleBase> T>
    struct tuple_size<T> : tuple_size<typename T::TensorTuple> {
    };

    /**
     * @brief tuple-element specialization to enable the use of structured bindings with multipoles
     *
     * @tparam I index of the element to retrieve the type of
     * @tparam T multipole type.
     *  must be derived from a specialization of @ref MultipoleBase, enforced by a concept
     */
    template<std::size_t I, symtensor::derived_from_template<symtensor::MultipoleBase> T>
    struct tuple_element<I, T> : tuple_element<I, typename T::TensorTuple> {
    };
}

#endif //SYMTENSOR_MULTIPOLEBASE_H
