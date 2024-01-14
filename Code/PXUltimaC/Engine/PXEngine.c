#include "PXEngine.h"

#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>
#include <OS/Async/PXProcess.h>

void PXCDECL PXEngineOnIllegalInstruction(const int signalID)
{
    PXLogPrint
    (
        PXLoggingFailure,
        "PX",
        "CPU tryed to exectue illegal instruction!"
    );
}

void PXCDECL PXEngineOnMemoryViolation(const int signalID)
{
    PXLogPrint
    (
        PXLoggingFailure,
        "PX",
        "CPU accessed memory illegally!"
    );
}

void PXAPI PXEngineUpdate(PXEngine* const pxEngine)
{
    const PXInt64U timeNow = PXTimeCounterStampGet();

    pxEngine->CounterTimeDelta = timeNow - pxEngine->CounterTimeLast;
    pxEngine->CounterTimeLast = timeNow;
    pxEngine->TimeFrequency = PXTimeCounterFrequencyGet();
    pxEngine->FramesPerSecound = PXMathFloor(pxEngine->TimeFrequency / (float)pxEngine->CounterTimeDelta);
    pxEngine->FrameTime = PXMathCeilingF((1000000 * pxEngine->CounterTimeDelta) / (float)pxEngine->TimeFrequency);

    pxEngine->CounterTimeWindow = PXTimeCounterStampGet();
    //PXWindowUpdate(&pxEngine->Window);
    pxEngine->CounterTimeWindow = PXTimeCounterStampGet() - pxEngine->CounterTimeWindow;

    pxEngine->CounterTimeUser = PXTimeCounterStampGet();
    PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine);
    pxEngine->CounterTimeUser = PXTimeCounterStampGet() - pxEngine->CounterTimeUser;
    
    pxEngine->CounterTimeNetwork = PXTimeCounterStampGet();
    PXFunctionInvoke(pxEngine->OnNetworkUpdate, pxEngine->Owner, pxEngine);
    pxEngine->CounterTimeNetwork = PXTimeCounterStampGet() - pxEngine->CounterTimeNetwork;

    pxEngine->CounterTimeCPU = PXTimeCounterStampGet();
    PXFunctionInvoke(pxEngine->OnGameUpdate, pxEngine->Owner, pxEngine);
    pxEngine->CounterTimeCPU = PXTimeCounterStampGet() - pxEngine->CounterTimeCPU;

    if ((timeNow - pxEngine->CounterTimeRenderLast) > 0.02)
    {
        pxEngine->CounterTimeRenderLast = timeNow;

        const PXColorRGBAF color = {0.01,0.01,0.01,1};

        pxEngine->Graphic.Clear(pxEngine->Graphic.EventOwner, &color);
        pxEngine->CounterTimeGPU = PXTimeCounterStampGet();
        PXFunctionInvoke(pxEngine->OnRenderUpdate, pxEngine->Owner, pxEngine);
        pxEngine->CounterTimeGPU = PXTimeCounterStampGet() - pxEngine->CounterTimeGPU;
        pxEngine->Graphic.SceneDeploy(pxEngine->Graphic.EventOwner);
    }

    ++(pxEngine->CounterTimeWindow);
    ++(pxEngine->CounterTimeUser);
    ++(pxEngine->CounterTimeNetwork);
    ++(pxEngine->CounterTimeCPU);
    ++(pxEngine->CounterTimeGPU);

