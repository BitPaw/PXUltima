#include "PXEngine.h"

#include <Math/PXMath.h>

void _cdecl PXEngineOnIllegalInstruction(const int signalID)
{
	printf("[PX][FATAL] CPU tryed to exectue illegal instruction!\n");
}

void _cdecl PXEngineOnMemoryViolation(const int signalID)
{
	printf("[PX][FATAL] CPU accessed memory illegally!\n");
}

#define PXConsoleClear() printf("\033[H\033[J")
#define PXConsoleGoToXY(x,y) printf("\033[%d;%dH", (y), (x))

void PXAPI PXEngineUpdate(PXEngine* const pxEngine)
{
    const PXInt64U timeNow = PXTimeCounterStampGet();

    pxEngine->CounterTimeDelta = timeNow - pxEngine->CounterTimeLast;
    pxEngine->CounterTimeLast = timeNow;
    pxEngine->TimeFrequency = PXTimeCounterFrequencyGet();
    pxEngine->FramesPerSecound = PXMathFloor(pxEngine->TimeFrequency / (float)pxEngine->CounterTimeDelta);
    pxEngine->FrameTime = PXMathCeilingF((1000000 * pxEngine->CounterTimeDelta) / (float)pxEngine->TimeFrequency);

    pxEngine->CounterTimeWindow = PXTimeCounterStampGet();
    PXWindowUpdate(&pxEngine->Window);
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
#endif
}

PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine)
{
    return pxEngine->IsRunning;
}

void PXAPI PXEngineStart(PXEngine* const pxEngine)
{
	PXSignalCallBackRegister(PXSignalTokenIllegalInstruction, PXEngineOnIllegalInstruction);
	PXSignalCallBackRegister(PXSignalTokenMemoryViolation, PXEngineOnMemoryViolation);

    // Load all mods now, not fully tho, they may need very early checks before anything happens
    PXText pxText;
    PXTextMakeFixedA(&pxText, "Mod\\");

    PXModLoaderScan(&pxEngine->ModLoader, &pxText);

	// Create window
    {
        PXWindowConstruct(&pxEngine->Window);
        PXWindowCreate(&pxEngine->Window, 0, 0, -1, -1, PXNull, PXFalse);
        PXWindowUpdate(&pxEngine->Window);
    }

    // Create graphic instance
    {
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

    PXFunctionInvoke(pxEngine->OnStartUp, pxEngine->Owner, pxEngine);
}

void PXAPI PXEngineStop(PXEngine* const pxEngine)
{
    PXFunctionInvoke(pxEngine->OnShutDown, pxEngine->Owner, pxEngine);

    PXGraphicRelease(&pxEngine->Graphic);
    PXWindowDestruct(&pxEngine->Window);
}