#include "PXOS.h"

#include "File/PXFile.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <DbgHelp.h>
#include <Psapi.h>
#endif




typedef BOOL(WINAPI* PXDebugActiveProcessStopFunction)(_In_ DWORD dwProcessId);
typedef BOOL(WINAPI* PXContinueDebugEventFunction)(_In_ DWORD dwProcessId, _In_ DWORD dwThreadId, _In_ DWORD dwContinueStatus);
typedef BOOL(WINAPI* PXWaitForDebugEventFunction)(_Out_ LPDEBUG_EVENT lpDebugEvent, _In_ DWORD dwMilliseconds);
typedef BOOL(WINAPI* PXDebugActiveProcessFunction)(_In_ DWORD dwProcessId);
typedef VOID(WINAPI* PXDebugBreakFunction)(VOID);
typedef VOID(WINAPI* PXOutputDebugStringAFunction)(_In_opt_ LPCSTR lpOutputString);
typedef    VOID(WINAPI* PXOutputDebugStringWFunction)(_In_opt_ LPCWSTR lpOutputString);
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


const char PXOSName[] = "OS-Kernel";
const char PXOSSemaphore[] = "Semaphore";
const char PXOSCriticalSection[] = "CriticalSec.";
const char PXOSVirtualAllocText[] = "Virtual-Alloc";
const char PXOSVirtualFreeText[] = "Virtual-Free";

const char PXWindowsLibrarNTDLL[] = "ntdll.dll";
const char PXWindowsLibraryKernel32[] = "KERNEL32.DLL";
const char PXWindowsLibraryDebugHelp[] = "DBGHELP.DLL";






#if OSUnix
#elif OSWindows


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









// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
    const DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for(i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}


PXActionResult PXAPI PXSystemPrelude()
{
    if(_PXOS.Init)
    {
        return PXActionSuccessful;
    }

    PXClear(PXOS, &_PXOS);

    // kernel32.dll
    {
        PXLibraryOpenA(&_PXOS.LibraryKernel, PXWindowsLibraryKernel32);

        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
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

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryKernel, pxLibraryFuntionEntryList, amount);
    }

    // Dbghelp.dll
    {
        PXLibraryOpenA(&_PXOS.LibraryDebugHelp, PXWindowsLibraryDebugHelp);

        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
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

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryDebugHelp, pxLibraryFuntionEntryList, amount);
    }


    // ntdll.dll
    {
        PXLibraryOpenA(&_PXOS.LibraryNT, PXWindowsLibrarNTDLL);

        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
        {
            { &_PXOS.RtlGetVersion, "RtlGetVersion"}
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryNT, pxLibraryFuntionEntryList, amount);
    }





    PXThreadCurrent(&_PXOS.MainThread);

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

    infoList = PXMemoryHeapCalloc(PXNull, amountInBytes, 1);
    amountInObjects = amountInBytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    resLog = GetLogicalProcessorInformation(infoList, &amountInBytes);

    _PXOS.ProcessorAmountPhysical = 0;
    _PXOS.ProcessorAmountLogical = 0;

    for(size_t i = 0; i < amountInObjects; i++)
    {
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION* const ptr = &infoList[i];
                
        switch(ptr->Relationship)
        {
            case RelationNumaNode:
                // Non-NUMA systems report a single record of this type.
                ++_PXOS.ProcessorAmountNUMA;
                break;

            case RelationProcessorCore:

                ++_PXOS.ProcessorAmountPhysical;

                // A hyperthreaded core supplies more than one logical processor.
                _PXOS.ProcessorAmountLogical += CountSetBits(ptr->ProcessorMask);
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
        return;
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

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferSize);
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

    free(buffer);
#endif

    //-----------------------------------------------------



    //VirtualQueryEx();



    return PXActionSuccessful;
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

PXActionResult PXAPI PXSystemVersionGetViaRegistry(PXOSVersion* const pxOSVersion)
{
    PXClear(PXOSVersion, pxOSVersion);

#if OSUnix
    FILE* const file = fopen("/etc/os-release", "r");

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
    FILE* file = fopen("/proc/version", "r");
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
        return PXActionInvalid;
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
        PXTextCopyA(OSNameNotFound, sizeof(OSNameNotFound), pxOSVersion->NameVersionLength, value_length);
    }

    RegCloseKey(hKey);

#else

#endif
}

PXActionResult PXAPI PXSystemVersionGetViaKernel(PXOSVersion* const pxOSVersion)
{
    return PXActionInvalid;
}

void PXAPI PXSystemVersionGet(PXOSVersion* const pxOSVersion)
{
#if OSUnix
    struct utsname buffer;
    if(uname(&buffer) != 0) {
        return "Unable to get Linux version.";
    }

    std::ifstream os_release("/etc/os-release");
    std::string line, name, version;
    while(std::getline(os_release, line)) {
        if(line.find("PRETTY_NAME=") == 0) {
            name = line.substr(13, line.length() - 14); // remove quotes
        }
    }

    std::ostringstream oss;
    oss << name << " (" << buffer.sysname << " " << buffer.release << ")";
    return oss.str();
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

    char* osName = 0;

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

PXActionResult PXAPI PXUserNameGet(char* const text, const PXSize textSizeMax, PXSize* const textSizeWritten)
{
    PXActionResult result;

#if OSUnix
    const int userID = getuid();
    struct passwd* pw = getpwuid();

#elif OSWindows

    DWORD length = textSizeMax;
    const BOOL success = GetUserNameA(text, &length); // Windows 2000, Advapi32.dll, winbase.h
    result = PXErrorCurrent(success);

    if(textSizeWritten && success)
    {
        *textSizeWritten = textSizeMax;
    }
#else
    result = PXActionRefusedNotImplemented;
#endif

    return result;
}

PXActionResult PXAPI PXComputerNameGet(char* const text, const PXSize textSizeMax, PXSize* const textSizeWritten)
{
#if OSUnix

#elif OSWindows

#else

#endif
}

PXActionResult PXAPI PXFilePathCleanse(const char* pathInput, char* const pathOutput, const PXSize pathOutputSizeMAX, PXSize* const pathOutputSizeWritten)
{
    PXActionResult pxActionResult;

#if OSUnix
    realpath()
#elif OSWindows
    const DWORD length = GetFullPathNameA(pathInput, pathOutputSizeMAX, pathOutput, NULL); // Windows XP (+UWP), Kernel32.dll, fileapi.h

    pxActionResult = PXErrorCurrent(0 != length);

    if(pathOutputSizeWritten)
    {
        *pathOutputSizeWritten = length;
    }
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXActionResult PXAPI PXFileNameViaHandleA(PXFile* const pxFile, char* const fileNameBuffer, const PXSize pathOutputSizeMAX, PXSize* const pathOutputSizeWritten)
{
    char filePathIntermediate[PXPathSizeMax];
    char* filePathIntermediateOff = filePathIntermediate;
    PXSize filePathIntermediateSize = 0;


#if OSUnix

    // if the filedescriptor is 0, assume we forgot to set it
    if(0 == pxFile->FileDescriptorID)
    {
        pxFile->FileDescriptorID = fileno(pxFile->FileID);
    }

    const PXBool isValidFileDescriptor = 3 <= pxFile->FileDescriptorID;

    if(!isValidFileDescriptor)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "OS-Kernel",
            "File",
            "Standard input, output and error stream are not files! ID:<%i> FILE*:%p",
            pxFile->FileDescriptorID,
            pxFile->FileID
        );
#endif

        return PXActionRefusedArgumentInvalid;
    }

    char namePathBuffer[64];
    const PXSize namePathBufferSIze = sizeof(namePathBuffer);

    PXTextPrintA(namePathBuffer, namePathBufferSIze, "/proc/self/fd/%d", pxFile->FileDescriptorID); // "/prof/self/fd/0123456789"

    const PXSize writtenBytes = readlink(namePathBuffer, filePath->TextA, filePath->SizeAllocated); // [POSIX.1 - 2008]
    const PXActionResult readResult = PXErrorCurrent(-1 != writtenBytes);

    if(PXActionSuccessful != readResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "OS-Kernel",
            "File",
            "Translate file descriptor <%i> failed! <%s>, FILE*:%p",
            pxFile->FileDescriptorID,
            namePathBuffer,
            pxFile->FileID
        );
#endif

        return readResult;
    }

    filePath->SizeUsed = writtenBytes;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "OS-Kernel",
        "File",
        "Translate file descriptor <%i> to <%s>",
        pxFile->FileDescriptorID,
        filePath->TextA
    );
#endif


    // realpath();
    //
    // Only for Apple-OSX
    //const int resultID = fcntl(pxFile->FileID, F_GETPATH, filePath->TextA); // [POSIX]

    return PXActionSuccessful;

