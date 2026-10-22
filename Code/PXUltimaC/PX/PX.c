#include "PX.h"
#include <PX/OS/Signal/PXSignal.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/PXOS.h>



#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <crtdbg.h> // _CrtSetDbgFlag, _CRTDBG_ALLOC_MEM_DF, _CRTDBG_CHECK_ALWAYS_DF
#include <Objbase.h>
#endif

const char PXSentinelText[] = "Sentinel-PX";


void PXAPI PXSentinelSignalHandle(PXSignalInfo PXREF pxSignalInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        PXSentinelText,
        "Signal",
        "=================================================="
    );
#endif

    PXDebugStackTraceWithSignal(pxSignalInfo);
}

const char PXSentinelInitMessage[] = "PX-Sentinel - Initialization - START\n";
const PXI8U PXSentinelInitMessageLength = sizeof(PXSentinelInitMessage);

const char PXSentineEndMessage[] = "PX-Sentinel - Initialization - FINISHED\n";
const PXI8U PXSentineEndMessageLength = sizeof(PXSentineEndMessage);

void PXAPI PXInitialize()
{
#if PXDEBUG
    // Force the CRT runtime to check every pointer on use.
    // This slows down the program by ALOT, thats why its not doing this by default.
    // But with this, we crash on illegal pointer use, not somewhere else.

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

    HRESULT resultID = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    PXConsoleFetch(PXTrue);
    PXConsoleWrite(PXSentinelInitMessageLength, PXSentinelInitMessage);

    // register failure callbacks on fatal crashes
    PXSignalCallBackSet(PXSentinelSignalHandle);

    // Init´the core system
    PXSystemPrelude();
    PXLogEnableASYNC();

    // Name the current thread to the engine
    {
        PXText pxText;
        PXTextFromAddressA(&pxText, PXSentinelText, sizeof(PXSentinelText), sizeof(PXSentinelText));        

        PXThreadNameSet(PXNull, &pxText);
    }

    // Stage A - System independed
    // Important, to know 
    PXECSInit();


    // Stage B - System depended
    PXThreadPoolCreate(PXNull);


    //host.CreateWorld();
    //host.RegisterAllSystems();
    //host.InitSystems(); // Engine.Init() happens here 
    //host.RunMainLoop();
    //host.Shutdown();

    PXConsoleWrite(PXSentineEndMessageLength, PXSentineEndMessage);
}