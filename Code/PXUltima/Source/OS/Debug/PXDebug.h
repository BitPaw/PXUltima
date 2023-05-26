#ifndef PXDebugINLCUDE
#define PXDebugINLCUDE

#include <OS/System/OSVersion.h>

#define PXDebugUSE OSDeviceToUse == OSDeviceDestop
#if PXDebugUSE

#include <Media/PXType.h>
#include <Media/PXText.h>

#include <OS/Process/PXProcess.h>
#include <OS/Thread/PXThread.h>
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

	PXPublic void PXDebugConstruct(PXDebug* const pxDebug);
	PXPublic void PXDebugDestruct(PXDebug* const pxDebug);

	// VS Debugger functions-- maybe
	PXPublic PXActionResult PXDebugProcessBeeingDebugged(PXDebug* const pxDebug, PXBool* const isPresent);
	PXPublic PXBool PXDebugProcessBeeingDebuggedCurrent();

	PXPublic void PXDebugDebuggerSendMessage(PXDebug* const pxDebug, PXText* const message);


	PXPublic PXBool PXDebugDebuggerInitialize(PXDebug* const pxDebug);


	PXPublic PXActionResult PXDebugStartProcessA(PXDebug* const pxDebug, const PXText* const applicationName);

	// Enables a debugger to continue a thread that previously reported a debugging event.
	PXPublic void PXDebugContinue(PXDebug* const pxDebug);
	PXPublic void PXDebugPause(PXDebug* const pxDebug);
	PXPublic PXBool PXDebugPauseOther(PXDebug* const pxDebug, const PXProcessID pxProcessID);
	PXPublic void PXDebugAttach(PXDebug* const pxDebug);
	//PXPublic void PXDebugAttachToDebugger(PXDebug* const pxDebug);
	PXPublic void PXDebugDetach(PXDebug* const pxDebug);

	PXPublic void PXDebugStackTrace(PXDebug* const pxDebug);

	PXPublic PXSize PXDebugMemoryRead(PXDebug* const pxDebug, const void* const adress, void* const outputData, const PXSize outputDataSize);
	PXPublic PXSize PXDebugMemoryWrite(PXDebug* const pxDebug, const void* const adress, const void* const inputData, const PXSize inputDataSize);

	// Only the thread that created the process being debugged can call this function.
	PXPublic PXActionResult PXDebugWaitForEvent(PXDebug* const pxDebug);

	PXPrivate void PXDebugLoop(PXDebug* const pxDebug);

#ifdef __cplusplus
}
#endif

#endif
#endif