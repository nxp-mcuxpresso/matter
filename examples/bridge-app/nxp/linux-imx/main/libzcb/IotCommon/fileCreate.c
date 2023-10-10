// ------------------------------------------------------------------
// File create
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief File create functions
 */
#include "fileCreate.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef TARGET_RASPBIAN
#include <pwd.h>
#include <grp.h>
#endif

// #define FC_DEBUG

#ifdef FC_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* FC_DEBUG */

// ------------------------------------------------------------------
// Create file
// ------------------------------------------------------------------

/**
 * \brief Create file with certain mode
 * \param name Name of the file to create
 * \param mode Mode of the file
 * \returns 1 on success, or 0 in case of an error
 */
int fileCreateMode( char * name, int mode ) {
    DEBUG_PRINTF( "Create file %s with mode %o\n", name, mode );
    int fd = open( name, O_RDWR | O_CREAT, 0777 );
    if ( fd >= 0 ) {
        fchmod( fd, mode );
        close( fd );
#ifdef TARGET_RASPBIAN
        uid_t uid=getuid(), euid=geteuid();
        DEBUG_PRINTF( "I am PI, uid=%d, euid=%d\n", uid, euid );
        if ( uid == 0 ) {
            struct passwd *pwd = getpwnam("pi");   /* Try getting UID for username */
            struct group  *grp = getgrnam("pi");
            if ( (pwd != NULL) && (pwd != NULL) ) {
                chown( name, pwd->pw_uid, grp->gr_gid );
            } else {
                printf( "Changing ownership to 'pi' failed\n" );
            }
        }
#endif
        return( 1 );
    }
    return( 0 );
}

/**
 * \brief Create read-write file
 * \param name Name of the file to create
 * \returns 1 on success, or 0 in case of an error
 */
int fileCreateRW( char * name ) {
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    return( fileCreateMode( name, mode ) );
}

/**
 * \brief Create read-write-exec file
 * \param name Name of the file to create
 * \returns 1 on success, or 0 in case of an error
 */
int fileCreateRWX( char * name ) {
    int mode = S_IRWXU | S_IRWXG | S_IRWXO;
    return( fileCreateMode( name, mode ) );
}

// ------------------------------------------------------------------
// Test for file existance
// ------------------------------------------------------------------

/**
 * \brief Test for file existance
 * \param name Name of the file to test
 * \returns 1 when exists, or 0 when not
 */
int fileTest( char * name ) {
    int fd = open( name, O_RDWR );
    if ( fd >= 0 ) {
        close( fd );
        return( 1 );
    }
    return( 0 );
}
