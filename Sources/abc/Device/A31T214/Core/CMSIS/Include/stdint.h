/* Copyright (C) ARM Ltd., 1999 */
/* All rights reserved */

/*
 * RCS $Revision: 172039 $
 * Checkin $Date: 2011-11-02 12:58:12 +0000 (Wed, 02 Nov 2011) $
 * Revising $Author: agrant $
 */
/* stdint.h standard header */
/* Copyright 2003-2010 IAR Systems AB.  */
#ifndef _STDINT
#define _STDINT

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
_C_STD_BEGIN

/* Fixed size types. These are all optional. */
#ifdef __INT8_T_TYPE__
  typedef __INT8_T_TYPE__   int8_t;
  typedef __UINT8_T_TYPE__ uint8_t;
#endif /* __INT8_T_TYPE__ */

#ifdef __INT16_T_TYPE__
  typedef __INT16_T_TYPE__   int16_t;
  typedef __UINT16_T_TYPE__ uint16_t;
#endif /* __INT16_T_TYPE__ */

#ifdef __INT32_T_TYPE__
  typedef __INT32_T_TYPE__   int32_t;
  typedef __UINT32_T_TYPE__ uint32_t;
#endif /* __INT32_T_TYPE__ */

#ifdef __INT64_T_TYPE__
  #pragma language=save
  #pragma language=extended
  typedef __INT64_T_TYPE__   int64_t;
  typedef __UINT64_T_TYPE__ uint64_t;
  #pragma language=restore
#endif /* __INT64_T_TYPE__ */

/* Types capable of holding at least a certain number of bits.
   These are not optional for the sizes 8, 16, 32, 64. */
typedef __INT_LEAST8_T_TYPE__   int_least8_t;
typedef __UINT_LEAST8_T_TYPE__ uint_least8_t;

typedef __INT_LEAST16_T_TYPE__   int_least16_t;
typedef __UINT_LEAST16_T_TYPE__ uint_least16_t;

typedef __INT_LEAST32_T_TYPE__   int_least32_t;
typedef __UINT_LEAST32_T_TYPE__ uint_least32_t;

/* This isn't really optional, but make it so for now. */
#ifdef __INT_LEAST64_T_TYPE__
  #pragma language=save
  #pragma language=extended
  typedef __INT_LEAST64_T_TYPE__ int_least64_t;
  #pragma language=restore
#endif /* __INT_LEAST64_T_TYPE__ */
#ifdef __UINT_LEAST64_T_TYPE__
  #pragma language=save
  #pragma language=extended
  typedef __UINT_LEAST64_T_TYPE__ uint_least64_t;
  #pragma language=restore
#endif /* __UINT_LEAST64_T_TYPE__ */

/* The fastest type holding at least a certain number of bits.
   These are not optional for the size 8, 16, 32, 64.
   For now, the 64 bit size is optional in IAR compilers. */
typedef __INT_FAST8_T_TYPE__   int_fast8_t;
typedef __UINT_FAST8_T_TYPE__ uint_fast8_t;

typedef __INT_FAST16_T_TYPE__   int_fast16_t;
typedef __UINT_FAST16_T_TYPE__ uint_fast16_t;

typedef __INT_FAST32_T_TYPE__   int_fast32_t;
typedef __UINT_FAST32_T_TYPE__ uint_fast32_t;

#ifdef __INT_FAST64_T_TYPE__
  #pragma language=save
  #pragma language=extended
  typedef __INT_FAST64_T_TYPE__ int_fast64_t;
  #pragma language=restore
#endif /* __INT_FAST64_T_TYPE__ */
#ifdef __UINT_FAST64_T_TYPE__
  #pragma language=save
  #pragma language=extended
  typedef __UINT_FAST64_T_TYPE__ uint_fast64_t;
  #pragma language=restore
#endif /* __UINT_FAST64_T_TYPE__ */

/* The integer type capable of holding the largest number of bits. */
#pragma language=save
#pragma language=extended
typedef __INTMAX_T_TYPE__   intmax_t;
typedef __UINTMAX_T_TYPE__ uintmax_t;
#pragma language=restore

/* An integer type large enough to be able to hold a pointer.
   This is optional, but always supported in IAR compilers. */
typedef __INTPTR_T_TYPE__   intptr_t;
typedef __UINTPTR_T_TYPE__ uintptr_t;

/* An integer capable of holding a pointer to a specific memory type. */
#define __DATA_PTR_MEM_HELPER1__(M, I)                     \
typedef __DATA_MEM##I##_INTPTR_TYPE__ M##_intptr_t;        \
typedef __DATA_MEM##I##_UINTPTR_TYPE__ M##_uintptr_t;
__DATA_PTR_MEMORY_LIST1__()
#undef __DATA_PTR_MEM_HELPER1__

