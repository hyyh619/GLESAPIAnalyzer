#ifndef __API_DUMPUTILS_H
#define __API_DUMPUTILS_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#include <Windows.h>
#endif

#define DUMP_STR_LEN        512
#define TMP_BUF_SIZE        256

#define MIN(x, y)            (((x) <= (y)) ?  (x) :  (y))
#define MAX(x, y)            (((x) >= (y)) ?  (x) :  (y))
#define CLAMP(x, min, max)   (((x) < (min)) ? (min) : \
                              ((x) > (max)) ? (max) : (x))
#define ABS(x)               (((x) < 0)    ? -(x) :  (x))
#define NEG(x)               (((x) < 0)    ?  (x) : -(x))

extern GLchar    dumpTmp1[TMP_BUF_SIZE];
extern GLchar    dumpTmp2[TMP_BUF_SIZE];
extern GLchar    dumpTmp3[TMP_BUF_SIZE];
extern GLchar    dumpTmp4[TMP_BUF_SIZE];
extern GLchar    dumpTmp5[TMP_BUF_SIZE];
extern GLchar    dumpTmp6[TMP_BUF_SIZE];

// Signed values.
typedef char        S01;
typedef char        S02;
typedef char        S03;
typedef char        S04;
typedef char        S05;
typedef char        S06;
typedef char        S07;
typedef char        S08;
typedef short int   S09;
typedef short int   S10;
typedef short int   S11;
typedef short int   S12;
typedef short int   S13;
typedef short int   S14;
typedef short int   S15;
typedef short int   S16;
typedef int         S17;
typedef int         S18;
typedef int         S19;
typedef int         S20;
typedef int         S21;
typedef int         S22;
typedef int         S23;
typedef int         S24;
typedef int         S25;
typedef int         S26;
typedef int         S27;
typedef int         S28;
typedef int         S29;
typedef int         S30;
typedef int         S31;
typedef int         S32;
typedef long long   S33;
typedef long long   S34;
typedef long long   S35;
typedef long long   S36;
typedef long long   S37;
typedef long long   S38;
typedef long long   S39;
typedef long long   S40;
typedef long long   S41;
typedef long long   S42;
typedef long long   S43;
typedef long long   S44;
typedef long long   S45;
typedef long long   S46;
typedef long long   S47;
typedef long long   S48;
typedef long long   S49;
typedef long long   S50;
typedef long long   S51;
typedef long long   S52;
typedef long long   S53;
typedef long long   S54;
typedef long long   S55;
typedef long long   S56;
typedef long long   S57;
typedef long long   S58;
typedef long long   S59;
typedef long long   S60;
typedef long long   S61;
typedef long long   S62;
typedef long long   S63;
typedef long long   S64;

// Unsigned values.
typedef unsigned char       U01;
typedef unsigned char       U02;
typedef unsigned char       U03;
typedef unsigned char       U04;
typedef unsigned char       U05;
typedef unsigned char       U06;
typedef unsigned char       U07;
typedef unsigned char       U08;
typedef unsigned short int  U09;
typedef unsigned short int  U10;
typedef unsigned short int  U11;
typedef unsigned short int  U12;
typedef unsigned short int  U13;
typedef unsigned short int  U14;
typedef unsigned short int  U15;
typedef unsigned short int  U16;
typedef unsigned int        U17;
typedef unsigned int        U18;
typedef unsigned int        U19;
typedef unsigned int        U20;
typedef unsigned int        U21;
typedef unsigned int        U22;
typedef unsigned int        U23;
typedef unsigned int        U24;
typedef unsigned int        U25;
typedef unsigned int        U26;
typedef unsigned int        U27;
typedef unsigned int        U28;
typedef unsigned int        U29;
typedef unsigned int        U30;
typedef unsigned int        U31;
typedef unsigned int        U32;
typedef unsigned long long  U33;
typedef unsigned long long  U34;
typedef unsigned long long  U35;
typedef unsigned long long  U36;
typedef unsigned long long  U37;
typedef unsigned long long  U38;
typedef unsigned long long  U39;
typedef unsigned long long  U40;
typedef unsigned long long  U41;
typedef unsigned long long  U42;
typedef unsigned long long  U43;
typedef unsigned long long  U44;
typedef unsigned long long  U45;
typedef unsigned long long  U46;
typedef unsigned long long  U47;
typedef unsigned long long  U48;
typedef unsigned long long  U49;
typedef unsigned long long  U50;
typedef unsigned long long  U51;
typedef unsigned long long  U52;
typedef unsigned long long  U53;
typedef unsigned long long  U54;
typedef unsigned long long  U55;
typedef unsigned long long  U56;
typedef unsigned long long  U57;
typedef unsigned long long  U58;
typedef unsigned long long  U59;
typedef unsigned long long  U60;
typedef unsigned long long  U61;
typedef unsigned long long  U62;
typedef unsigned long long  U63;
typedef unsigned long long  U64;

