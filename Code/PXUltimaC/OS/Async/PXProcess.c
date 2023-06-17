#include "PXProcess.h"

#if OSUnix
#include <unistd.h>
#include <sys/resource.h>
#elif OSWindows
//#include <processthreadsapi.h> Not found in XP build
#include <psapi.h>
#include <tlhelp32.h>
#endif

#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXEvent.h>

void PXProcessConstruct(PXProcess* const pxProcess)
{
	PXMemoryClear(pxProcess, sizeof(PXProcess));
}

void PXProcessCurrent(PXProcess* const pxProcess)
{
	PXProcessConstruct(pxProcess);

#if OSUnix
	pxProcess->ThreadHandle = 0;
	pxProcess->ProcessID = getpid();
#elif OSWindows
	pxProcess->ProcessHandle = GetCurrentProcess(); // Returns a pseudo handle to the current process. Its -1 but may change in feature versions.
	pxProcess->ProcessID = GetProcessId(pxProcess->ProcessHandle);
#endif
}

void PXProcessParent(PXProcess* const pxProcess)
{
	PXProcessConstruct(pxProcess);

#if OSUnix
	pxProcess->ProcessHandle = 0;
	pxProcess->ProcessID = getppid();
#elif OSWindows
	pxProcess->ProcessHandle = 0;
	pxProcess->ProcessID = 0;
#endif
}

void PXProcessExitCurrent(const PXInt32U exitCode)
{
#if OSUnix
	return PXActionNotImplemented;

#elif OSWindows
	ExitProcess(exitCode);
#endif
}

PXActionResult PXProcessCreate(PXProcess* const pxProcess, const PXText* const programmPath, const PXProcessCreationMode mode)
{
	PXProcessConstruct(pxProcess);

	switch (programmPath->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix
			return PXActionNotImplemented;

#elif OSWindows
			STARTUPINFOA startupInfo;
			PROCESS_INFORMATION processInfo;

			PXMemoryClear(&startupInfo, sizeof(STARTUPINFOA));
			PXMemoryClear(&processInfo, sizeof(PROCESS_INFORMATION));

			startupInfo.cb = sizeof(STARTUPINFOA);

			const DWORD creationflags =
				DEBUG_ONLY_THIS_PROCESS |
				CREATE_NEW_CONSOLE |
				PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ;

			const PXBool success = CreateProcessA // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
			(
				programmPath->TextA,
				NULL,
				NULL,
				NULL,
				0,
				creationflags,
				NULL,
				NULL,
				&startupInfo,
				&processInfo
			);

			PXActionOnErrorFetchAndReturn(!success);

			pxProcess->ProcessHandle = processInfo.hProcess;
			pxProcess->ProcessID = processInfo.dwProcessId;
			pxProcess->ThreadHandle = processInfo.hThread;
			pxProcess->ThreadID = processInfo.dwThreadId;

#endif

			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix
			return PXActionNotImplemented;

#elif OSWindows
			STARTUPINFOW startupInfo;
			PROCESS_INFORMATION processInfo;

			PXMemoryClear(&startupInfo, sizeof(STARTUPINFOW));
			PXMemoryClear(&processInfo, sizeof(PROCESS_INFORMATION));

			startupInfo.cb = sizeof(STARTUPINFOW);

			const PXBool success = CreateProcessW // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
			(
				programmPath->TextW,
				NULL,
				NULL,
				NULL,
				0,
				DEBUG_PROCESS,
				NULL,
				NULL,
				&startupInfo,
				&processInfo
			);

			PXActionOnErrorFetchAndReturn(!success);

			pxProcess->ProcessHandle = processInfo.hProcess;
			pxProcess->ProcessID = processInfo.dwProcessId;
			pxProcess->ThreadHandle = processInfo.hThread;
			pxProcess->ThreadID = processInfo.dwThreadId;

#endif

			break;
		}
	}

	return PXActionSuccessful;
}

