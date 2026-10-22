#include "PXOS.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/File/PXFile.h>

#if OSUnix
#define _GNU_SOURCE
#define __USE_GNU
#include <sys/mman.h>
#include <sys/utsname.h>
#include <sys/ptrace.h>
#include <fcntl.h> // O_RDONLY
#include <dlfcn.h> // Dl_info
#include <signal.h> // SIGTRAP
#elif OSWindows
#include <Windows.h>
#include <DbgHelp.h>
#include <Psapi.h>
#endif


const char PXUnknownElement[] = "???";

//void _chkstk(size_t s) {};
//extern int _fltused = 0;


#if OSUnix
#elif OSWindows
typedef BOOL(WINAPI* PXDebugActiveProcessStopFunction)(_In_ DWORD dwProcessId);
typedef BOOL(WINAPI* PXContinueDebugEventFunction)(_In_ DWORD dwProcessId, _In_ DWORD dwThreadId, _In_ DWORD dwContinueStatus);
typedef BOOL(WINAPI* PXWaitForDebugEventFunction)(_Out_ LPDEBUG_EVENT lpDebugEvent, _In_ DWORD dwMilliseconds);
typedef BOOL(WINAPI* PXDebugActiveProcessFunction)(_In_ DWORD dwProcessId);
typedef VOID(WINAPI* PXDebugBreakFunction)(VOID);
typedef VOID(WINAPI* PXOutputDebugStringAFunction)(_In_opt_ LPCSTR lpOutputString);
typedef VOID(WINAPI* PXOutputDebugStringWFunction)(_In_opt_ LPCWSTR lpOutputString);
typedef BOOL(WINAPI* PXDebugBreakProcessFunction)(_In_ HANDLE Process);
typedef BOOL(WINAPI* PXIsDebuggerPresentFunction)(VOID);
typedef BOOL(WINAPI* PXCheckRemoteDebuggerPresentFunction)(_In_ HANDLE hProcess, _Out_ PBOOL pbDebuggerPresent);

typedef BOOL(WINAPI* PXSymCleanupFunction)(_In_ HANDLE hProcess);
typedef DWORD(WINAPI* PXSymLoadModuleFunction)(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll);
typedef BOOL(WINAPI* PXSymInitializeFunction)(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
typedef BOOL(WINAPI* PXSymEnumSymbolsFunction)(_In_ HANDLE hProcess, _In_ ULONG64 BaseOfDll, _In_opt_ PCSTR Mask, _In_ PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback, _In_opt_ PVOID UserContext);
typedef BOOL(WINAPI* PXSymEnumSymbolsExFunction)(_In_ HANDLE hProcess, _In_ ULONG64 BaseOfDll, _In_opt_ PCSTR Mask, _In_ PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback, _In_opt_ PVOID UserContext, _In_ DWORD Options);
typedef PVOID(WINAPI* PXSymFunctionTableAccessFunction)(__in HANDLE hProcess, __in DWORD AddrBase);
typedef DWORD(WINAPI* PXSymGetModuleBaseFunction)(__in HANDLE hProcess, __in DWORD dwAddr);

// Debug
typedef BOOL(WINAPI* PXStackWalk64Function)
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

typedef DWORD(WINAPI* PXUnDecorateSymbolNameFunction)(_In_ PCSTR name, _Out_writes_(maxStringLength) PSTR outputString, _In_ DWORD maxStringLength, _In_ DWORD flags);
typedef BOOL(WINAPI* PXSymGetSymFromAddr64Function)(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_opt_ PDWORD64 pdwDisplacement, _Inout_ PIMAGEHLP_SYMBOL64  Symbol);
#endif

typedef struct PXSymbolStackWalkInfo_
{
#if OSUnix
    int x;
#elif OSWindows
    DWORD MachineType;
    STACKFRAME                     StackFrame;
    CONTEXT                        ContextRecord;
    PREAD_PROCESS_MEMORY_ROUTINE   ReadMemoryRoutine;
    PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine;
    PGET_MODULE_BASE_ROUTINE       GetModuleBaseRoutine;
    PTRANSLATE_ADDRESS_ROUTINE     TranslateAddress;

    HANDLE HandleProcessCurrent;
    HANDLE HandleThreadCurrent;

    PGET_MODULE_BASE_ROUTINE64 SymbolFunctionTableAccess;
    PTRANSLATE_ADDRESS_ROUTINE64 SymbolGetModuleBase;
#endif
}
PXSymbolStackWalkInfo;


PXOS _PXOS;

const char PXOSName[] = "Kernel-OS";
const char PXOSFile[] = "File";
const char PXOSThread[] = "Thread";
const char PXOSVirtualAllocText[] = "Virtual-Alloc";
const char PXOSVirtualFreeText[] = "Virtual-Free";


#if OSUnix
#elif OSWindows

const char PXWindowsLibrarNTDLL[] = "ntdll.dll";
const char PXWindowsLibraryKernel32[] = "KERNEL32.DLL";
const char PXWindowsLibraryDebugHelp[] = "DBGHELP.DLL";

const char PXWindows11[] = "11";
const char PXWindows10[] = "10";
const char PXWindowsServer2016[] = "Server 2016";
const char PXWindows81[] = "8.1";
const char PXWindowsServer2012R2[] = "Server 2012 R2";
const char PXWindows8[] = "8";
const char PXWindowsServer2012[] = "Server 2012";
const char PXWindows7[] = "7";
const char PXWindowsServer2008R2[] = "Server 2008 R2";
const char PXWindowsServer2008[] = "Server 2008";
const char PXWindowsVista[] = "Vista";
const char PXWindowsServer2003R2[] = "Server 2003 R2";
const char PXWindowsServer2003[] = "Server 2003";
const char PXWindowsXP[] = "XP";
const char PXWindows2000[] = "2000";
const char PXWindowsNT40[] = "NT 4.0";
const char PXWindowsNT31[] = "NT 3.1";



typedef LONG(WINAPI* PXWindowsRtlGetVersionPtr)(PRTL_OSVERSIONINFOW);


PXPrivate BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);

#endif

PXResult PXAPI PXSystemPrelude()
{
    if(_PXOS.Init)
    {
        return PXResultOK;
    }

    PXClear(PXOS, &_PXOS);

    _PXOS.MainThreadID = PXThreadCurrentID();

#if OSUnix
#elif OSWindows
    // kernel32.dll
    {
        PXText pxText;
        PXTextFromAddressA(&pxText, PXWindowsLibraryKernel32, sizeof(PXWindowsLibraryKernel32), sizeof(PXWindowsLibraryKernel32));
        PXLibraryOpen(&_PXOS.LibraryKernel, &pxText);

        PXLibraryFunctionEntry PXLibraryFunctionEntryList[] =
        {
            { &_PXOS.DebugEventContinue, "ContinueDebugEvent"},
            { &_PXOS.DebugBreak, "DebugBreak" },
            { &_PXOS.DebugProcessBreak, "DebugBreakProcess" },
            { &_PXOS.DebugEventWait, "WaitForDebugEvent" },
            { &_PXOS.DebugOutputDebugStringA, "OutputDebugStringA" },
            { &_PXOS.DebugOutputDebugStringW, "OutputDebugStringW" },
            { &_PXOS.DebugIsDebuggerPresent, "IsDebuggerPresent" },
            { &_PXOS.DebugRemotePresentCheck, "CheckRemoteDebuggerPresent" },
            { &_PXOS.DebugActiveProcessStop, "DebugActiveProcessStop" }
        };

        const PXSize amount = sizeof(PXLibraryFunctionEntryList) / sizeof(PXLibraryFunctionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryKernel, PXLibraryFunctionEntryList, amount);
    }

    // Dbghelp.dll
    {
        PXText pxText;
        PXTextFromAddressA(&pxText, PXWindowsLibraryDebugHelp, sizeof(PXWindowsLibraryDebugHelp), sizeof(PXWindowsLibraryDebugHelp));
        PXLibraryOpen(&_PXOS.LibraryDebugHelp, &pxText);

        PXLibraryFunctionEntry PXLibraryFunctionEntryList[] =
        {
            { &_PXOS.SymbolServerInitialize, "SymInitialize"},
            { &_PXOS.SymbolStackWalk, "StackWalk64"},
            { &_PXOS.SymbolUnDecorateName, "UnDecorateSymbolName"},
            { &_PXOS.SymbolFromAddress, "SymGetSymFromAddr64"},
            { &_PXOS.SymbolEnumerate, "SymEnumSymbols" },
            { &_PXOS.SymbolEnumerateEx, "SymEnumSymbolsEx" },
            { &_PXOS.SymbolModuleLoad, "SymLoadModule" },
            { &_PXOS.SymbolFunctionTableAccess, "SymFunctionTableAccess" },
            { &_PXOS.SymbolModuleBaseGet, "SymGetModuleBase" }
        };

        const PXSize amount = sizeof(PXLibraryFunctionEntryList) / sizeof(PXLibraryFunctionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryDebugHelp, PXLibraryFunctionEntryList, amount);
    }

    // ntdll.dll
    {
        PXText pxText;
        PXTextFromAddressA(&pxText, PXWindowsLibrarNTDLL, sizeof(PXWindowsLibrarNTDLL), sizeof(PXWindowsLibrarNTDLL));
        PXLibraryOpen(&_PXOS.LibraryNT, &pxText);

        PXLibraryFunctionEntry PXLibraryFunctionEntryList[] =
        {
            { &_PXOS.RtlGetVersion, "RtlGetVersion"}
        };

        const PXSize amount = sizeof(PXLibraryFunctionEntryList) / sizeof(PXLibraryFunctionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryNT, PXLibraryFunctionEntryList, amount);
    }
#endif


    _PXOS.Init = PXTrue;

   // SetConsoleOutputCP(CP_UTF8);
   // SetConsoleCP(CP_UTF8);

    PXOSVersion pxOSVersion;

    PXSystemVersionGet(&pxOSVersion);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Version",
        pxOSVersion.NameProduct
    );
#endif // 0



#if OSUnix
#elif OSWindows
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo); // Windows 2000 (+UWP), Kernel32.dll, sysinfoapi.h

    _PXOS.ApplicationAddressMinimum = systemInfo.lpMinimumApplicationAddress;
    _PXOS.ApplicationAddressMaximum = systemInfo.lpMaximumApplicationAddress;
    _PXOS.ProcessorActiveMask = systemInfo.dwActiveProcessorMask;
    _PXOS.ProcessorAmountPhysical = systemInfo.dwNumberOfProcessors; // [!] only detects logical!
    _PXOS.ProcessorAmountLogical = systemInfo.dwNumberOfProcessors;
    _PXOS.dwAllocationGranularity = systemInfo.dwAllocationGranularity;


    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* infoList;
    DWORD amountInBytes = 0;
    DWORD amountInObjects = 0;

        // Get size
    BOOL resLog = GetLogicalProcessorInformation(infoList, &amountInBytes);

    infoList = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)PXMemoryHeapCalloc(PXNull, amountInBytes, 1);
    amountInObjects = amountInBytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    resLog = GetLogicalProcessorInformation(infoList, &amountInBytes);

    _PXOS.ProcessorAmountPhysical = 0;
    _PXOS.ProcessorAmountLogical = 0;

    for(size_t i = 0; i < amountInObjects; i++)
    {
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION PXREF ptr = &infoList[i];

        switch(ptr->Relationship)
        {
            case RelationNumaNode:
                // Non-NUMA systems report a single record of this type.
                ++_PXOS.ProcessorAmountNUMA;
                break;

            case RelationProcessorCore:

                ++_PXOS.ProcessorAmountPhysical;

                // A hyperthreaded core supplies more than one logical processor.
                _PXOS.ProcessorAmountLogical += PXBitsCount(ptr->ProcessorMask);
                break;

            case RelationCache:
            {
                // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache.
                CACHE_DESCRIPTOR* cache = &ptr->Cache;
                if(cache->Level == 1)
                {
                    _PXOS.ProcessorCacheL1Size = cache->Size;
                }
                else if(cache->Level == 2)
                {
                    _PXOS.ProcessorCacheL2Size = cache->Size;
                }
                else if(cache->Level == 3)
                {
                    _PXOS.ProcessorCacheL3Size = cache->Size;
                }
                break;
            }
            case RelationProcessorPackage:
                // Logical processors share a physical package.
                //processorPackageCount++;
                break;

            default:
                //_tprintf(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
                break;
        }
    }