#elif OSWindows

#if WindowsAtleastVista

    // FILE_NAME_OPENED, VOLUME_NAME_DOS

    DWORD flags = 0;
    BOOL s = GetHandleInformation(pxFile->FileHandle, &flags);

    filePathIntermediateSize = GetFinalPathNameByHandleA
    (
        pxFile->FileHandle,
        filePathIntermediate,
        PXPathSizeMax,
        FILE_NAME_OPENED | VOLUME_NAME_DOS
    ); // Windows Vista, Kernel32.dll, Windows.h
    const PXActionResult readResult = PXErrorCurrent(0 != filePathIntermediateSize);


    // GetShortPathNameA() makes a path to something like "\\?\C:\Data\WORKSP~1\_GIT_~1\BITFIR~1\GAMECL~1\Shader\SKYBOX~2.GLS"
    // Why would you ever want this?

    // _fullpath(filePath->TextA, buffer, PXPathSizeMax); also, does not what we need it to do

    if(PXActionSuccessful != readResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOSName,
            "File",
            "Translate file handle <%p> failed!",
            pxFile->FileHandle,
            fileNameBuffer
        );
#endif

        return readResult;
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "File",
        "Translate file handle <%p> to <%s>",
        pxFile->FileHandle,
        fileNameBuffer
    );
#endif


    filePathIntermediateSize -= 4u;
    filePathIntermediateOff += 4u;

    char buffer[PXPathSizeMax];

    const DWORD currentPathSize = GetCurrentDirectoryA(PXPathSizeMax, buffer); // Windows XP (+UWP), Kernel32.dll, winbase.h

    const PXSize maxSize = PXMathMinimumIU(currentPathSize, filePathIntermediateSize);
    const PXBool isMatching = PXTextCompareA(buffer, currentPathSize, filePathIntermediateOff, maxSize, 0);

    if(isMatching)
    {
        filePathIntermediateOff += (currentPathSize + 1);
        filePathIntermediateSize -= (currentPathSize + 1);
    }

    if(pathOutputSizeWritten)
    {
        *pathOutputSizeWritten = filePathIntermediateSize;
    }


    PXTextReplace(filePathIntermediateOff, '\\', '/');

    return PXActionSuccessful;
#elif WindowsAtleastXP && 0

    GetMappedFileName(GetCurrentProcess(), pMem, pszFilename, MAX_PATH)

#endif

        // Last resort not to get the file name per handle but from self-storage
        // TODO:
        // PXTextCopy(&pxFile->FilePath, filePath);

        return PXActionSuccessful;
#endif
}

void PXAPI PXTextUTF8ToUNICODE(wchar_t* const textOutput, const char* const textInput)
{
#if OSUnix
#elif OSWindows

  //  MultiByteToWideChar();

#else

#endif
}

void PXAPI PXTextUNICODEToUTF8(char* const textOutput, const wchar_t* const textInput)
{
#if OSUnix
#elif OSWindows

   // WideCharToMultiByte();

#else

#endif
}

