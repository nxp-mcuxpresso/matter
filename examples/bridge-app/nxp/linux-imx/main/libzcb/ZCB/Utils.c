/****************************************************************************
 *
 * MODULE:             JN51xx Programmer
 *
 * COMPONENT:          Utils.c
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

/** \addtogroup zb
 * \file
 * \brief Utilities
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#ifndef WIN32
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
#endif /* WIN32 */

#include "Utils.h"

#ifndef DBG_THREADS
#define DBG_THREADS 0
#endif /* DBG_THREADS */

#ifndef DBG_QUEUE
#define DBG_QUEUE   0
#endif /* DBG_QUEUE */

#define THREAD_SIGNAL SIGUSR1


#ifdef WIN32
// MinGW includes out of date header files that don't include the prototypes for condition variables

typedef struct _RTL_CONDITION_VARIABLE {
        PVOID Ptr;
} RTL_CONDITION_VARIABLE, *PRTL_CONDITION_VARIABLE;
typedef struct _RTL_SRWLOCK {
        PVOID Ptr;
} RTL_SRWLOCK, *PRTL_SRWLOCK;
typedef RTL_CONDITION_VARIABLE CONDITION_VARIABLE, *PCONDITION_VARIABLE;
typedef RTL_SRWLOCK SRWLOCK, *PSRWLOCK;

WINBASEAPI
VOID
WINAPI
InitializeConditionVariable (
    PCONDITION_VARIABLE ConditionVariable
    );

WINBASEAPI
VOID
WINAPI
WakeConditionVariable (
     PCONDITION_VARIABLE ConditionVariable
    );

WINBASEAPI
VOID
WINAPI
WakeAllConditionVariable (
     PCONDITION_VARIABLE ConditionVariable
    );

WINBASEAPI
BOOL
WINAPI
SleepConditionVariableCS (
     PCONDITION_VARIABLE ConditionVariable,
     PCRITICAL_SECTION CriticalSection,
     DWORD dwMilliseconds
    );

WINBASEAPI
BOOL
WINAPI
SleepConditionVariableSRW (
     PCONDITION_VARIABLE ConditionVariable,
     PSRWLOCK SRWLock,
     DWORD dwMilliseconds,
     ULONG Flags
    );

#endif /* WIN32 */


/************************** Threads Functionality ****************************/

/** Structure representing an OS independant thread */
typedef struct
{
#ifndef WIN32
    pthread_t           thread;
#else
    DWORD               thread_pid;
    HANDLE              thread_handle;
#endif /* WIN32 */
	tprThreadFunction   prThreadFunction;
} tsThreadPrivate;


#ifndef WIN32
/** Signal handler to receive THREAD_SIGNAL.
 *  This is just used to interrupt system calls such as recv() and sleep().
 */
static void thread_signal_handler(int sig)
{
    DBG_vPrintf(DBG_THREADS, "Signal %d received\n", sig);
}
#endif /* WIN32 */


#ifndef WIN32
static void *pvThreadFunction(void *psThreadInfoVoid)
#else
static DWORD WINAPI dwThreadFunction(void *psThreadInfoVoid)
#endif /* WIN32 */
{
    tsUtilsThread *psThreadInfo = (tsUtilsThread *)psThreadInfoVoid;
    tsThreadPrivate *psThreadPrivate = (tsThreadPrivate *)psThreadInfo->pvPriv;

    DBG_vPrintf(DBG_THREADS, "Thread %p running for function %p\n", psThreadInfo, psThreadPrivate->prThreadFunction);

    if (psThreadInfo->eThreadDetachState == E_THREAD_DETACHED)
    {
        DBG_vPrintf(DBG_THREADS, "Detach Thread %p\n", psThreadInfo);
#ifndef WIN32
        if (pthread_detach(psThreadPrivate->thread))
        {
            perror("pthread_detach()");
        }
#endif /* WIN32 */
    }

    psThreadPrivate->prThreadFunction(psThreadInfo);
#ifndef WIN32
    pthread_exit(NULL);
    return NULL;
#else
    return 0;
#endif /* WIN32 */
}


