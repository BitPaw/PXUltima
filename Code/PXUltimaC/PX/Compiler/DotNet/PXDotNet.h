#ifndef PXDotNetIncludedd
#define PXDotNetIncludedd

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

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


typedef struct PXDelegate_
{
    char* NameLibrary;
    char* NameNamespace;
    char* NameClass;
    char* NameFunction;
    void* FunctionAdress;
}
PXDelegate;


typedef PXActionResult(PXAPI* PXDotNetDelegateFetch)(void* const pxDotNetEngine, PXDelegate* const pxDelegate);
typedef PXActionResult(PXAPI* PXDotNetExecute)(void* const pxDotNetEngine);

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

PXPrivate PXResult PXAPI  PXDotNetInitializeMSCoree(PXDotNetMSCoree* const pxDotNetMSCoree);
PXPrivate PXResult PXAPI  PXDotNetInitializeCoreCLR(PXDotNetCoreCLR* const pxDotNetCoreCLR);
PXPrivate PXResult PXAPI  PXDotNetInitializeHostFX(PXDotNetHostFX* const pxDotNetHostFX);

PXPrivate PXResult PXAPI  PXDotNetDelegateFetchMSCoree(PXDotNetMSCoree* const pxDotNetMSCoree, PXDelegate* const pxDelegate);
PXPrivate PXResult PXAPI  PXDotNetDelegateFetchCoreCLR(PXDotNetCoreCLR* const pxDotNetCoreCLR, PXDelegate* const pxDelegate);
PXPrivate PXResult PXAPI  PXDotNetDelegateFetchHostFX(PXDotNetHostFX* const pxDotNetHostFX, PXDelegate* const pxDelegate);

PXPrivate PXResult PXAPI  PXDotNetExecuteMSCoree(PXDotNetMSCoree* const pxDotNetMSCoree);
PXPrivate PXResult PXAPI  PXDotNetExecuteCoreCLR(PXDotNetCoreCLR* const pxDotNetCoreCLR);
PXPrivate PXResult PXAPI  PXDotNetExecuteHostFX(PXDotNetHostFX* const pxDotNetHostFX);

PXPublic PXResult PXAPI PXDotNetCompile(PXDotNet* const pxDotNet);

PXPublic PXResult PXAPI PXDotNetInitialize(PXDotNet* const pxDotNet, const PXI32U flagList);

#endif