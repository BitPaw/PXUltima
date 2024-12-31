#include "PXDebug.h"


static PXDebug _PXGLOBALDebug;
static PXBool _PXGLOBALDebugEnable = 0;

#if OSUnix
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <execinfo.h>
#include <dlfcn.h>
#elif OSWindows

#include <Windows.h> // debugapi.h
//#include <cvconst.h> // #define _NO_CVCONST_H_  ???
#include <DbgHelp.h>
#include <winternl.h>
#include <psapi.h>
#include <TlHelp32.h>

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
typedef    VOID(WINAPI* PXOutputDebugStringW)(_In_opt_ LPCWSTR lpOutputString);
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


PXDebug* PXAPI PXDebugInstanceGet()
{
    if(!_PXGLOBALDebugEnable)
    {
        PXClear(PXDebug, &_PXGLOBALDebug);

        PXDebugDebuggerInitialize(&_PXGLOBALDebug);

        _PXGLOBALDebugEnable = PXTrue;
    }

    return &_PXGLOBALDebug;
}

void PXAPI PXDebugInstanceRelease(PXDebug* const pxDebug)
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
    const PXActionResult pxActionResult = PXErrorCurrent(result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

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
    PXClear(PXDebug, pxDebug);

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
            { &pxDebug->DBGStackWalk, "StackWalk64"},
            { &pxDebug->XUnDecorateSymbolName, "UnDecorateSymbolName"},
            { &pxDebug->SymbolFromAddress, "SymGetSymFromAddr64"},
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
    const PXBool continueResultID = pxContinueDebugEvent(pxDebug->Process.ProcessID, dwThreadId, dwContinueStatus); // Windows XP, Kernel32.dll, debugapi.h
    const PXActionResult continueResult = PXErrorCurrent(continueResultID);

    return continueResult;
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
    __pid_t processID = pxDebug->Process.ProcessID;
    const long result = ptrace(PTRACE_ATTACH, processID, 0, 0);

    //ptrace(PTRACE_PEEKTEXT, processID, addr, 0); // PTRACE_PEEKTEXT / PEEKDATA / PEEKUSER
    //ptrace(PTRACE_POKETEXT, processID, addr, long_val); // PTRACE_POKETEXT / POKEDATA / POKEUSER
    //ptrace(PTRACE_GETREGS, processID, 0, &struct); // PTRACE_GETREGS / GETFPREGS
    //ptrace(PTRACE_SETREGS, processID, 0, &struct); // PTRACE_SETREGS / SETFPREGS
    //ptrace(PTRACE_GETREGSET, processID, NT_foo, &iov);
    //ptrace(PTRACE_SETREGSET, processID, NT_foo, &iov);
    //ptrace(PTRACE_GETSIGINFO, processID, 0, &siginfo);
    //ptrace(PTRACE_SETSIGINFO, processID, 0, &siginfo);
    //ptrace(PTRACE_GETEVENTMSG, processID, 0, &long_var);
    //ptrace(PTRACE_SETOPTIONS, processID, 0, PTRACE_O_flags);


#elif PXOSWindowsDestop
    const BOOL result = DebugActiveProcess(pxDebug->Process.ProcessID);
    const PXActionResult pxActionResult = PXErrorCurrent(result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

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
        return PXActionRefusedNotSupportedByOperatingSystem;
    }

    const BOOL successful = pxDebugActiveProcessStop(pxDebug->Process.ProcessID);
    const PXActionResult pxActionResult = PXErrorCurrent(successful);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    return PXActionSuccessful;
#endif
}

void PXAPI PXDebugStackTrace(PXDebug* const pxDebug, PXSymbol* const pxSymbolList, const PXSize pxSymbolListAmount, const PXSize start, const PXSize depth)
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






    // Prevent failure because not inizilized



    // TODO: FIX

#if PXLogEnable && 0
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


#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Debugger",
        "StackTrace",
        "----- BEGIN -----"
    );
