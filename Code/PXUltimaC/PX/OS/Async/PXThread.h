#pragma once

#ifndef PXThreadIncluded
#define PXThreadIncluded
#include <PX/Engine/PXResource.h>
#include "PXProcess.h"

// Return IDs

#if OSUnix
#include <pthread.h>
#include <unistd.h>
#include <setjmp.h>
typedef void* PXThreadResult;
//typedef pthread_t PXThreadIDType;
#define PXThreadIDUnused 0  // Adress
#elif OSWindows
#include <windows.h>
typedef DWORD PXThreadResult;
//typedef HANDLE PXThreadIDType;
#define PXThreadIDUnused nullptr
#if OSWindowsXP
typedef struct IUnknown IUnknown;
#endif
#endif

//#define PXThreadSucessful (PXThreadResult)0
//#define PXThreadActionFailed (PXThreadResult)1

typedef PXThreadResult(PXOSAPI* ThreadFunction)(void PXREF data);


// PXThreadState

#define PXExecuteStateMask (0x00FF0000)
#define PXExecuteStateInvalid     0<<16 // Resource does not exist. Not created or deleted
#define PXExecuteStateReady       1<<16 // Resource is inizialized ready to be used
#define PXExecuteStateReserve     2<<16 // Resource is taken by a handler and will be executed
#define PXExecuteStateRunning     3<<16 // Resource is currently running
#define PXExecuteStateWaiting     4<<16 // Resource waits for another resource
#define PXExecuteStateSuspended   5<<16 // Resource 
#define PXExecuteStateFailed      6<<16
#define PXExecuteStateFinished    7<<16 // Resource is done

PXPublic const char* PXExecuteStateToString(const PXI32U behaviour);



// Windows : priority
// Linux : nice value
// A value indicating how important a thread is relavtive to
// others running. The schedulter uses this hint to give important
// processes a faster response time to act and slow down threads
// that are not as important to respond fast.
typedef enum PXThreadPriorityMode_
{
    PXThreadPriorityModeInvalid,

    PXThreadPriorityModeLowest, // Lowest possible priority
    PXThreadPriorityModeLow19,
    PXThreadPriorityModeLow18,
    PXThreadPriorityModeLow17,
    PXThreadPriorityModeLow16,
    PXThreadPriorityModeLow15,
    PXThreadPriorityModeLow14,
    PXThreadPriorityModeLow13,
    PXThreadPriorityModeLow12,
    PXThreadPriorityModeLow11,
    PXThreadPriorityModeLow10,
    PXThreadPriorityModeLow09,
    PXThreadPriorityModeLow08,
    PXThreadPriorityModeLow07,
    PXThreadPriorityModeLow06,
    PXThreadPriorityModeLow05,
    PXThreadPriorityModeLow04,
    PXThreadPriorityModeLow03,
    PXThreadPriorityModeLow02,
    PXThreadPriorityModeLow01,
    PXThreadPriorityModeLower,
    PXThreadPriorityModeNormal,
    PXThreadPriorityModeHigher,
    PXThreadPriorityModeHigh01,
    PXThreadPriorityModeHigh02,
    PXThreadPriorityModeHigh03,
    PXThreadPriorityModeHigh04,
    PXThreadPriorityModeHigh05,
    PXThreadPriorityModeHigh06,
    PXThreadPriorityModeHigh07,
    PXThreadPriorityModeHigh08,
    PXThreadPriorityModeHigh09,
    PXThreadPriorityModeHigh10,
    PXThreadPriorityModeHigh11,
    PXThreadPriorityModeHigh12,
    PXThreadPriorityModeHigh13,
    PXThreadPriorityModeHigh14,
    PXThreadPriorityModeHigh15,
    PXThreadPriorityModeHigh16,
    PXThreadPriorityModeHigh17,
    PXThreadPriorityModeHigh18,
    PXThreadPriorityModeHigh19,
    PXThreadPriorityModeHighest
}
PXThreadPriorityMode;



#define PXTaskExecuteDirect    (1 <<  4) // Do not cache this task, execute it directly as is
#define PXTaskExecuteSYNC      (1 <<  5) // Cache task and execute it from the main thread 
#define PXTaskExecuteASYNC     (1 <<  6) // Cache task and execute it in another thread
#define PXTaskExecuteLoop      (1 <<  7) // Shall this task repeat itself after executing successful?
#define PXTaskParameterRelease (1 <<  8) // Delete the parameter after the task finished, to remove transphere objects
#define PXTaskDepended         (1 <<  9) // Task depends on other resources, wait for them to finish
#define PXTaskTimeDecay        (1 << 10) // Is a given Task can't be executed until a spesific time, mark it s failed and dont try again.

