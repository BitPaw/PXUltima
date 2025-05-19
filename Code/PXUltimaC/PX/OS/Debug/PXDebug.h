#ifndef PXDebugINLCUDE
#define PXDebugINLCUDE

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Async/PXProcess.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/GUI/PXGUI.h>
#include <PX/Media/PXResource.h>

typedef struct PXDebug_
{
    PXBool IsRunning;

    PXProcess Process;

    PXThread EventListenLoop;

    PXByte ApplicatioNameBuffer[260];
    PXText ApplicatioName;

    PXBool IsInit;
}
PXDebug;



PXPublic PXDebug* PXAPI PXDebugInstanceGet();
PXPublic void PXAPI PXDebugInstanceRelease(PXDebug* const pxDebug);


PXPublic void PXAPI PXDebugDebuggerSendMessage(PXDebug* const pxDebug, PXText* const message);


PXPublic PXActionResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug);


PXPublic PXActionResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName);

// Enables a debugger to continue a thread that previously reported a debugging event.
PXPublic PXActionResult PXAPI PXDebugAttach(PXDebug* const pxDebug);
//PXPublic void PXDebugAttachToDebugger(PXDebug* const pxDebug);

// Only the thread that created the process being debugged can call this function.
PXPublic PXActionResult PXAPI PXDebugWaitForEvent(PXDebug* const pxDebug);


PXPublic PXActionResult PXAPI PXDebugLibrarySymbolsFetch(PXDebug* const pxDebug, const PXText* const libraryFilePath, PXSymbol* const pxSymbolList, PXSize* const amount);

PXPublic void PXAPI PXDebugLogMessage(PXText* const pxText);



PXPublic PXActionResult PXAPI PXDebugHeapMemoryList(PXDebug* const pxDebug);

PXPublic PXActionResult PXAPI PXDebugFetchSymbolThread(PXDebug* const pxDebug, PXSymbol* const pxSymbol, PXThread* pxThread);
PXPublic PXActionResult PXAPI PXDebugFetchSymbolFromRougeAdress(PXDebug* const pxDebug, PXSymbol* const pxSymbol, void* adress);

PXPrivate PXThreadResult PXAPI PXDebugLoop(PXDebug* const pxDebug);

PXPublic PXActionResult PXAPI PXDebugDumpCreate(PXDebug* const pxDebug);


// Adress to module HANDLE
PXPublic PXActionResult PXAPI PXDebugModuleHandleFromAdress(PXHandleModule* const pxHandleModule, const void* const adress);



#endif
