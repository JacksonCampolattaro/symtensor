#pragma once

#if defined(__GNUC__)
    #define ALWAYS_INLINE [[gnu::always_inline]] inline
#elif defined(__clang__)
    #define ALWAYS_INLINE [[clang::always_inline]] inline
#elif defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
#else
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

#define LAMBDA_ALWAYS_INLINE __attribute__((always_inline))
