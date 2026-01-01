#include "PXDotNet.h"

#if OSUnix
#elif OSWindows
#include <Windows.h>

#define OSWindowsDotNETAviable 0
#if OSWindowsDotNETAviable
#include <MSCorEE.h>    // Missing on windows 10
#include <metahost.h>   // Missing on windows 10
#include "coreclrhost.h" // What is this??
#pragma comment(lib, "mscoree.lib")
#pragma comment(lib, "format.lib")
//#pragma comment(lib, "mscories.lib")

// This is not set as a directive, hardcoded, not good
#include "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.21/runtimes/win-x64/native/nethost.h"
#include "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.21/runtimes/win-x64/native/coreclr_delegates.h"
#include "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.21/runtimes/win-x64/native/hostfxr.h"
#include "coreclrhost.h"

#pragma comment(lib, "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.21/runtimes/win-x64/native/nethost.lib")

#endif

#endif


#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Media/PXVersion.h>

const char CSharpCoreClrPath[] = "C:/Program Files/dotnet/shared/Microsoft.NETCore.App";
const PXI8U CSharpCoreClrPathSize = sizeof(CSharpCoreClrPath)-1;
const char CSharpCoreClrDLLName[] = "coreclr.dll";
const char CSharpMSCoree[] = "mscoree.dll";
const char PXDotNetName[] = ".NET";

//"nethost.dll";
const char PXDotNetNetHost[] =
"C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\8.0.16\\runtimes\\win-x64\\native\\nethost.dll";
const char PXDotNetFunc[] = "get_hostfxr_path";

#if OSWindowsDotNETAviable
typedef int (NETHOST_CALLTYPE* PXget_hostfxr_path)(char* buffer, size_t* buffer_size, const struct get_hostfxr_parameters* parameters);
#endif










typedef struct PXDotNetCoreCLR_
{
    PXLibrary LibraryCoreCLR;

    void* CLRJitAttachState;
    void* GetCLRRuntimeHost;
    void* MetaDataGetDispenser;
    void* DelegateCreate; // coreclr_create_delegate_ptr
    void* AssemblyExecute; // coreclr_execute_assembly_ptr
    void* Initialize; // coreclr_initialize_ptr
    void* ErrorWriterSet; // coreclr_set_error_writer_ptr
    void* Shutdown; // coreclr_shutdown_ptr
    void* Shutdown2; // coreclr_shutdown_2_ptr
    void* g_CLREngineMetrics;
    void* g_dacTable;

    void* HostHandle;
    unsigned int DomainID;

    PXVersion* VersionList;
    PXVersion* VersionCurrent;
    PXSize VersionListAmount;
}
PXDotNetCoreCLR;

// Modern runtime 
typedef struct PXDotNetHostFX
{
    PXLibrary LibraryHostFX;

    void* RuntimeHandle; // hostfxr_handle

    BOOL IsDotNetCore;

    void* com_activation;
    void* load_in_memory_assembly;
    void* winrt_activation;
    void* com_register;
    void* com_unregister;

    // coreclr_delegatwe.h
    void* ComponentEntryPoint; // component_entry_point_fn
    void* AssemblyLoadAndFunctionPointerGet; // load_assembly_and_get_function_pointer_fn
    void* FunctionPointerGet; // get_function_pointer_fn
    void* AssemblyLoad; // load_assembly_fn
    void* AssemblyLoadBytes; // load_assembly_bytes_fn

    // hostfxr.h
    void* Main; // hostfxr_main_fn
    void* MainStartupInfo; // hostfxr_main_startupinfo_fn
    void* MainBundleStartupInfo; // hostfxr_main_bundle_startupinfo_fn
    void* ErrorWriterSet; // hostfxr_set_error_writer_fn
    void* InitializeForCommandLine; // hostfxr_initialize_for_dotnet_command_line_fn
    void* InitializeForRuntimeConfig; // hostfxr_initialize_for_runtime_config_fn
    void* RuntimePropertyValueGet; // hostfxr_get_runtime_property_value_fn
    void* RuntimePropertyValueSet; // hostfxr_set_runtime_property_value_fn
    void* RuntimePropertiesGet; // hostfxr_get_runtime_properties_fn
    void* RunApp; // hostfxr_run_app_fn
    void* RuntimeDelegateGet; // hostfxr_get_runtime_delegate_fn
    void* RuntimeClose; // "hostfxr_close"
    void* EnvironmentInfoResult; // hostfxr_get_dotnet_environment_info_result_fn
    void* EnvironmentInfoGet; // hostfxr_get_dotnet_environment_info_fn

    void* ResolveSDK;
    void* ResolveSDK2;
    void* AvailableSDKsGet; // hostfxr_get_available_sdks
    void* NativeSearchDirectoriesGet; // hostfxr_get_native_search_directories           
}
PXDotNetHostFX;


typedef struct PXDotNetMSCoree_
{
    PXLibrary Library;

    void* MetaHost;
    void* MetaHostPolicy;
    void* Debugging;

    void* RuntimeInfo;
    void* RuntimeHost;

    void* AppDomain;
}
PXDotNetMSCoree;

typedef struct PXDotNet_
{
    union
    {
        char API[1];
        PXDotNetMSCoree MSCoree;
        PXDotNetCoreCLR CoreCLR;
        PXDotNetHostFX HostFX;
    };

    PXDotNetDelegateFetch DelegateFetch;
    PXDotNetExecute Execute;
}
PXDotNet;



void PXDotNetCoreCLRErrorCallback(const char* message)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingError,
        PXDotNetName,
        "Error",
        "\n"
        "------------------------------------------------\n"
        "%s\n"
        "------------------------------------------------\n",
        message
    );
#endif
}

#if OSWindowsDotNETAviable
void HOSTFXR_CALLTYPE PXDotNetHostFXRuntimeErrorCallback(const char_t* message)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingError,
        PXDotNetName,
        "Error",
        "\n"
        "------------------------------------------------\n"
        "%ls\n"
        "------------------------------------------------\n",
        message
    );
#endif
}
#endif