void PXAPI PXProcessCurrent(PXProcess* const pxProcess)
{
    //const HANDLE processHandle = GetCurrentProcess();

    PXClear(PXProcess, pxProcess);

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

PXActionResult PXAPI PXProcessMemoryWrite
(
    const PXProcessHandle pxProcessHandle,
    const void* baseAddress,
    const void* bufferData,
    const PXSize bufferSizeMax,
    PXSize* const bufferSizeWritten
)
{
    PXActionResult pxActionResult;

#if OSUnix
#elif OSWindows
    const BOOL successful = WriteProcessMemory // Windows XP, Kernel32.dll, memoryapi.h
    (
        pxProcessHandle,
        baseAddress,
        bufferData,
        bufferSizeMax,
        bufferSizeWritten
    );
    pxActionResult = PXErrorCurrent(successful);
#else
    pxActionResult = PXActionRefusedNotImplemented;
#endif

    return pxActionResult;
}

PXActionResult PXAPI PXProcessMemoryRead
(
    const PXProcessHandle pxProcessHandle, 
    const void* baseAddress,
    void* const bufferData, 
    const PXSize bufferSizeMax, 
    PXSize* const bufferSizeWritten
)
{
    PXActionResult pxActionResult;

#if OSUnix
#elif OSWindows
    const BOOL successful = ReadProcessMemory // Windows XP, Kernel32.dll, memoryapi.h
    (
        pxProcessHandle,
        baseAddress,
        bufferData,
        bufferSizeMax,
        bufferSizeWritten
    );
    pxActionResult = PXErrorCurrent(successful);
#else
    pxActionResult = PXActionRefusedNotImplemented;
#endif

    return pxActionResult;
}

PXBool PXAPI PXThreadIsMain()
{
    PXThread pxThreadCurrent;

    PXThreadCurrent(&pxThreadCurrent);

    return _PXOS.MainThread.HandleID == pxThreadCurrent.HandleID;
}

PXActionResult PXAPI PXThreadCurrent(PXThread* const pxThread)
{
    PXClear(PXThread, pxThread);

#if OSUnix

    pxThread->Info.Handle.ThreadHandle = pthread_self();
    pxThread->HandleID = getpid();

    return PXActionSuccessful;

#elif OSWindows

    // Getting the thread handle yields a pseudo handle that is not useful
    pxThread->Info.Handle.ThreadHandle = GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    pxThread->HandleID = GetCurrentThreadId(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXThreadResume(PXThread* const pxThread)
{
    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= PXExecuteStateRunning;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    const PXProcessThreadHandle threadHandle = pxThread->Info.Handle.ThreadHandle;
    const DWORD suspendCount = ResumeThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXActionResult pxActionResult = PXErrorCurrent(-1 != suspendCount);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Thread-Resume",
        "Handle:<%p>, ID:<%i>",
        threadHandle,
        pxThread->HandleID
    );
#endif
}

PXActionResult PXAPI PXThreadSuspend(PXThread* const pxThread)
{
    const PXProcessThreadHandle threadHandle = pxThread->Info.Handle.ThreadHandle;

    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= PXExecuteStateSuspended;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    const DWORD result = SuspendThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXActionResult pxActionResult = PXErrorCurrent(-1 != result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Thread-Suspend",
        "Handle:<%p>, ID:<%i>",
        threadHandle,
        pxThread->HandleID
    );
#endif
}

PXActionResult PXAPI PXThreadWait(PXThread* const pxThread)
{
    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= PXExecuteStateWaiting;

    pxThread->ReturnResultCode = 0; // Reset Code

#if OSUnix
    const int resultID = pthread_join(pxThread->ThreadHandle, &pxThread->ReturnResult); // pthread.h
    const PXBool success = 0 == resultID;

    if(!success)
    {
        return PXErrorCodeFromID(resultID);
    }

    return PXActionSuccessful;

#elif OSWindows

    const PXProcessThreadHandle threadHandle = pxThread->Info.Handle.ThreadHandle;
    const DWORD resultID = WaitForSingleObject(threadHandle, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

    for(;;)
    {
        const BOOL result = GetExitCodeThread(threadHandle, &pxThread->ReturnResultCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
        const PXBool isDone = STILL_ACTIVE != pxThread->ReturnResultCode;

        if(isDone)
        {
            break;
        }

        PXThreadYieldToOtherThreads();
    }
#endif
}

PXActionResult PXAPI PXThreadCPUCoreAffinitySet(PXThread* const pxThread, const PXInt16U coreIndex)
{
    //MAXIMUM_PROCESSORS;

    const DWORD resultID = SetThreadIdealProcessor
    (
        pxThread->Info.Handle.ThreadHandle,
        coreIndex
    );
    PXActionResult result = PXErrorCurrent(-1 == resultID);


    /*
    DWORD_PTR SetThreadAffinityMask(
        [in] HANDLE    hThread,
        [in] DWORD_PTR dwThreadAffinityMask
    ); // Windows XP (+UWP), Kernel32.dll, winbase.h
    */

    return result;
}



PXActionResult PXAPI PXThreadYieldToOtherThreads()
{
    PXActionResult pxActionResult;

#if OSUnix
    const int yieldResultID =
#if 0
        pthread_yield(); // [deprecated] pthread.h
#else
        sched_yield(); // sched.h, libc, sets errno but on linux never fails
#endif
    pxActionResult = PXErrorCurrent(0 != yieldResultID);

#elif OSWindows
    // UmsThreadYield() // Windows 7 (64-Bit only), Kernel32.dll, winbase.h [Debcricated in Windows 11]

#if 0
    Sleep(0);
    pxActionResult = PXActionSuccessful;
#else
    const PXBool switchSuccessful = SwitchToThread(); // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h
    pxActionResult = PXActionSuccessful; // cant fail
#endif
  
#else
    pxActionResult = PXActionNotSupportedByOperatingSystem;
#endif

    return pxActionResult;
}






PXActionResult PXAPI PXMemoryHeapCreate(PXMemoryHeap* const pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    pxMemoryHeap->HeapHandle = HeapCreate(NULL, 0, 0); // Windows XP, Kernel32.dll, heapapi.h
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMemoryHeapRelease(PXMemoryHeap* const pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    const BOOL result = HeapDestroy(pxMemoryHeap->HeapHandle); // Windows XP, Kernel32.dll, heapapi.h
#endif

    return PXActionSuccessful;
}

void PXAPI PXMemoryHeapGetGlobal(PXMemoryHeap* const pxMemoryHeap)
{
    pxMemoryHeap->HeapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h

    return PXActionSuccessful;
}

PXSize PXAPI PXMemoryHeapBlockSize(PXMemoryHeap* pxMemoryHeap, const void* const adress)
{
    PXMemoryHeap redirectHeap;

    // HeapSize cant handle NULL pointers. If we get one here, it sure is 0.
    if(!adress)
    {
        return 0;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix  || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows

    return _msize(adress);

#elif OSWindows  

    const SIZE_T size = HeapSize(pxMemoryHeap->HeapHandle, 0, adress);
    const PXActionResult sizeError = PXErrorCurrent((SIZE_T)-1 != size);

    return size;
#else
    return 0;
#endif
}

void* PXAPI PXMemoryHeapCalloc(PXMemoryHeap* pxMemoryHeap, const PXSize amount, const PXSize objectSize)
{
    const PXSize totalAmount = amount * objectSize;
    PXMemoryHeap redirectHeap;
    void* adress = PXNull;

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
    adress = calloc(amount, objectSize);
#elif OSWindows
    adress = HeapAlloc(pxMemoryHeap->HeapHandle, HEAP_ZERO_MEMORY, totalAmount); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif

#if PXLogEnable && 0
    // Special logging behaviour
    {
        // PXSymbolMemory pxSymbolMemory;
        // pxSymbolMemory.Adress = adress;
        // pxSymbolMemory.Amount = amount;
         //pxSymbolMemory.ObjectSize = objectSize;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        //  pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
         // PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
         // PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
         // pxSymbolMemory.LineNumber = pxSymbol.LineNumber;

          //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);

        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Alloc",
            "<%p> %6ix %4i B %s::%s::%s::%i",
            adress,
            amount,
            objectSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbol.LineNumber
        );        
    }
#endif

    return adress;
}

void* PXAPI PXMemoryHeapMalloc(PXMemoryHeap* pxMemoryHeap, const PXSize memorySize)
{
    PXMemoryHeap redirectHeap;
    void* adress = PXNull;

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
    adress = malloc(memorySize);
#elif OSWindows
    adress = HeapAlloc(pxMemoryHeap->HeapHandle, 0, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif

    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = 1;
        pxSymbolMemory.ObjectSize = memorySize;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = pxSymbolMemory.LineNumber;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);

#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Alloc",
            "<%p> %4ix %4i B %s::%s::%s::%i",
            adress,
            pxSymbolMemory.Amount,
            pxSymbolMemory.ObjectSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber
        );
#endif
    }

    return adress;
}

PXBool PXAPI PXMemoryHeapFree(PXMemoryHeap* pxMemoryHeap, const void* const adress)
{
    PXMemoryHeap redirectHeap;

    if(!adress)
    {
        return PXFalse;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if PXLogEnable 
    const PXSize blockSize = PXMemoryHeapBlockSize(pxMemoryHeap, adress);
#endif


#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    free(adress);
#elif OSWindows
    const PXBool freeResult = HeapFree(pxMemoryHeap->HeapHandle, 0, adress); // Windows 2000 SP4, Kernel32.dll, heapapi.h


    // Special logging behaviour
    {
#if PXLogEnable 
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = -1;
        pxSymbolMemory.ObjectSize = -1;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = -1;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeRemove);

        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Free",
            "<%p> %4i B %s::%s::%s::%i",
            adress,
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

void* PXAPI PXMemoryHeapRealloc(PXMemoryHeap* pxMemoryHeap, const void* const adress, const PXSize memorySize)
{
    PXMemoryHeap redirectHeap;
    void* newAdress = PXNull;
    PXBool updatedLocation = PXFalse;

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if PXLogEnable 
    const PXSize blockSizeOLD = PXMemoryHeapBlockSize(pxMemoryHeap, adress);
#endif


#if OSUnix || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows
    // Function allows NULL as an adress
    newAdress = realloc(adress, memorySize);

    // clear new allocated space
    PXSize spaceNewSize = memorySize - blockSizeOLD;
    char* spaceNewAdress = (char*)newAdress + blockSizeOLD;

    PXMemoryClear(spaceNewAdress, spaceNewSize);

#elif OSWindows

    if(!adress)
    {
        void* memory = PXMemoryHeapCalloc(pxMemoryHeap, 1, memorySize);

        //PXMemorySet(memory, 0xFF, memorySize);

        return memory;
    }

    newAdress = HeapReAlloc(pxMemoryHeap->HeapHandle, HEAP_ZERO_MEMORY, adress, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    updatedLocation = newAdress != adress;

    // PXMemorySet(newAdress, '°', memorySize - oldSize);
#else
#error Memory reallocate seems not to be supported on this OS
#endif



#if PXLogEnable 
    const PXSize blockSizeNEW = PXMemoryHeapBlockSize(pxMemoryHeap, newAdress);
    const PXOffset offset = blockSizeNEW - blockSizeOLD;
#endif


#if PXLogEnable
    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = memorySize;
        pxSymbolMemory.ObjectSize = 1;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
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
                adress,
                blockSizeOLD,
                newAdress,
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
                adress,
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

    return newAdress;
}



#if OSWindows

PXBool isChecked = 0;

void PXAPI PXWindowsCheckPermission()
{
    // A call to VirtualAlloc() with MEM_LARGE_PAGES
       // WILL normally fail, because we dont have permissions..?
       // We have permissions, it is only disabled by default.



      // AddPrivileges();

    if(isChecked)
    {
        return;
    }



    TOKEN_PRIVILEGES privileges;
    HANDLE hToken;
    LUID luid;



    // Open the process token
    const BOOL openTokenID = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    const PXActionResult openTokenResult = PXErrorCurrent(openTokenID);


    const BOOL lookupSuccess = LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &luid); // SeLockMemoryPrivilege, NOT THREAD SAFE!
    const PXActionResult lookupSuccessResult = PXErrorCurrent(lookupSuccess);

    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Luid = luid;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    const BOOL privilgeResultID = AdjustTokenPrivileges
    (
        hToken,
        FALSE,
        &privileges,
        sizeof(TOKEN_PRIVILEGES),
        PXNull,
        PXNull
    );
    const PXActionResult privilgeResult = PXErrorCurrent(privilgeResultID);

    CloseHandle(hToken);

    isChecked = 1;
}
#endif





void* PXAPI PXMemoryVirtualAllocate(PXSize size, PXSize* const createdSize, const PXAccessMode pxAccessMode)
{
    PXMemoryPageInfo pxFilePageFileInfo;
    PXSize recievedSize = 0;


    PXMemoryPageInfoFetch(&pxFilePageFileInfo, size);

    // Calculate if large pages shall be used


#if PXLogEnable

    const char* text = 0;

    switch(pxFilePageFileInfo.Affinity)
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

    PXTextFormatSize(&pxTextPageSizeNormal, pxFilePageFileInfo.PageSizeNormal);
    PXTextFormatSize(&pxTextPageSizePhysical, pxFilePageFileInfo.PageSizePhysical);
    PXTextFormatSize(&pxTextPageSizeLarge, pxFilePageFileInfo.PageSizeLarge);
    PXTextFormatSize(&pxTextPageSizeHuge, pxFilePageFileInfo.PageSizeHuge);

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
        "Normal-PageSize", pxTextPageSizeNormal.TextA, (int)pxFilePageFileInfo.PageUtilizationNormal, pxFilePageFileInfo.PageAmountNormal,
        "Physical-PageSize", pxTextPageSizePhysical.TextA, (int)pxFilePageFileInfo.PageUtilizationPhysical, pxFilePageFileInfo.PageAmountPhysical,
        "Large-PageSize", pxTextPageSizeLarge.TextA, (int)pxFilePageFileInfo.PageUtilizationLarge, pxFilePageFileInfo.PageAmountLarge,
        "Huge-PageSize", pxTextPageSizeHuge.TextA, (int)pxFilePageFileInfo.PageUtilizationHuge, pxFilePageFileInfo.PageAmountHuge,
        "Targeted type", text
    );
#endif



    // Special behaviour if the object size is 0.
    // Ofcourse it would make no sense to create an object with 
    // a size of zero but lets assume we mean to just create one 
    // page for general useage
    if(0 == size)
    {
        size = pxFilePageFileInfo.PageSizeNormal;
    }






#if OSUnix
    // PROT_READ, PROT_WRITE = read&write permission
    // MAP_PRIVATE           = only for this process
    // MAP_ANONYMOUS         = no attached file, aka. "in memory"
    // MAP_POPULATE          = Preallocate, if not, we could get a SEGVAULT later if memory is suddenly not avalible
    int permission = PROT_READ | PROT_WRITE;
    int mode =
        MAP_PRIVATE |
        MAP_ANONYMOUS |
        MAP_POPULATE; // Will be ignored if not spesifcally enabled

    PXBool useLargePage = usePagesLarge && !usePagesHuge;

#if 0 // MAP_UNINITIALIZED not public??
    if(0) // Dont clear memory, can improve performance but will be ignored if not directly enabled. Safty reasons.
    {
        mode |= MAP_UNINITIALIZED;
    }
#endif

    if(useLargePage) // Create large page
    {
        mode |= MAP_HUGETLB | MAP_HUGE_2MB;
    }

    if(usePagesHuge) // Create huge page
    {
        mode |= MAP_HUGETLB | MAP_HUGE_1GB;
    }

    for(;;)
    {
        pxFile->Data = mmap(NULL, pxFileIOInfo->FileSizeRequest, permission, mode, -1, 0);
        const PXActionResult allocResult = PXErrorCurrent(MAP_FAILED != pxFile->Data);

        if(PXActionSuccessful == allocResult)
        {
            break;
        }

        // Did we do a normal allocation?
        if(!useLargePage && !usePagesHuge)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "File",
                "Open-Virtual",
                "Allocation failed! -> mmap()"
            );
#endif

            pxFile->Data = PXNull;
            pxFile->DataUsed = 0;
            pxFile->DataAllocated = 0;


            break;
        }


        // Try to recover to a normal page
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            "File",
            "Open-Virtual",
            "Allocation failed! We try again..."
        );
#endif


        if(useLargePage)
        {
            mode &= ~(MAP_HUGETLB | MAP_HUGE_2MB);
            useLargePage = PXFalse;
        }
        if(usePagesHuge)
        {
            mode &= ~(MAP_HUGETLB | MAP_HUGE_1GB);
            useLargePage = PXTrue;
        }
    }



#elif OSWindows

    DWORD permissions = PAGE_READWRITE;
    DWORD mode =
        MEM_RESERVE |
        MEM_COMMIT;


    const HANDLE currentProcess = NULL;//  GetCurrentProcess();
    MEM_EXTENDED_PARAMETER param;
    PXClear(MEM_EXTENDED_PARAMETER, &param);
    param.Type = MemExtendedParameterAttributeFlags;

    void* allocatedData = PXNull;
    void* baseAdress = PXNull;
    PXActionResult allocResult;
    PXBool done = PXFalse;

    while(!done)
    {
#if OSWindows       

        if(PXThreadIsMain())
        {
            PXWindowsCheckPermission();
        }
#endif

        switch(pxFilePageFileInfo.Affinity)
        {
            case PXMemoryPageNormal:
            {
                size = pxFilePageFileInfo.PageSizeNormal * pxFilePageFileInfo.PageAmountNormal;

                allocatedData = VirtualAlloc(baseAdress, size, mode, permissions);
                allocResult = PXErrorCurrent(NULL != allocatedData);

                done = PXTrue;

                break;
            }
            case PXMemoryPagePhysical:
            {
                size = pxFilePageFileInfo.PageSizePhysical * pxFilePageFileInfo.PageAmountPhysical;

                allocatedData = VirtualAlloc2
                (
                    currentProcess,
                    baseAdress,
                    size,
                    MEM_64K_PAGES | mode,
                    permissions,
                    0,
                    0
                );

                allocResult = PXErrorCurrent(NULL != allocatedData);

                if(PXActionSuccessful != allocResult)
                {
                    pxFilePageFileInfo.Affinity = PXMemoryPageNormal; // Demote

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

                    continue;
                }

                done = PXTrue;

                break;
            }
            case PXMemoryPageLarge:
            {
                // Adjust allocation size to be EXACTLY a multible of the page size
                // The documentation states it will be rounded up, this is true.
                // Except if we add the MEM_LARGE_PAGES flag, because microsoft.
                size = pxFilePageFileInfo.PageSizeLarge * pxFilePageFileInfo.PageAmountLarge;

                allocatedData = VirtualAlloc
                (
                    baseAdress,
                    size,
                    MEM_LARGE_PAGES | mode,
                    permissions
                );

                allocResult = PXErrorCurrent(NULL != allocatedData);

                if(PXActionSuccessful != allocResult)
                {
                    pxFilePageFileInfo.Affinity = PXMemoryPagePhysical; // Demote

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

                    continue;
                }

                done = PXTrue;

                break;
            }
            case PXMemoryPageHuge:
            {
                size = pxFilePageFileInfo.PageSizeHuge * pxFilePageFileInfo.PageAmountHuge;

                // NOW: Try allocating huge page..
                param.ULong64 = MEM_EXTENDED_PARAMETER_NONPAGED_HUGE; // TODO: Fails with "invalid parameter"

                allocatedData = VirtualAlloc2
                (
                    currentProcess,
                    baseAdress,
                    size,
                    MEM_LARGE_PAGES| mode,
                    permissions,
                    0,
                    0
                );

                allocResult = PXErrorCurrent(NULL != allocatedData);

                if(PXActionSuccessful != allocResult)
                {
                    pxFilePageFileInfo.Affinity = PXMemoryPageLarge; // Demote


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

                    continue;
                }

                done = PXTrue;

                break;
            }
        }
    }
   
    if(PXActionSuccessful != allocResult)
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


    MEMORY_BASIC_INFORMATION memoryInfo;
    PXClear(MEMORY_BASIC_INFORMATION, &memoryInfo);

    VirtualQuery(allocatedData, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    recievedSize = memoryInfo.RegionSize;

#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingAllocation,
        PXOSName,
        PXOSVirtualAllocText,
        "<%p> Requested:<%lu>, Got:<%lu>",
        allocatedData,
        size,
        recievedSize
    );
#endif

    if(createdSize)
    {
        *createdSize = recievedSize;
    }

    return allocatedData;
}

void PXAPI PXMemoryVirtualPrefetch(const void* adress, const PXSize size)
{
    PXOS* const pxOS = PXSystemGet();

#if OSUnix
#elif OSWindows

#if WindowsAtleast8 && PXOSWindowsDestop
    const HANDLE process = GetCurrentProcess();
    const PXSize numberOfEntries = 1;
    WIN32_MEMORY_RANGE_ENTRY memoryRangeEntry;
    PXClear(WIN32_MEMORY_RANGE_ENTRY, &memoryRangeEntry);

    const PXSize flags = 0; // reserved and needs to be 0

    memoryRangeEntry.VirtualAddress = (void*)adress;
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
        adress,
        pxTextSize.TextA
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
        sink ^= ((PXByte*)adress)[i]; // Touch one byte per page
    }
#endif

#else

#if MemoryDebug
    printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched [NOT SUPPORTED] Skipped...\n", adress, size);
#endif
#endif
}

