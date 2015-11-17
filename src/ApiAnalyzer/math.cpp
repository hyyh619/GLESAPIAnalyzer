#include "MathAPI.h"

inline U01 FLOAT16_GetSign(const F_S1E5M10 Float16)
{
    return U01(GET_BITS(Float16, 15, 15));
}

inline U05 FLOAT16_GetExponent(const F_S1E5M10 Float16)
{
    return U05(GET_BITS(Float16, 10, 14));
}

inline U11 FLOAT16_GetMantissa(const F_S1E5M10 Float16)
{
    return U11(GET_BITS(Float16, 0, 9)) | U11(1 << FLOAT16_MANTISSA_SIZE);
}

/*
** convert float16 to single float
*/
U32 Float16ToFloat(U16 in)
{
    U01 signIn = FLOAT16_GetSign(in);
    S06 expIn  = FLOAT16_GetExponent(in);

    if (expIn == 0)
    {
        return FORM_SINGLEFLOAT(signIn, 0, 0);
    }

    if (expIn == FLOAT16_INFINITY)
    {
        return FORM_SINGLEFLOAT(signIn, SINGLEFLOAT_EXP_INFINITY, 0);
    }

    U10 manIn = U10(FLOAT16_GetMantissa(in) & FLOAT16_MANTISSA_MASK);

    U08 expOut = expIn - FLOAT16_BIAS + SINGLEFLOAT_EXP_BIAS;
    U23 manOut = U23(manIn) << (SINGLEFLOAT_MAN - FLOAT16_MANTISSA_SIZE);

    return FORM_SINGLEFLOAT(signIn, expOut, manOut);
}

VECTOR4 Matrix4x4TMultiplyVector4(MATRIX4x4 matrix, VECTOR4 vector)
{
    VECTOR4 r;

    for (U32 i=0; i<4; i++)
    {
        r.u.f[0] = matrix.u.f[0] * vector.u.f[0] +
                   matrix.u.f[4] * vector.u.f[1] +
                   matrix.u.f[8] * vector.u.f[2] +
                   matrix.u.f[12] * vector.u.f[3];

        r.u.f[1] = matrix.u.f[1] * vector.u.f[0] +
                   matrix.u.f[5] * vector.u.f[1] +
                   matrix.u.f[9] * vector.u.f[2] +
                   matrix.u.f[13] * vector.u.f[3];

        r.u.f[2] = matrix.u.f[2] * vector.u.f[0] +
                   matrix.u.f[6] * vector.u.f[1] +
                   matrix.u.f[10] * vector.u.f[2] +
                   matrix.u.f[14] * vector.u.f[3];

        r.u.f[3] = matrix.u.f[3] * vector.u.f[0] +
                   matrix.u.f[7] * vector.u.f[1] +
                   matrix.u.f[11] * vector.u.f[2] +
                   matrix.u.f[15] * vector.u.f[3];
    }

    return r;
}

VECTOR4 Matrix4x4MultiplyVector4(MATRIX4x4 matrix, VECTOR4 vector)
{
    VECTOR4 r;

    for (U32 i=0; i<4; i++)
    {
        r.u.f[0] = matrix.u.f[0] * vector.u.f[0] +
                   matrix.u.f[1] * vector.u.f[1] +
                   matrix.u.f[2] * vector.u.f[2] +
                   matrix.u.f[3] * vector.u.f[3];

        r.u.f[1] = matrix.u.f[4] * vector.u.f[0] +
                   matrix.u.f[5] * vector.u.f[1] +
                   matrix.u.f[6] * vector.u.f[2] +
                   matrix.u.f[7] * vector.u.f[3];

        r.u.f[2] = matrix.u.f[8] * vector.u.f[0] +
                   matrix.u.f[9] * vector.u.f[1] +
                   matrix.u.f[10] * vector.u.f[2] +
                   matrix.u.f[11] * vector.u.f[3];

        r.u.f[3] = matrix.u.f[12] * vector.u.f[0] +
                   matrix.u.f[13] * vector.u.f[1] +
                   matrix.u.f[14] * vector.u.f[2] +
                   matrix.u.f[15] * vector.u.f[3];
    }

    return r;
}