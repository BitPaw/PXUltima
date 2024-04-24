#ifndef PXWindowsINCLUDE 
#define PXWindowsINCLUDE

#include <Media/PXResource.h>

#if OSWindows

#include <Windows.h>


BOOL WINAPI DllMainCRTStartup(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);



#if !WindowsAtleastXP

//#include <libloaderapi.h>
//#include <interlockedapi.h>

    int _fltused = 0; // prevent compile errors

    //-----------------------------------------------------
    // Declare prototypes
    //-----------------------------------------------------
    //typedef HMODULE(WINAPI* PXGetModuleHandleA)(_In_opt_ LPCSTR lpModuleName);
    //typedef HMODULE(WINAPI* PXGetModuleHandleW)(_In_opt_ LPCWSTR lpModuleName);

    //typedef BOOL(WINAPI* PXGetModuleHandleExA)(_In_ DWORD dwFlags, _In_opt_ LPCSTR lpModuleName, _Out_ HMODULE* phModule);
    typedef BOOL(WINAPI* PXWindowsGetModuleHandleExWFunction)(_In_ DWORD dwFlags, _In_opt_ LPCWSTR lpModuleName, _Out_ HMODULE* phModule);

    typedef PVOID(WINAPI* PXWindowsEncodePointerFunction)(PVOID Ptr);
    typedef PVOID(WINAPI* PXWindowsDecodePointerFunction)(PVOID Ptr);
    typedef void (WINAPI* PXWindowsInitializeSListHeadFunction)(PSLIST_HEADER ListHead);
    typedef PSINGLE_LIST_ENTRY* (WINAPI* PXWindowsInterlockedFlushSListFunction)(PSLIST_HEADER ListHead); // InterlockedFlushSList
    typedef VOID(WINAPI* PXWindowsRtlCaptureContextFunction)(PCONTEXT ContextRecord); // RtlCaptureContext
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Define globall callback position
    //-----------------------------------------------------
    PXPublic PXWindowsEncodePointerFunction PXWindowsEncodePointerCallBack = PXNull;
    PXPublic PXWindowsDecodePointerFunction PXWindowsDecodePointerCallBack = PXNull;
    PXPublic PXWindowsInitializeSListHeadFunction PXInitializeSListHeadFunctionCallBack = PXNull;
    PXPublic PXWindowsInterlockedFlushSListFunction PXInterlockedFlushSListCallBack = PXNull;
    PXPublic PXWindowsRtlCaptureContextFunction PXRtlCaptureContextCallBack = PXNull;
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Define own implementation 
    //-----------------------------------------------------

    // Windows XP with SP2 (+UWP), Kernel32.dll, Winbase.h 
    // Encodes the specified pointer. Encoded pointers can 
    // be used to provide another layer of protection for pointer values.   
    PXPublic PVOID WINAPI PXWindowsEncodePointer(PVOID Ptr);

    // Windows XP with SP2 (+UWP), Kernel32.dll, Winbase.h 
    // Decodes a pointer that was previously encoded with EncodePointer.
    PXPublic  PVOID WINAPI PXWindowsDecodePointer(PVOID Ptr);

    PXPublic BOOL WINAPI PXWindowsGetModuleHandleExW(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE* phModule);

    // Windows XP (+UWP), Kernel32.dll, interlockedapi.h 
    // Initializes the head of a singly linked list.
    PXPublic void WINAPI PXWindowsInitializeSListHead(PSLIST_HEADER ListHead);

    PXPublic PSINGLE_LIST_ENTRY* WINAPI PXWindowsInterlockedFlushSList(PSLIST_HEADER ListHead);
        
    PXPublic VOID WINAPI PXWindowsRtlCaptureContext(PCONTEXT ContextRecord);
    //-----------------------------------------------------

   
    /*
    extern "C" PVOID WINAPI     LibDecodePointer(PVOID Ptr)
    {
        if(!pfnDecodePointer)
        {
            // Check if the API is provided by kernel32, otherwise fall back to our implementation.
            HMODULE hKernel32 = GetModuleHandleW(L"kernel32");
            pfnDecodePointer = reinterpret_cast<PFN_DECODEPOINTER>(GetProcAddress(hKernel32, "DecodePointer"));
            if(!pfnDecodePointer)
            {
                pfnDecodePointer = _CompatDecodePointer;
            }
        }

        return pfnDecodePointer(Ptr);
    }

    extern "C" PVOID WINAPI       LibEncodePointer(PVOID Ptr)
    {
        if(!pfnEncodePointer)
        {
            // Check if the API is provided by kernel32, otherwise fall back to our implementation.
            HMODULE hKernel32 = GetModuleHandleW(L"kernel32");
            pfnEncodePointer = reinterpret_cast<PFN_ENCODEPOINTER>(GetProcAddress(hKernel32, "EncodePointer"));
            if(!pfnEncodePointer)
            {
                pfnEncodePointer = _CompatEncodePointer;
            }
        }

        return pfnEncodePointer(Ptr);
    }*/


#endif

#endif

#endif