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

#pragma comment(lib, "DbgHelp.lib")

#endif

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

const char PXDebugerText[] = "Debugger";
const char PXDebugerStackTraceText[] = "StackTrace";
const char PXDebugerLoopText[] = "PXDebugLoop";
const PXI8U PXDebugerLoopTextLength = sizeof(PXDebugerLoopText);

#if OSWindows
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

void PXAPI PXDebugDebuggerSendMessage(PXDebug PXREF pxDebug, PXText PXREF message)
{
    switch(message->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            PXDebugOutputDebugStringA(message->A);
            break;
        }
        case TextFormatUNICODE:
        {
            PXDebugOutputDebugStringW(message->W);
            break;
        }
    }
}

PXResult PXAPI PXDebugDebuggerInitialize(PXDebug PXREF pxDebug)
{
    PXClear(PXDebug, pxDebug);

#if OSUnix
#elif PXOSWindowsDestop

   

    // Init symbol server
    {
      
    }

    return PXResultOK;

#endif
}

PXResult PXAPI PXDebugStartProcess(PXDebug PXREF pxDebug, const PXText PXREF applicationName)
{
    PXTextCopy(applicationName, &pxDebug->ApplicatioName);

    // Start Thread that will listen to given process.
    {
        PXThreadCreateInfo pxThreadCreateInfo;
        PXClear(PXThreadCreateInfo, &pxThreadCreateInfo);
        PXTextFromAdressA(&pxThreadCreateInfo.Info.Name, PXDebugerLoopText, 0, PXDebugerLoopTextLength);
        pxThreadCreateInfo.ThreadFunction = PXDebugLoop;
        pxThreadCreateInfo.Parameter = pxDebug;
        pxThreadCreateInfo.Behaviour = PXThreadBehaviourDefault;

        const PXResult result = PXThreadCreate(&pxDebug->EventListenLoop, &pxThreadCreateInfo);

        // If thread cannot be started, stop.
        if(PXResultOK != result)
            return result;
    }

    // Start process


    return PXResultOK;
}

PXResult PXAPI PXDebugAttach(PXDebug PXREF pxDebug)
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
    const PXResult pxResult = PXErrorCurrent(result);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    return PXResultOK;
#endif
}

void PXAPI OnDebugProcessCreate(PXDebug PXREF pxDebug)
{

}

void PXAPI OnDebugProcessExit(PXDebug PXREF pxDebug, const PXI32U exitCode)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        "Process",
        "Exit Thread <%i>",
        exitCode
    );
#endif
}