teUtilsStatus eUtils_ThreadStart(tprThreadFunction prThreadFunction, tsUtilsThread *psThreadInfo, teThreadDetachState eDetachState)
{
    tsThreadPrivate *psThreadPrivate;

    psThreadInfo->eState = E_THREAD_STOPPED;

    DBG_vPrintf(DBG_THREADS, "Start Thread %p to run function %p\n", psThreadInfo, prThreadFunction);

    psThreadPrivate = malloc(sizeof(tsThreadPrivate));
    if (!psThreadPrivate)
    {
        return E_UTILS_ERROR_NO_MEM;
    }

    psThreadInfo->pvPriv = psThreadPrivate;

    psThreadInfo->eThreadDetachState = eDetachState;

    psThreadPrivate->prThreadFunction = prThreadFunction;

#ifndef WIN32
    {
        static int iFirstTime = 1;

        if (iFirstTime)
        {
            /* Set up sigmask to receive configured signal in the main thread.
             * All created threads also get this signal mask, so all threads
             * get the signal. But we can use pthread_signal to direct it at one.
             */
            struct sigaction sa;

            sa.sa_handler = thread_signal_handler;
            sa.sa_flags = 0;
            sigemptyset(&sa.sa_mask);

            if (sigaction(THREAD_SIGNAL, &sa, NULL) == -1)
            {
                perror("sigaction");
            }
            else
            {
                DBG_vPrintf(DBG_THREADS, "Signal action registered\n\r");
                iFirstTime = 0;
            }
        }
    }

    if (pthread_create(&psThreadPrivate->thread, NULL,
        pvThreadFunction, psThreadInfo))
    {
        perror("Could not start thread");
        return E_UTILS_ERROR_FAILED;
    }
#else
    psThreadPrivate->thread_handle = CreateThread(
        NULL,                                       // default security attributes
        0,                                          // use default stack size
        (LPTHREAD_START_ROUTINE)dwThreadFunction,   // thread function name
        psThreadInfo,                               // argument to thread function
        0,                                          // use default creation flags
        &psThreadPrivate->thread_pid);              // returns the thread identifier
    if (!psThreadPrivate->thread_handle)
    {
        perror("Could not start thread");
        return E_UTILS_ERROR_FAILED;
    }
#endif /* WIN32 */
    return  E_UTILS_OK;
}


teUtilsStatus eUtils_ThreadStop(tsUtilsThread *psThreadInfo)
{
    teUtilsStatus eStatus;
    tsThreadPrivate *psThreadPrivate = (tsThreadPrivate *)psThreadInfo->pvPriv;

    DBG_vPrintf(DBG_THREADS, "Stopping Thread %p\n", psThreadInfo);

    psThreadInfo->eState = E_THREAD_STOPPING;

    if (psThreadPrivate)
    {
#ifndef WIN32
        /* Send signal to the thread to kick it out of any system call it was in */
        pthread_kill(psThreadPrivate->thread, THREAD_SIGNAL);
        DBG_vPrintf(DBG_THREADS, "Signaled Thread %p\n", psThreadInfo);

        if (psThreadInfo->eThreadDetachState == E_THREAD_JOINABLE)
        {
            /* Thread is joinable */
            if (pthread_join(psThreadPrivate->thread, NULL))
            {
                perror("Could not join thread");
                return E_UTILS_ERROR_FAILED;
            }
        }
        else
        {
            DBG_vPrintf(DBG_THREADS, "Cannot join detached thread %p\n", psThreadInfo);
            return E_UTILS_ERROR_FAILED;
        }
#else

#endif /* WIN32 */
    }

    eStatus = eUtils_ThreadWait(psThreadInfo);

    DBG_vPrintf(DBG_THREADS, "Stopped Thread %p\n", psThreadInfo);
    return eStatus;
}


teUtilsStatus eUtils_ThreadFinish(tsUtilsThread *psThreadInfo)
{
    /* Free the Private data */
    tsThreadPrivate *psThreadPrivate = (tsThreadPrivate *)psThreadInfo->pvPriv;

    if (psThreadInfo->eThreadDetachState == E_THREAD_DETACHED)
    {
        /* Only free this if we are detached, otherwise another thread won't be able to join. */
        free(psThreadPrivate);
    }
    psThreadInfo->eState = E_THREAD_STOPPED;

    DBG_vPrintf(DBG_THREADS, "Finish Thread %p\n", psThreadInfo);

#ifndef WIN32
    /* Cleanup function is called when pthread quits */
    pthread_exit(NULL);
#else
    ExitThread(0);
#endif /* WIN32 */
    return E_UTILS_OK; /* Control won't get here */
}


