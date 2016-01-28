#ifndef DEF_H
#define DEF_H

#include "typ.h"

/*
 * GENERAL DEFINITIONS
 *
 * ___WORD           an integer type able to contain a pointer with no loss
 * ___WORD_WIDTH     is equal to the number of bits in the type ___WORD
 * ___LOG_WORD_WIDTH is equal to the base 2 logarithm of ___WORD_WIDTH
 * ___WS             is equal to sizeof (___WORD)
 * ___LWS            is equal to the base 2 logarithm of ___WS
 * ___FLONUM_SIZE    is equal to sizeof (___F64) / ___WS
 */

#ifdef ___USE_32_BIT_ADDR
#define ___SIZE_T ___U32
#define ___SSIZE_T ___S32
#define ___PTRDIFF_T ___S32
#define ___WORD ___S32
#define ___WORD_WIDTH 32
#define ___LOG_WORD_WIDTH 5
#define ___WS 4
#define ___LWS 2
#define ___FLONUM_SIZE 2
#else
#define ___SIZE_T ___U64
#define ___SSIZE_T ___S64
#define ___PTRDIFF_T ___S64
#define ___WORD ___S64
#define ___WORD_WIDTH 64
#define ___LOG_WORD_WIDTH 6
#define ___WS 8
#define ___LWS 3
#define ___FLONUM_SIZE 1
#endif

#define ___SIZE_TS ___SSIZE_T /* temporary hack... in principle ___SIZE_TS should be replaced with ___SIZE_T (unsigned integer) */

#define ___SCMOBJ ___WORD
#define ___SCMOBJ_WIDTH ___WORD_WIDTH

#define ___ISO_8859_1 ___UM8
#define ___UTF_8 char
#define ___UTF_16 ___UM16
#define ___UCS_2 ___UM16
#define ___UCS_4 ___UM32
#define ___ISO_8859_1STRING ___ISO_8859_1*
#define ___UTF_8STRING ___UTF_8*
#define ___UTF_16STRING ___UTF_16*
#define ___UCS_2STRING ___UCS_2*
#define ___UCS_4STRING ___UCS_4*
#define ___WCHARSTRING ___WCHAR*

#if ___MAX_CHR <= 0xff
#define ___CS 1
#else
#if ___MAX_CHR <= 0xffff
#define ___CS 2
#else
#define ___CS 4
#endif
#endif

#if ___CS == 1
#define ___CS_SELECT(cs1,cs2,cs4)cs1
#else
#if ___CS == 2
#define ___CS_SELECT(cs1,cs2,cs4)cs2
#else
#define ___CS_SELECT(cs1,cs2,cs4)cs4
#endif
#endif

#define ___LCS ___CS_SELECT(0,1,2)

#define ___C ___CS_SELECT(___U8,___U16,___U32)

#if ___WORD_WIDTH == ___VOIDSTAR_WIDTH
#define ___FAKEWORD ___WORD*
#define ___CAST_FAKEWORD_TO_WORD(x)___CAST(___WORD,x)
#define ___CAST_WORDSTAR_TO_FAKEWORD(x)x
#define ___FAKEHOST ___host
#define ___CAST_HOST_TO_FAKEHOST(x)x
#define ___CAST_FAKEHOST_TO_HOST(x)x
#define ___FAKEVOIDSTAR void*
#define ___CAST_VOIDSTAR_TO_FAKEVOIDSTAR(x)___CAST(___FAKEVOIDSTAR,x)
#define ___CAST_FAKEVOIDSTAR_TO_VOIDSTAR(x)___CAST(void*,x)
#else
#define ___FAKEWORD ___WORD
#define ___CAST_FAKEWORD_TO_WORD(x)x
#define ___CAST_WORDSTAR_TO_FAKEWORD(x)___CAST(___WORD,x)
#define ___FAKEHOST ___WORD
#define ___CAST_HOST_TO_FAKEHOST(x)___CAST(___WORD,x)
#define ___CAST_FAKEHOST_TO_HOST(x)___CAST(___host,x)
#define ___FAKEVOIDSTAR ___WORD
#define ___CAST_VOIDSTAR_TO_FAKEVOIDSTAR(x)___CAST(___WORD,x)
#define ___CAST_FAKEVOIDSTAR_TO_VOIDSTAR(x)___CAST(void*,x)
#endif

/* Padding at end of objects so that they can be aligned to an address */
/* multiple of 4 or 8 by moving them up in memory.                     */

#ifdef ___USE_32_BIT_ADDR
#define ___PADDING ,0
#define ___PADDING_SIZE 1
#define ___PADDING_LBL ,{0,0,0,0}
#define ___PADDING_64 ,0,0
#else
#define ___PADDING
#define ___PADDING_SIZE 0
#define ___PADDING_LBL
#define ___PADDING_64
#endif

/* Utility macro to select code for SINGLE/MULTIPLE host. */

#ifdef ___SINGLE_HOST
#define ___SM(s,m)s
#else
#define ___SM(s,m)m
#endif

#endif