PXActionResult PXAPI PXMemoryVirtualRelease(const void* adress, const PXSize size)
{
    if(!(adress && size))
    {
        return PXActionRefusedArgumentNull;
    }

#if PXLogEnable && PXMemoryDebug
    PXLogPrint
    (
        PXLoggingDeallocation,
        PXOSName,
        "Virtual-Free",
        "<%p> Size:%i",
        adress,
        size
    );
#endif

#if OSUnix
    const int resultID = munmap(adress, size); // sys/mman.h
    const PXActionResult unmapResult = PXErrorCurrent(0 == resultID);

    return unmapResult;

#elif OSWindows
    DWORD freeType = MEM_RELEASE;
    const PXBool freeResultID = VirtualFree((void*)adress, 0, freeType); // Windows XP (+UWP), Kernel32.dll, memoryapi.h
    const PXActionResult freeResult = PXErrorCurrent(freeResultID);

    return freeResult;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

void* PXAPI PXMemoryVirtualReallocate(const void* adress, const PXSize size)
{
    const PXBool newAllocation = adress == PXNull;

    if(newAllocation)
    {
        return PXMemoryVirtualAllocate(size, PXNull, PXAccessModeReadAndWrite);
    }

#if OSUnix
    return PXNull;
#elif OSWindows

    // Create new pages
    void* newSpaceMemory = PXMemoryVirtualAllocate(size, PXNull, PXAccessModeReadAndWrite);

    // Get size of current adress
    MEMORY_BASIC_INFORMATION memoryInfo;
    VirtualQuery(adress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    // Copy data to new pages
    PXMemoryCopy(adress, memoryInfo.RegionSize, newSpaceMemory, memoryInfo.RegionSize);

    // Delete old pages
    PXMemoryVirtualRelease(adress, memoryInfo.RegionSize);

    return newSpaceMemory;
#else
    return PXNull;
#endif

}

PXActionResult PXAPI PXMemoryVirtualInfoViaAdress(PXMemoryVirtualInfo* const pxMemoryVirtualInfo, const void* adress)
{
#if OSUnix
#elif OSWindows

    // Get size of current adress
    MEMORY_BASIC_INFORMATION memoryInfo;
    const SIZE_T size = VirtualQuery(adress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    pxMemoryVirtualInfo->BaseAddress = memoryInfo.BaseAddress;
    pxMemoryVirtualInfo->AllocationBase = memoryInfo.AllocationBase;
    pxMemoryVirtualInfo->RegionSize = memoryInfo.RegionSize;
    pxMemoryVirtualInfo->ProtectionAtCreation = memoryInfo.AllocationProtect;
    pxMemoryVirtualInfo->ProtectionCurrent = memoryInfo.Protect;
    pxMemoryVirtualInfo->State = memoryInfo.State;
    pxMemoryVirtualInfo->Type = memoryInfo.Type;
    pxMemoryVirtualInfo->PartitionID = memoryInfo.PartitionId;

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
            DebugBreak();
            break;
    }
#endif

#else

#endif
}
















PXActionResult PXAPI PXSymbolServerInitialize()
{
    PXOS* const pxOS = PXSystemGet();

    if(!pxOS->Init)
    {
        return PXActionRefusedNotInitialized;
    }


    const PXSymInitializeFunction pxSymbolServerInitialize = (PXSymInitializeFunction)_PXOS.SymbolServerInitialize;
    
   // PXThread pxThread;
   // PXThreadCurrent(&pxThread);
   // pxDebug->Process.ThreadHandle = pxThread.Info.Handle.ThreadHandle;

    HANDLE hProcess = GetCurrentProcess(); // Doc says this is not good?

    // Initializes the symbol handler for a process.
    const PCSTR UserSearchPath = NULL;
    const BOOL fInvadeProcess = TRUE;
    const BOOL initializeSuccessful = pxSymbolServerInitialize(hProcess, UserSearchPath, fInvadeProcess); // Dbghelp.dll, Dbghelp.h, SymInitialize
   // const PXActionResult result = PXErrorCurrent(initializeSuccessful);

    if(!initializeSuccessful)
    {
        return PXActionInvalid;
    }

   // const HANDLE processHandle = GetCurrentProcess();
   // const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);
  //  DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES


   // const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolServerCleanup()
{
    HANDLE processID = GetCurrentProcess();
    const PXSymCleanupFunction pxSymCleanup = (PXSymCleanupFunction)_PXOS.SymbolServerCleanup;
    const PXBool cleanupSuccess = pxSymCleanup(processID); // DbgHelp.dll 5.1 or later

    if(!cleanupSuccess)
    {
        return PXActionFailedCleanup;
    }

    return PXActionFailedLoad;
}

PXActionResult PXAPI PXSymbolServerOptionsSet()
{
   // DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolListLoad(const PXProcessHandle processHandle, const void* baseAdress)
{
#if OSUnix


#elif OSWindows

    PXSymbolEnumerator pxSymbolEnumerator;
    pxSymbolEnumerator.SymbolList = 0;
    pxSymbolEnumerator.Amount = 0;
    pxSymbolEnumerator.Size = 128;

    // SymEnumSym, SymEnumerateSymbols64 is outdated?
    const PXSymEnumSymbolsFunction pxSymbolEnumerate = (PXSymEnumSymbolsFunction)_PXOS.SymbolEnumerate; // DbgHelp.dll 5.1 or later
    const PXBool enumerateResult = pxSymbolEnumerate
    (
        processHandle,
        baseAdress,
        0,
        PXLibraryNameSymbolEnumerate,
        &pxSymbolEnumerator
    );

#else

#endif
}

PXActionResult PXAPI PXSymbolModuleLoad(const PXProcessHandle processHandle, const char* moduleName, void** baseAdress)
{
    const PXSymLoadModuleFunction pxSymbolModuleLoad = (PXSymLoadModuleFunction)_PXOS.SymbolModuleLoad;
    const DWORD64 baseOfDll = pxSymbolModuleLoad // DbgHelp.dll 5.1 or later        SymLoadModuleEx, DbgHelp.dll 6.0 or later
    (
        processHandle,
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


    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolStackWalk(PXSymbolStackWalkInfo* const pxSymbolStackWalkInfo)
{
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolStackTrace(PXSymbol* const pxSymbolList, const PXSize pxSymbolListAmount, const PXSize start, const PXSize depth)
{
    PXMemoryClear(pxSymbolList, sizeof(PXSymbol) * pxSymbolListAmount);

    //return 0;

#if OSUnix 
    void* array[10];
    size_t size;
    char** strings;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    //printf("Obtained %zd stack frames.\n", size);

    // Example "../../PXUltima/Code/[Export]/PXUltimaC/64B-Linux-Debug/PXUltima.so(PXMemoryHeapMalloc+0xb8) [0x7fce66aa763a]"

    PXSize symbolOffset = 0;


    for(PXSize i = start; i < start + depth; ++i)
    {
        PXSymbol* const pxSymbol = &pxSymbolList[symbolOffset];
        const char* traceEntry = strings[i];

        // printf("%s\n", traceEntry);

        const PXSize lastSlash = PXTextFindLastCharacterA(traceEntry, PXTextUnkownLength, '/');

        if(lastSlash != -1)
        {
            // Get module name
            char* moduleName = &traceEntry[lastSlash + 1];

            const PXSize moduleNameSize = PXTextFindLastCharacterA(moduleName, PXTextUnkownLength, '(');


            char* moduleFunction = &moduleName[moduleNameSize + 1];
            const PXSize moduleFunctionSize = PXTextFindLastCharacterA(moduleFunction, PXTextUnkownLength, '+');

            PXTextCopyA(moduleName, moduleNameSize, pxSymbol->NameModule, 64);
            PXTextCopyA(moduleFunction, moduleFunctionSize, pxSymbol->NameSymbol, 64);
            PXTextCopyA("???", 3, pxSymbol->NameFile, 64);
            pxSymbol->LineNumber = -1;

            //   printf("    - %s::%s\n", pxSymbol->NameModule, pxSymbol->NameSymbol);
        }
    }


    free(strings);

#elif PXOSWindowsDestop

    // Prevent failure because not inizilized

#if PXLogEnable && PXDebugStackTracePrint
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        PXDebugerStackTraceText,
        "----- BEGIN -----"
    );
#endif



     //const HANDLE processHeapHandle = GetProcessHeap();
    const HANDLE handleThreadCurrent = GetCurrentThread();
    const HANDLE handleProcessCurrent = GetCurrentProcess();
    const DWORD processID = GetProcessId(handleProcessCurrent);

    const PXActionResult initializeResult = PXSymbolServerInitialize();

    if(PXActionSuccessful != initializeResult)
    {
        for(size_t i = 0; i < pxSymbolListAmount; i++)
        {
            PXSymbol* pxSymbol = &pxSymbolList[i];

            PXTextCopyA("???", 3, pxSymbol->NameUndecorated, 64);
            PXTextCopyA("???", 3, pxSymbol->NameSymbol, 64);
            PXTextCopyA("???.c", 3, pxSymbol->NameFile, 64);
            PXTextCopyA("???.dll", 3, pxSymbol->NameModule, 64);
        }

        return PXActionInvalid;
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


    PXSize frame;
    PXSize symbolIndex = 0;

    PXClear(PXSymbol, pxSymbolList);

    for(frame = 0; frame < (start + depth); ++frame)
    {
        const PXActionResult result = PXSymbolStackWalk(&pxSymbolStackWalkInfo);

        if(PXActionSuccessful != result)
        {
            break;
        }

        if(frame < start)
        {
            continue;
        }

        void* symbolAdress = (ULONG64)pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset; // ULONG64

        PXSymbol* pxSymbol = &pxSymbolList[symbolIndex];
        // PXSymbol pxSymbol;
        // PXClear(PXSymbol, &pxSymbol);

        PXSymbolFromAddress(pxSymbol, symbolAdress);
        PXDebugModuleNameGet(pxSymbol->ModuleAdress, pxSymbol->NameModule, 64, PXNull, PXDebugModuleNameShort);

        ++symbolIndex;


#if PXLogEnable  && PXDebugStackTracePrint
        PXLogPrint
        (
            PXLoggingInfo,
            PXDebugerText,
            PXDebugerStackTraceText,
            "%c 0x%p - %23s - %s",
            (frame == 0 ? '/' : '|'),
            symbolAdress,
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
        char adress[64];

        sprintf_s(functioName, 64, "%s()", pxMSDebugSymbol.Symbol.Name);
        sprintf_s(adress, 64, "0x%p", (void*)pxMSDebugSymbol.Symbol.Address);

        printf("| %-34s | %-18s |\n", functioName, adress);
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
}

PXActionResult PXAPI PXSymbolUnDecorateName(const char* inputName, char* name, const PXSize nameLengthMax, PXSize* nameLengthWritten)
{
    if(!(inputName && name && nameLengthMax))
    {
        return PXActionRefusedArgumentNull;
    }

    const PXUnDecorateSymbolNameFunction pxUnDecorateSymbolName = (PXUnDecorateSymbolNameFunction)_PXOS.SymbolUnDecorateName;

    if(!pxUnDecorateSymbolName)
    {
        return PXActionRefusedNoSuchFunction;
    }

    const DWORD decResultSize = pxUnDecorateSymbolName(inputName, (PSTR)name, nameLengthMax, UNDNAME_COMPLETE); // UnDecorateSymbolName
    const PXActionResult pxActionResult = PXErrorCurrent(0 != decResultSize);

    if(nameLengthWritten)
    {
        nameLengthWritten = 0;
    }

    return pxActionResult;
}

PXActionResult PXAPI PXSymbolFromAddress(PXSymbol* const pxSymbol, const void* const adress)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    PXClear(PXSymbol, pxSymbol);

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

        PXSize written =
            PXSymbolUnDecorateName(symbolInfo.SymbolInfo.Name, (PSTR)pxSymbol->NameUndecorated, 64, &written);


        pxSymbol->Amount = 1;
        pxSymbol->ObjectSize = symbolInfo.SymbolInfo.Size;
        pxSymbol->ModuleAdress = symbolInfo.SymbolInfo.ModBase;
        pxSymbol->SymbolAdress = symbolInfo.SymbolInfo.Address;
        
        PXHandleModule pxHandleModule;
        
        PXSymbolModuleHandleFromAdress(&pxHandleModule, pxSymbol->ModuleAdress);
        PXSymbolModuleName(pxHandleModule, pxSymbol->NameModule);

        PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);


        // Index
        {
            const BOOL succca = SymFromIndex
            (
                processHandle,
                pxSymbol->ModuleAdress,
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

PXActionResult PXAPI PXSymbolEnumerate()
{
   
}

PXActionResult PXAPI PXSymbolFunctionTableAccess()
{
   
}

PXActionResult PXAPI PXSymbolModuleBaseGet()
{
 
}

PXActionResult PXAPI PXSymbolModuleHandleFromAdress(PXHandleModule* const pxHandleModule, const void* const adress)
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
    * pxHandleModule = PXNull;

    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXSymbolModuleName(const PXHandleModule pxHandleModule, char* const name)
{
#if OSUnix
#elif OSWindows
    char buffer[MAX_PATH];

    DWORD size = GetModuleFileName(pxHandleModule, buffer, MAX_PATH);

    const PXSize index = PXTextFindLastCharacterA(buffer, -1, '\\');

    if(-1 != index)
    {
        char* source = &buffer[index+1];
        DWORD sourceSize = size - index-1;

        PXTextCopyA(source, sourceSize, name, sourceSize);
    }

    return PXActionSuccessful;
#endif
}




const PXInt32U _lockWaitSpan = 100;
const PXInt32U _lockWaitTrys = 3;


PXActionResult PXAPI PXSemaphorCreate(PXLock* const pxLock)
{
    PXActionResult pxActionResult;


#if OSUnix
 
    int sharedPointer = 0;
    unsigned int value = 1;

    const int resultID = sem_init(&pxLock->ID, sharedPointer, value);
    pxActionResult = PXErrorCurrent(0 == resultID); // 0=sucessful, -1=Error

#elif OSWindows
    LONG lInitialCount = 1;
    LONG lMaximumCount = 1;

    pxLock->SemaphoreHandle = CreateSemaphoreA(PXNull, lInitialCount, lMaximumCount, PXNull);
    pxActionResult = PXErrorCurrent(pxLock->SemaphoreHandle != PXNull); 

#else
    pxActionResult = PXActionRefusedNotImplemented;
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Created: HANDLE:<%p>",
        pxLock->SemaphoreHandle
    );
#endif

    return pxActionResult; // Success
}

PXActionResult PXAPI PXSemaphorDelete(PXLock* const pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Created: HANDLE:<%p>",
        pxLock->SemaphoreHandle
    );
#endif

#if OSUnix
    const int closingResult = sem_destroy(&lock->ID);
#elif OSWindows
    const BOOL closingResult = CloseHandle(pxLock->SemaphoreHandle);
#endif
}

PXActionResult PXAPI PXSemaphorEnter(PXLock* const pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Enter"
    );
#endif

    PXActionResult waitResult = PXActionInvalid;

#if OSUnix
    const int waitResultID = sem_wait(&lock->SemaphoreHandle);
    waitResult = PXErrorCurrent(0 == waitResultID);
#elif OSWindows

    PXSize tryCounter = 0;

    for(;;)
    {
        const DWORD result = WaitForSingleObject(pxLock->SemaphoreHandle, _lockWaitSpan); // INFINITE


        if(WAIT_OBJECT_0 == result)
        {
            waitResult = PXActionSuccessful;
            // We got runtime
            break;
        }   

        if(WAIT_TIMEOUT == result)
        {
            ++tryCounter;

            if(_lockWaitTrys >= tryCounter)
            {
                continue;
            }

#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingWarning,
                PXOSName,
                PXOSSemaphore,
                "Failed.. timeout!"
            );
#endif


            waitResult = PXActionRefusedNotReady;
            break;
        }


        switch(result)
        {
            case WAIT_ABANDONED:
                waitResult = PXActionRefusedStateInvalid;
                break;

            case WAIT_FAILED:
                waitResult = 0;// PXErrorCurrent(0);
                break;
        }
    }
    
#endif

    return waitResult;
}

PXActionResult PXAPI PXSemaphorLeave(PXLock* const pxLock)
{ 
    if(!pxLock)
    {
        return PXActionInvalid;
    }

    if(0 == pxLock->SemaphoreHandle)
    {
        return PXActionInvalid;
    }

#if PXLogEnable  && 0
    PXLogPrint 
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Leave"
    );
#endif

#if OSUnix
    const int releaseResultID = sem_post(&pxLock->ID);
    const PXActionResult releaseResult = PXErrorCurrent(0 == releaseResultID);
#elif OSWindows
    const BOOL releaseResultID = ReleaseSemaphore(pxLock->SemaphoreHandle, 1, PXNull);
    const PXActionResult releaseResult = PXErrorCurrent(releaseResultID);
#endif   
}

PXActionResult PXAPI PXCriticalSectionCreate(PXLock* const pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Create"
    );
#endif

#if OSUnix

#elif OSWindows     
    InitializeCriticalSection(&pxLock->SectionHandle);
#endif  
}

PXActionResult PXAPI PXCriticalSectionDelete(PXLock* const pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Delete"
    );
#endif

#if OSUnix

#elif OSWindows
    DeleteCriticalSection(&pxLock->SectionHandle);
#endif
}




PXActionResult PXAPI PXCriticalSectionEnter(PXLock* const pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Enter"
    );
#endif

#if OSUnix

#elif OSWindows
    CRITICAL_SECTION* const criticalSectionCast = (CRITICAL_SECTION*)&pxLock->SectionHandle;

    PXSize failEnterCounter = 0;

    for(;;)
    {
        const BOOL success = TryEnterCriticalSection(criticalSectionCast);

        if(success)
        {
            break;
        }

        ++failEnterCounter;

        if(failEnterCounter < 10)
        {
            Sleep(0);
            continue;
        }

#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingInfo,
            PXOSName,
            PXOSCriticalSection,
            "Enter failed! Waiting..."
        );
#endif

        Sleep(100);


    }

    //EnterCriticalSection(criticalSectionCast);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCriticalSectionLeave(PXLock* const pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Leave"
    );
#endif

#if OSUnix

#elif OSWindows
    CRITICAL_SECTION* const criticalSectionCast = (CRITICAL_SECTION*)&pxLock->SectionHandle;

    LeaveCriticalSection(criticalSectionCast);
#endif

    return PXActionSuccessful;
}

