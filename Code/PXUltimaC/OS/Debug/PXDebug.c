#include "PXDebug.h"

#if OSUnix
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <execinfo.h>
#elif OSWindows

#include <Windows.h> // debugapi.h
#include <DbgHelp.h>

//#pragma comment(lib, "DbgHelp.lib")

#endif

#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXThread.h>
#include <OS/Library/PXLibrary.h>
#include <OS/File/PXFile.h>
#include <OS/Console/PXConsole.h>


#if OSWindows
typedef BOOL(WINAPI* PXDebugActiveProcessStop)(_In_ DWORD dwProcessId);
typedef BOOL(WINAPI* PXContinueDebugEvent)(_In_ DWORD dwProcessId, _In_ DWORD dwThreadId, _In_ DWORD dwContinueStatus);
typedef BOOL(WINAPI* PXWaitForDebugEvent)(_Out_ LPDEBUG_EVENT lpDebugEvent, _In_ DWORD dwMilliseconds);
typedef BOOL(WINAPI* PXDebugActiveProcess)(_In_ DWORD dwProcessId);
typedef VOID(WINAPI* PXDebugBreak)(VOID);
typedef VOID(WINAPI* PXOutputDebugStringA)(_In_opt_ LPCSTR lpOutputString);
typedef	VOID(WINAPI* PXOutputDebugStringW)(_In_opt_ LPCWSTR lpOutputString);
typedef BOOL(WINAPI* PXDebugBreakProcess)(_In_ HANDLE Process);
typedef BOOL(WINAPI* PXIsDebuggerPresent)(VOID);
typedef BOOL(WINAPI* PXCheckRemoteDebuggerPresent)(_In_ HANDLE hProcess, _Out_ PBOOL pbDebuggerPresent);

typedef BOOL(WINAPI* PXSymCleanup)(_In_ HANDLE hProcess);
typedef DWORD(WINAPI* PXSymLoadModule)(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll);
typedef BOOL(WINAPI* PXSymInitialize)(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
typedef BOOL(WINAPI* PXSymEnumSymbols)(_In_ HANDLE hProcess, _In_ ULONG64 BaseOfDll, _In_opt_ PCSTR Mask, _In_ PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback, _In_opt_ PVOID UserContext);
typedef BOOL(WINAPI* PXSymEnumSymbolsEx)(_In_ HANDLE hProcess, _In_ ULONG64 BaseOfDll, _In_opt_ PCSTR Mask, _In_ PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback, _In_opt_ PVOID UserContext, _In_ DWORD Options);
typedef PVOID(WINAPI* PXSymFunctionTableAccess)(__in HANDLE hProcess, __in DWORD AddrBase);
typedef DWORD(WINAPI* PXSymGetModuleBase)(__in HANDLE hProcess, __in DWORD dwAddr);


// Debug
typedef BOOL(WINAPI* PXStackWalk64)
(
	_In_ DWORD MachineType,
	_In_ HANDLE hProcess,
	_In_ HANDLE hThread,
	_Inout_ LPSTACKFRAME64 StackFrame,
	_Inout_ PVOID ContextRecord,
	_In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
	_In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
	_In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
	_In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress
	);

typedef DWORD(WINAPI* PXUnDecorateSymbolName)(_In_ PCSTR name, _Out_writes_(maxStringLength) PSTR outputString, _In_ DWORD maxStringLength, _In_ DWORD flags);
typedef BOOL(WINAPI* PXSymGetSymFromAddr64)(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_opt_ PDWORD64 pdwDisplacement, _Inout_ PIMAGEHLP_SYMBOL64  Symbol);

PXPrivate BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);

#endif


void PXAPI PXDebugConstruct(PXDebug* const pxDebug)
{
	PXClear(PXDebug, pxDebug);

	PXTextConstructFromAdressA(&pxDebug->ApplicatioName, pxDebug->ApplicatioNameBuffer, 0, sizeof(pxDebug->ApplicatioNameBuffer));
}

void PXAPI PXDebugDestruct(PXDebug* const pxDebug)
{
	PXLibraryClose(&pxDebug->LibraryKernel);
	PXLibraryClose(&pxDebug->LibraryDebugHelp);
}