#endif


    //-----------------------------------------------------
    // Page sizes
    //-----------------------------------------------------
#if OSUnix

// Might need : "sudo apt-get install libhugetlbfs-dev"
// Seems to not do anything.
// Does linux have large table support?
// getconf PAGESIZE
// getconf LARGE_PAGESIZE
// getconf HUGE_PAGESIZE

    _PXOS.PageSizeNormal = getpagesize(); //  also works, sysconf(PAGESIZE );
    _PXOS.PageSizeLarge = 1 << 21;//sysconf(_SC_LARGE_PAGESIZE); MAP_HUGE_2MB
    _PXOS.PageSizeHuge = 1 << 30;//sysconf(_SC_HUGE_PAGESIZE); MAP_HUGE_1GB

#elif OSWindows
    PERFORMANCE_INFORMATION perfInfo;
    DWORD size = sizeof(PERFORMANCE_INFORMATION);
    PXClear(PERFORMANCE_INFORMATION, &perfInfo);
    perfInfo.cb = size;

    const BOOL result = GetPerformanceInfo(&perfInfo, size);

    if(!result)
    {
        return PXResultInvalid;
    }

    _PXOS.PageSizeNormal = perfInfo.PageSize;
    _PXOS.PageSizeLarge = GetLargePageMinimum(); // Windows Vista, Kernel32.dll, memoryapi.h
    _PXOS.PageSizeHuge = 1<<30; // Does this even exist?
    _PXOS.PageSizePhysical = 1 << 16;
#endif
    //-----------------------------------------------------




    //-----------------------------------------------------
    // Cacheline
    //-----------------------------------------------------

#if OSUnix
#elif OSWindows
    DWORD bufferSize = 0;
    GetLogicalProcessorInformation(NULL, &bufferSize);



    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)PXMemoryHeapCalloc(PXNull, 1, bufferSize);
    GetLogicalProcessorInformation(buffer, &bufferSize);

    const DWORD count = bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    for(DWORD i = 0; i < count; ++i)
    {
        if(buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
        {
            _PXOS.CacheLineSize = buffer[i].Cache.LineSize;
            break;
        }
    }

    PXMemoryHeapFree(PXNull, buffer);
#endif

    //-----------------------------------------------------

    //VirtualQueryEx();

    return PXResultOK;
}

PXOS* PXAPI PXSystemGet()
{
    PXOS* pxSystem = &_PXOS;

    if(!pxSystem->Init)
    {
      //  PXSystemPrelude();

       // pxSystem->Init = PXTrue;
    }

    return pxSystem;
}

const char OSNameNotFound[] = "**Unkown**";

PXResult PXAPI PXSystemVersionGetViaRegistry(PXOSVersion PXREF pxOSVersion)
{
    PXClear(PXOSVersion, pxOSVersion);

#if OSUnix
    FILE* file = fopen("/etc/os-release", "r");

    if(!file)
        return;

    char line[256];
    while(fgets(line, sizeof(line), file)) {
        if(strncmp(line, "PRETTY_NAME=", 12) == 0) {
            char* name = strchr(line, '=');
            if(name) {
                name++;
                name[strcspn(name, "\n")] = 0;
                printf("OS: %s\n", name);
            }
        }
    }
    fclose(file);


    // Kernel version
    file = fopen("/proc/version", "r");
    if(!file) return;

    char buffer[512];
    if(fgets(buffer, sizeof(buffer), file)) {
        printf("Kernel Build Info: %s\n", buffer);
    }
    fclose(file);


#elif OSWindows

    HKEY hKey;
    DWORD value_length =0;
    DWORD type = 0;

    // WinREVersion - 10.0.19041.5728
    // Contains additional "patch" element of version

    // ProductName - Windows 10 Enterprise
    // Full name of OS

    // DisplayVersion - 22H2
    // Version String?

    LSTATUS resultStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey);

    if(ERROR_SUCCESS != resultStatus)
    {
        return PXResultInvalid;
    }



    // CurrentMajorVersionNumber
    DWORD number = 0;
    value_length = 4;
    resultStatus = RegQueryValueExA(hKey, "CurrentMajorVersionNumber", NULL, &type, (LPBYTE)&number, &value_length);
    pxOSVersion->Major = number;

    // CurrentMinorVersionNumber
    value_length = 4;
    resultStatus = RegQueryValueExA(hKey, "CurrentMinorVersionNumber", NULL, &type, (LPBYTE)&number, &value_length);
    pxOSVersion->Minor = number;

    // CurrentBuild
    char textInt[16];
    value_length = sizeof(textInt);
    resultStatus = RegQueryValueExA(hKey, "CurrentBuild", NULL, &type, (LPBYTE)&textInt, &value_length);

    int textIntNumber = 0;
    PXTextToIntA(textInt, value_length, &textIntNumber);
    pxOSVersion->Build = textIntNumber;

    // UBR
    value_length = 4;
    resultStatus = RegQueryValueExA(hKey, "UBR", NULL, &type, (LPBYTE)&number, &value_length);
    pxOSVersion->Patch = number;




    // ProductName
    value_length = sizeof(pxOSVersion->NameProduct);
    resultStatus = RegQueryValueExA(hKey, "ProductName", NULL, &type, (LPBYTE)pxOSVersion->NameProduct, &value_length);
    pxOSVersion->NameProductLength = value_length;


    // Try DisplayVersion first (Windows 20H2+)
    if(RegQueryValueExA(hKey, "DisplayVersion", NULL, &type, (LPBYTE)pxOSVersion->NameVersion, &value_length) == ERROR_SUCCESS)
    {
        pxOSVersion->NameVersionLength = value_length;
    }
    else if(RegQueryValueExA(hKey, "ReleaseId", NULL, &type, (LPBYTE)pxOSVersion->NameVersion, &value_length) == ERROR_SUCCESS)
    {
        pxOSVersion->NameVersionLength = value_length;
    }
    else
    {
        pxOSVersion->NameVersionLength = PXTextCopyA(OSNameNotFound, sizeof(OSNameNotFound), pxOSVersion->NameVersion, value_length);
    }

    RegCloseKey(hKey);

#else

#endif
}

PXResult PXAPI PXSystemVersionGetViaKernel(PXOSVersion PXREF pxOSVersion)
{
    return PXResultInvalid;
}

void PXAPI PXSystemVersionGet(PXOSVersion PXREF pxOSVersion)
{
#if OSUnix
    struct utsname buffer;

    int errorID = uname(&buffer); // Linux, sys/utsname.h
    PXResult pxResult = PXErrorCurrent(0 == errorID);

    if(PXResultOK != pxResult)
    {
        // Unable to get Linux version.
        return pxResult;
    }

    FILE* f = fopen("/etc/os-release", "r");

    if(!f)
    {
        pxOSVersion->NameVersionLength = PXTextPrintA
        (
            pxOSVersion->NameProduct,
            64,
            "%s %s",
            buffer.sysname,
            buffer.release
        );

        return PXResultOK;;
    }

    char line[256];
    char prettyName[256] = { 0 };

    while(fgets(line, sizeof(line), f))
    {
        if(strncmp(line, "PRETTY_NAME=", 12) == 0)
        {
            /* Extract the value without quotes */
            const char* start = strchr(line, '"');
            const char* end = strrchr(line, '"');

            if(start && end && end > start)
            {
                size_t len = end - start - 1;

                if(len >= sizeof(prettyName))
                    len = sizeof(prettyName) - 1;

                memcpy(prettyName, start + 1, len);
                prettyName[len] = '\0';
            }
        }
    }

    fclose(f);

    if(prettyName[0] == '\0')
        snprintf(prettyName, sizeof(prettyName), "Unknown Linux");

    pxOSVersion->NameVersionLength = PXTextPrintA
    (
        pxOSVersion->NameProduct,
        64,
        "%s (%s %s)",
        prettyName,
        buffer.sysname,
        buffer.release
    );

    return PXResultOK;
#elif OSWindows



    /*

    // Registry fetch
    {
        FILE* file = fopen("/etc/os-release", "r");
        if(!file) return;

        char line[256];
        while(fgets(line, sizeof(line), file)) {
            if(strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char* name = strchr(line, '=');
                if(name) {
                    name++;
                    name[strcspn(name, "\n")] = 0;
                    printf("OS: %s\n", name);
                }
            }
        }
        fclose(file);
    }
    */



    PXSystemVersionGetViaRegistry(pxOSVersion);


    // GetVersionEx is debricated to the beginning of windows 10
    // and will only yield windows 8.1 as the highest option.
    // This was choosen, because Microsoft planned to use Build numbers as versions.
    // VerifyVersionInfoA() is somewhat debricated aswell, as it relyes on
    // a manifest file that might not exist, then it also defaults to windows 8.1
    // Solution is to call the kernel function directly, as its better any bypasses this wierd behaviour


    DWORD MajorVersion = 0;
    DWORD MinorVersion = 0;
    DWORD BuildNumber = 0;
    int isWorkstation = 0;

    PXWindowsRtlGetVersionPtr pxWindowsRtlGetVersion = (PXWindowsRtlGetVersionPtr)_PXOS.RtlGetVersion;

    if(pxWindowsRtlGetVersion)
    {
        RTL_OSVERSIONINFOW osVERSIONINFO = { 0 };
        osVERSIONINFO.dwOSVersionInfoSize = sizeof(osVERSIONINFO);

        pxWindowsRtlGetVersion(&osVERSIONINFO);

        MajorVersion = osVERSIONINFO.dwMajorVersion;
        MinorVersion = osVERSIONINFO.dwMinorVersion;
        BuildNumber = osVERSIONINFO.dwBuildNumber;
    }
    else
    {

        OSVERSIONINFOEX osVERSIONINFOEx;
        ZeroMemory(&osVERSIONINFOEx, sizeof(OSVERSIONINFOEX));
        osVERSIONINFOEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

        const BOOL result = GetVersionExA((OSVERSIONINFO*)&osVERSIONINFOEx);

        MajorVersion = osVERSIONINFOEx.dwMajorVersion;
        MinorVersion = osVERSIONINFOEx.dwMinorVersion;
        BuildNumber = osVERSIONINFOEx.dwBuildNumber;
        isWorkstation = VER_NT_WORKSTATION == osVERSIONINFOEx.wProductType;
    }


    const int isServer = GetSystemMetrics(SM_SERVERR2);

    const char* osName = 0;

    switch(MajorVersion)
    {
        case 4:
            osName = PXWindowsNT40;
            break;

        case 5:
        {
            switch(MinorVersion)
            {
                case 0:
                    osName = PXWindows2000;
                    break;

                case 1:
                    osName = PXWindowsXP;
                    break;

                case 2:
                {
                    if(isServer)
                    {
                        osName = PXWindowsServer2003;
                    }
                    else
                    {
                        osName = PXWindowsServer2003R2;
                    }

                    break;
                }
            }
        }
        case 6:
        {
            switch(MinorVersion)
            {
                case 0:
                {
                    if(isWorkstation)
                    {
                        osName = PXWindowsVista;
                    }
                    else
                    {
                        osName = PXWindowsServer2008;
                    }

                    break;
                }
                case 1:
                {
                    if(isWorkstation)
                    {
                        osName = PXWindowsServer2008R2;
                    }
                    else
                    {
                        osName = PXWindows7;
                    }

                    break;
                }
                case 2:
                {
                    if(isWorkstation)
                    {
                        osName = PXWindowsServer2012R2;
                    }
                    else
                    {
                        osName = PXWindows8;
                    }

                    break;
                }
                case 3:
                {
                    if(isWorkstation)
                    {
                        osName = PXWindowsServer2012R2;
                    }
                    else
                    {
                        osName = PXWindows81;
                    }

                    break;
                }
            }

            break;
        }
        case 10:
        {
            if(isWorkstation)
            {
                osName = PXWindowsServer2016;
            }
            else
            {
                osName = PXWindows10;
            }

            break;
        }
        case 11:
            osName = "11";
            break;

        default:
            osName = "Unkown";
            break;
    }

    PXTextPrintA
    (
        pxOSVersion->NameProduct,
      64,
        "Windows %s (v.%i.%i - Build: %i)",
        osName,
        MajorVersion,
        MinorVersion,
        BuildNumber
    );

#else
    // We cant detect any OS here
#endif
}

