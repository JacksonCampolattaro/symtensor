#ifndef SYMTENSOR_SYMMETRICTENSORBASE_H
#define SYMTENSOR_SYMMETRICTENSORBASE_H

#include "util.h"
#include "Index.h"

#include <array>

namespace symtensor {

    template<typename S, std::size_t D, std::size_t R, typename I = Index<D>>
    class SymmetricTensorBase {
    public:

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

        explicit SymmetricTensorBase(auto ...s) : _data{s...} {}

    public:

        constexpr const Scalar &operator[](std::array<Index, R> dimensionalIndices) const {
            // todo
            auto i = linearIndex(dimensionalIndices, D);
            assert(i < _data.size());
            return _data[linearIndex(dimensionalIndices, D)];
        }

    };

    template<std::size_t R>
    using SymmetricTensor3f = SymmetricTensorBase<float, 3, R>;
}

#endif //SYMTENSOR_SYMMETRICTENSORBASE_H