teUtilsStatus eUtils_ThreadWait(tsUtilsThread *psThreadInfo)
{
    tsThreadPrivate *psThreadPrivate = (tsThreadPrivate *)psThreadInfo->pvPriv;

    DBG_vPrintf(DBG_THREADS, "Wait for Thread %p\n", psThreadInfo);

    if (psThreadPrivate)
    {
        if (psThreadInfo->eThreadDetachState == E_THREAD_JOINABLE)
        {
#if defined POSIX
            /* Thread is joinable */
            if (pthread_join(psThreadPrivate->thread, NULL))
            {
                perror("Could not join thread");
                return E_UTILS_ERROR_FAILED;
            }
#elif defined WIN32
            WaitForSingleObject(psThreadPrivate->thread_handle, INFINITE);
#endif
            /* We can now free the thread private info */
            free(psThreadPrivate);
        }
        else
        {
            DBG_vPrintf(DBG_THREADS, "Cannot join detached thread %p\n", psThreadInfo);
            return E_UTILS_ERROR_FAILED;
        }
    }

    psThreadInfo->eState = E_THREAD_STOPPED;

    return E_UTILS_OK;
}


teUtilsStatus eUtils_ThreadYield(void)
{
#ifndef WIN32
    sched_yield();
#else
    SwitchToThread();
#endif
    return E_UTILS_OK;
}


/************************** Lock Functionality *******************************/


typedef struct
{
#if DBG_LOCKS
    const char         *pcLastLockLocation;
#endif /* DBG_LOCKS */

#ifndef WIN32
     pthread_mutex_t    mMutex;
#else
     HANDLE             hMutex;
#endif /* WIN32 */
} tsLockPrivate;


teUtilsStatus eUtils_LockCreate(tsUtilsLock *psLock)
{
    tsLockPrivate *psLockPrivate;

    psLockPrivate = malloc(sizeof(tsLockPrivate));
    if (!psLockPrivate)
    {
        return E_UTILS_ERROR_NO_MEM;
    }

    psLock->pvPriv = psLockPrivate;

#if DBG_LOCKS
    psLockPrivate->pcLastLockLocation = NULL;
#endif /* DBG_LOCKS */

#ifndef WIN32
    {
        pthread_mutexattr_t     attr;
        /* Create a recursive mutex, as we need to allow the same thread to lock mutexes a number of times */
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        if (pthread_mutex_init(&psLockPrivate->mMutex, &attr) != 0)
        {
            DBG_vPrintf(DBG_LOCKS, "Error initialising mutex\n");
            return E_UTILS_ERROR_FAILED;
        }
    }
#else
    psLockPrivate->hMutex = CreateMutex(NULL, FALSE, NULL);
    if (psLockPrivate->hMutex == NULL)
    {
        DBG_vPrintf(DBG_LOCKS, "Error initialising mutex\n");
        return E_UTILS_ERROR_FAILED;
    }
#endif /* WIN32 */
    DBG_vPrintf(DBG_LOCKS, "Lock Create: %p\n", psLock);
    return E_UTILS_OK;
}


teUtilsStatus eUtils_LockDestroy(tsUtilsLock *psLock)
{
    tsLockPrivate *psLockPrivate = (tsLockPrivate *)psLock->pvPriv;
    DBG_vPrintf(DBG_LOCKS, "Lock Destroy: %p\n", psLock);
#ifndef WIN32
    if (pthread_mutex_destroy(&psLockPrivate->mMutex) == EBUSY)
    {
        DBG_vPrintf(DBG_LOCKS, "Lock is busy, can't destroy: %p\n", psLock);
        return E_UTILS_ERROR_BUSY;
    }
#else
    CloseHandle(psLockPrivate->hMutex);
#endif /* WIN32 */
    free(psLockPrivate);
    DBG_vPrintf(DBG_LOCKS, "Lock Destroyed: %p\n", psLock);
    return E_UTILS_OK;
}