PXI8U PXAPI PXBitsCount(const PXSize bitMask)
{
    // Helper function to count set bits in the processor mask.
    const PXI8U LSHIFT = sizeof(PXSize) * 8 - 1;
    PXSize bitTest = (PXSize)1 << LSHIFT;
    PXI8U bitSetCount = 0;

    for(PXI8U i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}

PXResult PXAPI PXUserNameGet(PXText PXREF pxText)
{
    PXResult result;

#if OSUnix
    const int userID = getuid();
    struct passwd* pw = getpwuid();

#elif OSWindows

    DWORD length = pxText->SizeAllocated;
    BOOL success = FALSE;

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            success = GetUserNameA(pxText->A, &length); // Windows 2000, Advapi32.dll, winbase.h
            break;
        }
        case TextFormatUTF16:
        case TextFormatUNICODE:
        {
            success = GetUserNameW(pxText->W, &length); // Windows 2000, Advapi32.dll, winbase.h
            break;
        }
        case TextFormatInvalid:
        default:
            break;
    }

    result = PXErrorCurrent(success);

    pxText->SizeUsed = length;


    if(success)
    {
        PXTextDemote(pxText);
    }

#else
    result = PXResultRefusedNotImplemented;
#endif

    return result;
}

PXResult PXAPI PXComputerNameGet(char PXREF text, const PXSize textSizeMax, PXSize PXREF textSizeWritten)
{
#if OSUnix

#elif OSWindows

#else

#endif
}

PXResult PXAPI PXFilePathCleanse(const char* pathInput, char PXREF pathOutput, const PXSize pathOutputSizeMAX, PXSize PXREF pathOutputSizeWritten)
{
    PXResult pxResult;

#if OSUnix
    //realpath()
#elif OSWindows
    const DWORD length = GetFullPathNameA(pathInput, pathOutputSizeMAX, pathOutput, NULL); // Windows XP (+UWP), Kernel32.dll, fileapi.h

    pxResult = PXErrorCurrent(0 != length);

    if(pathOutputSizeWritten)
    {
        *pathOutputSizeWritten = length;
    }
#else
    pxResult = PXResultRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

void PXAPI PXTextUTF8ToUNICODE(wchar_t PXREF textOutput, const char PXREF textInput)
{
#if OSUnix
#elif OSWindows

  //  MultiByteToWideChar();

#else

#endif
}

void PXAPI PXTextUNICODEToUTF8(char PXREF textOutput, const wchar_t PXREF textInput)
{
#if OSUnix
#elif OSWindows

   // WideCharToMultiByte();

#else

#endif
}

void PXAPI PXProcessCurrent(PXProcess PXREF pxProcess)
{
    PXClear(PXProcess, pxProcess);

    pxProcess->ProcessHandle = PXProcessCurrentHandle();
    pxProcess->ProcessID = PXProcessCurrentID();
}

PXProcessHandle PXAPI PXProcessCurrentHandle()
{
    PXProcessHandle pxProcessHandle;

#if OSUnix
    pxProcessHandle.Data = open("/proc/self", O_RDONLY);
#elif OSWindows
    // Returns a pseudo handle to the current process. Its -1 but may change in feature versions.
    pxProcessHandle.Data = GetCurrentProcess(); // Windows 2000 SP4, Kernel32.dll, processthreadsapi.h
#else
    pxProcessHandle.Data = 0;
#endif

    return pxProcessHandle;
}

PXProcessID PXAPI PXProcessCurrentID()
{
    PXProcessID pxProcessID;

#if OSUnix
    pxProcessID.Data = getpid(); // Linux, (get) (p)rocess (id)entification
#elif OSWindows
    pxProcessID.Data = GetCurrentProcessId();
#else
    pxProcessID.Data = 0;
#endif

    return pxProcessID;
}

PXProcessID PXAPI PXProcessHandleToID(const PXProcessHandle pxProcessHandle)
{
    PXProcessID pxProcessID;

#if OSUnix

    // ???

#elif OSWindows

#if WindowsAtleastXP
    pxProcessID.Data = GetProcessId(pxProcessHandle.Data); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#else
    pxProcessID.Data = 0;
#endif

#endif

    return pxProcessID;
}

PXResult PXAPI PXProcessMemoryWrite
(
    const PXProcessHandle pxProcessHandle,
    void* baseAddress,
    const void* bufferData,
    const PXSize bufferSizeMax,
    PXSize PXREF bufferSizeWritten
)
{
    PXResult pxResult;

#if OSUnix
#elif OSWindows
    const BOOL successful = WriteProcessMemory // Windows XP, Kernel32.dll, memoryapi.h
    (
        pxProcessHandle.Data,
        baseAddress,
        bufferData,
        bufferSizeMax,
        bufferSizeWritten
    );
    pxResult = PXErrorCurrent(successful);
#else
    pxResult = PXResultRefusedNotImplemented;
#endif

    return pxResult;
}

PXResult PXAPI PXProcessMemoryRead
(
    const PXProcessHandle pxProcessHandle,
    const void* baseAddress,
    void PXREF bufferData,
    const PXSize bufferSizeMax,
    PXSize PXREF bufferSizeWritten
)
{
    PXResult pxResult;

#if OSUnix
#elif OSWindows
    const BOOL successful = ReadProcessMemory // Windows XP, Kernel32.dll, memoryapi.h
    (
        pxProcessHandle.Data,
        baseAddress,
        bufferData,
        bufferSizeMax,
        bufferSizeWritten
    );
    pxResult = PXErrorCurrent(successful);
#else
    pxResult = PXResultRefusedNotImplemented;
#endif

    return pxResult;
}

PXResult PXAPI PXMemoryHeapCreate(PXMemoryHeap PXREF pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    pxMemoryHeap->HeapHandle = HeapCreate(NULL, 0, 0); // Windows XP, Kernel32.dll, heapapi.h
#endif

    return PXResultOK;
}

PXResult PXAPI PXMemoryHeapRelease(PXMemoryHeap PXREF pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    const BOOL result = HeapDestroy(pxMemoryHeap->HeapHandle); // Windows XP, Kernel32.dll, heapapi.h
#endif

    return PXResultOK;
}

void PXAPI PXMemoryHeapGetGlobal(PXMemoryHeap PXREF pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    pxMemoryHeap->HeapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#endif

    pxMemoryHeap->Allocated = 0;
}

PXSize PXAPI PXMemoryHeapBlockSize(PXMemoryHeap* pxMemoryHeap, const void PXREF address)
{
    PXSize allocatedSize = 0;
    PXMemoryHeap redirectHeap;

    // HeapSize cant handle NULL pointers. If we get one here, it sure is 0.
    if(!address)
    {
        return 0;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix

    allocatedSize = malloc_usable_size(address); // Linux

#elif OSWindows

    #if PXMemoryUseFunctionSTD || OSForcePOSIXForWindows
    allocatedSize = _msize(address); // Windows CRT only
    #else
    allocatedSize = HeapSize(pxMemoryHeap->HeapHandle, 0, address);
    const PXResult sizeError = PXErrorCurrent((SIZE_T)-1 != allocatedSize);
    #endif
#endif

    return allocatedSize;
}

void* PXAPI PXMemoryHeapCalloc(PXMemoryHeap* pxMemoryHeap, const PXSize amount, const PXSize objectSize)
{
    const PXSize totalAmount = amount * objectSize;
    PXMemoryHeap redirectHeap;
    void* address = PXNull;

    if(!totalAmount)
    {
        return PXNull;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows
    address = calloc(amount, objectSize);
#elif OSWindows
    address = HeapAlloc(pxMemoryHeap->HeapHandle, HEAP_ZERO_MEMORY, totalAmount); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif

#if PXLogEnable && 0
    // Special logging behaviour
    {
        // PXSymbolMemory pxSymbolMemory;
        // pxSymbolMemory.Address = address;
        // pxSymbolMemory.Amount = amount;
         //pxSymbolMemory.ObjectSize = objectSize;

        PXSymbol pxSymbol;
        PXSymbolCaller(&pxSymbol);

        //  pxSymbolMemory.ModuleAddress = pxSymbol.ModuleAddress;
         // PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAddress, 64);
         // PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAddress, 64);
         // pxSymbolMemory.LineNumber = pxSymbol.LineNumber;

          //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);

        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Alloc",
            "<%p> %6ix %4i B %s::%s::%s::%i",
            address,
            amount,
            objectSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbol.LineNumber
        );
    }
#endif

    return address;
}

void* PXAPI PXMemoryHeapMalloc(PXMemoryHeap* pxMemoryHeap, const PXSize memorySize)
{
    PXMemoryHeap redirectHeap;
    void* address = PXNull;

    if(!memorySize)
    {
        return PXNull;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    address = malloc(memorySize);
#elif OSWindows
    address = HeapAlloc(pxMemoryHeap->HeapHandle, 0, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif

    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Address = address;
        pxSymbolMemory.Amount = 1;
        pxSymbolMemory.ObjectSize = memorySize;

        PXSymbol pxSymbol;
        PXSymbolCaller(&pxSymbol);

        pxSymbolMemory.ModuleAddress = pxSymbol.AddressModule;
        PXTextCopyA(pxSymbol.NameFile, sizeof(pxSymbol.NameFile), pxSymbolMemory.FileAddress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, sizeof(pxSymbol.NameSymbol), pxSymbolMemory.FunctionAddress, 64);
        pxSymbolMemory.LineNumber = pxSymbolMemory.LineNumber;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Alloc",
            "<%p> %4ix %4i B %s::%s::%s::%i",
            address,
            pxSymbolMemory.Amount,
            pxSymbolMemory.ObjectSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber
        );
#endif
    }

    return address;
}

PXBool PXAPI PXMemoryHeapFree(PXMemoryHeap* pxMemoryHeap, void PXREF address)
{
    PXMemoryHeap redirectHeap;

    if(!address)
    {
        return PXFalse;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if PXLogEnable
    const PXSize blockSize = PXMemoryHeapBlockSize(pxMemoryHeap, address);
#endif


#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    free(address);
#elif OSWindows
    const PXBool freeResult = HeapFree(pxMemoryHeap->HeapHandle, 0, address); // Windows 2000 SP4, Kernel32.dll, heapapi.h


    // Special logging behaviour
    {
#if PXLogEnable
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Address = address;
        pxSymbolMemory.Amount = -1;
        pxSymbolMemory.ObjectSize = -1;

        PXSymbol pxSymbol;
        PXSymbolCaller(&pxSymbol);

        pxSymbolMemory.ModuleAddress = pxSymbol.AddressModule;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAddress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAddress, 64);
        pxSymbolMemory.LineNumber = -1;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeRemove);

        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Free",
            "<%p> %4i B %s::%s::%s::%i",
            address,
            blockSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber
        );
#endif
    }

    return freeResult;

#else
#error Memory release seems not to be supported on this OS
#endif
}

