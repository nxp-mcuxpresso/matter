// ------------------------------------------------------------------
// New Log - Based on shared memory - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief New Log - Based on shared memory
 */

#if defined __cplusplus
extern "C" {
#endif

#define NEWLOG_MAX_TEXT       98
#define MAX_LOG_BUFFER        200

typedef enum {
    NEWLOG_FROM_NONE = 0,
    NEWLOG_FROM_CONTROL_INTERFACE,   // 1
    NEWLOG_FROM_DATABASE,            // 2
    NEWLOG_FROM_NFC_DAEMON,          // 3
    NEWLOG_FROM_SECURE_JOINER,       // 4
    NEWLOG_FROM_ZCB_IN,              // 5
    NEWLOG_FROM_ZCB_OUT,             // 6
    NEWLOG_FROM_CGI,                 // 7
    NEWLOG_FROM_DBP,                 // 8
    NEWLOG_FROM_GW_DISCOVERY,        // 9
    NEWLOG_FROM_TEST,                // 10    See logNames in .c file and in logs.js
} newLogFrom;

typedef struct onelog {
    int from;
    int ts;
    char text[NEWLOG_MAX_TEXT+2];    // Extra  bytes e.g. for \0
} onelog_t;

typedef int (*newlogCb_t)( int i, onelog_t * l );


extern char * logNames[11];
extern char logbuffer[MAX_LOG_BUFFER];

int newLogAdd( newLogFrom from, char * text );
int newLogEmpty( void );
int newLogGetIndex( void );
int newLogLoop( int from, int startIndex, newlogCb_t cb );

int filelog( char * filename, char * text );

#if defined __cplusplus
}
#endif
