// ------------------------------------------------------------------
// Parsing - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

#if defined __cplusplus
extern "C" {
#endif

#define MAXSTRINGATTRS     25
#define MAXINTATTRS        30

#ifndef MAXSTRINGVALUE
#define MAXSTRINGVALUE     40
#endif

typedef struct intattr {
    char * name;
    int  hash;
    int  value;
} t_intattr;

typedef struct stringattr {
    char * name;
    int  hash;
    int  maxlen;
    char value[MAXSTRINGVALUE+2];
} t_stringattr;

extern int numIntAttrs;
extern int numStringAttrs;

extern t_intattr    parsingIntAttrs[MAXINTATTRS];
extern t_stringattr parsingStringAttrs[MAXSTRINGATTRS];

void emptyString( char * string );
int isEmptyString( char * string );

int parsingAddIntAttr( char * name );
int parsingAddStringAttr( char * name, int maxlen );

void parsingReset( void );

int parsingIntAttr( char * name, int value );
int parsingStringAttr( char * name, char * value );

int parsingGetIntAttr( char * name );
char * parsingGetStringAttr( char * name );
char * parsingGetStringAttr0( char * name );


#if defined __cplusplus
}
#endif