#if OSWindows
PXResult PXAPI PXDotNetInitializeMSCoree(PXDotNetMSCoree PXREF pxDotNetMSCoree)
{
#if OSWindowsDotNETAviable
    CreateInterfaceFnPtr createInterface;

    // open
    {
        const PXResult pxActionResult = PXLibraryOpenA(&pxDotNetMSCoree->Library, CSharpMSCoree);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }

        PXLibraryGetSymbolA(&pxDotNetMSCoree->Library, &createInterface, "CreateInterface", PXTrue);
    }


    // fetch
    {
        const HRESULT resA = createInterface(&CLSID_CLRMetaHost, &IID_ICLRMetaHost, &pxDotNetMSCoree->MetaHost);
        //const HRESULT resB = createInterface(&CLSID_CLRMetaHostPolicy, &IID_ICLRMetaHostPolicy, &pxDotNet->MetaHostPolicy);
        //const HRESULT resC = createInterface(&CLSID_CLRDebugging,      &IID_ICLRDebugging,      &pxDotNet->Debugging);
    }

    ICLRMetaHost PXREF metaHost = (ICLRMetaHost*)pxDotNetMSCoree->MetaHost;
    ICLRMetaHostPolicy PXREF metaHostPolicy = (ICLRMetaHostPolicy*)pxDotNetMSCoree->MetaHostPolicy;
    ICLRDebugging PXREF debugging = (ICLRDebugging*)pxDotNetMSCoree->Debugging;
    ICLRRuntimeInfo* runtimeInfo = PXNull;

    IEnumUnknown* enumUnknown = 0; // ICLRRuntimeInfo
    ULONG fetched = 0;


    WCHAR bufferVersion[64];

    // Get a list of all versions
    {
        const HRESULT resultID = metaHost->lpVtbl->EnumerateInstalledRuntimes(metaHost, &enumUnknown);
        const PXResult result = PXErrorCurrent(S_OK == resultID);

        BOOL isLoadable = PXFalse;
        BOOL isLoaded = PXFalse;
        BOOL isStarted = PXFalse;
        DWORD startedFlags = 0;
        DWORD bufferSize = 0;

        WCHAR bufferPath[260];

        ICLRRuntimeInfo* runtimeInfoList = 0;

        HANDLE processHandle = GetCurrentProcess();

        for(PXSize i = 0; S_OK == enumUnknown->lpVtbl->Next(enumUnknown, 1, &runtimeInfoList, &fetched); ++i)
        {
            const HRESULT esa = runtimeInfoList->lpVtbl->GetRuntimeDirectory(runtimeInfoList, PXNull, &bufferSize);
            const HRESULT ewr = runtimeInfoList->lpVtbl->GetRuntimeDirectory(runtimeInfoList, bufferPath, &bufferSize);
            const HRESULT aaa = runtimeInfoList->lpVtbl->GetVersionString(runtimeInfoList, PXNull, &bufferSize);
            const HRESULT sss = runtimeInfoList->lpVtbl->GetVersionString(runtimeInfoList, bufferVersion, &bufferSize);
            const HRESULT ww = runtimeInfoList->lpVtbl->IsLoadable(runtimeInfoList, &isLoadable);
            const HRESULT ee = runtimeInfoList->lpVtbl->IsLoaded(runtimeInfoList, processHandle, &isLoaded);
            const HRESULT rr = runtimeInfoList->lpVtbl->IsStarted(runtimeInfoList, &isStarted, &startedFlags);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXDotNetName,
                "Init",
                "Path : %ls, Version: %ls",
                bufferPath,
                bufferVersion
            );
#endif

        }
    }





    // load the runtime into this process
    {
        const HRESULT hresult = metaHost->lpVtbl->GetRuntime(metaHost, bufferVersion, &IID_ICLRRuntimeInfo, &pxDotNetMSCoree->RuntimeInfo);

        runtimeInfo = (ICLRRuntimeInfo*)pxDotNetMSCoree->RuntimeInfo;

        // &CLSID_CorRuntimeHost, &IID_ICorRuntimeHost
        const HRESULT hresultre = runtimeInfo->lpVtbl->GetInterface(runtimeInfo, &CLSID_CLRRuntimeHost, &IID_ICLRRuntimeHost, &pxDotNetMSCoree->RuntimeHost);
    }

    //ICorRuntimeHost
    ICLRRuntimeHost PXREF runtimeHost = (ICLRRuntimeHost*)pxDotNetMSCoree->RuntimeHost;

    // Start the runtime
    const HRESULT asddssd = runtimeHost->lpVtbl->Start(runtimeHost);



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "Init",
        "--- Done ---"
    );