/* Minimum and maximum limits. */
#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

#ifdef __INT8_T_TYPE__
  #define INT8_MAX   __INT8_T_MAX__
  #define INT8_MIN   __INT8_T_MIN__
  #define UINT8_MAX __UINT8_T_MAX__
#endif

#ifdef __INT16_T_TYPE__
  #define INT16_MAX   __INT16_T_MAX__
  #define INT16_MIN   __INT16_T_MIN__
  #define UINT16_MAX __UINT16_T_MAX__
#endif

#ifdef __INT32_T_TYPE__
  #define INT32_MAX   __INT32_T_MAX__
  #define INT32_MIN   __INT32_T_MIN__
  #define UINT32_MAX __UINT32_T_MAX__
#endif

#ifdef __INT64_T_TYPE__
  #define INT64_MAX   __INT64_T_MAX__
  #define INT64_MIN   __INT64_T_MIN__
  #define UINT64_MAX __UINT64_T_MAX__
#endif

#define INT_LEAST8_MAX   __INT_LEAST8_T_MAX__
#define INT_LEAST8_MIN   __INT_LEAST8_T_MIN__
#define UINT_LEAST8_MAX __UINT_LEAST8_T_MAX__

#define INT_LEAST16_MAX   __INT_LEAST16_T_MAX__
#define INT_LEAST16_MIN   __INT_LEAST16_T_MIN__
#define UINT_LEAST16_MAX __UINT_LEAST16_T_MAX__

#define INT_LEAST32_MAX   __INT_LEAST32_T_MAX__
#define INT_LEAST32_MIN   __INT_LEAST32_T_MIN__
#define UINT_LEAST32_MAX __UINT_LEAST32_T_MAX__

#ifdef __INT_LEAST64_T_TYPE__
  #define INT_LEAST64_MAX __INT_LEAST64_T_MAX__
  #define INT_LEAST64_MIN __INT_LEAST64_T_MIN__
#endif

#ifdef __UINT_LEAST64_T_TYPE__
  #define UINT_LEAST64_MAX __UINT_LEAST64_T_MAX__
#endif

#define INT_FAST8_MAX   __INT_FAST8_T_MAX__
#define INT_FAST8_MIN   __INT_FAST8_T_MIN__
#define UINT_FAST8_MAX __UINT_FAST8_T_MAX__

#define INT_FAST16_MAX   __INT_FAST16_T_MAX__
#define INT_FAST16_MIN   __INT_FAST16_T_MIN__
#define UINT_FAST16_MAX __UINT_FAST16_T_MAX__

#define INT_FAST32_MAX   __INT_FAST32_T_MAX__
#define INT_FAST32_MIN   __INT_FAST32_T_MIN__
#define UINT_FAST32_MAX __UINT_FAST32_T_MAX__

#ifdef __INT_FAST64_T_TYPE__
  #define INT_FAST64_MAX __INT_FAST64_T_MAX__
  #define INT_FAST64_MIN __INT_FAST64_T_MIN__
#endif

#ifdef __UINT_FAST64_T_TYPE__
  #define UINT_FAST64_MAX __UINT_FAST64_T_MAX__
#endif

#define INTMAX_MAX   __INTMAX_T_MAX__
#define INTMAX_MIN   __INTMAX_T_MIN__
#define UINTMAX_MAX __UINTMAX_T_MAX__

#define SIZE_MAX __SIZE_T_MAX__

#define PTRDIFF_MAX __PTRDIFF_T_MAX__
#define PTRDIFF_MIN __PTRDIFF_T_MIN__

#define INTPTR_MAX   __INTPTR_T_MAX__
#define INTPTR_MIN   __INTPTR_T_MIN__
#define UINTPTR_MAX __UINTPTR_T_MAX__

#define WCHAR_MIN _WCMIN
#define WCHAR_MAX _WCMAX

#define WINT_MIN _WIMIN
#define WINT_MAX _WIMAX

#define SIG_ATOMIC_MIN __SIGNED_CHAR_MIN__
#define SIG_ATOMIC_MAX __SIGNED_CHAR_MAX__


#endif /* !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */


/* Macros expanding to integer constants. */
#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#ifdef __INT8_T_TYPE__
  #define INT8_C(x)  _GLUE(x,__INT8_C_SUFFIX__)
  #define UINT8_C(x) _GLUE(x,__UINT8_C_SUFFIX__)
#endif

#ifdef __INT16_T_TYPE__
  #define INT16_C(x)  _GLUE(x,__INT16_C_SUFFIX__)
  #define UINT16_C(x) _GLUE(x,__UINT16_C_SUFFIX__)
#endif

