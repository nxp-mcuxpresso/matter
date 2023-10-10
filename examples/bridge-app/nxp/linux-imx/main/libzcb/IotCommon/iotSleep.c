// ------------------------------------------------------------------
// IotSleep
// ------------------------------------------------------------------
// IoT sleep functions
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief IoT sleep function
 */

#include <stdio.h>
#include <unistd.h>

#define SLEEP_DEBUG

#ifdef SLEEP_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* SLEEP_DEBUG */

// ------------------------------------------------------------------
// Function
// ------------------------------------------------------------------

/**
 * \brief Sleep for so many milli-seconds
 * \param who String describing who is sleeping
 * \param msec Number of milli-seconds
 */
int iotMsleep( const char * who, int msec ) {
    DEBUG_PRINTF( "Function '%s' sleeps for %d msec\n", who, msec );
    return usleep( (unsigned int)msec * 1000 );
}

/**
 * \brief Sleep for so many seconds
 * \param who String describing who is sleeping
 * \param sec Number of seconds
 */
int iotSleep( const char * who, int sec ) {
    DEBUG_PRINTF( "Function '%s' sleeps for %d sec\n", who, sec );
    return sleep( sec );
}
