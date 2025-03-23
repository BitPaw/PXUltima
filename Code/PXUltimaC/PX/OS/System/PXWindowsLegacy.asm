.model flat

EXTERN _PXWindowsDecodePointer@4 : PROC
EXTERN _PXWindowsEncodePointer@4 : PROC

EXTERN _PXWindowsGetModuleHandleExW@12 : PROC

EXTERN _PXWindowsInitializeSListHead@4 : PROC
EXTERN _PXWindowsInterlockedFlushSList@4 : PROC
EXTERN _PXWindowsRtlCaptureContext@4 : PROC

.data

PUBLIC __imp__DecodePointer@4
__imp__DecodePointer@4 dd _PXWindowsDecodePointer@4

PUBLIC __imp__EncodePointer@4
__imp__EncodePointer@4 dd _PXWindowsEncodePointer@4


PUBLIC __imp__GetModuleHandleExW@12
__imp__GetModuleHandleExW@12 dd _PXWindowsGetModuleHandleExW@12


PUBLIC __imp__InitializeSListHead@4
__imp__InitializeSListHead@4 dd _PXWindowsInitializeSListHead@4

PUBLIC __imp__InterlockedFlushSList@4
__imp__InterlockedFlushSList@4 dd _PXWindowsInterlockedFlushSList@4

PUBLIC __imp__RtlCaptureContext@4
__imp__RtlCaptureContext@4 dd _PXWindowsRtlCaptureContext@4

END