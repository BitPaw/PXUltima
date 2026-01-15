#pragma once

#ifndef PXProcessIncluded
#define PXProcessIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/OS/Time/PXTime.h>

#if OSUnix
#include <unistd.h>
#include <pthread.h>
#elif OSWindows
#include <windows.h>
#endif

#if OSUnix
typedef unsigned int PXProcessHandle;
typedef __pid_t PXProcessID;
typedef unsigned int PXProcessThreadHandle;
typedef pthread_t PXProcessThreadID;
#elif OSWindows
typedef HANDLE PXProcessHandle;
typedef DWORD PXProcessID;
typedef HANDLE PXProcessThreadHandle;
typedef DWORD PXProcessThreadID;
#endif

typedef struct PXThread_ PXThread;
typedef struct PXDebug_ PXDebug;

typedef struct PXProcess_
{
    // Main Process
    PXProcessHandle ProcessHandle;
    PXProcessID ProcessID; // Context

    // Main Thread
    PXProcessThreadHandle ThreadHandle;
    PXProcessThreadID ThreadID;
    PXI32U ThreadsAtStart;
    PXI32U ThreadBasePriority;

    // Parent Process
    PXI32U ProcessIDParent;

    PXText ExecutableFilePath;
}
PXProcess;

typedef struct PXProcessMemoryInfo_
{
    PXTime ExecuteStartTime;
    PXTime ExecuteTimeUser;
    PXTime ExecuteTimeKernel;

    // RAM
    long PageReclaims; // soft page fault
    long PageFaults; // hard page fault
    long IOOperationRead;
    long IOOperationWrite;
    long ContextSwapVoluntary;
    long ContextSwapInvoluntary;


    PXSize PeakWorkingSetSize; // The peak working set size, in bytes.
    PXSize WorkingSetSize; // The current working set size, in bytes.
    PXSize QuotaPeakPagedPoolUsage; // The peak paged pool usage, in bytes.
    PXSize QuotaPagedPoolUsage; // The current paged pool usage, in bytes.
    PXSize QuotaPeakNonPagedPoolUsage; // The peak nonpaged pool usage, in bytes.
    PXSize QuotaNonPagedPoolUsage; // The peak value in bytes of the Commit Charge during the lifetime of this process.
    PXSize PagefileUsage;
    PXSize PeakPagefileUsage; // The peak value in bytes of the Commit Charge during the lifetime of this process.
    PXSize PrivateUsage; // Windows new value
}
PXProcessMemoryInfo;

typedef enum PXProcessCreationMode_
{
    PXProcessCreationModeInvalid,
    PXProcessCreationModeBREAKAWAY_FROM_JOB,
    PXProcessCreationModeDEFAULT_ERROR_MODE,
    PXProcessCreationModeNEW_CONSOLE,
    PXProcessCreationModeNEW_PROCESS_GROUP,
    PXProcessCreationModeWindowless,
    PXProcessCreationModePROTECTED_PROCESS,
    PXProcessCreationModePRESERVE_CODE_AUTHZ_LEVEL,
    PXProcessCreationModeSECURE_PROCESS,
    PXProcessCreationModeSEPARATE_WOW_VDM,
    PXProcessCreationModeSHARED_WOW_VDM,
    PXProcessCreationModeSuspended, // Process created but does not start.
    PXProcessCreationModeUNICODE_ENVIRONMENT,
    PXProcessCreationModeDebugProcessOnly, // Debug only this process, ignored
    PXProcessCreationModeDebugAll, // Debug this process and every child and more.
    PXProcessCreationModeDETACHED_PROCESS,
    PXProcessCreationModeEXTENDED_STARTUPINFO_PRESENT,
    PXProcessCreationModeINHERIT_PARENT_AFFINITY
}
PXProcessCreationMode;

typedef void (PXAPI* PXProcessDetectedEvent)(PXProcess PXREF pxProcess);

#if OSWindows
PXPublic void PXAPI PXProcessConstructFromHandle(PXProcess PXREF pxProcess, HANDLE processHandle);
#endif

PXPublic void PXAPI PXProcessCurrent(PXProcess PXREF pxProcess);
PXPublic void PXAPI PXProcessParent(PXProcess PXREF pxProcess);

PXPublic void PXAPI PXProcessExitCurrent(const PXI32U exitCode);

// Get all open handles from given process.
// pxProcess can be NULL, current process is used.
PXPublic PXResult PXAPI PXProcessHandleCountGet(PXProcess* pxProcess, PXSize PXREF handlesAmount);

PXPublic PXResult PXAPI PXProcessHandleListAll(PXDebug PXREF pxDebug, PXProcess* pxProcess);


PXPublic PXResult PXAPI PXProcessCreate(PXProcess PXREF pxProcess, const PXText PXREF programmPath, const PXProcessCreationMode mode);

PXPublic PXResult PXAPI PXProcessListAll(PXProcessDetectedEvent pxProcessDetectedEvent);

PXPublic PXResult PXAPI PXProcessThreadsListAll(PXProcess PXREF pxProcess, PXThread** pxThreadListRef, const PXSize amount, PXSize* resultSIze);

PXPublic PXResult PXAPI PXProcessOpenViaID(PXProcess PXREF pxProcess, const PXProcessID pxProcessID);
PXPublic PXResult PXAPI PXProcessClose(PXProcess PXREF pxProcess);

PXPublic PXResult PXAPI PXProcessMemoryInfoFetch(PXProcessMemoryInfo PXREF pxProcessMemoryInfo);

#endif