typedef PXResult (PXAPI* PXThreadX1CallFunction)(void* objectAdress);
typedef PXResult (PXAPI* PXThreadX2CallFunction)(void* objectAdressA, void* objectAdressB);

typedef struct PXTask_
{
    PXECSInfo Info;

    union
    {
        // Task function that is to be executed
        PXThreadX1CallFunction FunctionX1Adress;
        PXThreadX2CallFunction FunctionX2Adress;
    };

    void* ArgumentObject1; // Parameter that is given into the function
    void* ArgumentObject2;

    int TimeStart; // Start time of the task. To know how much time has passed.
    int TimeDeadline;  // The targeted end time of the task. Can be a higher priority the less time is present

    PXResult FunctionReturnCode; // As this is a PX function call, we dont get a number back but this enum
}
PXTask;

void PXAPI PXTaskStateChange(PXTask PXREF pxTask, const PXI32U newState);
void PXAPI PXTaskStateChangeRemote(PXThreadPool* pxThreadPool, PXTask PXREF pxTask, const PXI32U newState);









#define PXThreadContextUse (1<<0)

typedef struct PXThreadContext32_
{
    PXByte ExtendedRegisters[512];
    //PXF32ING_SAVE_AREA PXF32Save;

    PXI32U Dr0;
    PXI32U Dr1;
    PXI32U Dr2;
    PXI32U Dr3;
    PXI32U Dr6;
    PXI32U Dr7;

    PXI32U SegGs;
    PXI32U SegFs;
    PXI32U SegEs;
    PXI32U SegDs;

    PXI32U EDI;
    PXI32U ESI;
    PXI32U EBX;
    PXI32U EDX;
    PXI32U ECX;
    PXI32U EAX;

    PXI32U EBP;
    PXI32U EIP;
    PXI32U SegCs;
    PXI32U EFlags;
    PXI32U ESP;
    PXI32U SegSs;
}
PXThreadContext32;

typedef struct PXThreadContext64_
{
    // Parameter adress for integers. What are they for`?
    PXI64U P1Home;
    PXI64U P2Home;
    PXI64U P3Home;
    PXI64U P4Home;
    PXI64U P5Home;
    PXI64U P6Home;

    PXI32U   MxCsr; // SSE - PXF32 unit flags


    // Code Segment registers.
    PXI16U  SegCs;
    PXI16U  SegDs;
    PXI16U  SegEs;
    PXI16U  SegFs;
    PXI16U  SegGs;
    PXI16U  SegSs; // Stack segment register.


    // General flags
    PXI32U   EFlags;

    // Debug register, Used for hardware breakpoints.
    PXI64U Dr0;
    PXI64U Dr1;
    PXI64U Dr2;
    PXI64U Dr3;
    // Debug status and control registers.
    PXI64U Dr6;
    PXI64U Dr7;

    // General-purpose registers.
    PXI64U RAX; // Accumulator
    PXI64U RBX; // Base register
    PXI64U RCX; // Counter register
    PXI64U RDX; // Data register 

    PXI64U RSI; // Source index register
    PXI64U RDI; // Destination index register

    PXI64U RBP; // Base pointer register.
    PXI64U RSP; // Stack pointer register.

    // Extended 64-Bit registers
    PXI64U R8;
    PXI64U R9;
    PXI64U R10;
    PXI64U R11;
    PXI64U R12;
    PXI64U R13;
    PXI64U R14;
    PXI64U R15;

    PXI64U RIP; // Instruction pointer register.

    union
    {
        // XMM_SAVE_AREA32 FltSave;
         //NEON128         Q[16];
        PXI64U       D[32];


        struct
        {
            PXI128S Header[2];
            PXI128S Legacy[8];
            PXI128S Xmm0;
            PXI128S Xmm1;
            PXI128S Xmm2;
            PXI128S Xmm3;
            PXI128S Xmm4;
            PXI128S Xmm5;
            PXI128S Xmm6;
            PXI128S Xmm7;
            PXI128S Xmm8;
            PXI128S Xmm9;
            PXI128S Xmm10;
            PXI128S Xmm11;
            PXI128S Xmm12;
            PXI128S Xmm13;
            PXI128S Xmm14;
            PXI128S Xmm15;
        };
        PXI32U           S[32];
    };
    PXI128S   VectorRegister[26];
    PXI64U VectorControl;
    PXI64U DebugControl;
    PXI64U LastBranchToRip;
    PXI64U LastBranchFromRip;
    PXI64U LastExceptionToRip;
    PXI64U LastExceptionFromRip;
}
PXThreadContext64;


