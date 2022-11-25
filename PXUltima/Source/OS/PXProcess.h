#ifndef PXProcessINCLUDE
#define PXProcessINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <Time/PXTime.h>



#if OSUnix
#include <unistd.h>
#elif OSWindows
#include <windows.h>
#endif

typedef
#if OSUnix
pid_t
#elif OSWindows
HANDLE
#endif
PXProcessID;

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXProcess_
	{
		unsigned int ID;
		PXProcessID Context;
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


		size_t PeakWorkingSetSize; // The peak working set size, in bytes.
		size_t WorkingSetSize; // The current working set size, in bytes.
		size_t QuotaPeakPagedPoolUsage; // The peak paged pool usage, in bytes.
		size_t QuotaPagedPoolUsage; // The current paged pool usage, in bytes.
		size_t QuotaPeakNonPagedPoolUsage; // The peak nonpaged pool usage, in bytes.
		size_t QuotaNonPagedPoolUsage; // The peak value in bytes of the Commit Charge during the lifetime of this process.
		size_t PagefileUsage;
		size_t PeakPagefileUsage; // The peak value in bytes of the Commit Charge during the lifetime of this process.
		size_t PrivateUsage; // Windows new value
	}
	PXProcessMemoryInfo;



	PXPublic void PXProcessCurrent(PXProcess* const pxProcess);
	PXPublic void PXProcessParent(PXProcess* const pxProcess);

	PXPublic ActionResult PXProcessMemoryInfoFetch(PXProcessMemoryInfo* const pxProcessMemoryInfo);

#ifdef __cplusplus
}
#endif

#endif
