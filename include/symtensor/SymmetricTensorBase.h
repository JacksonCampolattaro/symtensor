#ifndef SYMTENSOR_SYMMETRICTENSORBASE_H
#define SYMTENSOR_SYMMETRICTENSORBASE_H

#include "util.h"
#include "concepts.h"
#include "Index.h"

#include <array>
#include <iterator>

namespace symtensor {

    template<typename S, std::size_t D, std::size_t R, typename I = Index<D>>
    class SymmetricTensorBase {
    public:

        using Self = SymmetricTensorBase<S, D, R, I>;

        using Scalar = S;
        static constexpr std::size_t Dimensions = D;
        static constexpr std::size_t Rank = R;

        static constexpr std::size_t NumValues = numValuesInTensor(D, R);
        static constexpr std::size_t NumUniqueValues = numUniqueValuesInSymmetricTensor(D, R);

        using Index = I;

    public:

        // todo: this could provided by a CRTP type
        std::array<Scalar, NumUniqueValues> _data{0};

        explicit constexpr SymmetricTensorBase(auto ...s) : _data{static_cast<S>(s)...} {}

        static consteval Self Identity() {
            return NullaryExpression([](auto indices) { return kroneckerDelta(indices); });
        }

        template<typename F>
        static constexpr Self NullaryExpression(F function = {}) {
            if constexpr (requires { function.template operator()<dimensionalIndices(0)>(); }) {
                // If a function provides a template parameter for compile-time indexing, prefer that
                return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    return Self{function.template operator()<dimensionalIndices(i)>()...};
                }(std::make_index_sequence<NumUniqueValues>());
            } else {
                // Otherwise, the function must take the indices as its only argument
                return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    return Self{function(dimensionalIndices(i))...};
                }(std::make_index_sequence<NumUniqueValues>());
            }
        }

    public:

        inline constexpr const Scalar &operator[](const std::array<Index, R> &indices) const {
            return _data[flatIndex(indices)];
        }

        inline constexpr Scalar &operator[](const std::array<Index, R> &indices) {
            return _data[flatIndex(indices)];
        }

        template<std::array<I, R> Indices>
        inline constexpr const Scalar &at() const {
            return _data[flatIndex<Indices>()];
        }

        template<std::array<I, R> Indices>
        inline constexpr Scalar &at() {
            return _data[flatIndex<Indices>()];
        }

        template<Index... Indices>
        inline constexpr const Scalar &at() const {
            return _data[flatIndex({static_cast<Index>(Indices)...})];
        }

        template<Index... Indices>
        inline constexpr Scalar &at() {
            return _data[flatIndex({static_cast<Index>(Indices)...})];
        }

        template<typename Integer, Integer... Indices>
        inline constexpr const Scalar &operator[](std::integer_sequence<Integer, Indices...>) const {
            static_assert(std::is_integral_v<Integer>);
            return _data[flatIndex({Indices...})];
        }

        inline constexpr const Scalar &operator[](auto flatIndex) const {
            static_assert(std::is_integral_v<decltype(flatIndex)>);
            return _data[static_cast<std::size_t>(flatIndex)];
        }

        inline constexpr Scalar &operator[](auto flatIndex) {
            static_assert(std::is_integral_v<decltype(flatIndex)>);
            return _data[static_cast<std::size_t>(flatIndex)];
        }

    public: // tensor-scalar operators

        Self &operator*=(const Scalar &scalar) {
            for (int i = 0; i < NumUniqueValues; ++i)
                _data[i] *= scalar;
            return *this;
        }

        Self operator*(const Scalar &scalar) const { return Self{*this} *= scalar; }

    public: // tensor-tensor element-wise operations

        Self &operator+=(const Self &other) {
            for (int i = 0; i < NumUniqueValues; ++i)
                _data[i] += other._data[i];
            return *this;
        }

        Self &operator-=(const Self &other) {
            for (int i = 0; i < NumUniqueValues; ++i)
                _data[i] -= other._data[i];
            return *this;
        }

        Self operator+(const Self &other) const { return Self{*this} += other; }

        Self operator-(const Self &other) const { return Self{*this} -= other; }

    public: // tensor-vector operations

        [[clang::always_inline]] friend inline constexpr auto
        operator*(const Self &tensor, const SymmetricTensorBase<S, D, 1, I> &vector) {
            using Result = SymmetricTensorBase<S, D, R + 1, I>;
            return Result::template NullaryExpression([&]<std::array<I, Result::Rank> index>() constexpr {
                return vector.template at<std::array<I, 1>{index[0]}>() *
                       tensor.template at<tail(index)>();
            });
        }

    public:

        bool operator==(const Self &other) const = default;

    public:

        template<std::array<I, R> Indices>
        static inline consteval std::size_t flatIndex() {
            return flatIndex(Indices);
        }

        static inline constexpr std::size_t flatIndex(std::array<I, R> indices) {
            //            return symtensor::flatIndex(indices, D);
            static_assert(NumValues > 0);
            return [&]<std::size_t... i>(std::index_sequence<i...>) {
                return as_lookup_table<
                        decltype([](std::array<I, R> ind) consteval { return symtensor::flatIndex(ind, D); }),
                        std::array<I, R>,
                        lexicographicalIndices(i)...
                >(indices);
            }(std::make_index_sequence<NumValues>());
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

        friend std::ostream &operator<<(std::ostream &out, const Self &self) {
            // todo: this should be replaced with something prettier
            out << "[";
            std::copy(self._data.begin(), self._data.end() - 1, std::ostream_iterator<Scalar>(out, ", "));
            out << self._data.back() << "]";
            return out;
        }

    };

    template<std::size_t R>
    using SymmetricTensor2f = SymmetricTensorBase<float, 2, R>;

    template<std::size_t R>
    using SymmetricTensor3f = SymmetricTensorBase<float, 3, R>;
}

#endif //SYMTENSOR_SYMMETRICTENSORBASE_H