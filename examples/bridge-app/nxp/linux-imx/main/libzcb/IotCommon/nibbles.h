// ------------------------------------------------------------------
// Nibble stuff - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------
#if defined __cplusplus
extern "C" {
#endif

#include <stdint.h>

int nibble2num( char c );
char num2nibble( int num );
int nibblestr2hex( char * dst, int dstlen, char * src, int srclen );
void hex2nibblestr( char * dst, char * src, int srclen );
uint64_t hex2u64( char * hex );
uint64_t nibblestr2u64( char * nibblestr );
void u642nibblestr( uint64_t u64, char * nibblestr );

#if defined __cplusplus
}
#endif