#endif


    const PXStackWalk64 pxStackWalk64 = pxDebug->DBGStackWalk;
    const PXSymGetSymFromAddr64 pXSymGetSymFromAddr64 = pxDebug->SymbolFromAddress;
    const HANDLE processHandle = GetCurrentProcess();
    //const HANDLE processHeapHandle = GetProcessHeap();
    const HANDLE hThread = GetCurrentThread();
    const BOOL symbolServerInitialize = SymInitialize(processHandle, PXNull, PXTrue);

    SymSetOptions(SYMOPT_DEBUG | SYMOPT_LOAD_ANYTHING | SYMOPT_LOAD_LINES);
    // SymLoadModuleEx(,);

    DWORD                          machineType = IMAGE_FILE_MACHINE_AMD64; // IMAGE_FILE_MACHINE_I386

    STACKFRAME                     stackFrame;
    CONTEXT                        contextRecord;
    PREAD_PROCESS_MEMORY_ROUTINE   readMemoryRoutine = 0;
    PFUNCTION_TABLE_ACCESS_ROUTINE functionTableAccessRoutine = 0;
    PGET_MODULE_BASE_ROUTINE       getModuleBaseRoutine = 0;
    PTRANSLATE_ADDRESS_ROUTINE     translateAddress = 0;

    PXClear(STACKFRAME, &stackFrame);
    PXClear(CONTEXT, &contextRecord);

    RtlCaptureContext(&contextRecord);
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Mode = AddrModeFlat;

#if OS32B
    stackFrame.AddrPC.Offset = contextRecord.Eip;
    stackFrame.AddrStack.Offset = contextRecord.Esp;
    stackFrame.AddrFrame.Offset = contextRecord.Ebp;
#elif OS64B
    stackFrame.AddrPC.Offset = contextRecord.Rip;
    stackFrame.AddrStack.Offset = contextRecord.Rsp;
    stackFrame.AddrFrame.Offset = contextRecord.Rbp;
#endif


    PXSize frame;
    PXSize symbolIndex = 0;

    for (frame = 0; frame < (start+depth); ++frame)
    {
        PXSymbol* pxSymbol = &pxSymbolList[symbolIndex];

        const BOOL result = pxStackWalk64
                            (
                                machineType,
                                processHandle,
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

        if(frame < start)
        {
            continue;
        }


        void* symbolAdress = (ULONG64)stackFrame.AddrPC.Offset; // ULONG64


        PXDebugSymbolReadFromAdress(pxDebug, pxSymbol, symbolAdress);

        ++symbolIndex;


#if PXLogEnable  && 0
        PXLogPrint
        (
            PXLoggingInfo,
            "Debugger",
            "StackTrace",
            "%c %-25s %10s 0x%p",
            (frame == 0  ? '/' : '|'),
            pxSymbol->NameUndecorated,
            pxSymbol->NameModule,
            symbolAdress
        );
#endif







        /*

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

        const BOOL getResult = pXSymGetSymFromAddr64(pxDebug->Process.ProcessHandle, (ULONG64)stackFrame.AddrPC.Offset, &displacement, &pxMSDebugSymbol.Symbol);
        const PXBool getFailed = getResult != 0;*/




#if PXLogEnable && 0
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


#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Debugger",
        "StackTrace",
        "%c %s %i Jumps",
        '^',
        "(START)",
        frame
    );
#endif

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Debugger",
        "StackTrace",
        "----- DONE -----"
    );
#endif



    // TODO: FIX

#if PXLogEnable && 0
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
    __pid_t processID = pxDebug->Process.ProcessID;

    // Trance
    {
        const long result = ptrace(PTRACE_GETEVENTMSG, processID, 0); // since Linux 2.5.46
        const PXActionResult tranceResult = PXErrorCurrent(-1 != result);

        if(PXActionSuccessful != tranceResult)
        {
            return tranceResult;
        }
    }

    // wait
    {
        int waitStatus = 0;
        pid_t waitedProcessID = waitpid(processID, &waitStatus, __WALL);
        const PXActionResult waitResult = PXErrorCurrent(-1 != waitedProcessID);

        if(PXActionSuccessful != waitResult)
        {
            return waitResult;
        }
    }

    return PXActionSuccessful;