#endif






    /*

        /*


    const HRESULT asdasdt = runtimeHost->lpVtbl->GetDefaultDomain(runtimeHost, &pxDotNet->AppDomain);

    IAppDomainBinding PXREF appDomainBinding = (IAppDomainBinding*)pxDotNet->AppDomain;

    *


    //Call the GetDefaultDomain method from the ICorRuntimeHost object to get the IAppDomain object
    //Then you can load libraries using IAppDomain.Load_2.
    //If you want to load.NET DLLs from network shares it is more complex, because you need to call UnsafeLoadFrom, which is not available in IAppDomain.But this is also possible.


    IUnknown* pAppDomainThunk = NULL;
    IAppDomainBinding* pDefaultAppDomain = NULL; // _AppDomain
    AssemblyBindInfo* pAssembly = NULL; // _Assembly
   // _Type* pType = NULL;
    VARIANT vtEmpty;
    VariantInit(&vtEmpty);
    HRESULT hr = runtimeHost->lpVtbl->GetDefaultDomain(runtimeHost, &pAppDomainThunk);



    if(FAILED(hr))
    {
       // printf("Failed to get default AppDomain: 0x%x\n", hr);
        //pRuntimeHost->Release();
        return -1;
    }

    // IID_IAppDomainBinding
    hr = pAppDomainThunk->lpVtbl->QueryInterface(pAppDomainThunk, PXNull, &pDefaultAppDomain); // IID_PPV_ARGS(&pDefaultAppDomain)

    if(FAILED(hr))
    {
      //  printf("Failed to get _AppDomain interface: 0x%x\n", hr);
       // pAppDomainThunk->Release();
       // pRuntimeHost->Release();
        return -1;
    }

    // Load the C# assembly
    BSTR bstrAssemblyName = SysAllocString(L"YourAssembly.dll");
  //  hr = pDefaultAppDomain->lpVtbl->Load_2(bstrAssemblyName, &pAssembly);
    SysFreeString(bstrAssemblyName);
    if(FAILED(hr))
    {
        //printf("Failed to load assembly: 0x%x\n", hr);
       // pDefaultAppDomain->Release();
       // pAppDomainThunk->Release();
       // pRuntimeHost->Release();
        return -1;
    } // Get the type
    BSTR bstrTypeName = SysAllocString(L"YourNamespace.YourClass");
  //  hr = pAssembly->GetType_2(bstrTypeName, &pType);
    SysFreeString(bstrTypeName);
    if(FAILED(hr))
    {
        //printf("Failed to get type: 0x%x\n", hr);
      //  pAssembly->Release();
       // pDefaultAppDomain->Release();
       // pAppDomainThunk->Release();
       // pRuntimeHost->Release();
        return -1;
    } // Get the method
    BSTR bstrMethodName = SysAllocString(L"YourMethod");
    VARIANT vtResult;
   // hr = pType->InvokeMember_3(bstrMethodName, BindingFlags_InvokeMethod, NULL, vtEmpty, NULL, &vtResult);
    SysFreeString(bstrMethodName);
    if(FAILED(hr))
    {
       // printf("Failed to invoke method: 0x%x\n", hr);
    }
    else
    {
        //printf("Method executed successfully\n");
    }





    */
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDotNetInitializeCoreCLR(PXDotNetCoreCLR PXREF pxDotNetCoreCLR)
{
#if OSUnix

    return PXActionRefusedNotImplemented;

#elif OSWindows
    // Open lib
    {
        char runtimeDLLPath[PXPathSizeMax];
        char versionString[64];
        PXVersionToString(pxDotNetCoreCLR->VersionCurrent, versionString);

        PXTextPrintA(runtimeDLLPath, PXPathSizeMax, "%s/%s/%s", CSharpCoreClrPath, versionString, CSharpCoreClrDLLName);

        PXLibraryOpenA(&pxDotNetCoreCLR->LibraryCoreCLR, runtimeDLLPath);
    }


    // Functionpointers
    {
        PXLibraryFuntionEntry pxFunctionEntryList[] =
        {
            { &pxDotNetCoreCLR->CLRJitAttachState, "CLRJitAttachState"},
            { &pxDotNetCoreCLR->GetCLRRuntimeHost , "GetCLRRuntimeHost"},
            { &pxDotNetCoreCLR->MetaDataGetDispenser , "MetaDataGetDispenser"},
            { &pxDotNetCoreCLR->DelegateCreate , "coreclr_create_delegate"},
            { &pxDotNetCoreCLR->AssemblyExecute , "coreclr_execute_assembly"},
            { &pxDotNetCoreCLR->Initialize , "coreclr_initialize"},
            { &pxDotNetCoreCLR->ErrorWriterSet , "coreclr_set_error_writer"},
            { &pxDotNetCoreCLR->Shutdown ,"coreclr_shutdown"},
            { &pxDotNetCoreCLR->Shutdown2 , "coreclr_shutdown_2"},
            { &pxDotNetCoreCLR->g_CLREngineMetrics , "g_CLREngineMetrics"},
            { &pxDotNetCoreCLR->g_dacTable, "g_dacTable"} 
        };
        const PXI8U amount = sizeof(pxFunctionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&pxDotNetCoreCLR->LibraryCoreCLR, pxFunctionEntryList, amount);
    }


    // Initialize .NET Core runtime
    /*
     _X("TRUSTED_PLATFORM_ASSEMBLIES"),
        _X("NATIVE_DLL_SEARCH_DIRECTORIES"),
        _X("PLATFORM_RESOURCE_ROOTS"),
        _X("APP_CONTEXT_BASE_DIRECTORY"),
        _X("APP_CONTEXT_DEPS_FILES"),
        _X("FX_DEPS_FILE"),
        _X("PROBING_DIRECTORIES"),
        _X("STARTUP_HOOKS"),
        _X("APP_PATHS"),
        _X("RUNTIME_IDENTIFIER"),
        _X("BUNDLE_PROBE"),
        _X("HOSTPOLICY_EMBEDDED"),
        _X("PINVOKE_OVERRIDE")
    */

    char currentDirectory[PXPathSizeMax];
    PXSize currentDirectorySize = 0;
    PXDirectoryCurrentA(currentDirectory, PXPathSizeMax, &currentDirectorySize);


    PXFile* asseblyList = PXFileCreate();

    // We need to build string now for the next function
    {
        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual; // No size specified, we get atleast one pagesize
            
        PXFileOpen(asseblyList, &pxFileOpenInfo);
        
        PXText pxTextExtension;
        PXTextFromAdressA(&pxTextExtension, ".dll", 4, 4);

        // Add core DLLs from runtime folder
        {
            char versionFolder[PXPathSizeMax];
            char versionString[64];
            PXVersionToString(pxDotNetCoreCLR->VersionCurrent, versionString);

            const PXSize versionFolderSize = PXTextPrintA(versionFolder, PXPathSizeMax, "%s/%s", CSharpCoreClrPath, versionString);

            PXDirectorySearchCache pxDirectorySearchCache;
            PXFileEntry pxFileEntry;

            PXDirectoryOpenA(&pxDirectorySearchCache, &pxFileEntry, versionFolder);

            do
            {
                const PXBool isTarget = PXFilePathExtensionCompare(&pxFileEntry.FilePath, &pxTextExtension);

                if(!isTarget)
                {
                    continue;
                }

                const PXSize expectedGrouth = versionFolderSize + pxFileEntry.FilePath.SizeUsed + 2;

                PXFileAssureFreeSize(&asseblyList, expectedGrouth);

                PXFileWriteA(&asseblyList, versionFolder, versionFolderSize);
                PXFileWriteC(&asseblyList, '/');
                PXFileWriteText(&asseblyList, &pxFileEntry.FilePath);
                PXFileWriteC(&asseblyList, ';');
            } 
            while(PXDirectoryNext(&pxDirectorySearchCache, &pxFileEntry));

            PXDirectoryClose(&pxDirectorySearchCache);
        }

        // Add additional DLLs from the working directory
        {
            PXDirectorySearchCache pxDirectorySearchCache;
            PXFileEntry pxFileEntry;

            PXDirectoryOpenA(&pxDirectorySearchCache, &pxFileEntry, currentDirectory);  

            do
            {
                if(pxFileEntry.FilePath.SizeUsed < 5)
                {
                    continue;
                }                

                const PXBool isTarget = PXFilePathExtensionGet(&pxFileEntry.FilePath, &pxTextExtension);

                if(!isTarget)
                {
                    continue;
                }

                const PXSize expectedGrouth = currentDirectorySize + pxFileEntry.FilePath.SizeUsed + 2;

                PXFileAssureFreeSize(&asseblyList, expectedGrouth);

                PXFileWriteA(&asseblyList, currentDirectory, currentDirectorySize);
                PXFileWriteC(&asseblyList, '//');
                PXFileWriteText(&asseblyList, &pxFileEntry.FilePath);
                PXFileWriteC(&asseblyList, ';');
            } 
            while(PXDirectoryNext(&pxDirectorySearchCache, &pxFileEntry));

            PXDirectoryClose(&pxDirectorySearchCache);
        }

        // Add additional paths?
    }


    const char* appPropertyKeys[] =
    {
        "TRUSTED_PLATFORM_ASSEMBLIES", // List of trusted assemblies
        "APP_PATHS",                   // Additional paths to probe for assemblies
        "APP_NI_PATHS"                 // Additional paths to probe for native images
    };
    const char* appPropertyValues[] =
    {
       0,// (char*)asseblyList.Data,
        currentDirectory,
        currentDirectory
    };


  //  PXTextReplaceByte(asseblyList.Data, asseblyList.DataCursor, '/', '\\');


    PXSize cursorOffset = 0;

    for(;;)
    {
        char* cursor = 0;//(char*)asseblyList.Data + cursorOffset;
        PXSize size = 0;// PXTextFindFirstCharacterA(cursor, asseblyList.DataCursor - cursorOffset, ';');

        if(size == -1 || size == 0)
        {
            break;
        }

        char buffer[PXPathSizeMax];

        PXTextCopyA(cursor, size, buffer, PXPathSizeMax);

        cursorOffset += size+1;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXDotNetName,
            "Assembly",
            "%s",
            buffer
        );
#endif
    }


