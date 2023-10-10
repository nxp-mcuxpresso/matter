// ------------------------------------------------------------------
// New IoT DB - Based on shared memory
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief New IoT DB - Based on shared memory
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include "dump.h"
#include "iotSemaphore.h"
#include "fileCreate.h"
#include "newLog.h"
#include "newDb.h"
#include "ZigbeeConstant.h"



#ifdef TARGET_LINUX_PC
#define DB_FILEPATH     "/tmp/iot-test/usr/share/iot/"
#else
#define DB_FILEPATH     "/usr/share/iot/"
#endif

#define DB_FILENAME     DB_FILEPATH "/iot_newdb.db"
#define DB_FILENAME_BCK DB_FILEPATH "/iot_newdb.bck"

#define NEWDB_SHMKEY      86156
#define NEWDB_SEMKEY      8616
#define NEWDB_SEMSAVEKEY  8620

// #define DB_DEBUG

#ifdef DB_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* DB_DEBUG */

#define LL_LOG( f, t )
// #define LL_LOG( f, t ) filelog( f, t )

#define NEWDB_VERSION         1

#define NEWDB_MAX_SYSTEM      20
#define NEWDB_MAX_ROOMS       10
#define NEWDB_MAX_DEVICES     40
#define NEWDB_MAX_PLUGHIST    400
#define NEWDB_MAX_ZCB         40

// #define DB_MULTIPLE_FILES    1
// #define ALSO_SAVE_PLUGHIST   1
#define PLUGHIST_AUTO_REMOVE_OLDEST

typedef struct newdb {
    int version;
    int numwrites;
    int network_state;

    int lastupdate_sys;
    int lastupdate_rooms;
    int lastupdate_devices;
    int lastupdate_plughist;
    int lastupdate_zcb;

    int reserve[11];

    newdb_system_t system[NEWDB_MAX_SYSTEM];

//    newdb_room_t rooms[NEWDB_MAX_ROOMS];

    newdb_dev_t devices[NEWDB_MAX_DEVICES];

    newdb_plughist_t plughist[NEWDB_MAX_PLUGHIST];

    newdb_zcb_t zcb[NEWDB_MAX_ZCB];

} newdb_t;

// ------------------------------------------------------------------
// Globals
// ------------------------------------------------------------------

static char * newDbSharedMemory = NULL;

static int lastupdate_sys = 0;
static int lastupdate_rooms = 0;
static int lastupdate_devices = 0;
static int lastupdate_plughist = 0;
static int lastupdate_zcb = 0;

// ------------------------------------------------------------------
// Tables
// ------------------------------------------------------------------

#define NUM_COLUMNS_SYSTEM 5

static char * newdb_system_columns[NUM_COLUMNS_SYSTEM] = {
    "id",
    "name",
    "intval",
    "strval",
    "lastupdate"
};


#define NUM_COLUMNS_DEV         27

static char * newdb_devs_columns[NUM_COLUMNS_DEV] = {
    "id",
    "mac",
    "dev",
    "ty",
    "par",
    "nm",
    "heat",      // UI
    "cool",      // UI
    "tmp",       // SENSOR
    "hum",       // SENSOR
    "prs",       // SENSOR
    "co2",       // SENSOR
    "bat",       // SENSOR
    "batl",      // SENSOR
    "als",       // SENSOR
    "xloc",      // SENSOR
    "yloc",      // SENSOR
    "zloc",      // SENSOR
    "sid",       // ACT
    "cmd",       // ACT
    "lvl",       // ACT
    "rgb",       // LAMP
    "kelvin",    // LAMP
    "act",       // PLUG
    "sum",       // PLUG
    "flags",
    "lastupdate"
    };

    #define NUM_COLUMNS_PLUGHIST 4

    static char * newdb_plughist_columns[NUM_COLUMNS_PLUGHIST] = {
    "id",
    "mac",
    "sum",       // PLUG
    "lastupdate"
};

#define NUM_COLUMNS_ZCB 6

static char * newdb_zcb_columns[NUM_COLUMNS_ZCB] = {
    "id",
    "mac",
    "status",
    "saddr",
    "type",
    "lastupdate"
};

// ------------------------------------------------------------------
// File lock
// ------------------------------------------------------------------

/**
 * \brief dbSave lock
 */
void newDbFileLock( void ) {
    // Do not allow mutiple threads to save the dB at the same time
    semPautounlock( NEWDB_SEMSAVEKEY, 20 );
    DEBUG_PRINTF( "DB save locked\n" );
}

/**
 * \brief dbSave unlock
 */
void newDbFileUnlock( void ) {
    semV( NEWDB_SEMSAVEKEY );
    DEBUG_PRINTF( "DB save unlocked\n" );
}

// ------------------------------------------------------------------
// Save
// ------------------------------------------------------------------

/**
 * \brief Save a certain table-part of the database in a specific table file
 * \param tablename Name of the table
 * \param data Table-part of the database
 * \param len Size in bytes of this table-part
 * \returns 1 on success, 0 on error
 */
static int newDbSaveTable( char * tablename, char * data, int len ) {
    DEBUG_PRINTF( "DB save table %s (%d)\n", tablename, len );
    struct stat sb;
    char filename[40];
    char backup[40];

    if ( tablename && data && len > 0 ) {

        // Make full filenames
        sprintf( filename, "%siot_%s.db", DB_FILEPATH, tablename );
        sprintf( backup, "%siot_%s.bck", DB_FILEPATH, tablename );

        // Check if we still have a database file
        if ( stat(filename, &sb) == -1) {
            DEBUG_PRINTF( "No dB file found. Stat %s, errno = %d\n", filename, errno );
        } else {
            // Yes, so we can throw away the backup
            // First check if we have one
            if ( stat(backup, &sb) == -1) {
                DEBUG_PRINTF( "No backup found. Stat %s, errno = %d\n", backup, errno );
            } else {
                // Yes, we have a backup file
                if ( remove( backup ) != 0 ) {
                    printf( "Error removing database file backup %s (%d - %s)\n",
                            backup, errno, strerror( errno ) );
                    newLogAdd( NEWLOG_FROM_DATABASE, "Error removing database file" );
                    // Do not progress
                    return 0;
                }
            }

            // Rename the database file to the backup
            if ( rename( filename, backup ) != 0 ) {
                printf( "Error backing-up database file %s (%d - %s)\n",
                        filename, errno, strerror( errno ) );
                newLogAdd( NEWLOG_FROM_DATABASE, "Error backing-up database file" );
                // Do not progress
                return 0;
            }
        }

        if ( stat(filename, &sb) == -1) {
            printf( "dB file gone\n" );
        } else {
            printf( "dB file still there\n" );
        }
        if ( stat(backup, &sb) == -1) {
            printf( "dB backup gone\n" );
        } else {
            printf( "dB backup still there\n" );
        }

        if ( fileCreateRW( filename ) ) {
            DEBUG_PRINTF( "Re-created database file %s\n", filename );

            // Now write the data
            int bytestowrite = len;
            int fd = open( filename, O_RDWR );
            if ( fd >= 0 ) {
                char * buf = data;
                int byteswritten;
                while ( ( bytestowrite > 0 ) &&
                        ( byteswritten = write( fd, buf, bytestowrite ) ) >= 0 ) {
                    bytestowrite -= byteswritten;
                    buf += byteswritten;
                }
                // Make sure that the file is really written
                fsync( fd );

                // Now close
                close( fd );
            }
            return( bytestowrite == 0 );
        } else {
            printf( "Error re-creating database file %s\n", filename );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error re-creating database file" );
        }
    }

    return 0;
}

/**
 * \brief Save the IoT database as different tables
 * \returns 1 on success, 0 on error
 */