#elif PXOSWindowsDestop
    const PXWaitForDebugEvent pxWaitForDebugEvent = pxDebug->XWaitForDebugEvent;

    DEBUG_EVENT debugEvent;
    DWORD dwMilliseconds = 0;
    DWORD dwContinueStatus = DBG_CONTINUE; // This flag need to be set for the debugger in this functiom

    // WaitForDebugEvent() Windows XP, Kernel32.dll, debugapi.h
    // WaitForDebugEventEx() Windows 10, Kernel32.dll, debugapi.h
    const BOOL result = pxWaitForDebugEvent(&debugEvent, 0); // Windows XP, Kernel32.dll, debugapi.h
    const PXActionResult pxActionResult = PXErrorCurrent(result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

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
        file.FileHandle = createProcessDebugInfo->hFile;

        const PXActionResult res = PXFileName(&file, &pxText);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            "Debugger",
            "Exception",
            "Process (%i) create : %s",
            debugEvent.dwProcessId,
            pxText.TextA
        );
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
        PXLogPrint
        (
            PXLoggingEvent,
            "Debugger",
            "Exception",
            "Exit process <%i>",
            exitProcessDebugInfo->dwExitCode
        );
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
        file.FileHandle = loadDLLDebugInfo->hFile;

        const PXActionResult res = PXFileName(&file, &pxText);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            "Debugger",
            "Exception",
            "0x%p | DLL load <%s>",
            loadDLLDebugInfo->lpBaseOfDll,
            pxText.TextA
        );
#endif

        break;
    }
    case UNLOAD_DLL_DEBUG_EVENT:
    {
        // Display a message that the DLL has been unloaded.

        const UNLOAD_DLL_DEBUG_INFO* const outputDebugStringInfo = &debugEvent.u.UnloadDll;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            "Debugger",
            "Exception",
            "0x%p | DLL unload : %s",
            outputDebugStringInfo->lpBaseOfDll,
            "???"
        );
#endif

        break;
    }
    case OUTPUT_DEBUG_STRING_EVENT:
    {
        // Display the output debugging string.
        const OUTPUT_DEBUG_STRING_INFO* const outputDebugStringInfo = &debugEvent.u.DebugString;

#if PXLogEnable && 0
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
    const BOOL continueResult = pxContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, dwContinueStatus);
    const PXActionResult pxContinueActionResult = PXErrorCurrent(continueResult);

    if(PXActionSuccessful != pxContinueActionResult)
    {
        return pxContinueActionResult;
    }

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
    pxSymbol->ObjectSize = pSymInfo->Size;
    pxSymbol->ModuleAdress = pSymInfo->ModBase;
    pxSymbol->Flags = pSymInfo->Flags;
    pxSymbol->Address = pSymInfo->Address;
    pxSymbol->Register = pSymInfo->Register;
    pxSymbol->Scope = pSymInfo->Scope;
    pxSymbol->Type = (PXSymbolType)pSymInfo->Tag;

    PXTextCopyA(pSymInfo->Name, pSymInfo->NameLen, pxSymbol->NameSymbol, 64);

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

            return PXActionFailedLoad;
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
            return PXActionInvalid;
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

#if OSUnix
        // Does this exist?
#elif OSWindows
        OutputDebugStringA(buffer);
#endif
    }
}

