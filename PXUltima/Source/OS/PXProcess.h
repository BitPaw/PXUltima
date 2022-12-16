#ifndef PXProcessINCLUDE
#define PXProcessINCLUDE

#include <Error/PXActionResult.h>
#include <Format/Type.h>
#include <Time/PXTime.h>



#if OSUnix
#include <unistd.h>
#elif OSWindows
#include <windows.h>
#endif

#if OSUnix
typedef __pid_t PXProcessID;
#elif OSWindows
typedef HANDLE PXProcessID;
#endif

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



	PXPublic void PXProcessCurrent(PXProcess* const pxProcess);
	PXPublic void PXProcessParent(PXProcess* const pxProcess);

	PXPublic PXActionResult PXProcessMemoryInfoFetch(PXProcessMemoryInfo* const pxProcessMemoryInfo);

#ifdef __cplusplus
}
#endif

#endif
