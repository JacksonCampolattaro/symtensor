#pragma once

#include "../platform.h"

/*
 The following is generated from tensor diagram notation by Christoph Peters.
*/

#include <cmath>


struct A_3 {
    float a[3];
};

struct AA_3 {
    float a[6];
};

struct AAA_3 {
    float a[10];
};

struct AAAA_3 {
    float a[15];
};

struct AAAAA_3 {
    float a[21];
};

struct a_3 {
    float a[3];
};

struct aa_3A_3 {
    float a[6];
};

ALWAYS_INLINE A_3 einsum_a_ab_out_b_known_1(const a_3 &t_0) {
    A_3 result;
    result.a[0] = t_0.a[0];
    result.a[1] = t_0.a[1];
    result.a[2] = t_0.a[2];
    return result;
}

ALWAYS_INLINE aa_3A_3 einsum_a_b_ab_group_0_1(const A_3 &t_0) {
    aa_3A_3 result;
    result.a[0] = t_0.a[0] * t_0.a[0];
    result.a[1] = t_0.a[0] * t_0.a[1];
    result.a[2] = t_0.a[0] * t_0.a[2];
    result.a[3] = t_0.a[1] * t_0.a[1];
    result.a[4] = t_0.a[1] * t_0.a[2];
    result.a[5] = t_0.a[2] * t_0.a[2];
    return result;
}

ALWAYS_INLINE AAA_3 einsum_a_bc_abc(const A_3 &t_0, const aa_3A_3 &t_1) {
    AAA_3 result;
    result.a[0] = t_0.a[0] * t_1.a[0];
    result.a[1] = t_0.a[0] * t_1.a[1];
    result.a[2] = t_0.a[0] * t_1.a[2];
    result.a[3] = t_0.a[0] * t_1.a[3];
    result.a[4] = t_0.a[0] * t_1.a[4];
    result.a[5] = t_0.a[0] * t_1.a[5];
    result.a[6] = t_0.a[1] * t_1.a[3];
    result.a[7] = t_0.a[1] * t_1.a[4];
    result.a[8] = t_0.a[1] * t_1.a[5];
    result.a[9] = t_0.a[2] * t_1.a[5];
    return result;
}

ALWAYS_INLINE AAAA_3 einsum_a_bcd_abcd(const A_3 &t_0, const AAA_3 &t_1) {
    AAAA_3 result;
    result.a[0] = t_0.a[0] * t_1.a[0];
    result.a[1] = t_0.a[0] * t_1.a[1];
    result.a[2] = t_0.a[0] * t_1.a[2];
    result.a[3] = t_0.a[0] * t_1.a[3];
    result.a[4] = t_0.a[0] * t_1.a[4];
    result.a[5] = t_0.a[0] * t_1.a[5];
    result.a[6] = t_0.a[0] * t_1.a[6];
    result.a[7] = t_0.a[0] * t_1.a[7];
    result.a[8] = t_0.a[0] * t_1.a[8];
    result.a[9] = t_0.a[0] * t_1.a[9];
    result.a[10] = t_0.a[1] * t_1.a[6];
    result.a[11] = t_0.a[1] * t_1.a[7];
    result.a[12] = t_0.a[1] * t_1.a[8];
    result.a[13] = t_0.a[1] * t_1.a[9];
    result.a[14] = t_0.a[2] * t_1.a[9];
    return result;
}

