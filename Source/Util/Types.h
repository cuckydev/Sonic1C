#ifndef INCLUDE_TYPES_H
#define INCLUDE_TYPES_H

#include <limits.h>

typedef unsigned char u8;
typedef signed char s8;

#if SHRT_MAX == 0x7FFF
typedef unsigned short u16;
typedef signed short s16;
#elif INT_MAX == 0x7FFF
typedef unsigned int u16;
typedef signed int s16;
#else
#error "Cannot determine 16-bit unsigned integer type"
#endif

#if INT_MAX == 0x7FFFFFFF
typedef unsigned int u32;
typedef signed int s32;
#elif LONG_MAX == 0x7FFFFFFF
typedef unsigned long u32;
typedef signed long s32;
#else
#error "Cannot determine 32-bit unsigned integer type"
#endif

#endif
