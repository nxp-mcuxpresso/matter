// ------------------------------------------------------------------
// CMD - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------
#ifndef  CMD_H_INCLUDED
#define  CMD_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

int  eGetPermitJoining( void );
void cmdInit( void );
int  cmdHandle( void );


/* control command */
int eReqNwkState(void);
int ePermitJoinOn(uint8_t u8Interval);
int eStartNetwork(void);
int eStartGateway(void);
int eFactoryNew(void);
int eResetZcb(void);
int eCheckZcb(void);

// teZcbStatus cmdResetBridge();


#if defined __cplusplus
}
#endif

#endif  /* CMD_H_INCLUDED */