ALWAYS_INLINE AAAAA_3 einsum_a_bcde_abcde(const A_3 &t_0, const AAAA_3 &t_1) {
    AAAAA_3 result;
    result.a[0] = t_0.a[0] * t_1.a[0];
    result.a[1] = t_0.a[0] * t_1.a[1];
    result.a[2] = t_0.a[0] * t_1.a[2];
    result.a[3] = t_0.a[0] * t_1.a[3];
    result.a[4] = t_0.a[0] * t_1.a[4];
    result.a[5] = t_0.a[0] * t_1.a[5];
    result.a[6] = t_0.a[0] * t_1.a[6];
    result.a[7] = t_0.a[0] * t_1.a[7];
    result.a[8] = t_0.a[0] * t_1.a[8];
    result.a[9] = t_0.a[0] * t_1.a[9];
    result.a[10] = t_0.a[0] * t_1.a[10];
    result.a[11] = t_0.a[0] * t_1.a[11];
    result.a[12] = t_0.a[0] * t_1.a[12];
    result.a[13] = t_0.a[0] * t_1.a[13];
    result.a[14] = t_0.a[0] * t_1.a[14];
    result.a[15] = t_0.a[1] * t_1.a[10];
    result.a[16] = t_0.a[1] * t_1.a[11];
    result.a[17] = t_0.a[1] * t_1.a[12];
    result.a[18] = t_0.a[1] * t_1.a[13];
    result.a[19] = t_0.a[1] * t_1.a[14];
    result.a[20] = t_0.a[2] * t_1.a[14];
    return result;
}

ALWAYS_INLINE float einsum_a_a(const a_3 &t_0, const A_3 &t_1) {
    float result;
    result = t_0.a[0] * t_1.a[0];
    result += t_0.a[1] * t_1.a[1];
    result += t_0.a[2] * t_1.a[2];
    return result;
}

ALWAYS_INLINE float scale(const float &factor, const float &tensor) {
    float result;
    result = factor * tensor;
    return result;
}

ALWAYS_INLINE A_3 einsum_0_a_0_a_known_0_2(const A_3 &t_1) {
    A_3 result;
    result.a[0] = t_1.a[0];
    result.a[1] = t_1.a[1];
    result.a[2] = t_1.a[2];
    return result;
}

ALWAYS_INLINE A_3 einsum_0_a_a(const float &t_0, const A_3 &t_1) {
    A_3 result;
    result.a[0] = t_0 * t_1.a[0];
    result.a[1] = t_0 * t_1.a[1];
    result.a[2] = t_0 * t_1.a[2];
    return result;
}

ALWAYS_INLINE AA_3 einsum_0_ab_0_ab_known_0_2(const aa_3A_3 &t_1) {
    AA_3 result;
    result.a[0] = t_1.a[0];
    result.a[1] = t_1.a[1];
    result.a[2] = t_1.a[2];
    result.a[3] = t_1.a[3];
    result.a[4] = t_1.a[4];
    result.a[5] = t_1.a[5];
    return result;
}

ALWAYS_INLINE AA_3 einsum_0_ab_ab(const float &t_0, const AA_3 &t_1) {
    AA_3 result;
    result.a[0] = t_0 * t_1.a[0];
    result.a[1] = t_0 * t_1.a[1];
    result.a[2] = t_0 * t_1.a[2];
    result.a[3] = t_0 * t_1.a[3];
    result.a[4] = t_0 * t_1.a[4];
    result.a[5] = t_0 * t_1.a[5];
    return result;
}

ALWAYS_INLINE AA_3 einsum_0_ab_ab_known_1(const float &t_0) {
    AA_3 result;
    result.a[0] = t_0;
    result.a[3] = t_0;
    result.a[5] = t_0;
    result.a[1] = 0;
    result.a[2] = 0;
    result.a[4] = 0;
    return result;
}

ALWAYS_INLINE AA_3 add(const AA_3 &lhs, const AA_3 &rhs) {
    AA_3 result;
    result.a[0] = lhs.a[0] + rhs.a[0];
    result.a[1] = lhs.a[1] + rhs.a[1];
    result.a[2] = lhs.a[2] + rhs.a[2];
    result.a[3] = lhs.a[3] + rhs.a[3];
    result.a[4] = lhs.a[4] + rhs.a[4];
    result.a[5] = lhs.a[5] + rhs.a[5];
    return result;
}

