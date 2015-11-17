#ifndef _MATH_API_H
#define _MATH_API_H

#ifdef __APPLE__
typedef signed  char            INT8,  *PINT8;
typedef signed  short           INT16, *PINT16;
typedef signed  int             INT32, *PINT32;
typedef signed  long long       INT64, *PINT64;
typedef unsigned  char          UINT8,  *PUINT8;
typedef unsigned  short         UINT16, *PUINT16;
typedef unsigned  int           UINT32, *PUINT32;
typedef unsigned  long long     UINT64, *PUINT64;
#endif

typedef char                    S01;
typedef char                    S02;
typedef char                    S03;
typedef char                    S04;
typedef char                    S05;
typedef char                    S06;
typedef char                    S07;
typedef char                    S08;
typedef short int               S09;
typedef short int               S10;
typedef short int               S11;
typedef short int               S12;
typedef short int               S13;
typedef short int               S14;
typedef short int               S15;
typedef short int               S16;
typedef int                     S17;
typedef int                     S18;
typedef int                     S19;
typedef int                     S20;
typedef int                     S21;
typedef int                     S22;
typedef int                     S23;
typedef int                     S24;
typedef int                     S25;
typedef int                     S26;
typedef int                     S27;
typedef int                     S28;
typedef int                     S29;
typedef int                     S30;
typedef int                     S31;
typedef int                     S32;
typedef long long               S33;
typedef long long               S34;
typedef long long               S35;
typedef long long               S36;
typedef long long               S37;
typedef long long               S38;
typedef long long               S39;
typedef long long               S40;
typedef long long               S41;
typedef long long               S42;
typedef long long               S43;
typedef long long               S44;
typedef long long               S45;
typedef long long               S46;
typedef long long               S47;
typedef long long               S48;
typedef long long               S49;
typedef long long               S50;
typedef long long               S51;
typedef long long               S52;
typedef long long               S53;
typedef long long               S54;
typedef long long               S55;
typedef long long               S56;
typedef long long               S57;
typedef long long               S58;
typedef long long               S59;
typedef long long               S60;
typedef long long               S61;
typedef long long               S62;
typedef long long               S63;
typedef long long               S64;

// Unsigned values.
typedef unsigned char           U01;
typedef unsigned char           U02;
typedef unsigned char           U03;
typedef unsigned char           U04;
typedef unsigned char           U05;
typedef unsigned char           U06;
typedef unsigned char           U07;
typedef unsigned char           U08;
typedef unsigned short int      U09;
typedef unsigned short int      U10;
typedef unsigned short int      U11;
typedef unsigned short int      U12;
typedef unsigned short int      U13;
typedef unsigned short int      U14;
typedef unsigned short int      U15;
typedef unsigned short int      U16;
typedef unsigned int            U17;
typedef unsigned int            U18;
typedef unsigned int            U19;
typedef unsigned int            U20;
typedef unsigned int            U21;
typedef unsigned int            U22;
typedef unsigned int            U23;
typedef unsigned int            U24;
typedef unsigned int            U25;
typedef unsigned int            U26;
typedef unsigned int            U27;
typedef unsigned int            U28;
typedef unsigned int            U29;
typedef unsigned int            U30;
typedef unsigned int            U31;
typedef unsigned int            U32;
typedef unsigned long long      U33;
typedef unsigned long long      U34;
typedef unsigned long long      U35;
typedef unsigned long long      U36;
typedef unsigned long long      U37;
typedef unsigned long long      U38;
typedef unsigned long long      U39;
typedef unsigned long long      U40;
typedef unsigned long long      U41;
typedef unsigned long long      U42;
typedef unsigned long long      U43;
typedef unsigned long long      U44;
typedef unsigned long long      U45;
typedef unsigned long long      U46;
typedef unsigned long long      U47;
typedef unsigned long long      U48;
typedef unsigned long long      U49;
typedef unsigned long long      U50;
typedef unsigned long long      U51;
typedef unsigned long long      U52;
typedef unsigned long long      U53;
typedef unsigned long long      U54;
typedef unsigned long long      U55;
typedef unsigned long long      U56;
typedef unsigned long long      U57;
typedef unsigned long long      U58;
typedef unsigned long long      U59;
typedef unsigned long long      U60;
typedef unsigned long long      U61;
typedef unsigned long long      U62;
typedef unsigned long long      U63;
typedef unsigned long long      U64;

typedef U16                     F_S1E5M10;
typedef U32                     F_S1E8M23;