PXResult PXAPI PXDebugWaitForEvent(PXDebug PXREF pxDebug)
{
#if OSUnix
    __pid_t processID = pxDebug->Process.ProcessID;

    // Trance
    {
        const long result = ptrace(PTRACE_GETEVENTMSG, processID, 0); // since Linux 2.5.46
        const PXResult tranceResult = PXErrorCurrent(-1 != result);

        if(PXResultOK != tranceResult)
        {
            return tranceResult;
        }
    }

    // wait
    {
        int waitStatus = 0;
        pid_t waitedProcessID = waitpid(processID, &waitStatus, __WALL);
        const PXResult waitResult = PXErrorCurrent(-1 != waitedProcessID);

        if(PXResultOK != waitResult)
        {
            return waitResult;
        }
    }

    return PXResultOK;

#elif PXOSWindowsDestop 

    DEBUG_EVENT debugEvent; 
    DWORD dwContinueStatus = DBG_CONTINUE; // This flag need to be set for the debugger in this functiom

    const DWORD dwMilliseconds = 0;
    const PXResult pxResult = PXDebugEventWait(&debugEvent, dwMilliseconds);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    switch(debugEvent.dwDebugEventCode) // Process the debugging event code.
    {
        case EXCEPTION_DEBUG_EVENT:
        {
            const EXCEPTION_DEBUG_INFO PXREF exceptionDdebugInfo = &debugEvent.u.Exception;
            const EXCEPTION_RECORD PXREF exceptionRecord = &exceptionDdebugInfo->ExceptionRecord;

            switch(exceptionRecord->ExceptionCode)
            {
                case EXCEPTION_ACCESS_VIOLATION:
                {
                    // First chance: Pass this on to the system.
                    // Last chance: Display an appropriate error.

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingEvent,
                        PXDebugerText,
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
                        PXDebugerText,
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
                        PXDebugerText,
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
                        PXDebugerText,
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
                        PXDebugerText,
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
                        PXDebugerText,
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

            const CREATE_THREAD_DEBUG_INFO PXREF createThreadDebugInfo = &debugEvent.u.CreateThread;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXDebugerText,
                "Exception",
                "created Thread (%i) by Process (%i)",
                createThreadDebugInfo->lpStartAddress,
                debugEvent.dwThreadId,
                debugEvent.dwProcessId
            );
#endif

           // PXThread pxThread;
          //  PXThreadConstructFromHandle(&pxThread, pxDebug->Process.ThreadHandle);
            //pxThread.ThreadHandle = pxDebug->Process.ThreadHandle;

           // PXThreadStateChange(&pxThread, PXExecuteStateRunning);

            break;
        }
        case CREATE_PROCESS_DEBUG_EVENT:
        {
            // As needed, examine or change the registers of the process's initial thread with the GetThreadContext and
            // SetThreadContext functions; read from and write to the process's virtual memory with the ReadProcessMemory and
            // WriteProcessMemory functions; and suspend and resume thread execution with the SuspendThread and ResumeThread
            // functions. Be sure to close the handle to the process image file with CloseHandle.

            const CREATE_PROCESS_DEBUG_INFO PXREF createProcessDebugInfo = &debugEvent.u.CreateProcessInfo;


            PXText pxText;
            PXTextConstructNamedBufferA(&pxText, fileNameBuffer, 256);

            PXFile* pxFile = PXFileCreate();
            //file.FileHandle = createProcessDebugInfo->hFile;

            const PXResult res = PXFileName(pxFile, &pxText);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXDebugerText,
                "Exception",
                "Process (%i) create : %s",
                debugEvent.dwProcessId,
                pxText.A
            );
#endif

            break;
        }
        case EXIT_THREAD_DEBUG_EVENT:
        {
            // Display the thread's exit code.
            const EXIT_THREAD_DEBUG_INFO PXREF exitThreadDebugInfo = &debugEvent.u.ExitThread;

            OnDebugProcessExit(pxDebug, exitThreadDebugInfo->dwExitCode);

            break;
        }
        case EXIT_PROCESS_DEBUG_EVENT:
        {
            // Display the process's exit code.

            const EXIT_PROCESS_DEBUG_INFO PXREF exitProcessDebugInfo = &debugEvent.u.ExitProcess;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXDebugerText,
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
            // Read the debugging information Includedd in the newly
            // loaded DLL. Be sure to close the handle to the loaded DLL
            // with CloseHandle.

            const LOAD_DLL_DEBUG_INFO PXREF loadDLLDebugInfo = &debugEvent.u.LoadDll;


            PXText pxText;
            PXTextConstructNamedBufferA(&pxText, fileNameBuffer, 256);

            PXFile* file = PXFileCreate();
            //file.FileHandle = loadDLLDebugInfo->hFile;

            const PXResult res = PXFileName(file, &pxText);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXDebugerText,
                "Exception",
                "0x%p | DLL load <%s>",
                loadDLLDebugInfo->lpBaseOfDll,
                pxText.A
            );
#endif

            break;
        }
        case UNLOAD_DLL_DEBUG_EVENT:
        {
            // Display a message that the DLL has been unloaded.

            const UNLOAD_DLL_DEBUG_INFO PXREF outputDebugStringInfo = &debugEvent.u.UnloadDll;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXDebugerText,
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
            const OUTPUT_DEBUG_STRING_INFO PXREF outputDebugStringInfo = &debugEvent.u.DebugString;

#if PXLogEnable && 0
            printf("[PXDebuger] OUTPUT_DEBUG_STRING_EVENT : ");

            if(outputDebugStringInfo->fUnicode)
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
            const RIP_INFO PXREF ripInfo = &debugEvent.u.RipInfo;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXDebugerText,
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

    

    const PXResult pxContinueActionResult = PXDebugEventContinue(0, 0);

    if(PXResultOK != pxContinueActionResult)
    {
        return pxContinueActionResult;
    }

    return PXResultOK;

#endif
}

PXResult PXAPI PXDebugLibrarySymbolsFetch(PXDebug PXREF pxDebug, const PXText PXREF libraryFilePath, PXSymbol PXREF pxSymbolList, PXSize PXREF amount)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop

    PXSymbolEnumerator pxSymbolEnumerator;
    pxSymbolEnumerator.SymbolList = 0;
    pxSymbolEnumerator.Amount = 0;
    pxSymbolEnumerator.Size = 128;

    pxSymbolEnumerator.SymbolList = PXMemoryHeapCallocT(PXSymbol, pxSymbolEnumerator.Size);

    PXProcess pxProcess;
    PXProcessCurrent(&pxProcess);

    // Initialize
    {
        const PXResult initializeResult = PXSymbolServerInitialize();

        if(PXResultOK != initializeResult)
        {
            return initializeResult;
        }
    }

   // PXSymbolModuleLoad(libraryFilePath->A, 0);



   // PXSymbolListLoad();
  


    PXSymbolServerCleanup();



    return PXResultOK;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXDebugLogMessage(PXText PXREF pxText)
{
    // because we cant trust the string to be null terminated, and
    // windows does not have a function that takes in a size, we
    // have to do it per character. It's a mess. Microsoft please fix

    for(PXSize i = 0; i < pxText->SizeUsed; ++i)
    {
        char buffer[4] = { pxText->A[i], 0, 0, 0 };

#if OSUnix
        // Does this exist?
#elif OSWindows
        OutputDebugStringA(buffer);
#endif
    }
}


PXResult PXAPI PXDebugHeapMemoryList(PXDebug PXREF pxDebug)
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
        PXDebugerText,
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

            const PXResult symbolFetchResult = PXMemorySymbolFetch(target, &pxSymbol);
            const PXBool success = PXResultOK == symbolFetchResult;

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
                    PXDebugerText,
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

        return PXResultInvalid;
    }


    // Fetch first
    {
        const PXBool firstFetch = Heap32ListFirst(hHeapSnap, &heapList);

        if(!firstFetch)
        {
            return PXResultInvalid;
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
    } while(Heap32ListNext(hHeapSnap, &heapList));


    CloseHandle(hHeapSnap);
#endif


#endif

    return PXResultOK;
}