int newDbSave( void ) {
    DEBUG_PRINTF( "DB save\n" );

    if ( newDbSharedMemory ) {
        // First make a DB copy so that we do not block the database too long
        int len = sizeof( newdb_t );
        char * dbCopy = malloc( len );
        if ( dbCopy ) {
            semPautounlock( NEWDB_SEMKEY, 10 );
            memcpy( dbCopy, newDbSharedMemory, sizeof( newdb_t ) );
            semV( NEWDB_SEMKEY );

            newdb_t * pnewdb = (newdb_t *)dbCopy;

#if DB_MULTIPLE_FILES

            if ( pnewdb->lastupdate_sys != lastupdate_sys ) {
                if ( newDbSaveTable( "sys", (char *)pnewdb->system, sizeof( pnewdb->system ) ) ) {
                    lastupdate_sys = pnewdb->lastupdate_sys;
                }
            }
            if ( pnewdb->lastupdate_rooms != lastupdate_rooms ) {
                if ( newDbSaveTable( "rooms", (char *)pnewdb->rooms, sizeof( pnewdb->rooms ) ) ) {
                    lastupdate_rooms = pnewdb->lastupdate_rooms;
                }
            }
            if ( pnewdb->lastupdate_devices != lastupdate_devices ) {
                if ( newDbSaveTable( "devs", (char *)pnewdb->devices, sizeof( pnewdb->devices ) ) ) {
                    lastupdate_devices = pnewdb->lastupdate_devices;
                }
            }
            if ( pnewdb->lastupdate_plughist != lastupdate_plughist ) {
#if ALSO_SAVE_PLUGHIST
                if ( newDbSaveTable( "plughist", (char *)pnewdb->plughist, sizeof( pnewdb->plughist ) ) ) {
                    lastupdate_plughist = pnewdb->lastupdate_plughist;
                }
#else
                    lastupdate_plughist = pnewdb->lastupdate_plughist;
#endif
            }
            if ( pnewdb->lastupdate_zcb != lastupdate_zcb ) {
                if ( newDbSaveTable( "zcb", (char *)pnewdb->zcb, sizeof( pnewdb->zcb ) ) ) {
                    lastupdate_zcb = pnewdb->lastupdate_zcb;
                }
            }

#else // DB_MULTIPLE_FILES

            // Use single file
            if ( pnewdb->lastupdate_sys      != lastupdate_sys ||
                 pnewdb->lastupdate_rooms    != lastupdate_rooms ||
                 pnewdb->lastupdate_devices  != lastupdate_devices ||
                 pnewdb->lastupdate_plughist != lastupdate_plughist ||
                 pnewdb->lastupdate_zcb      != lastupdate_zcb ) {

                if ( newDbSaveTable( "database", (char *)dbCopy, sizeof( newdb_t ) ) ) {
                    lastupdate_sys      = pnewdb->lastupdate_sys;
                    lastupdate_rooms    = pnewdb->lastupdate_rooms;
                    lastupdate_devices  = pnewdb->lastupdate_devices;
                    lastupdate_plughist = pnewdb->lastupdate_plughist;
                    lastupdate_zcb      = pnewdb->lastupdate_zcb;
                    DEBUG_PRINTF( "db Save done\n" );
                } else {
                    DEBUG_PRINTF( "db Save error\n" );
                }
            }

#endif // DB_MULTIPLE_FILES

            free( dbCopy );

        } else {
            printf( "Error mallocing memory for dbCopy: %d - %s\n", errno, strerror( errno ) );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error mallocing memory for dbCopy" );
        }
    }

    return 0;
}

// ------------------------------------------------------------------
// Restore
// ------------------------------------------------------------------

/**
 * \brief Restore a certain table in the IoT database
 * \param tablename Name of the table
 * \param data Table-part of the database
 * \param len Size in bytes of this table-part
 * \returns 1 on success, 0 on error
 */
static int newDbRestoreTable( char * tablename, char * data, int len ) {
    DEBUG_PRINTF( "DB restore table %s (%d)\n", tablename, len );

    char filename[40];
    char backup[40];

    if ( tablename && data && len > 0 ) {

        // Make full filenames
        sprintf( filename, "%siot_%s.db", DB_FILEPATH, tablename );
        sprintf( backup, "%siot_%s.bck", DB_FILEPATH, tablename );

        int fd = open( filename, O_RDONLY );
        if ( fd >= 0 ) {
            DEBUG_PRINTF( "Opening database file %s succeeded\n", filename );
        } else {
            DEBUG_PRINTF( "Opening database file %s failed: try opening the backup %s\n", filename, backup );
            fd = open( backup, O_RDONLY );
        }
        if ( fd >= 0 ) {
            int bytestoread = len;
            int bytesread;
            char * pbuf = data;
            while ( ( bytestoread > 0 ) &&
                    ( bytesread = read( fd, pbuf, bytestoread ) ) > 0 ) {
                bytestoread -= bytesread;
                pbuf += bytesread;
                }
            close( fd );
            if ( bytestoread == 0 ) {
                DEBUG_PRINTF( "Successfully restored database table %s\n", tablename );
            }
            return( bytestoread == 0 );
        }
    }
    return 0;
}

/**
 * \brief Restores the IoT database from different tables. Note: needs to be called inside semaphore section
 * \returns 1 on success, 0 on error
 */
static int newDbRestore( void ) {

    int ret = 0;

    LL_LOG( "/tmp/dbby", "Restore DB from file(s)" );

    newDbFileLock();

    if ( newDbSharedMemory ) {
        int len = sizeof( newdb_t );
        // Create a tmp buffer
        char * dbCopy = malloc( len );
        if ( dbCopy ) {

            memset( dbCopy, 0, len );

            newdb_t * pnewdb = (newdb_t *)dbCopy;

#if DB_MULTIPLE_FILES

            newDbRestoreTable( "sys",      (char *)pnewdb->system,   sizeof( pnewdb->system ) );
            newDbRestoreTable( "rooms",    (char *)pnewdb->rooms,    sizeof( pnewdb->rooms ) );
            newDbRestoreTable( "devs",     (char *)pnewdb->devices,  sizeof( pnewdb->devices ) );
#if ALSO_SAVE_PLUGHIST
            newDbRestoreTable( "plughist", (char *)pnewdb->plughist, sizeof( pnewdb->plughist ) );
#endif
            newDbRestoreTable( "zcb",      (char *)pnewdb->zcb,      sizeof( pnewdb->zcb ) );

            memcpy( newDbSharedMemory, dbCopy, len );
            ret = 1;

#else // DB_MULTIPLE_FILES

            // Use single file
            if ( newDbRestoreTable( "database", (char *)pnewdb,   sizeof( newdb_t ) ) ) {

                // Check version on DB restore
                if ( pnewdb->version == NEWDB_VERSION ) {
                    LL_LOG( "/tmp/dbby", "\tDB version is OK" );
                    memcpy( newDbSharedMemory, dbCopy, len );
                    ret = 1;
                } else {
                    sprintf( logbuffer, "Incompatible database version: %d != %d", pnewdb->version, NEWDB_VERSION );
                    printf( "%s\n", logbuffer );
                    newLogAdd( NEWLOG_FROM_DATABASE, logbuffer );
                    LL_LOG( "/tmp/dbby", logbuffer );
                }

            }

#endif // DB_MULTIPLE_FILES

            free( dbCopy );
        } else {
            printf( "Error mallocing memory for dbCopy: %d - %s\n", errno, strerror( errno ) );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error mallocing memory for dbCopy" );
            LL_LOG( "/tmp/dbby", "\tMalloc error" );
        }
    }

    newDbFileUnlock();

    if ( ret ) {
        DEBUG_PRINTF( "DB restore succeeded\n" );
        LL_LOG( "/tmp/dbby", "\tDB restore succeeded" );
    } else {
        DEBUG_PRINTF( "DB restore failed\n" );
    }

    return ret;
}

// ------------------------------------------------------------------
// Open / Close
// ------------------------------------------------------------------

/**
 * \brief Open the IoT database
 * \returns 1 on success, 0 on error
 */

