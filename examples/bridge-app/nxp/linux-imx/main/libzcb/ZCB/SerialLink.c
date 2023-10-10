/****************************************************************************
*
* MODULE:             SerialLink
*
* COMPONENT:          $RCSfile: SerialLink.c,v $
*
* REVISION:           $Revision: 43420 $
*
* DATED:              $Date: 2012-06-18 15:13:17 +0100 (Mon, 18 Jun 2012) $
*
* AUTHOR:             Lee Mitchell
*
****************************************************************************
*
* This software is owned by NXP B.V. and/or its supplier and is protected
* under applicable copyright laws. All rights are reserved. We grant You,
* and any third parties, a license to use this software solely and
* exclusively on NXP products [NXP Microcontrollers such as JN5148, JN5142, JN5139].
* You, and any third parties must reproduce the copyright and warranty notice
* and any other legend of ownership on each copy or partial copy of the
* software.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.

* Copyright NXP B.V. 2012. All rights reserved
*
***************************************************************************/

/** \addtogroup zb
 * \file
 * \brief Serial Link layer
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "SerialLink.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#include "Serial.h"
#include "Utils.h"
#include "iotSleep.h"
#include "dump.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define DBG_SERIALLINK 0
#define DBG_SERIALLINK_CB 0
#define DBG_SERIALLINK_COMMS 0
#define DBG_SERIALLINK_QUEUE 0

#define SL_START_CHAR   0x01
#define SL_ESC_CHAR     0x02
#define SL_END_CHAR     0x03

#define SL_MAX_MESSAGE_LENGTH 256

#define SL_MAX_MESSAGE_QUEUES 3

#define SL_MAX_CALLBACK_QUEUES 3

#if DEBUG_SERIALLINK
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* DEBUG_SERIALLINK */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    FALSE,
    TRUE,
} bool;

typedef enum
{
    E_STATE_RX_WAIT_START,
    E_STATE_RX_WAIT_TYPEMSB,
    E_STATE_RX_WAIT_TYPELSB,
    E_STATE_RX_WAIT_LENMSB,
    E_STATE_RX_WAIT_LENLSB,
    E_STATE_RX_WAIT_CRC,
    E_STATE_RX_WAIT_DATA,
} teSL_RxState;


/** Forward definition of callback function entry */
struct _tsSL_CallbackEntry;

/** Linked list structure for a callback function entry */
typedef struct _tsSL_CallbackEntry
{
    uint16_t                u16Type;        /**< Message type for this callback */
    tprSL_MessageCallback   prCallback;     /**< User supplied callback function for this message type */
    void                    *pvUser;        /**< User supplied data for the callback function */
    struct _tsSL_CallbackEntry *psNext;     /**< Pointer to next in linked list */
} tsSL_CallbackEntry;


/** Structure used to contain a message */
typedef struct
{
    uint16_t u16Type;
    uint16_t u16Length;
    uint8_t  au8Message[SL_MAX_MESSAGE_LENGTH];
} tsSL_Message;


/** Structure of data for the serial link */
typedef struct
{
    int     iSerialFd;

#ifndef WIN32
    pthread_mutex_t         mutex;
#endif /* WIN32 */

    struct
    {
#ifndef WIN32
        pthread_mutex_t         mutex;
#endif /* WIN32 */
        tsSL_CallbackEntry      *psListHead;
    } sCallbacks;

    tsUtilsQueue sCallbackQueue;
    tsUtilsThread sCallbackThread;

    // Array of listeners for messages
    // eSL_MessageWait uses this array to wait on incoming messages.
    struct
    {
        uint16_t u16Type;
        uint16_t u16Length;
        uint8_t *pu8Message;
#ifndef WIN32
        pthread_mutex_t mutex;
        pthread_cond_t cond_data_available;
#else

#endif /* WIN32 */
    } asReaderMessageQueue[SL_MAX_MESSAGE_QUEUES];


    tsUtilsThread sSerialReader;
} tsSerialLink;


/** Structure allocated and passed to callback handler thread */
typedef struct
{
    tsSL_Message            sMessage;       /** The received message */
    tprSL_MessageCallback   prCallback;     /**< User supplied callback function for this message type */
    void *                  pvUser;         /**< User supplied data for the callback function */
} tsCallbackThreadData;




