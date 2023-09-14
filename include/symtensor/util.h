#ifndef SYMTENSOR_UTIL_H
#define SYMTENSOR_UTIL_H

#include <cstddef>
#include <array>
#include <algorithm>
#include <functional>
#include <cassert>

namespace symtensor {

    constexpr std::size_t pow(std::size_t V, std::size_t P) {
        if (P == 0)
            return 1;
        else
            return V * pow(V, P - 1);
    }

    constexpr std::size_t pascal(std::size_t x, std::size_t y) {
        if (x == 0 || y == 0) return 1;
        else return pascal(x - 1, y) + pascal(x, y - 1);
    }

    constexpr std::size_t numValuesInTensor(std::size_t D, std::size_t R) {
        return pow(D, R);
    }

    constexpr std::size_t numUniqueValuesInSymmetricTensor(std::size_t D, std::size_t R) {
        return pascal(D - 1, R);
    }

    template<std::size_t R, typename I>
    static constexpr int kroneckerDelta(std::array<I, R> dimensionalIndices) {
        return [&]<std::size_t... i>(std::index_sequence<i...>) {
            return ((dimensionalIndices[i]) == ...) ? 1 : 0;
        }(std::make_index_sequence<R>());
    }

    template<typename T, std::size_t N>
    static constexpr auto tail(std::array<T, N> array) {
        if constexpr (N == 0)
            // Never try to get the tail of an empty array! This makes the compiler very angry.
            return array;
        else {
            std::array<T, N - 1> copy{};
            std::copy(array.begin() + 1, array.end(), copy.begin());
            return copy;
        }
    }

}

#endif //SYMTENSOR_UTIL_H