int newDbOpen( void ) {

    int shmid = -1, created = 0, ret = 0;

    LL_LOG( "/tmp/dbby", "In newDbOpen" );

    semPautounlock( NEWDB_SEMKEY, 10 );

    // Locate the segment
    if ((shmid = shmget(NEWDB_SHMKEY, sizeof(newdb_t), 0666)) < 0) {
        LL_LOG( "/tmp/dbby", "\tDB SHM not found" );
        // SHM not found: try to create
        if ((shmid = shmget(NEWDB_SHMKEY, sizeof(newdb_t), IPC_CREAT | 0666)) < 0) {
            // Create error
            perror("shmget-create");
            printf( "Error creating SHM for DB\n" );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error creating SHM for DB" );
            LL_LOG( "/tmp/dbby", "\tDB SHM create error" );
        } else {
            // Creation Successful: init structure
            created = 1;
            DEBUG_PRINTF( "Created new SHM for DB\n" );
            LL_LOG( "/tmp/dbby", "\tDB SHM created" );
        }

    } else {
        DEBUG_PRINTF( "Linked to existing SHM for DB (%d)\n", shmid );
        LL_LOG( "/tmp/dbby", "\tLinked to existing DB SHM" );
    }

    if ( shmid >= 0 ) {
        // Attach the segment to our data space
        if ((newDbSharedMemory = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            printf( "Error attaching SHM for DB\n" );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error attaching SHM for DB" );
            newDbSharedMemory = NULL;
            LL_LOG( "/tmp/dbby", "\tDB SHM attach error" );

        } else {
            DEBUG_PRINTF( "Successfully attached SHM for DB (%d)\n", created );
            LL_LOG( "/tmp/dbby", "\tAttached to DB SHM" );
            if ( created ) {

                LL_LOG( "/tmp/dbby", "\tDB SHM created thus try to restore" );
                // Wipe memory
                memset( newDbSharedMemory, 0, sizeof( newdb_t ) );

                // Newly created: Try read from file
                if ( !newDbRestore() ) {
                    LL_LOG( "/tmp/dbby", "\tDB Restore failed: init" );
                    DEBUG_PRINTF( "dB Restore from file failed: init structure\n" );
                    newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;

                    pnewdb->version = NEWDB_VERSION;

                    int i;

                    for ( i=0; i<NEWDB_MAX_SYSTEM; i++ ) {
                        pnewdb->system[i].id      = i;
                        pnewdb->system[i].name[0] = '\0';
                    }
                    for ( i=0; i<NEWDB_MAX_DEVICES; i++ ) {
                        pnewdb->devices[i].id     = i;
                        pnewdb->devices[i].mac[0] = '\0';
                    }
                    for ( i=0; i<NEWDB_MAX_PLUGHIST; i++ ) {
                        pnewdb->plughist[i].id     = i;
                        pnewdb->plughist[i].mac[0] = '\0';
                    }
                    for ( i=0; i<NEWDB_MAX_ZCB; i++ ) {
                        pnewdb->zcb[i].id     = i;
                        pnewdb->zcb[i].status = ZCB_STATUS_FREE;
                    }
                }

                DEBUG_PRINTF( "Initialized new SHM for DB\n" );
            }
            ret = 1;
        }
    }

    semV( NEWDB_SEMKEY );

    if ( ret ) {
        DEBUG_PRINTF( "Opening DB succeeded\n" );
        LL_LOG( "/tmp/dbby", "\tDB Open succeeded" );
    } else {
        printf( "Error opening DB\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error opening DB" );
        LL_LOG( "/tmp/dbby", "\tDB Open failed" );
    }

    return( ret );
}

/**
 * \brief Close the IoT database
 * \returns 1 on success, 0 on error
 */
int newDbClose( void ) {
    return 1;
}

// -------------------------------------------------------------
// String helpers
// -------------------------------------------------------------

/**
 * \brief Save string copy into the database, taing the maximum field length into account
 * \param dst Destination
 * \param src Source
 * \param max Maximum length of the field
 */
void newDbStrNcpy( char * dst, char * src, int max ) {
    if ( dst ) {
        if ( src ) {
            strncpy( dst, src, max );
            dst[max] = '\0';
        }
    }
}

// -------------------------------------------------------------
// System table
// -------------------------------------------------------------

/**
 * \brief Get a personal/writable copy from the sytem table row with key <name>
 * \param name Key to the table row
 * \param psys Pointer to a caller's entry structure
 * \returns 1 when found, 0 when not found
 */
int newDbGetSystem( char * name, newdb_system_t * psys ) {
    int found = 0, index = 0;
    if ( newDbSharedMemory && name && psys) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_SYSTEM && !found; i++ ) {
            DEBUG_PRINTF( "Search system %s (%d) with name %s\n", pnewdb->system[i].name, i, name );
            if ( strcmp( pnewdb->system[i].name, name ) == 0 ) {
                memcpy( psys, &pnewdb->system[i], sizeof( newdb_system_t ) );
                found = 1;
                index = i;
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Found system with name %s (%d)\n", name, index );
        } else {
            DEBUG_PRINTF( "System %s not found\n", name );
        }
    } else {
        printf( "Error getting system\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error getting system" );
    }
    return( found );
}

/**
 * \brief Adds a new row to the system table and returns a personal/writable copy of this
 * table row with key <name>
 * \param name Key to the new table row
 * \param psys Pointer to a caller's entry structure
 * \returns 1 on success, 0 on error
 */
int newDbGetNewSystem( char * name, newdb_system_t * psys ) {
    int added = 0, index = 0;
    if ( newDbSharedMemory && name && psys ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_SYSTEM && !added; i++ ) {
            if ( pnewdb->system[i].name[0] == '\0' ) {
                DEBUG_PRINTF( "Adding %s to system (%d)\n", name, i );
                memset( &pnewdb->system[i], 0, sizeof( newdb_system_t ) );
                pnewdb->system[i].id = i;
                pnewdb->system[i].lastupdate = now;
                newDbStrNcpy( pnewdb->system[i].name, name, LEN_NM );
                memcpy( psys, &pnewdb->system[i], sizeof( newdb_system_t ) );
                added = 1;
                index = i;
                pnewdb->numwrites++;
                pnewdb->lastupdate_sys = now;
            }
        }
        semV( NEWDB_SEMKEY );
        if ( added ) {
            DEBUG_PRINTF( "Adding system %s succeeded\n", name );
            newLogAdd( NEWLOG_FROM_DATABASE, "Inserted entry in system table" );
#ifdef DB_DEBUG
            dump( (char *)&pnewdb->system[index], sizeof( newdb_system_t ) );
#endif
        } else {
            printf( "Error adding system %s\n", name );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error adding system" );
        }
    } else {
        printf( "Error adding system\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error adding system" );
    }
    return( added );
}

/**
 * \brief Sets the sytem table row with the same id as the row data
 * \param psys Pointer to an entry structure that was obtained by a get-function
 * \returns 1 on success, 0 on error
 */
int newDbSetSystem( newdb_system_t * psys ) {
    if ( newDbSharedMemory && psys && ( psys->id >= 0 && psys->id < NEWDB_MAX_SYSTEM ) ) {
        int now = (int)time( NULL );
        psys->lastupdate = now;
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        memcpy( &pnewdb->system[psys->id], psys, sizeof( newdb_system_t ) );
        pnewdb->numwrites++;
        pnewdb->lastupdate_sys = now;
        semV( NEWDB_SEMKEY );
        DEBUG_PRINTF( "Setting system %s succeeded (%d): %d, '%s'\n", psys->name, psys->id, psys->intval, psys->strval );
        newLogAdd( NEWLOG_FROM_DATABASE, "Updated system table" );
        return 1;
    } else {
        printf( "Error setting system\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error setting system" );
    }
    return 0;
}

/**
 * \brief Empty the system table
 * \returns 1 on success, 0 on error
 */
int newDbEmptySystem( void ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_SYSTEM; i++ ) {
            pnewdb->system[i].id      = i;
            pnewdb->system[i].name[0] = '\0';
        }
        pnewdb->numwrites++;
        pnewdb->lastupdate_sys = now;
        semV( NEWDB_SEMKEY );
        newLogAdd( NEWLOG_FROM_DATABASE, "Emptied system table" );
        return 1;
    }
    return 0;
}


// ------------------------------------------------------------------
// Device
// ------------------------------------------------------------------