/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

static uint8_t u8SL_CalculateCRC(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data);

static int iSL_TxByte(bool bSpecialCharacter, uint8_t u8Data);

static bool bSL_RxByte(uint8_t *pu8Data);

static teSL_Status eSL_WriteMessage(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data);
static teSL_Status eSL_ReadMessage(uint16_t *pu16Type, uint16_t *pu16Length, uint16_t u16MaxLength, uint8_t *pu8Message);

static void *pvReaderThread(tsUtilsThread *psThreadInfo);

static void *pvCallbackHandlerThread(tsUtilsThread *psThreadInfo);


/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern int verbosity;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

static tsSerialLink sSerialLink;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


teSL_Status eSL_Init(char *cpSerialDevice, uint32_t u32BaudRate)
{
    int i;

    if (eSerial_Init(cpSerialDevice, u32BaudRate, &sSerialLink.iSerialFd) != E_SERIAL_OK)
    {
        return E_SL_ERROR_SERIAL;
    }

    /* Initialise serial link mutex */
    pthread_mutex_init(&sSerialLink.mutex, NULL);

    /* Initialise message callbacks */
    pthread_mutex_init(&sSerialLink.sCallbacks.mutex, NULL);
    sSerialLink.sCallbacks.psListHead = NULL;

    /* Initialise message wait queue */
    for (i = 0; i < SL_MAX_MESSAGE_QUEUES; i++)
    {
        pthread_mutex_init(&sSerialLink.asReaderMessageQueue[i].mutex, NULL);
        pthread_cond_init(&sSerialLink.asReaderMessageQueue[i].cond_data_available, NULL);
        sSerialLink.asReaderMessageQueue[i].u16Type = 0;
    }

    /* Initialise callback queue */
    if (eUtils_QueueCreate(&sSerialLink.sCallbackQueue, SL_MAX_CALLBACK_QUEUES, 0) != E_UTILS_OK)
    {
        DEBUG_PRINTF( "Error creating callback queue\n");
        return E_SL_ERROR;
    }

    /* Start the callback handler thread */
    sSerialLink.sCallbackThread.pvThreadData = &sSerialLink;
    if (eUtils_ThreadStart(pvCallbackHandlerThread, &sSerialLink.sCallbackThread, E_THREAD_JOINABLE) != E_UTILS_OK)
    {
        DEBUG_PRINTF( "Failed to start callback handler thread");
        return E_SL_ERROR;
    }

    /* Start the serial reader thread */
    sSerialLink.sSerialReader.pvThreadData = &sSerialLink;
    if (eUtils_ThreadStart(pvReaderThread, &sSerialLink.sSerialReader, E_THREAD_JOINABLE) != E_UTILS_OK)
    {
        DEBUG_PRINTF( "Failed to start serial reader thread");
        return E_SL_ERROR;
    }

    return E_SL_OK;
}


teSL_Status eSL_Destroy(void)
{
    eUtils_ThreadStop(&sSerialLink.sSerialReader);

    while (sSerialLink.sCallbacks.psListHead)
    {
        eSL_RemoveListener(sSerialLink.sCallbacks.psListHead->u16Type, sSerialLink.sCallbacks.psListHead->prCallback);
    }

    return E_SL_OK;
}


teSL_Status eSL_SendMessage(uint16_t u16Type, uint16_t u16Length, void *pvMessage, uint8_t *pu8SequenceNo)
{
    teSL_Status eStatus;

    /* Make sure there is only one thread sending messages to the node at a time. */
    pthread_mutex_lock(&sSerialLink.mutex);

// printf( "a: 0x%04x - %d\n", u16Type, u16Length );
// dump( (char *)pvMessage, u16Length );
    eStatus = eSL_WriteMessage(u16Type, u16Length, (uint8_t *)pvMessage);

    if (eStatus == E_SL_OK)
    {
// printf( "b\n" );
        /* Command sent successfully */

        uint16_t    u16Length;
        tsSL_Msg_Status sStatus;
        tsSL_Msg_Status *psStatus = &sStatus;

        sStatus.u16MessageType = u16Type;

        /* Expect a status response within 500ms */
        eStatus = eSL_MessageWait(E_SL_MSG_STATUS, 500, &u16Length, (void**)&psStatus);
// printf( "c\n" );

        if (eStatus == E_SL_OK)
        {
// printf( "d\n" );
            DBG_vPrintf(DBG_SERIALLINK, "Status: %d, Sequence %d\n", psStatus->eStatus, psStatus->u8SequenceNo);
            eStatus = psStatus->eStatus;
            if (eStatus == E_SL_OK)
            {
                if (pu8SequenceNo)
                {
                    *pu8SequenceNo = psStatus->u8SequenceNo;
                }
            }
            free(psStatus);
        } else {
            printf("*** eSL_SendMessage : error 0x%2x\n", eStatus);
        }
    }
    pthread_mutex_unlock(&sSerialLink.mutex);
// printf( "f\n" );
    return eStatus;
}


