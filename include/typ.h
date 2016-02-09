#ifndef TYP_H
#define TYP_H

// require "env.h"

/*
 * The following symbols must be defined to reflect the integer and
 * floating-point data types supported by the C compiler.  A symbol is
 * only defined if the data type exists.  At least ___F64 and one of
 * the pairs ___S32/___U32 and ___S64/___U64 must be defined.
 *
 *  define ___S8 as the signed integer type of exactly 8 bits
 *  define ___U8 as the unsigned integer type of exactly 8 bits
 *  define ___SM8 as the signed integer type of at least 8 bits
 *  define ___UM8 as the unsigned integer type of at least 8 bits
 *
 *  define ___S16 as the signed integer type of exactly 16 bits
 *  define ___U16 as the unsigned integer type of exactly 16 bits
 *  define ___SM16 as the signed integer type of at least 16 bits
 *  define ___UM16 as the unsigned integer type of at least 16 bits
 *
 *  define ___S32 as the signed integer type of exactly 32 bits
 *  define ___U32 as the unsigned integer type of exactly 32 bits
 *  define ___SM32 as the signed integer type of at least 32 bits
 *  define ___UM32 as the unsigned integer type of at least 32 bits
 *
 *  define ___S64 as the signed integer type of exactly 64 bits
 *  define ___U64 as the unsigned integer type of exactly 64 bits
 *  define ___SM64 as the signed integer type of at least 64 bits
 *  define ___UM64 as the unsigned integer type of at least 64 bits
 *
 *  define ___F32 as the 32 bit floating-point type
 *  define ___F64 as the 64 bit floating-point type
 */

/*
 * First we make sure that the width of all the integer and
 * floating-point types is known.
 */

#ifndef ___CHAR_WIDTH
#error "The definition for ___CHAR_WIDTH is missing"
#endif

#ifndef ___SHORT_WIDTH
#error "The definition for ___SHORT_WIDTH is missing"
#endif

#ifndef ___INT_WIDTH
#error "The definition for ___INT_WIDTH is missing"
#endif

#ifndef ___LONG_WIDTH
#error "The definition for ___LONG_WIDTH is missing"
#endif

#ifndef ___LONGLONG_WIDTH
#error "The definition for ___LONGLONG_WIDTH is missing"
#endif

#ifndef ___FLOAT_WIDTH
#error "The definition for ___FLOAT_WIDTH is missing"
#endif

#ifndef ___DOUBLE_WIDTH
#error "The definition for ___DOUBLE_WIDTH is missing"
#endif

#if ___INT_WIDTH == 8
#define ___S8 int
#define ___U8 unsigned int
#else
#if ___LONG_WIDTH == 8
#define ___S8 long
#define ___U8 unsigned long
#else
#if ___SHORT_WIDTH == 8
#define ___S8 short
#define ___U8 unsigned short
#else
#if ___CHAR_WIDTH == 8
#define ___S8 ___SCHAR
#define ___U8 unsigned char
#else
#if ___LONGLONG_WIDTH == 8
#define ___S8 ___LONGLONG
#define ___U8 unsigned ___LONGLONG
#endif
#endif
#endif
#endif
#endif

#if ___INT_WIDTH == 16
#define ___S16 int
#define ___U16 unsigned int
#else
#if ___LONG_WIDTH == 16
#define ___S16 long
#define ___U16 unsigned long
#else
#if ___SHORT_WIDTH == 16
#define ___S16 short
#define ___U16 unsigned short
#else
#if ___CHAR_WIDTH == 16
#define ___S16 ___SCHAR
#define ___U16 unsigned char
#else
#if ___LONGLONG_WIDTH == 16
#define ___S16 ___LONGLONG
#define ___U16 unsigned ___LONGLONG
#endif
#endif
#endif
#endif
#endif

#if ___INT_WIDTH == 32
#define ___S32 int
#define ___U32 unsigned int
#else
#if ___LONG_WIDTH == 32
#define ___S32 long
#define ___U32 unsigned long
#else
#if ___SHORT_WIDTH == 32
#define ___S32 short
#define ___U32 unsigned short
#else
#if ___CHAR_WIDTH == 32
#define ___S32 ___SCHAR
#define ___U32 unsigned char
#else
#if ___LONGLONG_WIDTH == 32
#define ___S32 ___LONGLONG
#define ___U32 unsigned ___LONGLONG
#endif
#endif
#endif
#endif
#endif