/**
 * \brief Get a personal/writable copy from the device table row with key <mac>
 * \param mac Key to the table row
 * \param pdev Pointer to a caller's entry structure
 * \returns 1 when found, 0 when not found
 */
int newDbGetDevice( char * mac, newdb_dev_t * pdev ) {
    int found = 0;
    if ( newDbSharedMemory && mac && pdev ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES && !found; i++ ) {
            if ( strcmp( pnewdb->devices[i].mac, mac ) == 0 ) {
                memcpy( pdev, &pnewdb->devices[i], sizeof( newdb_dev_t ) );
                found = 1;
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Found device with mac %s\n", mac );
        } else {
            DEBUG_PRINTF( "Device %s not found\n", mac );
        }
    } else {
        printf( "Error getting device\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error getting device" );
    }
    return( found );
}

/**
 * \brief Get a personal/writable copy from the device table row with key <id>
 * \param id Key to the table row
 * \param pdev Pointer to a caller's entry structure
 * \returns 1 when found, 0 when not found
 */
int newDbGetDeviceId( int id, newdb_dev_t * pdev ) {
    if ( newDbSharedMemory && pdev && ( id >= 0 && id < NEWDB_MAX_DEVICES ) ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        memcpy( pdev, &pnewdb->devices[id], sizeof( newdb_dev_t ) );
        semV( NEWDB_SEMKEY );
        DEBUG_PRINTF( "Found device with id %d\n", id );
        return 1;
    } else {
        printf( "Error getting device\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error getting device" );
    }
    return 0;
}

/**
 * \brief Adds a new row to the device table and returns a personal/writable copy of this
 * table row with key <mac>
 * \param mac Key to the table row
 * \param pdev Pointer to a caller's entry structure
 * \returns 1 when found, 0 when not found
 */
int newDbGetNewDevice( char * mac, newdb_dev_t * pdev ) {
    int added = 0, index = 0;
    if ( newDbSharedMemory && mac && pdev ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES && !added; i++ ) {
            if ( pnewdb->devices[i].mac[0] == '\0' ) {
                memset( &pnewdb->devices[i], 0, sizeof( newdb_dev_t ) );
                pnewdb->devices[i].id = i;
                newDbStrNcpy( pnewdb->devices[i].mac, mac, LEN_MAC_NIBBLE );
                pnewdb->devices[i].lastupdate = now;
                memcpy( pdev, &pnewdb->devices[i], sizeof( newdb_dev_t ) );
                added = 1;
                index = i;
                pnewdb->numwrites++;
                pnewdb->lastupdate_devices = now;
            }
        }
        semV( NEWDB_SEMKEY );
        if ( added ) {
            DEBUG_PRINTF( "Adding device %s succeeded\n", mac );
            newLogAdd( NEWLOG_FROM_DATABASE, "Inserted entry in device table" );
            dump( (char *)&pnewdb->devices[index], sizeof( newdb_dev_t ) );
        } else {
            printf( "Error adding device %s\n", mac );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error adding device" );
        }
    } else {
        printf( "Error adding device\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error adding device" );
    }
    return( added );
}

/**
 * \brief Sets the device table row with the same id as the row data
 * \param pdev Pointer to an entry structure that was obtained by a get-function
 * \returns 1 on success, 0 on error
 */
int newDbSetDevice( newdb_dev_t * pdev ) {
    if ( newDbSharedMemory && pdev && ( pdev->id >= 0 && pdev->id < NEWDB_MAX_DEVICES ) ) {
        int now = (int)time( NULL );
        pdev->lastupdate = now;
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        memcpy( &pnewdb->devices[pdev->id], pdev, sizeof( newdb_dev_t ) );
        pnewdb->numwrites++;
        pnewdb->lastupdate_devices = now;
        semV( NEWDB_SEMKEY );
        DEBUG_PRINTF( "Setting device %s succeeded\n", pdev->mac );
        newLogAdd( NEWLOG_FROM_DATABASE, "Updated device table" );
#ifdef DB_DEBUG
        dump( (char *)&pnewdb->devices[pdev->id], sizeof( newdb_dev_t ) );
#endif
        return 1;
    } else {
        printf( "Error setting device\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error setting device" );
    }
    return 0;
}

/**
 * \brief Gets the mac of a device row with key <id>
 * \param id Key of the table row
 * \param mac Pointer to a user array, large enough to hold a (16 bytes+1) mac string
 * \returns mac pointer on success, or NULL on error
 */
char * newDbDeviceGetMac( int id, char * mac ) {
    if ( newDbSharedMemory && mac && ( id >= 0 && id < NEWDB_MAX_DEVICES ) ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        strcpy( mac, pnewdb->devices[id].mac );
        semV( NEWDB_SEMKEY );
        return mac;
    }
    return NULL;
}

/**
 * \brief Empty the entire device table, or a subset
 * \param mode One of MODE_DEV_EMPTY_ALL, MODE_DEV_EMPTY_CLIMATE, MODE_DEV_EMPTY_LAMPS
 * or MODE_DEV_EMPTY_PLUGS
 * \returns 1 on success, 0 on error
 */
int newDbEmptyDevices( int mode ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES; i++ ) {
            int dev = pnewdb->devices[i].dev;
            switch ( mode ) {
                case MODE_DEV_EMPTY_ALL:
                    pnewdb->devices[i].mac[0] = '\0';
                    break;
                case MODE_DEV_EMPTY_CLIMATE:
                    if ( dev >= DEVICE_DEV_MANAGER && dev <= DEVICE_DEV_PUMP ) {
                        pnewdb->devices[i].mac[0] = '\0';
                    }
                    break;
                case MODE_DEV_EMPTY_LAMPS:
                    if ( dev == DEVICE_DEV_LAMP ) {
                        pnewdb->devices[i].mac[0] = '\0';
                    }
                    break;
                case MODE_DEV_EMPTY_PLUGS:
                    if ( dev == DEVICE_DEV_PLUG ) {
                        pnewdb->devices[i].mac[0] = '\0';
                    }
                    break;
            }
            pnewdb->devices[i].id = i;
        }
        pnewdb->numwrites++;
        pnewdb->lastupdate_devices = now;
        semV( NEWDB_SEMKEY );
        newLogAdd( NEWLOG_FROM_DATABASE, "(Partially) Emptied device table" );
        return 1;
    }
    return 0;
}

/**
 * \brief Calulates a checksum over the device table, for all devices or for devices in a certain room
 * \param room 0 for all rooms, or a rooms number (starting at 1)
 * \returns Checksum (0 means an empty table/room)
 */
int newDbDevicesCalcChecksumRoom( int room ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i, len = 0, lastupdate = 0;

        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES; i++ ) {
            if ( pnewdb->devices[i].mac[0] != '\0' ) {
                if ( pnewdb->devices[i].lastupdate > lastupdate ) {
                    lastupdate = pnewdb->devices[i].lastupdate;
                }
                len++;
            }
        }
        semV( NEWDB_SEMKEY );

        int chksum = ( len * 100000 ) + ( lastupdate % 100000 );

        return( chksum );
    }
    return 0;
}

/**
 * \brief Set the FLAG_TOPO_CLEAR flag for all devices
 * \returns 1 on success, 0 on error
 */
int newDbDevicesSetClearFlag( void ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;

        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES; i++ ) {
            if ( pnewdb->devices[i].mac[0] != '\0' ) {
                pnewdb->devices[i].flags |= FLAG_TOPO_CLEAR;
            }
        }
        // Do not count this as a DB-write
        semV( NEWDB_SEMKEY );
        return 1;
    }
    return 0;
}

/**
 * \brief Remove all devices that still have the FLAG_TOPO_CLEAR flags set
 * \returns 1 on success, 0 on error
 */
int newDbDevicesRemoveWithClearFlag( void ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;

        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES; i++ ) {
            if ( pnewdb->devices[i].flags & FLAG_TOPO_CLEAR ) {
                pnewdb->devices[i].mac[0] = '\0';
            }
        }
        semV( NEWDB_SEMKEY );
        return 1;
    }
    return 0;
}

