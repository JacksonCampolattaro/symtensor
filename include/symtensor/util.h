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

    // This is a lovely little trick!
    // see: https://artificial-mind.net/blog/2020/11/14/cpp17-consteval
    template <auto V>
    static constexpr auto force_consteval = V;

    constexpr std::size_t pow(std::size_t V, std::size_t P) {
        if (P == 0)
            return 1;
        else
            return V * pow(V, P - 1);
    }

    template<std::size_t P, typename T>
    inline T pow(T value) {
        if constexpr (P == 0)
            return T(1);
        else
            return value * pow<P - 1>(value);
    }

    constexpr std::size_t pascal(std::size_t x, std::size_t y) {
        if (x == 0 || y == 0) return 1;
        else return pascal(x - 1, y) + pascal(x, y - 1);
    }

    constexpr std::size_t factorial(std::size_t n) {
        if (n == 0) return 1;
        return n * factorial(n - 1);
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

    template<typename T, std::size_t N, std::size_t NewN = N - 1>
    static constexpr auto head(std::array<T, N> array) {
        if constexpr (N == 0)
            // Never try to get the head of an empty array! This makes the compiler very angry.
            return array;
        else {
            std::array<T, NewN> copy{};
            std::copy(array.begin(), array.begin() + NewN, copy.begin());
            return copy;
        }
    }

    template<typename T, std::size_t N, std::size_t NewN = N - 1>
    static constexpr auto tail(std::array<T, N> array) {
        if constexpr (N == 0)
            // Never try to get the tail of an empty array! This makes the compiler very angry.
            return array;
        else {
            std::array<T, NewN> copy{};
            std::copy(array.end() - NewN, array.end(), copy.begin());
            return copy;
        }
    }

    template<typename T, std::size_t NA, std::size_t NB>
    constexpr std::array<T, NA + NB> concatenate(const std::array<T, NA> &a, const std::array<T, NB> &b) {
        std::array<T, NA + NB> result{};
        std::copy(a.begin(), a.end(), result.begin());
        std::copy(b.begin(), b.end(), result.begin() + a.size());
        return result;
    }

    template<typename T, std::size_t N>
    constexpr void swap(std::array<T, N> &array, std::size_t i, std::size_t j) {
        auto temp = array[i];
        array[i] = array[j];
        array[j] = temp;
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

    template<std::size_t I, typename... T>
    using type_at_index = std::remove_reference_t<decltype(std::get<I>(std::declval<std::tuple<T...>>()))>;

    template<typename... T>
    using last_type = type_at_index<sizeof...(T) - 1, T...>;

    template<typename Tuple>
    using last_type_of_tuple = std::tuple_element_t<std::tuple_size_v<Tuple> - 1, Tuple>;

    template<typename T, template<typename...> class Template>
    struct is_specialization_of : std::false_type {
    };

    template<template<typename...> class Template, typename... Args>
    struct is_specialization_of<Template<Args...>, Template> : std::true_type {
    };

    template<class T>
    concept derived_from_tuple = requires(T t) {
        []<typename ...Types>(const std::tuple<Types...> &) {}(t);
    };

    template<derived_from_tuple T>
    using underlying_tuple = decltype(
    []<typename ...Types>(const std::tuple<Types...> &&) -> std::tuple<Types...> {
        return std::declval<std::tuple<Types...>>();
    }(std::declval<T>())
    );

    template<derived_from_tuple T>
    using underlying_tuple_size = decltype([]<typename ...Types>
            (const std::tuple<Types...> &&) -> std::tuple_size<std::tuple<Types...>> {}
            (std::declval<T>())
    );

    template<std::size_t I, derived_from_tuple T>
    using underlying_tuple_element = decltype([]<typename ...Types>
            (const std::tuple<Types...> &&) -> std::tuple_element<I, std::tuple<Types...>> {}
            (std::declval<T>())
    );

    template<class T, template<typename...> typename Template>
    concept derived_from_template = requires(T t) {
        []<typename ...Types>(Template<Types...> &) {}(t);
    };

    namespace {

        template<template<typename...> class TemplateType, typename Tuple, typename ...Prefix>
        struct expand_tuple_helper;

        template<template<typename...> class TemplateType, typename ...Args, typename ...Prefix>
        struct expand_tuple_helper<TemplateType, std::tuple<Args...>, Prefix...> {
            using type = TemplateType<Prefix..., Args...>;
        };
    }

    template<template<typename...> class TemplateType, typename Tuple, typename ...Prefix>
    using expand_tuple = typename expand_tuple_helper<TemplateType, Tuple, Prefix...>::type;

    template<typename T, std::size_t N>
    constexpr std::array<T, N> nth_permutation(const std::array<T, N> &array, std::size_t n) {
        std::array<T, N> result = array;

        // Compute factorials
        std::array<std::size_t, N> factorials{};
        for (std::size_t i = 0; i < N; ++i) {
            factorials[i] = factorial(i);
        }

        // Compute the Lehmer code for nth permutation
        std::array<T, N> temp = array;
        for (std::size_t i = 0; i < N; ++i) {
            std::size_t factorial_index = N - 1 - i;
            std::size_t selected_index = n / factorials[factorial_index];
            n %= factorials[factorial_index];

            result[i] = temp[selected_index];

            // Remove the used element
            for (std::size_t j = selected_index; j < N - 1 - i; ++j) {
                temp[j] = temp[j + 1];
            }
        }

        return result;
    }

    template<typename T, std::size_t N>
    constexpr auto permutations(const std::array<T, N> &arr) {
        constexpr std::size_t num_permutations = factorial(N);
        std::array<std::array<T, N>, num_permutations> result{};

        for (std::size_t i = 0; i < num_permutations; ++i) {
            result[i] = nth_permutation(arr, i);
        }

        return result;
    }

    template<typename T>
    constexpr std::size_t n_choose_k(const T &n, const T &k) {
        if (k > n) return 0;
        if (k == 0 || n == k) return 1;
        if (n - k < k) return n_choose_k(n, n - k);

        std::size_t result = 1;
        for (int i = 1; i <= k; ++i) {
            result *= n - i + 1;
            result /= i;
        }
        return result;
    }

    template<std::size_t K, typename T, std::size_t N>
    constexpr auto binomial_partitions(const std::array<T, N> &set) {
        // todo: could I do this without calculating the factorial? That feels expensive.
        constexpr std::size_t num_partitions = n_choose_k(N, K);

        auto selection = std::array<bool, N>{};
        std::fill(selection.begin(), selection.begin() + K, true);

        std::array<std::tuple<std::array<T, K>, std::array<T, (N - K)>>, num_partitions> partitions{};
        auto pair_out = partitions.begin();
        do {
            auto a_out = get<0>(*pair_out).begin();
            auto b_out = get<1>(*pair_out).begin();

            for (int i = 0; i < set.size(); ++i) {
                if (selection[i])
                    *(a_out++) = set[i];
                else
                    *(b_out++) = set[i];
            }

            pair_out++;
        } while (std::prev_permutation(selection.begin(), selection.end()));
        // todo
        return partitions;
    }

}

#endif //SYMTENSOR_UTIL_H
