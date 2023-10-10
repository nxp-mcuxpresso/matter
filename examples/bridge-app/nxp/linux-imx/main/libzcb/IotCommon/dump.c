// ------------------------------------------------------------------
// Hex/Ascii Dump helper
// ------------------------------------------------------------------
// Dumps a block of data as hex bytes followd by a printable ASCII
// representation
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief Hex/Ascii Dump helper: Dumps a block of data as hex bytes followd by a printable ASCII representation
 */

#include <stdio.h>

/**
 * \brief Dumps array of data, 16 values in one row, and with printable ascii representation
 * \param data Array of bytes
 * \param len Length of the byte array
 */
void dump( char * data, int len ) {
    char c;
    int i = 0;
    int offset = 0;
    while ( offset < len ) {
        for ( i = 0; i < 16; i++ ) {
            if ( ( offset + i ) < len ) {
                c = data[ offset + i ];
                printf( "%02x ", (unsigned char) c );
            } else {
                printf( "   " );
            }
        }
        printf( "   " );
        for ( i = 0; i < 16; i++ ) {
            if ( ( offset + i ) < len ) {
                c = data[ offset + i ];
                if ( c >= ' ' && c <= '~' ) {
                    printf( "%c", c );
                } else {
                    printf( "." );
                }
            } else {
                printf( " " );
            }
        }
        offset += 16;
        printf( "\n" );
    }
}

/**
 * \brief Dumps array of data
 * \param data Array of bytes
 * \param len Length of the byte array
 */
void dump_raw( char * data, int len ) {
    int i = 0;
    for ( i = 0; i < len; i++ ) {
        printf( "%02x ", *(data++) );
    }
}