/**
 * \brief Print Joined Zigbee Device
 */
void newDbPrintDevices( void ) {

    int found = 0;

    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        for ( int i=0; i<NEWDB_MAX_ZCB; i++ ) {
            if( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                for ( int j=0; j<NEWDB_MAX_DEVICES && !found; j++ ) {

                    if ( strcmp( pnewdb->zcb[i].mac, pnewdb->devices[j].mac ) == 0 ) {
                        dump( (char *)&pnewdb->devices[j].mac, LEN_MAC_NIBBLE);
                        dump( (char *)&pnewdb->zcb[i].saddr, 2);
                        dump( (char *)&pnewdb->devices[j].ty, LEN_TY);
                        printf("dev id is %d \n", pnewdb->zcb[i].id);
                        found = 1;
                    } else if ( pnewdb->zcb[i].saddr == 0 && pnewdb->zcb[i].type == SIMPLE_DESCR_GATEWAY ) {
                        printf("found Zcb at index %d \n", pnewdb->zcb[i].id);
                        found = 1;
                    }
                }
            }
            found = 0;
        }
        semV( NEWDB_SEMKEY );
    }
}

// ------------------------------------------------------------------
// Plug history
// ------------------------------------------------------------------

#if 0
int newDbPlugHistRemoveOldest( char * mac ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i, oldest = -1, ts = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST; i++ ) {
            if ( !mac || strcmp( pnewdb->plughist[i].mac, mac ) == 0 ) {
                if ( ts > pnewdb->plughist[i].lastupdate ) {
                    ts = pnewdb->plughist[i].lastupdate;
                    oldest = i;
                }
            }
        }
        if ( oldest >= 0 ) {
            pnewdb->plughist[i].mac[0] = '\0';
        }
        semV( NEWDB_SEMKEY );
        return( oldest >= 0 );
    }
    return 0;
}
#endif

/**
 * \brief Returns a personal/writable copy of table row with key <mac> and a lastupdate timestamp
 * which lies in the <min>. When an entry already existed for the <mac>/<min> combination, then that
 * one is returned, otherwise a new row is added. When the table is full, the oldest entry is
 * automatically removed (overwritten)
 * \param mac Key1 to the (new) table row
 * \param min Key2 to the (new) table row
 * \param psys Pointer to a caller's entry structure
 * \returns 1 on success, 0 on error
 */
int newDbGetMatchingOrNewPlugHist( char * mac, int min, newdb_plughist_t * phist ) {
    int index = -1;
    if ( newDbSharedMemory && mac && phist ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i, firstempty = -1, matching = -1, oldest = -1, ts = -1;

        // First scan all samples to find a) first empty slot, b) matching hist, c) oldest slot
        // When we find a matching slot, then we can stop immediately
        semP( NEWDB_SEMKEY );

        for ( i=0; i<NEWDB_MAX_PLUGHIST && ( matching < 0 ); i++ ) {
            if ( pnewdb->plughist[i].mac[0] == '\0' ) {
                // empty: save first one
                if ( firstempty < 0 ) {
                    firstempty = i;
                }
            } else {
                // occupied: save oldest and look for match
                if ( ts < 0 || ts > pnewdb->plughist[i].lastupdate ) {
                    ts = pnewdb->plughist[i].lastupdate;
                    oldest = i;
                }
                if ( ( pnewdb->plughist[i].lastupdate / 60 ) == min ) {
                    // Same minute sample
                    if ( strcmp( pnewdb->plughist[i].mac, mac ) == 0 ) {
                        // Same plug
                        matching = i;
                    }
                }
            }
        }

        // See what we found and act accordingly
        if ( matching >= 0 ) {
            index = matching;
        } else if ( firstempty >= 0 ) {
            index = firstempty;
        } else {
            // No match nor empty slot found: overwrite the oldest sample
            index = oldest;
        }

        if ( index >= 0 ) {
            // Fill the slot with initial data
            int now = (int)time( NULL );
            memset( &pnewdb->plughist[index], 0, sizeof( newdb_plughist_t ) );
            pnewdb->plughist[index].id = index;
            newDbStrNcpy( pnewdb->plughist[index].mac, mac, LEN_MAC_NIBBLE );
            pnewdb->plughist[index].lastupdate = now;
            memcpy( phist, &pnewdb->plughist[index], sizeof( newdb_plughist_t ) );
            pnewdb->numwrites++;
            pnewdb->lastupdate_plughist = now;
        }

        semV( NEWDB_SEMKEY );

        if ( index >= 0 ) {
            DEBUG_PRINTF( "Adding plughist for device %s succeeded: ", mac );
            if ( matching >= 0 ) {
                DEBUG_PRINTF( "re-used mathing slot %d\n", matching );
                newLogAdd( NEWLOG_FROM_DATABASE, "Updated plughist table" );
            } else if ( firstempty >= 0 ) {
                DEBUG_PRINTF( "new slot %d\n", firstempty );
                newLogAdd( NEWLOG_FROM_DATABASE, "Inserted entry in plughist table" );
            } else {
                DEBUG_PRINTF( "Overwrote oldest slot %d\n", oldest );
                newLogAdd( NEWLOG_FROM_DATABASE, "Updated plughist table" );
            }
#ifdef DB_DEBUG
            dump( (char *)&pnewdb->plughist[index], sizeof( newdb_plughist_t ) );
#endif
        } else {
            printf( "Error adding plughist for device %s\n", mac );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error adding plughist" );
        }
    } else {
        printf( "Error adding plughist\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error adding plughist" );
    }
    return( index >= 0 );
}

/**
 * \brief Print table to stdout
 */
void newDbPrintPlugHist( void ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST; i++ ) {
            printf( "%4d : ", i );
            dump( (char *)&pnewdb->plughist[i], sizeof( newdb_plughist_t ) );
        }
        semV( NEWDB_SEMKEY );
    }
}

/**
 * \brief Sets the plughist table row with the same id as the row data
 * \param phist Pointer to an entry structure that was obtained by a get-function
 * \returns 1 on success, 0 on error
 */
int newDbSetPlugHist( newdb_plughist_t * phist ) {
    if ( newDbSharedMemory && phist && ( phist->id >= 0 && phist->id < NEWDB_MAX_PLUGHIST ) ) {
        int now = (int)time( NULL );
        phist->lastupdate = now;
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        memcpy( &pnewdb->plughist[phist->id], phist, sizeof( newdb_plughist_t ) );
        pnewdb->numwrites++;
        pnewdb->lastupdate_plughist = now;
        semV( NEWDB_SEMKEY );
        DEBUG_PRINTF( "Setting plughist for device %s succeeded\n", phist->mac );
        return 1;
    } else {
        printf( "Error setting plughist\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error setting plughist" );
    }
    return 0;
}

/**
 * \brief Gets the number of pluhist entries in the table
 * \returns The number of plughists
 */
int newDbGetNumOfPlughist( void ) {
    int cnt = 0;
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST; i++ ) {
            if ( pnewdb->plughist[i].mac[0] != '\0' ) {
                cnt++;
            }
        }
        semV( NEWDB_SEMKEY );
    }
    return cnt;
}

/**
 * \brief Empty the plughist table
 * \returns 1 on success, 0 on error
 */
int newDbEmptyPlugHist( void ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST; i++ ) {
            pnewdb->plughist[i].id     = i;
            pnewdb->plughist[i].mac[0] = '\0';
        }
        pnewdb->numwrites++;
        pnewdb->lastupdate_plughist = now;
        semV( NEWDB_SEMKEY );
        newLogAdd( NEWLOG_FROM_DATABASE, "Emptied plughist table" );
        return 1;
    }
    return 0;
}

// ------------------------------------------------------------------
// Zcb
// ------------------------------------------------------------------

/**
 * \brief Get a personal/writable copy from the zcb table row with key <mac>
 * \param mac Key to the table row
 * \param pzcb Pointer to a caller's entry structure
 * \returns 1 when found, 0 when not found
 */
