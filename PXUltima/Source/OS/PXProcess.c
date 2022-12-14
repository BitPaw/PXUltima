#include "PXProcess.h"

#if OSUnix
#include <unistd.h>
#include <sys/resource.h>
#elif OSWindows
#include <processthreadsapi.h>
#include <psapi.h>
#endif

#include <Memory/PXMemory.h>

void PXProcessCurrent(PXProcess* const pxProcess)
{
#if OSUnix
	pxProcess->Context = 0;
	pxProcess->ID = getpid();
#elif OSWindows
	pxProcess->Context = GetCurrentProcess(); // Returns a pseudo handle to the current process. Its -1 but may change in feature versions.
	pxProcess->ID = GetProcessId(pxProcess->Context);
#endif
}

void PXProcessParent(PXProcess* const pxProcess)
{
#if OSUnix
	pxProcess->Context = 0;
	pxProcess->ID = getppid();
#elif OSWindows
	pxProcess->Context = 0;
	pxProcess->ID = 0;
#endif
}

PXActionResult PXProcessMemoryInfoFetch(PXProcessMemoryInfo* const pxProcessMemoryInfo)
{
	MemoryClear(pxProcessMemoryInfo, sizeof(PXProcessMemoryInfo));

#if OSUnixx
	const int who = RUSAGE_SELF;
	rusage rusageData;
	const int returnCode = getrusage(who, &rusageData);
	const PXBool success = returnCode == 0;

	PXActionOnErrorFetchAndExit(!success);

	{
		PXTime* pxTime = &pxProcessMemoryInfo->ExecuteTimeUser;
		pxTime->Second = rusageData.ru_utime.tv_sec;
		pxTime->Milliseconds = rusageData.ru_utime.tv_usec;
	}

	{
		PXTime* pxTime = &pxProcessMemoryInfo->ExecuteTimeKernel;
		pxTime->Second = rusageData.ru_stime.tv_sec;
		pxTime->Milliseconds = rusageData.ru_stime.tv_usec;
	}

	pxProcessMemoryInfo->PageReclaims = rusageData.ru_minflt;
	pxProcessMemoryInfo->PageFaults = rusageData.ru_majflt;
	pxProcessMemoryInfo->IOOperationRead = rusageData.ru_inblock;
	pxProcessMemoryInfo->IOOperationWrite = rusageData.ru_oublock;
	pxProcessMemoryInfo->ContextSwapVoluntary = rusageData.ru_nvcsw;
	pxProcessMemoryInfo->ContextSwapInvoluntary = rusageData.ru_nivcsw;

#elif OSWindows

	const HANDLE currentProcess = GetCurrentProcess();

	// Process memory info
	{
		const PXSize processMemoryCountersExSize = sizeof(PROCESS_MEMORY_COUNTERS_EX); // minimum: Windows XP SP2, SP1 does not support EX version

		PROCESS_MEMORY_COUNTERS_EX processMemoryCountersEx;

		MemoryClear(&processMemoryCountersEx, processMemoryCountersExSize);

		processMemoryCountersEx.cb = processMemoryCountersExSize;

		const BOOL success = GetProcessMemoryInfo // GetPerformanceInfo is without process?
		(
			currentProcess,
			&processMemoryCountersEx,
			&processMemoryCountersExSize
		);

		PXActionOnErrorFetchAndExit(!success);

		pxProcessMemoryInfo->PageFaults = processMemoryCountersEx.PageFaultCount;
		pxProcessMemoryInfo->PeakWorkingSetSize = processMemoryCountersEx.PeakWorkingSetSize;
		pxProcessMemoryInfo->WorkingSetSize = processMemoryCountersEx.WorkingSetSize;
		pxProcessMemoryInfo->QuotaPeakPagedPoolUsage = processMemoryCountersEx.QuotaPeakPagedPoolUsage;
		pxProcessMemoryInfo->QuotaPagedPoolUsage = processMemoryCountersEx.QuotaPagedPoolUsage;
		pxProcessMemoryInfo->QuotaPeakNonPagedPoolUsage = processMemoryCountersEx.QuotaPeakNonPagedPoolUsage;
		pxProcessMemoryInfo->QuotaNonPagedPoolUsage = processMemoryCountersEx.QuotaNonPagedPoolUsage;
		pxProcessMemoryInfo->PagefileUsage = processMemoryCountersEx.PagefileUsage;
		pxProcessMemoryInfo->PeakPagefileUsage = processMemoryCountersEx.PeakPagefileUsage;
		pxProcessMemoryInfo->PrivateUsage = processMemoryCountersEx.PrivateUsage;
	}

	// Additional fetch "process time"
	{
		SYSTEMTIME systemList[4];
		FILETIME timeStampList[4];

		const BOOL success = GetProcessTimes
		(
			currentProcess,
			&timeStampList[0], // creationTime
			&timeStampList[1], // exitTime: undefined if process is not finished
			&timeStampList[2], // kernelTime
			&timeStampList[3] // userTime
		);

		PXActionOnErrorFetchAndExit(!success);

		// Convert
		for (PXSize i = 0; i < 4u; i++)
		{
			SYSTEMTIME systemTime;

			const BOOL convert = FileTimeToSystemTime(&timeStampList[i], &systemList[i]); // expressed as the amount of time that has elapsed since midnight on January 1, 1601 at Greenwich, England.
		}


		{
			PXTime* pxTime = &pxProcessMemoryInfo->ExecuteStartTime;
			SYSTEMTIME* systemTime = &systemList[0];
			pxTime->Year = systemTime->wYear;
			pxTime->Month = systemTime->wMonth;
			pxTime->DayOfWeek = systemTime->wDayOfWeek;
			pxTime->Day = systemTime->wDay;
			pxTime->Hour = systemTime->wHour;
			pxTime->Minute = systemTime->wMinute;
			pxTime->Second = systemTime->wSecond;
			pxTime->Milliseconds = systemTime->wMilliseconds;
		}

		{
			PXTime* pxTime = &pxProcessMemoryInfo->ExecuteTimeUser;
			SYSTEMTIME* systemTime = &systemList[3];
			pxTime->Year = systemTime->wYear - 1601u;
			pxTime->Month = systemTime->wMonth - 1u;
			pxTime->DayOfWeek = 0;
			pxTime->Day = systemTime->wDay - 1u;
			pxTime->Hour = systemTime->wHour;
			pxTime->Minute = systemTime->wMinute;
			pxTime->Second = systemTime->wSecond;
			pxTime->Milliseconds = systemTime->wMilliseconds;
		}

		{
			PXTime* pxTime = &pxProcessMemoryInfo->ExecuteTimeKernel;
			SYSTEMTIME* systemTime = &systemList[2];
			pxTime->Year = systemTime->wYear - 1601u;
			pxTime->Month = systemTime->wMonth - 1u;
			pxTime->DayOfWeek = 0;
			pxTime->Day = systemTime->wDay - 1u;
			pxTime->Hour = systemTime->wHour;
			pxTime->Minute = systemTime->wMinute;
			pxTime->Second = systemTime->wSecond;
			pxTime->Milliseconds = systemTime->wMilliseconds;
		}
	}

#endif

	return PXActionSuccessful;
}