#if OSWindowsDotNETAviable
    coreclr_initialize_ptr initialize = (coreclr_initialize_ptr)pxDotNetCoreCLR->Initialize;
    coreclr_set_error_writer_ptr errorWriterSet = (coreclr_set_error_writer_ptr)pxDotNetCoreCLR->ErrorWriterSet;

    const HRESULT initResultID = initialize
    (
        currentDirectory,
        "PXIernal",
        3,
        appPropertyKeys,
        appPropertyValues,
        &pxDotNetCoreCLR->HostHandle,
        &pxDotNetCoreCLR->DomainID
    );
    const PXResult initResult = PXErrorFromHRESULT(initResultID);

    if(errorWriterSet)
    {
        errorWriterSet(PXDotNetCoreCLRErrorCallback);
    }
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "Locate",
        "Results:\n"
        "DLLPath : %s\n"
        "runtimejson : %s",
        "dllFullPath",
        "runtimeConfigPath"
    );
#endif

    return PXActionSuccessful;

#else
// ???
return PXActionRefusedNotImplemented;
#endif
}

PXResult PXAPI PXGetloc(char* buffer)
{
#if OSUnix

    return PXActionRefusedNotImplemented;

#elif OSWindows
    // C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\8.0.13\\runtimes\\win-x64\\native\\nethost.dll"

#if OSWindowsDotNETAviable
	char hostfxrPathW[MAX_PATH];

    PXLibrary pxLibrary;

    PXLibraryOpenA(&pxLibrary, PXDotNetNetHost);


    PXget_hostfxr_path hostfxrPathGet;

    PXLibraryGetSymbolA(&pxLibrary, &hostfxrPathGet, PXDotNetFunc, PXTrue);

    struct get_hostfxr_parameters init_params;
    init_params.size = sizeof(struct hostfxr_initialize_parameters);
    init_params.assembly_path = L"dotnet";
    init_params.dotnet_root = L"C:\\Program Files\\dotnet";



    size_t bufferSize = MAX_PATH;

    const HRESULT resultGetID = hostfxrPathGet(hostfxrPathW, &bufferSize, &init_params);
    const PXResult resultGet = PXErrorFromHRESULT(resultGetID);

    PXLibraryClose(&pxLibrary);

    if(PXActionSuccessful != resultGet)
    {
        return resultGet;
    }

    PXTextCopyWA(hostfxrPathW, bufferSize, buffer, MAX_PATH);

    return PXActionSuccessful;
#endif

#else
    return PXActionRefusedNotImplemented;
#endif
}