PXActionResult PXProcessListAll(PXProcessDetectedEvent pxProcessDetectedEvent)
{
#if OSUnix
	return PXActionNotImplemented;
#elif PXOSWindowsDestop
	if (!pxProcessDetectedEvent)
	{
		return PXActionRefusedMissingCallBack;
	}

	const DWORD flag = TH32CS_SNAPPROCESS;
	const DWORD processID = PXNull;
	const HANDLE snapshotHandle = CreateToolhelp32Snapshot(flag, processID); // Windows XP, Kernel32.dll, tlhelp32.h
	const PXBool success = snapshotHandle != INVALID_HANDLE_VALUE && snapshotHandle != ((HANDLE)(LONG_PTR)ERROR_BAD_LENGTH);
	PROCESSENTRY32W processEntryW;
	processEntryW.dwSize = sizeof(PROCESSENTRY32W);

	PXActionOnErrorFetchAndReturn(!success);

	PXBool successfulFetch = Process32First(snapshotHandle, &processEntryW);

	if (!successfulFetch)
	{
		return PXActionFailedDataFetch;
	}

	for ( ; successfulFetch; )
	{
		PXProcess pxProcess;
		pxProcess.ProcessHandle = PXNull;
		pxProcess.ProcessID = processEntryW.th32ProcessID;
		pxProcess.ThreadHandle = PXNull;
		pxProcess.ThreadID = PXNull;
		pxProcess.ThreadsAtStart = processEntryW.cntThreads;
		pxProcess.ThreadBasePriority = processEntryW.pcPriClassBase;
		pxProcess.ProcessIDParent = processEntryW.th32ParentProcessID;
		pxProcess.ExecutableFilePath;

		const PXSize executableFilePathSize = PXTextLengthW(processEntryW.szExeFile, MAX_PATH);

		PXTextConstructFromAdressW(&pxProcess.ExecutableFilePath, processEntryW.szExeFile, executableFilePathSize);

		InvokeEvent(pxProcessDetectedEvent, &pxProcess);

		successfulFetch = Process32Next(snapshotHandle, &processEntryW);
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProcessOpenViaID(PXProcess* const pxProcess, const PXProcessID pxProcessID)
{
	PXProcessConstruct(pxProcess);

#if OSUnix
	return PXActionNotImplemented;
#elif OSWindows
	const DWORD desiredAccess = PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION;
	const HANDLE processHandle = OpenProcess(desiredAccess, FALSE, pxProcessID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = processHandle != 0;

	PXActionOnErrorFetchAndReturn(!successful);

	pxProcess->ProcessID = pxProcessID;
	pxProcess->ProcessHandle = processHandle;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProcessClose(PXProcess* const pxProcess)
{
#if OSUnix
	return PXActionNotImplemented;
#elif OSWindows
	const BOOL successful = CloseHandle(pxProcess->ProcessHandle); // Windows 2000 (+UWP), Kernel32.dll, handleapi.h

	PXActionOnErrorFetchAndReturn(!successful);

	pxProcess->ProcessHandle = PXNull;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProcessMemoryWrite(const PXProcess* const pxProcess, const void* const targetAdress, const void* const buffer, const PXSize bufferSize)
{
#if OSUnix
	return PXActionNotImplemented;
#elif PXOSWindowsDestop
	SIZE_T numberOfBytesRead;

	const BOOL result = WriteProcessMemory // Windows XP, Kernel32.dll, memoryapi.h
	(
		pxProcess->ProcessHandle,
		(LPVOID)targetAdress,
		buffer,
		bufferSize,
		&numberOfBytesRead
	);

	PXActionOnErrorFetchAndReturn(!result);

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProcessMemoryRead(const PXProcess* const pxProcess, const void* const targetAdress, void* const buffer, const PXSize bufferSize)
{
#if OSUnix
	return PXActionNotImplemented;

#elif PXOSWindowsDestop
	SIZE_T numberOfBytesRead;

	const BOOL result = ReadProcessMemory // Windows XP, Kernel32.dll, memoryapi.h
	(
		pxProcess->ProcessHandle,
		targetAdress,
		buffer,
		bufferSize,
		&numberOfBytesRead
	);

	PXActionOnErrorFetchAndReturn(!result);

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProcessMemoryInfoFetch(PXProcessMemoryInfo* const pxProcessMemoryInfo)
{
	PXMemoryClear(pxProcessMemoryInfo, sizeof(PXProcessMemoryInfo));

#if OSUnix
	const int who = RUSAGE_SELF;
	struct rusage rusageData;
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
#if WindowsAtleastVista && 0 // minimum: Windows XP SP2, SP1 does not support EX version
		const DWORD processMemoryCountersSize = sizeof(PROCESS_MEMORY_COUNTERS_EX);
		PROCESS_MEMORY_COUNTERS_EX processMemoryCounters;
#else
		const DWORD processMemoryCountersSize = sizeof(PROCESS_MEMORY_COUNTERS);
		PROCESS_MEMORY_COUNTERS processMemoryCounters;
#endif
		PXMemoryClear(&processMemoryCounters, processMemoryCountersSize);
		processMemoryCounters.cb = processMemoryCountersSize;

		const PXBool success = GetProcessMemoryInfo // Windows XP (+UWP), Kernel32.dll, psapi.h
		(
			currentProcess,
			&processMemoryCounters,
			processMemoryCountersSize
		);

		PXActionOnErrorFetchAndReturn(!success);

		pxProcessMemoryInfo->PageFaults = processMemoryCounters.PageFaultCount;
		pxProcessMemoryInfo->PeakWorkingSetSize = processMemoryCounters.PeakWorkingSetSize;
		pxProcessMemoryInfo->WorkingSetSize = processMemoryCounters.WorkingSetSize;
		pxProcessMemoryInfo->QuotaPeakPagedPoolUsage = processMemoryCounters.QuotaPeakPagedPoolUsage;
		pxProcessMemoryInfo->QuotaPagedPoolUsage = processMemoryCounters.QuotaPagedPoolUsage;
		pxProcessMemoryInfo->QuotaPeakNonPagedPoolUsage = processMemoryCounters.QuotaPeakNonPagedPoolUsage;
		pxProcessMemoryInfo->QuotaNonPagedPoolUsage = processMemoryCounters.QuotaNonPagedPoolUsage;
		pxProcessMemoryInfo->PagefileUsage = processMemoryCounters.PagefileUsage;
		pxProcessMemoryInfo->PeakPagefileUsage = processMemoryCounters.PeakPagefileUsage;

#if WindowsAtleastVista && 0
		pxProcessMemoryInfo->PrivateUsage = processMemoryCounters.PrivateUsage;
#endif

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

		PXActionOnErrorFetchAndReturn(!success);

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