void* PXAPI PXMemoryHeapRealloc(PXMemoryHeap* pxMemoryHeap, void PXREF address, const PXSize memorySize)
{
    PXMemoryHeap redirectHeap;
    void* newAddress = PXNull;
    PXBool updatedLocation = PXFalse;

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if PXLogEnable
    const PXSize blockSizeOLD = PXMemoryHeapBlockSize(pxMemoryHeap, address);
#endif


#if OSUnix || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows
    // Function allows NULL as an address
    newAddress = realloc(address, memorySize);

    // clear new allocated space
    PXSize spaceNewSize = memorySize - blockSizeOLD;
    char* spaceNewAddress = (char*)newAddress + blockSizeOLD;

    PXMemoryClear(spaceNewAddress, spaceNewSize);

#elif OSWindows

    if(!address)
    {
        void* memory = PXMemoryHeapCalloc(pxMemoryHeap, 1, memorySize);

        //PXMemorySet(memory, 0xFF, memorySize);

        return memory;
    }

    newAddress = HeapReAlloc(pxMemoryHeap->HeapHandle, HEAP_ZERO_MEMORY, address, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    updatedLocation = newAddress != address;

    // PXMemorySet(newAddress, '°', memorySize - oldSize);
#else
#error Memory reallocate seems not to be supported on this OS
#endif



#if PXLogEnable
    const PXSize blockSizeNEW = PXMemoryHeapBlockSize(pxMemoryHeap, newAddress);
    const PXOffset offset = blockSizeNEW - blockSizeOLD;
#endif


#if PXLogEnable
    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Address = address;
        pxSymbolMemory.Amount = memorySize;
        pxSymbolMemory.ObjectSize = 1;

        PXSymbol pxSymbol;
        PXSymbolCaller(&pxSymbol);

        pxSymbolMemory.ModuleAddress = pxSymbol.AddressModule;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAddress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAddress, 64);
        pxSymbolMemory.LineNumber = -1;

        PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeUpdate);

        if(updatedLocation)
        {
            PXLogPrint
            (
                PXLoggingAllocation,
                PXOSName,
                "Heap-ReAlloc",
                "<%p> %i B -> <%p> %i B, (%i), %s::%s::%s::%i",
                address,
                blockSizeOLD,
                newAddress,
                blockSizeNEW,
                offset,
                pxSymbol.NameModule,
                pxSymbol.NameFile,
                pxSymbol.NameSymbol,
                pxSymbolMemory.LineNumber
            );
        }
        else
        {
            PXLogPrint
            (
                PXLoggingAllocation,
                PXOSName,
                "Heap-ReAlloc",
                "<%p> %i B -> <No-Move> %i B, (%i), %s::%s::%s::%i",
                address,
                blockSizeOLD,
                blockSizeNEW,
                offset,
                pxSymbol.NameModule,
                pxSymbol.NameFile,
                pxSymbol.NameSymbol,
                pxSymbolMemory.LineNumber
            );
        }
    }
#endif

    return newAddress;
}



#if OSWindows
void PXAPI PXWindowsCheckPermission()
{
    // A call to VirtualAlloc() with MEM_LARGE_PAGES
       // WILL normally fail, because we dont have permissions..?
       // We have permissions, it is only disabled by default.

    static PXBool isChecked = 0;

      // AddPrivileges();

    if(isChecked)
    {
        return;
    }

    TOKEN_PRIVILEGES privileges;
    HANDLE hToken;
    LUID luid;

    BOOL result = 0;
    PXResult pxResult = PXResultInvalid;

    // Open the process token
    result = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    pxResult = PXErrorCurrent(result);


    result = LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &luid); // SeLockMemoryPrivilege, NOT THREAD SAFE!
    pxResult = PXErrorCurrent(result);

    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Luid = luid;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    result = AdjustTokenPrivileges
    (
        hToken,
        FALSE,
        &privileges,
        sizeof(TOKEN_PRIVILEGES),
        PXNull,
        PXNull
    );
    pxResult = PXErrorCurrent(result);

    result = CloseHandle(hToken);

    isChecked = 1;
}
#endif

void* PXAPI PXMemoryVirtualAllocate(const PXSize size, PXSize PXREF createdSize, const PXAccessMode pxAccessMode)
{
    // Some systems actually support zero sized allocation.
    // We don't.
    if(0 == size)
    {
        return PXNull;
    }

    PXMemoryPageAllocInfo pxMemoryPageAllocInfo;
    PXClear(PXMemoryPageAllocInfo, &pxMemoryPageAllocInfo);

    PXMemoryPageInfoFetch(&pxMemoryPageAllocInfo.PageFileInfo, size);

    // Calculate if large pages shall be used
#if PXLogEnable
    const char* text = 0;

    switch(pxMemoryPageAllocInfo.PageFileInfo.Affinity)
    {
        case PXMemoryPageNormal:
            text = "Normal";
            break;
        case PXMemoryPagePhysical:
            text = "Physical";
            break;
        case PXMemoryPageLarge:
            text = "Large";
            break;
        case PXMemoryPageHuge:
            text = "Huge";
            break;

        default:
            break;
    }


    PXText pxTextPageSizeNormal;
    PXText pxTextPageSizePhysical;
    PXText pxTextPageSizeLarge;
    PXText pxTextPageSizeHuge;

    PXTextConstructNamedBufferA(&pxTextPageSizeNormal, pxTextPageSizeNormalBuffer, 32);
    PXTextConstructNamedBufferA(&pxTextPageSizePhysical, pxTextPageSizePhysicalBuffer, 32);
    PXTextConstructNamedBufferA(&pxTextPageSizeLarge, pxTextPageSizeLargeBuffer, 32);
    PXTextConstructNamedBufferA(&pxTextPageSizeHuge, pxTextPageSizeHugeBuffer, 32);

    PXTextFormatSize(&pxTextPageSizeNormal, pxMemoryPageAllocInfo.PageFileInfo.PageSizeNormal);
    PXTextFormatSize(&pxTextPageSizePhysical, pxMemoryPageAllocInfo.PageFileInfo.PageSizePhysical);
    PXTextFormatSize(&pxTextPageSizeLarge, pxMemoryPageAllocInfo.PageFileInfo.PageSizeLarge);
    PXTextFormatSize(&pxTextPageSizeHuge, pxMemoryPageAllocInfo.PageFileInfo.PageSizeHuge);

    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Virtual-Alloc",
        "Allocating space for %i...\n"
        "%20s : %7s -> %3i%% %6ix\n"
        "%20s : %7s -> %3i%% %6ix\n"
        "%20s : %7s -> %3i%% %6ix\n"
        "%20s : %7s -> %3i%% %6ix\n"
        "%20s : %s",
        size,
        "Normal-PageSize", pxTextPageSizeNormal.A, (int)pxMemoryPageAllocInfo.PageFileInfo.PageUtilizationNormal, pxMemoryPageAllocInfo.PageFileInfo.PageAmountNormal,
        "Physical-PageSize", pxTextPageSizePhysical.A, (int)pxMemoryPageAllocInfo.PageFileInfo.PageUtilizationPhysical, pxMemoryPageAllocInfo.PageFileInfo.PageAmountPhysical,
        "Large-PageSize", pxTextPageSizeLarge.A, (int)pxMemoryPageAllocInfo.PageFileInfo.PageUtilizationLarge, pxMemoryPageAllocInfo.PageFileInfo.PageAmountLarge,
        "Huge-PageSize", pxTextPageSizeHuge.A, (int)pxMemoryPageAllocInfo.PageFileInfo.PageUtilizationHuge, pxMemoryPageAllocInfo.PageFileInfo.PageAmountHuge,
        "Targeted type", text
    );
#endif

#if OSUnix
    // PROT_READ, PROT_WRITE = read&write permission
    // MAP_PRIVATE           = only for this process
    // MAP_ANONYMOUS         = no attached file, aka. "in memory"
    // MAP_POPULATE          = Preallocate, if not, we could get a SEGVAULT later if memory is suddenly not avalible
    pxMemoryPageAllocInfo.Permission =
        PROT_READ |
        PROT_WRITE;

    pxMemoryPageAllocInfo.Mode =
        MAP_PRIVATE |
        MAP_ANONYMOUS |
        MAP_POPULATE; // Will be ignored if not spesifcally enabled

#if 0 // MAP_UNINITIALIZED not public??
    if(0) // Dont clear memory, can improve performance but will be ignored if not directly enabled. Safty reasons.
    {
        mode |= MAP_UNINITIALIZED;
    }
#endif

#elif OSWindows

    pxMemoryPageAllocInfo.Permission =
        PAGE_READWRITE;

    pxMemoryPageAllocInfo.Mode =
        MEM_RESERVE |
        MEM_COMMIT;

    pxMemoryPageAllocInfo.CurrentProcess = GetCurrentProcess();
    pxMemoryPageAllocInfo.param.Type = MemExtendedParameterAttributeFlags;

    PXWindowsCheckPermission();
#endif

    //-----------------------------------------------------
    // STAGE 2: Try to allocate a page with the best method, try every solution until somehting works.
    while(!pxMemoryPageAllocInfo.Address)
    {
        if(PXMemoryPageNone == pxMemoryPageAllocInfo.PageFileInfo.Affinity)
        {
            break;
        }

        switch(pxMemoryPageAllocInfo.PageFileInfo.Affinity)
        {
            case PXMemoryPageNormal:
            {
                PXMemoryVirtualAllocateNormal(&pxMemoryPageAllocInfo);
                break;
            }
            case PXMemoryPagePhysical:
            {
                PXMemoryVirtualAllocatePhysical(&pxMemoryPageAllocInfo);
                break;
            }
            case PXMemoryPageLarge:
            {
                PXMemoryVirtualAllocateLarge(&pxMemoryPageAllocInfo);
                break;
            }
            case PXMemoryPageHuge:
            {
                PXMemoryVirtualAllocateHuge(&pxMemoryPageAllocInfo);
                break;
            }
        }
    }
    //-----------------------------------------------------


    if(PXResultOK != pxMemoryPageAllocInfo.Result)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOSName,
            PXOSVirtualAllocText,
            "failed! -> VirtualAlloc()"
        );
#endif

        return PXNull;
    }

    // Alloc successful, lengh is atlest our data
    pxMemoryPageAllocInfo.SizeCreated = pxMemoryPageAllocInfo.SizeRequested;

    //-----------------------------------------------------
    // The OS allocates sometimes more than we request,
    // we ask for the value it has used.
    PXMemoryAddressInfo pxMemoryAddressInfo;

    PXResult pxResult = PXMemoryAddressInfoGet(&pxMemoryAddressInfo, pxMemoryPageAllocInfo.Address);

    if(PXResultOK != pxResult)
    {
        // ToDo: Handle?
    }
    //-----------------------------------------------------


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingAllocation,
        PXOSName,
        PXOSVirtualAllocText,
        "OK!\n"
        "%20s : %p\n"
        "%20s : %lu\n"
        "%20s : %lu",
        "Address", pxMemoryPageAllocInfo.Address,
        "Requested", pxMemoryPageAllocInfo.SizeRequested,
        "Got", pxMemoryPageAllocInfo.SizeCreated
    );
#endif

    if(createdSize)
    {
        *createdSize = pxMemoryPageAllocInfo.SizeCreated;
    }

    return pxMemoryPageAllocInfo.Address;
}

void PXAPI PXMemoryVirtualAllocateNormal(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo)
{
    pxMemoryPageAllocInfo->SizeRequested =
        pxMemoryPageAllocInfo->PageFileInfo.PageSizeNormal *
        pxMemoryPageAllocInfo->PageFileInfo.PageAmountNormal;

#if OSUnix
    pxMemoryPageAllocInfo->Address = mmap
    (
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Permission,
        pxMemoryPageAllocInfo->Mode,
        -1,
        0
    );
#elif OSWindows
    pxMemoryPageAllocInfo->Address = VirtualAlloc
    (
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Mode,
        pxMemoryPageAllocInfo->Permission
    );
#endif

    pxMemoryPageAllocInfo->Result = PXErrorCurrent(NULL != pxMemoryPageAllocInfo->Address);
}