#if ___INT_WIDTH == 64
#define ___S64 int
#define ___U64 unsigned int
#else
#if ___LONG_WIDTH == 64
#define ___S64 long
#define ___U64 unsigned long
#else
#if ___SHORT_WIDTH == 64
#define ___S64 short
#define ___U64 unsigned short
#else
#if ___CHAR_WIDTH == 64
#define ___S64 ___SCHAR
#define ___U64 unsigned char
#else
#if ___LONGLONG_WIDTH == 64
#define ___S64 ___LONGLONG
#define ___U64 unsigned ___LONGLONG
#endif
#endif
#endif
#endif
#endif

#if ___FLOAT_WIDTH == 32
#define ___F32 float
#else
#if ___DOUBLE_WIDTH == 32
#define ___F32 double
#endif
#endif

#if ___DOUBLE_WIDTH == 64
#define ___F64 double
#else
#if ___FLOAT_WIDTH == 64
#define ___F64 float
#endif
#endif

#ifdef ___S64
#define ___BUILTIN_64BIT_INT_TYPE
#define ___SM64 ___S64
#define ___UM64 ___U64
#endif

#ifdef ___S32
#define ___SM32 ___S32
#define ___UM32 ___U32
#else
#define ___SM32 ___SM64
#define ___UM32 ___UM64
#endif

#ifdef ___S16
#define ___SM16 ___S16
#define ___UM16 ___U16
#else
#define ___SM16 ___SM32
#define ___UM16 ___UM32
#endif

#ifdef ___S8
#define ___SM8 ___S8
#define ___UM8 ___U8
#else
#define ___SM8 ___SM16
#define ___UM8 ___UM16
#endif


/*---------------------------------------------------------------------------*/

/*
 * IMPORTANT NOTE
 *
 * As a general principle, the macros are written in such a way that all
 * macros which expand into C expressions yield PRIMARY C expressions.
 * Also, macros assume that arguments are PRIMARY expressions.  A PRIMARY
 * expression is either:
 *
 *   an identifier, or
 *   a non-negative constant, or
 *   a parenthesized expression.
 *
 * This convention avoids many operator priority problems and helps keep
 * the number of parentheses down.  On the other hand, it is easy to
 * introduce errors when modifying these macros so be careful.
 */
 
/*---------------------------------------------------------------------------*/

/*
 * The "___CAST" macro is useful to easily locate type casts in the
 * source code.  Unfortunately there are still places in the sources
 * that don't use this macro.
 */

#define ___CAST(type,val)((type)(val))

/*
 * Macros to access ___S64 and ___U64 data types.  Note that ___S64
 * and ___U64 are either defined as a macro or as a typedef.
 */

#define ___FETCH_S64(base,i)*(___CAST(___S64*,base)+(i))
#define ___STORE_S64(base,i,val)*(___CAST(___S64*,base)+(i)) = (val)
#define ___FETCH_U64(base,i)*(___CAST(___U64*,base)+(i))
#define ___STORE_U64(base,i,val)*(___CAST(___U64*,base)+(i)) = (val)
#define ___CAST_S64(x)___CAST(___S64,x)
#define ___CAST_U64(x)___CAST(___U64,x)

/*
 * Macros to access ___S32 and ___U32 data types.
 */

#ifdef ___S32

#define ___FETCH_S32(base,i)*(___CAST(___S32*,base)+(i))
#define ___STORE_S32(base,i,val)*(___CAST(___S32*,base)+(i)) = (val)
#define ___FETCH_U32(base,i)*(___CAST(___U32*,base)+(i))
#define ___STORE_U32(base,i,val)*(___CAST(___U32*,base)+(i)) = (val)
#define ___CAST_S32(x)___CAST(___S32,x)
#define ___CAST_U32(x)___CAST(___U32,x)

#else

#define ___FETCH_S32(base,i)___CAST_S32(___FETCH_U32(base,i))
#define ___STORE_S32(base,i,val)___STORE_U32(base,i,___CAST_U32(val))

