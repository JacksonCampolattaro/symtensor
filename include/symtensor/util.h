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

    template<std::size_t R, typename I>
    constexpr std::size_t linearIndex(std::array<I, R> dimensionalIndices, std::size_t D, std::size_t lowestIndex = 0) {

        // Base case: a vector has a single index
        if (R == 1) return static_cast<std::size_t>(dimensionalIndices[0]) - lowestIndex;

        // Ensure the indices are in the canonical order
        std::sort(dimensionalIndices.begin(), dimensionalIndices.end());

        if (dimensionalIndices[0] == I{lowestIndex}) {
            // If the first index is X, then we know we are in the first portion of the range,
            // we can defer to lower-rank indexing
            return linearIndex(tail(dimensionalIndices), D, lowestIndex);

        } else {
            // Otherwise, we know that the dimensional index does not contain _any_ Xs,
            // so we must be in the second portion of the range.
            return numUniqueValuesInSymmetricTensor(D, R - 1) +
                   // We can treat our indices as though the dimension is lower, since they don't contain X
                   linearIndex(dimensionalIndices, D - 1, lowestIndex + 1);
        }
    }

}

#endif //SYMTENSOR_UTIL_H
