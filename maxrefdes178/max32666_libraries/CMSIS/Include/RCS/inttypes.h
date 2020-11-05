// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __inttypes_H
#define __inttypes_H

#if defined(__CROSSWORKS_ARM) || defined(__CROSSWORKS_MAXQ30) || defined(__SES_ARM)
  // 32-bit int
  #define __RAL_PRI_32_PREFIX  ""
  #define __RAL_SCN_16_PREFIX  "h"      // 16-bit types are same size as short
  #define __RAL_SCN_32_PREFIX  ""       // 32-bit types are same size as int
  #define __RAL_SCN_LEAST16_PREFIX "h"  // least16 are same size as short
#else
  // 16-bit int
  #define __RAL_PRI_32_PREFIX  "l"
  #define __RAL_SCN_16_PREFIX  ""       // 16-bit types are same size as int
  #define __RAL_SCN_32_PREFIX  "l"      // 32-bit types are same size as long
  #define __RAL_SCN_LEAST16_PREFIX ""   // least16 types are same size as int
#endif

// =============
// Print formats
// =============

// %d
#define PRId8        "d"
#define PRId16       "d"
#define PRId32       __RAL_PRI_32_PREFIX "d"
#define PRId64       "lld"

#define PRIdFAST8    "d"
#define PRIdFAST16   "d"
#define PRIdFAST32   __RAL_PRI_32_PREFIX "d"
#define PRIdFAST64   "lld"

#define PRIdLEAST8   "d"
#define PRIdLEAST16  "d"
#define PRIdLEAST32  __RAL_PRI_32_PREFIX "d"
#define PRIdLEAST64  "lld"

#define PRIdMAX      "lld"

// %i
#define PRIi8        "i"
#define PRIi16       "i"
#define PRIi32       __RAL_PRI_32_PREFIX "i"
#define PRIi64       "lli"

#define PRIiFAST8    "i"
#define PRIiFAST16   "i"
#define PRIiFAST32   __RAL_PRI_32_PREFIX "i"
#define PRIiFAST64   "lli"

#define PRIiLEAST8   "i"
#define PRIiLEAST16  "i"
#define PRIiLEAST32  __RAL_PRI_32_PREFIX "i"
#define PRIiLEAST64  "lli"

#define PRIiMAX      "lli"

// %o
#define PRIo8        "o"
#define PRIo16       "o"
#define PRIo32       __RAL_PRI_32_PREFIX "o"
#define PRIo64       "llo"

#define PRIoFAST8    "o"
#define PRIoFAST16   "o"
#define PRIoFAST32   __RAL_PRI_32_PREFIX "o"
#define PRIoFAST64   "llo"

#define PRIoLEAST8   "o"
#define PRIoLEAST16  "o"
#define PRIoLEAST32  __RAL_PRI_32_PREFIX "o"
#define PRIoLEAST64  "llo"

#define PRIoMAX      "llo"

// %u
#define PRIu8        "u"
#define PRIu16       "u"
#define PRIu32       __RAL_PRI_32_PREFIX "u"
#define PRIu64       "llu"

#define PRIuFAST8    "u"
#define PRIuFAST16   "u"
#define PRIuFAST32   __RAL_PRI_32_PREFIX "u"
#define PRIuFAST64   "llu"

#define PRIuLEAST8   "u"
#define PRIuLEAST16  "u"
#define PRIuLEAST32  __RAL_PRI_32_PREFIX "u"
#define PRIuLEAST64  "llu"

#define PRIuMAX      "llu"

// %x
#define PRIx8        "x"
#define PRIx16       "x"
#define PRIx32       __RAL_PRI_32_PREFIX "x"
#define PRIx64       "llx"

#define PRIxFAST8    "x"
#define PRIxFAST16   "x"
#define PRIxFAST32   __RAL_PRI_32_PREFIX "x"
#define PRIxFAST64   "llx"

#define PRIxLEAST8   "x"
#define PRIxLEAST16  "x"
#define PRIxLEAST32  __RAL_PRI_32_PREFIX "x"
#define PRIxLEAST64  "llx"

#define PRIxMAX      "llx"

// %X
#define PRIX8        "X"
#define PRIX16       "X"
#define PRIX32       __RAL_PRI_32_PREFIX "X"
#define PRIX64       "llX"

