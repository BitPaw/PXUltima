#ifndef PXDebugINLCUDE
#define PXDebugINLCUDE

#include <OS/System/PXOSVersion.h>

#define PXDebugUSE OSDeviceToUse == OSDeviceDestop
#if PXDebugUSE

#include <Media/PXType.h>
#include <Media/PXText.h>

#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>
#include <OS/Error/PXActionResult.h>

#include <OS/Window/PXWindow.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if OSWindows
	typedef BOOL(WINAPI* PXSymInitialize)(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
	typedef BOOL(WINAPI* PXContinueDebugEvent)(_In_ DWORD dwProcessId, _In_ DWORD dwThreadId, _In_ DWORD dwContinueStatus);
	typedef BOOL(WINAPI* PXWaitForDebugEvent)(_Out_ LPDEBUG_EVENT lpDebugEvent, _In_ DWORD dwMilliseconds);
	typedef BOOL(WINAPI* PXDebugActiveProcess)(_In_ DWORD dwProcessId);	
	typedef VOID(WINAPI* PXDebugBreak)(VOID);
	typedef VOID(WINAPI* PXOutputDebugStringA)(_In_opt_ LPCSTR lpOutputString);
	typedef	VOID(WINAPI* PXOutputDebugStringW)(_In_opt_ LPCWSTR lpOutputString);
	typedef BOOL(WINAPI* PXDebugBreakProcess)(_In_ HANDLE Process);
	typedef BOOL(WINAPI* PXIsDebuggerPresent)(VOID);
	typedef BOOL(WINAPI* PXCheckRemoteDebuggerPresent)(_In_ HANDLE hProcess, _Out_ PBOOL pbDebuggerPresent);

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
#endif

	typedef struct PXDebug_
	{
		PXLibrary LibraryKernel;
		PXLibrary LibraryDebugHelp;

		PXBool IsRunning;

		PXProcess Process;

		PXThread EventListenLoop;

		PXByte ApplicatioNameBuffer[260];
		PXText ApplicatioName;

#if OSWindows
		// Kernel	
		PXContinueDebugEvent XContinueDebugEvent;
		PXWaitForDebugEvent XWaitForDebugEvent;
		PXDebugActiveProcess XDebugActiveProcess;
		PXDebugBreak XDebugBreak;
		PXOutputDebugStringA WOutputDebugStringA;
		PXOutputDebugStringW WOutputDebugStringW;
		PXDebugBreakProcess XDebugBreakProcess;
		PXIsDebuggerPresent XIsDebuggerPresent;
		PXCheckRemoteDebuggerPresent XCheckRemoteDebuggerPresent;

		// Debug
		PXSymInitialize SymbolServerInitialize;
		PXStackWalk64 XStackWalk64;
		PXUnDecorateSymbolName XUnDecorateSymbolName;
		PXSymGetSymFromAddr64 XSymGetSymFromAddr64;
#endif
	}
	PXDebug;

	PXPublic void PXAPI PXDebugConstruct(PXDebug* const pxDebug);
	PXPublic void PXAPI PXDebugDestruct(PXDebug* const pxDebug);

	// VS Debugger functions-- maybe
	PXPublic PXActionResult PXAPI PXDebugProcessBeeingDebugged(PXDebug* const pxDebug, PXBool* const isPresent);
	PXPublic PXBool PXAPI PXDebugProcessBeeingDebuggedCurrent(PXDebug* const pxDebug);

	PXPublic void PXAPI PXDebugDebuggerSendMessage(PXDebug* const pxDebug, PXText* const message);


	PXPublic PXActionResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug);


	PXPublic PXActionResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName);

	// Enables a debugger to continue a thread that previously reported a debugging event.
	PXPublic void PXAPI PXDebugContinue(PXDebug* const pxDebug);
	PXPublic void PXAPI PXDebugPause(PXDebug* const pxDebug);
	PXPublic PXBool PXAPI PXDebugPauseOther(PXDebug* const pxDebug, const PXProcessHandle pxProcessHandle);
	PXPublic PXActionResult PXAPI PXDebugAttach(PXDebug* const pxDebug);
	//PXPublic void PXDebugAttachToDebugger(PXDebug* const pxDebug);
	PXPublic PXActionResult PXAPI PXDebugDetach(PXDebug* const pxDebug);

	PXPublic void PXAPI PXDebugStackTrace(PXDebug* const pxDebug);

	PXPublic PXSize PXAPI PXDebugMemoryRead(PXDebug* const pxDebug, const void* const adress, void* const outputData, const PXSize outputDataSize);
	PXPublic PXSize PXAPI PXDebugMemoryWrite(PXDebug* const pxDebug, const void* const adress, const void* const inputData, const PXSize inputDataSize);

	// Only the thread that created the process being debugged can call this function.
	PXPublic PXActionResult PXAPI PXDebugWaitForEvent(PXDebug* const pxDebug);

	PXPrivate PXThreadResult PXAPI PXDebugLoop(PXDebug* const pxDebug);

#ifdef __cplusplus
}
#endif

#endif
#endif