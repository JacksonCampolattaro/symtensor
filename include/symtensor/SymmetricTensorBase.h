/**
 * @file
 * @brief Provides the base symmetric tensor template.
 */
#ifndef SYMTENSOR_SYMMETRICTENSORBASE_H
#define SYMTENSOR_SYMMETRICTENSORBASE_H

#include "util.h"
#include "concepts.h"
#include "Index.h"

#include <array>
#include <iterator>

namespace symtensor {

    namespace {

        // todo: these should be made public, so that users can specialize them for their own types

        template<typename, class>
        struct ReplaceScalarHelper {
        };
        template<
                typename NewScalar,
                template<typename, std::size_t, std::size_t, typename> class ST,
                typename OldScalar, std::size_t D, std::size_t R, typename I
        >
        struct ReplaceScalarHelper<NewScalar, ST<OldScalar, D, R, I>> {
            using type = ST<NewScalar, D, R, I>;
        };

        template<std::size_t, class>
        struct ReplaceRankHelper {
        };
        template<
                std::size_t NewRank,
                template<typename, std::size_t, std::size_t, typename> class ST,
                typename S, std::size_t D, std::size_t OldRank, typename I
        >
        struct ReplaceRankHelper<NewRank, ST<S, D, OldRank, I>> {
            using type = ST<S, D, NewRank, I>;
        };

    }

    template<class ST, typename S>
    using ReplaceScalar = typename ReplaceScalarHelper<S, ST>::type;

    /**
     * @brief Produces a type which modifies the rank of a symmetric tensor
     *
     * All properties of the symmetric tensor ST are preserved aside from the rank.
     * The type of
     * @code{.cpp}
     * ReplaceRank<SymmetricTensor3f<3>, 5>
     * @endcode
     * is equivalent to
     * @code{.cpp}
     * SymmetricTensor3f<5>
     * @endcode.
     *
     * @tparam ST symmetric tensor type to replace the rank of
     * @tparam R the new rank
     */
    template<class ST, std::size_t R>
    using ReplaceRank = typename ReplaceRankHelper<R, ST>::type;

    /**
     * @brief Given a symmetric tensor type, finds the symmetric tensor of the next higher rank
     *
     * All properties of the symmetric tensor ST are preserved aside from the rank.
     * The type of
     * @code{.cpp}
     * NextHigherRank<SymmetricTensor3f<3>>
     * @endcode
     * is equivalent to
     * @code{.cpp}
     * SymmetricTensor3f<4>
     * @endcode.
     *
     * @tparam ST a symmetric tensor
     */
    template<class ST>
    using NextHigherRank = ReplaceRank<ST, ST::Rank + 1>;

    /**
     * @brief Given a symmetric tensor type, finds the symmetric tensor of the next lower rank
     *
     * All properties of the symmetric tensor ST are preserved aside from the rank.
     * The type of
     * @code{.cpp}
     * NextHigherRank<SymmetricTensor3f<3>>
     * @endcode
     * is equivalent to
     * @code{.cpp}
     * SymmetricTensor3f<2>
     * @endcode.
     *
     * @tparam ST a symmetric tensor
     */
    template<class ST>
    using NextLowerRank = ReplaceRank<ST, ST::Rank - 1>;

    template<class ST>
    static consteval std::size_t Rank() {
        if constexpr (requires { ST::Rank; })
            return ST::Rank;
        else if constexpr (indexable<ST>)
            return 1;
        else
            return 0;
    }

    /**
     * @brief Symmetric tensor base-type for use with CRTP implementations.
     *
     * In order to create a new type which behaves like a symmetric tensor,
     * you can derive from this class so that operators will function correctly.
     *
     * @tparam Implementation CRTP subclass
     * @tparam S scalar type
     * @tparam D number of dimensions (2d, 3d, etc.)
     * @tparam R rank
     * @tparam I index type, defaults to the appropriate @ref Index
     */
    template<class Implementation, typename S, std::size_t D, std::size_t R, typename I = Index<D>>
    class SymmetricTensorBase {
    public:

