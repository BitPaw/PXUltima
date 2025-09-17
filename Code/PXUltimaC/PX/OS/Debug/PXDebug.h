#ifndef PXDebugINLCUDE
#define PXDebugINLCUDE

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Async/PXProcess.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/Engine/PXResource.h>
#include <PX/Engine/PXGUI.h>

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


PXPublic PXResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug);


PXPublic PXResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName);

// Enables a debugger to continue a thread that previously reported a debugging event.
PXPublic PXResult PXAPI PXDebugAttach(PXDebug* const pxDebug);
//PXPublic void PXDebugAttachToDebugger(PXDebug* const pxDebug);

// Only the thread that created the process being debugged can call this function.
PXPublic PXResult PXAPI PXDebugWaitForEvent(PXDebug* const pxDebug);


PXPublic PXResult PXAPI PXDebugLibrarySymbolsFetch(PXDebug* const pxDebug, const PXText* const libraryFilePath, PXSymbol* const pxSymbolList, PXSize* const amount);

PXPublic void PXAPI PXDebugLogMessage(PXText* const pxText);



PXPublic PXResult PXAPI PXDebugHeapMemoryList(PXDebug* const pxDebug);

PXPublic PXResult PXAPI PXDebugFetchSymbolThread(PXDebug* const pxDebug, PXSymbol* const pxSymbol, PXThread* pxThread);
PXPublic PXResult PXAPI PXDebugFetchSymbolFromRougeAdress(PXDebug* const pxDebug, PXSymbol* const pxSymbol, void* adress);

PXPrivate PXThreadResult PXAPI PXDebugLoop(PXDebug* const pxDebug);

PXPublic PXResult PXAPI PXDebugDumpCreate(PXDebug* const pxDebug);


// Adress to module HANDLE
PXPublic PXResult PXAPI PXDebugModuleHandleFromAdress(PXHandleModule* const pxHandleModule, const void* const adress);



#endif