int newDbGetZcb( char * mac, newdb_zcb_t * pzcb ) {
    int found = 0;
    if ( newDbSharedMemory && mac && pzcb ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB && !found; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                if ( strcmp( pnewdb->zcb[i].mac, mac ) == 0 ) {
                    memcpy( pzcb, &pnewdb->zcb[i], sizeof( newdb_zcb_t ) );
                    found = 1;
                }
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Found zcb with mac %s\n", mac );
        } else {
            DEBUG_PRINTF( "Zcb %s not found\n", mac );
        }
    } else {
        printf( "Error getting zcb\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error getting zcb" );
    }
    return( found );
}

/**
 * \brief Get a personal/writable copy from the zcb table row with key <saddr>
 * \param saddr Key to the table row
 * \param pzcb Pointer to a caller's entry structure
 * \returns 1 when found, 0 when not found
 */
int newDbGetZcbSaddr( int saddr, newdb_zcb_t * pzcb ) {
    int found = 0;
    if ( newDbSharedMemory && pzcb ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB && !found; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                if ( pnewdb->zcb[i].saddr == saddr ) {
                    memcpy( pzcb, &pnewdb->zcb[i], sizeof( newdb_zcb_t ) );
                    found = 1;
                }
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Found zcb with saddr 0x%04X\n", saddr );
        } else {
            DEBUG_PRINTF( "Zcb %d not found\n", saddr );
        }
    } else {
        printf( "Error getting zcb\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error getting zcb" );
    }
    return( found );
}

/**
 * \brief Adds a new row to the zcb table and returns a personal/writable copy of this
 * table row with key <mac>
 * \param mac Key to the new table row
 * \param pzcb Pointer to a caller's entry structure
 * \returns 1 on success, 0 on error
 */
int newDbGetNewZcb( char * mac, newdb_zcb_t * pzcb ) {
    int added = 0, index = -1;
    if ( newDbSharedMemory && mac && pzcb ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        int oldest = now;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB && !added; i++ ) {
            if ( pnewdb->zcb[i].status == ZCB_STATUS_FREE ) {
                added = 1;
                index = i;
            } else if ( pnewdb->zcb[i].lastupdate < oldest ) {
                oldest = pnewdb->zcb[i].lastupdate;
                index = i;
            }
        }
        if ( index >= 0 ) {
            memset( &pnewdb->zcb[index], 0, sizeof( newdb_zcb_t ) );
            pnewdb->zcb[index].id = index;
            pnewdb->zcb[index].status = ZCB_STATUS_USED;
            newDbStrNcpy( pnewdb->zcb[index].mac, mac, LEN_MAC_NIBBLE );
            pnewdb->zcb[index].lastupdate = now;
            memcpy( pzcb, &pnewdb->zcb[index], sizeof( newdb_zcb_t ) );
            pnewdb->numwrites++;
            pnewdb->lastupdate_zcb = now;
        }
        semV( NEWDB_SEMKEY );

        if ( index >= 0 ) {
            if ( added ) {
                DEBUG_PRINTF( "Adding zcb %s to slot %d succeeded\n", mac, index );
                newLogAdd( NEWLOG_FROM_DATABASE, "Inserted entry in zcb table" );
            } else {
                DEBUG_PRINTF( "Overwrite oldest slot %d to add zcb %s\n", index, mac );
                newLogAdd( NEWLOG_FROM_DATABASE, "Updated zcb table" );
            }
#ifdef DB_DEBUG
            dump( (char *)&pnewdb->zcb[index], sizeof( newdb_zcb_t ) );
#endif
        } else {
            printf( "Error adding zcb %s\n", mac );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error adding zcb" );
        }
    } else {
        printf( "Error adding zcb\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error adding zcb" );
    }
    return( added );
}

/**
 * \brief Sets the zcb table row with the same id as the row data
 * \param pzcb Pointer to an entry structure that was obtained by a get-function
 * \returns 1 on success, 0 on error
 */
int newDbSetZcb( newdb_zcb_t * pzcb ) {
    if ( newDbSharedMemory && pzcb && ( pzcb->id >= 0 && pzcb->id < NEWDB_MAX_ZCB ) ) {
        int now = (int)time( NULL );
        pzcb->lastupdate = now;
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        memcpy( &pnewdb->zcb[pzcb->id], pzcb, sizeof( newdb_zcb_t ) );
        pnewdb->numwrites++;
        pnewdb->lastupdate_zcb = now;
        semV( NEWDB_SEMKEY );
        DEBUG_PRINTF( "Setting zcb %s succeeded\n", pzcb->mac );
        newLogAdd( NEWLOG_FROM_DATABASE, "Updated zcb table" );
        return 1;
    } else {
        printf( "Error setting zcb\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error setting zcb" );
    }
    return 0;
}

int newDbSetZcbMapID( newdb_zcb_t * pzcb, int index)
{
    if ( newDbSharedMemory && pzcb && ( pzcb->id >= 0 && pzcb->id < NEWDB_MAX_ZCB ) ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        pnewdb->zcb[pzcb->id].matterIndex = index;
        semV( NEWDB_SEMKEY );

        return 1;
    }

    return 0;
}

int newDbCheckZcbType( int saddr) {
    int found = 0;
    int type = 0xFFFF;
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB && !found; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                if ( pnewdb->zcb[i].saddr == saddr ) {
                    type = pnewdb->zcb[i].type;
                    found = 1;
                }
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Found zcb 0x%04X type is 0x%04X\n", saddr, type);
        } else {
            DEBUG_PRINTF( "Check Zcb Type failed because Zcb %d not found\n", saddr );
        }
    } else {
        printf( "Error getting zcb\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error getting zcb" );
    }
    return( type );
}

/**
 * \brief Empty the zcb table
 * \returns 1 on success, 0 on error
 */
int newDbEmptyZcb( void ) {
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB; i++ ) {
            pnewdb->zcb[i].id     = i;
            pnewdb->zcb[i].status = ZCB_STATUS_FREE;
        }
        pnewdb->numwrites++;
        pnewdb->lastupdate_zcb = now;
        semV( NEWDB_SEMKEY );
        newLogAdd( NEWLOG_FROM_DATABASE, "Emptied zcb table" );
        return 1;
    }
    return 0;
}

// ------------------------------------------------------------------
// Serialization helpers
// ------------------------------------------------------------------

/**
 * \brief Concatenates string <str> to string <buf>, proceeded by a comma when requested.
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \param str String to concatenate
 * \param proceedComma Boolean indicating that first a comma needs to be added
 * \returns A pointer to the end of the string, or NULL in case of an error
 */
static char * newDbSerializeHelperStr( int MAXBUF, char * buf, char * str, int proceedComma ) {
    if ( buf == NULL || str == NULL ) return NULL;
    int blen = strlen( buf );
    int slen = strlen( str );
    if ( ( blen + slen + 1 ) < MAXBUF ) {
        if ( proceedComma ) strcat( buf, "," );
        strcat( buf, str );
        return( &buf[blen+slen] );  // Address of '\0' character
    } else {
        printf( "Serialization buffer overrun\n" );
    }
    return NULL;
}

/**
 * \brief Concatenates integer <num> to string <buf>, proceeded by a comma when requested.
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \param num Number to concatenate
 * \param proceedComma Boolean indicating that first a comma needs to be added
 * \returns A pointer to the end of the string, or NULL in case of an error
 */
static char * newDbSerializeHelperInt( int MAXBUF, char * buf, int num, int proceedComma ) {
    char intbuf[20];
    sprintf( intbuf, "%d", num );
    return newDbSerializeHelperStr( MAXBUF, buf, intbuf, proceedComma );
}

/**
 * \brief Concatenates a header row to string <buf>
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \param num Number of header strings to concatenate
 * \param headers Array of header strings (of length <num>) to be added
 * \returns A pointer to the end of the string, or NULL in case of an error
 */
static char * newDbSerializeHelperHeader( int MAXBUF, char * buf, int num, char * headers[] ) {
    // Headers
    int i, addComma = 0;
    char * start = buf;
    for ( i=0; i<num && buf != NULL; i++ ) {
        buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, headers[i], addComma );
        addComma = 1;
    }
    return buf;
}