#define PRIXFAST8    "X"
#define PRIXFAST16   "X"
#define PRIXFAST32   __RAL_PRI_32_PREFIX "X"
#define PRIXFAST64   "llX"

#define PRIXLEAST8   "X"
#define PRIXLEAST16  "X"
#define PRIXLEAST32  __RAL_PRI_32_PREFIX "X"
#define PRIXLEAST64  "llX"

#define PRIXMAX      "llX"

// ============
// Scan formats
// ============


// %d
#define SCNd8        "d"
#define SCNd16       __RAL_SCN_16_PREFIX "d"
#define SCNd32       __RAL_SCN_32_PREFIX "d"
#define SCNd64       "lld"

#define SCNdFAST8    "hhd"
#define SCNdFAST16   __RAL_SCN_16_PREFIX "d"
#define SCNdFAST32   __RAL_SCN_32_PREFIX "d"
#define SCNdFAST64   "lld"

#define SCNdLEAST8   "hhd"
#define SCNdLEAST16  __RAL_SCN_LEAST16_PREFIX "d"
#define SCNdLEAST32  __RAL_SCN_32_PREFIX "d"
#define SCNdLEAST64  "lld"

#define SCNdMAX      "lld"

// %i
#define SCNi8        "i"
#define SCNi16       __RAL_SCN_16_PREFIX "i"
#define SCNi32       __RAL_SCN_32_PREFIX "i"
#define SCNi64       "lli"

#define SCNiFAST8    "hhi"
#define SCNiFAST16   __RAL_SCN_16_PREFIX "i"
#define SCNiFAST32   __RAL_SCN_32_PREFIX "i"
#define SCNiFAST64   "lli"

#define SCNiLEAST8   "hhi"
#define SCNiLEAST16  __RAL_SCN_LEAST16_PREFIX "i"
#define SCNiLEAST32  __RAL_SCN_32_PREFIX "i"
#define SCNiLEAST64  "lli"

#define SCNiMAX      "lli"

// %o
#define SCNo8        "o"
#define SCNo16       __RAL_SCN_16_PREFIX "o"
#define SCNo32       __RAL_SCN_32_PREFIX "o"
#define SCNo64       "llo"

#define SCNoFAST8    "hho"
#define SCNoFAST16   __RAL_SCN_16_PREFIX "o"
#define SCNoFAST32   __RAL_SCN_32_PREFIX "o"
#define SCNoFAST64   "llo"

#define SCNoLEAST8   "hho"
#define SCNoLEAST16  __RAL_SCN_LEAST16_PREFIX "o"
#define SCNoLEAST32  __RAL_SCN_32_PREFIX "o"
#define SCNoLEAST64  "llo"

#define SCNoMAX      "llo"

// %u
#define SCNu8        "u"
#define SCNu16       __RAL_SCN_16_PREFIX "u"
#define SCNu32       __RAL_SCN_32_PREFIX "u"
#define SCNu64       "llu"

#define SCNuFAST8    "hhu"
#define SCNuFAST16   __RAL_SCN_16_PREFIX "u"
#define SCNuFAST32   __RAL_SCN_32_PREFIX "u"
#define SCNuFAST64   "llu"

#define SCNuLEAST8   "hhu"
#define SCNuLEAST16  __RAL_SCN_LEAST16_PREFIX "u"
#define SCNuLEAST32  __RAL_SCN_32_PREFIX "u"
#define SCNuLEAST64  "llu"

#define SCNuMAX      "llu"

// %x
#define SCNx8        "x"
#define SCNx16       __RAL_SCN_16_PREFIX "x"
#define SCNx32       __RAL_SCN_32_PREFIX "x"
#define SCNx64       "llx"

#define SCNxFAST8    "hhx"
#define SCNxFAST16   __RAL_SCN_16_PREFIX "x"
#define SCNxFAST32   __RAL_SCN_32_PREFIX "x"
#define SCNxFAST64   "llx"

#define SCNxLEAST8   "hhx"
#define SCNxLEAST16  __RAL_SCN_LEAST16_PREFIX "x"
#define SCNxLEAST32  __RAL_SCN_32_PREFIX "x"
#define SCNxLEAST64  "llx"

#define SCNxMAX      "llx"

#endif