teUtilsStatus eUtils_LockLockImpl(tsUtilsLock *psLock, const char *pcLocation)
{
    tsLockPrivate *psLockPrivate = (tsLockPrivate *)psLock->pvPriv;
#ifndef WIN32
    int err;
    DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx locking: %p at %s\n", pthread_self(), psLock, pcLocation);

    if ((err = pthread_mutex_lock(&psLockPrivate->mMutex)))
    {
        DBG_vPrintf(DBG_LOCKS, "Could not lock mutex (%s)\n", strerror(err));
    }
    else
    {
        DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx locked: %p at %s\n", pthread_self(), psLock, pcLocation);
#if DBG_LOCKS
        psLockPrivate->pcLastLockLocation = pcLocation;
#endif /* DBG_LOCKS */
    }
#else
    DBG_vPrintf(DBG_LOCKS, "Locking %p at %s\n", psLock, pcLocation);
    WaitForSingleObject(psLockPrivate->hMutex, INFINITE);
    DBG_vPrintf(DBG_LOCKS, "Locked %p at %s\n", psLock, pcLocation);
#endif /* WIN32 */

    return E_UTILS_OK;
}


teUtilsStatus eUtils_LockLockTimedImpl(tsUtilsLock *psLock, uint32_t u32WaitTimeout, const char *pcLocation)
{
    tsLockPrivate *psLockPrivate = (tsLockPrivate *)psLock->pvPriv;
#ifndef WIN32
    struct timeval sNow;
    struct timespec sTimeout;

    gettimeofday(&sNow, NULL);
    sTimeout.tv_sec = sNow.tv_sec + u32WaitTimeout;
    sTimeout.tv_nsec = sNow.tv_usec * 1000;

    DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx time locking: %p\n", pthread_self(), psLock);

    switch (pthread_mutex_timedlock(&psLockPrivate->mMutex, &sTimeout))
    {
        case (0):
            DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx: time locked: %p\n", pthread_self(), psLock);
#if DBG_LOCKS
            psLockPrivate->pcLastLockLocation = pcLocation;
#endif /* DBG_LOCKS */
            return E_UTILS_OK;

        case (ETIMEDOUT):
            DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx: time out locking: %p\n", pthread_self(), psLock);
            return E_UTILS_ERROR_TIMEOUT;

        case (EDEADLK):
            DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx: already had locked: %p\n", pthread_self(), psLock);
            return E_UTILS_ERROR_FAILED;

        default:
            DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx: error locking: %p\n", pthread_self(), psLock);
    }
#else
    switch(WaitForSingleObject(psLockPrivate->hMutex, u32WaitTimeout * 1000))
    {
        case (WAIT_OBJECT_0):
            DBG_vPrintf(DBG_LOCKS, "Time locked\n");
            return E_UTILS_OK;

        case (WAIT_TIMEOUT):
            DBG_vPrintf(DBG_LOCKS, "Time out locking\n");
            return E_UTILS_ERROR_TIMEOUT;

        default:
            DBG_vPrintf(DBG_LOCKS, "Error locking\n");
    }
#endif /* WIN32 */
    return E_UTILS_ERROR_FAILED;
}


teUtilsStatus eUtils_LockTryLock(tsUtilsLock *psLock)
{
    tsLockPrivate *psLockPrivate = (tsLockPrivate *)psLock->pvPriv;
#ifndef WIN32
    DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx try locking: %p\n", pthread_self(), psLock);
    if (pthread_mutex_trylock(&psLockPrivate->mMutex) != 0)
    {
        DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx could not lock: %p\n", pthread_self(), psLock);
        return E_UTILS_ERROR_FAILED;
    }
    DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx locked: %p\n", pthread_self(), psLock);
#else
    // Wait with 0mS timeout
   switch(WaitForSingleObject(psLockPrivate->hMutex, 0))
           {
            case (WAIT_OBJECT_0):
                DBG_vPrintf(DBG_LOCKS, "Try lock - locked\n");
                return E_UTILS_OK;
                break;

            default: // Including timeout
                DBG_vPrintf(DBG_LOCKS, "Try lock - failed to get lock\n");
                return E_UTILS_ERROR_FAILED;
                break;
        }
#endif /* WIN32 */

    return E_UTILS_OK;
}