// ------------------------------------------------------------------
// Serialization
// ------------------------------------------------------------------

/**
 * \brief Serialize the system table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializeSystem( int MAXBUF, char * buf ) {
    if ( newDbSharedMemory && buf ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        char * start = buf;
        buf[0] = '\0';

        // Headers
        buf = newDbSerializeHelperHeader( MAXBUF, buf, NUM_COLUMNS_SYSTEM, newdb_system_columns );

        // Data
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_SYSTEM && buf != NULL; i++ ) {
            if ( pnewdb->system[i].name[0] != '\0' ) {
                strcat( buf, ";" );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->system[i].id, 0 );
                buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->system[i].name, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->system[i].intval, 1 );
                buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->system[i].strval, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->system[i].lastupdate, 1 );
            }
        }
        semV( NEWDB_SEMKEY );

        if ( buf ) return( start );
    }
    return( NULL );
}

/**
 * \brief Serialize the device table from device type <dev1> to <dev2> (and including)
 * \param dev1 From (and including) device type. When 0 (= DEVICE_DEV_UNKNOWN), then all
 * devices are serialized
 * \param dev2 To (and including) device type
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
static char * newDbSerializeDevsDev( int dev1, int dev2, int MAXBUF, char * buf ) {
    if ( newDbSharedMemory && buf ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        char * start = buf;
        buf[0] = '\0';

        // Headers
        buf = newDbSerializeHelperHeader( MAXBUF, buf, sizeof(newdb_devs_columns)/sizeof(char*), newdb_devs_columns );

        // Data
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES && buf != NULL; i++ ) {
            if ( !dev1 || ( pnewdb->devices[i].dev >= dev1 && pnewdb->devices[i].dev <= dev2 ) ) {
                if ( pnewdb->devices[i].mac[0] != '\0' ) {
                    strcat( buf, ";" );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].id, 0 );
                    buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].mac, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].dev, 1 );
                    buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].ty, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].par, 1 );
                    buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].nm, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].heat, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].cool, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].tmp, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].hum, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].prs, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].co2, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].bat, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].batl, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].als, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].xloc, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].yloc, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].zloc, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].sid, 1 );
                    buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].cmd, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].lvl, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].rgb, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].kelvin, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].act, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].sum, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].flags, 1 );
                    buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->devices[i].lastupdate, 1 );
                }
            }
        }
        semV( NEWDB_SEMKEY );

        if ( buf ) return( start );
    }
    return( NULL );
}

/**
 * \brief Serialize the complete device table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializeDevs( int MAXBUF, char * buf ) {
    return newDbSerializeDevsDev( DEVICE_DEV_UNKNOWN, DEVICE_DEV_UNKNOWN, MAXBUF, buf );
}

/**
 * \brief Serialize the subset of plugs in the device table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializePlugs( int MAXBUF, char * buf ) {
    return newDbSerializeDevsDev( DEVICE_DEV_PLUG, DEVICE_DEV_PLUG, MAXBUF, buf );
}

/**
 * \brief Serialize the subset of lamps in the device table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializeLamps( int MAXBUF, char * buf ) {
    return newDbSerializeDevsDev( DEVICE_DEV_LAMP, DEVICE_DEV_LAMP, MAXBUF, buf );
}

/**
 * \brief Serialize the subset of lamps and plugs in the device table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializeLampsAndPlugs( int MAXBUF, char * buf ) {
    return newDbSerializeDevsDev( DEVICE_DEV_LAMP, DEVICE_DEV_PLUG, MAXBUF, buf );
}

/**
 * \brief Serialize the subset of climate devices in the device table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializeClimate( int MAXBUF, char * buf ) {
    return newDbSerializeDevsDev( DEVICE_DEV_MANAGER, DEVICE_DEV_PUMP, MAXBUF, buf );
}

/**
 * \brief Serialize the plughistory table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializePlugHist( int MAXBUF, char * buf ) {
    if ( newDbSharedMemory && buf ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        char * start = buf;
        buf[0] = '\0';

        // Headers
        buf = newDbSerializeHelperHeader( MAXBUF, buf, NUM_COLUMNS_PLUGHIST, newdb_plughist_columns );

        // Data
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST && buf != NULL; i++ ) {
            if ( pnewdb->plughist[i].mac[0] != '\0' ) {
                strcat( buf, ";" );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->plughist[i].id, 0 );
                buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->plughist[i].mac, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->plughist[i].sum, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->plughist[i].lastupdate, 1 );
            }
        }
        semV( NEWDB_SEMKEY );

        if ( buf ) return( start );
    }
    return( NULL );
}

/**
 * \brief Serialize the zcb table
 * \param MAXBUF Maximum length of the serialized string
 * \param buf User allocated area to store the serialization result
 * \returns The buf pointer, or NULL in case of an error
 */
char * newDbSerializeZcb( int MAXBUF, char * buf ) {
    if ( newDbSharedMemory && buf ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        char * start = buf;
        buf[0] = '\0';

        // Headers
        buf = newDbSerializeHelperHeader( MAXBUF, buf, NUM_COLUMNS_ZCB, newdb_zcb_columns );

        // Data
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB && buf != NULL; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                strcat( buf, ";" );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->zcb[i].id, 0 );
                buf = newDbSerializeHelperStr( MAXBUF - (int)( buf-start ), buf, pnewdb->zcb[i].mac, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->zcb[i].status, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->zcb[i].saddr, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->zcb[i].type, 1 );
                buf = newDbSerializeHelperInt( MAXBUF - (int)( buf-start ), buf, pnewdb->zcb[i].lastupdate, 1 );
            }
        }
        semV( NEWDB_SEMKEY );

        if ( buf ) return( start );
    }
    return( NULL );
}

// ------------------------------------------------------------------
// Status
// ------------------------------------------------------------------

/**
 * \brief Get last update of the system table
 * \returns Last update timestamp
 */
int newDbGetLastupdateSystem( void ) {
    int lastupdate = 0, sum = 0;
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_SYSTEM; i++ ) {
            if ( pnewdb->system[i].name[0] != '\0' ) {
                if ( pnewdb->system[i].lastupdate > lastupdate ) {
                    lastupdate = pnewdb->system[i].lastupdate;
                }
                sum++;
            }
        }
        semV( NEWDB_SEMKEY );
        lastupdate += sum;
    }
    return lastupdate;
}

/**
 * \brief Get last update of the entire device table or a subset
 * \param dev1 From (and including) device type. When 0 (= DEVICE_DEV_UNKNOWN), then all
 * devices are serialized
 * \param dev2 To (and including) device type
 * \param dev1
 * \returns Last update timestamp
 */
static int newDbGetLastupdateDevicesDev( int dev1, int dev2 ) {
    int lastupdate = 0, sum = 0;
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES; i++ ) {
            if ( ( pnewdb->devices[i].mac[0] != '\0' ) &&
                 ( !dev1 || ( pnewdb->devices[i].dev >= dev1 && pnewdb->devices[i].dev <= dev2 ) ) ) {
                if ( pnewdb->devices[i].lastupdate > lastupdate ) {
                    lastupdate = pnewdb->devices[i].lastupdate;
                }
                sum++;
            }
        }
        semV( NEWDB_SEMKEY );
        lastupdate += sum;
    }
    return lastupdate;
}

/**
 * \brief Get last update of the device table
 * \returns Last update timestamp
 */
int newDbGetLastupdateDevices( void ) {
    return newDbGetLastupdateDevicesDev( DEVICE_DEV_UNKNOWN, DEVICE_DEV_UNKNOWN );
}

/**
 * \brief Get last update of the plugs in the device table
 * \returns Last update timestamp
 */
int newDbGetLastupdatePlug( void ) {
    return newDbGetLastupdateDevicesDev( DEVICE_DEV_PLUG, DEVICE_DEV_PLUG );
}