void PXAPI PXMemoryVirtualAllocatePhysical(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo)
{
    pxMemoryPageAllocInfo->SizeRequested =
        pxMemoryPageAllocInfo->PageFileInfo.PageSizePhysical *
        pxMemoryPageAllocInfo->PageFileInfo.PageAmountPhysical;

#if OSUnix
    pxMemoryPageAllocInfo->Address = mmap
    (
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Permission,
        pxMemoryPageAllocInfo->Mode,
        -1,
        0
    );
#elif OSWindows
    pxMemoryPageAllocInfo->Address = VirtualAlloc2
    (
        pxMemoryPageAllocInfo->CurrentProcess,
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Mode | MEM_64K_PAGES,
        pxMemoryPageAllocInfo->Permission,
        0,
        0
    );
#endif

    pxMemoryPageAllocInfo->Result = PXErrorCurrent(NULL != pxMemoryPageAllocInfo->Address);

    if(PXResultOK != pxMemoryPageAllocInfo->Result)
    {
        pxMemoryPageAllocInfo->PageFileInfo.Affinity = PXMemoryPageNormal; // Demote

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOSName,
            PXOSVirtualAllocText,
            "Failed! Demoting to <%s>",
            "Normal"
        );
#endif
    }

    return pxMemoryPageAllocInfo->Result;
}

void PXAPI PXMemoryVirtualAllocateLarge(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo)
{
    // Adjust allocation size to be EXACTLY a multible of the page size
    // The documentation states it will be rounded up, this is true.
    // Except if we add the MEM_LARGE_PAGES flag, because microsoft.
    pxMemoryPageAllocInfo->SizeRequested =
        pxMemoryPageAllocInfo->PageFileInfo.PageSizeLarge *
        pxMemoryPageAllocInfo->PageFileInfo.PageAmountLarge;

#if OSUnix
    int mode = pxMemoryPageAllocInfo->Mode;
#if (defined(MAP_HUGETLB) && defined(MAP_HUGE_2MB))
    mode |= MAP_HUGETLB | MAP_HUGE_2MB;
#endif

    pxMemoryPageAllocInfo->Address = mmap
    (
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Permission,
        mode,
        -1,
        0
    );
#elif OSWindows
    pxMemoryPageAllocInfo->Address = VirtualAlloc
    (
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Mode | MEM_LARGE_PAGES,
        pxMemoryPageAllocInfo->Permission
    );
#endif
    pxMemoryPageAllocInfo->Result = PXErrorCurrent(NULL != pxMemoryPageAllocInfo->Address);

    if(PXResultOK != pxMemoryPageAllocInfo->Result)
    {
        pxMemoryPageAllocInfo->PageFileInfo.Affinity = PXMemoryPagePhysical; // Demote

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOSName,
            PXOSVirtualAllocText,
            "Failed! Demoting to <%s>",
            "Physical"
        );
#endif
    }
}

void PXAPI PXMemoryVirtualAllocateHuge(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo)
{
    pxMemoryPageAllocInfo->SizeRequested =
        pxMemoryPageAllocInfo->PageFileInfo.PageSizeHuge *
        pxMemoryPageAllocInfo->PageFileInfo.PageAmountHuge;

#if OSUnix
    int mode = pxMemoryPageAllocInfo->Mode;
#if (defined(MAP_HUGETLB) && defined(MAP_HUGE_2MB))
    mode |= MAP_HUGETLB | MAP_HUGE_1GB;
#endif

    pxMemoryPageAllocInfo->Address = mmap
    (
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Permission,
        mode,
        -1,
        0
    );
#elif OSWindows
    // NOW: Try allocating huge page..
    pxMemoryPageAllocInfo->param.ULong64 = MEM_EXTENDED_PARAMETER_NONPAGED_HUGE; // TODO: Fails with "invalid parameter"

    pxMemoryPageAllocInfo->Address = VirtualAlloc2
    (
        pxMemoryPageAllocInfo->CurrentProcess,
        pxMemoryPageAllocInfo->AddressBase,
        pxMemoryPageAllocInfo->SizeRequested,
        pxMemoryPageAllocInfo->Mode | MEM_LARGE_PAGES,
        pxMemoryPageAllocInfo->Permission,
        0,
        0
    );
#endif

    pxMemoryPageAllocInfo->Result = PXErrorCurrent(NULL != pxMemoryPageAllocInfo->Address);

    if(PXResultOK != pxMemoryPageAllocInfo->Result)
    {
        pxMemoryPageAllocInfo->PageFileInfo.Affinity = PXMemoryPageLarge; // Demote


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOSName,
            PXOSVirtualAllocText,
            "Failed! Demoting to <%s>",
            "Large"
        );
#endif
    }
}

void PXAPI PXMemoryVirtualPrefetch(const void* address, const PXSize size)
{
    PXOS PXREF pxOS = PXSystemGet();

#if OSUnix
#elif OSWindows

#if WindowsAtleast8 && PXOSWindowsDestop
    const HANDLE process = GetCurrentProcess();
    const PXSize numberOfEntries = 1;
    WIN32_MEMORY_RANGE_ENTRY memoryRangeEntry;
    PXClear(WIN32_MEMORY_RANGE_ENTRY, &memoryRangeEntry);

    const PXSize flags = 0; // reserved and needs to be 0

    memoryRangeEntry.VirtualAddress = (void*)address;
    memoryRangeEntry.NumberOfBytes = size;

    const BOOL prefetchResult = 1;// PrefetchVirtualMemory(process, numberOfEntries, &memoryRangeEntry, flags); // Windows 8, Kernel32.dll, memoryapi.h

#if PXLogEnable
    PXText pxTextSize;
    PXTextConstructNamedBufferA(&pxTextSize, pxTextBuffer, 32);
    PXTextFormatSize(&pxTextSize, size);

    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Prefetch",
        "<%p> %s",
        address,
        pxTextSize.A
    );
#endif

#else
    // Not supported function
#endif

#if 1
    // Memory pages are the smalest entity in the virtual space.
    // Touching even one byte will trigger a load on the whole page.

    volatile PXByte sink = 0;

    for(PXSize i = 0; i < size; i += pxOS->PageSizeNormal)
    {
        sink ^= ((PXByte*)address)[i]; // Touch one byte per page
    }
#endif

#else

#if MemoryDebug
    printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched [NOT SUPPORTED] Skipped...\n", address, size);
#endif
#endif
}

PXResult PXAPI PXMemoryVirtualRelease(const void* address, const PXSize size)
{
    if(!(address && size))
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable && PXMemoryDebug
    PXLogPrint
    (
        PXLoggingDeallocation,
        PXOSName,
        "Virtual-Free",
        "<%p> Size:%i",
        address,
        size
    );
#endif

#if OSUnix
    const int resultID = munmap(address, size); // sys/mman.h
    const PXResult unmapResult = PXErrorCurrent(0 == resultID);

    return unmapResult;

#elif OSWindows
    DWORD freeType = MEM_RELEASE;
    const PXBool freeResultID = VirtualFree((void*)address, 0, freeType); // Windows XP (+UWP), Kernel32.dll, memoryapi.h
    const PXResult freeResult = PXErrorCurrent(freeResultID);

    return freeResult;
#else
    return PXResultRefusedNotSupportedByLibrary;
#endif
}

void* PXAPI PXMemoryVirtualReallocate(const void* address, const PXSize size)
{
    const PXBool newAllocation = address == PXNull;

    if(newAllocation)
    {
        return PXMemoryVirtualAllocate(size, PXNull, PXAccessModeReadAndWrite);
    }

#if OSUnix
    return PXNull;
#elif OSWindows

    // Create new pages
    void* newSpaceMemory = PXMemoryVirtualAllocate(size, PXNull, PXAccessModeReadAndWrite);

    // Get size of current address
    MEMORY_BASIC_INFORMATION memoryInfo;
    VirtualQuery(address, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    // Copy data to new pages
    PXMemoryCopy(address, newSpaceMemory, memoryInfo.RegionSize);

    // Delete old pages
    PXMemoryVirtualRelease(address, memoryInfo.RegionSize);

    return newSpaceMemory;
#else
    return PXNull;
#endif

}

PXResult PXAPI PXMemoryVirtualInfoViaAddress(PXMemoryVirtualInfo PXREF pxMemoryVirtualInfo, const void* address)
{
#if OSUnix
#elif OSWindows

    // Get size of current address
    MEMORY_BASIC_INFORMATION memoryInfo;
    const SIZE_T size = VirtualQuery(address, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    pxMemoryVirtualInfo->BaseAddress = memoryInfo.BaseAddress;
    pxMemoryVirtualInfo->AllocationBase = memoryInfo.AllocationBase;
    pxMemoryVirtualInfo->RegionSize = memoryInfo.RegionSize;
    pxMemoryVirtualInfo->ProtectionAtCreation = memoryInfo.AllocationProtect;
    pxMemoryVirtualInfo->ProtectionCurrent = memoryInfo.Protect;
    pxMemoryVirtualInfo->State = memoryInfo.State;
    pxMemoryVirtualInfo->Type = memoryInfo.Type;

#if OS64B
    pxMemoryVirtualInfo->PartitionID = memoryInfo.PartitionId;
#endif

#if 0
    switch(memoryInfo.AllocationProtect)
    {
        case PAGE_NOACCESS: { break; }
        case PAGE_READONLY: { break; }
        case PAGE_READWRITE: { break; }
        case PAGE_WRITECOPY: { break; }
        case PAGE_EXECUTE: { break; }
        case PAGE_EXECUTE_READ: { break; }
        case PAGE_EXECUTE_READWRITE: { break; }
        case PAGE_EXECUTE_WRITECOPY: { break; }
        case PAGE_TARGETS_INVALID: { break; }
        case PAGE_TARGETS_NO_UPDATE: { break; }
        case PAGE_GUARD: { break; }
        case PAGE_NOCACHE: { break; }
        case PAGE_WRITECOMBINE: { break; }
        case PAGE_ENCLAVE_DECOMMIT: { break; }
        case PAGE_ENCLAVE_THREAD_CONTROL: { break; }
        case PAGE_ENCLAVE_UNVALIDATED: { break; }

        default:
            PXDebugBreak();
            break;
    }
#endif

#else

#endif
}


typedef struct PXStackFrame_
{
    PXProcessHandle pxProcessHandle;
    PXProcessID pxProcessID;
    PXThreadHandle pxThreadHandle;
    PXThreadID pxThreadID;

    PXSize FrameAmount;
    PXSymbol SymbolList[PXStackFrameAmountMax];

#if OSUnix
    void* FrameList[PXStackFrameAmountMax];
#elif OSWindows
    STACKFRAME64 frame;
#endif
}
PXStackFrame;


void PXAPI PXDebugStackTrace(void)
{
    PXStackFrame pxStackFrame;
    PXClear(PXStackFrame, &pxStackFrame);
    pxStackFrame.pxProcessHandle = PXProcessCurrentHandle();
    pxStackFrame.pxProcessID = PXProcessCurrentID();
    pxStackFrame.pxThreadHandle = PXThreadCurrentHandle();
    pxStackFrame.pxThreadID = PXThreadCurrentID();

    PXSymbolStackTrace(pxStackFrame.SymbolList, PXStackFrameAmountMax);

    for(PXSize i = 0; i < pxStackFrame.FrameAmount; ++i)
    {
        PXSymbol* pxSymbol = &pxStackFrame.SymbolList[i];

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "StackTrace",
            "~~>",
            "%s:%s()+0x%lx",
            pxSymbol->NameModule,
            pxSymbol->NameSymbol,
            pxSymbol->Offset
        );
#endif
    }

    PXDebugBreak();
}

void PXAPI PXDebugStackTraceWithSignal(PXSignalInfo PXREF pxSignalInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "StackTrace",
        "Error-State",
        "\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %p\n"
        "%20s : %i",
        "ID", pxSignalInfo->ID,
        "PXResult", pxSignalInfo->Result,
        "ThreadID", pxSignalInfo->ThreadID,
        "ProcessID", pxSignalInfo->ProcessID,
        "FaultAddress", pxSignalInfo->FaultAddress,
        "ParameterAmount", pxSignalInfo->ParameterAmount
    );

    PXLogPrint
    (
        PXLoggingInfo,
        "StackTrace",
        "CPU-State",
        "\n"
        "%20s : %llx (s)\n"
        "%20s : %llx (s)\n"
        "%20s : %llx (s)\n"
        "%20s : %llx (s)\n"
        "%20s : %llx (s)\n"
        "%20s : %llx (s)",
        "RAX", pxSignalInfo->Context.X64.RAX, "Accumulator",
        "RBX", pxSignalInfo->Context.X64.RBX, "Base register",
        "RCX", pxSignalInfo->Context.X64.RCX, "Counter register",
        "RDX", pxSignalInfo->Context.X64.RDX, "Data register ",
        "RSI", pxSignalInfo->Context.X64.RSI, "Source index register",
        "RDI", pxSignalInfo->Context.X64.RDI, "Destination index register",
        "RBP", pxSignalInfo->Context.X64.RBP, "Base pointer register.",
        "RSP", pxSignalInfo->Context.X64.RSP, "Stack pointer register."
    );

#endif

    PXDebugStackTrace();
}

