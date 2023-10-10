/****************************************************************************
 *
 * MODULE:             JN51xx Programmer
 *
 * COMPONENT:          Threads / Mutexes Library
 *
 * REVISION:           $Revision$
 *
 * DATED:              $Date$
 *
 * AUTHOR:             Matt Redfearn
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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <arpa/inet.h>
#endif

#if defined __cplusplus
extern "C" {
#endif

#ifndef DBG_LOCKS
#define DBG_LOCKS 0
#endif /* DBG_LOCKS */


#if DBG_LOCKS

#define UTILS_STRINGIZE(x) UTILS_STRINGIZE2(x)
#define UTILS_STRINGIZE2(x) #x
#define UTILS_LINE_STRING UTILS_STRINGIZE(__LINE__)

#define eUtils_LockLock(psLock)                         eUtils_LockLockImpl(psLock, __FILE__ ":" UTILS_LINE_STRING)
#define eUtils_LockLockTimed(psLock, u32WaitTimeout)    eUtils_LockLockImpl(psLock, u32WaitTimeout, __FILE__ ":" UTILS_LINE_STRING)

#else

#define eUtils_LockLock(psLock)                         eUtils_LockLockImpl(psLock, NULL)
#define eUtils_LockLockTimed(psLock, u32WaitTimeout)    eUtils_LockLockImpl(psLock, u32WaitTimeout, NULL)

#endif /* DBG_LOCKS */



#define DBG_vPrintf(a,b,ARGS...) do {  if (a) printf("%s: " b, __FUNCTION__, ## ARGS); } while(0)
#define DBG_vAssert(a,b) do {  if (a && !(b)) printf(__FILE__ " %d : Asset Failed\n", __LINE__ ); } while(0)


#define DBG_vPrintf_IPv6Address(a,b) do {                                   \
    if (a) {                                                                \
        char buffer[INET6_ADDRSTRLEN] = "Could not determine address\n";    \
        inet_ntop(AF_INET6, &b, buffer, INET6_ADDRSTRLEN);                  \
        printf("%s\n", buffer);                                             \
    }                                                                       \
} while(0)


/* Macro to generate a mask of a given flag number */
#define UTILS_FLAG(a)   (1 << a)


/** Enumerated type of thread status's */
typedef enum
{
    E_UTILS_OK,
    E_UTILS_ERROR_FAILED,
    E_UTILS_ERROR_TIMEOUT,
    E_UTILS_ERROR_NO_MEM,
    E_UTILS_ERROR_BUSY,
    E_UTILS_ERROR_BLOCK,    /**< The operation would have blocked */
} teUtilsStatus;


/** Enumerated type for thread detached / joinable states */
typedef enum
{
    E_THREAD_JOINABLE,      /**< Thread is created so that it can be waited on and joined */
    E_THREAD_DETACHED,      /**< Thread is created detached so all resources are automatically free'd at exit. */
} teThreadDetachState;


/** Structure to represent a thread */
typedef struct
{
    volatile enum
    {
        E_THREAD_STOPPED,   /**< Thread stopped */
        E_THREAD_RUNNING,   /**< Thread running */
        E_THREAD_STOPPING,  /**< Thread signaled to stop */
    } eState;               /**< Enumerated type of thread states */
    teThreadDetachState
        eThreadDetachState; /**< Detach state of the thread */
    void *pvPriv;           /**< Implementation specfific private structure */
    void *pvThreadData;     /**< Pointer to threads data parameter */
} tsUtilsThread;

/** Structure to represent a lock(mutex) used on data structures
 */
typedef struct
{
    void *pvPriv;                               /**< Implementation specific private structure */
} tsUtilsLock;

typedef void *(*tprThreadFunction)(tsUtilsThread *psThreadInfo);

/** Function to start a thread */
teUtilsStatus eUtils_ThreadStart(tprThreadFunction prThreadFunction, tsUtilsThread *psThreadInfo, teThreadDetachState eDetachState);


