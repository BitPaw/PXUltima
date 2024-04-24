#include "PXWindows.h"

#if OSWindows

/*
BOOL __stdcall DllMainCRTStartup(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{


    if(fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH)
        if(!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
            return(FALSE);

    if(fdwReason == DLL_PROCESS_DETACH || fdwReason == DLL_THREAD_DETACH)
        if(!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
            return(FALSE);

    return(TRUE);
}*/

#if !WindowsAtleastXP

PVOID WINAPI PXWindowsEncodePointer(PVOID Ptr)
{
    return Ptr;
}

PVOID WINAPI PXWindowsDecodePointer(PVOID Ptr)
{
    return Ptr;
}

BOOL WINAPI PXWindowsGetModuleHandleExW(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE* phModule)
{
    return PXFalse;
}

void WINAPI PXWindowsInitializeSListHead(PSLIST_HEADER ListHead)
{

}

PSINGLE_LIST_ENTRY* WINAPI PXWindowsInterlockedFlushSList(PSLIST_HEADER ListHead)
{
    return PXNull;
}

VOID WINAPI PXWindowsRtlCaptureContext(PCONTEXT ContextRecord)
{

}
#endif

#endif