PXResult PXAPI PXDotNetInitializeHostFX(PXDotNetHostFX PXREF pxDotNetHostFX)
{
    // Load the hostfxr library
    {
        char hostfxrPath[MAX_PATH];

        const PXResult result = PXGetloc(hostfxrPath);

        if(PXActionSuccessful != result)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXDotNetName,
                "Locate",
                "Failed to find hostfxr location!"
            );
#endif

            return PXActionInvalid;
        }

        PXLibraryOpenA(&pxDotNetHostFX->LibraryHostFX, hostfxrPath);
    } 

    // Load symbols
    {
        PXLibraryFuntionEntry pxFunctionEntryList[] =
        {
            { &pxDotNetHostFX->AvailableSDKsGet, "hostfxr_get_available_sdks"},
            { &pxDotNetHostFX->EnvironmentInfoGet,  "hostfxr_get_dotnet_environment_info" },
            { &pxDotNetHostFX->NativeSearchDirectoriesGet, "hostfxr_get_native_search_directories"},
            { &pxDotNetHostFX->RuntimeDelegateGet,  "hostfxr_get_runtime_delegate"},
            { &pxDotNetHostFX->RuntimePropertiesGet,  "hostfxr_get_runtime_properties"},
            { &pxDotNetHostFX->RuntimePropertyValueGet,  "hostfxr_get_runtime_property_value"},
            { &pxDotNetHostFX->InitializeForCommandLine,  "hostfxr_initialize_for_dotnet_command_line"},
            { &pxDotNetHostFX->InitializeForRuntimeConfig,  "hostfxr_initialize_for_runtime_config"},
            { &pxDotNetHostFX->Main,  "hostfxr_main"},
            { &pxDotNetHostFX->MainBundleStartupInfo,  "hostfxr_main_bundle_startupinfo"},
            { &pxDotNetHostFX->MainStartupInfo,  "hostfxr_main_startupinfo"},
            { &pxDotNetHostFX->ResolveSDK,  "hostfxr_resolve_sdk"},
            { &pxDotNetHostFX->ResolveSDK2,  "hostfxr_resolve_sdk2"},
            { &pxDotNetHostFX->RunApp,  "hostfxr_run_app"},
            { &pxDotNetHostFX->ErrorWriterSet,  "hostfxr_set_error_writer"},
            { &pxDotNetHostFX->RuntimePropertyValueSet, "hostfxr_set_runtime_property_value"}
        };
        const PXSize size = sizeof(pxFunctionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&pxDotNetHostFX->LibraryHostFX, pxFunctionEntryList, size);
    }






    //  hostfxr_get_dotnet_environment_info_fn

    /*

    if (!init || !get_delegate || !close)
    {
        printf("Failed to get hostfxr functions\n");
        FreeLibrary(hostfxr);
        return -1;
    }*/


    _putenv("DOTNET_ROOT=C:\\Program Files\\dotnet");
    _putenv("COREHOST_TRACE=1");
    _putenv("COREHOST_TRACEFILE=host_trace.log");

    /*

    // Create a minimal runtime configuration
    const char* runtimeConfig = "{ \"runtimeOptions\": { \"tfm\": \"net8.0\" } }";
    FILE* configFile = fopen("runtimeconfig.json", "w");
    if (configFile) {
        fputs(runtimeConfig, configFile);
        fclose(configFile);
    }
    else {
        printf("Failed to create runtime configuration file\n");
        FreeLibrary(hostfxr);
        return -1;
    }
    */


#if OSWindowsDotNETAviable
    // Initialize the .NET runtime
    {
        const hostfxr_initialize_for_runtime_config_fn initializeForRuntimeConfig = (hostfxr_initialize_for_runtime_config_fn)pxDotNetHostFX->InitializeForRuntimeConfig;
        const hostfxr_set_error_writer_fn errorWriterSet = (hostfxr_set_error_writer_fn)pxDotNetHostFX->ErrorWriterSet;

        const HRESULT initializeResult = initializeForRuntimeConfig
        (
            L"PXTestDLLRelay.runtimeconfig.json",
            PXNull,
            &pxDotNetHostFX->RuntimeHandle
        );
        const BOOL initRuntime = 0 == initializeResult;

        if(!initRuntime)
        {
            //printf("Failed to initialize .NET runtime\n");
            //  FreeLibrary(hostfxr);
            return PXActionInvalid;
        }

        // Add error callback to find problems instandly
        errorWriterSet(PXDotNetHostFXRuntimeErrorCallback);
    }

  

    // Setup properties
    {

        // HRESULT ccccccc = pxDotNetHostFX->RuntimePropertyValueSet(pxDotNetHostFX->RuntimeHandle, L"TRUSTED_PLATFORM_ASSEMBLIES", L";C:...\\PXTestDLLRelay.dll;");

        // HRESULT sdadss = pxDotNetHostFX->RuntimePropertyValueSet(pxDotNetHostFX->RuntimeHandle, L"APP_PATHS", L";...\\PXTESTCToCSharp\\;");
        const hostfxr_set_runtime_property_value_fn runtimePropertyValueSet = (hostfxr_set_runtime_property_value_fn)pxDotNetHostFX->RuntimePropertyValueSet;

        runtimePropertyValueSet(pxDotNetHostFX->RuntimeHandle, L"TEST_PROPERTY", L"TRUE");



    }


    // Fetch propeteis
    {
        size_t count = 0;
        const char** keys = 0;
        const char** values = 0;

        // Get the amount of properties
        const hostfxr_get_runtime_properties_fn runtimePropertiesGet = (hostfxr_get_runtime_properties_fn)pxDotNetHostFX->RuntimePropertiesGet;

        runtimePropertiesGet(pxDotNetHostFX->RuntimeHandle, &count, 0, 0);

       // printf("Detected <%i> properties\n", count);

#if 0
        keys = (const char_t**)malloc(count * sizeof(char_t*));
        values = (const char_t**)malloc(count * sizeof(char_t*));

        pxDotNetHostFX->RuntimePropertiesGet(pxDotNetHostFX->RuntimeHandle, &count, keys, values);

        for(size_t i = 0; i < count; i++)
        {
            const char_t* key = keys[i];
            const char_t* value = values[i];

           // printf("\n<%i/%i> %-20ls\n%ls\n\n", i + 1, count, key, value);
        }
#endif
    }
    

    // Get the delegate for the managed method
    {
        const hostfxr_get_runtime_delegate_fn runtimeDelegateGet = (hostfxr_get_runtime_delegate_fn)pxDotNetHostFX->RuntimeDelegateGet;
        const hostfxr_handle runtimeHandle = (hostfxr_handle)pxDotNetHostFX->RuntimeHandle;

        runtimeDelegateGet(runtimeHandle, hdt_com_activation, &pxDotNetHostFX->com_activation);
        runtimeDelegateGet(runtimeHandle, hdt_load_in_memory_assembly, &pxDotNetHostFX->load_in_memory_assembly);
        runtimeDelegateGet(runtimeHandle, hdt_winrt_activation, &pxDotNetHostFX->winrt_activation);
        runtimeDelegateGet(runtimeHandle, hdt_com_register, &pxDotNetHostFX->com_register);
        runtimeDelegateGet(runtimeHandle, hdt_com_unregister, &pxDotNetHostFX->com_unregister);
        runtimeDelegateGet(runtimeHandle, hdt_load_assembly_and_get_function_pointer, &pxDotNetHostFX->AssemblyLoadAndFunctionPointerGet);

        if(!pxDotNetHostFX->IsDotNetCore)
        {
            runtimeDelegateGet(runtimeHandle, hdt_get_function_pointer, &pxDotNetHostFX->FunctionPointerGet);
            runtimeDelegateGet(runtimeHandle, hdt_load_assembly, &pxDotNetHostFX->AssemblyLoad);
            runtimeDelegateGet(runtimeHandle, hdt_load_assembly_bytes, &pxDotNetHostFX->AssemblyLoadBytes);
        }
    }
  

    //printf(".NET runtime handle <%p>\n", pxDotNetHostFX->RuntimeHandle);


    pxDotNetHostFX->IsDotNetCore = 0;


    // Call the managed method
   // int x = executeScript();