teUtilsStatus eUtils_LockUnlock(tsUtilsLock *psLock)
{
    tsLockPrivate *psLockPrivate = (tsLockPrivate *)psLock->pvPriv;

#ifndef WIN32
    DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx unlocking: %p\n", pthread_self(), psLock);
    pthread_mutex_unlock(&psLockPrivate->mMutex);
    DBG_vPrintf(DBG_LOCKS, "Thread 0x%lx unlocked: %p\n", pthread_self(), psLock);
#else
    DBG_vPrintf(DBG_LOCKS, "Unlocking %p\n", psLock);
    ReleaseMutex(psLockPrivate->hMutex);
#endif /* WIN32 */
    return E_UTILS_OK;
}


/************************** Queue Functionality ******************************/


typedef struct
{
    void              **apvBuffer;
    uint32_t            u32Capacity;
    uint32_t            u32Size;
    uint32_t            u32In;
    uint32_t            u32Out;

    int                 iFlags;

#ifndef WIN32
    pthread_mutex_t     mMutex;
    pthread_cond_t      cond_space_available;
    pthread_cond_t      cond_data_available;
#else
    CRITICAL_SECTION    hMutex;
    CONDITION_VARIABLE  hSpaceAvailable;
    CONDITION_VARIABLE  hDataAvailable;
#endif /* WIN32 */
} tsQueuePrivate;


teUtilsStatus eUtils_QueueCreate(tsUtilsQueue *psQueue, uint32_t u32MaxCapacity, int iFlags)
{
    tsQueuePrivate *psQueuePrivate;

    psQueuePrivate = malloc(sizeof(tsQueuePrivate));
    if (!psQueuePrivate)
    {
        return E_UTILS_ERROR_NO_MEM;
    }

    psQueue->pvPriv = psQueuePrivate;

    psQueuePrivate->apvBuffer = malloc(sizeof(void *) * u32MaxCapacity);

    if (!psQueuePrivate->apvBuffer)
    {
        free(psQueue->pvPriv);
        return E_UTILS_ERROR_NO_MEM;
    }

    psQueuePrivate->u32Capacity = u32MaxCapacity;
    psQueuePrivate->u32Size     = 0;
    psQueuePrivate->u32In       = 0;
    psQueuePrivate->u32Out      = 0;

    psQueuePrivate->iFlags      = iFlags;

#ifndef WIN32
    pthread_mutex_init          (&psQueuePrivate->mMutex, NULL);
    pthread_cond_init           (&psQueuePrivate->cond_space_available, NULL);
    pthread_cond_init           (&psQueuePrivate->cond_data_available, NULL);
#else
    InitializeCriticalSection   (&psQueuePrivate->hMutex);
    InitializeConditionVariable (&psQueuePrivate->hSpaceAvailable);
    InitializeConditionVariable (&psQueuePrivate->hDataAvailable);
#endif
    DBG_vPrintf(DBG_QUEUE, "Queue %p created (capacity %d)\n", psQueue, u32MaxCapacity);
    return E_UTILS_OK;
}


teUtilsStatus eUtils_QueueDestroy(tsUtilsQueue *psQueue)
{
    tsQueuePrivate *psQueuePrivate = (tsQueuePrivate*)psQueue->pvPriv;
    if (!psQueuePrivate)
    {
        return E_UTILS_ERROR_FAILED;
    }
    free(psQueuePrivate->apvBuffer);

#ifndef WIN32
    pthread_mutex_destroy   (&psQueuePrivate->mMutex);
    pthread_cond_destroy    (&psQueuePrivate->cond_space_available);
    pthread_cond_destroy    (&psQueuePrivate->cond_data_available);
#else
    DeleteCriticalSection   (&psQueuePrivate->hMutex);
    /* Can't find a reference to deleting condition variables in MSDN */
#endif
    free(psQueuePrivate);
    DBG_vPrintf(DBG_QUEUE, "Queue %p destroyed\n", psQueue);
    return E_UTILS_OK;
}