/**
 * \brief Get last update of the lamps in the device table
 * \returns Last update timestamp
 */
int newDbGetLastupdateLamp( void ) {
    return newDbGetLastupdateDevicesDev( DEVICE_DEV_LAMP, DEVICE_DEV_LAMP );
}

/**
 * \brief Get last update of the climate devices in the device table
 * \returns Last update timestamp
 */
int newDbGetLastupdateClimate( void ) {
    return newDbGetLastupdateDevicesDev( DEVICE_DEV_MANAGER, DEVICE_DEV_PUMP );
}

/**
 * \brief Get last update of the zcb table
 * \returns Last update timestamp
 */
int newDbGetLastupdateZcb( void ) {
    int lastupdate = 0, sum = 0;
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                if ( pnewdb->zcb[i].lastupdate > lastupdate ) {
                    lastupdate = pnewdb->zcb[i].lastupdate;
                }
                sum++;
            }
        }
        semV( NEWDB_SEMKEY );
        lastupdate += sum;
    }
    return lastupdate;
}

/**
 * \brief Get last update of the plughist table
 * \returns Last update timestamp
 */
int newDbGetLastupdatePlugHist( void ) {
    int lastupdate = 0, sum = 0;
    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST; i++ ) {
            if ( pnewdb->plughist[i].mac[0] != '\0' ) {
                if ( pnewdb->plughist[i].lastupdate > lastupdate ) {
                    lastupdate = pnewdb->plughist[i].lastupdate;
                }
                sum++;
            }
        }
        semV( NEWDB_SEMKEY );
        lastupdate += sum;
    }
    return lastupdate;
}

// ------------------------------------------------------------------
// Clear
// ------------------------------------------------------------------

/**
 * \brief Delete one entry in the plughist table
 * \param id Key in the row
 * \returns 1 on success, 0 on error
 */
int newDbDeletePlugHist( int id ) {
    if ( newDbSharedMemory && id >= 0 && id < NEWDB_MAX_PLUGHIST ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        pnewdb->plughist[id].mac[0] = '\0';
        pnewdb->numwrites++;
        pnewdb->lastupdate_plughist = now;
        semV( NEWDB_SEMKEY );
        newLogAdd( NEWLOG_FROM_DATABASE, "Deleted entry from plughist table" );
        return 1;
    }
    return 0;
}

/**
 * \brief Delete one entry in the system table
 * \param name Key in the row
 * \returns 1 on success, 0 on error
 */
int newDbDeleteSystem( char * name ) {
    int found = 0;
    if ( newDbSharedMemory && name ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_SYSTEM && !found; i++ ) {
            if ( strcmp( pnewdb->system[i].name, name ) == 0 ) {
                pnewdb->system[i].name[0] = '\0';
                pnewdb->numwrites++;
                pnewdb->lastupdate_sys = now;
                found = 1;
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Deleting system %s succeeded\n", name );
            newLogAdd( NEWLOG_FROM_DATABASE, "Deleted entry from system table" );
            return 1;
        } else {
            printf( "Error deleting system %s\n", name );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error deleting system" );
        }
    } else {
        printf( "Error deleting system\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error deleting system" );
    }
    return 0;
}

/**
 * \brief Delete one entry in the device table
 * \param mac Key in the row
 * \returns 1 on success, 0 on error
 */
int newDbDeleteDevice( char * mac ) {
    int found = 0;
    if ( newDbSharedMemory && mac ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i;
        int now = (int)time( NULL );
        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES && !found; i++ ) {
            if ( strcmp( pnewdb->devices[i].mac, mac ) == 0 ) {
                pnewdb->devices[i].mac[0] = '\0';
                pnewdb->numwrites++;
                pnewdb->lastupdate_devices = now;
                found = 1;
            }
        }
        semV( NEWDB_SEMKEY );
        if ( found ) {
            DEBUG_PRINTF( "Deleting device %s succeeded\n", mac );
            newLogAdd( NEWLOG_FROM_DATABASE, "Deleted entry from device table" );
            return 1;
        } else {
            printf( "Error deleting device %s\n", mac );
            newLogAdd( NEWLOG_FROM_DATABASE, "Error deleting device" );
        }
    } else {
        printf( "Error deleting device\n" );
        newLogAdd( NEWLOG_FROM_DATABASE, "Error deleting device" );
    }
    return 0;
}

// ------------------------------------------------------------------
// Loopers
// ------------------------------------------------------------------

/*
int newDbLoopDevices( deviceCb_t deviceCb ) {
    if ( newDbSharedMemory && deviceCb ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i, ok = 1;

        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_DEVICES && ok; i++ ) {
            ok = deviceCb( &pnewdb->devices[i] );
        }
        semV( NEWDB_SEMKEY );

        return( ok );
    }
    return 0;
}
*/


/**
 * \brief Loop through the plughist table and call call-back with each row from the plug meter
 * with mac <mac>. Note that the call-back may not alter the table row as it peeks right into
 * the database
 * \param mac Mac of the plug meter
 * \param plugHistCb Call-back function
 * \returns 1 on success, 0 on error
 */
int newDbLoopPlugHist( char * mac, plughistCb_t plugHistCb ) {
    if ( newDbSharedMemory && mac && plugHistCb ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i, ok = 1;

        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_PLUGHIST && ok; i++ ) {
            if ( strcmp( pnewdb->plughist[i].mac, mac ) == 0 ) {
                ok = plugHistCb( &pnewdb->plughist[i] );
            }
        }
        semV( NEWDB_SEMKEY );

        return( ok );
    }
    return 0;
}

/**
 * \brief Loop through the zcb table and call call-back with each row. Note that the
 * call-back may not alter the table row as it peeks right into the database
 * \param zcbCb Call-back function
 * \returns 1 on success, 0 on error
 */
int newDbLoopZcb( zcbCb_t zcbCb ) {
    if ( newDbSharedMemory && zcbCb ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        int i, ok = 1;

        semP( NEWDB_SEMKEY );
        for ( i=0; i<NEWDB_MAX_ZCB && ok; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE ) {
                ok = zcbCb( &pnewdb->zcb[i] );
            }
        }
        semV( NEWDB_SEMKEY );

        return( ok );
    }
    return 0;
}

int newDbGetZigbeeDeviceList(ZigbeeDev_t** zbList, int maxDevices, int *nodeNum) {

    int idx = 0;
    *nodeNum = 0;

    newdb_dev_t device;

    if ( newDbSharedMemory ) {
        newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
        semP( NEWDB_SEMKEY );
        for ( int i=0; i<maxDevices; i++ ) {
            if ( pnewdb->zcb[i].status != ZCB_STATUS_FREE && newDbGetDevice( pnewdb->zcb[i].mac, &device ) ) {
                ZigbeeDev_t *zigbee = (ZigbeeDev_t*)malloc(sizeof(ZigbeeDev_t));
                memcpy( &zigbee->zcb, &pnewdb->zcb[i], sizeof( newdb_zcb_t ) );
                memcpy( &zigbee->dev, &device, sizeof( newdb_dev_t ) );
                zbList[idx] = zigbee;
                idx++;
                (*nodeNum)++;
            }
        }
        semV( NEWDB_SEMKEY );
    }

    return 1;
}

int newDbfreeZigbeeDeviceList(ZigbeeDev_t** zbList, int nodeNum)
{
    for ( int i=0; i<nodeNum; i++ ) {
        free(zbList[i]);
    }

    return 1;
}

void newDbSetNwkState( uint8_t state )
{
    newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
    semP( NEWDB_SEMKEY );

    pnewdb->network_state = (int)state;

    semV( NEWDB_SEMKEY );
}

void newDbGetNwkState( int* state )
{
    newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
    semP( NEWDB_SEMKEY );

    *state = pnewdb->network_state;

    semV( NEWDB_SEMKEY );
}

void newDbReset(void)
{
    newdb_t * pnewdb = (newdb_t *)newDbSharedMemory;
    semP( NEWDB_SEMKEY );
    memset( pnewdb, 0, sizeof( newdb_t ) );
    semV( NEWDB_SEMKEY );
}