#ifdef __INT32_T_TYPE__
  #define INT32_C(x)  _GLUE(x,__INT32_C_SUFFIX__)
  #define UINT32_C(x) _GLUE(x,__UINT32_C_SUFFIX__)
#endif

#ifdef __INT_LEAST64_T_TYPE__
  #define INT64_C(x) _GLUE(x,__INT64_C_SUFFIX__)
#endif

#ifdef __UINT_LEAST64_T_TYPE__
  #define UINT64_C(x) _GLUE(x,__UINT64_C_SUFFIX__)
#endif

#define INTMAX_C(x)  _GLUE(x,__INTMAX_C_SUFFIX__)
#define UINTMAX_C(x) _GLUE(x,__UINTMAX_C_SUFFIX__)

#endif /* !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS) */

_C_STD_END
#endif /* _STDINT */

#if defined(_STD_USING)
  using _CSTD int8_t; using _CSTD int16_t;
  using _CSTD int32_t; using _CSTD int64_t;

  using _CSTD uint8_t; using _CSTD uint16_t;
  using _CSTD uint32_t; using _CSTD uint64_t;

  using _CSTD int_least8_t; using _CSTD int_least16_t;
  using _CSTD int_least32_t;  using _CSTD int_least64_t;
  using _CSTD uint_least8_t; using _CSTD uint_least16_t;
  using _CSTD uint_least32_t; using _CSTD uint_least64_t;

  using _CSTD intmax_t; using _CSTD uintmax_t;

  using _CSTD uintptr_t;
  using _CSTD intptr_t;

  using _CSTD int_fast8_t; using _CSTD int_fast16_t;
  using _CSTD int_fast32_t; using _CSTD int_fast64_t;
  using _CSTD uint_fast8_t; using _CSTD uint_fast16_t;
  using _CSTD uint_fast32_t; using _CSTD uint_fast64_t;
#endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */

