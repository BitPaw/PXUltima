#include "PXDotNet.h"

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <MSCorEE.h>
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")
#pragma comment(lib, "format.lib")
//#pragma comment(lib, "mscories.lib")


//#include "coreclrhost.h" // What is this??

// This is not set as a directive, hardcoded, not good

#include "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.13/runtimes/win-x64/native/nethost.h"
#include "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.13/runtimes/win-x64/native/coreclr_delegates.h"
#include "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.13/runtimes/win-x64/native/hostfxr.h"

#pragma comment(lib, "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.13/runtimes/win-x64/native/nethost.lib")





// TODO: REMOVE THIS EMBEDED HEADER. it does not exist in windows for some reson or its somewhere else

// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

//
// APIs for hosting CoreCLR
//

#ifndef __CORECLR_HOST_H__
#define __CORECLR_HOST_H__

#if defined(_WIN32) && defined(_M_IX86)
#define CORECLR_CALLING_CONVENTION __stdcall
#else
#define CORECLR_CALLING_CONVENTION
#endif

#include <stdint.h>

#ifdef __cplusplus
#define CORECLR_HOSTING_API_LINKAGE extern "C"
#else
#define CORECLR_HOSTING_API_LINKAGE
#endif

// For each hosting API, we define a function prototype and a function pointer
// The prototype is useful for implicit linking against the dynamic coreclr
// library and the pointer for explicit dynamic loading (dlopen, LoadLibrary)
#define CORECLR_HOSTING_API(function, ...) \
    CORECLR_HOSTING_API_LINKAGE int CORECLR_CALLING_CONVENTION function(__VA_ARGS__); \
    typedef int (CORECLR_CALLING_CONVENTION *function##_ptr)(__VA_ARGS__)

//
// Initialize the CoreCLR. Creates and starts CoreCLR host and creates an app domain
//
// Parameters:
//  exePath                 - Absolute path of the executable that invoked the ExecuteAssembly (the native host application)
//  appDomainFriendlyName   - Friendly name of the app domain that will be created to execute the assembly
//  propertyCount           - Number of properties (elements of the following two arguments)
//  propertyKeys            - Keys of properties of the app domain
//  propertyValues          - Values of properties of the app domain
//  hostHandle              - Output parameter, handle of the created host
//  domainId                - Output parameter, id of the created app domain
//
// Returns:
//  HRESULT indicating status of the operation. S_OK if the assembly was successfully executed
//
CORECLR_HOSTING_API(coreclr_initialize,
                    const char* exePath,
                    const char* appDomainFriendlyName,
                    int propertyCount,
                    const char** propertyKeys,
                    const char** propertyValues,
                    void** hostHandle,
                    unsigned int* domainId);

//
// Type of the callback function that can be set by the coreclr_set_error_writer
//
typedef void (*coreclr_error_writer_callback_fn) (const char* message);

//
// Set callback for writing error logging
//
// Parameters:
//  errorWriter             - callback that will be called for each line of the error info
//                          - passing in NULL removes a callback that was previously set
//
// Returns:
//  S_OK
//
CORECLR_HOSTING_API(coreclr_set_error_writer,
                    coreclr_error_writer_callback_fn errorWriter);

//
// Shutdown CoreCLR. It unloads the app domain and stops the CoreCLR host.
//
// Parameters:
//  hostHandle              - Handle of the host
//  domainId                - Id of the domain
//
// Returns:
//  HRESULT indicating status of the operation. S_OK if the assembly was successfully executed
//
CORECLR_HOSTING_API(coreclr_shutdown,
                    void* hostHandle,
                    unsigned int domainId);

//
// Shutdown CoreCLR. It unloads the app domain and stops the CoreCLR host.
//
// Parameters:
//  hostHandle              - Handle of the host
//  domainId                - Id of the domain
//  latchedExitCode         - Latched exit code after domain unloaded
//
// Returns:
//  HRESULT indicating status of the operation. S_OK if the assembly was successfully executed
//
CORECLR_HOSTING_API(coreclr_shutdown_2,
                    void* hostHandle,
                    unsigned int domainId,
                    int* latchedExitCode);

//
// Create a native callable function pointer for a managed method.
//
// Parameters:
//  hostHandle              - Handle of the host
//  domainId                - Id of the domain
//  entryPointAssemblyName  - Name of the assembly which holds the custom entry point
//  entryPointTypeName      - Name of the type which holds the custom entry point
//  entryPointMethodName    - Name of the method which is the custom entry point
//  delegate                - Output parameter, the function stores a native callable function pointer to the delegate at the specified address
//
// Returns:
//  HRESULT indicating status of the operation. S_OK if the assembly was successfully executed
//
CORECLR_HOSTING_API(coreclr_create_delegate,
                    void* hostHandle,
                    unsigned int domainId,
                    const char* entryPointAssemblyName,
                    const char* entryPointTypeName,
                    const char* entryPointMethodName,
                    void** delegate);

//
// Execute a managed assembly with given arguments
//
// Parameters:
//  hostHandle              - Handle of the host
//  domainId                - Id of the domain
//  argc                    - Number of arguments passed to the executed assembly
//  argv                    - Array of arguments passed to the executed assembly
//  managedAssemblyPath     - Path of the managed assembly to execute (or NULL if using a custom entrypoint).
//  exitCode                - Exit code returned by the executed assembly
//
// Returns:
//  HRESULT indicating status of the operation. S_OK if the assembly was successfully executed
//
CORECLR_HOSTING_API(coreclr_execute_assembly,
                    void* hostHandle,
                    unsigned int domainId,
                    int argc,
                    const char** argv,
                    const char* managedAssemblyPath,
                    unsigned int* exitCode);

#undef CORECLR_HOSTING_API

//
// Callback types used by the hosts
//
//typedef bool(CORECLR_CALLING_CONVENTION BundleProbeFn)(const char* path, int64_t* offset, int64_t* size, int64_t* compressedSize);
typedef const void* (CORECLR_CALLING_CONVENTION PInvokeOverrideFn)(const char* libraryName, const char* entrypointName);


#endif // __CORECLR_HOST_H__




#endif


#include <OS/Console/PXConsole.h>


const char CSharpCoreClrPath[] = "C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App\\";
const char CSharpCoreClrDLLName[] = "coreclr.dll";
const char CSharpMSCoree[] = "mscoree.dll";


void PXDotNetCoreCLRErrorCallback(const char* message)
{
    printf
    (
        "\n\n"
        "------------------------------------------------\n"
        "DotNET Error : %s\n"
        "------------------------------------------------\n"
        "\n\n",
        message);
}

void HOSTFXR_CALLTYPE PXDotNetHostFXRuntimeErrorCallback(const char_t* message)
{
    printf
    (
        "\n\n"
        "------------------------------------------------\n"
        "DotNET Error : %ls\n"
        "------------------------------------------------\n"
        "\n\n",
        message);
}



PXActionResult PXAPI PXDotNetInitializeMSCoree(PXDotNetMSCoree* const pxDotNetMSCoree)
{
#if OSWindows
    CreateInterfaceFnPtr createInterface;

    // open
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDotNetMSCoree->Library, CSharpMSCoree);

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

    ICLRMetaHost* const metaHost = (ICLRMetaHost*)pxDotNetMSCoree->MetaHost;
    ICLRMetaHostPolicy* const metaHostPolicy = (ICLRMetaHostPolicy*)pxDotNetMSCoree->MetaHostPolicy;
    ICLRDebugging* const debugging = (ICLRDebugging*)pxDotNetMSCoree->Debugging;
    ICLRRuntimeInfo* runtimeInfo = PXNull;

    IEnumUnknown* enumUnknown = 0; // ICLRRuntimeInfo
    ULONG fetched = 0;


    WCHAR bufferVersion[64];

    // Get a list of all versions
    {
        const HRESULT resultID = metaHost->lpVtbl->EnumerateInstalledRuntimes(metaHost, &enumUnknown);
        const PXActionResult result = PXErrorCurrent(S_OK == resultID);

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
                "DotNet",
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
    ICLRRuntimeHost* const runtimeHost = (ICLRRuntimeHost*)pxDotNetMSCoree->RuntimeHost;

    // Start the runtime
    const HRESULT asddssd = runtimeHost->lpVtbl->Start(runtimeHost);

    // Done..?
    WCHAR filePath[] = L"C:\\Data\\WorkSpace\\PXLibTest.dll";

    //  FILE* ff = _wfopen(filePath, L"r");


    HRESULT appReturn = 0;

    const HRESULT execResult = runtimeHost->lpVtbl->ExecuteInDefaultAppDomain
    (
        runtimeHost,
        filePath,
        L"PX.Test",
        L"main",
        L"EEEEEEEEEEEEEEE",
        &appReturn
    );

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DotNet",
        "Init",
        "--- Done ---"
    );
#endif






    /*

        /*


    const HRESULT asdasdt = runtimeHost->lpVtbl->GetDefaultDomain(runtimeHost, &pxDotNet->AppDomain);

    IAppDomainBinding* const appDomainBinding = (IAppDomainBinding*)pxDotNet->AppDomain;

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

    return PXActionSuccessful;

#else
return PXActionRefusedTypeNotSupported;
#endif
}

PXActionResult PXAPI PXDotNetInitializeCoreCLR(PXDotNetCoreCLR* const pxDotNetCoreCLR)
{
    // Look into folder. If it exists
     // If the folder does not exist, we dont have a runtime.
     // if it exists, get the folder names, those are the versions
    char targetedVersionFileText[] = "8.0.13";

    char runtimeDLLPath[_MAX_PATH];

    PXTextPrintA(runtimeDLLPath, _MAX_PATH, "%s%s\\%s", CSharpCoreClrPath, targetedVersionFileText, CSharpCoreClrDLLName);

    PXLibraryOpenA(&pxDotNetCoreCLR->LibraryCoreCLR, runtimeDLLPath);

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

        PXLibraryGetSymbolListA(&pxDotNetCoreCLR->LibraryCoreCLR, pxFunctionEntryList, sizeof(pxFunctionEntryList));
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

    char currentDir[_MAX_PATH];

    GetCurrentDirectoryA(_MAX_PATH, currentDir);



    const char* appPropertyKeys[] =
    {
        "TRUSTED_PLATFORM_ASSEMBLIES", // List of trusted assemblies
        "APP_PATHS",                   // Additional paths to probe for assemblies
        "APP_NI_PATHS"                 // Additional paths to probe for native images
    };
    const char* appPropertyValues[] =
    {
        "PXTestDLLRelay.dll",
        currentDir,
        currentDir
    };

    coreclr_initialize_ptr initialize = (coreclr_initialize_ptr)pxDotNetCoreCLR->Initialize;
    coreclr_set_error_writer_ptr errorWriterSet = (coreclr_set_error_writer_ptr)pxDotNetCoreCLR->ErrorWriterSet;

    const HRESULT result = initialize
    (
        "PXTestDLLRelay.runtimeconfig.json",
        "PX_Internal",
        3,
        appPropertyKeys,
        appPropertyValues,
        &pxDotNetCoreCLR->HostHandle,
        &pxDotNetCoreCLR->DomainID
    );

    if(errorWriterSet)
    {
        errorWriterSet(PXDotNetCoreCLRErrorCallback);
    }

    return PXActionSuccessful;
}

const char PXDotNetNetHost[] =
//"nethost.dll";
"C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\8.0.13\\runtimes\\win-x64\\native\\nethost.dll";
const char PXDotNetFunc[] = "get_hostfxr_path";

typedef int (NETHOST_CALLTYPE* PXget_hostfxr_path)(
    char_t* buffer,
    size_t* buffer_size,
    const struct get_hostfxr_parameters* parameters);


PXActionResult PXAPI PXGetloc(char* buffer)
{
    // C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\8.0.13\\runtimes\\win-x64\\native\\nethost.dll"
    char_t hostfxrPathW[MAX_PATH];

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
    const PXActionResult resultGet = PXErrorCurrent(0 == resultGetID);

    PXLibraryClose(&pxLibrary);

    if(PXActionSuccessful != resultGet)
    {
        return resultGet;
    }

    PXTextCopyWA(hostfxrPathW, bufferSize, buffer, MAX_PATH);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetInitializeHostFX(PXDotNetHostFX* const pxDotNetHostFX)
{
    // Load the hostfxr library
    {
        char hostfxrPath[MAX_PATH];

        const PXActionResult result = PXGetloc(hostfxrPath);

        if(PXActionSuccessful != result)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "DotNET",
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
            return -1;
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
        const char_t** keys = 0;
        const char_t** values = 0;

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


    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetDelegateFetchMSCoree(PXDotNetMSCoree* const pxDotNetMSCoree, PXDelegate* const pxDelegate)
{


    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetDelegateFetchCoreCLR(PXDotNetCoreCLR* const pxDotNetCoreCLR, PXDelegate* const pxDelegate)
{
    const coreclr_create_delegate_ptr delegateCreate = (coreclr_create_delegate_ptr)pxDotNetCoreCLR->DelegateCreate;
    const HRESULT crteateResult = delegateCreate
    (
        pxDotNetCoreCLR->HostHandle,
        pxDotNetCoreCLR->DomainID,
        "PXTestDLLRelay", // NO ".DLL" !!
        "PX.TESTClass",
        pxDelegate->NameFunction,
        &pxDelegate->FunctionAdress
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetDelegateFetchHostFX(PXDotNetHostFX* const pxDotNetHostFX, PXDelegate* const pxDelegate)
{
    wchar_t functionNameW[64];

    PXTextCopyAW(pxDelegate->NameFunction, 64, functionNameW, 64);
    
    const load_assembly_and_get_function_pointer_fn assemblyLoadAndFunctionPointerGet = (load_assembly_and_get_function_pointer_fn)pxDotNetHostFX->AssemblyLoadAndFunctionPointerGet;

    const HRESULT loadResultID = assemblyLoadAndFunctionPointerGet
    (
        L"PXTestDLLRelay.dll", // Name must contain DLL 
        L"PXTestDLLRelay.TESTB, TESTB",
        functionNameW,
        UNMANAGEDCALLERSONLY_METHOD,
        PXNull,
        &pxDelegate->FunctionAdress
    );
    const PXActionResult loadResult = PXErrorCurrent(0 == loadResultID);

    return loadResult;

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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetExecuteMSCoree(PXDotNetMSCoree* const pxDotNetMSCoree)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetExecuteCoreCLR(PXDotNetCoreCLR* const pxDotNetCoreCLR)
{
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetExecuteHostFX(PXDotNetHostFX* const pxDotNetHostFX)
{


    return PXActionSuccessful;
}

PXActionResult PXAPI PXDotNetInitialize(PXDotNet* const pxDotNet, const PXInt32U flagList)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DotNet",
        "Initialize",
        "--- Start ---"
    );
#endif

    PXClear(PXDotNet, pxDotNet);

    
    pxDotNet->DelegateFetch = PXDotNetDelegateFetchHostFX;
    pxDotNet->Execute = PXDotNetExecuteHostFX;
    PXActionResult resultA = PXDotNetInitializeHostFX(&pxDotNet->HostFX);

    if(PXActionSuccessful == resultA)
    {
        return PXActionSuccessful;
    }

    pxDotNet->DelegateFetch = PXDotNetDelegateFetchCoreCLR;
    pxDotNet->Execute = PXDotNetExecuteCoreCLR;
    PXActionResult resultB = PXDotNetInitializeCoreCLR(&pxDotNet->CoreCLR);

    if(PXActionSuccessful == resultA)
    {
        return PXActionSuccessful;
    }

    pxDotNet->DelegateFetch = PXDotNetDelegateFetchMSCoree;
    pxDotNet->Execute = PXDotNetExecuteMSCoree;
    PXActionResult resultC = PXDotNetInitializeMSCoree(&pxDotNet->MSCoree);

    if(PXActionSuccessful == resultA)
    {
        return PXActionSuccessful;
    }

    PXClear(PXDotNet, pxDotNet);

    return resultC;
}