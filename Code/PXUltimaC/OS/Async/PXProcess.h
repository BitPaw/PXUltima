#ifndef PXProcessINCLUDE
#define PXProcessINCLUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Time/PXTime.h>

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

typedef struct PXProcess_
{
	// Main Process
	PXProcessHandle ProcessHandle;
	PXProcessID ProcessID; // Context

	// Main Thread
	PXProcessThreadHandle ThreadHandle;
	PXProcessThreadID ThreadID;
	PXInt32U ThreadsAtStart;
	PXInt32U ThreadBasePriority;

	// Parent Process
	PXInt32U ProcessIDParent;

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

typedef void (PXAPI* PXProcessDetectedEvent)(PXProcess* const pxProcess);

PXPublic void PXAPI PXProcessConstruct(PXProcess* const pxProcess);
#if OSWindows
PXPublic void PXAPI PXProcessConstructFromHandle(PXProcess* const pxProcess, HANDLE processHandle);
#endif

PXPublic void PXAPI PXProcessCurrent(PXProcess* const pxProcess);
PXPublic void PXAPI PXProcessParent(PXProcess* const pxProcess);

PXPublic void PXAPI PXProcessExitCurrent(const PXInt32U exitCode);

// Get all open handles from given process.
// pxProcess can be NULL, current process is used. 
PXPublic PXActionResult PXAPI PXProcessHandleCountGet(PXProcess* pxProcess, PXSize* const handlesAmount);

PXPublic PXActionResult PXAPI PXProcessHandleListAll(PXProcess* pxProcess);


PXPublic PXActionResult PXAPI PXProcessCreate(PXProcess* const pxProcess, const PXText* const programmPath, const PXProcessCreationMode mode);

PXPublic PXActionResult PXAPI PXProcessListAll(PXProcessDetectedEvent pxProcessDetectedEvent);

PXPublic PXActionResult PXAPI PXProcessOpenViaID(PXProcess* const pxProcess, const PXProcessID pxProcessID);
PXPublic PXActionResult PXAPI PXProcessClose(PXProcess* const pxProcess);

PXPublic PXActionResult PXAPI PXProcessMemoryWrite(const PXProcess* const pxProcess, const void* const targetAdress, const void* const buffer, const PXSize bufferSize);
PXPublic PXActionResult PXAPI PXProcessMemoryRead(const PXProcess* const pxProcess, const void* const targetAdress, void* const buffer, const PXSize bufferSize);

PXPublic PXActionResult PXAPI PXProcessMemoryInfoFetch(PXProcessMemoryInfo* const pxProcessMemoryInfo);

#endif