PXResult PXAPI PXDebugFetchSymbolThread(PXDebug PXREF pxDebug, PXSymbol PXREF pxSymbol, PXThread* pxThread)
{
    /*
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    // if we dont have a handle or even an ID, we cant proceed
    const PXBool hasAtlestOne = pxThread->ThreadID || pxThread->ThreadHandle;

    if(!hasAtlestOne)
    {
        return PXResultInvalid;
    }


    void* adress = 0;

    // create thread handle if not exists
    if(!pxThread->ThreadHandle)
    {
        pxThread->ThreadHandle = OpenThread(THREAD_ALL_ACCESS, PXFalse, pxThread->ThreadID);
    }

    const THREADINFOCLASS infoClass = (THREADINFOCLASS)9; // ThreadQuerySetWin32StartAddress, undocumented magic number?

    const NTSTATUS querryResult = NtQueryInformationThread
    (
        pxThread->ThreadHandle,
        infoClass, 
        &adress,
        sizeof(void*),
        PXNull
    ); // KERNEL

    const PXResult symbolResult = PXDebugFetchSymbolFromRougeAdress(pxDebug, pxSymbol, adress);

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
*/

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
        PXDebugerText,
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
        PXDebugerText,
        "XXX",
        "XXXX"
    );
#endif

    return PXTrue;
}
#endif

PXResult PXAPI PXDebugFetchSymbolFromRougeAdress(PXDebug PXREF pxDebug, PXSymbol PXREF pxSymbol, void* adress)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    PXClear(PXSymbol, pxSymbol);

    PXSymbolServerInitialize();



    // Fetch symbol
    PXSymbolFromAddress(pxSymbol, adress);


    return PXResultOK;

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
    //  const PXResult linres = PXErrorCurrent();


    // Store symvol name

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXThreadResult PXAPI PXDebugLoop(PXDebug PXREF pxDebug)
{
    // Create process to debug on
    {
        const PXResult result = PXProcessCreate(&pxDebug->Process, &pxDebug->ApplicatioName, PXProcessCreationModeDebugProcessOnly);

        // If starting the process failed, stop.
        //PXActionReturnOnError(result);
        return -1;
    }

    pxDebug->IsRunning = PXTrue;

    while(pxDebug->IsRunning)
    {
        PXResult pxResult = PXDebugWaitForEvent(pxDebug);

        pxResult = PXResultInvalid;
    }

    return PXResultOK;
}

PXResult PXAPI PXDebugDumpCreate(PXDebug PXREF pxDebug)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows && 0

    const HANDLE hFile = CreateFileA("minidump.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    const PXResult pxResult = PXWindowsErrorCurrent(INVALID_HANDLE_VALUE != hFile);

    if(PXResultOK != pxResult)
    {
        return pxResult;
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

