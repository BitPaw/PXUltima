#include "PXSentinel.h"
#include <PX/OS/Signal/PXSignal.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXGUI.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/PXOS.h>

const char PXSentinelText[] = "Sentinel-PX";

void PXCDECL PXSentinelOnIllegalInstruction(const int signalID)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        PXSentinelText,
        "Signal-SIGILL",
        "CPU tryed to exectue illegal instruction!"
    );
#endif
}

void PXCDECL PXSentinelOnMemoryViolation(const int signalID)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        PXSentinelText,
        "Signal-SIGSEGV",
        "Illegal memory access by CPU!"
    );
#endif
}

const char PXSentinelInitMessage[] = "PX-Sentinel - Initialization - START\n";
const PXI8U PXSentinelInitMessageLength = sizeof(PXSentinelInitMessage);

const char PXSentineEndMessage[] = "PX-Sentinel - Initialization - FINISHED\n";
const PXI8U PXSentineEndMessageLength = sizeof(PXSentineEndMessage);

void PXAPI PXSentinelInit()
{
    PXConsoleWrite(PXSentinelInitMessageLength, PXSentinelInitMessage);

    // register failure callbacks on fatal crashes
    PXSignalCallBackRegister(PXSignalTokenIllegalInstruction, PXSentinelOnIllegalInstruction);
    PXSignalCallBackRegister(PXSignalTokenMemoryViolation, PXSentinelOnMemoryViolation);

    // Init´the core system
    PXSystemPrelude();
    PXLogEnableASYNC();

    // Name the current thread to the engine
    {
        PXText pxText;
        PXTextFromAdressA(&pxText, PXSentinelText, sizeof(PXSentinelText), sizeof(PXSentinelText));        

        PXThreadNameSet(PXNull, &pxText);
    }

    // Stage A - System independed
    // Important, to know 
    PXECSInit();

    // Stage B - System depended
    PXThreadPoolCreate(PXNull);
    PXGUIInitialize();


    //host.CreateWorld();
    //host.RegisterAllSystems();
    //host.InitSystems(); // Engine.Init() happens here 
    //host.RunMainLoop();
    //host.Shutdown();

    PXConsoleWrite(PXSentineEndMessageLength, PXSentineEndMessage);

    return PXActionSuccessful;
}