PXResult PXAPI PXSymbolServerInitialize()
{
    PXOS PXREF pxOS = PXSystemGet();

    if(!pxOS->Init)
    {
        return PXResultRefusedNotInitialized;
    }

#if OSUnix
#elif OSWindows
    const PXSymInitializeFunction pxSymbolServerInitialize = (PXSymInitializeFunction)_PXOS.SymbolServerInitialize;

   // PXThread pxThread;
   // PXThreadCurrent(&pxThread);
   // pxDebug->Process.ThreadHandle = pxThread.Info.Handle.ThreadHandle;

    HANDLE hProcess = GetCurrentProcess(); // Doc says this is not good?

    // Initializes the symbol handler for a process.
    const PCSTR UserSearchPath = NULL;
    const BOOL fInvadeProcess = TRUE;
    const BOOL initializeSuccessful = pxSymbolServerInitialize(hProcess, UserSearchPath, fInvadeProcess); // Dbghelp.dll, Dbghelp.h, SymInitialize
   // const PXResult result = PXErrorCurrent(initializeSuccessful);

    if(!initializeSuccessful)
    {
        return PXResultInvalid;
    }

   // const HANDLE processHandle = GetCurrentProcess();
   // const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);
  //  DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES


   // const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);
#endif

    return PXResultOK;
}

PXResult PXAPI PXSymbolServerCleanup(void)
{
#if OSUnix
#elif OSWindows
    HANDLE processID = GetCurrentProcess();
    const PXSymCleanupFunction pxSymCleanup = (PXSymCleanupFunction)_PXOS.SymbolServerCleanup;
    const PXBool cleanupSuccess = pxSymCleanup(processID); // DbgHelp.dll 5.1 or later

    if(!cleanupSuccess)
    {
        return PXResultFailedCleanup;
    }
#endif

    return PXResultFailedLoad;
}

PXResult PXAPI PXSymbolServerOptionsSet(void)
{
   // DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXSymbolListLoad(const PXProcessHandle processHandle, const void* baseAddress)
{
#if OSUnix


#elif OSWindows

    PXSymbolEnumerator pxSymbolEnumerator;
    pxSymbolEnumerator.SymbolList = 0;
    pxSymbolEnumerator.Amount = 0;
    pxSymbolEnumerator.Size = 128;

    ULONG64 baseAddressOfDLL = (ULONG64)baseAddress;

    // SymEnumSym, SymEnumerateSymbols64 is outdated?
    const PXSymEnumSymbolsFunction pxSymbolEnumerate = (PXSymEnumSymbolsFunction)_PXOS.SymbolEnumerate; // DbgHelp.dll 5.1 or later
    const PXBool enumerateResult = pxSymbolEnumerate
    (
        processHandle.Data,
        baseAddressOfDLL,
        0,
        PXLibraryNameSymbolEnumerate,
        &pxSymbolEnumerator
    );

#else

#endif

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXSymbolModuleLoad(const PXProcessHandle processHandle, const char* moduleName, void** baseAddress)
{
#if OSUnix
#elif OSWindows
    const PXSymLoadModuleFunction pxSymbolModuleLoad = (PXSymLoadModuleFunction)_PXOS.SymbolModuleLoad;
    const DWORD64 baseOfDll = pxSymbolModuleLoad // DbgHelp.dll 5.1 or later        SymLoadModuleEx, DbgHelp.dll 6.0 or later
    (
        processHandle.Data,
        PXNull,
        moduleName,
        PXNull,
        0,
        0
    );

    const PXBool wasLoadingSuccessful = baseOfDll != 0;

    if(!wasLoadingSuccessful)
    {

    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXSymbolStackWalk(PXSymbolStackWalkInfo PXREF pxSymbolStackWalkInfo)
{
#if OSUnix
#elif OSWindows
     const PXStackWalk64Function pxStackWalk64 = (PXStackWalk64Function)_PXOS.SymbolStackWalk;
// const PXSymGetSymFromAddr64 pXSymGetSymFromAddr64 = (PXSymGetSymFromAddr64)pxDebug->SymbolFromAddress;

    const BOOL result = pxStackWalk64
    (
        pxSymbolStackWalkInfo->MachineType,
        pxSymbolStackWalkInfo->HandleProcessCurrent,
        pxSymbolStackWalkInfo->HandleThreadCurrent,
        &pxSymbolStackWalkInfo->StackFrame,
        &pxSymbolStackWalkInfo->ContextRecord,
        NULL,
        pxSymbolStackWalkInfo->SymbolFunctionTableAccess,
        pxSymbolStackWalkInfo->SymbolGetModuleBase,
        NULL
    );
    const PXBool sucessfull = result != 0;
#endif

    return PXResultOK;
}

PXResult PXAPI PXSymbolStackTrace(PXSymbol PXREF pxSymbolList, const PXSize pxSymbolListAmount)
{
    PXMemoryClear(pxSymbolList, sizeof(PXSymbol) * pxSymbolListAmount);

#if OSUnix
    void *buffer[64];
    const PXI8U amountOfBuffers = sizeof(buffer) / sizeof(void*);
    const PXI8U minAmount = PXMathMinimumIU(amountOfBuffers, pxSymbolListAmount);

    PXSize frameAmount = backtrace(buffer, minAmount);

    for(PXSize i = 0; i < frameAmount; ++i)
    {
        PXSymbol* pxSymbol = &pxSymbolList[i];
        void* frameAddress = buffer[i];

        PXSymbolFromAddress(pxSymbol, frameAddress);
    }

#elif OSWindows // PXOSWindowsDestop

    // Prevent failure because not inizilized
    const PXResult initializeResult = PXSymbolServerInitialize();

    if(PXResultOK != initializeResult)
    {
        for(PXSize i = 0; i < pxSymbolListAmount; i++)
        {
            PXSymbol* pxSymbol = &pxSymbolList[i];

            PXTextCopyA("???", 3, pxSymbol->NameUndecorated, 64);
            PXTextCopyA("???", 3, pxSymbol->NameSymbol, 64);
            PXTextCopyA("???.c", 3, pxSymbol->NameFile, 64);
            PXTextCopyA("???.dll", 3, pxSymbol->NameModule, 64);
        }

        return PXResultInvalid;
    }

    const DWORD optionsMask = SymSetOptions(SYMOPT_DEBUG | SYMOPT_LOAD_ANYTHING | SYMOPT_LOAD_LINES);

    // SymLoadModuleEx(,);

    PXSymbolStackWalkInfo pxSymbolStackWalkInfo;
    PXClear(PXSymbolStackWalkInfo, &pxSymbolStackWalkInfo);

    pxSymbolStackWalkInfo.MachineType = IMAGE_FILE_MACHINE_AMD64; // IMAGE_FILE_MACHINE_I386

    RtlCaptureContext(&pxSymbolStackWalkInfo.ContextRecord);
    pxSymbolStackWalkInfo.StackFrame.AddrPC.Mode = AddrModeFlat;
    pxSymbolStackWalkInfo.StackFrame.AddrStack.Mode = AddrModeFlat;
    pxSymbolStackWalkInfo.StackFrame.AddrFrame.Mode = AddrModeFlat;

#if OS32B
    pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset = pxSymbolStackWalkInfo.ContextRecord.Eip;
    pxSymbolStackWalkInfo.StackFrame.AddrStack.Offset = pxSymbolStackWalkInfo.ContextRecord.Esp;
    pxSymbolStackWalkInfo.StackFrame.AddrFrame.Offset = pxSymbolStackWalkInfo.ContextRecord.Ebp;
#elif OS64B
    pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset = pxSymbolStackWalkInfo.ContextRecord.Rip;
    pxSymbolStackWalkInfo.StackFrame.AddrStack.Offset = pxSymbolStackWalkInfo.ContextRecord.Rsp;
    pxSymbolStackWalkInfo.StackFrame.AddrFrame.Offset = pxSymbolStackWalkInfo.ContextRecord.Rbp;
#endif

    for(PXSize symbolIndex = 0; symbolIndex < pxSymbolListAmount; ++symbolIndex)
    {
        PXSymbol* pxSymbol = &pxSymbolList[symbolIndex];
        const PXResult result = PXSymbolStackWalk(&pxSymbolStackWalkInfo);

        if(PXResultOK != result)
        {
            break;
        }

        void* symbolAddress = (ULONG64)pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset; // ULONG64

        PXSymbolFromAddress(pxSymbol, symbolAddress);

        PXText pxText;
        PXTextFromAddressA(&pxText, pxSymbol->NameModule, 0, 64);
        PXLibraryNameFromAddress(PXNull, &pxText, pxSymbol->AddressModule); // PXDebugModuleNameShort

#if PXLogEnable  && PXDebugStackTracePrint
        PXLogPrint
        (
            PXLoggingInfo,
            PXDebugerText,
            PXDebugerStackTraceText,
            "%c 0x%p - %23s - %s",
            (frame == 0 ? '/' : '|'),
            symbolAddress,
            pxSymbol->NameModule,
            pxSymbol->NameUndecorated
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




#if PXLogEnable && PXDebugStackTracePrint && 0
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
        char address[64];

        sprintf_s(functioName, 64, "%s()", pxMSDebugSymbol.Symbol.Name);
        sprintf_s(address, 64, "0x%p", (void*)pxMSDebugSymbol.Symbol.Address);

        printf("| %-34s | %-18s |\n", functioName, address);
#endif
#endif
    }


#if PXLogEnable && PXDebugStackTracePrint
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        PXDebugerStackTraceText,
        "%c %s %i Jumps",
        '^',
        "(START)",
        frame
    );
#endif

#if PXLogEnable && PXDebugStackTracePrint
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        PXDebugerStackTraceText,
        "----- END -----"
    );
#endif

#endif

    return PXResultOK;
}

PXResult PXAPI PXSymbolCaller(PXSymbol PXREF pxSymbol)
{
    PXSymbol pxSymbolList[4];
    const PXSize pxSymbolListAmount = sizeof(pxSymbolList) / sizeof(PXSymbol);

#if 0
    PXSymbolStackTrace(pxSymbolList, pxSymbolListAmount);

    *pxSymbol = pxSymbolList[3];
#else
    PXClear(PXSymbol, pxSymbol);
    PXTextCopyA("???", 3, pxSymbol->NameUndecorated, 64);
    PXTextCopyA("???", 3, pxSymbol->NameSymbol, 64);
    PXTextCopyA("???", 3, pxSymbol->NameFile, 64);
    PXTextCopyA("???", 3, pxSymbol->NameModule, 64);
#endif

    return PXResultOK;
}

PXResult PXAPI PXSymbolUnDecorateName(const char* inputName, char* name, const PXSize nameLengthMax, PXSize* nameLengthWritten)
{
    PXResult pxResult = PXResultInvalid;

    if(!(inputName && name && nameLengthMax))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows

    const PXUnDecorateSymbolNameFunction pxUnDecorateSymbolName = (PXUnDecorateSymbolNameFunction)_PXOS.SymbolUnDecorateName;

    if(!pxUnDecorateSymbolName)
    {
        return PXResultRefusedNoSuchFunction;
    }

    const DWORD decResultSize = pxUnDecorateSymbolName(inputName, (PSTR)name, nameLengthMax, UNDNAME_COMPLETE); // UnDecorateSymbolName
    pxResult = PXErrorCurrent(0 != decResultSize);

    if(nameLengthWritten)
    {
        nameLengthWritten = 0;
    }
#endif

    return pxResult;
}

PXResult PXAPI PXSymbolFromAddress(PXSymbol PXREF pxSymbol, const void PXREF address)
{
    if(!(pxSymbol && address))
    {
        return PXResultRefusedParameterNull;
    }

    PXClear(PXSymbol, pxSymbol);

    char moduleNameBuffer[260];
    PXText moduleNameCurrent;
    PXTextFromAddressA(&moduleNameCurrent, moduleNameBuffer, 0, sizeof(moduleNameBuffer));
    // The path of our current running executable
    PXModuleCurrentPath(&moduleNameCurrent);

    PXText moduleNameSymbol;

#if OSUnix

    Dl_info info;
    PXBool hasResult = dladdr(address, &info); // linux, dlfcn.h

    if(hasResult)
    {
        PXSize written = 0;

        pxSymbol->AddressSymbol = info.dli_saddr;
        pxSymbol->AddressModule = info.dli_fbase;
        pxSymbol->Offset = PXMemoryAddressStartEndToSize(info.dli_saddr, address);

        PXSize textLength = PXTextLengthA(info.dli_fname, PXTextLengthUnkown);
        PXTextFromAddressA(&moduleNameSymbol, info.dli_fname, textLength, textLength);

        //-------------------------------------------------
        // Module
        PXBool isCurrentModule = PXModuleInSameDirectory(&moduleNameCurrent, &moduleNameSymbol);

        if(isCurrentModule)
        {
            written = PXTextCopyA
            (
                moduleNameSymbol.A + moduleNameCurrent.SizeUsed, moduleNameSymbol.SizeUsed - moduleNameCurrent.SizeUsed,
                pxSymbol->NameModule, sizeof(pxSymbol->NameModule)
            );
        }
        else
        {
            written = PXTextCopyA
            (
                moduleNameSymbol.A, moduleNameSymbol.SizeUsed,
                pxSymbol->NameModule, sizeof(pxSymbol->NameModule)
            );
        }

        if(0 == written)
        {
            written = PXTextCopyA
            (
                "???", 3,
                pxSymbol->NameModule, sizeof(pxSymbol->NameModule)
            );
        }
        //-------------------------------------------------

        //-------------------------------------------------
        // Symbol
        written = PXTextCopyA
        (
            info.dli_sname, PXTextLengthUnkown,
            pxSymbol->NameSymbol, sizeof(pxSymbol->NameSymbol)
        );

        if(0 == written)
        {
            written = PXTextCopyA
            (
                PXUnknownElement, sizeof(PXUnknownElement),
                pxSymbol->NameModule, sizeof(pxSymbol->NameModule)
            );
        }
        //-------------------------------------------------
    }
    else
    {
       // dprintf(STDERR_FILENO, "  at ???:??? (%p)\n", addr);
    }


#elif OSWindows

    const HANDLE processHandle = GetCurrentProcess(); // TODO: what if we want another process?

    // Extract symbol
    {
        DWORD64 displacement = 0;

        union
        {
            char buffer[sizeof(SYMBOL_INFO) + sizeof(TCHAR) * 200]; // MAX_SYM_NAME
            SYMBOL_INFO SymbolInfo;
        }
        symbolInfo;


        symbolInfo.SymbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
        symbolInfo.SymbolInfo.MaxNameLen = 200;

        const PXBool symbolFetchSuccess = SymFromAddr
        (
            processHandle,
            address,
            &displacement,
            &symbolInfo.SymbolInfo
        );
        const PXResult symbolFetchResult = PXErrorCurrent(symbolFetchSuccess);

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

            const PXBool symbolFetchSuccessW = SymGetLineFromAddr64(processHandle, address, &displacement, &lineInfo);
            const PXResult symbolFetchSuccessWRes = PXWindowsErrorCurrent(symbolFetchSuccess);


            IMAGEHLP_SYMBOL64* imageHelpSYMBOL = &symbolInfo; // function avove failed, reuse memory
            imageHelpSYMBOL->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            imageHelpSYMBOL->MaxNameLength = 2000;

            const PXBool symbolFetchSuccessB = SymGetSymFromAddr64(processHandle, address, &displacement, imageHelpSYMBOL);

            return PXResultInvalid;
        }*/


        //  SYM_TYPE xx = symbolInfo.SymbolInfo.TypeIndex;
        // SymTagEnum xxxx = symbolInfo->Tag;

        PXSize written =
            PXSymbolUnDecorateName(symbolInfo.SymbolInfo.Name, (PSTR)pxSymbol->NameUndecorated, 64, &written);


        pxSymbol->Amount = 1;
        pxSymbol->ObjectSize = symbolInfo.SymbolInfo.Size;
        pxSymbol->AddressModule = symbolInfo.SymbolInfo.ModBase;
        pxSymbol->AddressSymbol = symbolInfo.SymbolInfo.Address;

        PXLibrary pxLibrary;

        PXLibraryFromAddress(&pxLibrary, pxSymbol->AddressModule);
        PXLibraryName(&pxLibrary, pxSymbol->NameModule);

        PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);


        // Index
        {
            ULONG64 address = (ULONG64)pxSymbol->AddressModule;

            const BOOL succca = SymFromIndex
            (
                processHandle,
                address,
                symbolInfo.SymbolInfo.Index,
                &symbolInfo.SymbolInfo
            );

            if(succca)
            {
                // override
                PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);

                char bufferB[64];
                IMAGEHLP_LINE64 imageHelperLine;
                imageHelperLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                imageHelperLine.FileName = bufferB;

                const PXBool lineResult = SymGetLineFromAddr(processHandle, symbolInfo.SymbolInfo.Address, &displacement, &imageHelperLine);
                char* fileNameAddress = imageHelperLine.FileName;
                PXSize fileNameAddressSize = 0;

                if(lineResult)
                {
                    const PXSize index = PXTextFindLastCharacterA(imageHelperLine.FileName, -1, '\\');

                    if(-1 != index)
                    {
                        fileNameAddress += index + 1;
                        fileNameAddressSize = PXTextLengthA(fileNameAddress, 64);
                    }

                    pxSymbol->LineNumber = imageHelperLine.LineNumber;
                }

                PXTextCopyA(fileNameAddress, -1, pxSymbol->NameFile, 64);

            }
        }
    }
#else
    return PXResultRefusedNotSupportedByOperatingSystem;
#endif

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXModuleCurrentPath(PXText PXREF pxText)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxText)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    char path[256];

    PXSize len = readlink("/proc/self/exe", path, sizeof(path) - 1);

    if(len <= 0)
        return PXEndianInvalid;

    path[len] = '\0';

    char* slash = strrchr(path, '/');

    if(!slash)
    {
        PXTextCopyFromA(pxText, ".", 1);

        return PXResultOK;
    }

    PXSize dlen = slash - path;

    PXTextCopyFromA(pxText, path, dlen+1);