#if 0 
#ifndef __stdint_h
#define __stdint_h
#define __ARMCLIB_VERSION 410000

  #ifndef __STDINT_DECLS
  #define __STDINT_DECLS

    #undef __CLIBNS

    #ifdef __cplusplus
      namespace std {
          #define __CLIBNS std::
          extern "C" {
    #else
      #define __CLIBNS
    #endif  /* __cplusplus */


/*
 * 'signed' is redundant below, except for 'signed char' and if
 * the typedef is used to declare a bitfield.
 * '__int64' is used instead of 'long long' so that this header
 * can be used in --strict mode.
 */

    /* 7.18.1.1 */

    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

    /* 7.18.1.2 */

    /* smallest type of at least n bits */
    /* minimum-width signed integer types */
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

    /* minimum-width unsigned integer types */
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

    /* 7.18.1.3 */

    /* fastest minimum-width signed integer types */
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

    /* fastest minimum-width unsigned integer types */
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

    /* 7.18.1.4 integer types capable of holding object pointers */
typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;

    /* 7.18.1.5 greatest-width integer types */
typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;


#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

    /* 7.18.2.1 */

    /* minimum values of exact-width signed integer types */
#define INT8_MIN                   -128
#define INT16_MIN                -32768
#define INT32_MIN          (~0x7fffffff)   /* -2147483648 is unsigned */
#define INT64_MIN  __ESCAPE__(~0x7fffffffffffffffll) /* -9223372036854775808 is unsigned */

    /* maximum values of exact-width signed integer types */
#define INT8_MAX                    127
#define INT16_MAX                 32767
#define INT32_MAX            2147483647
#define INT64_MAX  __ESCAPE__(9223372036854775807ll)

    /* maximum values of exact-width unsigned integer types */
#define UINT8_MAX                   255
#define UINT16_MAX                65535
#define UINT32_MAX           4294967295u
#define UINT64_MAX __ESCAPE__(18446744073709551615ull)

    /* 7.18.2.2 */

    /* minimum values of minimum-width signed integer types */
#define INT_LEAST8_MIN                   -128
#define INT_LEAST16_MIN                -32768
#define INT_LEAST32_MIN          (~0x7fffffff)
#define INT_LEAST64_MIN  __ESCAPE__(~0x7fffffffffffffffll)

    /* maximum values of minimum-width signed integer types */
#define INT_LEAST8_MAX                    127
#define INT_LEAST16_MAX                 32767
#define INT_LEAST32_MAX            2147483647
#define INT_LEAST64_MAX  __ESCAPE__(9223372036854775807ll)

    /* maximum values of minimum-width unsigned integer types */
#define UINT_LEAST8_MAX                   255
#define UINT_LEAST16_MAX                65535
#define UINT_LEAST32_MAX           4294967295u
#define UINT_LEAST64_MAX __ESCAPE__(18446744073709551615ull)

    /* 7.18.2.3 */

    /* minimum values of fastest minimum-width signed integer types */
#define INT_FAST8_MIN           (~0x7fffffff)
#define INT_FAST16_MIN          (~0x7fffffff)
#define INT_FAST32_MIN          (~0x7fffffff)
#define INT_FAST64_MIN  __ESCAPE__(~0x7fffffffffffffffll)

    /* maximum values of fastest minimum-width signed integer types */
#define INT_FAST8_MAX             2147483647
#define INT_FAST16_MAX            2147483647
#define INT_FAST32_MAX            2147483647
#define INT_FAST64_MAX  __ESCAPE__(9223372036854775807ll)

    /* maximum values of fastest minimum-width unsigned integer types */
#define UINT_FAST8_MAX            4294967295u
#define UINT_FAST16_MAX           4294967295u
#define UINT_FAST32_MAX           4294967295u
#define UINT_FAST64_MAX __ESCAPE__(18446744073709551615ull)

    /* 7.18.2.4 */

    /* minimum value of pointer-holding signed integer type */
#define INTPTR_MIN (~0x7fffffff)

    /* maximum value of pointer-holding signed integer type */
#define INTPTR_MAX   2147483647

    /* maximum value of pointer-holding unsigned integer type */
#define UINTPTR_MAX  4294967295u

    /* 7.18.2.5 */

    /* minimum value of greatest-width signed integer type */
#define INTMAX_MIN  __ESCAPE__(~0x7fffffffffffffffll)

    /* maximum value of greatest-width signed integer type */
#define INTMAX_MAX  __ESCAPE__(9223372036854775807ll)

    /* maximum value of greatest-width unsigned integer type */
#define UINTMAX_MAX __ESCAPE__(18446744073709551615ull)

    /* 7.18.3 */

    /* limits of ptrdiff_t */
#define PTRDIFF_MIN (~0x7fffffff)
#define PTRDIFF_MAX   2147483647

    /* limits of sig_atomic_t */
#define SIG_ATOMIC_MIN (~0x7fffffff)
#define SIG_ATOMIC_MAX   2147483647

    /* limit of size_t */
#define SIZE_MAX 4294967295u

    /* limits of wchar_t */
    /* NB we have to undef and redef because they're defined in both
     * stdint.h and wchar.h */
#undef WCHAR_MIN
#undef WCHAR_MAX

#if defined(__WCHAR32)
  #define WCHAR_MIN   0
  #define WCHAR_MAX   0xffffffffU
#else
  #define WCHAR_MIN   0
  #define WCHAR_MAX   65535
#endif

    /* limits of wint_t */
#define WINT_MIN (~0x7fffffff)
#define WINT_MAX 2147483647

#endif /* __STDC_LIMIT_MACROS */

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

    /* 7.18.4.1 macros for minimum-width integer constants */
#define INT8_C(x)   (x)
#define INT16_C(x)  (x)
#define INT32_C(x)  (x)
#define INT64_C(x)  __ESCAPE__(x ## ll)

#define UINT8_C(x)  (x ## u)
#define UINT16_C(x) (x ## u)
#define UINT32_C(x) (x ## u)
#define UINT64_C(x) __ESCAPE__(x ## ull)

    /* 7.18.4.2 macros for greatest-width integer constants */
#define INTMAX_C(x)  __ESCAPE__(x ## ll)
#define UINTMAX_C(x) __ESCAPE__(x ## ull)

#endif /* __STDC_CONSTANT_MACROS */

    #ifdef __cplusplus
         }  /* extern "C" */
      }  /* namespace std */
    #endif /* __cplusplus */
  #endif /* __STDINT_DECLS */

  #ifdef __cplusplus
    #ifndef __STDINT_NO_EXPORTS
      using ::std::int8_t;
      using ::std::int16_t;
      using ::std::int32_t;
      using ::std::int64_t;
      using ::std::uint8_t;
      using ::std::uint16_t;
      using ::std::uint32_t;
      using ::std::uint64_t;
      using ::std::int_least8_t;
      using ::std::int_least16_t;
      using ::std::int_least32_t;
      using ::std::int_least64_t;
      using ::std::uint_least8_t;
      using ::std::uint_least16_t;
      using ::std::uint_least32_t;
      using ::std::uint_least64_t;
      using ::std::int_fast8_t;
      using ::std::int_fast16_t;
      using ::std::int_fast32_t;
      using ::std::int_fast64_t;
      using ::std::uint_fast8_t;
      using ::std::uint_fast16_t;
      using ::std::uint_fast32_t;
      using ::std::uint_fast64_t;
      using ::std::intptr_t;
      using ::std::uintptr_t;
      using ::std::intmax_t;
      using ::std::uintmax_t;
    #endif 
  #endif /* __cplusplus */

#endif /* __stdint_h */

/* end of stdint.h */

#endif
      

