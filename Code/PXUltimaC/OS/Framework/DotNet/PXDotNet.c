#include "PXDotNet.h"

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <MSCorEE.h>
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")
#pragma comment(lib, "format.lib")
//#pragma comment(lib, "mscories.lib")



#endif

#include <OS/Console/PXConsole.h>

PXActionResult PXAPI PXDotNetInit(PXDotNet* const pxDotNet)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DotNet",
        "Init",
        "--- Start ---"
    );
#endif

    PXClear(PXDotNet, pxDotNet);

#if OSWindows
    CreateInterfaceFnPtr createInterface;

    // open
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDotNet->Library, "mscoree.dll");
        
        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }

        PXLibraryGetSymbolA(&pxDotNet->Library, &createInterface, "CreateInterface", PXTrue);
    }


    // fetch
    {
        const HRESULT resA = createInterface(&CLSID_CLRMetaHost,       &IID_ICLRMetaHost,       &pxDotNet->MetaHost);
        //const HRESULT resB = createInterface(&CLSID_CLRMetaHostPolicy, &IID_ICLRMetaHostPolicy, &pxDotNet->MetaHostPolicy);
        //const HRESULT resC = createInterface(&CLSID_CLRDebugging,      &IID_ICLRDebugging,      &pxDotNet->Debugging);
    }

    ICLRMetaHost* const metaHost = (ICLRMetaHost*)pxDotNet->MetaHost;
    ICLRMetaHostPolicy* const metaHostPolicy = (ICLRMetaHostPolicy*)pxDotNet->MetaHostPolicy;
    ICLRDebugging* const debugging = (ICLRDebugging*)pxDotNet->Debugging;
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

        for(PXSize i = 0 ; S_OK == enumUnknown->lpVtbl->Next(enumUnknown, 1, &runtimeInfoList, &fetched) ; ++i)
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
        const HRESULT hresult = metaHost->lpVtbl->GetRuntime(metaHost, bufferVersion, &IID_ICLRRuntimeInfo, &pxDotNet->RuntimeInfo);

        runtimeInfo = (ICLRRuntimeInfo*)pxDotNet->RuntimeInfo;

        // &CLSID_CorRuntimeHost, &IID_ICorRuntimeHost
        const HRESULT hresultre = runtimeInfo->lpVtbl->GetInterface(runtimeInfo, &CLSID_CLRRuntimeHost, &IID_ICLRRuntimeHost, &pxDotNet->RuntimeHost);
    }

    //ICorRuntimeHost
    ICLRRuntimeHost * const runtimeHost = (ICLRRuntimeHost*)pxDotNet->RuntimeHost;

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