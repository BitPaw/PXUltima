#include "PXDebug.h"

#if PXDebugUSE

#if OSUnix
#include <sys/ptrace.h>
#elif OSWindows

#include <Windows.h> // debugapi.h
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

#endif

#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXThread.h>
#include <OS/Library/PXLibrary.h>
#include <OS/File/PXFile.h>

#include <stdio.h>

#if OSUnix

#elif OSWindows

#endif

void PXAPI PXDebugConstruct(PXDebug* const pxDebug)
{
	PXClear(PXDebug, pxDebug);

	PXTextConstructFromAdressA(&pxDebug->ApplicatioName, pxDebug->ApplicatioNameBuffer, 0, sizeof(pxDebug->ApplicatioNameBuffer));
}

void PXAPI PXDebugDestruct(PXDebug* const pxDebug)
{

}

PXActionResult PXAPI PXDebugProcessBeeingDebugged(PXDebug* const pxDebug, PXBool* const isPresent)
{
#if OSUnix
#elif PXOSWindowsDestop
	BOOL debuggerPresent = 0;
	const BOOL result = CheckRemoteDebuggerPresent(pxDebug->Process.ProcessHandle, &debuggerPresent); // Windows XP, Kernel32.dll, debugapi.h
	const PXBool success = result != 0;

	PXActionOnErrorFetchAndReturn(!success);

	*isPresent = debuggerPresent;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif

	return PXActionSuccessful;
}

PXBool PXAPI PXDebugProcessBeeingDebuggedCurrent()
{
#if OSUnix
#elif OSWindows
	return IsDebuggerPresent(); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
}

void PXAPI PXDebugDebuggerSendMessage(PXDebug* const pxDebug, PXText* const message)
{
	switch (message->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix
#elif OSWindows
			OutputDebugStringA(message->TextA); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix
#elif OSWindows
			OutputDebugStringW(message->TextW); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
			break;
		}
	}
}

PXActionResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug)
{
#if OSUnix
#elif PXOSWindowsDestop
	PCSTR UserSearchPath = NULL;
	BOOL fInvadeProcess = TRUE;

	// Initializes the symbol handler for a process.
	const BOOL result = SymInitialize(pxDebug->Process.ThreadHandle, UserSearchPath, fInvadeProcess); // Dbghelp.dll, Dbghelp.h
	const PXBool success = result != 0;

	PXActionReturnOnError(!success);

	return PXActionSuccessful;

#endif
}

PXActionResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName)
{
	PXDebugConstruct(pxDebug);

	PXTextCopy(applicationName, &pxDebug->ApplicatioName);

	// Start Thread that will listen to given process.
	{
		const PXActionResult result = PXThreadRun(&pxDebug->EventListenLoop, (ThreadFunction)PXDebugLoop, pxDebug);

		// If thread cannot be started, stop.
		PXActionReturnOnError(result);
	}

	// Start process


	return PXActionSuccessful;
}

void PXAPI PXDebugContinue(PXDebug* const pxDebug)
{
#if OSUnix
#elif PXOSWindowsDestop
	const DWORD dwThreadId = 0;
	const DWORD dwContinueStatus = 0;
	const PXBool x = ContinueDebugEvent(pxDebug->Process.ProcessID, dwThreadId, dwContinueStatus); // Windows XP, Kernel32.dll, debugapi.h
	const PXBool isSicc = x != 0;

	//PXActionOnErrorFetchAndReturn(!isSicc);

#endif
}

void PXAPI PXDebugPause(PXDebug* const pxDebug)
{
#if OSUnix

#elif OSWindows
    DebugBreak(); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
}

PXBool PXAPI PXDebugPauseOther(PXDebug* const pxDebug, const PXProcessHandle pxProcessHandle)
{
#if OSUnix
    return PXFalse;
#elif PXOSWindowsDestop
    return DebugBreakProcess(pxProcessHandle); // Windows XP, Kernel32.dll, winbase.h
#endif
}