        using Self = SymmetricTensorBase<Implementation, S, D, R, I>;

        using Scalar = S;
        static constexpr std::size_t Dimensions = D;
        static constexpr std::size_t Rank = R;

        static constexpr std::size_t NumValues = numValuesInTensor(D, R);
        static constexpr std::size_t NumUniqueValues = numUniqueValuesInSymmetricTensor(D, R);

        using Index = I;

    public:

        // todo: this could provided by a CRTP type
        std::array<S, NumUniqueValues> _data{0};

    public:
        /// @name Constructors
        /// @{

        /**
         * @brief Default constructor.
         *
         * Initializes all indices to 0.
         */
        explicit constexpr SymmetricTensorBase() : _data{0} {}

        constexpr SymmetricTensorBase(const Self &other) = default;

        /**
         * @brief Constructor from a sequence of values.
         *
         * @param s a sequence of scalar values to initialize the tensor.
         */
        constexpr SymmetricTensorBase(auto ...s) : _data{static_cast<S>(s)...} {
            static_assert(sizeof...(s) == NumUniqueValues);
        }

        // todo: this might perform better in some cases
        //        constexpr SymmetricTensorBase(std::initializer_list<Scalar> scalars) {
        //            assert(scalars.size() == NumUniqueValues);
        //            std::copy(scalars.begin(), scalars.end(), _data.begin());
        //        }

        constexpr Self &operator=(const Self &other) = default;

        constexpr Self &operator=(Self &&other) noexcept = default;

        /**
         * @brief Identity matrix constructor.
         *
         * @return a symmetric tensor with a value of 1 along the diagonal, 0 elsewhere.
         */
        inline static consteval Implementation Identity() {
            return NullaryExpression([]<auto ...indices>() consteval { return kroneckerDelta(indices...); });
        }

        /**
         * @brief Unit matrix constructor.
         *
         * @return a symmetric tensor with a value of 1 at every index.
         */
        inline static consteval Implementation Ones() {
            return NullaryExpression([]<auto ...>() consteval { return Scalar{1}; });
        }

        /**
         * @brief Zero matrix constructor.
         *
         * @return a symmetric tensor with a value of 0 at every index.
         */
        inline static consteval Implementation Zeros() {
            return NullaryExpression([]<auto ...>() { return Scalar{0}; });
        }