ALWAYS_INLINE AAA_3 einsum_0_a_bc_abc_known_0_2(const A_3 &t_1) {
    AAA_3 result;
    result.a[0] = 3 * t_1.a[0];
    result.a[3] = t_1.a[0];
    result.a[5] = t_1.a[0];
    result.a[1] = t_1.a[1];
    result.a[6] = 3 * t_1.a[1];
    result.a[8] = t_1.a[1];
    result.a[2] = t_1.a[2];
    result.a[7] = t_1.a[2];
    result.a[9] = 3 * t_1.a[2];
    result.a[4] = 0;
    return result;
}

ALWAYS_INLINE AAA_3 einsum_0_abc_abc(const float &t_0, const AAA_3 &t_1) {
    AAA_3 result;
    result.a[0] = t_0 * t_1.a[0];
    result.a[1] = t_0 * t_1.a[1];
    result.a[2] = t_0 * t_1.a[2];
    result.a[3] = t_0 * t_1.a[3];
    result.a[4] = t_0 * t_1.a[4];
    result.a[5] = t_0 * t_1.a[5];
    result.a[6] = t_0 * t_1.a[6];
    result.a[7] = t_0 * t_1.a[7];
    result.a[8] = t_0 * t_1.a[8];
    result.a[9] = t_0 * t_1.a[9];
    return result;
}

ALWAYS_INLINE AAA_3 einsum_0_abc_0_abc_known_0_2(const AAA_3 &t_1) {
    AAA_3 result;
    result.a[0] = t_1.a[0];
    result.a[1] = t_1.a[1];
    result.a[2] = t_1.a[2];
    result.a[3] = t_1.a[3];
    result.a[4] = t_1.a[4];
    result.a[5] = t_1.a[5];
    result.a[6] = t_1.a[6];
    result.a[7] = t_1.a[7];
    result.a[8] = t_1.a[8];
    result.a[9] = t_1.a[9];
    return result;
}

ALWAYS_INLINE AAA_3 add(const AAA_3 &lhs, const AAA_3 &rhs) {
    AAA_3 result;
    result.a[0] = lhs.a[0] + rhs.a[0];
    result.a[1] = lhs.a[1] + rhs.a[1];
    result.a[2] = lhs.a[2] + rhs.a[2];
    result.a[3] = lhs.a[3] + rhs.a[3];
    result.a[4] = lhs.a[4] + rhs.a[4];
    result.a[5] = lhs.a[5] + rhs.a[5];
    result.a[6] = lhs.a[6] + rhs.a[6];
    result.a[7] = lhs.a[7] + rhs.a[7];
    result.a[8] = lhs.a[8] + rhs.a[8];
    result.a[9] = lhs.a[9] + rhs.a[9];
    return result;
}

ALWAYS_INLINE AAAA_3 einsum_0_ab_cd_abcd_known_0_2(const aa_3A_3 &t_1) {
    AAAA_3 result;
    result.a[0] = 6 * t_1.a[0];
    result.a[3] = t_1.a[0];
    result.a[5] = t_1.a[0];
    result.a[1] = 3 * t_1.a[1];
    result.a[6] = 3 * t_1.a[1];
    result.a[8] = t_1.a[1];
    result.a[2] = 3 * t_1.a[2];
    result.a[7] = t_1.a[2];
    result.a[9] = 3 * t_1.a[2];
    result.a[3] += t_1.a[3];
    result.a[10] = 6 * t_1.a[3];
    result.a[12] = t_1.a[3];
    result.a[4] = t_1.a[4];
    result.a[11] = 3 * t_1.a[4];
    result.a[13] = 3 * t_1.a[4];
    result.a[5] += t_1.a[5];
    result.a[12] += t_1.a[5];
    result.a[14] = 6 * t_1.a[5];
    return result;
}