typedef U16 FL16;
typedef U32 FL32;

typedef U16 F_S1E5M10;
typedef U32 F_S1E8M23;

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
inline DATATYPE GETBITS(
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

#define AQFLOAT16_SIGNMASK          0x8000
#define AQFLOAT16_EXPONENTSIZE      5
#define AQFLOAT16_EXPONENTMASK      ((1 << AQFLOAT16_EXPONENTSIZE) - 1)
#define AQFLOAT16_MANTISSASIZE      10
#define AQFLOAT16_MANTISSAMASK      ((1 << AQFLOAT16_MANTISSASIZE) - 1)
#define AQFLOAT16_BIAS              (AQFLOAT16_EXPONENTMASK >> 1)
#define AQFLOAT16_INFINITY          AQFLOAT16_EXPONENTMASK
#define AQFLOAT16_EXTRABITS         2

// Frequently used constants
#define AQFLOAT16_VALUEOF_0         0x0000
#define AQFLOAT16_VALUEOF_1         0x3C00
#define AQFLOAT16_VALUEOF_17        0x4C40
#define AQFLOAT16_VALUEOF_255       0x5BF8
#define AQFLOAT16_ONEOVER2          0x3800
#define AQFLOAT16_ONEOVER3          0x3555
#define AQFLOAT16_ONEOVER4          0x3400
#define AQFLOAT16_ONEOVER5          0x3266
#define AQFLOAT16_ONEOVER6          0x3155
#define AQFLOAT16_ONEOVER7          0x3092
#define AQFLOAT16_ONEOVER8          0x3000
#define AQFLOAT16_ONEOVER15         0x2C44
#define AQFLOAT16_ONEOVER31         0x2821
#define AQFLOAT16_ONEOVER63         0x2410
#define AQFLOAT16_ONEOVER255        0x1C04

inline U01 AQFLOAT16_GetSign(const F_S1E5M10 Float16)
{
   return U01(GETBITS(Float16, 15, 15));
}

inline U05 AQFLOAT16_GetExponent(const F_S1E5M10 Float16)
{
   return U05(GETBITS(Float16, 10, 14));
}

inline U11 AQFLOAT16_GetMantissa(const F_S1E5M10 Float16)
{
   return U11(GETBITS(Float16, 0, 9)) | U11(1 << AQFLOAT16_MANTISSASIZE);
}

inline F_S1E5M10 AQFLOAT16_Construct(U01 Sign, U05 Exponent, U10 Mantissa)
{
   return
      ((Sign & 0x0001) << 15)
      | ((Exponent & 0x001F) << 10)
      | (Mantissa & 0x03FF);
}

#define SINGLEFLOAT                 32
#define SINGLEFLOATSIGN             1
#define GETSINGLEFLOATSIGN(x)       ((x >> (32 - 1)) & 0x01)
#define SINGLEFLOATEXP              8
#define SINGLEFLOATEXPMASK          0xFF
#define SINGLEFLOATEXPBIAS          (SINGLEFLOATEXPMASK >> 1)
#define GETSINGLEFLOATEXP(x)        ((x >> (32 - 1 - 8)) & SINGLEFLOATEXPMASK)
#define SINGLEFLOATMAN              23
#define SINGLEFLOATSIGNIFICANT      (1 + SINGLEFLOATMAN)
#define SINGLEFLOATMAN16            15
#define SINGLEFLOATMAN_16           7
#define SINGLEFLOATMANMASK          0x7FFFFF
#define SINGLEFLOATMANMASKPLUS      0xFFFFFF
#define SINGLEFLOATMANMASK_24       0x7FFF00
#define GETSINGLEFLOATMAN(x)        ((x & SINGLEFLOATMANMASK) | 0x800000)
#define FORMSINGLEFLOAT(s,e,m)      (((s)<<(32 - 1))|((e)<<(32 - 9))|(m))
#define AQFLOATNOTAMAXMANTISA       0x7FFFFF
#define AQFLOATNOTANUMMANTISA       0x000000  // intel is 0x400000
#define AQFLOATNOTANUMMANTISA_16    0x0000  // intel is 0x400000
#define INVERTSINGLEFLOATSIGN(x)    (x^0x80000000)
#define SINGLEFLOATEXPINFINITY      0xFF
#define SINGLEFLOATFINITYMAXEXP     0xFE
#define SINGLEFLOATONE              0x3f800000
#define SINGLEFLOATNEGONE           0xbf800000
#define SINGLEFLOAT_TWO             0x40000000
#define SINGLEFLOATZERO             0x00000000
#define SINGLEFLOATNEGZERO          0x80000000
#define SINGLEFLOATDOT5             0x3f000000
#define SINGLEFLOAT255              0x437F0000
#define SINGLEFLOAT_ONEOVER127      0x3c010204
#define SINGLEFLOAT_ONEOVER255      0x3b808081
#define SINGLEFLOAT_ONEOVER511      0x3b004020
#define SINGLEFLOAT_ONEOVER1023     0x3a802008
#define SINGLEFLOAT_ONEOVER32767    0x38000100
#define SINGLEFLOAT_ONEOVER65535    0x37800080
#define SINGLEFLOAT_TWOOVER65535    0x38000080
#define SINGLEFLOAT_ONEOVER7FFFFFFF 0x30000000
#define SINGLEFLOAT_ONEOVERFFFFFFFF 0x2f800000
#define SINGLEFLOAT_ONEOVER3        0x3EAAAAAB
#define NEGFLOATSMALLFRAC           0xbd800000
#define FLOATLARGEFRAC              0x3f700000
#define SINGLEFLOATPOSITIVENAN      0x7fc00000
#define SINGLEFLOATNEGTIVENAN       0xffc00000
#define SINGLEFLOATPOSITIVEINF      0x7f800000
#define SINGLEFLOATNEGTIVEINF       0xff800000
#define SINGLEFLOATPOSITIVEZERO     0x00000000
#define SINGLEFLOATNEGTIVEZERO      0x80000000
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


U32 DumpAQFloat16ToFloat(U16 in);
F_S1E5M10 DumpAQFloatToFloat16(FL32 In);

GLvoid* DumpDecompressETC1(GLsizei Width, GLsizei Height, GLsizei ImageSize, const GLvoid *Data, GLint *Format);
GLint   DumpConvertFormat(GLenum texFormat, GLenum texType);
GLuint  DumpGetShaderDataSize(GLenum type);
GLuint  DumpGetDataSize(GLenum type);
GLuint  DumpGetPixelSize(GLenum texFormat, GLenum texType);
GLuint  DumpGetPrimitiveCountPerMode(GLenum mode, GLuint count);
GLvoid  DumpTranslateFunc(GLenum Func, GLchar *output);
GLvoid  DumpTranslateBlendFunc(GLenum BlendFuncGL, GLchar *output);
GLvoid  DumpTranslateBlendMode(GLenum BlendModeGL, GLchar *output);
GLvoid  DumpTranslateOperation(GLenum Operation, GLchar *output);
GLvoid  DumpWriteFile(FILE *pFile, const GLchar *fmt, ...);
GLvoid  DumpWriteFileWithFlush(FILE *pFile, const GLchar *fmt, ...);
GLvoid  DumpShaderToSingleFile(GLchar *path, GLuint name, GLenum type, GLchar *source);
GLvoid  DumpFlushFile(FILE *pFile);
GLvoid  DumpWriteBytes(FILE *pFile, GLvoid *pData, GLuint size);
GLvoid  DumpWriteInt(FILE *pFile, GLint data);
GLvoid  DumpWriteUInt(FILE *pFile, GLint data);
GLvoid  DumpWriteShort(FILE *pFile, GLshort data);
GLvoid  DumpWriteUShort(FILE *pFile, GLushort data);
GLvoid  DumpWriteFloat(FILE *pFile, GLfloat data);
GLvoid  DumpSavePicture(GLchar *fileName, GLint width, GLint height, GLubyte *pData, GLboolean bPng);

#ifdef WIN32
BOOL    DumpCreateDirectory(LPCTSTR lpPath);
BOOL    DumpFilePathExists(LPCTSTR lpPath);
#endif

#endif /* __API_DUMPUTILS_H */