// Add/sub functions definition.
#define ADD                             0
#define SUB                             1

///////////////////////////////////////////////////////////////////////////////
// 32-bit floating point defines.

#define SINGLEFLOAT                     32
#define SINGLEFLOAT_SIGN                1
#define GET_SINGLEFLOAT_SIGN(x)         ((x >> (32 - 1)) & 0x01)
#define SINGLEFLOAT_EXP                 8
#define SINGLEFLOAT_EXP_MASK            0xFF
#define SINGLEFLOAT_EXP_BIAS            (SINGLEFLOAT_EXP_MASK >> 1)
#define GET_SINGLEFLOAT_EXP(x)          ((x >> (32 - 1 - 8)) & SINGLEFLOAT_EXP_MASK)
#define SINGLEFLOAT_MAN                 23
#define SINGLEFLOAT_SIGNIFICANT         (1 + SINGLEFLOAT_MAN)
#define SINGLEFLOAT_MAN16               15
#define SINGLEFLOAT_MAN_16              7
#define SINGLEFLOAT_MAN_MASK            0x7FFFFF
#define SINGLEFLOAT_MAN_MASK_PLUS       0xFFFFFF
#define SINGLEFLOAT_MAN_MASK_24         0x7FFF00
#define GET_SINGLEFLOAT_MAN(x)          ((x & SINGLEFLOAT_MAN_MASK) | 0x800000)
#define FORM_SINGLEFLOAT(s,e,m)         (((s)<<(32 - 1))|((e)<<(32 - 9))|(m))
#define FLOAT_NOT_A_MAX_MANTISSA        0x7FFFFF
#define FLOAT_NOT_A_NUM_MANTISSA        0x000000  // intel is 0x400000
#define FLOAT_NOT_A_NUM_MANTISSA_16     0x0000  // intel is 0x400000
#define INVERTSINGLEFLOATSIGN(x)    (x^0x80000000)
#define SINGLEFLOAT_EXP_INFINITY        0xFF
#define SINGLEFLOAT_FINITY_MAX_EXP      0xFE
#define SINGLEFLOAT_ONE                 0x3f800000
#define SINGLEFLOAT_NEG_ONE             0xbf800000
#define SINGLEFLOAT_TWO                 0x40000000
#define SINGLEFLOAT_ZERO                0x00000000
#define SINGLEFLOAT_NEG_ZERO            0x80000000
#define SINGLEFLOAT_DOT5                0x3f000000
#define SINGLEFLOAT_255                 0x437F0000
#define SINGLEFLOAT_ONE_OVER127         0x3c010204
#define SINGLEFLOAT_ONE_OVER255         0x3b808081
#define SINGLEFLOAT_ONE_OVER511         0x3b004020
#define SINGLEFLOAT_ONE_OVER1023        0x3a802008
#define SINGLEFLOAT_ONE_OVER32767       0x38000100
#define SINGLEFLOAT_ONE_OVER65535       0x37800080
#define SINGLEFLOAT_TWOOVER65535    0x38000080
#define SINGLEFLOAT_ONE_OVER7FFFFFFF    0x30000000
#define SINGLEFLOAT_ONE_OVERFFFFFFFF    0x2f800000
#define SINGLEFLOAT_ONE_OVER3           0x3EAAAAAB
#define NEGFLOATSMALLFRAC           0xbd800000
#define FLOATLARGEFRAC              0x3f700000
#define SINGLEFLOAT_POSITIVE_NAN        0x7fc00000
#define SINGLEFLOAT_NEGTIVE_NAN         0xffc00000
#define SINGLEFLOAT_POSITIVE_INF        0x7f800000
#define SINGLEFLOAT_NEGTIVE_INF         0xff800000
#define SINGLEFLOAT_POSITIVE_ZERO       0x00000000
#define SINGLEFLOAT_NEGTIVE_ZERO        0x80000000
#define HALFFLOAT                   16
#define HALFFLOATSIGN               1
#define GETHALFFLOATSIGN(x)         ((x >> (16 - 1)) & 0x01)
#define HALFFLOATEXP                5
#define HALFFLOATEXPMASK            0x1F
#define HALFFLOATEXPBIAS            (HALFFLOATEXPMASK >> 1)
#define GETHALFFLOATEXP(x)          ((x >> (16 - 1 - 5)) & HALFFLOATEXPMASK)
#define HALFFLOATMAN                10
#define HALFFLOATSIGNIFICANT        (1 + HALFFLOATMAN)
#define HALFFLOATMANMASK            0x3FF
#define HALFFLOATMANMASKPLUS        0x7FF
#define GETHALFFLOATMAN(x)          ((x & HALFFLOATMANMASK) | 0x400)
#define HALFFLOATPOSITIVEZERO       0x0000
#define HALFFLOATNEGTIVEZERO        0x8000
#define HALFFLOATEXPINFINITY        0x1F
#define HALFFLOATFINITYMAXEXP       0x1E