teSL_Status eSL_SendMessageNoWait(uint16_t u16Type, uint16_t u16Length, void *pvMessage, uint8_t *pu8SequenceNo)
{
    teSL_Status eStatus;

    /* Make sure there is only one thread sending messages to the node at a time. */
    pthread_mutex_lock(&sSerialLink.mutex);

//printf( "a: 0x%02x - %d\n", u16Type, u16Length );
//dump( (char *)pvMessage, u16Length );
    eStatus = eSL_WriteMessage(u16Type, u16Length, (uint8_t *)pvMessage);

    pthread_mutex_unlock(&sSerialLink.mutex);
// printf( "b\n" );
    return eStatus;
}


teSL_Status eSL_MessageWait(uint16_t u16Type, uint32_t u32WaitTimeout, uint16_t *pu16Length, void **ppvMessage)
{
    int i;
    tsSerialLink *psSerialLink = &sSerialLink;

    for (i = 0; i < SL_MAX_MESSAGE_QUEUES; i++)
    {
        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Locking queue %d mutex\n", i);
        pthread_mutex_lock(&psSerialLink->asReaderMessageQueue[i].mutex);
        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Acquired queue %d mutex\n", i);

        if (psSerialLink->asReaderMessageQueue[i].u16Type == 0)
        {
            struct timeval sNow;
            struct timespec sTimeout;

            DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Found free slot %d to wait for message 0x%04X\n", i, u16Type);

            psSerialLink->asReaderMessageQueue[i].u16Type = u16Type;

            if (u16Type == E_SL_MSG_STATUS)
            {
                psSerialLink->asReaderMessageQueue[i].pu8Message = *ppvMessage;
            }

            memset(&sNow, 0, sizeof(struct timeval));
            gettimeofday(&sNow, NULL);
            sTimeout.tv_sec = sNow.tv_sec + (u32WaitTimeout/1000);
            sTimeout.tv_nsec = (sNow.tv_usec + ((u32WaitTimeout % 1000) * 1000)) * 1000;
            if (sTimeout.tv_nsec > 1000000000)
            {
                sTimeout.tv_sec++;
                sTimeout.tv_nsec -= 1000000000;
            }
            DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Time now    %lu s, %lu ns\n", sNow.tv_sec, sNow.tv_usec * 1000);
            DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Wait until  %lu s, %lu ns\n", sTimeout.tv_sec, sTimeout.tv_nsec);

            switch (pthread_cond_timedwait(&psSerialLink->asReaderMessageQueue[i].cond_data_available, &psSerialLink->asReaderMessageQueue[i].mutex, &sTimeout))
            {
                case (0):
                    DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Got message type 0x%04x, length %d\n",
                                psSerialLink->asReaderMessageQueue[i].u16Type,
                                psSerialLink->asReaderMessageQueue[i].u16Length);
                    *pu16Length = psSerialLink->asReaderMessageQueue[i].u16Length;
                    *ppvMessage = psSerialLink->asReaderMessageQueue[i].pu8Message;

                    /* Reset queue for next user */
                    psSerialLink->asReaderMessageQueue[i].u16Type = 0;
                    pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
                    return E_SL_OK;

                case (ETIMEDOUT):
                    DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Timed out\n");
                    /* Reset queue for next user */
                    psSerialLink->asReaderMessageQueue[i].u16Type = 0;
                    pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
                    return E_SL_NOMESSAGE;
                    break;

                default:
                    /* Reset queue for next user */
                    psSerialLink->asReaderMessageQueue[i].u16Type = 0;
                    pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
                    return E_SL_ERROR;
            }
        }
        else
        {
            pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
        }
    }
    DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Error, no free queue slots\n");
    return E_SL_ERROR;
}