PXActionResult PXAPI PXDebugProcessBeeingDebugged(PXDebug* const pxDebug, PXBool* const isPresent)
{
#if OSUnix
#elif PXOSWindowsDestop
	const PXCheckRemoteDebuggerPresent pxCheckRemoteDebuggerPresent = pxDebug->XCheckRemoteDebuggerPresent;

	BOOL debuggerPresent = 0;
	const BOOL result = pxCheckRemoteDebuggerPresent(pxDebug->Process.ProcessHandle, &debuggerPresent); // Windows XP, Kernel32.dll, debugapi.h
	const PXBool success = result != 0;

	PXActionOnErrorFetchAndReturn(!success);

	*isPresent = debuggerPresent;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif

	return PXActionSuccessful;
}

PXBool PXAPI PXDebugProcessBeeingDebuggedCurrent(PXDebug* const pxDebug)
{
#if OSUnix
#elif OSWindows
	const PXIsDebuggerPresent pxIsDebuggerPresent = (PXIsDebuggerPresent)pxDebug->XIsDebuggerPresent;
	const PXBool result = pxIsDebuggerPresent(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

	return result;
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
			const PXOutputDebugStringA pxOutputDebugStringA = (PXOutputDebugStringA)pxDebug->WOutputDebugStringA;

			pxOutputDebugStringA(message->TextA); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix
#elif OSWindows
			const PXOutputDebugStringA pxOutputDebugStringW = (PXOutputDebugStringA)pxDebug->WOutputDebugStringW;

			pxOutputDebugStringW(message->TextW); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
			break;
		}
	}
}

PXActionResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug)
{
#if OSUnix
#elif PXOSWindowsDestop

	// kernel32.dll
	{
		PXLibraryOpenA(&pxDebug->LibraryKernel, "KERNEL32.DLL");

		PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
		{
			{ &pxDebug->XContinueDebugEvent, "ContinueDebugEvent"},
			{ &pxDebug->XDebugBreak, "DebugBreak" },
			{ &pxDebug->XDebugBreakProcess, "DebugBreakProcess" },
			{ &pxDebug->XWaitForDebugEvent, "WaitForDebugEvent" },
			{ &pxDebug->WOutputDebugStringA, "OutputDebugStringA" },
			{ &pxDebug->WOutputDebugStringW, "OutputDebugStringW" },
			{ &pxDebug->XIsDebuggerPresent, "IsDebuggerPresent" },
			{ &pxDebug->XCheckRemoteDebuggerPresent, "CheckRemoteDebuggerPresent" },
			{ &pxDebug->XDebugActiveProcessStop, "DebugActiveProcessStop" }	
		};

		const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

		PXLibraryGetSymbolListA(&pxDebug->LibraryDebugHelp, pxLibraryFuntionEntryList, amount);
	}

	// Dbghelp.dll
	{
		PXLibraryOpenA(&pxDebug->LibraryDebugHelp, "DBGHELP.DLL"); 

		PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
		{
			{ &pxDebug->SymbolServerInitialize, "SymInitialize"},
			{ &pxDebug->XStackWalk64, "StackWalk64"},
			{ &pxDebug->XUnDecorateSymbolName, "UnDecorateSymbolName"},
			{ &pxDebug->XSymGetSymFromAddr64, "SymGetSymFromAddr64"},
			{ &pxDebug->XUnDecorateSymbolName, "UnDecorateSymbolName"},
			{ &pxDebug->SymbolEnumerate, "SymEnumSymbols" },
			{ &pxDebug->SymbolEnumerateEx, "SymEnumSymbolsEx" },
			{ &pxDebug->SymbolModuleLoad, "SymLoadModule" },
			{ &pxDebug->SymbolFunctionTableAccess, "SymFunctionTableAccess" },
			{ &pxDebug->SymbolGetModuleBase, "SymGetModuleBase" }
		};

		const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

		PXLibraryGetSymbolListA(&pxDebug->LibraryDebugHelp, pxLibraryFuntionEntryList, amount);
	}

	// Init symbol server
	{
		const PXSymInitialize pxSymbolServerInitialize = pxDebug->SymbolServerInitialize;

		PCSTR UserSearchPath = NULL;
		BOOL fInvadeProcess = TRUE;

		// Initializes the symbol handler for a process.
		const BOOL result = pxSymbolServerInitialize(pxDebug->Process.ThreadHandle, UserSearchPath, fInvadeProcess); // Dbghelp.dll, Dbghelp.h
		const PXBool success = result != 0;

		PXActionReturnOnError(!success);
	}

	return PXActionSuccessful;

#endif
}

PXActionResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName)
{
	PXDebugConstruct(pxDebug);

	PXTextCopy(applicationName, &pxDebug->ApplicatioName);

	// Start Thread that will listen to given process.
	{
		const PXActionResult result = PXThreadRun(&pxDebug->EventListenLoop, "PXDebugLoop", (ThreadFunction)PXDebugLoop, pxDebug);

		// If thread cannot be started, stop.
		PXActionReturnOnError(result);
	}

	// Start process


	return PXActionSuccessful;
}

PXActionResult PXAPI PXDebugContinue(PXDebug* const pxDebug)
{
	if(!pxDebug)
	{
		return PXActionRefusedArgumentNull;
	}

#if OSUnix

	const long ree = ptrace(PTRACE_CONT, pxDebug->Process.ProcessID, NULL, NULL);

	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const PXContinueDebugEvent pxContinueDebugEvent = pxDebug->XContinueDebugEvent;
	const DWORD dwThreadId = 0;
	const DWORD dwContinueStatus = 0;
	const PXBool x = pxContinueDebugEvent(pxDebug->Process.ProcessID, dwThreadId, dwContinueStatus); // Windows XP, Kernel32.dll, debugapi.h
	const PXBool isSicc = x != 0;

	//PXActionOnErrorFetchAndReturn(!isSicc);

	return PXActionSuccessful;

#else

	return PXActionRefusedNotImplemented;

#endif
}

PXActionResult PXAPI PXDebugPause(PXDebug* const pxDebug)
{
	if(!pxDebug)
	{
		return PXActionRefusedArgumentNull;
	}

#if OSUnix

	return PXActionRefusedNotImplemented;

#elif OSWindows
	const PXDebugBreak pxDebugBreak = pxDebug->XDebugBreak;

	pxDebugBreak(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

	return PXActionSuccessful; // We dont have any other result

#else

	return PXActionRefusedNotImplemented;

#endif
}

PXBool PXAPI PXDebugPauseOther(PXDebug* const pxDebug, const PXProcessHandle pxProcessHandle)
{
#if OSUnix
    return PXFalse;
#elif PXOSWindowsDestop
	const PXDebugBreakProcess pxDebugBreakProcess = pxDebug->XDebugBreakProcess;

    return pxDebugBreakProcess(pxProcessHandle); // Windows XP, Kernel32.dll, winbase.h
#endif
}

PXActionResult PXAPI PXDebugAttach(PXDebug* const pxDebug)
{
#if OSUnix
	const long result = ptrace(PTRACE_ATTACH, pxDebug->Process.ProcessID, 0, 0);

	ptrace(PTRACE_PEEKTEXT / PEEKDATA / PEEKUSER, pid, addr, 0);
	ptrace(PTRACE_POKETEXT / POKEDATA / POKEUSER, pid, addr, long_val);
	ptrace(PTRACE_GETREGS / GETFPREGS, pid, 0, &struct);
	ptrace(PTRACE_SETREGS / SETFPREGS, pid, 0, &struct);
	ptrace(PTRACE_GETREGSET, pid, NT_foo, &iov);
	ptrace(PTRACE_SETREGSET, pid, NT_foo, &iov);
	ptrace(PTRACE_GETSIGINFO, pid, 0, &siginfo);
	ptrace(PTRACE_SETSIGINFO, pid, 0, &siginfo);
	ptrace(PTRACE_GETEVENTMSG, pid, 0, &long_var);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_flags);


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

	const PXDebugActiveProcessStop pxDebugActiveProcessStop = pxDebug->XDebugActiveProcessStop;

	if(!pxDebugActiveProcessStop)
	{
		return PXActionRefusedNotSupported;
	}

	const BOOL sucessfulCode = pxDebugActiveProcessStop(pxDebug->Process.ProcessID);
	const PXBool sucessful = sucessfulCode != 0;

	PXActionOnErrorFetchAndReturn(!sucessful);

	return PXActionSuccessful;
#endif
}