PXActionResult PXAPI PXDebugAttach(PXDebug* const pxDebug)
{
#if OSUnix
	const long result = ptrace(PTRACE_ATTACH, pxDebug->Process.ProcessID, 0, 0);

#elif PXOSWindowsDestop
	const BOOL result = DebugActiveProcess(pxDebug->Process.ProcessID);
	const PXBool sucessful = result != 0;

	PXActionOnErrorFetchAndReturn(!sucessful);

	return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXDebugDetach(PXDebug* const pxDebug)
{
#if OSUnix
	const long result = ptrace(PTRACE_DETACH, pxDebug->Process.ProcessID, 0, 0);

#elif PXOSWindowsDestop
	const BOOL sucessfulCode = DebugActiveProcessStop(pxDebug->Process.ProcessID);
	const PXBool sucessful = sucessfulCode != 0;

	PXActionOnErrorFetchAndReturn(!sucessful);

	return PXActionSuccessful;
#endif
}

void PXAPI PXDebugStackTrace(PXDebug* const pxDebug)
{
#if OSUnix


#elif PXOSWindowsDestop

	DWORD                          machineType = IMAGE_FILE_MACHINE_AMD64; // IMAGE_FILE_MACHINE_I386
	HANDLE                         hThread = GetCurrentThread();
	STACKFRAME                   stackFrame;
	CONTEXT                          contextRecord;
	PREAD_PROCESS_MEMORY_ROUTINE   readMemoryRoutine = 0;
	PFUNCTION_TABLE_ACCESS_ROUTINE functionTableAccessRoutine = 0;
	PGET_MODULE_BASE_ROUTINE       getModuleBaseRoutine = 0;
	PTRANSLATE_ADDRESS_ROUTINE     translateAddress = 0;




	// Prevent failure because not inizilized
	PXMemoryClear(&stackFrame, sizeof(STACKFRAME));
	PXMemoryClear(&contextRecord, sizeof(CONTEXT));

	RtlCaptureContext(&contextRecord);

	pxDebug->Process.ProcessHandle = GetCurrentProcess();

	DWORD64 displacement = 0;
	//stackFrame.AddrPC.Offset = contextRecord.Eip;
	//stackFrame.AddrPC.Mode = AddrModeFlat;
	//stackFrame.AddrStack.Offset = contextRecord.Esp;
	//stackFrame.AddrStack.Mode = AddrModeFlat;
	//stackFrame.AddrFrame.Offset = contextRecord.Ebp;
	//stackFrame.AddrFrame.Mode = AddrModeFlat;


	printf("+");

	for (size_t i = 0; i < 57; i++)
	{
		printf("-");
	}

	printf("+\n");
	printf("| %-55s |\n", "StackTrace");
	printf("+");

	for (size_t i = 0; i < 57; i++)
	{
		printf("-");
	}

	printf("+\n");

	for (PXSize frame = 0; ; ++frame)
	{
		const BOOL result = StackWalk
		(
			machineType,
			pxDebug->Process.ProcessHandle,
			hThread,
			&stackFrame,
			&contextRecord,
			NULL,
			SymFunctionTableAccess,
			SymGetModuleBase,
			NULL
		);
		const PXBool sucessfull = result != 0;

		if (!sucessfull)
		{
			break;
		}

#define PXMSDebugSymbolNameSize 268

		typedef union PXMSDebugSymbol_
		{
			IMAGEHLP_SYMBOL Symbol;
			PXByte Buffer[sizeof(IMAGEHLP_SYMBOL) + PXMSDebugSymbolNameSize];
		}
		PXMSDebugSymbol;

		PXMSDebugSymbol pxMSDebugSymbol;

		PXMemoryClear(&pxMSDebugSymbol, sizeof(PXMSDebugSymbol));
		pxMSDebugSymbol.Symbol.SizeOfStruct = sizeof(PXMSDebugSymbol);
		pxMSDebugSymbol.Symbol.MaxNameLength = PXMSDebugSymbolNameSize;

		const BOOL getResult = SymGetSymFromAddr(pxDebug->Process.ProcessHandle, (ULONG64)stackFrame.AddrPC.Offset, &displacement, &pxMSDebugSymbol.Symbol);
		const PXBool getFailed = getResult != 0;

		PXSize nameBufferSize = 512;
		PXByte nameBuffer[512];

		PXActionResult xxx = PXErrorCurrent();


		const DWORD  decResultSize = UnDecorateSymbolName(pxMSDebugSymbol.Symbol.Name, (PSTR)nameBuffer, nameBufferSize, UNDNAME_COMPLETE);

#if 0
		printf
		(
			"| %-55s() |\n"
			"|  PC address:     0x%08p |\n"
			"|  Stack address:  0x%08p |\n"
			"|  Frame address:  0x%08p |\n",
			pxMSDebugSymbol.Symbol.Name,
			(void*)stackFrame.AddrPC.Offset,
			(void*)stackFrame.AddrStack.Offset,
			(void*)stackFrame.AddrFrame.Offset
		);
#else
		char functioName[64];
		char adress[64];

		sprintf_s(functioName, 64, "%s()", pxMSDebugSymbol.Symbol.Name);
		sprintf_s(adress, 64, "0x%p", (void*)pxMSDebugSymbol.Symbol.Address);

		printf("| %-34s | %-18s |\n", functioName, adress);
#endif
	}


	printf("+");

	for (size_t i = 0; i < 57; i++)
	{
		printf("-");
	}

	printf("+\n");

#endif
}

PXSize PXAPI PXDebugMemoryRead(PXDebug* const pxDebug, const void* const adress, void* const outputData, const PXSize outputDataSize)
{
	PXProcess pxProcess;
	PXProcessConstruct(&pxProcess);

	pxProcess.ProcessID = pxDebug->Process.ProcessID;

	const PXSize readBytes = PXProcessMemoryRead(&pxProcess, adress, outputData, outputDataSize);

	return readBytes;
}

PXSize PXAPI PXDebugMemoryWrite(PXDebug* const pxDebug, const void* const adress, const void* const inputData, const PXSize inputDataSize)
{
	PXProcess pxProcess;
	PXProcessConstruct(&pxProcess);

	pxProcess.ProcessID = pxDebug->Process.ProcessID;

	const PXSize writtenBytes = PXProcessMemoryWrite(&pxProcess, adress, inputData, inputDataSize);

	return writtenBytes;
}

void PXAPI OnDebugProcessCreate(PXDebug* const pxDebug)
{

}
void PXAPI OnDebugProcessExit(PXDebug* const pxDebug, const PXInt32U exitCode)
{
	printf("[PXDebuger] Exit Thread <%i>\n", exitCode);
}

void PXAPI OnDebugThreadCreate(PXDebug* const pxDebug)
{

}
void PXAPI OnDebugThreadExit(PXDebug* const pxDebug, const PXInt32U exitCode)
{

}


void PXAPI OnDebugBreakPoint(PXDebug* const pxDebug)
{


}

PXActionResult PXAPI PXDebugWaitForEvent(PXDebug* const pxDebug)
{
#if OSUnix
#elif PXOSWindowsDestop

	DEBUG_EVENT debugEvent;
	DWORD dwMilliseconds = 0;
	DWORD dwContinueStatus = DBG_CONTINUE; // This flag need to be set for the debugger in this functiom

	// WaitForDebugEvent() Windows XP, Kernel32.dll, debugapi.h
	// WaitForDebugEventEx() Windows 10, Kernel32.dll, debugapi.h
	const BOOL result = WaitForDebugEvent(&debugEvent, 0);
	PXActionOnErrorFetchAndReturn(!result);

	switch (debugEvent.dwDebugEventCode) // Process the debugging event code.
	{
		case EXCEPTION_DEBUG_EVENT:
		{
			const EXCEPTION_DEBUG_INFO* const exceptionDdebugInfo = &debugEvent.u.Exception;
			const EXCEPTION_RECORD* const exceptionRecord = &exceptionDdebugInfo->ExceptionRecord;

			switch (exceptionRecord->ExceptionCode)
			{
				case EXCEPTION_ACCESS_VIOLATION:
					// First chance: Pass this on to the system.
					// Last chance: Display an appropriate error.

					printf("[PXDebuger] 0x%p | Memory access violation\n", (void*)exceptionRecord->ExceptionAddress);
					break;

				case EXCEPTION_BREAKPOINT:
					// First chance: Display the current instruction and register values.

					printf("[PXDebuger] 0x%p | BREAKPOINT\n", (void*)exceptionRecord->ExceptionAddress);
					break;

				case EXCEPTION_DATATYPE_MISALIGNMENT:
					// First chance: Pass this on to the system.
					// Last chance: Display an appropriate error.

					printf("[PXDebuger] EXCEPTION_DATATYPE_MISALIGNMENT\n");
					break;

				case EXCEPTION_SINGLE_STEP:
					// First chance: Update the display of the
					// current instruction and register values.

					printf("[PXDebuger] EXCEPTION_SINGLE_STEP\n");
					break;

				case DBG_CONTROL_C:
					// First chance: Pass this on to the system.
					// Last chance: Display an appropriate error.

					printf("[PXDebuger] DBG_CONTROL_C\n");
					break;

				default:
				{
					// Handle other exceptions.

					printf("[PXDebuger] Other exception\n");

					dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
					break;
				}

			}
			break;
		}
		case CREATE_THREAD_DEBUG_EVENT:
		{
			// As needed, examine or change the thread's registers
			// with the GetThreadContext and SetThreadContext functions;
			// and suspend and resume thread execution with the
			// SuspendThread and ResumeThread functions.

			const CREATE_THREAD_DEBUG_INFO* const createThreadDebugInfo = &debugEvent.u.CreateThread;

			printf("[PXDebuger] 0x%p | created Thread (%i) by Process (%i).\n", createThreadDebugInfo->lpStartAddress, debugEvent.dwThreadId, debugEvent.dwProcessId);



			ResumeThread(pxDebug->Process.ThreadHandle);

			break;
		}
		case CREATE_PROCESS_DEBUG_EVENT:
		{
			// As needed, examine or change the registers of the process's initial thread with the GetThreadContext and
			// SetThreadContext functions; read from and write to the process's virtual memory with the ReadProcessMemory and
			// WriteProcessMemory functions; and suspend and resume thread execution with the SuspendThread and ResumeThread
			// functions. Be sure to close the handle to the process image file with CloseHandle.

			const CREATE_PROCESS_DEBUG_INFO* const createProcessDebugInfo = &debugEvent.u.CreateProcessInfo;


			PXText pxText;
			PXTextConstructNamedBufferA(&pxText, fileNameBuffer, 256);

			PXFile file;
			file.ID = createProcessDebugInfo->hFile;

			const PXActionResult res = PXFileName(&file, &pxText);

			printf("[PXDebuger] Process (%i) create : %s\n", debugEvent.dwProcessId, pxText.TextA);

			break;
		}
		case EXIT_THREAD_DEBUG_EVENT:
		{
			// Display the thread's exit code.
			const EXIT_THREAD_DEBUG_INFO* const exitThreadDebugInfo = &debugEvent.u.ExitThread;

			OnDebugProcessExit(pxDebug, exitThreadDebugInfo->dwExitCode);

			break;
		}
		case EXIT_PROCESS_DEBUG_EVENT:
		{
			// Display the process's exit code.

			const EXIT_PROCESS_DEBUG_INFO* const exitProcessDebugInfo = &debugEvent.u.ExitProcess;

			printf("[PXDebuger] Exit process <%i>\n", exitProcessDebugInfo->dwExitCode);

			pxDebug->IsRunning = PXFalse;

			break;
		}
		case LOAD_DLL_DEBUG_EVENT:
		{
			// Read the debugging information included in the newly
			// loaded DLL. Be sure to close the handle to the loaded DLL
			// with CloseHandle.

			const LOAD_DLL_DEBUG_INFO* const loadDLLDebugInfo = &debugEvent.u.LoadDll;


			PXText pxText;
			PXTextConstructNamedBufferA(&pxText, fileNameBuffer, 256);

			PXFile file;
			file.ID = loadDLLDebugInfo->hFile;

			const PXActionResult res = PXFileName(&file, &pxText);

			printf("[PXDebuger] 0x%p | DLL load <%s>\n", loadDLLDebugInfo->lpBaseOfDll, pxText.TextA);
			break;
		}
		case UNLOAD_DLL_DEBUG_EVENT:
		{
			// Display a message that the DLL has been unloaded.

			const UNLOAD_DLL_DEBUG_INFO* const outputDebugStringInfo = &debugEvent.u.UnloadDll;

			printf("[PXDebuger] 0x%p | DLL unload : %s\n", outputDebugStringInfo->lpBaseOfDll, "???");

			break;
		}
		case OUTPUT_DEBUG_STRING_EVENT:
		{
			// Display the output debugging string.
			const OUTPUT_DEBUG_STRING_INFO* const outputDebugStringInfo = &debugEvent.u.DebugString;

			printf("[PXDebuger] OUTPUT_DEBUG_STRING_EVENT : ");

			if (outputDebugStringInfo->fUnicode)
			{
				printf("%s", outputDebugStringInfo->lpDebugStringData);
			}
			else
			{
				printf("%ls", (wchar_t*)outputDebugStringInfo->lpDebugStringData);
			}

			printf("\n");

			break;
		}
		case RIP_EVENT:
		{
			const RIP_INFO* const ripInfo = &debugEvent.u.RipInfo;

			printf("[PXDebuger] RIP_EVENT\n");

			break;
		}
		default:
		{
			dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
			break;
		}
	}

	dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;

	// Resume executing the thread that reported the debugging event.
	const BOOL succ = ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, dwContinueStatus);

	PXActionOnErrorFetchAndReturn(!succ);

	return PXActionSuccessful;

#endif
}

PXThreadResult PXAPI PXDebugLoop(PXDebug* const pxDebug)
{
	// Create process to debug on
	{
		const PXActionResult result = PXProcessCreate(&pxDebug->Process, &pxDebug->ApplicatioName, PXProcessCreationModeDebugProcessOnly);

		// If starting the process failed, stop.
		//PXActionReturnOnError(result);
		return -1;
	}

	pxDebug->IsRunning = PXTrue;

	while (pxDebug->IsRunning)
	{
		PXActionResult pxActionResult = PXDebugWaitForEvent(pxDebug);

		pxActionResult = PXActionInvalid;
	}

	return PXThreadSucessful;
}
#endif
