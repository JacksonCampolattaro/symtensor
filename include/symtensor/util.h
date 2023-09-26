#ifndef SYMTENSOR_UTIL_H
#define SYMTENSOR_UTIL_H

#include <cstddef>
#include <array>
#include <algorithm>
#include <functional>
#include <cassert>
#include <type_traits>
#include <iostream>

#include "concepts.h"

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
        return std::all_of(
                dimensionalIndices.begin(), dimensionalIndices.end(),
                [&](auto i) { return i == dimensionalIndices[0]; }
        );
    }

    template<std::size_t N, typename T>
    static constexpr auto repeat(const T &value) {
        std::array<T, N> array;
        std::fill(array.begin(), array.end(), value);
        return array;
    }

    template<typename T, std::size_t N>
    static constexpr auto head(std::array<T, N> array) {
        if constexpr (N == 0)
            // Never try to get the head of an empty array! This makes the compiler very angry.
            return array;
        else {
            std::array<T, N - 1> copy{};
            std::copy(array.begin(), array.end() - 1, copy.begin());
            return copy;
        }
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

    template<typename T, std::size_t N, typename Tuple>
    constexpr std::array<T, N> to_array(Tuple &&tuple) {
        return std::apply([](auto &&...v) {
            return std::array{static_cast<T>(std::forward<decltype(v)>(v)) ...};
        }, std::forward<Tuple>(tuple));
    }

    template<typename Function, typename Arg, Arg ...PossibleArgs>
    constexpr auto as_lookup_table(Arg arg, Function function = {}) {
        static_assert(sizeof...(PossibleArgs) > 0);
        using Result = std::invoke_result_t<Function, Arg>;
        Result result{};
        ([&] {
            if (arg == PossibleArgs)
                result = function(PossibleArgs);
        }(), ...);
        return result;
    }

}

#endif //SYMTENSOR_UTIL_H
