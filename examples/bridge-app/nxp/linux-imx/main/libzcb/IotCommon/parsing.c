// ------------------------------------------------------------------
// Parsing
// ------------------------------------------------------------------
// Help functions to parse JSON messages
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief Help functions to parse JSON messages
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "parsing.h"

// #define PARSING_DEBUG

#ifdef PARSING_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* PARSING_DEBUG */

int numIntAttrs    = 0;
int numStringAttrs = 0;

t_intattr    parsingIntAttrs[MAXINTATTRS];
t_stringattr parsingStringAttrs[MAXSTRINGATTRS];

// -------------------------------------------------------------
// String helpers
// -------------------------------------------------------------

/**
 * \brief Make a string empty
 */
void emptyString( char * string ) {
    string[0] = '\0';
}

/**
 * \brief Check if a string is empty
 * \returns 1 when empty, or 0 when not
 */
int isEmptyString( char * string ) {
    return( string[0] == '\0' );
}

// -------------------------------------------------------------
// Attributes
// -------------------------------------------------------------

/**
 * \brief Check if an integer with name <name> already exists in the parser array
 * \param name Name of the integer to parse
 * \returns The index in the array when found, or -1 when not found
 */
static int parsingExistsIntAttr( char * name ) {
    int i;
    for ( i=0; i<numIntAttrs; i++ ) {
        if ( strcmp( parsingIntAttrs[i].name, name ) == 0 ) {
            DEBUG_PRINTF( "INT %s exists (%d)\n", name, i );
            return( i );
        }
    }
    DEBUG_PRINTF( "INT %s is new\n", name );
    return( -1 );
}

/**
 * \brief Check if a string with name <name> already exists in the parser array
 * \param name Name of the string to parse
 * \returns The index in the array when found, or -1 when not found
 */
static int parsingExistsStringAttr( char * name ) {
    int i;
    for ( i=0; i<numStringAttrs; i++ ) {
        if ( strcmp( parsingStringAttrs[i].name, name ) == 0 ) {
            DEBUG_PRINTF( "STRING %s exists (%d)\n", name, i );
            return( i );
        }
    }
    DEBUG_PRINTF( "STRING %s is new\n", name );
    return( -1 );
}

// ------------------------------------------------------------------
// Add
// ------------------------------------------------------------------

/**
 * \brief Add an integer name to the parsing array
 * \param name Name of the integer
 * \returns 1 on success, or 0 on error
 */
int parsingAddIntAttr( char * name ) {
    DEBUG_PRINTF( "Add INT attr %s (%d)\n", name, numIntAttrs );
    if ( parsingExistsIntAttr( name ) < 0 ) {
        if ( numIntAttrs < MAXINTATTRS ) {
            parsingIntAttrs[numIntAttrs].name = name;
            numIntAttrs++;
            return( 1 );
        } else {
            printf( "********** Error adding INT-attr %s\n", name );
        }
    }
    return( 0 );
}

/**
 * \brief Add an string name to the parsing array
 * \param name Name of the string
 * \returns 1 on success, or 0 on error
 */
int parsingAddStringAttr( char * name, int maxlen ) {
    DEBUG_PRINTF( "Add STRING attr %s (%d)\n", name, numStringAttrs );
    int num;
    if ( ( num = parsingExistsStringAttr( name ) ) < 0 ) {
        if ( numStringAttrs < MAXSTRINGATTRS ) {
            parsingStringAttrs[numStringAttrs].name   = name;
            parsingStringAttrs[numStringAttrs].maxlen = maxlen;
            numStringAttrs++;
            return( 1 );
        } else {
            printf( "********** Error adding STRING-attr %s\n", name );
        }
    } else if ( parsingStringAttrs[num].maxlen < maxlen ) {
        parsingStringAttrs[num].maxlen = maxlen;
    }
    return( 0 );
}

// ------------------------------------------------------------------
// Reset
// ------------------------------------------------------------------

/**
 * \brief Reset the parser (and the integer and string arrays)
 */
void parsingReset( void ) {
    int i;
    for ( i=0; i<numIntAttrs; i++ ) {
         parsingIntAttrs[i].value = INT_MIN;
    }
    for ( i=0; i<numStringAttrs; i++ ) {
         emptyString( parsingStringAttrs[i].value );
    }
}

// ------------------------------------------------------------------
// Set
// ------------------------------------------------------------------

/**
 * \brief Set an integer in the parsing array to a certain value
 * \param name Name of the integer
 * \param value Value to be set
 * \returns 1 on success, or 0 on error
 */
int parsingIntAttr( char * name, int value ) {
    int i;
    for ( i=0; i<numIntAttrs; i++ ) {
        if ( strcmp( parsingIntAttrs[i].name, name ) == 0 ) {
            parsingIntAttrs[i].value = value;
            DEBUG_PRINTF( "INT %s = %d\n", name, value );
            return( 1 );
        }
    }
    return( 0 );
}

/**
 * \brief Set a string in the parsing array to a certain value
 * \param name Name of the string
 * \param value Value to be set
 * \returns 1 on success, or 0 on error
 */
int parsingStringAttr( char * name, char * value ) {
    int i;
    for ( i=0; i<numStringAttrs; i++ ) {
        if ( strcmp( parsingStringAttrs[i].name, name ) == 0 ) {
            if ( strlen( value ) <= parsingStringAttrs[i].maxlen ) {
                strcpy( parsingStringAttrs[i].value, value );
                DEBUG_PRINTF( "STRING %s = %s\n", name, value );
                return( 1 );
            } else {
                printf( "********** Error: String value for %s too long\n",
                     parsingStringAttrs[i].name );
            }
        }
    }
    return( 0 );
}

// ------------------------------------------------------------------
// Get
// ------------------------------------------------------------------

/**
 * \brief Get the value of an integer after parsing took place
 * \param name Name of the integer
 * \return The value for the integer or INT_MIN when the integer was not found/parsed
 */
int parsingGetIntAttr( char * name ) {
    int i;
    for ( i=0; i<numIntAttrs; i++ ) {
        if ( strcmp( parsingIntAttrs[i].name, name ) == 0 ) {
            return( parsingIntAttrs[i].value );
        }
    }
    return( INT_MIN );
}

/**
 * \brief Get the value of a string after parsing took place
 * \param name Name of the string
 * \return The value for the string or NULL when the string was not found/parsed
 */
char * parsingGetStringAttr( char * name ) {
    int i;
    for ( i=0; i<numStringAttrs; i++ ) {
        if ( strcmp( parsingStringAttrs[i].name, name ) == 0 ) {
            return( parsingStringAttrs[i].value );
        }
    }
    return( NULL );
}

/**
 * \brief Get the value of a string after parsing took place
 * \param name Name of the string
 * \return The value for the string or NULL when the string was not found/parsed or was empty
 */
char * parsingGetStringAttr0( char * name ) {
    int i;
    for ( i=0; i<numStringAttrs; i++ ) {
        if ( strcmp( parsingStringAttrs[i].name, name ) == 0 ) {
            if ( parsingStringAttrs[i].value[0] != '\0' ) {
                return( parsingStringAttrs[i].value );
            } else {
                return( NULL );
            }
        }
    }
    return( NULL );
}
