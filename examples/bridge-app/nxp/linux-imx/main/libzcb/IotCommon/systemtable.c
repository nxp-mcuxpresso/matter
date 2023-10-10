// ------------------------------------------------------------------
// System table helper
// ------------------------------------------------------------------
// Database functions with special wrappers for the System Table
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief Database functions with special wrappers for the System Table
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "newDb.h"
#include "systemtable.h"

// #define SYS_DEBUG

#ifdef SYS_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* SYS_DEBUG */

// -------------------------------------------------------------
// Get
// -------------------------------------------------------------

/**
 * \brief Get the integer value of an entry in the system table
 * \param name Name of the entry
 * \returns The value, or -1 when entry was not found
 */
int newDbSystemGetInt( char * name ) {
    newdb_system_t sys;
    if ( newDbGetSystem( name, &sys ) ) {
        return( sys.intval );
    }
    return -1;
}

/**
 * \brief Get the string value of an entry in the system table
 * \param name Name of the entry
 * \param str User-allocated area to receive the sting value (must be big enough, LEN_SYSSTR + 1)
 * \returns The value, or NULL when entry was not found
 */
char * newDbSystemGetString( char * name, char * str ) {
    newdb_system_t sys;
    if ( str && newDbGetSystem( name, &sys ) ) {
        strcpy( str, sys.strval );
        return( str );
    }
    return NULL;
}

// -------------------------------------------------------------
// Save
// - Updates when exists, otherwise adds
// -------------------------------------------------------------

/**
 * \brief Set an integer entry in the system database. Creates it when it does not yet exists
 * \param name Name of the entry
 * \param intval New value
 * \returns 1 on success, or 0 on error
 */
int newDbSystemSaveIntval( char * name, int intval ) {
    newdb_system_t sys;
    if ( newDbGetSystem( name, &sys ) ) {
        sys.intval = intval;
        sys.strval[0] = '\0';
        return newDbSetSystem( &sys );
    } else if ( newDbGetNewSystem( name, &sys ) ) {
        sys.intval = intval;
        sys.strval[0] = '\0';
        return newDbSetSystem( &sys );
    }
    return 0;
}

/**
 * \brief Set a string entry in the system database. Creates it when it does not yet exists
 * \param name Name of the entry
 * \param stringval New value
 * \returns 1 on success, or 0 on error
 */
int newDbSystemSaveStringval( char * name, char * stringval ) {
    if ( stringval ) {
        newdb_system_t sys;
        if ( newDbGetSystem( name, &sys ) ) {
            sys.intval = 0;
            newDbStrNcpy( sys.strval, stringval, LEN_SYSSTR );
            return newDbSetSystem( &sys );
        } else if ( newDbGetNewSystem( name, &sys ) ) {
            sys.intval = 0;
            newDbStrNcpy( sys.strval, stringval, LEN_SYSSTR );
            return newDbSetSystem( &sys );
        }
    }
    return 0;
}

/**
 * \brief Set an entry in the system database. Creates it when it does not yet exists
 * \param name Name of the entry
 * \param intval New value
 * \param stringval New value
 * \returns 1 on success, or 0 on error
 */
int newDbSystemSaveVal( char * name, int intval, char * stringval ) {
    if ( stringval ) {
        newdb_system_t sys;
        if ( newDbGetSystem( name, &sys ) ) {
            sys.intval = intval;
            newDbStrNcpy( sys.strval, stringval, LEN_SYSSTR );
            return newDbSetSystem( &sys );
        } else if ( newDbGetNewSystem( name, &sys ) ) {
            sys.intval = intval;
            newDbStrNcpy( sys.strval, stringval, LEN_SYSSTR );
            return newDbSetSystem( &sys );
        }
    }
    return 0;
}
