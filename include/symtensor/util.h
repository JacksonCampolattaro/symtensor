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
    template<auto V>
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
            return T{1};
        else if constexpr (P % 2 == 0)
            return pow<P/2>(value * value);
        else if constexpr (P % 3 == 0)
            return pow<P/3>(value * value * value);
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

    template<typename T=bool, std::size_t R, typename I>
    static constexpr T kronecker_delta(std::array<I, R> dimensionalIndices) {
        return std::all_of(
                dimensionalIndices.begin(), dimensionalIndices.end(),
                [&](auto i) { return i == dimensionalIndices[0]; }
        );
    }

    template<auto array>
    static consteval auto kronecker_delta() { return kronecker_delta(array); }

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

    template<std::size_t N, typename T>
    inline constexpr std::array<T, N> sorted(std::array<T, N> array) {
        std::sort(array.begin(), array.end());
        return array;
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


    template<typename A, std::size_t NA, typename B, std::size_t NB, std::size_t N>
    constexpr auto unzip(const std::array<std::tuple<std::array<A, NA>, std::array<B, NB>>, N> &input) {
        std::array<std::array<A, NA>, N> arrayA{};
        std::array<std::array<B, NB>, N> arrayB{};

        for (std::size_t i = 0; i < N; ++i) {
            arrayA[i] = std::get<0>(input[i]);
            arrayB[i] = std::get<1>(input[i]);
        }

        return std::make_tuple(arrayA, arrayB);
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
    inline constexpr std::array<T, N> nth_permutation(const std::array<T, N> &array, std::size_t n) {
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
    inline constexpr auto permutations(const std::array<T, N> &arr) {
        constexpr std::size_t num_permutations = factorial(N);
        std::array<std::array<T, N>, num_permutations> result{};

        for (std::size_t i = 0; i < num_permutations; ++i) {
            result[i] = nth_permutation(arr, i);
        }

        return result;
    }

    template<typename T>
    inline constexpr std::size_t n_choose_k(const T &n, const T &k) {
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
    inline constexpr auto binomial_partitions(const std::array<T, N> &set) {
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
            get<0>(*pair_out) = sorted(get<0>(*pair_out));
            get<1>(*pair_out) = sorted(get<1>(*pair_out));

            pair_out++;
        } while (std::prev_permutation(selection.begin(), selection.end()));
        return partitions;
    }

    template<typename T, std::size_t N, typename F>
    inline constexpr std::size_t count_if(const std::array<T, N> &array, const F &f = {}) {
        std::size_t count = 0;
        for (auto v: array)
            if (f(v)) ++count;
        return count;
    }

    template<auto array, auto f>
    inline consteval auto filter() {
        // todo: this might not be workable, actually
        using T = typename decltype(array)::value_type;
        constexpr auto N = array.size();

        // Create a temporary vector to store the filtered elements
        std::vector<T> temp;

        // Iterate through the array and apply the filter function
        std::copy_if(array.begin(), array.end(), std::back_inserter(temp), f);

        // Create a new array to store the filtered elements
        constexpr std::size_t num_matches = count_if(array, f);
        std::array<T, num_matches> result;
        std::copy(temp.begin(), temp.end(), result.begin());

        return result;
    }

    template<typename T, std::size_t N>
    inline constexpr std::size_t num_unique_elements(const std::array<T, N> &array) {
        std::size_t count = 0;

        for (auto it = array.begin(); it != array.end(); ++it) {
            bool found = false;
            for (auto jt = array.begin(); jt != it; ++jt) {
                if (*it == *jt) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                ++count;
            }
        }

        return count;
    }

    template<auto array>
    inline consteval auto unique() {
        // todo: this might not be workable, actually
        using T = typename decltype(array)::value_type;
        constexpr auto N = array.size();

        auto counts = std::array<std::size_t, N>{0};

        // Check each element
        for (const auto &value: array) {

            // Get the index of the first appearance of this value
            auto it = std::find_if(array.begin(), array.end(), [&](const auto &elem) { return elem == value; });
            std::size_t index = std::distance(array.begin(), it);

            // Increment the appropriate counter
            counts[index]++;
        }

        // Construct the output container now that we know the size,
        // and fill it with unique elements and their counts.
        std::array<std::tuple<T, std::size_t>, num_unique_elements(array)> result{};
        auto out = result.begin();
        for (int i = 0; i < N; ++i) {
            if (counts[i] > 0)
                *(out++) = {array[i], counts[i]};
        }

        return result;
    }

    template<typename Filter, typename Transform, typename T, std::size_t N>
    inline constexpr auto filtered_transform_sum(
            const std::array<T, N> &values,
            Filter filter = {},
            Transform transform = {},
            decltype(transform(values[0])) init = {}
    ) {
        using Out = decltype(transform(values[0]));
        auto sum = Out{0};
        [&]<auto... i>(std::index_sequence<i...>) constexpr {
            ((filter(values[i]) ? (sum[i] += transform(values[i])) : 0), ...);
        }(std::make_index_sequence<N>());
        return sum;
    }

    template<typename T, std::size_t N>
    constexpr std::array<std::size_t, N> repeats_table(const std::array<T, N> &array) {
        std::array<std::size_t, N> counts = {0};
        for (const auto &value: array) {

            // Get the index of the first appearance of this value
            auto it = std::find_if(array.begin(), array.end(), [&](const auto &elem) { return elem == value; });
            std::size_t index = std::distance(array.begin(), it);

            // Increment the appropriate counter
            counts[index]++;
        }
        return counts;
    }

    template<typename T, std::size_t N>
    constexpr std::size_t num_unique_permutations(const std::array<T, N> &arr) {
        const auto counts = countOccurrences(arr);
        unsigned long long divisor = 1;
        for (std::size_t i = 0; i < N; ++i)
            divisor *= factorial(counts[i]);
        return factorial(N) / divisor;
    }

    template<typename F, typename T, std::size_t N>
    inline constexpr auto deduplicated_sum(const std::array<T, N> &values, F function = {}) {
        // todo: This may be very important for good performance!
    }

}

#endif //SYMTENSOR_UTIL_H