        /**
         * @brief Generic constructor from an expression.
         *
         * @param function a functor which returns a scalar type given an index
         *
         * @return a symmetric tensor with each value set by evaluating the function.
         */
        template<typename F>
        inline static constexpr Implementation NullaryExpression(F function = {}) {
            if constexpr (requires { function.template operator()<dimensionalIndices(0)>(); }) {
                // If a function provides a template parameter for compile-time indexing, prefer that
                return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    return Implementation{static_cast<S>(function.template operator()<dimensionalIndices(i)>())...};
                }(std::make_index_sequence<NumUniqueValues>());
            } else {
                // Otherwise, the function must take the indices as its only argument
                return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    return Implementation{static_cast<S>(function(dimensionalIndices(i)))...};
                }(std::make_index_sequence<NumUniqueValues>());
            }
        }

        template<typename F>
        inline static constexpr Implementation LexicographicalNullaryExpression(F function = {}) {
            // todo: There may be cases where this provides a performance benefit
            Self tensor;
            if constexpr (requires { function.template operator()<dimensionalIndices(0)>(); }) {
                // If a function provides a template parameter for compile-time indexing, prefer that
                [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    ((tensor.at<lexicographicalIndices(i)>()
                              = function.template operator()<lexicographicalIndices(i)>()), ...);
                }(std::make_index_sequence<NumValues>());
            } else {
                // Otherwise, the function must take the indices as its only argument
                [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    ((tensor.at<lexicographicalIndices(i)>()
                              = function(lexicographicalIndices(i))), ...);
                }(std::make_index_sequence<NumValues>());
            }
            return static_cast<Implementation>(tensor);
        }


        /**
         * @brief Constructor by repeated cartesian product.
         *
         * @tparam Vector vector-like type which provides a subscript operator, with size D
         *
         * @param vector vector which will be multiplied with itself
         *
         * @return a symmetric tensor produced by the repeated outer product of vector.
         */
        template<typename Vector>
        inline static constexpr Implementation CartesianPower(const Vector &vector) {
            return NullaryExpression([&]<std::array<I, Rank> index>() constexpr {
                return [&]<std::size_t... r>(std::index_sequence<r...>) constexpr {
                    return (vector[static_cast<std::size_t>(index[r])] * ...);
                }(std::make_index_sequence<Rank>());
            });
        }

        template<typename Tensor, typename Vector>
        inline static constexpr Implementation CartesianProduct(
                const Tensor &tensor,
                const Vector &vector
        ) {

            static_assert(requires { Tensor::Rank; } && Tensor::Rank == Rank - 1 || Rank == 2,
                          "CartesianProduct() is only defined for Tensor types with Rank one lower than this tensor");

            if constexpr (requires { Tensor::Rank; } && Tensor::Rank == Rank - 1) {

                // Promoting a tensor of Rank-1 to Rank by cartesian product
                return NullaryExpression([&]<std::array<I, Rank> index>() constexpr {
                    return vector[static_cast<std::size_t>(index[0])] *
                           tensor.template at<tail(index)>();
                });

            } else {

                // Producing a rank-2 tensor by the cartesian product of 2 vectors
                static_assert(Rank == 2);
                return NullaryExpression([&]<std::array<I, Rank> index>() constexpr {
                    return tensor[static_cast<std::size_t>(index[0])] *
                           vector[static_cast<std::size_t>(index[1])];
                });

            }
        }

        /**
         * @brief Constructor with a set vector as the diagonal.
         *
         * @tparam Vector vector-like type which provides a subscript operator, with size D
         *
         * @param vector vector which will define the diagonal of the tensor
         *
         * @return a symmetric tensor with diagonal equal to vector, zero elsewhere.
         */
        template<typename Vector>
        inline static constexpr Implementation Diagonal(const Vector &vector) {
            // todo: add a unit test for this
            Implementation tensor;
            [&]<std::size_t... d>(std::index_sequence<d...>) constexpr {
                ((tensor.template at<repeat<R>(static_cast<I>(d))>() = vector[d]), ...);
            }(std::make_index_sequence<Dimensions>());
            return tensor;
        }


        /// @}
    public:
        /// @name Member access
        /// @{

        /**
         * @brief Indexed member access
         *
         * @warning This method is provided only for the sake of completeness, expect poor performance.
         *
         * @param indices Array of R Index values which specify an element of the tensor.
         *  Indices may be in arbitrary order, because the tensor is symmetric.
         *
         * @return the scalar element at the requested index
         */
        inline constexpr const Scalar &operator[](const std::array<Index, R> &indices) const {
            return _data[flatIndex(indices)];
        }

        /**
         * @copydoc operator[](const std::array<Index, R> &) const
         */
        inline constexpr Scalar &operator[](const std::array<Index, R> &indices) {
            return _data[flatIndex(indices)];
        }

        /**
         * @brief Compile-time indexed member access
         *
         * The index sequence is converted to a flat index at compile-time,
         * allowing for performance equivalent to hand-written member access.
         * @code{.cpp}mytensor.at<X, X, X>()@endcode should be equivalent to
         * @code{.cpp}mytensor.flat()[0]@endcode.
         *
         * @tparam Indices Sequence of R Index values which specify an element of the tensor.
         *  Indices may be in arbitrary order, because the tensor is symmetric.
         *
         * @return the scalar element at the requested index
         */
        template<Index... Indices>
        inline constexpr const Scalar &at() const {
            return _data[flatIndex<std::array<I, R>{static_cast<Index>(Indices)...}>()];
        }

        /**
         * @copydoc at() const
         */
        template<Index... Indices>
        inline constexpr Scalar &at() {
            return _data[flatIndex<std::array<I, R>{static_cast<Index>(Indices)...}>()];
        }

        /**
         * @brief Direct access to underlying data structure
         *
         * @note No guarantees are provided about the definition and layout of the flat data structure,
         *  except for the fact that it is an iterable with a `size()`, `begin()` and `end()` method.
         *  The elements of the symmetric tensor may be rearranged in order to
         *  facilitate vectorized operations, for the best possible performance.
         *
         * @return a reference to the underlying flat storage.
         */
        inline constexpr const auto &flat() const { return _data; }

        /**
         * @copydoc flat() const
         */
        inline constexpr auto &flat() { return _data; }

        template<std::array<I, R> Indices>
        inline constexpr const Scalar &at() const {
            return _data[flatIndex<Indices>()];
        }

        template<std::array<I, R> Indices>
        inline constexpr Scalar &at() {
            return _data[flatIndex<Indices>()];
        }

        inline constexpr const Scalar &operator[](auto flatIndex) const {
            static_assert(std::is_integral_v<decltype(flatIndex)>);
            return _data[static_cast<std::size_t>(flatIndex)];
        }

        inline constexpr Scalar &operator[](auto flatIndex) {
            static_assert(std::is_integral_v<decltype(flatIndex)>);
            return _data[static_cast<std::size_t>(flatIndex)];
        }

        /// @}
    public:
        /// @name Tensor properties
        /// @{

        /**
         * @brief Computes the trace of the tensor
         *
         * This is equivalent to the sum of the diagonal elements.
         *
         * @return the sum of the scalar values at the diagonal positions of the tensor
         */
        inline constexpr Scalar trace() const {
            return [&]<std::size_t... d>(std::index_sequence<d...>) constexpr {
                return (at<repeat<R>(static_cast<I>(d))>() + ...);
            }(std::make_index_sequence<Dimensions>());
        }

        /// @}
    public:
        /// @name Tensor-scalar operators
        /// @{

        /**
         * @brief Element-wise addition of a scalar
         *
         * @param scalar value to add to each element of the tensor
         * @return the modified tensor
         */
        inline constexpr Implementation &operator+=(const Scalar &scalar) {
            for (std::size_t i = 0; i < NumUniqueValues; ++i)
                _data[i] += scalar;
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise subtraction of a scalar
         *
         * @param scalar value to subtract from each element of the tensor
         * @return the modified tensor
         */
        inline constexpr Implementation &operator-=(const Scalar &scalar) {
            for (std::size_t i = 0; i < NumUniqueValues; ++i)
                _data[i] -= scalar;
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise multiplication by a scalar
         *
         * @param scalar value to multiply each element of the tensor by.
         * @return the modified tensor
         */
        inline constexpr Implementation &operator*=(const Scalar &scalar) {
            for (std::size_t i = 0; i < NumUniqueValues; ++i)
                _data[i] *= scalar;
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise division by a scalar
         *
         * @param scalar value to divide each element of the tensor by.
         * @return the modified tensor
         */
        inline constexpr Implementation &operator/=(const Scalar &scalar) {
            for (std::size_t i = 0; i < NumUniqueValues; ++i)
                _data[i] /= scalar;
            return *static_cast<Implementation *>(this);
        }

        /// @copydoc operator+=(const Scalar &)
        inline constexpr Implementation operator+(const Scalar &scalar) const { return Self{*this} += scalar; }

        /// @copydoc operator-=(const Scalar &)
        inline constexpr Implementation operator-(const Scalar &scalar) const { return Self{*this} -= scalar; }

        /// @copydoc operator*=(const Scalar &)
        inline constexpr Implementation operator*(const Scalar &scalar) const { return Self{*this} *= scalar; }

        friend inline constexpr Implementation operator*(const Scalar &scalar, const Self &tensor) {
            return tensor * scalar;
        }

        /// @copydoc operator/=(const Scalar &)
        inline constexpr Implementation operator/(const Scalar &scalar) const { return Self{*this} /= scalar; }

        /// @}
    public:
        /// @name Tensor-tensor element-wise operations
        /// @{

        /**
         * @brief Element-wise addition with another tensor
         *
         * @param other tensor to add with this tensor
         * @return the modified tensor
         */
        inline constexpr Implementation &operator+=(const Implementation &other) {
            for (std::size_t i = 0; i < NumUniqueValues; ++i)
                _data[i] += other._data[i];
            return *static_cast<Implementation *>(this);
        }

        /**
         * @brief Element-wise subtraction by another tensor
         *
         * @param other tensor to subtract from this tensor
         * @return the modified tensor
         */
        inline constexpr Implementation &operator-=(const Implementation &other) {
            for (std::size_t i = 0; i < NumUniqueValues; ++i)
                _data[i] -= other._data[i];
            return *static_cast<Implementation *>(this);
        }

        /// @copydoc operator+=(const Implementation &)
        inline constexpr Implementation operator+(const Implementation &other) const {
            return Self{*this} += other;
        }

        /// @copydoc operator-=(const Implementation &)
        inline constexpr Implementation operator-(const Implementation &other) const {
            return Self{*this} -= other;
        }

        /// @}
    public:
        /// @name Other Tensor-tensor operations
        /// @{

        template<symmetric_tensor<D, I> OtherTensor>
        friend auto operator*(
                const Implementation &lhs,
                const OtherTensor &rhs
        ) {


            constexpr std::size_t OtherRank = OtherTensor::Rank;
            constexpr std::size_t ProductRank = Rank - OtherRank;
            using ProductTensor = ReplaceRank<Implementation, ProductRank>;

            // todo: is this correct?
            ProductTensor product{};
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((
                        product.template at<head<Index, Rank, ProductRank>(lexicographicalIndices<i>())>() +=
                                lhs.template at<lexicographicalIndices<i>()>() *
                                rhs.template at<(tail<Index, Rank, OtherRank>(lexicographicalIndices<i>()))>()
                ), ...);
            }(std::make_index_sequence<NumValues>());
            return product;
        }

        /// @}
    public:
        /// @name Comparison operations
        /// @{

        /**
         * @brief Comparison with another symmetric tensor
         *
         * @param other symmetric tensor to compare with
         * @return true if all elements of the tensors are equivalent, false otherwise
         */
        inline constexpr bool operator==(const Self &other) const = default;

        /// @}
    public: // utility functions

        template<std::array<I, R> Indices>
        static inline consteval std::size_t flatIndex() {
            return flatIndex(Indices);
        }

        static inline constexpr std::size_t flatIndex(std::array<I, R> indices) {
            static_assert(NumValues > 0);

            // Use a lookup table when the range of options is small enough
            if constexpr (R < 3)
                return [&]<std::size_t... i>(std::index_sequence<i...>) {
                    return as_lookup_table<
                            decltype([](std::array<I, R> ind) consteval { return symtensor::flatIndex(ind, D); }),
                            std::array<I, R>,
                            lexicographicalIndices(i)...
                    >(indices);
                }(std::make_index_sequence<NumValues>());
            else
                return symtensor::flatIndex(indices, D);
        }

        static inline constexpr std::array<I, R> dimensionalIndices(std::size_t flatIndex) {
            return symtensor::dimensionalIndices<R, I>(flatIndex, D);
        }

        template<std::size_t flatIndex>
        static inline consteval std::array<I, R> dimensionalIndices() {
            return dimensionalIndices(flatIndex);
        }

        static inline constexpr std::array<I, R> lexicographicalIndices(std::size_t flatIndex) {
            return symtensor::lexicographicalIndices<R, I>(flatIndex, D);
        }

        template<std::size_t flatIndex>
        static inline consteval std::array<I, R> lexicographicalIndices() {
            return lexicographicalIndices(flatIndex);
        }


    public:

        friend std::ostream &operator<<(std::ostream &out, const Implementation &self) {
            // todo: this should be replaced with something prettier
            out << "[";
            std::copy(self._data.begin(), self._data.end() - 1, std::ostream_iterator<Scalar>(out, ", "));
            out << self._data.back() << "]";
            return out;
        }

    };

}

#endif //SYMTENSOR_SYMMETRICTENSORBASE_H
