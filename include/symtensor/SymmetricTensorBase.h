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

    private:

        // todo: this could provided by a CRTP type
        std::array<Scalar, NumUniqueValues> _data{0};

    public:

        SymmetricTensorBase() = default;

        explicit SymmetricTensorBase(auto ...s) : _data{static_cast<S>(s)...} {}

        static constexpr Self Identity() {
            Self result{};
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((result._data[i] = kroneckerDelta(dimensionalIndices(i))), ...);
            }(std::make_index_sequence<NumUniqueValues>());
            return result;
        }

        template<typename F>
        static constexpr Self NullaryExpression(const F &function) {
            Self result{};
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((result._data[i] = function(dimensionalIndices(i))), ...);
            }(std::make_index_sequence<NumUniqueValues>());
            return result;
        }

    public:

        constexpr const Scalar &operator[](std::array<Index, R> dimensionalIndices) const {
            auto i = flatIndex(dimensionalIndices);
            assert(i < _data.size());
            return _data[flatIndex(dimensionalIndices)];
        }

        constexpr const Scalar &operator[](auto flatIndex) const {
            static_assert(std::is_integral_v<decltype(flatIndex)>);
            return _data(static_cast<std::size_t>(flatIndex));
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
        friend auto operator*(const Self &tensor, const Vector &vector) {
            // fixme: this isn't correct
            return SymmetricTensorBase<S, D, R + 1, I>::NullaryExpression([&](auto indices) {
                return vector[static_cast<std::size_t>(indices[0])] + tensor[tail(indices)];
            });
        }


    public:

        bool operator==(const Self &other) const = default;

    public:

        static constexpr std::size_t flatIndex(std::array<I, R> dimensionalIndices) {
            return symtensor::flatIndex(dimensionalIndices, D);
        }

        static constexpr std::array<I, R> dimensionalIndices(std::size_t flatIndex) {
            return symtensor::dimensionalIndices<R, I>(flatIndex, D);
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