#endif


    return PXActionSuccessful;
}

PXResult PXAPI PXDotNetDelegateFetchMSCoree(PXDotNetMSCoree PXREF pxDotNetMSCoree, PXDelegate PXREF pxDelegate)
{
#if OSWindowsDotNETAviable
    ICLRRuntimeHost PXREF runtimeHost = (ICLRRuntimeHost*)pxDotNetMSCoree->RuntimeHost;

    HRESULT appReturn = 0;

    wchar_t assemblyPathW[128];
    wchar_t typeNameW[128];
    wchar_t methodNameW[128];

    PXTextCopyAW(pxDelegate->NameLibrary, 128, assemblyPathW, 128);
    PXTextCopyAW(pxDelegate->NameFunction, 128, methodNameW, 128);

    if(pxDelegate->NameNamespace)
    {
        char buffer[128];

        PXTextPrintA(buffer, 128, "%s.%s", pxDelegate->NameNamespace, pxDelegate->NameClass);

        PXTextCopyAW(buffer, 128, typeNameW, 128);
    }
    else
    {
        PXTextCopyAW(pxDelegate->NameClass, 128, typeNameW, 128);
    }



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "DelegateFetch",
        "Result:\n"
        "%25s : %ls\n"
        "%25s : %ls\n"
        "%25s : %ls\n"
        "%25s : %s %ls%s\n",
        "DLL", assemblyPathW,
        "Class", typeNameW,
        "Function", methodNameW,
        "ExpectedFormat", "public static int", methodNameW, "(string)"
    );
#endif


    const HRESULT execResultID = runtimeHost->lpVtbl->ExecuteInDefaultAppDomain
    (
        runtimeHost,
        assemblyPathW,
        typeNameW,
        methodNameW,
        L"Data from C-Library",
        &appReturn
    );
    const PXResult exeResult = PXErrorFromHRESULT(execResultID);

    if(PXActionSuccessful != exeResult)
    {
        return exeResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "DelegateFetch",
        "Result: %i",
        appReturn
    );
#endif
#endif

    return PXActionSuccessful;
}

PXSize PXAPI PXPathCurrentAndAddFileA(char* buffer, char* fileName)
{
    char currentDir[PXPathSizeMax];

    GetCurrentDirectoryA(PXPathSizeMax, currentDir);

    return PXTextPrintA(buffer, PXPathSizeMax, "%s\\%s", currentDir, fileName);
}

PXSize PXAPI PXPathCurrentAndAddFileW(wchar_t* buffer, char* fileName)
{
    char currentDir[PXPathSizeMax];

    PXPathCurrentAndAddFileA(currentDir, fileName);

    return PXTextCopyAW(currentDir, PXPathSizeMax, buffer, PXPathSizeMax * 2);
}

PXResult PXAPI PXDotNetDelegateFetchCoreCLR(PXDotNetCoreCLR PXREF pxDotNetCoreCLR, PXDelegate PXREF pxDelegate)
{
#if OSWindowsDotNETAviable

    char fileNameBuffer[_MAX_PATH];
    char typeNameA[64];
    char* typeNameRef = pxDelegate->NameClass;

    if(pxDelegate->NameNamespace)
    {
        typeNameRef = typeNameA;
        PXTextPrintA(typeNameA, 64, "%s.%s", pxDelegate->NameNamespace, pxDelegate->NameClass);
    }
  
    const PXSize size = PXTextCopyA(pxDelegate->NameLibrary, _MAX_PATH, fileNameBuffer, _MAX_PATH);

    // Delete extension
    const PXSize amount = PXTextFindLastCharacterA(fileNameBuffer, size, '.');

    if(amount != (PXSize)-1)
    {
        fileNameBuffer[amount] = 0;
    }

    // Syntax:
    // HostHandle             : Defined by previous called function "coreclr_initialize_ptr" 
    // DomainID               : Same as HostHandle
    // entryPointAssemblyName : No path, bare name of the DLL but without the extension!!
    // entryPointTypeName     : Format: "NameSpace.ClassName"
    // entryPointMethodName   : Normal ANSI name of the function
    // delegate               : Adress for storage
    const coreclr_create_delegate_ptr delegateCreate = (coreclr_create_delegate_ptr)pxDotNetCoreCLR->DelegateCreate;
    const HRESULT crteateResultID = delegateCreate
    (
        pxDotNetCoreCLR->HostHandle,
        pxDotNetCoreCLR->DomainID,
        fileNameBuffer,
        typeNameRef,
        pxDelegate->NameFunction,
        &pxDelegate->FunctionAdress
    );
    const PXResult crteateResult = PXErrorFromHRESULT(crteateResultID);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "DelegateFetch",
        "Result:\n"
        "%25s : %s\n"
        "%25s : %s\n"
        "%25s : %s\n"
        "%25s : %p\n",
        "DLL", pxDelegate->NameLibrary,
        "Class", typeNameRef,
        "Function", pxDelegate->NameFunction,
        "Adress", pxDelegate->FunctionAdress
    );