teSL_Status eSL_AddListener(uint16_t u16Type, tprSL_MessageCallback prCallback, void *pvUser)
{
    tsSL_CallbackEntry *psCurrentEntry;
    tsSL_CallbackEntry *psNewEntry;

    DBG_vPrintf(DBG_SERIALLINK_CB, "Register handler %p for message type 0x%04x\n", prCallback, u16Type);

    psNewEntry = malloc(sizeof(tsSL_CallbackEntry));
    if (!psNewEntry)
    {
        return E_SL_ERROR_NOMEM;
    }

    psNewEntry->u16Type     = u16Type;
    psNewEntry->prCallback  = prCallback;
    psNewEntry->pvUser      = pvUser;
    psNewEntry->psNext      = NULL;

    pthread_mutex_lock(&sSerialLink.sCallbacks.mutex);
    if (sSerialLink.sCallbacks.psListHead == NULL)
    {
        /* Insert at start of list */
        sSerialLink.sCallbacks.psListHead = psNewEntry;
    }
    else
    {
        /* Insert at end of list */
        psCurrentEntry = sSerialLink.sCallbacks.psListHead;
        while (psCurrentEntry->psNext)
        {
            psCurrentEntry = psCurrentEntry->psNext;
        }

        psCurrentEntry->psNext = psNewEntry;
    }
    pthread_mutex_unlock(&sSerialLink.sCallbacks.mutex);
    return E_SL_OK;
}