/** Function to stop a thread
 *  This function signals a thread to stop, then blocks until the specified thread exits.
 */
teUtilsStatus eUtils_ThreadStop(tsUtilsThread *psThreadInfo);


/** Function to be called within the thread when it is finished to clean up memory */
teUtilsStatus eUtils_ThreadFinish(tsUtilsThread *psThreadInfo);

/** Function to wait for a thread to exit.
 *  This function blocks the calling thread until the specified thread terminates.
 */
teUtilsStatus eUtils_ThreadWait(tsUtilsThread *psThreadInfo);

/** Function to yield the CPU to another thread
 *  \return E_THREAD_OK on success.
 */
teUtilsStatus eUtils_ThreadYield(void);


/** Enumerated type of thread status's */

teUtilsStatus eUtils_LockCreate(tsUtilsLock *psLock);

teUtilsStatus eUtils_LockDestroy(tsUtilsLock *psLock);

/** Lock the data structure associated with this lock
 *  \param  psLock  Pointer to lock structure
 *  \param  pcLocation String describing source line that is acquiring the lock.
 *  \return E_LOCK_OK if locked ok
 */
teUtilsStatus eUtils_LockLockImpl(tsUtilsLock *psLock, const char *pcLocation);

/** Lock the data structure associated with this lock
 *  If possible within \param u32WaitTimeout seconds.
 *  \param  psLock  Pointer to lock structure
 *  \param  u32WaitTimeout  Number of seconds to attempt to lock the structure for
 *  \param  pcLocation String describing source line that is acquiring the lock.
 *  \return E_LOCK_OK if locked ok, E_LOCK_ERROR_TIMEOUT if structure could not be acquired
 */
teUtilsStatus eUtils_LockLockTimedImpl(tsUtilsLock *psLock, uint32_t u32WaitTimeout, const char *pcLocation);


/** Attempt to lock the data structure associated with this lock
 *  If the lock could not be acquired immediately, return E_LOCK_ERROR_FAILED.
 *  \param  psLock  Pointer to lock structure
 *  \return E_LOCK_OK if locked ok or E_LOCK_ERROR_FAILED if not
 */
teUtilsStatus eUtils_LockTryLock(tsUtilsLock *psLock);


/** Unlock the data structure associated with this lock
 *  \param  psLock  Pointer to lock structure
 *  \return E_LOCK_OK if unlocked ok
 */
teUtilsStatus eUtils_LockUnlock(tsUtilsLock *psLock);


/** @{ Queue flags */
#define UTILS_QUEUE_NONBLOCK_INPUT      (UTILS_FLAG(0))     /**< Attempts to add items to the queue will not block if the queue is full */
#define UTILS_QUEUE_NONBLOCK_OUTPUT     (UTILS_FLAG(1))     /**< Attempts to remove items from the queue will not block if the queue is empty */
/** @} */


typedef struct
{
    void *pvPriv;
} tsUtilsQueue;

teUtilsStatus eUtils_QueueCreate(tsUtilsQueue *psQueue, uint32_t u32MaxCapacity, int iFlags);

teUtilsStatus eUtils_QueueDestroy(tsUtilsQueue *psQueue);

teUtilsStatus eUtils_QueueQueue(tsUtilsQueue *psQueue, void *pvData);

teUtilsStatus eUtils_QueueDequeue(tsUtilsQueue *psQueue, void **ppvData);

teUtilsStatus eUtils_QueueDequeueTimed(tsUtilsQueue *psQueue, uint32_t u32WaitMs, void **ppvData);


/** Atomically add a 32 bit value to another.
 *  \param pu32Value        Pointer to value to update
 *  \param u32Operand       Value to add
 *  \return New value
 */
uint32_t u32AtomicAdd(volatile uint32_t *pu32Value, uint32_t u32Operand);


/** Atomically get the value of a 32 bit value */
#define u32AtomicGet(pu32Value) u32AtomicAdd(pu32Value, 0)


#if defined __cplusplus
}
#endif

#endif /* __UTILS_H__ */