#endif

#endif


    return PXActionSuccessful;
}

PXResult PXAPI PXDotNetDelegateFetchHostFX(PXDotNetHostFX PXREF pxDotNetHostFX, PXDelegate PXREF pxDelegate)
{
#if OSWindowsDotNETAviable
    wchar_t fileNameW[_MAX_PATH];// = L"C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\PXUltimaCTest\\PXTestDLLRelay.dll";
    wchar_t classNameW[64]; //L"PXTestDLLRelay.TESTB, TESTB";
    wchar_t functionNameW[64];

    PXPathCurrentAndAddFileW(fileNameW, pxDelegate->NameLibrary);
    PXTextCopyAW(pxDelegate->NameFunction, 64, functionNameW, 64);

    const load_assembly_and_get_function_pointer_fn assemblyLoadAndFunctionPointerGet = (load_assembly_and_get_function_pointer_fn)pxDotNetHostFX->AssemblyLoadAndFunctionPointerGet;
    const load_assembly_fn assemblyLoad = (load_assembly_fn)pxDotNetHostFX->AssemblyLoad;
    const get_function_pointer_fn functionPointerGet = (get_function_pointer_fn)pxDotNetHostFX->FunctionPointerGet;



    /*
    ,
        L"YourNamespace.YourClass, YourAssembly",
        L"YourMethod",
        L"YourNamespace.YourClass+YourMethodDelegate, YourAssembly"
    */

    wchar_t* delegateSignatureRef = 0;
    wchar_t delegateSignatureDef[128];
    char delegateSignatureText[128];

    // Signature check/build
    {
        if(1) // Use default
        {
            delegateSignatureRef = PXNull; // null is defined as this signature
            PXTextPrintA(delegateSignatureText, 128, "public static int %s(IntPtr, int)", pxDelegate->NameFunction);

            PXTextCopyAW("WONKMain", 11, functionNameW, 64);
        }
        else
        {
            // "System.Func<System.String, System.Int32>"

            // build custom format
            delegateSignatureRef = delegateSignatureDef;
            PXTextPrintW(delegateSignatureDef, 64, "PX.TESTClass+YourMethodDelegate"); 
            PXTextPrintA(delegateSignatureText, 64, "Custom");

            PXTextCopyAW("YourMethod", 11, functionNameW, 64);
        }
    }

 


    char dllName[] = "PXTestDLLRelay";

    if(pxDelegate->NameNamespace)
    {
        PXTextPrintW(classNameW, 64, "%s.%s, %s", pxDelegate->NameNamespace, pxDelegate->NameClass, dllName);
    }
    else
    {
        PXTextPrintW(classNameW, 64, "%s, %s", pxDelegate->NameClass, dllName);
    }






    // Use modern API?
    const PXBool canCallModern = assemblyLoad && functionPointerGet;

    if(canCallModern)
    {
        const HRESULT loadAssemblyResultID = assemblyLoad(fileNameW, PXNull, PXNull);
        const PXResult loadAssemblyResult = PXErrorFromHRESULT(loadAssemblyResultID);

        const HRESULT fetchResultID = functionPointerGet
        (
            classNameW,
            functionNameW,
            delegateSignatureRef,
            PXNull,
            PXNull,
            &pxDelegate->FunctionAdress
        );
        const PXResult fetchResult = PXErrorFromHRESULT(fetchResultID);

        PXConsoleWrite(0, 0);
    }
    else
    {
        // System.Action`0[]
        // System.Func`2[System.String,System.Int32]

        const HRESULT loadResultID = assemblyLoadAndFunctionPointerGet
        (
            fileNameW,      // Name must contain DLL, full path 
            classNameW,     // Format: "NameSpace.Class, DLLNameWithoutExtension"
            functionNameW,  // Function name
            delegateSignatureRef,         // L"YourNamespace.YourClass+YourMethodDelegate, YourAssembly", UNMANAGEDCALLERSONLY_METHOD
            PXNull,
            &pxDelegate->FunctionAdress
        );
        const PXResult loadResult = PXErrorFromHRESULT(loadResultID);

        PXConsoleWrite(0, 0);
    }







#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "DelegateFetch",
        "Result:\n"
        "%25s : %ls\n"
        "%25s : %ls\n"
        "%25s : %ls\n"
        "%25s : %s\n"
        "%25s : %p\n",
        "DLL", fileNameW,
        "Class", classNameW,
        "Function", functionNameW,
        "Expected format", delegateSignatureText,
        "Adress", pxDelegate->FunctionAdress
    );
#endif



    // If "INVALID_ARGS" -> DLL found, CLASS:OK, Function: has wrong signature

    //0x80131509
    // 0x80131621

    return PXActionSuccessful;

    /*

    component_entry_point_fn entryPoint = 0;

    if(!pxDotNetHostFX->IsDotNetCore)
    {
        HRESULT loadingResult = pxDotNetHostFX->AssemblyLoad(librayName, nullptr, nullptr);

        printf("Assebmly load <%ls> %i\n", librayName, result);

        HRESULT resultFunctionPointer = pxDotNetHostFX->FunctionPointerGet
        (
            L"PX.TESTClass",
            L"WONKMain",
            nullptr, nullptr, nullptr, (void**)&entryPoint
        );
        HRESULT resultFunctionPointerAA = pxDotNetHostFX->FunctionPointerGet
        (
            L"TESTB",
            L"WONKMain",
            nullptr, nullptr, nullptr, (void**)&entryPoint
        );
        HRESULT resultFunctionPointerBB = pxDotNetHostFX->FunctionPointerGet
        (
            L"TESTB",
            L"NoPam",
            L"System.Action, System.Private.CoreLib",
            nullptr,
            nullptr, 
            (void**)&entryPoint
        );

        const HRESULT notFound = 0x80131522; // COR_E_TYPELOAD
        const HRESULT invalidArgument = 0x80070057; // E_INVALIDARG
        const HRESULT refLoadedButExecExpected = 0x80131058;

        printf("");
    }
    else
    {
    


        printf("");
    }
    */