PXActionResult PXAPI PXDebugSymbolReadFromAdress(PXDebug* const pxDebug, struct PXSymbol_* const pxSymbol, void* adress)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    PXClear(PXSymbol, pxSymbol);

    const HANDLE processHandle = GetCurrentProcess(); // TODO: what if we want another process?

    // Extract symbol
    {
        PDWORD64 displacement = 0;

        union
        {
            char buffer[sizeof(SYMBOL_INFO) + sizeof(TCHAR) * 200]; // MAX_SYM_NAME
            SYMBOL_INFO SymbolInfo;
        }
        symbolInfo;


        symbolInfo.SymbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
        symbolInfo.SymbolInfo.MaxNameLen = 200;

        const PXBool symbolFetchSuccess = SymFromAddr(processHandle, adress, &displacement, &symbolInfo.SymbolInfo);
        const PXActionResult symbolFetchResult = PXErrorCurrent(symbolFetchSuccess);

        /*
        BOOL asas = SymGetTypeInfo
        (
          processHandle,
          [in]  DWORD64                   ModBase,
          [in]  ULONG                     TypeId,
          [in]  IMAGEHLP_SYMBOL_TYPE_INFO GetType,
          [out] PVOID                     pInfo
        );
        */

        /*
        if(!symbolFetchSuccess)
        {
            // Try again but with another functiuon

            IMAGEHLP_LINE64 lineInfo;
            lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            lineInfo.FileName = &symbolInfo;

            const PXBool symbolFetchSuccessW = SymGetLineFromAddr64(processHandle, adress, &displacement, &lineInfo);
            const PXActionResult symbolFetchSuccessWRes = PXWindowsErrorCurrent(symbolFetchSuccess);


            IMAGEHLP_SYMBOL64* imageHelpSYMBOL = &symbolInfo; // function avove failed, reuse memory
            imageHelpSYMBOL->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            imageHelpSYMBOL->MaxNameLength = 2000;

            const PXBool symbolFetchSuccessB = SymGetSymFromAddr64(processHandle, adress, &displacement, imageHelpSYMBOL);

            return PXActionInvalid;
        }*/


        //  SYM_TYPE xx = symbolInfo.SymbolInfo.TypeIndex;
        // SymTagEnum xxxx = symbolInfo->Tag;


        const PXUnDecorateSymbolName pxUnDecorateSymbolName = pxDebug->XUnDecorateSymbolName;
        const DWORD  decResultSize = pxUnDecorateSymbolName(symbolInfo.SymbolInfo.Name, (PSTR)pxSymbol->NameUndecorated, 64, UNDNAME_COMPLETE);


        pxSymbol->Amount = 1;
        pxSymbol->ObjectSize = symbolInfo.SymbolInfo.Size;
        pxSymbol->ModuleAdress = symbolInfo.SymbolInfo.ModBase;
        pxSymbol->SymbolAdress = symbolInfo.SymbolInfo.Address;

        PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);


        // Index
        {
            const PXBool succca = SymFromIndex(processHandle, pxSymbol->ModuleAdress, symbolInfo.SymbolInfo.Index, &symbolInfo.SymbolInfo);

            if(succca)
            {
                // override
                PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);

                char bufferB[64];
                IMAGEHLP_LINE64 imageHelperLine;
                imageHelperLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                imageHelperLine.FileName = bufferB;

                const PXBool lineResult = SymGetLineFromAddr(processHandle, symbolInfo.SymbolInfo.Address, &displacement, &imageHelperLine);
                char* fileNameAdress = imageHelperLine.FileName;
                PXSize fileNameAdressSize = 0;

                if(lineResult)
                {
                    const PXSize index = PXTextFindLastCharacterA(imageHelperLine.FileName, -1, '\\');

                    if(-1 != index)
                    {
                        fileNameAdress += index + 1;
                        fileNameAdressSize = PXTextLengthA(fileNameAdress, 64);
                    }

                    pxSymbol->LineNumber = imageHelperLine.LineNumber;
                }

                PXTextCopyA(fileNameAdress, -1, pxSymbol->NameFile, 64);

            }
        }
    }
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXDebugHeapMemoryList(PXDebug* const pxDebug)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    const HANDLE processHandle = GetProcessHeap();

    const BOOL ursiu = SymInitialize(processHandle, PXNull, PXTrue);

    HEAP_SUMMARY heapSUMMARY;
    heapSUMMARY.cb = sizeof(HEAP_SUMMARY);

    HeapSummary(processHandle, 0, &heapSUMMARY);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        "Debugger",
        "Heap",
        "\n"
        " %20s : %20i B\n"
        " %20s : %20i B\n"
        " %20s : %20i B",
        "Allocated", heapSUMMARY.cbAllocated,
        "Committed", heapSUMMARY.cbCommitted,
        "MaxReserve", heapSUMMARY.cbMaxReserve
    );
#endif


