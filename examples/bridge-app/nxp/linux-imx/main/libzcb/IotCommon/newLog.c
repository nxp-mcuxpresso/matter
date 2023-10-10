// ------------------------------------------------------------------
// New Log - Based on shared memory
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief New Log - Based on shared memory
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
// #include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "dump.h"
#include "iotSemaphore.h"
#include "newLog.h"

#define NEWLOG_SHMKEY   99613
#define NEWLOG_SEMKEY   99621

// #define LOG_DEBUG

#ifdef LOG_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* LOG_DEBUG */

#define LL_LOG( f, t )
// #define LL_LOG( f, t ) filelog( f, t )

#define NEWLOG_MAX_LOGS        120


typedef struct newlog {

    int cnt;
    int index;
    int lastupdate;

    int reserve[7];

    onelog_t log[NEWLOG_MAX_LOGS];

} newlog_t;

// ------------------------------------------------------------------
// Globals
// ------------------------------------------------------------------

char * logNames[11] = { "none",
    "Control Interface",  // 1
    "Database",           // 2
    "NFC Daemon",         // 3
    "Secure Joiner",      // 4
    "ZCB-in",             // 5
    "ZCB-out",            // 6
    "CGI-script",         // 7
    "DBP",                // 8
    "Gw Discovery",       // 9
    "Test" };             // 10

static char * newLogSharedMemory = NULL;

char logbuffer[MAX_LOG_BUFFER];

// ------------------------------------------------------------------
// Open / Close
// ------------------------------------------------------------------

/**
 * \brief Open the Log module
 * \returns 1 on success, 0 on error
 */