// Current execution point of a thread.
// Used as a recoverypoint for fatal execution failure
// https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/x86-architecture
typedef struct PXThreadContext_
{
#if OSUnix
    jmp_buf POSIXJumpBuffer;
#elif OSWindows
    CONTEXT WindowsContext;
#endif

    union
    {
        PXThreadContext64 X64;
        PXThreadContext32 X86;
    };
}
PXThreadContext;


typedef struct PXThread_ PXThread;


#define PXThreadBehaviourDefault            0
#define PXThreadBehaviourCreateSuspended    (1<<8)
#define PXThreadStartOnAdd                  (1<<10)

typedef struct PXThreadCreateInfo_
{
    PXECSCreateInfo Info;

    // PXText ThreadName; // Is Name
    PXProcessHandle TargetProcessHandle;
    ThreadFunction ThreadFunction;
    void* Parameter;
    PXI32U Behaviour;
}
PXThreadCreateInfo;




PXPublic PXResult PXAPI PXThreadRegisterToECS();

PXPublic void PXAPI PXThreadDestruct(PXThread PXREF pxThread);

#if OSWindows
PXPublic void PXAPI PXThreadConstructFromHandle(PXThread PXREF pxThread, HANDLE threadHandle);
#endif


PXPublic PXResult PXAPI PXThreadCurrent(PXThread PXREF pxThread);
PXPublic PXI32U PXAPI PXThreadCurrentID();

PXPublic PXResult PXAPI PXThreadResume(PXThread PXREF pxThread);
PXPublic PXResult PXAPI PXThreadSuspend(PXThread PXREF pxThread);
PXPublic PXResult PXAPI PXThreadWait(PXThread PXREF pxThread);

PXPublic PXResult PXAPI PXThreadCPUCoreAffinitySet(PXThread PXREF pxThread, const PXI16U coreIndex);


// Causes the calling thread to yield execution to another
// thread that is ready to run on the current processor.
// The operating system selects the next thread to be executed.
// The function returns true if a yield was caused, otherwise the
// current thread proceeds execution and false is returned.
PXPublic PXResult PXAPI PXThreadYieldToOtherThreads();
//---------------------------------------------------------







// Create thread and start it if defined.
// The thread will clean itself up, yet you need to release the handle as a final step.
// "targetProcessHandle" can be NULL, will target own process
// "threadName" can be NULL, it helps for debugging
PXPublic PXResult PXAPI PXThreadCreate(PXThread** pxThreadREF, PXThreadCreateInfo PXREF pxThreadCreateInfo);

// Dont call this function if you dont have to.
// In C++ this can cause memory leaks as destructors might not be called.
PXPublic PXResult PXAPI PXThreadExitCurrent(const PXI32U exitCode);

// Causes the calling thread to yield execution to another
// thread that is ready to run on the current processor.
// The operating system selects the next thread to be executed.
// The function returns true if a yield was caused, otherwise the
// current thread proceeds execution and false is returned.
PXPublic PXResult PXAPI PXThreadYieldToOtherThreads();

PXPublic PXResult PXAPI PXThreadOpen(PXThread PXREF pxThread);


PXPublic PXResult PXAPI PXThreadPrioritySet(PXThread* pxThread, const PXThreadPriorityMode pxThreadPriorityMode);
PXPublic PXResult PXAPI PXThreadPriorityGet(PXThread* pxThread, PXThreadPriorityMode PXREF pxThreadPriorityMode);

// Change the current thread state to the wanted state if possible.
PXPublic PXResult PXAPI PXThreadStateChange(PXThread PXREF pxThread, const PXI32U pxThreadState);

PXPublic PXResult PXAPI PXThreadSleep(PXThread PXREF pxThread, const PXSize sleepTime);

PXPublic PXResult PXAPI PXThreadCurrentProcessorID(PXI32U PXREF processorID);

PXPublic PXResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText PXREF threadName);
PXPublic PXResult PXAPI PXThreadNameGet(PXDebug PXREF pxDebug, PXThread PXREF pxThread, PXText PXREF threadName);

#endif