#define MAXSIZELISTEE 1024*10

PSAPI_WS_WATCH_INFORMATION watchInformationList[MAXSIZELISTEE];


PXActionResult PXAPI PXFileMapToMemoryEE(PXFile* const pxFile, const PXSize requestedSize, const PXAccessMode pxAccessMode, const PXBool prefetch)
{
    PXInt64U start = PXTimeCounterStampGet();



#if OSUnix
    const int flags = MAP_SHARED;// MAP_PRIVATE;// | MAP_POPULATE;
    int accessType = 0;

    if(pxFileIOInfo->AccessMode & PXAccessREAD)
    {
        accessType |= PROT_READ;
    }

    if(pxFileIOInfo->AccessMode & PXAccessWRITE)
    {
        accessType |= PROT_WRITE;
    }

    if(pxFileIOInfo->AccessMode & PXAccessEXECUTE)
    {
        accessType |= PROT_EXEC;
    }

    // Map data into virtual memory space
    pxFile->Data = mmap
    (
        0, // addressPrefered
        pxFile->DataUsed,
        accessType,
        flags,
        pxFile->FileDescriptorID, // fileDescriptor
        0 // offset
    );
    const PXActionResult mappingResult = PXErrorCurrent(PXNull != pxFile->Data);

    if(PXActionSuccessful != openResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "File",
            "Mapping",
            "Failed for <%s>\n",
            pxFileIOInfo->FilePathAdress
        );
#endif

        return PXActionFailedFileMapping;
    }

    pxFile->LocationMode = PXFileLocationModeMappedFromDisk;

    // We are allowed to close the file handle and/or descriptor?
    // YES! but better not close the descriptor,
    // Access works in any case but you loose the file reference
    //
    // const int closeResultID = fclose(pxFile->FileID);
    // pxFile->FileID = PXNull;