ALWAYS_INLINE AAAA_3 einsum_0_abcd_abcd(const float &t_0, const AAAA_3 &t_1) {
    AAAA_3 result;
    result.a[0] = t_0 * t_1.a[0];
    result.a[1] = t_0 * t_1.a[1];
    result.a[2] = t_0 * t_1.a[2];
    result.a[3] = t_0 * t_1.a[3];
    result.a[4] = t_0 * t_1.a[4];
    result.a[5] = t_0 * t_1.a[5];
    result.a[6] = t_0 * t_1.a[6];
    result.a[7] = t_0 * t_1.a[7];
    result.a[8] = t_0 * t_1.a[8];
    result.a[9] = t_0 * t_1.a[9];
    result.a[10] = t_0 * t_1.a[10];
    result.a[11] = t_0 * t_1.a[11];
    result.a[12] = t_0 * t_1.a[12];
    result.a[13] = t_0 * t_1.a[13];
    result.a[14] = t_0 * t_1.a[14];
    return result;
}

ALWAYS_INLINE AAAA_3 einsum_0_abcd_abcd_known_1(const float &t_0) {
    AAAA_3 result;
    result.a[0] = 3 * t_0;
    result.a[3] = t_0;
    result.a[5] = t_0;
    result.a[10] = 3 * t_0;
    result.a[12] = t_0;
    result.a[14] = 3 * t_0;
    result.a[1] = 0;
    result.a[2] = 0;
    result.a[4] = 0;
    result.a[6] = 0;
    result.a[7] = 0;
    result.a[8] = 0;
    result.a[9] = 0;
    result.a[11] = 0;
    result.a[13] = 0;
    return result;
}

ALWAYS_INLINE AAAA_3 einsum_0_abcd_0_abcd_known_0_2(const AAAA_3 &t_1) {
    AAAA_3 result;
    result.a[0] = t_1.a[0];
    result.a[1] = t_1.a[1];
    result.a[2] = t_1.a[2];
    result.a[3] = t_1.a[3];
    result.a[4] = t_1.a[4];
    result.a[5] = t_1.a[5];
    result.a[6] = t_1.a[6];
    result.a[7] = t_1.a[7];
    result.a[8] = t_1.a[8];
    result.a[9] = t_1.a[9];
    result.a[10] = t_1.a[10];
    result.a[11] = t_1.a[11];
    result.a[12] = t_1.a[12];
    result.a[13] = t_1.a[13];
    result.a[14] = t_1.a[14];
    return result;
}

ALWAYS_INLINE AAAA_3 add(const AAAA_3 &lhs, const AAAA_3 &rhs) {
    AAAA_3 result;
    result.a[0] = lhs.a[0] + rhs.a[0];
    result.a[1] = lhs.a[1] + rhs.a[1];
    result.a[2] = lhs.a[2] + rhs.a[2];
    result.a[3] = lhs.a[3] + rhs.a[3];
    result.a[4] = lhs.a[4] + rhs.a[4];
    result.a[5] = lhs.a[5] + rhs.a[5];
    result.a[6] = lhs.a[6] + rhs.a[6];
    result.a[7] = lhs.a[7] + rhs.a[7];
    result.a[8] = lhs.a[8] + rhs.a[8];
    result.a[9] = lhs.a[9] + rhs.a[9];
    result.a[10] = lhs.a[10] + rhs.a[10];
    result.a[11] = lhs.a[11] + rhs.a[11];
    result.a[12] = lhs.a[12] + rhs.a[12];
    result.a[13] = lhs.a[13] + rhs.a[13];
    result.a[14] = lhs.a[14] + rhs.a[14];
    return result;
}

ALWAYS_INLINE AAAAA_3 einsum_0_a_bcde_abcde_known_0_2(const A_3 &t_1) {
    AAAAA_3 result;
    result.a[0] = 15 * t_1.a[0];
    result.a[3] = 3 * t_1.a[0];
    result.a[5] = 3 * t_1.a[0];
    result.a[10] = 3 * t_1.a[0];
    result.a[12] = t_1.a[0];
    result.a[14] = 3 * t_1.a[0];
    result.a[1] = 3 * t_1.a[1];
    result.a[6] = 3 * t_1.a[1];
    result.a[8] = t_1.a[1];
    result.a[15] = 15 * t_1.a[1];
    result.a[17] = 3 * t_1.a[1];
    result.a[19] = 3 * t_1.a[1];
    result.a[2] = 3 * t_1.a[2];
    result.a[7] = t_1.a[2];
    result.a[9] = 3 * t_1.a[2];
    result.a[16] = 3 * t_1.a[2];
    result.a[18] = 3 * t_1.a[2];
    result.a[20] = 15 * t_1.a[2];
    result.a[4] = 0;
    result.a[11] = 0;
    result.a[13] = 0;
    return result;
}