#ifdef ___BIG_ENDIAN
#define ___FETCH_U32(base,i)(((i)&1) \
? (*(___CAST(___U64*,base)+((i)>>1))&0xffffffff) \
: ((*(___CAST(___U64*,base)+((i)>>1)))>>32))
#define ___STORE_U32(base,i,val)*(___CAST(___U64*,base)+((i)>>1)) = ((i)&1) \
? (*(___CAST(___U64*,base)+((i)>>1))&___CAST_U64(0xffffffff<<32))|(val) \
: (*(___CAST(___U64*,base)+((i)>>1))&0xffffffff)|___CAST_U64((val)<<32)
#else
#define ___FETCH_U32(base,i)(((i)&1) \
? ((*(___CAST(___U64*,base)+((i)>>1)))>>32) \
: (*(___CAST(___U64*,base)+((i)>>1))&0xffffffff))
#define ___STORE_U32(base,i,val)*(___CAST(___U64*,base)+((i)>>1)) = ((i)&1) \
? (*(___CAST(___U64*,base)+((i)>>1))&0xffffffff)|___CAST_U64((val)<<32) \
: (*(___CAST(___U64*,base)+((i)>>1))&___CAST_U64(0xffffffff<<32))|(val)
#endif

#define ___CAST_S32(x)(___CAST_S64((x)<<32)>>32)
#define ___CAST_U32(x)___CAST_U64((x)&0xffffffff)

#endif

/*
 * Macros to access ___S16 and ___U16 data types.
 */

#ifdef ___S16

#define ___FETCH_S16(base,i)*(___CAST(___S16*,base)+(i))
#define ___STORE_S16(base,i,val)*(___CAST(___S16*,base)+(i)) = (val)
#define ___FETCH_U16(base,i)*(___CAST(___U16*,base)+(i))
#define ___STORE_U16(base,i,val)*(___CAST(___U16*,base)+(i)) = (val)
#define ___CAST_S16(x)___CAST(___S16,x)
#define ___CAST_U16(x)___CAST(___U16,x)

#else

#define ___FETCH_S16(base,i)___CAST_S16(___FETCH_U16(base,i))
#define ___STORE_S16(base,i,val)___STORE_U16(base,i,___CAST_U16(val))

#ifdef ___S32

#ifdef ___BIG_ENDIAN
#define ___FETCH_U16(base,i)(((i)&1) \
? (*(___CAST(___U32*,base)+((i)>>1))&0xffff) \
: ((*(___CAST(___U32*,base)+((i)>>1)))>>16))
#define ___STORE_U16(base,i,val)*(___CAST(___U32*,base)+((i)>>1)) = ((i)&1) \
? (*(___CAST(___U32*,base)+((i)>>1))&___CAST_U32(0xffff<<16))|(val) \
: (*(___CAST(___U32*,base)+((i)>>1))&0xffff)|___CAST_U32((val)<<16)
#else
#define ___FETCH_U16(base,i)(((i)&1) \
? ((*(___CAST(___U32*,base)+((i)>>1)))>>16) \
: (*(___CAST(___U32*,base)+((i)>>1))&0xffff))
#define ___STORE_U16(base,i,val)*(___CAST(___U32*,base)+((i)>>1)) = ((i)&1) \
? (*(___CAST(___U32*,base)+((i)>>1))&0xffff)|___CAST_U32((val)<<16) \
: (*(___CAST(___U32*,base)+((i)>>1))&(0xffff<<16))|(___U32)(val)
#endif

#define ___CAST_S16(x)(___CAST_S32((x)<<16)>>16)
#define ___CAST_U16(x)___CAST_U32((x)&0xffff)

#else

#ifdef ___BIG_ENDIAN
#define ___FETCH_S16(base,i)(((i)&1) \
? ((*(___CAST(___S64*,base)+((i)>>1)))>>16) \
: ((*(___CAST(___S64*,base)+((i)>>1))<<16)>>16))
#define ___FETCH_U16(base,i) \
((*(___CAST(___U64*,base)+((i)>>2))>>((~(i)&3)<<4))&0xffff)
#define ___STORE_U16(base,i,val)*(___CAST(___U64*,base)+((i)>>2)) = \
(*(___CAST(___U64*,base)+((i)>>2))&~___CAST_U64(0xffff<<((~(i)&3)<<4))) | \
___CAST_U64((val)<<((~(i)&3)<<4))
#else
#define ___FETCH_U16(base,i) \
((*(___CAST(___U64*,base)+((i)>>2))>>(((i)&3)<<4))&0xffff)
#define ___STORE_U16(base,i,val)*(___CAST(___U64*,base)+((i)>>2)) = \
(*(___CAST(___U64*,base)+((i)>>2))&~___CAST_U64(0xffff<<(((i)&3)<<4))) | \
___CAST_U64((val)<<(((i)&3)<<4))
#endif

