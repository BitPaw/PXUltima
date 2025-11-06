#pragma once

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
PXPublic void PXAPI PXDebugInstanceRelease(PXDebug PXREF pxDebug);


PXPublic void PXAPI PXDebugDebuggerSendMessage(PXDebug PXREF pxDebug, PXText PXREF message);


PXPublic PXResult PXAPI PXDebugDebuggerInitialize(PXDebug PXREF pxDebug);


PXPublic PXResult PXAPI PXDebugStartProcess(PXDebug PXREF pxDebug, const PXText PXREF applicationName);

// Enables a debugger to continue a thread that previously reported a debugging event.
PXPublic PXResult PXAPI PXDebugAttach(PXDebug PXREF pxDebug);
//PXPublic void PXDebugAttachToDebugger(PXDebug PXREF pxDebug);

// Only the thread that created the process being debugged can call this function.
PXPublic PXResult PXAPI PXDebugWaitForEvent(PXDebug PXREF pxDebug);


PXPublic PXResult PXAPI PXDebugLibrarySymbolsFetch(PXDebug PXREF pxDebug, const PXText PXREF libraryFilePath, PXSymbol PXREF pxSymbolList, PXSize PXREF amount);

PXPublic void PXAPI PXDebugLogMessage(PXText PXREF pxText);



PXPublic PXResult PXAPI PXDebugHeapMemoryList(PXDebug PXREF pxDebug);

PXPublic PXResult PXAPI PXDebugFetchSymbolThread(PXDebug PXREF pxDebug, PXSymbol PXREF pxSymbol, PXThread* pxThread);
PXPublic PXResult PXAPI PXDebugFetchSymbolFromRougeAdress(PXDebug PXREF pxDebug, PXSymbol PXREF pxSymbol, void* adress);

PXPrivate PXThreadResult PXAPI PXDebugLoop(PXDebug PXREF pxDebug);

PXPublic PXResult PXAPI PXDebugDumpCreate(PXDebug PXREF pxDebug);


// Adress to module HANDLE
//PXPublic PXResult PXAPI PXDebugModuleHandleFromAdress(PXHandleModule PXREF pxHandleModule, const void PXREF adress);



#endif
