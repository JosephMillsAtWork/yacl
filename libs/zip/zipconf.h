#ifndef _HAD_ZIPCONF_H
#define _HAD_ZIPCONF_H

//#define ZIP_STATIC

#include <stdint.h>

//typedef int8_t zip_int8_t;
//typedef uint8_t zip_uint8_t;
//typedef int16_t zip_int16_t;
//typedef uint16_t zip_uint16_t;
//typedef int32_t zip_int32_t;
//typedef uint32_t zip_uint32_t;
//typedef int64_t zip_int64_t;
//typedef uint64_t zip_uint64_t;

//#define ZIP_INT8_MIN	 (-ZIP_INT8_MAX-1)
//#define ZIP_INT8_MAX	 0x7f
//#define ZIP_UINT8_MAX	 0xff

//#define ZIP_INT16_MIN	 (-ZIP_INT16_MAX-1)
//#define ZIP_INT16_MAX	 0x7fff
//#define ZIP_UINT16_MAX	 0xffff

//#define ZIP_INT32_MIN	 (-ZIP_INT32_MAX-1L)
//#define ZIP_INT32_MAX	 0x7fffffffL
//#define ZIP_UINT32_MAX	 0xffffffffLU

//#define ZIP_INT64_MIN	 (-ZIP_INT64_MAX-1LL)
//#define ZIP_INT64_MAX	 0x7fffffffffffffffLL
//#define ZIP_UINT64_MAX	 0xffffffffffffffffULL



#include <inttypes.h>

typedef int8_t zip_int8_t;
#define ZIP_INT8_MIN INT8_MIN
#define ZIP_INT8_MAX INT8_MAX

typedef uint8_t zip_uint8_t;
#define ZIP_UINT8_MAX UINT8_MAX

typedef int16_t zip_int16_t;
#define ZIP_INT16_MIN INT16_MIN
#define ZIP_INT16_MAX INT16_MAX

typedef uint16_t zip_uint16_t;
#define ZIP_UINT16_MAX UINT16_MAX

typedef int32_t zip_int32_t;
#define ZIP_INT32_MIN INT32_MIN
#define ZIP_INT32_MAX INT32_MAX

typedef uint32_t zip_uint32_t;
#define ZIP_UINT32_MAX UINT32_MAX

typedef int64_t zip_int64_t;
#define ZIP_INT64_MIN INT64_MIN
#define ZIP_INT64_MAX INT64_MAX

typedef uint64_t zip_uint64_t;
#define ZIP_UINT64_MAX UINT64_MAX

#endif /* zipconf.h */
