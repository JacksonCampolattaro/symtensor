/**
 * @file
 * @brief Index types and convenience functions for working with indices.
 */
#ifndef SYMTENSOR_INDEX_H
#define SYMTENSOR_INDEX_H

#include <cstddef>
#include <array>
#include <algorithm>
#include <iterator>
#include <span>

#include <symtensor/util.h>

namespace symtensor {

    /**
     * @brief Determines the number of indices in a square tensor of rank R and size D
     *
     * Equivalent to \f$ D^R \f$.
     *
     * @param D size of the tensor along all axes
     * @param R rank of the tensor
     * @return the number of values the the tensor
     */
    constexpr std::size_t numValuesInTensor(std::size_t D, std::size_t R) {
        return pow(D, R);
    }

    /**
     * @brief Determines the number of _unique_ values in a symmetric tensor of rank R and size D
     *
     * Equivalent to Pascal's triangle evaluated at D-1, R.
     *
     * @param D size of the symmetric tensor along all axes
     * @param R rank of the symmetric tensor
     * @return the number of unique values in the symmetric tensor
     *  (indices xxy, xyx, and yxx are not counted separately).
     */
    constexpr std::size_t numUniqueValuesInSymmetricTensor(std::size_t D, std::size_t R) {
        return pascal(D - 1, R);
    }

    namespace {

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

    }

    /**
     * @brief Enum-class index type for a given dimension
     *
     * For 2d, this contains elements @code{.cpp} X, Y @endcode,
     * for 3d @code{.cpp} X, Y, Z @endcode,
     * for 4d @code{.cpp} W, X, Y, Z @endcode.
     */
    template<std::size_t D>
    using Index = typename IndexTypeForDimension<D>::type;

    template<std::size_t D>
    static std::ostream &operator<<(std::ostream &out, const Index<D> &index) {
        // todo: this can eventually provide pretty-printing (e.g. 0 --> X)
        return out << static_cast<std::size_t>(index);
    }

    template<typename I, std::size_t R>
    static std::ostream &operator<<(std::ostream &out, const std::array<I, R> &indices) {
        // todo: this is not ideal
        out << "(";
        out << static_cast<std::size_t>(indices[0]);
        for (std::size_t i = 1; i < R; ++i)
            out << ", " << static_cast<std::size_t>(indices[i]);
        out << ")";
        return out;
    }

    namespace {

        template<typename I>
        inline constexpr void recursiveDimensionalIndices(
                const std::size_t &flatIndex,
                [[maybe_unused]] std::size_t D,
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

    /**
     * @brief converts a flat index to an equivalent multi-dimensional index.
     *
     * The index is in "canonical" form.
     * For a \f$ 3 \times 3 \times 3 \f$ symmetric tensor, this means the following mapping:
     *
     * @m_div{m-text-center}
     * 0 --> X, X, X \n
     * 1 --> X, X, Y \n
     * 2 --> X, X, Z \n
     * 3 --> X, Y, Y \n
     * 4 --> X, Y, Z \n
     * 5 --> X, Z, Z \n
     * 6 --> Y, Y, Y \n
     * 7 --> Y, Y, Z \n
     * 8 --> Y, Z, Z \n
     * 9 --> Z, Z, Z \n
     * @m_enddiv
     *
     * @tparam R Rank of the tensor
     * @tparam I Index type
     *
     * @param flatIndex 1-dimensional index, in the range [0, N)\n
     *  where N can be found using @ref numUniqueValuesInSymmetricTensor
     * @param D size of the tensor
     * @return std::array of indices, with type I and size R
     */
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

    /**
     * @brief Converts a multi-dimensional index in a symmetric tensor into a flat index
     *
     * First puts the index in "canonical" form, where indices are sorted as follows:
     *
     * @m_div{m-text-center}
     * {X, X, Y} or {X, Y, X} or {Y, X, X} --> {X, X, Y}
     * @m_enddiv
     *
     * Next, converts the indices to a flat index using the reverse process of @ref dimensionalIndices()
     *
     * @m_div{m-text-center}
     * X, X, X --> 0 \n
     * X, X, Y --> 1 \n
     * X, X, Z --> 2 \n
     * X, Y, Y --> 3 \n
     * X, Y, Z --> 4 \n
     * X, Z, Z --> 5 \n
     * Y, Y, Y --> 6 \n
     * Y, Y, Z --> 7 \n
     * Y, Z, Z --> 8 \n
     * Z, Z, Z --> 9 \n
     * @m_enddiv
     *
     * @tparam R Rank of the tensor
     * @tparam I Index type
     *
     * @param dimensionalIndices array of R Index types which specifies a value in the tensor
     * @param D size of the tensor
     * @param lowestIndex (implementation detail)
     *
     * @return the corresponding index in a flat array with no redundant values
     */
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
                [[maybe_unused]] std::size_t D,
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
            recursiveLexicographicalIndices(lowerLinearIndex, D,
                                            std::span<I, R - 1>{indices.begin(), indices.end() - 1});
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

template<typename I, std::size_t R>
static std::ostream &operator<<(std::ostream &out, const std::array<I, R> &indices) {
    // todo: this should be replaced with something cleaner
    out << "(";
    std::copy(indices.begin(), indices.end() - 1, std::ostream_iterator<I>(out, ", "));
    out << indices.back() << ")";
    return out;
}

#endif //SYMTENSOR_INDEX_H