#elif OSWindows

#else
#endif

    return pxResult;
}

PXResult PXAPI PXModuleCurrentName(PXText PXREF pxText)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxText)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    char path[256];

    PXSize len = readlink("/proc/self/exe", path, sizeof(path) - 1);

    if(len <= 0)
        return PXEndianInvalid;

    path[len] = '\0';

    PXSize slashPosition = PXTextFindLastCharacterA(path, len, '/');

    if(-1 == slashPosition)
    {
        PXTextCopyFromA(pxText, path, len);
    }
    else
    {
        PXTextCopyFromA(pxText, path + slashPosition, len - slashPosition);
    }

#elif OSWindows

#else
#endif

    return pxResult;
}

PXBool PXAPI PXModuleInSameDirectory(PXText PXREF pxTextA, PXText PXREF pxTextB)
{
    PXSize indexA = PXTextFindLastCharacter(pxTextA, '/');
    PXSize indexB = PXTextFindLastCharacter(pxTextB, '/');

    if(-1 == indexA || -1 == indexB)
    {
        return PXFalse;
    }

    PXBool same = PXTextCompareA
    (
        pxTextA->A,
        indexA,
        pxTextB->A,
        indexB,
        PXTextCompareRequireSameLength
    );

    return same;
}


#if OSUnix
#elif OSWindows
#define MAXSIZELISTEE 1024*10

PSAPI_WS_WATCH_INFORMATION watchInformationList[MAXSIZELISTEE];
#endif

