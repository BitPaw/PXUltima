#include "PXWindows.h"

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