#endif


    return PXActionSuccessful;
}

PXResult PXAPI PXDotNetExecuteMSCoree(PXDotNetMSCoree PXREF pxDotNetMSCoree)
{
#if OSWindowsDotNETAviable
    ICLRRuntimeHost PXREF runtimeHost = (ICLRRuntimeHost*)pxDotNetMSCoree->RuntimeHost;

    HRESULT appReturn = 0;

    const HRESULT execResult = runtimeHost->lpVtbl->ExecuteInDefaultAppDomain
    (
        runtimeHost,
        L"filePath",
        L"PX.Test",
        L"main",
        L"EEEEEEEEEEEEEEE",
        &appReturn
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDotNetExecuteCoreCLR(PXDotNetCoreCLR PXREF pxDotNetCoreCLR)
{
#if OSWindowsDotNETAviable
    const char* args[] = { "MyScriptApp.dll" };

    unsigned int exitCode = 0;

    const coreclr_execute_assembly_ptr assemblyExecute = (coreclr_execute_assembly_ptr)pxDotNetCoreCLR->AssemblyExecute;
    const HRESULT result = assemblyExecute
    (
        pxDotNetCoreCLR->HostHandle,
        pxDotNetCoreCLR->DomainID,
        1,
        args,
        "C:\\...\\PXTestDLLRelay.dll",
        &exitCode
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDotNetExecuteHostFX(PXDotNetHostFX PXREF pxDotNetHostFX)
{
    return PXActionSuccessful;
}
#endif

PXResult PXAPI PXDotNetCompile(PXDotNet PXREF pxDotNet)
{
#if OSUnix
    return PXActionSuccessful;

#elif OSWindows
    char command[1024];

    char dllName[] = "PXTestDLLRelay";
    char fileName[128];

    // Create the project file
    // netcoreapp3.0
    {
        const char templateFile[] = 
            "<Project Sdk=\"Microsoft.NET.Sdk\">\n"
            "<PropertyGroup>\n"
            "<TargetFramework>net5.0</TargetFramework>\n"
            "<Nullable>enable</Nullable>\n"
            "<AssemblyName>%s</AssemblyName>\n"
            "</PropertyGroup>\n"
            "</Project>";

        const PXSize amount = PXTextPrintA(command, sizeof(command), templateFile, dllName);


        PXTextPrintA(fileName, sizeof(fileName), "_/%s.csproj", dllName);

        FILE* file = fopen(fileName, "w");
        fwrite(command, 1, amount, file);
        fclose(file);
    }

    //
    {

        char currentDir[PXPathSizeMax];
        PXDirectoryCurrentA(currentDir, PXPathSizeMax, PXNull);

        PXTextPrintA(command, sizeof(command), "dotnet build -o _/%s %s", currentDir, fileName);

        const int returnID = system(command);

        PXDirectoryDeleteA("obj"); // Cleanup

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXDotNetName,
            "Initialize",
            "%i",
            returnID
        );
#endif
    }

    return PXActionSuccessful;
#endif
}

PXResult PXAPI PXDotNetInitialize(PXDotNet PXREF pxDotNet, const PXI32U flagList)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDotNetName,
        "Initialize",
        "--- Start ---"
    );
#endif

    PXClear(PXDotNet, pxDotNet);


#if OSUnix
#elif OSWindows

    // Probe for versions

    {
        PXDirectorySearchCache pxDirectorySearchCache;
        PXFileEntry pxFileEntry;

        PXDirectoryOpenA(&pxDirectorySearchCache, &pxFileEntry, CSharpCoreClrPath);

        pxDotNet->CoreCLR.VersionListAmount = 0;


        // Shit solution
        pxDotNet->CoreCLR.VersionList = PXMemoryHeapCallocT(PXVersion, 10);

        do
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXDotNetName,
                "Version",
                "Detected: %s",
                pxFileEntry.FilePath.A
            );
#endif

            PXVersion PXREF pxVersion = &pxDotNet->CoreCLR.VersionList[pxDotNet->CoreCLR.VersionListAmount];

            PXVersionFromString(pxVersion, pxFileEntry.FilePath.A);

            pxDotNet->CoreCLR.VersionCurrent = pxVersion;
            
            ++pxDotNet->CoreCLR.VersionListAmount;
        }
        while(PXDirectoryNext(&pxDirectorySearchCache, &pxFileEntry));

    }




#if 0
    pxDotNet->DelegateFetch = PXDotNetDelegateFetchHostFX;
    pxDotNet->Execute = PXDotNetExecuteHostFX;
    PXActionResult resultA = PXDotNetInitializeHostFX(&pxDotNet->HostFX);

    if(PXActionSuccessful == resultA)
    {
        return PXActionSuccessful;
    }
#endif

#if 1

    pxDotNet->DelegateFetch = PXDotNetDelegateFetchCoreCLR;
    pxDotNet->Execute = PXDotNetExecuteCoreCLR;
    PXActionResult resultB = PXDotNetInitializeCoreCLR(&pxDotNet->CoreCLR);

    if(PXActionSuccessful == resultB)
    {
        return PXActionSuccessful;
    }
#endif

    pxDotNet->DelegateFetch = PXDotNetDelegateFetchMSCoree;
    pxDotNet->Execute = PXDotNetExecuteMSCoree;
    PXActionResult resultC = PXDotNetInitializeMSCoree(&pxDotNet->MSCoree);

    if(PXActionSuccessful == resultC)
    {
        return PXActionSuccessful;
    }

    PXClear(PXDotNet, pxDotNet);
#endif

    return resultC;
}