#define ___CAST_S16(x)(___CAST_S64((x)<<48)>>48)
#define ___CAST_U16(x)___CAST_U64((x)&0xffff)

#endif

#endif

/*
 * Macros to access ___S8 and ___U8 data types.
 */

#ifdef ___S8

#define ___FETCH_S8(base,i)*(___CAST(___S8*,base)+(i))
#define ___FETCH_U8(base,i)*(___CAST(___U8*,base)+(i))
#define ___STORE_S8(base,i,val)*(___CAST(___S8*,base)+(i)) = (val)
#define ___STORE_U8(base,i,val)*(___CAST(___U8*,base)+(i)) = (val)
#define ___CAST_S8(x)(___S8)(x)
#define ___CAST_U8(x)(___U8)(x)

#else

#define ___FETCH_S8(base,i)___CAST_S8(___FETCH_U8(base,i))
#define ___STORE_S8(base,i,val)___STORE_U8(base,i,___CAST_U8(val))

#ifdef ___S32

#ifdef ___BIG_ENDIAN
#define ___FETCH_U8(base,i) \
((*(___CAST(___U32*,base)+((i)>>2))>>((~(i)&3)<<3))&0xff)
#define ___STORE_U8(base,i,val)*(___CAST(___U32*,base)+((i)>>2)) = \
(*(___CAST(___U32*,base)+((i)>>2))&~___CAST_U32(0xff<<((~(i)&3)<<3))) | \
___CAST_U32((val)<<((~(i)&3)<<3))
#else
#define ___FETCH_U8(base,i) \
((*(___CAST(___U32*,base)+((i)>>2))>>(((i)&3)<<3))&0xff)
#define ___STORE_U8(base,i,val)*(___CAST(___U32*,base)+((i)>>2)) = \
(*(___CAST(___U32*,base)+((i)>>2))&~___CAST_U32(0xff<<(((i)&3)<<3))) | \
___CAST_U32((val)<<(((i)&3)<<3))
#endif

#define ___CAST_S8(x)(___CAST_S32((x)<<24)>>24)
#define ___CAST_U8(x)___CAST_U32((x)&0xff)

#else

#ifdef ___BIG_ENDIAN
#define ___FETCH_U8(base,i) \
((*(___CAST(___U64*,base)+((i)>>3))>>((~(i)&7)<<3))&0xff)
#define ___STORE_U8(base,i,val)*(___CAST(___U64*,base)+((i)>>3)) = \
(*(___CAST(___U64*,base)+((i)>>3))&~___CAST_U64(0xff<<((~(i)&7)<<3))) | \
___CAST_U64((val)<<((~(i)&7)<<3))
#else
#define ___FETCH_U8(base,i) \
((*(___CAST(___U64*,base)+((i)>>3))>>(((i)&7)<<3))&0xff)
#define ___STORE_U8(base,i,val)*(___CAST(___U64*,base)+((i)>>3)) = \
(*(___CAST(___U64*,base)+((i)>>3))&~___CAST_U64(0xff<<(((i)&7)<<3))) | \
___CAST_U64((val)<<(((i)&7)<<3))
#endif

#define ___CAST_S8(x)(___CAST_S64((x)<<56)>>56)
#define ___CAST_U8(x)___CAST_U64((x)&0xff)

#endif

#endif

/*
 * Macros to access ___F32 and ___F64 data types.
 */

#ifdef ___F32
#define ___FETCH_F32(base,i)*(___CAST(___F32*,base)+(i))
#define ___STORE_F32(base,i,val)*(___CAST(___F32*,base)+(i)) = (val)
#define ___CAST_F32(x)___CAST(___F32,x)
#endif

#ifdef ___F64
#define ___FETCH_F64(base,i)*(___CAST(___F64*,base)+(i))
#define ___STORE_F64(base,i,val)*(___CAST(___F64*,base)+(i)) = (val)
#define ___CAST_F64(x)___CAST(___F64,x)
#endif


#endif

