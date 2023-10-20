#ifndef SYMTENSOR_CONCEPTS_H
#define SYMTENSOR_CONCEPTS_H

#include <cstddef>

namespace symtensor {

    template<class T, class I>
    concept indexable_by = requires(T &t, const I &i) {
        { t[i] };
    };

    template<class T>
    concept indexable = indexable_by<T, std::size_t>;

    template<typename T>
    concept tuple_like = requires(T){
        std::tuple_size<T>::value;
    };

    //    namespace {
    //
    //        template<typename ST, typename IndexSequence>
    //        struct compile_time_indexable_helper;
    //
    //        template<typename ST, std::size_t... Indices>
    //        struct compile_time_indexable_helper<ST, std::integer_sequence<std::size_t, Indices...>> {
    //            static constexpr bool value = requires(ST tensor) {
    //                tensor.at<static_cast<typename ST::Index>(Indices)...>();
    //            };
    //        };
    //
    //    }
    //
    //    template<typename T>
    //    concept compile_time_indexable = requires{ compile_time_indexable_helper<T, decltype(std::index_sequence<T::Dimensions>)> };

    template<typename T, std::size_t R, std::size_t D, typename I>
    concept symmetric_tensor = requires(T tensor){

//        requires T::Dimensions == D;
//        requires T::Rank == R;
        requires std::same_as<typename T::Index, I>;
        // todo: check for at<>
    };

}
#endif //SYMTENSOR_CONCEPTS_H