void PXAPI PXDebugStackTrace(PXDebug* const pxDebug)
{
#if OSUnix
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
        printf("%s\n", strings[i]);

    free(strings);

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
	PXClear(STACKFRAME , &stackFrame);
	PXClear(CONTEXT, &contextRecord);

	RtlCaptureContext(&contextRecord);

	pxDebug->Process.ProcessHandle = GetCurrentProcess();

	DWORD64 displacement = 0;
	//stackFrame.AddrPC.Offset = contextRecord.Eip;
	//stackFrame.AddrPC.Mode = AddrModeFlat;
	//stackFrame.AddrStack.Offset = contextRecord.Esp;
	//stackFrame.AddrStack.Mode = AddrModeFlat;
	//stackFrame.AddrFrame.Offset = contextRecord.Ebp;
	//stackFrame.AddrFrame.Mode = AddrModeFlat;

#if PXLogEnable
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
#endif


	const PXStackWalk64 pxStackWalk64 = pxDebug->XStackWalk64;

	for (PXSize frame = 0; ; ++frame)
	{
		const BOOL result = pxStackWalk64
		(
			machineType,
			pxDebug->Process.ProcessHandle,
			hThread,
			&stackFrame,
			&contextRecord,
			NULL,
			pxDebug->SymbolFunctionTableAccess,
			pxDebug->SymbolGetModuleBase,
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

		PXClear(PXMSDebugSymbol, &pxMSDebugSymbol);
		pxMSDebugSymbol.Symbol.SizeOfStruct = sizeof(PXMSDebugSymbol);
		pxMSDebugSymbol.Symbol.MaxNameLength = PXMSDebugSymbolNameSize;

		const PXSymGetSymFromAddr64 pXSymGetSymFromAddr64 = pxDebug->XSymGetSymFromAddr64;
		const BOOL getResult = pXSymGetSymFromAddr64(pxDebug->Process.ProcessHandle, (ULONG64)stackFrame.AddrPC.Offset, &displacement, &pxMSDebugSymbol.Symbol);
		const PXBool getFailed = getResult != 0;

		PXSize nameBufferSize = 512;
		PXByte nameBuffer[512];

		PXActionResult xxx = PXErrorCurrent();

		const PXUnDecorateSymbolName pxUnDecorateSymbolName = pxDebug->XUnDecorateSymbolName;
		const DWORD  decResultSize = pxUnDecorateSymbolName(pxMSDebugSymbol.Symbol.Name, (PSTR)nameBuffer, nameBufferSize, UNDNAME_COMPLETE);

#if PXLogEnable
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
#endif
	}

#if PXLogEnable
	printf("+");

	for (size_t i = 0; i < 57; i++)
	{
		printf("-");
	}

	printf("+\n");
#endif

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
#if PXLogEnable 
	PXLogPrint
	(
		PXLoggingInfo,
		"Debugger",
		"Process",
		"Exit Thread <%i>",
		exitCode
	);
#endif
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

	const long result = ptrace(PTRACE_GETEVENTMSG, pxDebug->Process.ProcessID, ); // since Linux 2.5.46

	int waitStatus = 0;
	pid_t processID = waitpid(pxDebug->Process.ProcessID, &waitStatus, __WALL);
	PXBool success = -1 != processID;

	if(!success)
	{
		PXActionResult pxActionResult = PXErrorCurrent();

		return pxActionResult;
	}


#elif PXOSWindowsDestop
	const PXWaitForDebugEvent pxWaitForDebugEvent = pxDebug->XWaitForDebugEvent;

	DEBUG_EVENT debugEvent;
	DWORD dwMilliseconds = 0;
	DWORD dwContinueStatus = DBG_CONTINUE; // This flag need to be set for the debugger in this functiom

	// WaitForDebugEvent() Windows XP, Kernel32.dll, debugapi.h
	// WaitForDebugEventEx() Windows 10, Kernel32.dll, debugapi.h
	const BOOL result = pxWaitForDebugEvent(&debugEvent, 0); // Windows XP, Kernel32.dll, debugapi.h
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
				{
					// First chance: Pass this on to the system.
					// Last chance: Display an appropriate error.

#if PXLogEnable 
					PXLogPrint
					(
						PXLoggingEvent,
						"Debugger",
						"Exception",
						"Memory access violation at 0x%p",
						(void*)exceptionRecord->ExceptionAddress
					);
#endif

					break;
				}
				case EXCEPTION_BREAKPOINT:
				{
					// First chance: Display the current instruction and register values.

#if PXLogEnable 
					PXLogPrint
					(
						PXLoggingEvent,
						"Debugger",
						"Exception",
						"Breakpoint at 0x%p",
						(void*)exceptionRecord->ExceptionAddress
					);
#endif

					break;
				}
				case EXCEPTION_DATATYPE_MISALIGNMENT:
				{
					// First chance: Pass this on to the system.
					// Last chance: Display an appropriate error.

#if PXLogEnable 
					PXLogPrint
					(
						PXLoggingEvent,
						"Debugger",
						"Exception",
						"EXCEPTION_DATATYPE_MISALIGNMENT at 0x%p",
						(void*)exceptionRecord->ExceptionAddress
					);
#endif

					break;
				}
				case EXCEPTION_SINGLE_STEP:
				{
					// First chance: Update the display of the
					// current instruction and register values.

#if PXLogEnable 
					PXLogPrint
					(
						PXLoggingEvent,
						"Debugger",
						"Exception",
						"EXCEPTION_SINGLE_STEP at 0x%p",
						(void*)exceptionRecord->ExceptionAddress
					);
#endif

					break;
				}
				case DBG_CONTROL_C:
				{
					// First chance: Pass this on to the system.
					// Last chance: Display an appropriate error.

#if PXLogEnable 
					PXLogPrint
					(
						PXLoggingEvent,
						"Debugger",
						"Exception",
						"DBG_CONTROL_C at 0x%p",
						(void*)exceptionRecord->ExceptionAddress
					);
#endif

					break;
				}
				default:
				{
					// Handle other exceptions.

#if PXLogEnable 
					PXLogPrint
					(
						PXLoggingEvent,
						"Debugger",
						"Exception",
						"Unkown at 0x%p",
						(void*)exceptionRecord->ExceptionAddress
					);
#endif

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

#if PXLogEnable 
			PXLogPrint
			(
				PXLoggingEvent,
				"Debugger",
				"Exception",
				"created Thread (%i) by Process (%i)",
				createThreadDebugInfo->lpStartAddress,
				debugEvent.dwThreadId,
				debugEvent.dwProcessId
			);
#endif

			PXThread pxThread;
			pxThread.ThreadHandle = pxDebug->Process.ThreadHandle;

			PXThreadResume(&pxThread);

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

#if PXLogEnable 
			printf("[PXDebuger] Process (%i) create : %s\n", debugEvent.dwProcessId, pxText.TextA);
#endif

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

#if PXLogEnable 
			printf("[PXDebuger] Exit process <%i>\n", exitProcessDebugInfo->dwExitCode);
#endif

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

#if PXLogEnable 
			printf("[PXDebuger] 0x%p | DLL load <%s>\n", loadDLLDebugInfo->lpBaseOfDll, pxText.TextA);
#endif

			break;
		}
		case UNLOAD_DLL_DEBUG_EVENT:
		{
			// Display a message that the DLL has been unloaded.

			const UNLOAD_DLL_DEBUG_INFO* const outputDebugStringInfo = &debugEvent.u.UnloadDll;

#if PXLogEnable 
			printf("[PXDebuger] 0x%p | DLL unload : %s\n", outputDebugStringInfo->lpBaseOfDll, "???");
#endif

			break;
		}
		case OUTPUT_DEBUG_STRING_EVENT:
		{
			// Display the output debugging string.
			const OUTPUT_DEBUG_STRING_INFO* const outputDebugStringInfo = &debugEvent.u.DebugString;

#if PXLogEnable 
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
#endif

			break;
		}
		case RIP_EVENT:
		{
			const RIP_INFO* const ripInfo = &debugEvent.u.RipInfo;

#if PXLogEnable 
			PXLogPrint
			(
				PXLoggingEvent,
				"Debugger",
				"Exception",
				"RIP_EVENT"
			);
#endif

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
	const PXContinueDebugEvent pxContinueDebugEvent = (PXContinueDebugEvent)pxDebug->XContinueDebugEvent;
	const BOOL succ = pxContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, dwContinueStatus);

	PXActionOnErrorFetchAndReturn(!succ);

	return PXActionSuccessful;

#endif
}


#if OSUnix
#elif PXOSWindowsDestop
BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	PXSymbolEnumerator* const pxSymbolEnumerator = (PXSymbolEnumerator*)UserContext;

	PXSymbol* pxSymbol = &pxSymbolEnumerator->SymbolList[pxSymbolEnumerator->Amount];
	pxSymbol->TypeIndex = pSymInfo->TypeIndex;
	pxSymbol->Index = pSymInfo->Index;
	pxSymbol->Size = pSymInfo->Size;
	pxSymbol->ModBase = pSymInfo->ModBase;
	pxSymbol->Flags = pSymInfo->Flags;
	pxSymbol->Address = pSymInfo->Address;
	pxSymbol->Register = pSymInfo->Register;
	pxSymbol->Scope = pSymInfo->Scope;
	pxSymbol->Type = (PXSymbolType)pSymInfo->Tag;

	PXTextCopyA(pSymInfo->Name, pSymInfo->NameLen, pxSymbol->Name, 64);

	++(pxSymbolEnumerator->Amount);

	return PXTrue;
}
#endif


