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
            Self result{};
            //            [&]<std::size_t... i>(std::index_sequence<i...>) {
            //                ((result._data[i] = function(dimensionalIndices(i))), ...);
            //            }(std::make_index_sequence<NumUniqueValues>());
            for (int i = 0; i < NumUniqueValues; ++i) {
                result._data[i] = function(dimensionalIndices(i));
            }
            return result;
        }

    public:

        inline constexpr const Scalar &operator[](const std::array<Index, R> &indices) const {
            return _data[flatIndex(indices)];
        }

        inline constexpr Scalar &operator[](const std::array<Index, R> &indices) {
            return _data[flatIndex(indices)];
        }

        template<auto... Indices>
        inline constexpr const Scalar &at() const {
            return _data[flatIndex<Indices...>()];
        }

        template<auto... Indices>
        inline constexpr Scalar &at() {
            return _data[flatIndex<Indices...>()];
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

        template<indexable Vector>
        [[clang::always_inline]] friend inline constexpr auto operator*(const Self &tensor, const Vector &vector) {
            using Result = SymmetricTensorBase<S, D, R + 1, I>;
            Result result{};
            //            [&]<std::size_t... i>(std::index_sequence<i...>) {
            //                ((result._data[i] = vector[static_cast<std::size_t>(Result::dimensionalIndices(i)[0])] *
            //                                    tensor[tail(Result::dimensionalIndices(i))]), ...);
            //            }(std::make_index_sequence<NumUniqueValues>());
            //            [&]<std::size_t... i>(std::index_sequence<i...>) {
            //                ((result._data[i] = vector[static_cast<std::size_t>(Result::lexicographicalIndices(i)[0])] *
            //                                    tensor[tail(Result::lexicographicalIndices(i))]), ...);
            //            }(std::make_index_sequence<NumValues>());
            for (int i = 0; i < Result::NumValues; ++i) {
                auto lex = Result::lexicographicalIndices(i);
                result._data[i] = vector[static_cast<std::size_t>(lex[0])] * tensor[tail(lex)];
            }

            return result;

//            return SymmetricTensorBase<S, D, R + 1, I>::NullaryExpression([&](auto indices) constexpr {
//                return vector[static_cast<std::size_t>(indices[0])] * tensor[tail(indices)];
//            });
        }


    public:

        bool operator==(const Self &other) const = default;

    public:

        template<auto... Indices>
        static inline consteval std::size_t flatIndex() {
            return flatIndex({static_cast<Index>(Indices)...});
        }

        static inline constexpr std::size_t flatIndex(std::array<I, R> indices) {
            return symtensor::flatIndex(indices, D);
//            static_assert(NumValues > 0);
//            return [&]<std::size_t... i>(auto) {
//                return as_lookup_table<
//                        decltype([](std::array<I, R> ind) consteval { return symtensor::flatIndex(ind, D); }),
//                        std::array<I, R>,
//                        lexicographicalIndices(i)...
//                >(indices);
//            }(std::make_index_sequence<NumValues>());
        }

        static inline constexpr std::array<I, R> dimensionalIndices(std::size_t flatIndex) {
            return symtensor::dimensionalIndices<R, I>(flatIndex, D);
        }

        static inline constexpr std::array<I, R> lexicographicalIndices(std::size_t flatIndex) {
            return symtensor::lexicographicalIndices<R, I>(flatIndex, D);
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
