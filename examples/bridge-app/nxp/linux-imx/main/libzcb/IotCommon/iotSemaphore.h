// ------------------------------------------------------------------
// IOT Semaphore - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

#if defined __cplusplus
extern "C" {
#endif

void semP( int key );
int  semPtimeout( int key, int secs );
void semPautounlock( int key, int secs );
void semV( int key );
void semClear( int key );
int  semInit( int startval, int key );

#if defined __cplusplus
}
#endif