ALWAYS_INLINE AAAAA_3 einsum_0_abcde_abcde(const float &t_0, const AAAAA_3 &t_1) {
    AAAAA_3 result;
    result.a[0] = t_0 * t_1.a[0];
    result.a[1] = t_0 * t_1.a[1];
    result.a[2] = t_0 * t_1.a[2];
    result.a[3] = t_0 * t_1.a[3];
    result.a[4] = t_0 * t_1.a[4];
    result.a[5] = t_0 * t_1.a[5];
    result.a[6] = t_0 * t_1.a[6];
    result.a[7] = t_0 * t_1.a[7];
    result.a[8] = t_0 * t_1.a[8];
    result.a[9] = t_0 * t_1.a[9];
    result.a[10] = t_0 * t_1.a[10];
    result.a[11] = t_0 * t_1.a[11];
    result.a[12] = t_0 * t_1.a[12];
    result.a[13] = t_0 * t_1.a[13];
    result.a[14] = t_0 * t_1.a[14];
    result.a[15] = t_0 * t_1.a[15];
    result.a[16] = t_0 * t_1.a[16];
    result.a[17] = t_0 * t_1.a[17];
    result.a[18] = t_0 * t_1.a[18];
    result.a[19] = t_0 * t_1.a[19];
    result.a[20] = t_0 * t_1.a[20];
    return result;
}

ALWAYS_INLINE AAAAA_3 einsum_0_abc_de_abcde_known_0_2(const AAA_3 &t_1) {
    AAAAA_3 result;
    result.a[0] = 10 * t_1.a[0];
    result.a[3] = t_1.a[0];
    result.a[5] = t_1.a[0];
    result.a[1] = 6 * t_1.a[1];
    result.a[6] = 3 * t_1.a[1];
    result.a[8] = t_1.a[1];
    result.a[2] = 6 * t_1.a[2];
    result.a[7] = t_1.a[2];
    result.a[9] = 3 * t_1.a[2];
    result.a[3] += 3 * t_1.a[3];
    result.a[10] = 6 * t_1.a[3];
    result.a[12] = t_1.a[3];
    result.a[4] = 3 * t_1.a[4];
    result.a[11] = 3 * t_1.a[4];
    result.a[13] = 3 * t_1.a[4];
    result.a[5] += 3 * t_1.a[5];
    result.a[12] += t_1.a[5];
    result.a[14] = 6 * t_1.a[5];
    result.a[6] += t_1.a[6];
    result.a[15] = 10 * t_1.a[6];
    result.a[17] = t_1.a[6];
    result.a[7] += t_1.a[7];
    result.a[16] = 6 * t_1.a[7];
    result.a[18] = 3 * t_1.a[7];
    result.a[8] += t_1.a[8];
    result.a[17] += 3 * t_1.a[8];
    result.a[19] = 6 * t_1.a[8];
    result.a[9] += t_1.a[9];
    result.a[18] += t_1.a[9];
    result.a[20] = 10 * t_1.a[9];
    return result;
}

ALWAYS_INLINE AAAAA_3 einsum_0_abcde_0_abcde_known_0_2(const AAAAA_3 &t_1) {
    AAAAA_3 result;
    result.a[0] = t_1.a[0];
    result.a[1] = t_1.a[1];
    result.a[2] = t_1.a[2];
    result.a[3] = t_1.a[3];
    result.a[4] = t_1.a[4];
    result.a[5] = t_1.a[5];
    result.a[6] = t_1.a[6];
    result.a[7] = t_1.a[7];
    result.a[8] = t_1.a[8];
    result.a[9] = t_1.a[9];
    result.a[10] = t_1.a[10];
    result.a[11] = t_1.a[11];
    result.a[12] = t_1.a[12];
    result.a[13] = t_1.a[13];
    result.a[14] = t_1.a[14];
    result.a[15] = t_1.a[15];
    result.a[16] = t_1.a[16];
    result.a[17] = t_1.a[17];
    result.a[18] = t_1.a[18];
    result.a[19] = t_1.a[19];
    result.a[20] = t_1.a[20];
    return result;
}

