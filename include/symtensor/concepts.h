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

}
#endif //SYMTENSOR_CONCEPTS_H
