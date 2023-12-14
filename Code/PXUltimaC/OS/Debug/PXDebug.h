#ifndef PXDebugINLCUDE
#define PXDebugINLCUDE

#include <OS/System/PXOSVersion.h>

#define PXDebugUSE OSDeviceToUse == OSDeviceDestop
#if PXDebugUSE

#include <Media/PXType.h>
#include <Media/PXText.h>

#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXDebug_
	{
		PXBool IsRunning;

		PXProcess Process;

		PXThread EventListenLoop;

		PXByte ApplicatioNameBuffer[260];
		PXText ApplicatioName;
	}
	PXDebug;

	PXPublic void PXAPI PXDebugConstruct(PXDebug* const pxDebug);
	PXPublic void PXAPI PXDebugDestruct(PXDebug* const pxDebug);

	// VS Debugger functions-- maybe
	PXPublic PXActionResult PXAPI PXDebugProcessBeeingDebugged(PXDebug* const pxDebug, PXBool* const isPresent);
	PXPublic PXBool PXAPI PXDebugProcessBeeingDebuggedCurrent();

	PXPublic void PXAPI PXDebugDebuggerSendMessage(PXDebug* const pxDebug, PXText* const message);


	PXPublic PXActionResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug);


	PXPublic PXActionResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName);

	// Enables a debugger to continue a thread that previously reported a debugging event.
	PXPublic void PXAPI PXDebugContinue(PXDebug* const pxDebug);
	PXPublic void PXAPI PXDebugPause(PXDebug* const pxDebug);
	PXPublic PXBool PXAPI PXDebugPauseOther(PXDebug* const pxDebug, const PXProcessHandle pxProcessHandle);
	PXPublic PXActionResult PXAPI PXDebugAttach(PXDebug* const pxDebug);
	//PXPublic void PXDebugAttachToDebugger(PXDebug* const pxDebug);
	PXPublic PXActionResult PXAPI PXDebugDetach(PXDebug* const pxDebug);

	PXPublic void PXAPI PXDebugStackTrace(PXDebug* const pxDebug);

	PXPublic PXSize PXAPI PXDebugMemoryRead(PXDebug* const pxDebug, const void* const adress, void* const outputData, const PXSize outputDataSize);
	PXPublic PXSize PXAPI PXDebugMemoryWrite(PXDebug* const pxDebug, const void* const adress, const void* const inputData, const PXSize inputDataSize);

	// Only the thread that created the process being debugged can call this function.
	PXPublic PXActionResult PXAPI PXDebugWaitForEvent(PXDebug* const pxDebug);

	PXPrivate PXThreadResult PXAPI PXDebugLoop(PXDebug* const pxDebug);

#ifdef __cplusplus
}
#endif

#endif
#endif