#if 0
    PXConsoleGoToXY(0, 0);

    printf("%15s : Hz %-20i\n", "FPS", pxEngine->FramesPerSecound);
    printf("%15s : us %-20i\n", "Frame-Time", pxEngine->FrameTime);
    printf("%15s : us %-20i %3i%%\n", "Window", pxEngine->CounterTimeWindow, (int)((pxEngine->CounterTimeWindow / (float)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "User", pxEngine->CounterTimeUser, (int)((pxEngine->CounterTimeUser / (float)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "Network", pxEngine->CounterTimeNetwork, (int)((pxEngine->CounterTimeNetwork / (float)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "Gamelogic", pxEngine->CounterTimeCPU, (int)((pxEngine->CounterTimeCPU / (float)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "Render", pxEngine->CounterTimeGPU, (int)((pxEngine->CounterTimeGPU / (float)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i\n", "Full rotation", pxEngine->CounterTimeDelta);

    printf("\n");

    PXKeyBoardInputPrint(&pxEngine->Window.KeyBoardCurrentInput);

    PXMouseInputPrint(&pxEngine->Window.MouseCurrentInput);
#endif
}

PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine)
{
    return pxEngine->IsRunning;
}

void PXAPI PXEngineStart(PXEngine* const pxEngine)
{
    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Starting..."
    );

	PXSignalCallBackRegister(PXSignalTokenIllegalInstruction, PXEngineOnIllegalInstruction);
	PXSignalCallBackRegister(PXSignalTokenMemoryViolation, PXEngineOnMemoryViolation);

    {
        PXText pxText;
        PXTextMakeFixedA(&pxText, "PXEngineMain");

        PXThreadNameSet(PXNull, &pxText);
    }

  


    // Load all mods now, not fully tho, they may need very early checks before anything happens
    {

        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Loading Mods..."
        );

      
        PXText pxText;
        PXTextMakeFixedA(&pxText, "Mod\\");

        PXModLoaderScan(&pxEngine->ModLoader, &pxText);
    }




	// Create window
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Creating Window..."
        );

        PXWindowConstruct(&pxEngine->Window);
        PXWindowCreate(&pxEngine->Window, 0, 0, -1, -1, PXNull, PXTrue);
        //PXWindowUpdate(&pxEngine->Window);
    }

    // Create graphic instance
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Creating graphical instance..."
        );

        PXGraphicInitializeInfo pxGraphicInitializeInfo;
        pxGraphicInitializeInfo.WindowReference = &pxEngine->Window;
        pxGraphicInitializeInfo.Width = -1;
        pxGraphicInitializeInfo.Height = -1;
        pxGraphicInitializeInfo.DirectXVersion = PXDirectXVersionNewest;
        pxGraphicInitializeInfo.DirectXDriverType = PXDirectXDriverTypeHardwareDevice;
        pxGraphicInitializeInfo.GraphicSystem = PXGraphicSystemOpenGL;

        PXGraphicInstantiate(&pxEngine->Graphic, &pxGraphicInitializeInfo);

        pxEngine->Graphic.SwapIntervalSet(pxEngine->Graphic.EventOwner, 1);
    }



  // PXControllerAttachToWindow(&pxBitFireEngine->Controller, pxBitFireEngine->WindowMain.ID);
  // PXCameraAspectRatioChange(pxBitFireEngine->CameraCurrent, pxBitFireEngine->WindowMain.Width, pxBitFireEngine->WindowMain.Height);

    pxEngine->CounterTimeLast = 0;
    pxEngine->CounterTimeGPU = 0;
    pxEngine->CounterTimeCPU = 0;
    pxEngine->FramesPerSecound = 0;
    pxEngine->FrameTime = 0;
    pxEngine->IsRunning = PXTrue;

    {
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Engine is up and running. Invoking callback for extended load."
        );

        PXFunctionInvoke(pxEngine->OnStartUp, pxEngine->Owner, pxEngine);
    }

    PXSize amount = 0;
    PXActionResult res = PXProcessHandleCountGet(PXNull, &amount);

    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Currently <%i> Handles open",
        amount
    );

    PXActionResult ww = PXProcessHandleListAll(PXNull);

    printf("");
}

void PXAPI PXEngineStop(PXEngine* const pxEngine)
{
    PXFunctionInvoke(pxEngine->OnShutDown, pxEngine->Owner, pxEngine);

    PXGraphicRelease(&pxEngine->Graphic);
    PXWindowDestruct(&pxEngine->Window);
}