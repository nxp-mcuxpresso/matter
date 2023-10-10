// ------------------------------------------------------------------
// System table helpers
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------

#ifndef __SYSTEMTABLE_H__
#define __SYSTEMTABLE_H__

#if defined __cplusplus
extern "C" {
#endif

int newDbSystemGetInt( char * name );
char * newDbSystemGetString( char * name, char * str );

int newDbSystemSaveIntval( char * name, int intval );
int newDbSystemSaveStringval( char * name, char * stringval );
int newDbSystemSaveVal( char * name, int intval, char * stringval );

#if defined __cplusplus
}
#endif

#endif  /* __SYSTEMTABLE_H__ */