static int newLogOpen( void ) {

    int shmid = -1, created = 0, ret = 0;

    LL_LOG( "/tmp/loggy", "In newLogOpen" );

    semPautounlock( NEWLOG_SEMKEY, 10 );

    // Locate the segment
    if ((shmid = shmget(NEWLOG_SHMKEY, sizeof(newlog_t), 0666)) < 0) {

        LL_LOG( "/tmp/loggy", "SHM not found" );

        // SHM not found: try to create
        if ((shmid = shmget(NEWLOG_SHMKEY, sizeof(newlog_t), IPC_CREAT | 0666)) < 0) {
            // Create error
            perror("shmget-create");
            printf( "Error creating SHM for Logs\n" );

            LL_LOG( "/tmp/loggy", "SHM create error" );
        } else {
            // Creation Successful: init structure
            created = 1;
            DEBUG_PRINTF( "Created new SHM for Logs\n" );
            LL_LOG( "/tmp/loggy", "SHM created" );
        }

    } else {
        DEBUG_PRINTF( "Linked to existing SHM for Logs (%d)\n", shmid );
        LL_LOG( "/tmp/loggy", "Linked to existing SHM" );
    }

    if ( shmid >= 0 ) {
        // Attach the segment to our data space
        if ((newLogSharedMemory = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            printf( "Error attaching SHM for Logs\n" );
            newLogSharedMemory = NULL;
            LL_LOG( "/tmp/loggy", "SHM attach error" );

        } else {
            DEBUG_PRINTF( "Successfully attached SHM for Logs\n" );
            LL_LOG( "/tmp/loggy", "Attached to SHM" );
            if ( created ) {
                LL_LOG( "/tmp/loggy", "Created, thus initialize" );
                // Wipe memory
                memset( newLogSharedMemory, 0, sizeof( newlog_t ) );

                // Newly created:  init structure
                newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;

                pnewlog->index = 0;

                int i;

                for ( i=0; i<NEWLOG_MAX_LOGS; i++ ) {
                    pnewlog->log[i].from    = 0;
                    pnewlog->log[i].text[0] = '\0';
                }

                DEBUG_PRINTF( "Initialized new SHM for DB\n" );
            }
            ret = 1;
        }
    } else {
        LL_LOG( "/tmp/loggy", "SHM error" );
    }

    semV( NEWLOG_SEMKEY );

    if ( ret ) {
        DEBUG_PRINTF( "Opening Log module succeeded\n" );
        LL_LOG( "/tmp/loggy", "Open succeeded" );
    } else {
        printf( "Error opening Log module\n" );
        LL_LOG( "/tmp/loggy", "Open failed" );
    }

    return( ret );
}

// -------------------------------------------------------------
// Add
// -------------------------------------------------------------

/**
 * \brief Adds a log line to the log module
 * \param from Where the log comes from
 * \param text Log-text
 * \returns 1 when ok, 0 on error
 */
int newLogAdd( newLogFrom from, char * text ) {
    if ( newLogSharedMemory || newLogOpen() ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        int now = (int)time( NULL );
        int len = strlen( text );

        semP( NEWLOG_SEMKEY );

        while ( len > 0 ) {
            pnewlog->log[pnewlog->index].from = from;
            pnewlog->log[pnewlog->index].ts = now;

            strncpy( pnewlog->log[pnewlog->index].text, text, NEWLOG_MAX_TEXT );
            pnewlog->log[pnewlog->index].text[NEWLOG_MAX_TEXT] = '\0';

            text += NEWLOG_MAX_TEXT;
            len -= NEWLOG_MAX_TEXT;

            if ( ++pnewlog->index >= NEWLOG_MAX_LOGS ) pnewlog->index = 0;
            pnewlog->cnt++;
        }

        pnewlog->lastupdate = now;
        semV( NEWLOG_SEMKEY );

        return 1;
    } else {
        printf( "Error adding log\n" );
    }
    return 0;
}


/**
 * \brief Empty the Log module
 * \returns 1 on success, 0 on error
 */
int newLogEmpty( void ) {
    if ( newLogSharedMemory || newLogOpen() ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        int now = (int)time( NULL );
        semP( NEWLOG_SEMKEY );
        pnewlog->cnt        = 0;
        pnewlog->index      = 0;
        pnewlog->lastupdate = now;
        semV( NEWLOG_SEMKEY );
        return 1;
    }
    return 0;
}

/**
 * \brief Gets the current Log module index
 * \returns index on success, -1 when empty
 */
int newLogGetIndex( void ) {
    int index = -1;
    if ( newLogSharedMemory || newLogOpen() ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        semP( NEWLOG_SEMKEY );
        if ( pnewlog->cnt > 0 ) index = pnewlog->index;
        semV( NEWLOG_SEMKEY );
    }
    return index;
}

// ------------------------------------------------------------------
// Loopers
// ------------------------------------------------------------------

/**
 * \brief Loops the current Log module from <fromindex> to current index, or all when <fromindex> < 0.
 * Note that the call-back may not alter the logs as it peeks right into the shared memory
 * \param cb Call-back function
 * \returns Last index on success, -1 on error
 */
int newLogLoop( int from, int startIndex, newlogCb_t cb ) {
    if ( cb && ( newLogSharedMemory || newLogOpen() ) ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        int i, cnt;

        semP( NEWLOG_SEMKEY );
        if ( startIndex < 0 ) {
            // Loop all
            cnt = pnewlog->cnt % NEWLOG_MAX_LOGS;
        } else {
            // Loop since
            cnt = pnewlog->index - startIndex;
            if ( cnt < 0 ) cnt += NEWLOG_MAX_LOGS;
        }
        int ok = 1, start = pnewlog->index - cnt;
        if ( start < 0 ) start += NEWLOG_MAX_LOGS;
        for ( i=0; i<cnt && ok; i++ ) {
            if ( from == NEWLOG_FROM_NONE || pnewlog->log[start].from == from ) {
                ok = cb( start, &pnewlog->log[start] );
            }
            if ( ++start >= NEWLOG_MAX_LOGS ) start = 0;
        }
        semV( NEWLOG_SEMKEY );

        return( start );
    }
    return -1;
}

// ------------------------------------------------------------------
// Low level logging
// ------------------------------------------------------------------

/**
 * \brief Log simply to file
 * \returns 1 on success, 0 on error
 */
int filelog( char * filename, char * text ) {
    char timestr[40];
    time_t clk;
    clk = time( NULL );
    sprintf( timestr, "%s", ctime( &clk ) );
    timestr[ strlen( timestr ) - 1 ] = '\0';

    printf( "Logging %s to %s\n", text, filename );

    FILE * f;
    if ( ( f = fopen( filename, "a" ) ) != NULL ) {
        fprintf( f, "%s - %d : %s\n", timestr, getpid(), text );
        fclose( f );
        return 1;
    }
    printf( "Error logging %s to %s\n", text, filename );
    return 0;
}