#if 1

    PROCESS_HEAP_ENTRY processHeapEntry;
    PXClear(PROCESS_HEAP_ENTRY, &processHeapEntry);

    // Windows XP, Kernel32.dll, heapapi.h
    for(PXSize i = 0; HeapWalk(processHandle, &processHeapEntry); ++i)
    {


        PXBool isBusy = PROCESS_HEAP_ENTRY_BUSY & processHeapEntry.wFlags;
        PXBool isShare = PROCESS_HEAP_ENTRY_DDESHARE & processHeapEntry.wFlags;
        PXBool ismovable = PROCESS_HEAP_ENTRY_MOVEABLE & processHeapEntry.wFlags;
        PXBool isRegion = PROCESS_HEAP_REGION & processHeapEntry.wFlags;
        PXBool isUncommited = PROCESS_HEAP_UNCOMMITTED_RANGE & processHeapEntry.wFlags;

        void* a = processHeapEntry.lpData;
        char* target = isRegion ? processHeapEntry.Region.lpFirstBlock : processHeapEntry.Block.hMem;

        // target += processHeapEntry.cbOverhead;


        // As we have a memory space-adress here, we cant use the
        // windows symbol lookup, because they exist in a whole other memory region
        // To solve this we have a seperate lookup



        //_malloc_dbg




        if(isRegion)
        {
#if PXLogEnable   && 0
            PXLogPrint
            (
                PXLoggingEvent,
                "Debugger",
                "Heap",
                "0x%p %6s - %8i B : 0x%p-0x%p Commit: %i B/ %i B",
                processHeapEntry.lpData,
                "REGION",
                processHeapEntry.cbData,
                processHeapEntry.Region.lpFirstBlock,
                processHeapEntry.Region.lpLastBlock,
                processHeapEntry.Region.dwCommittedSize,
                processHeapEntry.Region.dwUnCommittedSize,
                "Unkown"
            );
#endif
        }
        else
        {
            char buffer[260];
            void* ObjectInformation = buffer;
            ULONG ObjectInformationLength = 260;
            ULONG ReturnLength = 0;

            const NTSTATUS oikasdo = NtQueryObject
            (
                processHeapEntry.Block.hMem,
                ObjectBasicInformation,
                ObjectInformation,
                ObjectInformationLength,
                &ReturnLength
            );

            target = (char*)processHeapEntry.lpData;// +(int)processHeapEntry.cbOverhead;


            PXSymbol pxSymbol;

            const PXActionResult symbolFetchResult = PXMemorySymbolFetch(target, &pxSymbol);
            const PXBool success = PXActionSuccessful == symbolFetchResult;

            char symbolInfo[128];

            if(success)
            {
                PXTextPrintA
                (
                    symbolInfo,
                    128,
                    "%s::%s::%s::%i ~ %ix %i B",
                    pxSymbol.NameModule,
                    pxSymbol.NameFile,
                    pxSymbol.NameSymbol,
                    pxSymbol.LineNumber,
                    pxSymbol.Amount,
                    pxSymbol.ObjectSize
                );

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingEvent,
                    "Debugger",
                    "Heap",
                    "0x%p %6s - %8i B + OS: %3i B, - %s",
                    processHeapEntry.lpData,
                    "BLOCK",
                    processHeapEntry.cbData,
                    (int)processHeapEntry.cbOverhead,
                    symbolInfo
                );
#endif
            }
            else
            {
                PXTextPrintA
                (
                    symbolInfo,
                    128,
                    "???"
                );
            }




        }

    }


#else


    const HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, GetCurrentProcessId());
    const PXBool heapSnapSuccess = INVALID_HANDLE_VALUE != hHeapSnap;

    HEAPLIST32 heapList;
    PXClear(HEAPLIST32, &heapList);
    heapList.dwSize = sizeof(HEAPLIST32);

    if(!heapSnapSuccess)
    {
        // GetLastError())

        return PXActionInvalid;
    }


    // Fetch first
    {
        const PXBool firstFetch = Heap32ListFirst(hHeapSnap, &heapList);

        if(!firstFetch)
        {
            return PXActionInvalid;
        }
    }




    do
    {
        HEAPENTRY32 he;
        ZeroMemory(&he, sizeof(HEAPENTRY32));
        he.dwSize = sizeof(HEAPENTRY32);

        if(Heap32First(&he, GetCurrentProcessId(), heapList.th32HeapID))
        {
            printf("Heap ID: %d\n", heapList.th32HeapID);

            do
            {


                PXSymbol pxSymbol;

                void* adress = he.dwAddress;

                PXDebugFetchSymbolFromRougeAdress(pxDebug, &pxSymbol, adress);


                printf("0x%p - %6i B   %s\n", adress, he.dwBlockSize, pxSymbol.NameUndecorated);


                he.dwSize = sizeof(HEAPENTRY32);
            }

            while(Heap32Next(&he));
        }

        heapList.dwSize = sizeof(HEAPLIST32);
    }
    while(Heap32ListNext(hHeapSnap, &heapList));


    CloseHandle(hHeapSnap);
#endif