#elif OSWindows

    // Create mapping
    {
        DWORD flProtect = SEC_COMMIT;

        LARGE_INTEGER maximumSize;
        maximumSize.QuadPart = 0;

        if(PXAccessModeReadAndWrite == pxFile->AccessMode || PXAccessModeWriteOnly == pxFile->AccessMode)
        {
            maximumSize.QuadPart = requestedSize;
        }


        //  DWORD dwMaximumSizeHigh = 0;
        // DWORD dwMaximumSizeLow = 0; // Problem if file is 0 Length

#if OS32Bit
        dwMaximumSizeHigh = 0;
        dwMaximumSizeLow = pxFile->DataSize;
#elif OS64Bit
        dwMaximumSizeHigh = (pxFile->DataSize & 0xFFFFFFFF00000000) >> 32u;
        dwMaximumSizeLow = pxFile->DataSize & 0x00000000FFFFFFFF;
#endif

        switch(pxAccessMode)
        {
            case PXAccessModeNoAccess:
                flProtect |= PAGE_NOACCESS;
                break;

            case PXAccessModeReadOnly:
                flProtect |= PAGE_READONLY;
                break;

            case PXAccessModeWriteOnly:
                flProtect |= PAGE_READWRITE; // PAGE_WRITECOPY
                break;

            case PXAccessModeReadAndWrite:
                flProtect |= PAGE_READWRITE;
                break;
        }

        // [i] I want to add LargePage support but it seems you cant do that with files.

        pxFile->MappingHandle = CreateFileMappingA
        (
            pxFile->FileHandle,
            PXNull, // No security attributes
            flProtect,
            maximumSize.HighPart,   // Is those are both zero, the mapping
            maximumSize.LowPart,    // will be as big as the size.
            PXNull // No Name
        );
        const PXActionResult createMappingResult = PXErrorCurrent(PXNull != pxFile->MappingHandle);

        // We can get a "ERROR_ALREADY_EXISTS" if the mapping
        // exist already, the HANDLE will still be a valid one

        if(PXActionSuccessful != createMappingResult)
        {
#if PXLogEnable
            char permissionText[8];

            PXAccessModeToStringA(permissionText, pxFile->AccessMode);

            PXLogPrint
            (
                PXLoggingError,
                PXOSName,
                "Mapping-Create",
                "Failed, %s -> <%s>",
                permissionText,
                "--err---"
            );
#endif

            return createMappingResult;
        }

        // TODO: What is this for?
        if(pxFile->DataUsed == 0)
        {
            pxFile->DataUsed = maximumSize.QuadPart;
            pxFile->DataAllocated = maximumSize.QuadPart;
        }

        {
            DWORD desiredAccess = 0;
            DWORD fileOffsetHigh = 0;
            DWORD fileOffsetLow = 0;
            PXSize numberOfBytesToMap = 0;
            void* baseAddressTarget = 0;
            //DWORD  numaNodePreferred = -1; // (NUMA_NO_PREFERRED_NODE)

            switch(pxAccessMode)
            {
                case PXAccessModeReadOnly:
                    desiredAccess |= FILE_MAP_READ;
                    break;

                case PXAccessModeWriteOnly:
                    desiredAccess |= FILE_MAP_WRITE;
                    break;

                case PXAccessModeReadAndWrite:
                    desiredAccess |= FILE_MAP_ALL_ACCESS;
                    break;
            }

            // if large pages are supported, anable if
#if WindowsAtleastVista && 0
            if(useLargeMemoryPages)
            {
                desiredAccess |= FILE_MAP_LARGE_PAGES;
            }
#endif

            pxFile->Data = MapViewOfFile // MapViewOfFileExNuma is only useable starting windows vista, this function in XP
            (
                pxFile->MappingHandle,
                desiredAccess,
                fileOffsetHigh,
                fileOffsetLow,
                numberOfBytesToMap
            );
            const PXActionResult viewMappingResult = PXErrorCurrent(PXNull != pxFile->Data);

            if(PXActionSuccessful != viewMappingResult)
            {
#if PXLogEnable
                char permissionText[8];

                PXAccessModeToStringA(permissionText, pxAccessMode);

                PXLogPrint
                (
                    PXLoggingError,
                    PXOSName,
                    "Mapping-View",
                    "Failed, %s -> <%s>",
                    permissionText,
                    "--err---"
                );
#endif

                return viewMappingResult;
            }
        }

        pxFile->LocationMode = PXFileLocationModeMappedFromDisk;

    }