ALWAYS_INLINE AAAAA_3 add(const AAAAA_3 &lhs, const AAAAA_3 &rhs) {
    AAAAA_3 result;
    result.a[0] = lhs.a[0] + rhs.a[0];
    result.a[1] = lhs.a[1] + rhs.a[1];
    result.a[2] = lhs.a[2] + rhs.a[2];
    result.a[3] = lhs.a[3] + rhs.a[3];
    result.a[4] = lhs.a[4] + rhs.a[4];
    result.a[5] = lhs.a[5] + rhs.a[5];
    result.a[6] = lhs.a[6] + rhs.a[6];
    result.a[7] = lhs.a[7] + rhs.a[7];
    result.a[8] = lhs.a[8] + rhs.a[8];
    result.a[9] = lhs.a[9] + rhs.a[9];
    result.a[10] = lhs.a[10] + rhs.a[10];
    result.a[11] = lhs.a[11] + rhs.a[11];
    result.a[12] = lhs.a[12] + rhs.a[12];
    result.a[13] = lhs.a[13] + rhs.a[13];
    result.a[14] = lhs.a[14] + rhs.a[14];
    result.a[15] = lhs.a[15] + rhs.a[15];
    result.a[16] = lhs.a[16] + rhs.a[16];
    result.a[17] = lhs.a[17] + rhs.a[17];
    result.a[18] = lhs.a[18] + rhs.a[18];
    result.a[19] = lhs.a[19] + rhs.a[19];
    result.a[20] = lhs.a[20] + rhs.a[20];
    return result;
}