#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDebugFetchSymbolThread(PXDebug* const pxDebug, PXSymbol* const pxSymbol, PXThread* pxThread)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    // if we dont have a handle or even an ID, we cant proceed
    const PXBool hasAtlestOne = pxThread->ThreadID || pxThread->ThreadHandle;

    if(!hasAtlestOne)
    {
        return PXActionInvalid;
    }


    void* adress = 0;

    // create thread handle if not exists
    if(!pxThread->ThreadHandle)
    {
        pxThread->ThreadHandle = OpenThread(THREAD_ALL_ACCESS, PXFalse, pxThread->ThreadID);
    }

    const NTSTATUS fdgx = NtQueryInformationThread
                          (
                              pxThread->ThreadHandle,
                              9, // ThreadQuerySetWin32StartAddress // undocumented magic number?
                              &adress,
                              sizeof(void*),
                              PXNull
                          );

    const PXActionResult symbolResult = PXDebugFetchSymbolFromRougeAdress(pxDebug, pxSymbol, adress);

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

#if OSWindows
BOOL CALLBACK PXPSYM_ENUMLINES_CALLBACK(PSRCCODEINFO LineInfo, PVOID UserContext)
{
    const HANDLE processHandle = GetCurrentProcess();


    char* fileName = LineInfo->FileName;





    const PXSize index = PXTextFindLastCharacterA(LineInfo->FileName, -1, '\\');

    if(-1 != index)
    {
        fileName += index + 1;
    }






    void* kkey = LineInfo->Key;
    void* aaaa = LineInfo->Address;

    DWORD displacement = 0;



    //  const PXBool symbolFetchSuccess = SymFromAddr(processHandle, aaaa, &displacement, symb);




    /*

    char bufferB[PXDebugWindowsSymbolStringLength];
    IMAGEHLP_LINE64 imageHelperLine;
    imageHelperLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    imageHelperLine.FileName = bufferB;

    const PXBool lineResult = SymGetLineFromAddr(processHandle, LineInfo->Address, &displacement, &imageHelperLine);
    */

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        "Debugger",
        "Symbol-Enum",
        "P:%-20s, F:%-20s L:%-4i, %p <=> %p (%i)",
        "",//symb->Name,
        //LineInfo->Obj,
        fileName,
        LineInfo->LineNumber,
        UserContext,
        LineInfo->Address,
        (PXSize)UserContext - (PXSize)LineInfo->Address
    );
#endif

    return PXTrue;
}
#endif // OSWindows

#if OSWindows
BOOL CALLBACK PXENUMERATESYMBOLSUEUE(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PSYMBOL_INFO pSymbolRes)
{
    PXCopyList(char, SymbolSize, pSymInfo, pSymbolRes);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        "Debugger",
        "XXX",
        "XXXX"
    );
#endif

    return PXTrue;
}
#endif