teUtilsStatus eUtils_QueueQueue(tsUtilsQueue *psQueue, void *pvData)
{
    tsQueuePrivate *psQueuePrivate = (tsQueuePrivate*)psQueue->pvPriv;

    DBG_vPrintf(DBG_QUEUE, "Queue %p: Queue %p\n", psQueue, pvData);

#ifndef WIN32
    pthread_mutex_lock      (&psQueuePrivate->mMutex);
#else
    EnterCriticalSection    (&psQueuePrivate->hMutex);
#endif /* WIN32 */
    DBG_vPrintf(DBG_QUEUE, "Got mutex on queue %p (size = %d)\n", psQueue, psQueuePrivate->u32Size);

    if (psQueuePrivate->iFlags & UTILS_QUEUE_NONBLOCK_INPUT)
    {
        // Check if buffer is full and return if so.
        if (psQueuePrivate->u32Size == psQueuePrivate->u32Capacity)
        {
            DBG_vPrintf(DBG_QUEUE, "Queue full, could not enqueue entry\n");
#ifndef WIN32
            pthread_mutex_unlock    (&psQueuePrivate->mMutex);
#else
            LeaveCriticalSection    (&psQueuePrivate->hMutex);
#endif /* WIN32 */
            return E_UTILS_ERROR_BLOCK;
        }
    }
    else
    {
        // Block until space is available
        while (psQueuePrivate->u32Size == psQueuePrivate->u32Capacity)
#ifndef WIN32
            pthread_cond_wait       (&psQueuePrivate->cond_space_available, &psQueuePrivate->mMutex);
#else
            SleepConditionVariableCS(&psQueuePrivate->hSpaceAvailable, &psQueuePrivate->hMutex, INFINITE);
#endif /* WIN32 */
    }

    psQueuePrivate->apvBuffer[psQueuePrivate->u32In] = pvData;
    psQueuePrivate->u32Size++;
    psQueuePrivate->u32In = (psQueuePrivate->u32In+1) % psQueuePrivate->u32Capacity;

#ifndef WIN32
    pthread_mutex_unlock    (&psQueuePrivate->mMutex);
    pthread_cond_broadcast  (&psQueuePrivate->cond_data_available);
#else
    LeaveCriticalSection    (&psQueuePrivate->hMutex);
    WakeConditionVariable   (&psQueuePrivate->hDataAvailable);
#endif /* WIN32 */

    DBG_vPrintf(DBG_QUEUE, "Queue %p: Queued %p\n", psQueue, pvData);
    return E_UTILS_OK;
}


teUtilsStatus eUtils_QueueDequeue(tsUtilsQueue *psQueue, void **ppvData)
{
    tsQueuePrivate *psQueuePrivate = (tsQueuePrivate*)psQueue->pvPriv;
    DBG_vPrintf(DBG_QUEUE, "Queue %p: Dequeue\n", psQueue);

#ifndef WIN32
    pthread_mutex_lock      (&psQueuePrivate->mMutex);
#else
    EnterCriticalSection    (&psQueuePrivate->hMutex);
#endif /* WIN32 */
    DBG_vPrintf(DBG_QUEUE, "Got mutex on queue %p (size=%d)\n", psQueue, psQueuePrivate->u32Size);

    if (psQueuePrivate->iFlags & UTILS_QUEUE_NONBLOCK_OUTPUT)
    {
        // Check if buffer is empty and return if so.
        if (psQueuePrivate->u32Size == 0)
        {
            DBG_vPrintf(DBG_QUEUE, "Queue empty\n");
#ifndef WIN32
            pthread_mutex_unlock    (&psQueuePrivate->mMutex);
#else
            LeaveCriticalSection    (&psQueuePrivate->hMutex);
#endif /* WIN32 */
            return E_UTILS_ERROR_BLOCK;
        }
    }
    else
    {
        // Wait for data to become available
        while (psQueuePrivate->u32Size == 0)
#ifndef WIN32
            pthread_cond_wait       (&psQueuePrivate->cond_data_available, &psQueuePrivate->mMutex);
#else
            SleepConditionVariableCS(&psQueuePrivate->hDataAvailable, &psQueuePrivate->hMutex, INFINITE);
#endif /* WIN32 */
    }

    *ppvData = psQueuePrivate->apvBuffer[psQueuePrivate->u32Out];
    psQueuePrivate->u32Size--;
    psQueuePrivate->u32Out = (psQueuePrivate->u32Out + 1) % psQueuePrivate->u32Capacity;

#ifndef WIN32
    pthread_mutex_unlock    (&psQueuePrivate->mMutex);
    pthread_cond_broadcast  (&psQueuePrivate->cond_space_available);
#else
    LeaveCriticalSection    (&psQueuePrivate->hMutex);
    WakeConditionVariable   (&psQueuePrivate->hSpaceAvailable);
#endif /* WIN32 */

    DBG_vPrintf(DBG_QUEUE, "Queue %p: Dequeued %p\n", psQueue, *ppvData);
    return E_UTILS_OK;
}