#endif



    if(prefetch)
    {
        PXMemoryVirtualPrefetch(pxFile->Data, pxFile->DataUsed);
    }

    PXInt64U stop = PXTimeCounterStampGet();
    PXInt64U diff = stop - start;

    PXF32 timeS = PXTimeCounterStampToSecoundsF(diff);

    PXInt64U bps = pxFile->DataUsed / timeS;


#if PXLogEnable
    PXText pxTextbps;
    PXTextConstructNamedBufferA(&pxTextbps, pxTextbpsBuffer, 32);
    PXTextFormatSize(&pxTextbps, bps);

    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Open-Mapping",
        "Read with %s/s",
        pxTextbps.TextA
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXPerformanceInfoGet(PXPerformanceInfo* const pxPerformanceInfo)
{
    if(0 == pxPerformanceInfo->UpdateCounter)
    {
        PXClear(PXPerformanceInfo, pxPerformanceInfo);
        pxPerformanceInfo->TimeStamp = PXTimeCounterStampGet();
    }
    else
    {
        PXInt64U timestampPrevious = pxPerformanceInfo->TimeStamp;

        pxPerformanceInfo->TimeStamp = PXTimeCounterStampGet(); // override new

        PXInt64U timestampDelta = pxPerformanceInfo->TimeStamp - timestampPrevious;

        pxPerformanceInfo->TimeDelta = PXTimeCounterStampToSecoundsF(timestampDelta);
    }

#if OSUnix
#elif OSWindows

    PROCESS_MEMORY_COUNTERS_EX2 processMemoryCounters; // PROCESS_MEMORY_COUNTERS

    const HANDLE processHandle = GetCurrentProcess();
    processMemoryCounters.cb = sizeof(processMemoryCounters);;

    const BOOL processMemoryInfo = GetProcessMemoryInfo
    (
        processHandle,
        &processMemoryCounters,
        processMemoryCounters.cb
    );
    const PXActionResult pxActionResult = PXErrorCurrent(processMemoryInfo);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
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
        const PXActionResult pageWatchUpdateResult = PXErrorCurrent(pageWatchUpdate);

        if(PXActionSuccessful != pageWatchUpdateResult)
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

                PXMemoryVirtualInfoViaAdress(&pxMemoryVirtualInfo, watchInformation->FaultingVa);


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



    return pxActionResult;

#else
    return PXActionInvalid;
#endif
}