PXActionResult PXAPI PXDebugFetchSymbolFromRougeAdress(PXDebug* const pxDebug, PXSymbol* const pxSymbol, void* adress)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    const HANDLE processHandle = GetCurrentProcess();
    const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);
    DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES

    PXClear(PXSymbol, pxSymbol);

    // Fetch symbol
    PXDebugSymbolReadFromAdress(pxDebug, pxSymbol, adress);


    return PXActionSuccessful;

    /*

        PXSize displacement = 0;

    char symBuffer[PXDebugWindowsSymbolStringLength];
    void* symbolAdress = 0;
    void* moduleAdress = 0;
    IMAGEHLP_LINE64 imageHelperLine;
    imageHelperLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    imageHelperLine.FileName = symBuffer;



    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;



    IMAGEHLP_SYMBOL64* imgsYMBOL64 = buffer;
    imgsYMBOL64->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64)+200;
    */


    // this loops over allsymbols in a file
    //const PXBool assaas = SymEnumLines(processHandle, moduleAdress, PXNull, PXNull, PXPSYM_ENUMLINES_CALLBACK, symbolAdress);


    //const PXBool asd = SymGetSymFromAddr64(processHandle, symbolAdress, &displacement, imgsYMBOL64);

    // const PXBool lineResultSHU = SymEnumSymbolsForAddr(processHandle, symbolAdress, PXENUMERATESYMBOLSUEUE, pSymbol);

    // always fails because the recieved adress here is always invalid
    //  const PXBool lineResult = SymGetLineFromAddr(processHandle, symbolAdress, &displacement, &imageHelperLine);
    //  const PXActionResult linres = PXErrorCurrent();


    // Store symvol name

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDebugDumpCreate(PXDebug* const pxDebug)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows && 0

    const HANDLE hFile = CreateFileA("minidump.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    const PXActionResult pxActionResult = PXWindowsErrorCurrent(INVALID_HANDLE_VALUE != hFile);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    const HANDLE processHandle = GetCurrentProcess();
    const DWORD processID = GetCurrentProcessId();

    EXCEPTION_POINTERS* exceptionPointers = GetExceptionInformation();

    MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
    exceptionInfo.ThreadId = processID;
    exceptionInfo.ExceptionPointers = exceptionPointers;
    exceptionInfo.ClientPointers = TRUE;

    const BOOL write = MiniDumpWriteDump(processHandle, processID, hFile, MiniDumpNormal, &exceptionInfo, NULL, NULL);

    CloseHandle(hFile);

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXDebugModuleHandleFromAdress(PXHandleModule* const pxHandleModule, const void* const adress)
{
#if OSUnix && 0
    Dl_info info;

    const int resultID = dladdr(adress, &info); // dlfcn.h
    const PXActionResult moduleFetchResult = PXErrorCurrent(0 != resultID);

    return moduleFetchResult;
#elif OSWindows
    const PXBool moduleFetchSuccess = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)adress, pxHandleModule);
    const PXActionResult moduleFetchResult = PXErrorCurrent(moduleFetchSuccess);

    return moduleFetchResult;
#else
    *pxHandleModule = PXNull;

    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXDebugModuleHandleToName(const PXHandleModule pxHandleModule, char* moduleName)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    // net to get moduleinto, but we have those
    // IMAGEHLP_MODULE64 mMAGEHLP_MODULE64;
    // mMAGEHLP_MODULE64.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
    //  const PXBool moduleFetchSuccessaSAS = SymGetModuleInfo64(processHandle, moduleHandle, &mMAGEHLP_MODULE64);


    // Extract names
    {
        char moduleName[MAX_PATH];
        PXClearList(char, moduleName, MAX_PATH);
        const DWORD moduleNameLength = GetModuleFileNameA(pxHandleModule, moduleName, sizeof(moduleName));

        char* molduleNamefixed = moduleName;
        PXSize actualSize = moduleNameLength;

        const char systemPath[] = "C:\\WINDOWS\\SYSTEM32\\";
        const PXSize systemPathSize = sizeof(systemPath);
        const PXBool isSystem = PXTextCompareIgnoreCaseA(systemPath, systemPathSize, moduleName, systemPathSize);

        if(isSystem)
        {
            molduleNamefixed += systemPathSize - 1;
            actualSize -= systemPathSize - 1;

            // Check if its a driver
            const char driverStoreText[] = "DriverStore";
            const PXSize driverStoreTextSize = sizeof(driverStoreText);

            const PXBool isDriver = PXTextCompareA(driverStoreText, driverStoreTextSize, molduleNamefixed, driverStoreTextSize);

            if(isDriver)
            {
                // The path is very long and useless, only get the DLL name.
                const PXSize lastSlash = PXTextFindLastCharacterA(molduleNamefixed, actualSize, '\\');

                if(lastSlash != -1)
                {
                    molduleNamefixed += lastSlash + 1;
                    actualSize -= lastSlash + 1;
                }
            }
        }
        else
        {
            char currentWorkPath[MAX_PATH];
            PXClearList(char, currentWorkPath, MAX_PATH);
            PXSize currentWorkPathSize = GetModuleFileName(NULL, currentWorkPath, MAX_PATH);


            PXSize lastSlashA = PXTextFindLastCharacterA(currentWorkPath, currentWorkPathSize, '\\');
            PXSize lastSlashB = PXTextFindLastCharacterA(moduleName, currentWorkPathSize, '\\');
            PXBool isRelativePath = PXTextCompareA(currentWorkPath, lastSlashA, moduleName, lastSlashB);

            if(isRelativePath)
            {
                molduleNamefixed += lastSlashA + 1;
                actualSize -= lastSlashA + 1;
            }
        }

        PXTextCopyA(molduleNamefixed, actualSize, moduleName, 64);
    }

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}