PXActionResult PXAPI PXDebugLibrarySymbolsFetch(PXDebug* const pxDebug, const PXText* const libraryFilePath, PXSymbol* const pxSymbolList, PXSize* const amount)
{
#if OSUnix
	return PXFalse;

#elif PXOSWindowsDestop

	PXSymbolEnumerator pxSymbolEnumerator;
	pxSymbolEnumerator.SymbolList = 0;
	pxSymbolEnumerator.Amount = 0;
	pxSymbolEnumerator.Size = 128;

	PXNewList(PXSymbolEnumerator, pxSymbolEnumerator.Size, &pxSymbolEnumerator.SymbolList, PXNull);

	PXProcess pxProcess;
	PXProcessCurrent(&pxProcess);

	// Initialize
	{
		const PXSymInitialize pxSymInitialize = (PXSymInitialize)pxDebug->SymbolServerInitialize;
		const PXBool status = pxSymInitialize(pxProcess.ProcessHandle, PXNull, PXFalse); // DbgHelp.dll 5.1 or later 

		if(!status)
		{
			return PXActionFailedInitialization;
		}
	}

	const PXSymLoadModule pxSymbolModuleLoad = pxDebug->SymbolModuleLoad;
	const DWORD64 baseOfDll = pxSymbolModuleLoad // DbgHelp.dll 5.1 or later        SymLoadModuleEx, DbgHelp.dll 6.0 or later
	(
		pxProcess.ProcessHandle,
		PXNull,
		libraryFilePath->TextA,
		PXNull,
		0,
		0
	);

	// On load failure, cleanup and exit
	{
		const PXBool wasLoadingSuccessful = baseOfDll != 0;

		if(!wasLoadingSuccessful)
		{
			const PXSymCleanup pxSymCleanup = (PXSymCleanup)pxDebug->SymbolServerCleanup;
			const PXBool cleanupSuccess = pxSymCleanup(pxProcess.ProcessHandle); // DbgHelp.dll 5.1 or later 

			if(!cleanupSuccess)
			{
				return PXActionFailedCleanup;
			}

			return PXActionFailedModuleLoad;
		}
	}

	// Enumerate all symvols
	{
		// SymEnumSym, SymEnumerateSymbols64 is outdated?
		const PXSymEnumSymbols pxSymbolEnumerate = (PXSymEnumSymbols)pxDebug->SymbolEnumerate; // DbgHelp.dll 5.1 or later 
		const PXBool enumerateResult = pxSymbolEnumerate
		(
			pxProcess.ProcessHandle,
			baseOfDll,
			0,
			PXLibraryNameSymbolEnumerate,
			&pxSymbolEnumerator
		);

		if(!enumerateResult)
		{
			return PXActionFailedDataFetch;
		}
	}

	// Final cleanup
	{
		const PXSymCleanup pxSymCleanup = pxDebug->SymbolServerCleanup;
		const PXBool cleanupSuccess = pxSymCleanup(pxProcess.ProcessHandle); // DbgHelp.dll 5.1 or later 

		if(!cleanupSuccess)
		{
			return PXActionFailedCleanup;
		}
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXDebugLogMessage(PXText* const pxText)
{
	// because we cant trust the string to be null terminated, and
	// windows does not have a function that takes in a size, we
	// have to do it per character. It's a mess. Microsoft please fix

	for(PXSize i = 0; i < pxText->SizeUsed; ++i)
	{
		char buffer[4] = { pxText->TextA[i], 0, 0, 0 };

		OutputDebugStringA(buffer);
	}
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

	return PXActionSuccessful;
}
