#pragma once

#ifndef PXThreadPoolIncluded
#define PXThreadPoolIncluded

#include <PX/OS/Async/PXThread.h>

#define PXThreadPoolEnableASYNC (1<<0)
#define PXThreadPoolCreated     (1<<1)

typedef struct PXThreadPool_ PXThreadPool;

PXPrivate PXBool PXAPI PXThreadPoolTaskNextWorkGet(PXThreadPool* pxThreadPool, PXTask PXREF pxTask);
PXPublic PXTask* PXAPI PXThreadPoolTaskUpdateWork(PXThreadPool* pxThreadPool, const PXI32U taskID, void* function, void* parameter1, void* parameter2, const PXI32U behaviour);

// Function WILL copy thread for thread-safety as data could be moved without notice
// struct shall not be modified, use the threadpool
PXPrivate PXResult PXAPI PXThreadPoolThreadSelf(PXThreadPool PXREF pxThreadPool, PXThread PXREF pxThread);
PXPrivate PXResult PXAPI PXThreadPoolTaskInvoke(PXThreadPool PXREF pxThreadPool, PXTask PXREF pxTask);

PXPublic PXBool PXAPI PXThreadPoolIsMainThread();
PXPublic PXResult PXAPI PXThreadPoolClose(PXThreadPool* pxThreadPool);
PXPublic PXResult PXAPI PXThreadPoolCreate(PXThreadPool* pxThreadPool);
PXPublic PXResult PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool PXREF pxThreadPool, const PXI32U amount);
PXPublic PXResult PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool PXREF pxThreadPool, const PXI32U amount);

PXPublic void PXAPI PXThreadPoolWaking(PXThreadPool* pxThreadPool);

// Task will be added to the task queue. 
// Inserted to any task that is avalibe. 
// If not a task will be created internally.
PXPublic PXResult PXAPI PXThreadPoolQueueWork(PXThreadPool PXREF pxThreadPool, const PXI32U taskID, void* function, void* parameter1, void* parameter2, const PXI32U behaviour);

// Prepares an amount of tasks, optionally write IDs of the tasks into a list to wait for later
PXPublic PXResult PXAPI PXThreadPoolQueuePrepare(PXThreadPool* pxThreadPool, PXI32U** listIDs, const PXSize amount);

PXPublic PXResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* pxThreadPool, const PXBool cancelRunning);
PXPublic PXResult PXAPI PXThreadPoolWaitForSpesific(PXThreadPool* pxThreadPool, PXI32U* listIDs, const PXSize amount, const PXBool cancelRunning);


// Call this function in the main thread for syncronous functions
PXPublic PXResult PXAPI PXThreadPoolProcessSYNC(PXThreadPool PXREF pxThreadPool);
// Internal function to process async calls
PXPrivate PXThreadResult PXOSAPI PXThreadPoolProcessASYNC(PXThreadPool PXREF pxThreadPool);

#endif
