#ifndef SYMTENSOR_SYMMETRICTENSORBASE_H
#define SYMTENSOR_SYMMETRICTENSORBASE_H

#include "util.h"
#include "concepts.h"
#include "Index.h"

#include <array>
#include <iterator>

namespace symtensor {

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
        std::array<Scalar, NumUniqueValues> _data{0};

        explicit constexpr SymmetricTensorBase(auto ...s) : _data{static_cast<S>(s)...} {}

        inline static consteval Implementation Identity() {
            return NullaryExpression([](auto indices) { return kroneckerDelta(indices); });
        }

        template<typename F>
        inline static constexpr Implementation NullaryExpression(F function = {}) {
            if constexpr (requires { function.template operator()<dimensionalIndices(0)>(); }) {
                // If a function provides a template parameter for compile-time indexing, prefer that
                return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    return Implementation{function.template operator()<dimensionalIndices(i)>()...};
                }(std::make_index_sequence<NumUniqueValues>());
            } else {
                // Otherwise, the function must take the indices as its only argument
                return [&]<std::size_t... i>(std::index_sequence<i...>) constexpr {
                    return Implementation{function(dimensionalIndices(i))...};
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

        template<typename V>
        inline static constexpr Implementation CartesianPower(const V &vector) {
            return NullaryExpression([&]<std::array<I, Rank> index>() constexpr {
                return [&]<std::size_t... r>(std::index_sequence<r...>) constexpr {
                    return (vector[static_cast<std::size_t>(index[r])] * ...);
                }(std::make_index_sequence<Rank>());
            });
        }

    public: // Member Access

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
            return _data[flatIndex<std::array<I, R>{static_cast<Index>(Indices)...}>()];
        }

        template<Index... Indices>
        inline constexpr Scalar &at() {
            return _data[flatIndex<std::array<I, R>{static_cast<Index>(Indices)...}>()];
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

    public: // tensor properties

        inline constexpr Scalar trace() const {
            return [&]<std::size_t... d>(std::index_sequence<d...>) constexpr {
                return (at<repeat<R>(static_cast<I>(d))>() + ...);
            }(std::make_index_sequence<Dimensions>());
        }

    public: // tensor-scalar operators

        inline constexpr Implementation &operator*=(const Scalar &scalar) {
            for (int i = 0; i < NumUniqueValues; ++i)
                _data[i] *= scalar;
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation operator*(const Scalar &scalar) const { return Self{*this} *= scalar; }

    public: // tensor-tensor element-wise operations

        inline constexpr Implementation &operator+=(const Implementation &other) {
            for (int i = 0; i < NumUniqueValues; ++i)
                _data[i] += other._data[i];
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator-=(const Implementation &other) {
            for (int i = 0; i < NumUniqueValues; ++i)
                _data[i] -= other._data[i];
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation operator+(const Implementation &other) const { return Self{*this} += other; }

        inline constexpr Implementation operator-(const Implementation &other) const { return Self{*this} -= other; }

    public:

        // fixme: this might not work with CRTP
        inline constexpr bool operator==(const Self &other) const = default;

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