// Defines and constants for 16-bit floating point.
#define FLOAT16_SIGN_MASK               0x8000
#define FLOAT16_EXPONENT_SIZE           5
#define FLOAT16_EXPONENT_MASK           ((1 << FLOAT16_EXPONENT_SIZE) - 1)
#define FLOAT16_MANTISSA_SIZE           10
#define FLOAT16_MANTISSA_MASK           ((1 << FLOAT16_MANTISSA_SIZE) - 1)
#define FLOAT16_BIAS                    (FLOAT16_EXPONENT_MASK >> 1)
#define FLOAT16_INFINITY                FLOAT16_EXPONENT_MASK
#define FLOAT16_EXTRABITS               2

#define ADD_SUB_EXTRA_BITS              2  // used for rounding and over flow protection

#define FLOAT17_SIGN_MASK               0x10000
#define FLOAT17_EXPONENT_SIZE           6
#define FLOAT17_EXPONENT_MASK           ((1 << FLOAT17_EXPONENT_SIZE) - 1)
#define FLOAT17_MANTISSA_SIZE           10
#define FLOAT17_MANTISSA_MASK           ((1 << FLOAT17_MANTISSA_SIZE) - 1)
#define FLOAT17_MANTISSA_MASK_PLUS      0x7FF
#define FLOAT17_BIAS                    (FLOAT17_EXPONENT_MASK >> 1)
#define FLOAT17_INFINITY                FLOAT17_EXPONENT_MASK
#define FLOAT17_EXTRABITS               2
#define FLOAT17_EXP_INFINITY            0x2F
#define FLOAT17_MAN                     10

/*******************************************************************************
** MIN and MAX templates.
*/

template <typename DATATYPE>
inline const DATATYPE& _max(
    const DATATYPE& _left,
    const DATATYPE& _right
    )
{
    return (_left < _right)
        ? _right
        : _left;
}

template <typename DATATYPE>
inline const DATATYPE& _min(
    const DATATYPE& _left,
    const DATATYPE& _right
    )
{
    return (_right < _left)
        ? _right
        : _left;
}

/*******************************************************************************
** Compute bit mask.
*/

template <typename DATATYPE>
inline DATATYPE COMPUTEMASK(
    const unsigned int Position,
    const unsigned int Count
    )
{
    // Define the result.
    U64 result;

    // Compute the mask.
    result = Count
        ? ((U64) ~0) >> (64 - Count)
        : 0;

    // Shift to the proper position.
    result = result << Position;

    // Return result.
    return (DATATYPE) result;
}

template <typename DATATYPE>
inline DATATYPE GET_BITS(
    const DATATYPE Data,
    const unsigned int Start,
    const unsigned int End
    )
{
    // Sort the end points.
    const unsigned int _Start = _min(Start, End);
    const unsigned int _End   = _max(Start, End);

    // Determine the size of the field.
    const unsigned int _Size = _End - _Start + 1;

    // Compute the mask.
    const DATATYPE _Mask = COMPUTEMASK<DATATYPE>(0, _Size);

    // Extract the value.
    const DATATYPE _Value = (Data >> _Start) & _Mask;

    // Return the result.
    return _Value;
}

template <typename DATATYPE>
inline U01 GET_BIT(
    const DATATYPE Data,
    const unsigned int Position
    )
{
    return U01(GET_BITS(Data, Position, Position));
}

/* MATH function */
U32 Float16ToFloat(U16 in);

#define MATH_DivideUInt64(X, Y) (GLuint64)((X) / (Y))

typedef struct _VECTOR4
{
    union
    {
        int     i[4];
        float   f[4];
    } u;
} VECTOR4;

typedef struct _MATRIX4x4
{
    union
    {
        int     i[16];
        float   f[16];
    } u;
} MATRIX4x4;

VECTOR4 Matrix4x4MultiplyVector4(MATRIX4x4 matrix, VECTOR4 vector);
VECTOR4 Matrix4x4TMultiplyVector4(MATRIX4x4 matrix, VECTOR4 vector);

#endif // _MATH_API_H