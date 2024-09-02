#include "PXProcess.h"

#if OSUnix
#include <unistd.h>
#include <sys/resource.h>
#elif OSWindows
//#include <processthreadsapi.h> Not found in XP build
#include <psapi.h>
#include <tlhelp32.h>
#include <winternl.h> // ntexapi.h.
#endif

#include <OS/Memory/PXMemory.h>
#include <OS/Library/PXLibrary.h>
#include <OS/Console/PXConsole.h>
#include <OS/Async/PXThread.h>
#include <OS/File/PXFile.h>
#include <OS/Debug/PXDebug.h>

void PXAPI PXProcessConstruct(PXProcess* const pxProcess)
{
    PXClear(PXProcess, pxProcess);
}

#if OSWindows
void PXAPI PXProcessConstructFromHandle(PXProcess* const pxProcess, HANDLE processHandle)
{
    PXProcessConstruct(pxProcess);

#if WindowsAtleastXP
    pxProcess->ProcessID = GetProcessId(processHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif    
}
#endif

void PXAPI PXProcessCurrent(PXProcess* const pxProcess)
{
    PXProcessConstruct(pxProcess);

#if OSUnix
    pxProcess->ThreadHandle = 0;
    pxProcess->ProcessID = getpid();
#elif OSWindows
    // Returns a pseudo handle to the current process. Its -1 but may change in feature versions.
    pxProcess->ProcessHandle = GetCurrentProcess(); // Windows 2000 SP4, Kernel32.dll, processthreadsapi.h

#if WindowsAtleastXP
    pxProcess->ProcessID = GetProcessId(pxProcess->ProcessHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

#endif
}

void PXAPI PXProcessParent(PXProcess* const pxProcess)
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

void PXAPI PXProcessExitCurrent(const PXInt32U exitCode)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    ExitProcess(exitCode);
#endif
}

PXActionResult PXAPI PXProcessHandleCountGet(PXProcess* pxProcess, PXSize* const handlesAmount)
{
    PXProcess pxProcessOverride;

    if (!handlesAmount)
    {
        return PXActionRefusedArgumentInvalid;
    }

    *handlesAmount = 0;

    // if pxProcess is NULL, override
    if (!pxProcess)
    {
        pxProcess = &pxProcessOverride;

        PXProcessCurrent(pxProcess);
    }

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows && WindowsAtleastXP
    DWORD handleCount = 0;

    const BOOL result = GetProcessHandleCount // Windows XP, Kernel32.dll, processthreadsapi.h
    (
        pxProcess->ProcessHandle,
        &handleCount
    );

    PXActionOnErrorFetchAndReturn(!result);

    *handlesAmount = handleCount;

    return PXActionSuccessful;

#else 
    return PXActionNotSupportedByOperatingSystem;
#endif    
}

typedef enum PXHandleType_
{
    PXHandleTypeInvalid,
    PXHandleTypeDirectory,
    PXHandleTypeThread,
    PXHandleTypeProcess,
    PXHandleTypeIOCompletionReserve,
    PXHandleTypeEvent, 
    PXHandleTypeMutant,
    PXHandleTypeSemaphore,
    PXHandleTypeTimer,
    PXHandleTypeIETimer,
    PXHandleTypeWindowStation,
    PXHandleTypeDesktop,
    PXHandleTypeFile,
    PXHandleTypeWaitCompleation,
    PXHandleTypeIoCompleation,
    PXHandleTypeTpWorkerFactor,
    PXHandleTypeSection,
    PXHandleTypeKey,
    PXHandleTypeALPCPort,
    PXHandleTypeEtwRegistration
}
PXHandleType;

typedef struct PXHandle_
{
    char TypeName[32];
    PXHandleType Type;

    char Description[256];

}
PXHandle;

PXHandleType PXHandleTypeFromID(const PXInt8U typeID)
{
    switch (typeID)
    {
        case 37: 
            return PXHandleTypeFile;

        case 3: return PXHandleTypeDirectory;
        case 8: return PXHandleTypeThread;
        case 11: return PXHandleTypeIOCompletionReserve;
        case 16: return PXHandleTypeEvent; 
        case 17: return PXHandleTypeMutant; 
        case 19: return PXHandleTypeSemaphore; 
        case 20: return PXHandleTypeTimer;
        case 21: return PXHandleTypeIETimer;
        case 24: return PXHandleTypeWindowStation;
        case 25: return PXHandleTypeDesktop;

        case 36: return PXHandleTypeWaitCompleation; 

        case 35: 
            return PXHandleTypeIoCompleation;

        case 30: return PXHandleTypeTpWorkerFactor;
        case 42: return PXHandleTypeSection;
        case 44: return PXHandleTypeKey;
        case 46: return PXHandleTypeALPCPort;
        case 50: return PXHandleTypeEtwRegistration;

        default:
            return PXHandleTypeInvalid;
    }
}

PXActionResult PXAPI PXProcessHandleListAll(PXDebug* const pxDebug, PXProcess* pxProcess)
{
    PXProcess pxProcessOverride;

    // if pxProcess is NULL, override
    if (!pxProcess)
    {
        pxProcess = &pxProcessOverride;

        PXProcessCurrent(pxProcess);
    }

#if OSUnix
#elif OSWindows

#if 0
    typedef struct _SYSTEM_HANDLE
    {
        DWORD    ProcessID;
        WORD    HandleType;
        WORD    HandleNumber;
        DWORD    KernelAddress;
        DWORD    Flags;
    } SYSTEM_HANDLE;
#elif 0

#else
    typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
    {
        USHORT  UniqueProcessId;
        USHORT  CreatorBackTraceIndex;
        UCHAR    ObjectTypeIndex;
        UCHAR    HandleAttributes;
        USHORT    HandleValue;
        PVOID   Object;
        ULONG   GrantedAccess;
    }
    SYSTEM_HANDLE_TABLE_ENTRY_INFO;
#endif

    const PXSize sysHandleSize = sizeof(SYSTEM_HANDLE_TABLE_ENTRY_INFO);

    typedef struct _SYSTEM_HANDLE_INFORMATION
    {
        ULONG            NumberOfHandles;
        SYSTEM_HANDLE_TABLE_ENTRY_INFO    Handles[1];
    } 
    SYSTEM_HANDLE_INFORMATION;

    typedef LONG (*NTAPI PXNtQuerySystemInformation)
    (
            const PXInt32U SystemInformationClass, // enum SYSTEM_INFORMATION_CLASS
            void* const SystemInformation,
            const PXInt32U SystemInformationLength,
            PXInt32U* const ReturnLength
    );

    typedef LONG(*NTAPI PXNtQueryObject)
    (
        HANDLE                   Handle,
        OBJECT_INFORMATION_CLASS ObjectInformationClass,
        PVOID                    ObjectInformation,
        ULONG                    ObjectInformationLength,
        PULONG                   ReturnLength
    );



    //NtQuerySystemInformation();

    PXLibrary pxLibraryNTDLL;
    PXLibraryOpenA(&pxLibraryNTDLL, "ntdll.dll");


    PXNtQuerySystemInformation pxNtQuerySystemInformation;
    PXNtQueryObject pxNtQueryObject;
    
    PXLibraryGetSymbolA(&pxLibraryNTDLL, &pxNtQuerySystemInformation, "NtQuerySystemInformation");
    PXLibraryGetSymbolA(&pxLibraryNTDLL, &pxNtQueryObject, "NtQueryObject");





    ULONG sizeNeeded = 0;

    PXSize sizeAllocated = 0;

    // Dummy peek
    {
    
    }

    {
        SYSTEM_HANDLE_INFORMATION systemHandleInfo;

        LONG resultA = pxNtQuerySystemInformation(16, &systemHandleInfo, sizeof(SYSTEM_HANDLE_INFORMATION), &sizeNeeded); // Peek size needed

        
        sizeAllocated = sizeNeeded;


        //PXSize amunum = 0;
        //PXProcessHandleCountGet(PXNull, &amunum); // Get Peeked size from this source, for some reason other one is bad
        //sizeAllocated = sizeof(SYSTEM_HANDLE_INFORMATION) * sizeNeeded * amunum;
    }    

    //PXSize sizeAllocated = sizeof(SYSTEM_HANDLE_INFORMATION) * sizeNeeded;
    SYSTEM_HANDLE_INFORMATION* memory = (SYSTEM_HANDLE_INFORMATION*)(sizeAllocated);
    PXClearList(char, memory, sizeAllocated);

    LONG resultB = pxNtQuerySystemInformation(16, memory, sizeAllocated, &sizeNeeded);

    
    char buffer[200];
    PUBLIC_OBJECT_TYPE_INFORMATION* objectTypeInfo = buffer;

    PXInt32U amountHandleTotalSystem = memory->NumberOfHandles;
    PXInt32U amountHandleTotalProgram = 0;

    for (size_t i = 0; i < memory->NumberOfHandles; i++)
    {
        SYSTEM_HANDLE_TABLE_ENTRY_INFO* sysHandle = &memory->Handles[i];

        HANDLE handleCurrent = sysHandle->HandleValue;
        const PXBool isSameProcess = pxProcess->ProcessID == sysHandle->UniqueProcessId; 

        // We only want our own handles, if we would want the others aswell
        // use OpenProcess() and DuplicateHandle() to fetch the handle to this process and use it
        
        if (!isSameProcess) 
        {
#if 1
            continue;
#else
            // Open the remote process
            const HANDLE remoteProcessHandle = OpenProcess
            (
                sysHandle->UniqueProcessId
            );
            const PXBool success = NULL != remoteProcessHandle;

            if (!success)
                continue;

            // Duplicate the handle
            HANDLE supedHandle = 0;

            const PXBool duplicateResult = DuplicateHandle
            (
                remoteProcessHandle,
                handleCurrent,
                0,
                &supedHandle,
                DUPLICATE_SAME_ACCESS,
                FALSE,
                DUPLICATE_SAME_ACCESS
            );

            if (!duplicateResult)
                continue;
                            

            handleCurrent = supedHandle;
#endif

            //continue;
        }

        ++amountHandleTotalProgram;

        ULONG length = 0;
        LONG xx = pxNtQueryObject(handleCurrent, ObjectTypeInformation, objectTypeInfo, 200, &length);

        PXHandle pxHandle;
        pxHandle.Type = PXHandleTypeFromID(sysHandle->ObjectTypeIndex);

        PXTextCopyWA(objectTypeInfo->TypeName.Buffer, objectTypeInfo->TypeName.Length, pxHandle.TypeName, 32);


        PXClearList(char, pxHandle.Description, 256);

        switch (pxHandle.Type)
        {
            case PXHandleTypeFile:
            {    
                PXFile pxFile;
                pxFile.ID = handleCurrent;

                PXText buffer;
                PXTextConstructNamedBufferA(&buffer, bufferAA, 256);

                PXActionResult result = PXFilePathGet(&pxFile, &buffer);

                if (PXActionSuccessful == result)
                {
                    PXTextCopyA(buffer.TextA, buffer.SizeUsed, pxHandle.Description, 256);
                }
                else
                {
                    PXTextCopyA("[N/A]", 5, pxHandle.Description, 256);
                }        

                break;
            }
            case PXHandleTypeProcess:
            {
                PXProcess pxProcess;
                PXProcessConstructFromHandle(&pxProcess, handleCurrent);

                break;
            }
            case PXHandleTypeThread:
            {
                PXThread pxThread;
                PXThreadConstructFromHandle(&pxThread, handleCurrent);
        

                // GetThreadInformation()
                PXText buffer;
                PXTextConstructFromAdressA(&buffer, pxHandle.Description, 0, 256);

                PXThreadNameGet(pxDebug, &pxThread, &buffer);
                

                // Fetch thread Description
                {
            
#if 0

                    HANDLE process = GetCurrentProcess();

                    const BOOL ww = SymInitialize
                    (
                        process,
                        PXNull,
                        0
                    );
                    auto wwwwa = GetLastError();
                    PXActionResult xwwx = PXWindowsHandleErrorFromID(wwwwa);


                    char symBuffer[300];
                    SYMBOL_INFO* symbolInfo = symBuffer;

                    PXClearList(char, symbolInfo, 300);
                    symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
                    symbolInfo->MaxNameLen = symbolInfo->SizeOfStruct - sizeof(SYMBOL_INFO);

                    const BOOL rr = SymFromAddr
                    (
                        process,
                        sysHandle->Object,
                        PXNull,
                        symbolInfo
                    );
                    auto wwa = GetLastError();
                    PXActionResult xx = PXWindowsHandleErrorFromID(wwa);

                    if (rr)
                    {
                        PXTextCopyAW(symbolInfo->Name, symbolInfo->NameLen, pxHandle.Description, 256);
                        printf("");
                    }
#endif

                }

                break;
            }

            default:
                break;
        }

#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingInfo,
            "Process",
            "Handle",
            "[%2i:%c] <%s> <%s>",
            sysHandle->ObjectTypeIndex,
            (pxHandle.Type != PXHandleTypeInvalid) ? 'Y' : 'N',
            pxHandle.TypeName,
            pxHandle.Description
        );
#endif
    }

    PXDeleteStackList(char, sizeAllocated, &memory, PXNull);

    PXLibraryClose(&pxLibraryNTDLL);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXProcessCreate(PXProcess* const pxProcess, const PXText* const programmPath, const PXProcessCreationMode mode)
{
    PXProcessConstruct(pxProcess);

    switch (programmPath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix
            return PXActionRefusedNotImplemented;

#elif OSWindows
            STARTUPINFOA startupInfo;
            PROCESS_INFORMATION processInfo;

            PXClear(STARTUPINFOA , &startupInfo);
            PXClear(PROCESS_INFORMATION, &processInfo);

            startupInfo.cb = sizeof(STARTUPINFOA);

            const DWORD creationflags =
                DEBUG_ONLY_THIS_PROCESS |
                CREATE_NEW_CONSOLE |
                PROCESS_QUERY_INFORMATION |
                PROCESS_VM_READ;

            const PXBool success = CreateProcessA // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h
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
            return PXActionRefusedNotImplemented;

#elif OSWindows
            STARTUPINFOW startupInfo;
            PROCESS_INFORMATION processInfo;

            PXClear(STARTUPINFOW, &startupInfo);
            PXClear(PROCESS_INFORMATION, &processInfo);

            startupInfo.cb = sizeof(STARTUPINFOW);

            const PXBool success = CreateProcessW // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h
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

PXActionResult PXAPI PXProcessListAll(PXProcessDetectedEvent pxProcessDetectedEvent)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop
    if (!pxProcessDetectedEvent)
    {
        return PXActionRefusedMissingCallBack;
    }

    const DWORD flag = TH32CS_SNAPPROCESS;
    const DWORD processID = PXNull;
    const HANDLE snapshotHandle = CreateToolhelp32Snapshot(flag, processID); // Windows XP, Kernel32.dll, tlhelp32.h
    const PXBool success = snapshotHandle != INVALID_HANDLE_VALUE && snapshotHandle != ((HANDLE)(LONG_PTR)ERROR_BAD_LENGTH);
    PROCESSENTRY32 processEntryW;
    processEntryW.dwSize = sizeof(PROCESSENTRY32);
    
    PXActionOnErrorFetchAndReturn(!success);

    PXBool successfulFetch = Process32First(snapshotHandle, &processEntryW);

    if (!successfulFetch)
    {
        return PXActionInvalid;
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

        PXTextConstructFromAdressA(&pxProcess.ExecutableFilePath, processEntryW.szExeFile, PXTextLengthUnkown, PXTextLengthUnkown);

        PXFunctionInvoke(pxProcessDetectedEvent, &pxProcess);

        successfulFetch = Process32Next(snapshotHandle, &processEntryW);
    }

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXProcessThreadsListAll(PXProcess* const pxProcess, struct PXThread_** pxThreadListRef, const PXSize amount, PXSize* resultSIze)
{
    PXSize threadIndex = 0;

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    PXDebug* pxDebug = PXDebugInstanceGet();

    const HANDLE processHandle = GetProcessHeap();
    const BOOL symbolServerInitialize = SymInitialize(processHandle, PXNull, PXTrue);

    const HANDLE snapShotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    const PXBool isValid = snapShotHandle != INVALID_HANDLE_VALUE;

    if(!isValid)
    {
        return PXActionInvalid;
    }

  


    DWORD processID = GetCurrentProcessId();


    PXThread* pxThreadList = *pxThreadListRef;


    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(threadEntry);

    if(Thread32First(snapShotHandle, &threadEntry))
    {
        do
        {
            const PXBool isTarget = 
                (threadEntry.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(threadEntry.th32OwnerProcessID)) &&
                (threadEntry.th32OwnerProcessID == processID);
                 
            if(isTarget)
            {
                PXThread* const pxThread = &pxThreadList[threadIndex];    

                PXText text;
                PXTextConstructBufferA(&text, 128);               

                pxThread->ThreadID = threadEntry.th32ThreadID;

               PXThreadNameGet(pxDebug, pxThread, &text);

                ++threadIndex;
            }

            threadEntry.dwSize = sizeof(threadEntry);
        } 
        while(Thread32Next(snapShotHandle, &threadEntry));
    }


   
    PXSymbol pxSymbol[6];


    PXDebugStackTrace(pxDebug, pxSymbol, 6, 0, 6);
    PXDebugHeapMemoryList(pxDebug);


    CloseHandle(snapShotHandle);

    *resultSIze = threadIndex;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXProcessOpenViaID(PXProcess* const pxProcess, const PXProcessID pxProcessID)
{
    PXProcessConstruct(pxProcess);

#if OSUnix
    return PXActionRefusedNotImplemented;
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

PXActionResult PXAPI PXProcessClose(PXProcess* const pxProcess)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    const BOOL successful = CloseHandle(pxProcess->ProcessHandle); // Windows 2000 (+UWP), Kernel32.dll, handleapi.h

    PXActionOnErrorFetchAndReturn(!successful);

    pxProcess->ProcessHandle = PXNull;

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXProcessMemoryWrite(const PXProcess* const pxProcess, const void* const targetAdress, const void* const buffer, const PXSize bufferSize)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
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

PXActionResult PXAPI PXProcessMemoryRead(const PXProcess* const pxProcess, const void* const targetAdress, void* const buffer, const PXSize bufferSize)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

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

PXActionResult PXAPI PXProcessMemoryInfoFetch(PXProcessMemoryInfo* const pxProcessMemoryInfo)
{
    PXClear(PXProcessMemoryInfo, pxProcessMemoryInfo);

#if OSUnix
    const int who = RUSAGE_SELF;
    struct rusage rusageData;
    const int returnCode = getrusage(who, &rusageData);
    const PXBool success = returnCode == 0;

    PXActionOnErrorFetchAndReturn(!success);

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
        PXClear(PROCESS_MEMORY_COUNTERS, &processMemoryCounters);
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