PXActionResult PXAPI PXDebugEventContinue(const PXInt32U processID, const PXInt32U threadID)
{
#if OSUnix

    const long ree = ptrace(PTRACE_CONT, pxDebug->Process.ProcessID, NULL, NULL);

    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
    const PXContinueDebugEventFunction pxContinueDebugEvent = (PXContinueDebugEventFunction)_PXOS.DebugEventContinue;
    const DWORD dwContinueStatus = 0;
    const PXBool continueResultID = pxContinueDebugEvent(processID, threadID, dwContinueStatus); // Windows XP, Kernel32.dll, debugapi.h
    const PXActionResult continueResult = PXErrorCurrent(continueResultID);

    return continueResult;
#else

    return PXActionRefusedNotImplemented;

#endif
}

PXActionResult PXAPI PXDebugEventWait(void* pxDebugEventInfo, const PXInt32U time)
{
    // WaitForDebugEvent() Windows XP, Kernel32.dll, debugapi.h
// WaitForDebugEventEx() Windows 10, Kernel32.dll, debugapi.h

    // Windows XP, Kernel32.dll, debugapi.h

    const PXWaitForDebugEventFunction pxWaitForDebugEvent = (PXWaitForDebugEventFunction)_PXOS.DebugEventWait;
    DEBUG_EVENT* debugEvent = (DEBUG_EVENT*)pxDebugEventInfo;

    const BOOL success = pxWaitForDebugEvent(debugEvent, time);
    const PXActionResult pxActionResult = PXErrorCurrent(success);

    return pxActionResult;
}

PXActionResult PXAPI PXDebugProcessActive()
{
    
}

PXActionResult PXAPI PXDebugBreak()
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    const PXDebugBreakFunction pxDebugBreak = (PXDebugBreakFunction)_PXOS.DebugBreak;

    PXDebugBreak(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

    return PXActionSuccessful; // We dont have any other result
#else
    return PXActionRefusedNotImplemented;
#endif
}

PXActionResult PXAPI PXDebugOutputDebugStringA(const char* text)
{
#if OSUnix
#elif OSWindows
    const PXOutputDebugStringAFunction pxOutputDebugStringA = (PXOutputDebugStringAFunction)_PXOS.DebugOutputDebugStringA;

    pxOutputDebugStringA(text); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
}

PXActionResult PXAPI PXDebugOutputDebugStringW(const wchar_t* text)
{
#if OSUnix
#elif OSWindows
    const PXOutputDebugStringWFunction pxOutputDebugStringW = (PXOutputDebugStringWFunction)_PXOS.DebugOutputDebugStringW;

    pxOutputDebugStringW(text); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
}

PXActionResult PXAPI PXDebugProcessBreak()
{
 
}

PXActionResult PXAPI PXDebugIsPresent(PXBool* const isPresent)
{
#if OSUnix
#elif OSWindows
    const PXIsDebuggerPresentFunction pxIsDebuggerPresent = (PXIsDebuggerPresentFunction)_PXOS.DebugIsDebuggerPresent;
    const PXBool result = pxIsDebuggerPresent(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

    if(isPresent)
    {
        *isPresent = result;
    }

    return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXDebugIsPresentRemote(const PXProcessHandle processHandle, PXBool* const isPresent)
{
    if(!isPresent)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif PXOSWindowsDestop
    const PXCheckRemoteDebuggerPresentFunction pxCheckRemoteDebuggerPresent = (PXCheckRemoteDebuggerPresentFunction)_PXOS.DebugRemotePresentCheck;

    BOOL debuggerPresent = 0;
    const BOOL result = pxCheckRemoteDebuggerPresent(processHandle, &debuggerPresent); // Windows XP, Kernel32.dll, debugapi.h
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

PXActionResult PXAPI PXDebugActiveProcessStop(const PXInt32U processID)
{
#if OSUnix
    const long result = ptrace(PTRACE_DETACH, pxDebug->Process.ProcessID, 0, 0);

#elif PXOSWindowsDestop

    const PXDebugActiveProcessStopFunction pxDebugActiveProcessStop = (PXDebugActiveProcessStopFunction)_PXOS.DebugActiveProcessStop;

    if(!pxDebugActiveProcessStop)
    {
        return PXActionRefusedNotSupportedByOperatingSystem;
    }

    const BOOL successful = pxDebugActiveProcessStop(processID);
    const PXActionResult pxActionResult = PXErrorCurrent(successful);

    return pxActionResult;
#endif
}

PXActionResult PXAPI PXDebugModuleNameGet
(
    const PXHandleModule pxHandleModule,
    char* moduleName,
    const PXSize moduleNameSize,
    PXSize* const sizeWritten,
    const PXInt32U flags
)
{
    char moduleNameBuffer[PXPathSizeMax];
    PXClearList(char, moduleNameBuffer, PXPathSizeMax);
    PXSize moduleNameLength = 0;

    PXActionResult moduleFetchResult;

    // Stage 1) Module to string
#if OSUnix

    if(pxHandleModule)
    {
        /*
        Dl_info info;

        const int resultID = dladdr(current_module, &info);
        const PXActionResult moduleFetchResult = PXErrorCurrent((0 != resultID));

        if(info.dli_fname)
        {
            printf("Current shared object: %s\n", info.dli_fname);
        }
        else {
            printf("Could not determine the current shared object.\n");
            */
    }
    else
    {
        // The HANDLE is NULL. So we want the path of the current executable
        moduleNameLength = readlink("/proc/self/exe", moduleName, PXPathSizeMax); // unistd.h
        moduleFetchResult = PXErrorCurrent(-1 != count);
    }

#elif OSWindows

    moduleNameLength = GetModuleFileNameA(pxHandleModule, moduleNameBuffer, PXPathSizeMax);
    moduleFetchResult = PXErrorCurrent(0 != moduleNameLength);

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif


    if(PXActionSuccessful != moduleFetchResult)
    {
        moduleName[0] = '?';
        moduleName[1] = '?';
        moduleName[2] = '?';
        moduleName[3] = '\0';

        if(sizeWritten)
        {
            *sizeWritten = 3;
        }

        return moduleFetchResult;
    }


    // Stage 2) Truncate name
    if(!(PXDebugModuleNameShort & flags))
    {
        PXTextReplaceByte(moduleNameBuffer, moduleNameLength, '\\', '/');
        PXTextCopyA(moduleNameBuffer, moduleNameLength, moduleName, moduleNameSize);


        if(sizeWritten)
        {
            *sizeWritten = moduleNameLength;
        }

        return PXActionSuccessful; // We dont do any further processing
    }




#if OSUnix

    moduleName[count] = '\0';
    *sizeWritten = count;

    // Truncate
    const PXSize lastSlashPosition = PXTextFindLastCharacterA(moduleName, count, '/');

    if(lastSlashPosition != -1)
    {
        PXSize sourceSize = count - lastSlashPosition;
        char* source = &moduleName[lastSlashPosition];

        *sizeWritten = PXMemoryMove(source, sourceSize, moduleName, moduleNameSize);
    }


#elif OSWindows


    // net to get moduleinto, but we have those
    // IMAGEHLP_MODULE64 mMAGEHLP_MODULE64;
    // mMAGEHLP_MODULE64.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
    //  const PXBool moduleFetchSuccessaSAS = SymGetModuleInfo64(processHandle, moduleHandle, &mMAGEHLP_MODULE64);


    // Extract names
    {
        char* molduleNamefixed = moduleNameBuffer;
        PXSize actualSize = moduleNameLength;

        const char systemPath[] = "C:\\WINDOWS\\SYSTEM32\\";
        const PXSize systemPathSize = sizeof(systemPath);
        const PXBool isSystem = PXTextCompareIgnoreCaseA(systemPath, systemPathSize, moduleNameBuffer, systemPathSize);

        if(isSystem)
        {
            molduleNamefixed += systemPathSize - 1;
            actualSize -= systemPathSize - 1;

            // Check if its a driver
            const char driverStoreText[] = "DriverStore";
            const PXSize driverStoreTextSize = sizeof(driverStoreText);

            const PXBool isDriver = PXTextCompareA(driverStoreText, driverStoreTextSize, molduleNamefixed, driverStoreTextSize, 0);

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
            PXSize lastSlashB = PXTextFindLastCharacterA(moduleNameBuffer, currentWorkPathSize, '\\');
            PXBool isRelativePath = PXTextCompareA(currentWorkPath, lastSlashA, moduleNameBuffer, lastSlashB, 0);

            if(isRelativePath)
            {
                molduleNamefixed += lastSlashA + 1;
                actualSize -= lastSlashA + 1;
            }
        }

        moduleNameLength = PXTextCopyA(molduleNamefixed, actualSize, moduleName, PXPathSizeMax);
    }
#endif

    if(sizeWritten)
    {
        *sizeWritten = moduleNameLength;
    }

    return PXActionSuccessful;
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
    pxSymbol->ModuleAdress = (void*)pSymInfo->ModBase;
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