PXResult PXAPI PXPerformanceInfoGet(PXPerformanceInfo PXREF pxPerformanceInfo)
{
    // Only when its update zero, set our start time
    if(!pxPerformanceInfo->UpdateCounter)
    {
        pxPerformanceInfo->TimeStamp = PXTimeStampNow();
    }
    else
    {
        pxPerformanceInfo->TimeDelta = PXTimeStampDeltaSF(pxPerformanceInfo->TimeStamp);
    }

#if OSUnix
#elif OSWindows

    PROCESS_MEMORY_COUNTERS_EX2 processMemoryCounters; // PROCESS_MEMORY_COUNTERS

    const HANDLE processHandle = GetCurrentProcess();
    processMemoryCounters.cb = sizeof(processMemoryCounters);;

    const BOOL processMemoryInfo = GetProcessMemoryInfo
    (
        processHandle,
        (PPROCESS_MEMORY_COUNTERS)&processMemoryCounters,
        processMemoryCounters.cb
    );
    const PXResult pxResult = PXErrorCurrent(processMemoryInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }


    PERFORMANCE_INFORMATION performanceInformation;
    PXClear(PERFORMANCE_INFORMATION, &performanceInformation);
    performanceInformation.cb = sizeof(performanceInformation);

    const BOOL performanceInfo = GetPerformanceInfo(&performanceInformation, performanceInformation.cb);


    // GetWsChanges


    if(0 == pxPerformanceInfo->UpdateCounter)
    {
        pxPerformanceInfo->PageFaultCount = processMemoryCounters.PageFaultCount;
        pxPerformanceInfo->PeakWorkingSetSize = processMemoryCounters.PeakWorkingSetSize;
        pxPerformanceInfo->WorkingSetSize = processMemoryCounters.WorkingSetSize;
        pxPerformanceInfo->QuotaPeakPagedPoolUsage = processMemoryCounters.QuotaPeakPagedPoolUsage;
        pxPerformanceInfo->QuotaPagedPoolUsage = processMemoryCounters.QuotaPagedPoolUsage;
        pxPerformanceInfo->QuotaPeakNonPagedPoolUsage = processMemoryCounters.QuotaPeakNonPagedPoolUsage;
        pxPerformanceInfo->QuotaNonPagedPoolUsage = processMemoryCounters.QuotaNonPagedPoolUsage;
        pxPerformanceInfo->PagefileUsage = processMemoryCounters.PagefileUsage;
        pxPerformanceInfo->PeakPagefileUsage = processMemoryCounters.PeakPagefileUsage;
        pxPerformanceInfo->PrivateUsage = processMemoryCounters.PrivateUsage;
        pxPerformanceInfo->PrivateWorkingSetSize = processMemoryCounters.PrivateWorkingSetSize;
        pxPerformanceInfo->SharedCommitUsage = processMemoryCounters.SharedCommitUsage;

        pxPerformanceInfo->CommitTotal = performanceInformation.CommitTotal;
        pxPerformanceInfo->CommitLimit = performanceInformation.CommitLimit;
        pxPerformanceInfo->CommitPeak = performanceInformation.CommitPeak;
        pxPerformanceInfo->PhysicalTotal = performanceInformation.PhysicalTotal;
        pxPerformanceInfo->PhysicalAvailable = performanceInformation.PhysicalAvailable;
        pxPerformanceInfo->SystemCache = performanceInformation.SystemCache;
        pxPerformanceInfo->KernelTotal = performanceInformation.KernelTotal;
        pxPerformanceInfo->KernelPaged = performanceInformation.KernelPaged;
        pxPerformanceInfo->KernelNonpaged = performanceInformation.KernelNonpaged;
        pxPerformanceInfo->PageSize = performanceInformation.PageSize;
        pxPerformanceInfo->HandleCount = performanceInformation.HandleCount;
        pxPerformanceInfo->ProcessCount = performanceInformation.ProcessCount;
        pxPerformanceInfo->ThreadCount = performanceInformation.ThreadCount;

        const BOOL pageWatchInit = InitializeProcessForWsWatch(processHandle);
    }
    else
    {
        pxPerformanceInfo->PageFaultCount = processMemoryCounters.PageFaultCount - pxPerformanceInfo->PageFaultCount;
        pxPerformanceInfo->PeakWorkingSetSize = processMemoryCounters.PeakWorkingSetSize - pxPerformanceInfo->PeakWorkingSetSize;
        pxPerformanceInfo->WorkingSetSize = processMemoryCounters.WorkingSetSize - pxPerformanceInfo->WorkingSetSize;
        pxPerformanceInfo->QuotaPeakPagedPoolUsage = processMemoryCounters.QuotaPeakPagedPoolUsage - pxPerformanceInfo->QuotaPeakPagedPoolUsage;
        pxPerformanceInfo->QuotaPagedPoolUsage = processMemoryCounters.QuotaPagedPoolUsage - pxPerformanceInfo->QuotaPagedPoolUsage;
        pxPerformanceInfo->QuotaPeakNonPagedPoolUsage = processMemoryCounters.QuotaPeakNonPagedPoolUsage - pxPerformanceInfo->QuotaPeakNonPagedPoolUsage;
        pxPerformanceInfo->QuotaNonPagedPoolUsage = processMemoryCounters.QuotaNonPagedPoolUsage - pxPerformanceInfo->QuotaNonPagedPoolUsage;
        pxPerformanceInfo->PagefileUsage = processMemoryCounters.PagefileUsage - pxPerformanceInfo->PagefileUsage;
        pxPerformanceInfo->PeakPagefileUsage = processMemoryCounters.PeakPagefileUsage - pxPerformanceInfo->PeakPagefileUsage;
        pxPerformanceInfo->PrivateUsage = processMemoryCounters.PrivateUsage - pxPerformanceInfo->PrivateUsage;
        pxPerformanceInfo->PrivateWorkingSetSize = processMemoryCounters.PrivateWorkingSetSize - pxPerformanceInfo->PrivateWorkingSetSize;
        pxPerformanceInfo->SharedCommitUsage = processMemoryCounters.SharedCommitUsage - pxPerformanceInfo->SharedCommitUsage;

        pxPerformanceInfo->CommitTotal = performanceInformation.CommitTotal - pxPerformanceInfo->CommitTotal;
        pxPerformanceInfo->CommitLimit = performanceInformation.CommitLimit - pxPerformanceInfo->CommitLimit;
        pxPerformanceInfo->CommitPeak = performanceInformation.CommitPeak - pxPerformanceInfo->CommitPeak;
        pxPerformanceInfo->PhysicalTotal = performanceInformation.PhysicalTotal - pxPerformanceInfo->PhysicalTotal;
        pxPerformanceInfo->PhysicalAvailable = performanceInformation.PhysicalAvailable - pxPerformanceInfo->PhysicalAvailable;
        pxPerformanceInfo->SystemCache = performanceInformation.SystemCache - pxPerformanceInfo->SystemCache;
        pxPerformanceInfo->KernelTotal = performanceInformation.KernelTotal - pxPerformanceInfo->KernelTotal;
        pxPerformanceInfo->KernelPaged = performanceInformation.KernelPaged - pxPerformanceInfo->KernelPaged;
        pxPerformanceInfo->KernelNonpaged = performanceInformation.KernelNonpaged - pxPerformanceInfo->KernelNonpaged;
        pxPerformanceInfo->PageSize = performanceInformation.PageSize - pxPerformanceInfo->PageSize;
        pxPerformanceInfo->HandleCount = performanceInformation.HandleCount - pxPerformanceInfo->HandleCount;
        pxPerformanceInfo->ProcessCount = performanceInformation.ProcessCount - pxPerformanceInfo->ProcessCount;
        pxPerformanceInfo->ThreadCount = performanceInformation.ThreadCount - pxPerformanceInfo->ThreadCount;


#if 0
        PXClearList(PSAPI_WS_WATCH_INFORMATION, watchInformationList, MAXSIZELISTEE);
        const DWORD sizeOfList = sizeof(watchInformationList);

        DWORD amount = 0;

        const BOOL pageWatchUpdate = GetWsChanges(processHandle, watchInformationList, sizeOfList);
        const PXResult pageWatchUpdateResult = PXErrorCurrent(pageWatchUpdate);

        if(PXResultOK != pageWatchUpdateResult)
        {
            // handle..??
        }
        else
        {
            // Count
            for(size_t i = 0; i < MAXSIZELISTEE; i++)
            {
                const PSAPI_WS_WATCH_INFORMATION* watchInformation = &watchInformationList[i];
                const PXBool hasData = (watchInformation->FaultingPc || watchInformation->FaultingVa) > 0;

                if(!hasData)
                {
                    break;
                }

                ++amount;
            }


            for(size_t i = 0; i < amount; i++)
            {
                const PSAPI_WS_WATCH_INFORMATION* watchInformation = &watchInformationList[i];

                PXMemoryVirtualInfo pxMemoryVirtualInfo;

                PXMemoryVirtualInfoViaAddress(&pxMemoryVirtualInfo, watchInformation->FaultingVa);


                PXSymbol pxSymbol;
                PXSymbolFromAddress(&pxSymbol, watchInformation->FaultingVa);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXOSName,
                    "PageFault",
                    "%p %i B -> %s::%s::%s::%i",
                    pxMemoryVirtualInfo.BaseAddress,
                    pxMemoryVirtualInfo.RegionSize,
                    pxSymbol.NameModule,
                    pxSymbol.NameFile,
                    pxSymbol.NameSymbol,
                    pxSymbol.LineNumber
                );
#endif

            }
        }
#endif

    }

    ++pxPerformanceInfo->UpdateCounter;

    return pxResult;

#else
    return PXResultInvalid;
#endif
}

PXResult PXAPI PXDebugEventContinue(const PXI32U processID, const PXI32U threadID)
{
#if OSUnix

    const long ree = ptrace(PTRACE_CONT, processID, NULL, NULL);

    return PXResultRefusedNotImplemented;

#elif PXOSWindowsDestop
    const PXContinueDebugEventFunction pxContinueDebugEvent = (PXContinueDebugEventFunction)_PXOS.DebugEventContinue;
    const DWORD dwContinueStatus = 0;
    const PXBool continueResultID = pxContinueDebugEvent(processID, threadID, dwContinueStatus); // Windows XP, Kernel32.dll, debugapi.h
    const PXResult continueResult = PXErrorCurrent(continueResultID);

    return continueResult;
#else

    return PXResultRefusedNotImplemented;

#endif
}

PXResult PXAPI PXDebugEventWait(void* pxDebugEventInfo, const PXI32U time)
{
    // WaitForDebugEvent() Windows XP, Kernel32.dll, debugapi.h
// WaitForDebugEventEx() Windows 10, Kernel32.dll, debugapi.h

    // Windows XP, Kernel32.dll, debugapi.h

    PXResult pxResult = PXResultInvalid;

    #if OSUnix

#elif OSWindows
    const PXWaitForDebugEventFunction pxWaitForDebugEvent = (PXWaitForDebugEventFunction)_PXOS.DebugEventWait;
    DEBUG_EVENT* debugEvent = (DEBUG_EVENT*)pxDebugEventInfo;

    const BOOL success = pxWaitForDebugEvent(debugEvent, time);
    pxResult = PXErrorCurrent(success);
#endif
    return pxResult;
}

PXResult PXAPI PXDebugProcessActive(void)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDebugBreak(void)
{
#if OSUnix
    raise(SIGTRAP); // Linux, signal.h

#elif OSWindows
    const PXDebugBreakFunction pxDebugBreak = (PXDebugBreakFunction)_PXOS.DebugBreak;
  
    if(!pxDebugBreak)
    {
        DebugBreak();
    }
    else
    {
        pxDebugBreak(); // Windows XP (+UWP), Kernel32.dll, debugapi.h
    }

    return PXResultOK; // We dont have any other result
#else

    // x86 instruction
    // __asm__("int3");

    return PXResultRefusedNotImplemented;
#endif
}

PXResult PXAPI PXDebugOutputDebugStringA(const char* text)
{
#if OSUnix
#elif OSWindows
    const PXOutputDebugStringAFunction pxOutputDebugStringA = (PXOutputDebugStringAFunction)_PXOS.DebugOutputDebugStringA;

    pxOutputDebugStringA(text); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDebugOutputDebugStringW(const wchar_t* text)
{
#if OSUnix
#elif OSWindows
    const PXOutputDebugStringWFunction pxOutputDebugStringW = (PXOutputDebugStringWFunction)_PXOS.DebugOutputDebugStringW;

    pxOutputDebugStringW(text); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDebugProcessBreak(void)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDebugIsPresent(PXBool PXREF isPresent)
{
#if OSUnix
#elif OSWindows
    const PXIsDebuggerPresentFunction pxIsDebuggerPresent = (PXIsDebuggerPresentFunction)_PXOS.DebugIsDebuggerPresent;
    const PXBool result = pxIsDebuggerPresent(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

    if(isPresent)
    {
        *isPresent = result;
    }

    return PXResultOK;
#endif
}

PXResult PXAPI PXDebugIsPresentRemote(const PXProcessHandle processHandle, PXBool PXREF isPresent)
{
    if(!isPresent)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif PXOSWindowsDestop
    const PXCheckRemoteDebuggerPresentFunction pxCheckRemoteDebuggerPresent = (PXCheckRemoteDebuggerPresentFunction)_PXOS.DebugRemotePresentCheck;

    BOOL debuggerPresent = 0;
    const BOOL result = pxCheckRemoteDebuggerPresent(processHandle.Data, &debuggerPresent); // Windows XP, Kernel32.dll, debugapi.h
    const PXResult pxResult = PXErrorCurrent(result);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    *isPresent = debuggerPresent;
#else
    return PXResultNotSupportedByOperatingSystem;
#endif

    return PXResultOK;
}

PXResult PXAPI PXDebugActiveProcessStop(const PXI32U processID)
{
#if OSUnix
    const long result = ptrace(PTRACE_DETACH, processID, 0, 0);

#elif PXOSWindowsDestop

    const PXDebugActiveProcessStopFunction pxDebugActiveProcessStop = (PXDebugActiveProcessStopFunction)_PXOS.DebugActiveProcessStop;

    if(!pxDebugActiveProcessStop)
    {
        return PXResultRefusedNotSupportedByOperatingSystem;
    }

    const BOOL successful = pxDebugActiveProcessStop(processID);
    const PXResult pxResult = PXErrorCurrent(successful);

    return pxResult;
#endif
}


#if OSUnix
#elif PXOSWindowsDestop
BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
    PXSymbolEnumerator PXREF pxSymbolEnumerator = (PXSymbolEnumerator*)UserContext;

    PXSymbol* pxSymbol = &pxSymbolEnumerator->SymbolList[pxSymbolEnumerator->Amount];
    pxSymbol->TypeIndex = pSymInfo->TypeIndex;
    pxSymbol->Index = pSymInfo->Index;
    pxSymbol->ObjectSize = pSymInfo->Size;
    pxSymbol->AddressModule = (void*)pSymInfo->ModBase;
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

void PXAPI PXConvertAnyToFloat(void* a, const PXI32U aType, float* b)
{
    const PXSize isFloat = (PXTypeF32 & aType) == PXTypeF32;

    if(aType == isFloat)
    {
        *b = *(float*)a;
        return;
    }

    switch(PXTypeSizeGet(aType))
    {
        case PXTypeI08S:
        {
            *b = *(PXI8S*)a;
            return;
        }
        case PXTypeI16S:
        {
            *b = *(PXI16S*)a;
            return;
        }
        case PXTypeI32S:
        {
            *b = *(PXI32S*)a;
            return;
        }
        default:
        {
            PXDebugBreak();
            return;
        }
    }
}