teUtilsStatus eUtils_QueueDequeueTimed(tsUtilsQueue *psQueue, uint32_t u32WaitMs, void **ppvData)
{
    tsQueuePrivate *psQueuePrivate = (tsQueuePrivate*)psQueue->pvPriv;
#ifndef WIN32
    pthread_mutex_lock      (&psQueuePrivate->mMutex);
#else
    EnterCriticalSection    (&psQueuePrivate->hMutex);
#endif /* WIN32 */

    while (psQueuePrivate->u32Size == 0)
    {
#ifndef WIN32
        struct timeval sNow;
        struct timespec sTimeout;

        memset(&sNow, 0, sizeof(struct timeval));
        gettimeofday(&sNow, NULL);
        sTimeout.tv_sec = sNow.tv_sec + (u32WaitMs/1000);
        sTimeout.tv_nsec = (sNow.tv_usec + ((u32WaitMs % 1000) * 1000)) * 1000;
        if (sTimeout.tv_nsec > 1000000000)
        {
            sTimeout.tv_sec++;
            sTimeout.tv_nsec -= 1000000000;
        }
        //DBG_vPrintf(DBG_QUEUE, "Dequeue timed: now    %lu s, %lu ns\n", sNow.tv_sec, sNow.tv_usec * 1000);
        //DBG_vPrintf(DBG_QUEUE, "Dequeue timed: until  %lu s, %lu ns\n", sTimeout.tv_sec, sTimeout.tv_nsec);

        switch (pthread_cond_timedwait(&psQueuePrivate->cond_data_available, &psQueuePrivate->mMutex, &sTimeout))
#else
        // Set n = 0 for success or the error code.
        int n = 0;

        if (!SleepConditionVariableCS(&psQueuePrivate->hDataAvailable, &psQueuePrivate->hMutex, u32WaitMs))
        {
            n = GetLastError();
        }
        switch(n)
#endif /* WIN32 */
        {
            case (0):
                break;
#ifndef WIN32
            case (ETIMEDOUT):
                pthread_mutex_unlock(&psQueuePrivate->mMutex);
#else
            case (ERROR_TIMEOUT):
                LeaveCriticalSection(&psQueuePrivate->hMutex);
#endif /* WIN32 */
                return E_UTILS_ERROR_TIMEOUT;

            default:
#ifndef WIN32
                pthread_mutex_unlock(&psQueuePrivate->mMutex);
#else
                LeaveCriticalSection(&psQueuePrivate->hMutex);
#endif /* WIN32 */
                return E_UTILS_ERROR_FAILED;
        }
    }

    *ppvData = psQueuePrivate->apvBuffer[psQueuePrivate->u32Out];
    --psQueuePrivate->u32Size;
    DBG_vPrintf(DBG_QUEUE, "Queue %p (size=%d)\n", psQueue, psQueuePrivate->u32Size);

    psQueuePrivate->u32Out = (psQueuePrivate->u32Out + 1) % psQueuePrivate->u32Capacity;
#ifndef WIN32
    pthread_mutex_unlock    (&psQueuePrivate->mMutex);
    pthread_cond_broadcast  (&psQueuePrivate->cond_space_available);
#else
    LeaveCriticalSection    (&psQueuePrivate->hMutex);
    WakeConditionVariable   (&psQueuePrivate->hSpaceAvailable);
#endif /* WIN32 */
    return E_UTILS_OK;
}

uint32_t u32AtomicAdd(volatile uint32_t *pu32Value, uint32_t u32Operand)
{
#if defined(_MSC_VER)
    return InterlockedExchangeAdd   (pu32Value, u32Operand);
#else
    return __sync_add_and_fetch     (pu32Value, u32Operand);
#endif
}
