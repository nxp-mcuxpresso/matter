// ------------------------------------------------------------------
// Nibble stuff
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief Nibble string conversions
 */

#include <stdio.h>

#include "nibbles.h"

// ------------------------------------------------------------------
// NIBBLE / HEX conversions
// ------------------------------------------------------------------

/**
 * \brief Convert a nibble character into a number
 * \param c Nibble character between (and including) [0-9], [a-f] or [A-F]
 * \returns Conversion or 0 in case of a non-hex character
 */
int nibble2num( char c ) {
    // Warning: input must be a hex-nibble character, else 0 is returned
    int val = 0;
    if      ( c >= '0' && c <= '9' ) val = c - '0';
    else if ( c >= 'A' && c <= 'F' ) val = c - 'A' + 10;
    else if ( c >= 'a' && c <= 'f' ) val = c - 'a' + 10;
    return( val );
}

/**
 * \brief Convert a number into a nibble character
 * \param num Number between (and including) [0-15]
 * \returns Conversion character or '0' when num is out of range
 */
char num2nibble( int num ) {
    // Warning: input must be 0<=num<16), else '0' is returned
    if ( num < 0 || num > 16 ) return( '0' );
    if ( num < 10 ) return( num + '0' );
    return( num - 10 + 'A' );
}

/**
 * \brief Converts a nibble string into a hex-string with leading zeros
 * \param src Nibble string
 * \param srclen Length of the nibble string
 * \param dst User provided space to store the hex string
 * \param dstlen Length of the hex string storage (typically half the size of the nibble string)
 * \returns 1 when conversion was succesfull, or 0 in case of an error
 */
int nibblestr2hex( char * dst, int dstlen, char * src, int srclen ) {
    // Warning: src must be a nibble-hex string, else result is unpredictable

    // Check pointers
    if ( dst == NULL || src == NULL ) return( 0 );

    // Check srclen: must be an even number, and hexbytes between 0 and dstlen
    int hexbytes = srclen / 2;
    if ( srclen < 0 || hexbytes > dstlen || (( srclen & 0x01 ) == 1 ) ) return( 0 );

    // Zero leading (hex)
    int i;
    for ( i=0; i < (dstlen-hexbytes); i++ ) *(dst++) = 0;

    // Process src nibbles
    int val;
    for ( i=0; i<srclen; i++ ) {
        if ( ( i & 0x01 ) == 0 ) {
            val = nibble2num( *(src++) );
        } else {
            val = ( val << 4 ) + nibble2num( *(src++) );
            *(dst++) = val;
        }
    }

    return( 1 );
}

/**
 * \brief Convert a hex number into a nibble string (with twice the size)
 * \param src Hex string
 * \param srclen Length of the hex string
 * \param dst User area to store the conversion result (minimally 2* srclen + 1)
 */
void hex2nibblestr( char * dst, char * src, int srclen ) {
    // Warning: dst buffer should be able to receive 2*srclen characters + 1
    int i;
    for ( i=0; i<srclen; i++ ) {
        *(dst++) = num2nibble( ( *src >> 4 ) & 0x0F );
        *(dst++) = num2nibble( *src & 0x0F );
        src++;
    }

    // + 1
    *(dst) = '\0';
}

/**
 * \brief Convert a hex string into a uint64
 * \param hex Hex string
 * \returns The converted uint64
 */
uint64_t hex2u64( char * hex ) {
    uint64_t u64 = 0;
    int i = 0;
    while ( i < 8 && hex[i] != '\0' ) {
        u64 = ( u64 << 8 ) + hex[i];
        i++;
    }
    return( u64 );
}

/**
 * \brief Convert a nibble string into a uint64
 * \param nibblestr Nibble string
 * \returns The converted uint64
 */
uint64_t nibblestr2u64( char * nibblestr ) {
    uint64_t u64 = 0;
    int i = 0;
    while ( i < 16 && nibblestr[i] != '\0' ) {
        u64 = ( u64 << 4 ) + (uint64_t)nibble2num( nibblestr[i] );
        i++;
    }
    return( u64 );
}

/**
 * \brief Converts a uint64 into a nibble string
 * \param u64 uint64 number
 * \param nibblestr User provided space to receive the conversion result (minimally 16 + 1 bytes)
 */
void u642nibblestr( uint64_t u64, char * nibblestr ) {
    // printf( "Converting 0x%016llx\n", (unsigned long long int) u64 );
    int i = 0;
    while ( i < 16 ) {
        nibblestr[15-i] = num2nibble( u64 & 0x0F );
        u64 >>= 4;
        i++;
    }
    nibblestr[16] = '\0';
}
