#ifndef SYMTENSOR_INDEX_H
#define SYMTENSOR_INDEX_H

#include <cstddef>
#include <array>
#include <algorithm>
#include <iterator>
#include <span>

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

    template<std::size_t D>
    static std::ostream &operator<<(std::ostream &out, const Index<D> &index) {
        // todo: this can eventually provide pretty-printing (e.g. 0 --> X)
        return out << static_cast<std::size_t>(index);
    }

    template<std::size_t D, std::size_t R>
    static std::ostream &operator<<(std::ostream &out, const std::array<Index<D>, R> &indices) {
        // todo: this should be replaced with something cleaner
        out << "(";
        std::copy(indices.begin(), indices.end() - 1, std::ostream_iterator<Index<D>>(out, ", "));
        out << indices.back() << ")";
        return out;
    }

    namespace {

        template<typename I>
        inline constexpr void recursiveDimensionalIndices(
                const std::size_t &flatIndex,
                std::size_t D,
                std::span<I, 1> indices,
                std::size_t offset = 0
        ) {
            // Base case: a vector's flat index is the same as its dimensional index
            indices[0] = I{flatIndex + offset};
        }

        template<typename I, std::size_t R>
        inline constexpr void recursiveDimensionalIndices(
                const std::size_t &flatIndex,
                std::size_t D,
                std::span<I, R> indices,
                std::size_t offset = 0
        ) {

            if (flatIndex < numUniqueValuesInSymmetricTensor(D, R - 1)) {

                // If the linear index would fit in a smaller matrix, prefix with an X and recursively find the rest
                indices[0] = I{offset};
                recursiveDimensionalIndices<I>(
                        flatIndex, D,
                        std::span<I, R - 1>{indices.begin() + 1, indices.end()},
                        offset
                );

            } else {

                // Otherwise, we know the index will not contain an X, we need to find the rest recursively.
                // We can act as though there are fewer dimensions, since X isn't present.
                // Use the offset to write Z instead of Y and Y instead of X.
                recursiveDimensionalIndices(
                        flatIndex - numUniqueValuesInSymmetricTensor(D, R - 1),
                        D - 1,
                        indices,
                        offset + 1
                );

            }
        }
    }

    template<std::size_t R, typename I>
    inline constexpr std::array<I, R> dimensionalIndices(
            const std::size_t &flatIndex,
            std::size_t D
    ) {
        assert(flatIndex < numUniqueValuesInSymmetricTensor(D, R));

        std::array<I, R> array{};
        recursiveDimensionalIndices(flatIndex, D, std::span<I, R>{array.begin(), array.end()});
        return array;
    }

    template<std::size_t R, typename I>
    inline constexpr std::size_t flatIndex(
            std::array<I, R> dimensionalIndices,
            std::size_t D,
            std::size_t lowestIndex = 0
    ) {

        // Base case: a vector has a single index
        if (R == 1) return static_cast<std::size_t>(dimensionalIndices[0]) - lowestIndex;

        // Ensure the indices are in the canonical order
        std::sort(dimensionalIndices.begin(), dimensionalIndices.end());

        if (dimensionalIndices[0] == I{lowestIndex}) {
            // If the first index is X, then we know we are in the first portion of the range,
            // we can defer to lower-rank indexing
            return flatIndex(tail(dimensionalIndices), D, lowestIndex);

        } else {
            // Otherwise, we know that the dimensional index does not contain _any_ Xs,
            // so we must be in the second portion of the range.
            return numUniqueValuesInSymmetricTensor(D, R - 1) +
                   // We can treat our indices as though the dimension is lower, since they don't contain X
                   flatIndex(dimensionalIndices, D - 1, lowestIndex + 1);
        }
    }

    namespace {

        template<typename I>
        inline constexpr void recursiveLexicographicalIndices(
                const std::size_t &linearIndex,
                std::size_t D,
                std::span<I, 1> indices
        ) {
            // Base case: a vector's flat index is the same as its lexicographical index
            indices[0] = static_cast<I>(linearIndex);
        }

        template<typename I, std::size_t R>
        inline constexpr void recursiveLexicographicalIndices(
                const std::size_t &linearIndex,
                std::size_t D,
                std::span<I, R> indices
        ) {

            std::size_t topIndex = linearIndex % D;
            std::size_t lowerLinearIndex = (linearIndex - topIndex) / D;

            indices.back() = I(topIndex);
            recursiveLexicographicalIndices(lowerLinearIndex, D, std::span<I, R-1>{indices.begin(), indices.end() - 1});
        }
    }

    template<std::size_t R, typename I>
    static inline constexpr std::array<I, R> lexicographicalIndices(std::size_t linearIndex, std::size_t D) {
        //static_assert(LinearIndex < NumValues, "Lexicographical index is out of bounds");
        std::array<I, R> indices{};
        recursiveLexicographicalIndices(linearIndex, D, std::span<I, R>{indices.begin(), indices.end()});
        return indices;
    }

}

#endif //SYMTENSOR_INDEX_H