teSL_Status eSL_RemoveListener(uint16_t u16Type, tprSL_MessageCallback prCallback)
{
    tsSL_CallbackEntry *psCurrentEntry;
    tsSL_CallbackEntry *psOldEntry = NULL;

    DBG_vPrintf(DBG_SERIALLINK_CB, "Remove handler %p for message type 0x%04x\n", prCallback, u16Type);

    pthread_mutex_lock(&sSerialLink.sCallbacks.mutex);

    if (sSerialLink.sCallbacks.psListHead->prCallback == prCallback)
    {
        /* Start of the list */
        psOldEntry = sSerialLink.sCallbacks.psListHead;
        sSerialLink.sCallbacks.psListHead = psOldEntry->psNext;
    }
    else
    {
        psCurrentEntry = sSerialLink.sCallbacks.psListHead;
        while (psCurrentEntry->psNext)
        {
            if (psCurrentEntry->psNext->prCallback == prCallback)
            {
                psOldEntry = psCurrentEntry->psNext;
                psCurrentEntry->psNext = psCurrentEntry->psNext->psNext;
                break;
            }
        }
    }
    pthread_mutex_unlock(&sSerialLink.sCallbacks.mutex);

    if (!psOldEntry)
    {
        DBG_vPrintf(DBG_SERIALLINK_CB, "Entry not found\n");
        return E_SL_ERROR;
    }

    /* Free removed entry from list */
    free(psOldEntry);
    return E_SL_OK;
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


static teSL_Status eSL_ReadMessage(uint16_t *pu16Type, uint16_t *pu16Length, uint16_t u16MaxLength, uint8_t *pu8Message)
{

    static teSL_RxState eRxState = E_STATE_RX_WAIT_START;
    static uint8_t u8CRC;
    uint8_t u8Data;
    static uint16_t u16Bytes;
    static bool bInEsc = FALSE;

    while(bSL_RxByte(&u8Data))
    {
        DBG_vPrintf(DBG_SERIALLINK_COMMS, "0x%02x\n", u8Data);
        switch(u8Data)
        {

        case SL_START_CHAR:
            u16Bytes = 0;
            bInEsc = FALSE;
            DBG_vPrintf(DBG_SERIALLINK_COMMS, "RX Start\n");
            eRxState = E_STATE_RX_WAIT_TYPEMSB;
            break;

        case SL_ESC_CHAR:
            DBG_vPrintf(DBG_SERIALLINK_COMMS, "Got ESC\n");
            bInEsc = TRUE;
            break;

        case SL_END_CHAR:
            DBG_vPrintf(DBG_SERIALLINK_COMMS, "Got END\n");

            if(*pu16Length > u16MaxLength)
            {
                /* Sanity check length before attempting to CRC the message */
                DBG_vPrintf(DBG_SERIALLINK_COMMS, "Length > MaxLength\n");
                eRxState = E_STATE_RX_WAIT_START;
                break;
            }

            if(u8CRC == u8SL_CalculateCRC(*pu16Type, *pu16Length, pu8Message))
            {
#if DBG_SERIALLINK
                int i;
                DBG_vPrintf(DBG_SERIALLINK, "RX Message type 0x%04x length %d: { ", *pu16Type, *pu16Length);
                for (i = 0; i < *pu16Length; i++)
                {
                    printf("0x%02x ", pu8Message[i]);
                }
                printf("}\n");
#endif /* DBG_SERIALLINK */

                eRxState = E_STATE_RX_WAIT_START;
                return E_SL_OK;
            }
            DBG_vPrintf(DBG_SERIALLINK_COMMS, "CRC BAD\n");
            break;

        default:
            if(bInEsc)
            {
                u8Data ^= 0x10;
                bInEsc = FALSE;
            }

            switch(eRxState)
            {

                case E_STATE_RX_WAIT_START:
                    break;


                case E_STATE_RX_WAIT_TYPEMSB:
                    *pu16Type = (uint16_t)u8Data << 8;
                    eRxState++;
                    break;

                case E_STATE_RX_WAIT_TYPELSB:
                    *pu16Type += (uint16_t)u8Data;
                    eRxState++;
                    break;

                case E_STATE_RX_WAIT_LENMSB:
                    *pu16Length = (uint16_t)u8Data << 8;
                    eRxState++;
                    break;

                case E_STATE_RX_WAIT_LENLSB:
                    *pu16Length += (uint16_t)u8Data;
                    DBG_vPrintf(DBG_SERIALLINK_COMMS, "Length %d\n", *pu16Length);
                    if(*pu16Length > u16MaxLength)
                    {
                        DBG_vPrintf(DBG_SERIALLINK_COMMS, "Length > MaxLength\n");
                        eRxState = E_STATE_RX_WAIT_START;
                    }
                    else
                    {
                        eRxState++;
                    }
                    break;

                case E_STATE_RX_WAIT_CRC:
                    DBG_vPrintf(DBG_SERIALLINK_COMMS, "CRC %02x\n", u8Data);
                    u8CRC = u8Data;
                    eRxState++;
                    break;

                case E_STATE_RX_WAIT_DATA:
                    if(u16Bytes < *pu16Length)
                    {
                        DBG_vPrintf(DBG_SERIALLINK_COMMS, "Data\n");
                        pu8Message[u16Bytes++] = u8Data;
                    }
                    break;

                default:
                    DBG_vPrintf(DBG_SERIALLINK_COMMS, "Unknown state\n");
                    eRxState = E_STATE_RX_WAIT_START;
            }
            break;

        }

    }

    return E_SL_NOMESSAGE;
}


/****************************************************************************
*
* NAME: vSL_WriteRawMessage
*
* DESCRIPTION:
*
* PARAMETERS: Name        RW  Usage
*
* RETURNS:
* void
****************************************************************************/
static teSL_Status eSL_WriteMessage(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data)
{
    int n;
    uint8_t u8CRC;

    u8CRC = u8SL_CalculateCRC(u16Type, u16Length, pu8Data);

    DBG_vPrintf(DBG_SERIALLINK_COMMS, "(%d, %d, %02x)\n", u16Type, u16Length, u8CRC);

    if (verbosity >= 10)
    {
        char acBuffer[4096];
        int iPosition = 0, i;

        iPosition = sprintf(&acBuffer[iPosition], "Host->Node 0x%04X (Length % 4d)", u16Type, u16Length);
        for (i = 0; i < u16Length; i++)
        {
            iPosition += sprintf(&acBuffer[iPosition], " 0x%02X", pu8Data[i]);
        }
        DEBUG_PRINTF( "%s", acBuffer);
    }

    /* Send start character */
    if (iSL_TxByte(TRUE, SL_START_CHAR) < 0) return E_SL_ERROR;

    /* Send message type */
    if (iSL_TxByte(FALSE, (u16Type >> 8) & 0xff) < 0) return E_SL_ERROR;
    if (iSL_TxByte(FALSE, (u16Type >> 0) & 0xff) < 0) return E_SL_ERROR;

    /* Send message length */
    if (iSL_TxByte(FALSE, (u16Length >> 8) & 0xff) < 0) return E_SL_ERROR;
    if (iSL_TxByte(FALSE, (u16Length >> 0) & 0xff) < 0) return E_SL_ERROR;

    /* Send message checksum */
    if (iSL_TxByte(FALSE, u8CRC) < 0) return E_SL_ERROR;

    /* Send message payload */
    for(n = 0; n < u16Length; n++)
    {
        if (iSL_TxByte(FALSE, pu8Data[n]) < 0) return E_SL_ERROR;
    }

    /* Send end character */
    if (iSL_TxByte(TRUE, SL_END_CHAR) < 0) return E_SL_ERROR;

    return E_SL_OK;
}

static uint8_t u8SL_CalculateCRC(uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Data)
{
    int n;
    uint8_t u8CRC = 0;

    u8CRC ^= (u16Type >> 8) & 0xff;
    u8CRC ^= (u16Type >> 0) & 0xff;

    u8CRC ^= (u16Length >> 8) & 0xff;
    u8CRC ^= (u16Length >> 0) & 0xff;

    for(n = 0; n < u16Length; n++)
    {
        u8CRC ^= pu8Data[n];
    }
    return(u8CRC);
}

/****************************************************************************
*
* NAME: vSL_TxByte
*
* DESCRIPTION:
*
* PARAMETERS:  Name                RW  Usage
*
* RETURNS:
* void
****************************************************************************/
static int iSL_TxByte(bool bSpecialCharacter, uint8_t u8Data)
{
    if(!bSpecialCharacter && (u8Data < 0x10))
    {
        u8Data ^= 0x10;

        if (eSerial_Write(SL_ESC_CHAR) != E_SERIAL_OK) return -1;
        //DBG_vPrintf(DBG_SERIALLINK_COMMS, " 0x%02x", SL_ESC_CHAR);
    }
    //DBG_vPrintf(DBG_SERIALLINK_COMMS, " 0x%02x", u8Data);

    return eSerial_Write(u8Data);
}


/****************************************************************************
*
* NAME: bSL_RxByte
*
* DESCRIPTION:
*
* PARAMETERS:  Name                RW  Usage
*
* RETURNS:
* void
****************************************************************************/
static bool bSL_RxByte(uint8_t *pu8Data)
{
    if (eSerial_Read(pu8Data) == E_SERIAL_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


static teSL_Status eSL_MessageQueue(tsSerialLink *psSerialLink, uint16_t u16Type, uint16_t u16Length, uint8_t *pu8Message)
{
    int i;
    for (i = 0; i < SL_MAX_MESSAGE_QUEUES; i++)
    {
        pthread_mutex_lock(&psSerialLink->asReaderMessageQueue[i].mutex);

        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "reader message type 0x%04X\n", psSerialLink->asReaderMessageQueue[i].u16Type);

        if (psSerialLink->asReaderMessageQueue[i].u16Type == u16Type)
        {
            DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Found listener for message type 0x%04x in slot %d\n", u16Type, i);

            if (u16Type == E_SL_MSG_STATUS)
            {
                tsSL_Msg_Status *psRxStatus = (tsSL_Msg_Status*)pu8Message;
                tsSL_Msg_Status *psWaitStatus = (tsSL_Msg_Status*)psSerialLink->asReaderMessageQueue[i].pu8Message;

                /* Also check the type of the message that this is status to. */
                if (psWaitStatus)
                {
                    DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Status listener for message type 0x%04X, rx 0x%04X\n", psWaitStatus->u16MessageType, ntohs(psRxStatus->u16MessageType));

                    if (psWaitStatus->u16MessageType != ntohs(psRxStatus->u16MessageType))
                    {
                        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Not the status listener for this message\n");
                        pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
                        continue;
                    }
                }
            }

            uint8_t  *pu8MessageCopy = malloc(u16Length);
            if (!pu8MessageCopy)
            {
                printf( "Memory allocation failure");
                return E_SL_ERROR_NOMEM;
            }
            memcpy(pu8MessageCopy, pu8Message, u16Length);


            psSerialLink->asReaderMessageQueue[i].u16Length = u16Length;
            psSerialLink->asReaderMessageQueue[i].pu8Message = pu8MessageCopy;

            /* Signal data available */
            DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Unlocking queue %d mutex\n", i);
            pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
            pthread_cond_broadcast(&psSerialLink->asReaderMessageQueue[i].cond_data_available);
            return E_SL_OK;
        }
        else
        {
            pthread_mutex_unlock(&psSerialLink->asReaderMessageQueue[i].mutex);
        }
    }
    DBG_vPrintf(DBG_SERIALLINK_QUEUE, "No listeners for message type 0x%04X\n", u16Type);
    return E_SL_NOMESSAGE;
}


static void *pvReaderThread(tsUtilsThread *psThreadInfo)
{
    tsSerialLink *psSerialLink = (tsSerialLink *)psThreadInfo->pvThreadData;
    tsSL_Message  sMessage;
    int iHandled;

    DBG_vPrintf(DBG_SERIALLINK, "Starting reader thread\n");

    psThreadInfo->eState = E_THREAD_RUNNING;

    while (psThreadInfo->eState == E_THREAD_RUNNING)
    {
        /* Initialise buffer */
        memset(&sMessage, 0, sizeof(tsSL_Message));
        /* Initialise length to large value so CRC is skipped if end received */
        sMessage.u16Length = 0xFFFF;

        if (eSL_ReadMessage(&sMessage.u16Type, &sMessage.u16Length, SL_MAX_MESSAGE_LENGTH, sMessage.au8Message) == E_SL_OK)
        {
            iHandled = 0;

            if (verbosity >= 10)
            {
                char acBuffer[4096];
                int iPosition = 0, i;

                iPosition = sprintf(&acBuffer[iPosition], "Node->Host 0x%04X (Length % 4d)", sMessage.u16Type, sMessage.u16Length);
                for (i = 0; i < sMessage.u16Length; i++)
                {
                    iPosition += sprintf(&acBuffer[iPosition], " 0x%02X", sMessage.au8Message[i]);
                }
                DEBUG_PRINTF( "%s", acBuffer);
            }

            if (sMessage.u16Type == E_SL_MSG_LOG)
            {
                /* Log messages handled here first, and passed to new thread in case user has added another handler */
                sMessage.au8Message[sMessage.u16Length] = '\0';
#ifdef DEBUG_SERIALLINK
                uint8_t u8LogLevel = sMessage.au8Message[0];
                char *pcMessage = (char *)&sMessage.au8Message[1];
                DEBUG_PRINTF( "Module: %s (%d)", pcMessage, u8LogLevel);
#endif
                iHandled = 1; /* Message handled by logger */
            }
            else
            {
                // See if any threads are waiting for this message
                int i;
                for (i = 0; i < 2; i++)
                {
                    teSL_Status eStatus = eSL_MessageQueue(psSerialLink, sMessage.u16Type, sMessage.u16Length, sMessage.au8Message);
                    if (eStatus == E_SL_OK)
                    {
                        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Message queued for listener\n");
                        iHandled = 1;
                        break;
                    }
                    else if (eStatus == E_SL_NOMESSAGE)
                    {
                        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "No listener waiting for message type 0x%04X\n", sMessage.u16Type);
                        break;
                    }
                    else
                    {
                        DBG_vPrintf(DBG_SERIALLINK_QUEUE, "Error enqueueing message attempt %d\n", i);
                        /* Wait 200ms before submitting again */
                        printf( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
                        IOT_MSLEEP(200);
                    }
                }
            }

            {
                // Search for callback handlers for this message type
                tsSL_CallbackEntry *psCurrentEntry;
                /* Search through list of registered callbacks */
                pthread_mutex_lock(&psSerialLink->sCallbacks.mutex);

                for (psCurrentEntry = psSerialLink->sCallbacks.psListHead; psCurrentEntry; psCurrentEntry = psCurrentEntry->psNext)
                {
                    if (psCurrentEntry->u16Type == sMessage.u16Type)
                    {
                        tsCallbackThreadData *psCallbackData;
                        DBG_vPrintf(DBG_SERIALLINK_CB, "Found callback routine %p for message 0x%04x\n", psCurrentEntry->prCallback, sMessage.u16Type);

                        // Put the message into the queue for the callback handler thread
                        psCallbackData = malloc(sizeof(tsCallbackThreadData));
                        if (!psCallbackData)
                        {
                            printf( "Memory allocation error\n");
                        }
                        else
                        {
                            memcpy(&psCallbackData->sMessage, &sMessage, sizeof(tsSL_Message));
                            psCallbackData->prCallback = psCurrentEntry->prCallback;
                            psCallbackData->pvUser = psCurrentEntry->pvUser;

                            if (eUtils_QueueQueue(&psSerialLink->sCallbackQueue, psCallbackData) == E_UTILS_OK)
                            {
                                iHandled = 1;
                            }
                            else
                            {
                                DEBUG_PRINTF( "Failed to queue message for callback\n");
                                free(psCallbackData);
                            }
                        }
                        break; // just a single callback for each message type
                    }
                }
                pthread_mutex_unlock(&sSerialLink.sCallbacks.mutex);
            }
            if (!iHandled)
            {
                DEBUG_PRINTF( "Message 0x%04X was not handled\n", sMessage.u16Type);
            }
        }
    }

    {
        int i;
        for (i = 0; i < SL_MAX_MESSAGE_QUEUES; i++)
        {
            psSerialLink->asReaderMessageQueue[i].u16Length  = 0;
            psSerialLink->asReaderMessageQueue[i].pu8Message = NULL;
            pthread_cond_broadcast(&psSerialLink->asReaderMessageQueue[i].cond_data_available);
        }
    }

    DBG_vPrintf(DBG_SERIALLINK, "Exit reader thread\n");

    /* Return from thread clearing resources */
    eUtils_ThreadFinish(psThreadInfo);
    return NULL;
}


static void *pvCallbackHandlerThread(tsUtilsThread *psThreadInfo)
{
    tsSerialLink *psSerialLink = (tsSerialLink *)psThreadInfo->pvThreadData;

    DBG_vPrintf(DBG_SERIALLINK_CB, "Starting callback thread\n");

    psThreadInfo->eState = E_THREAD_RUNNING;

    while (psThreadInfo->eState == E_THREAD_RUNNING)
    {
        tsCallbackThreadData *psCallbackData;

        // int stat = eUtils_QueueDequeue(&psSerialLink->sCallbackQueue, (void**)&psCallbackData);
        int stat = eUtils_QueueDequeueTimed(&psSerialLink->sCallbackQueue, 2000, (void**)&psCallbackData);
        if (stat == E_UTILS_OK)
        {
            DBG_vPrintf(DBG_SERIALLINK_CB, "++++++++++++++++++++++ Calling callback\n" );   // RH
            DBG_vPrintf(DBG_SERIALLINK_CB, "Calling callback %p for message 0x%04X\n", psCallbackData->prCallback, psCallbackData->sMessage.u16Type);

            psCallbackData->prCallback(psCallbackData->pvUser, psCallbackData->sMessage.u16Length, psCallbackData->sMessage.au8Message);

            free(psCallbackData);
            DBG_vPrintf(DBG_SERIALLINK_CB, "++++++++++++++++++++++ Callback ready\n" );   // RH
        } else if ( stat == E_UTILS_ERROR_TIMEOUT ) {
            // printf( "CB heartbeat\n" );
        }
    }

    DBG_vPrintf(DBG_SERIALLINK_CB, "Exit callback thread\n");

    /* Return from thread clearing resources */
    eUtils_ThreadFinish(psThreadInfo);
    return NULL;
}

char * eSL_asErrorString[] =
{
    "E_SL_OK",
    "E_SL_ERROR",
    "E_SL_NOMESSAGE",
    "E_SL_ERROR_SERIAL",
    "E_SL_ERROR_NOMEM",
};

const char * eSL_PrintError(teSL_Status eStatus)
{
  if (eStatus <= E_SL_ERROR_NOMEM)
    return eSL_asErrorString[eStatus];
  else
    return "..."; // Code coming from ZB stack
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
