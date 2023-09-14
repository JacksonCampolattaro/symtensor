#ifndef SYMTENSOR_INDEX_H
#define SYMTENSOR_INDEX_H

#include <cstddef>
#include <array>
#include <algorithm>

namespace symtensor {

    template<std::size_t D>
    struct IndexTypeForDimension;

    template<>
    struct IndexTypeForDimension<2> {
        enum class type : std::size_t {
            X = 0,
            Y,
        };
    };

    template<>
    struct IndexTypeForDimension<3> {
        enum class type : std::size_t {
            X = 0,
            Y,
            Z
        };
    };

    template<>
    struct IndexTypeForDimension<4> {
        enum class type : std::size_t {
            W = 0,
            X,
            Y,
            Z
        };
    };

    template<std::size_t D>
    struct IndexTypeForDimension {
        using type = std::size_t;
    };

    template<std::size_t D>
    using Index = IndexTypeForDimension<D>::type;

}

#endif //SYMTENSOR_INDEX_H