ALWAYS_INLINE void
get_multipole_derivatives(float &derivative_0, A_3 &derivative_1, AA_3 &derivative_2, AAA_3 &derivative_3,
                          AAAA_3 &derivative_4, AAAAA_3 &derivative_5, const a_3 &point) {
    //
    A_3 pA_power_1 = einsum_a_ab_out_b_known_1(point);
    // fma: 6
    aa_3A_3 pA_power_2 = einsum_a_b_ab_group_0_1(pA_power_1);
    // fma: 10
    AAA_3 pA_power_3 = einsum_a_bc_abc(pA_power_1, pA_power_2);
    // fma: 15
    AAAA_3 pA_power_4 = einsum_a_bcd_abcd(pA_power_1, pA_power_3);
    // fma: 21
    AAAAA_3 pA_power_5 = einsum_a_bcde_abcde(pA_power_1, pA_power_4);
    // fma: 3
    float tmp = einsum_a_a(point, pA_power_1);
    // sqrt: 1
    float g_0 = 1 / std::sqrt(tmp);
    // fma: 1
    float g_power_1 = scale(1, g_0);
    // fma: 1
    float g_power_2 = g_power_1 * g_0;
    // fma: 1
    float g_power_3 = g_power_2 * g_0;
    // fma: 1
    float g_power_4 = g_power_3 * g_0;
    // fma: 1
    float g_power_5 = g_power_4 * g_0;
    // fma: 1
    float g_power_6 = g_power_5 * g_0;
    // fma: 1
    float g_power_7 = g_power_6 * g_0;
    // fma: 1
    float g_power_8 = g_power_7 * g_0;
    // fma: 1
    float g_power_9 = g_power_8 * g_0;
    // fma: 1
    float g_power_10 = g_power_9 * g_0;
    // fma: 1
    float g_power_11 = g_power_10 * g_0;
    // fma: 1
    float tmp_01 = scale(1, g_power_1);
    //
    float g_0_01 = tmp_01;
    // fma: 1
    float tmp_02 = scale(-1, g_power_3);
    //
    A_3 tmp_03 = einsum_0_a_0_a_known_0_2(pA_power_1);
    // fma: 3
    A_3 g_1 = einsum_0_a_a(tmp_02, tmp_03);
    // fma: 1
    float tmp_04 = scale(3, g_power_5);
    //
    AA_3 tmp_05 = einsum_0_ab_0_ab_known_0_2(pA_power_2);
    // fma: 6
    AA_3 tmp_06 = einsum_0_ab_ab(tmp_04, tmp_05);
    // fma: 1
    float tmp_07 = scale(-1, g_power_3);
    //
    AA_3 tmp_08 = einsum_0_ab_ab_known_1(tmp_07);
    // fma: 6
    AA_3 g_2 = add(tmp_06, tmp_08);
    // fma: 1
    float tmp_09 = scale(3, g_power_5);
    // fma: 3
    AAA_3 tmp_10 = einsum_0_a_bc_abc_known_0_2(pA_power_1);
    // fma: 10
    AAA_3 tmp_11 = einsum_0_abc_abc(tmp_09, tmp_10);
    // fma: 1
    float tmp_12 = scale(-15, g_power_7);
    //
    AAA_3 tmp_13 = einsum_0_abc_0_abc_known_0_2(pA_power_3);
    // fma: 10
    AAA_3 tmp_14 = einsum_0_abc_abc(tmp_12, tmp_13);
    // fma: 10
    AAA_3 g_3 = add(tmp_11, tmp_14);
    // fma: 1
    float tmp_15 = scale(-15, g_power_7);
    // fma: 9
    AAAA_3 tmp_16 = einsum_0_ab_cd_abcd_known_0_2(pA_power_2);
    // fma: 15
    AAAA_3 tmp_17 = einsum_0_abcd_abcd(tmp_15, tmp_16);
    // fma: 1
    float tmp_18 = scale(3, g_power_5);
    // fma: 3
    AAAA_3 tmp_19 = einsum_0_abcd_abcd_known_1(tmp_18);
    // fma: 1
    float tmp_20 = scale(1.05e+02f, g_power_9);
    //
    AAAA_3 tmp_21 = einsum_0_abcd_0_abcd_known_0_2(pA_power_4);
    // fma: 15
    AAAA_3 tmp_22 = einsum_0_abcd_abcd(tmp_20, tmp_21);
    // fma: 15
    AAAA_3 tmp_23 = add(tmp_17, tmp_19);
    // fma: 15
    AAAA_3 g_4 = add(tmp_23, tmp_22);
    // fma: 1
    float tmp_24 = scale(-15, g_power_7);
    // fma: 15
    AAAAA_3 tmp_25 = einsum_0_a_bcde_abcde_known_0_2(pA_power_1);
    // fma: 21
    AAAAA_3 tmp_26 = einsum_0_abcde_abcde(tmp_24, tmp_25);
    // fma: 1
    float tmp_27 = scale(1.05e+02f, g_power_9);
    // fma: 21
    AAAAA_3 tmp_28 = einsum_0_abc_de_abcde_known_0_2(pA_power_3);
    // fma: 21
    AAAAA_3 tmp_29 = einsum_0_abcde_abcde(tmp_27, tmp_28);
    // fma: 1
    float remaining_factor = scale(-9.45e+02f, g_power_11);
    //
    AAAAA_3 tmp_30 = einsum_0_abcde_0_abcde_known_0_2(pA_power_5);
    // fma: 21
    AAAAA_3 term = einsum_0_abcde_abcde(remaining_factor, tmp_30);
    // fma: 21
    AAAAA_3 tmp_31 = add(tmp_26, tmp_29);
    // fma: 21
    AAAAA_3 g_5 = add(tmp_31, term);
    derivative_0 = g_0_01;
    derivative_1 = g_1;
    derivative_2 = g_2;
    derivative_3 = g_3;
    derivative_4 = g_4;
    derivative_5 = g_5;
    // fma: 339, sqrt: 1
}

