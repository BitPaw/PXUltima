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
        "Signal",
        "CPU tryed to exectue illegal instruction!"
    );
}

void PXCDECL PXEngineOnMemoryViolation(const int signalID)
{
    PXLogPrint
    (
        PXLoggingFailure,
        "PX",
        "Signal",
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

    // User input
    {
        PXWindow* pxWindow = &pxEngine->Window;
        PXKeyBoard* keyboard = &pxWindow->KeyBoardCurrentInput;
        PXMouse* mouse = &pxWindow->MouseCurrentInput;

        pxEngine->CounterTimeUser = PXTimeCounterStampGet();

        PXPlayerMoveInfo pxPlayerMoveInfo;    
        PXClear(PXPlayerMoveInfo, &pxPlayerMoveInfo);

        if (keyboard->Commands & KeyBoardIDShiftLeft)   { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk,  0, -1,  0); }
        if (keyboard->Letters & KeyBoardIDLetterW)      { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk,  0,  0,  1); }
        if (keyboard->Letters & KeyBoardIDLetterA)      { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, -1,  0,  0); }
        if (keyboard->Letters & KeyBoardIDLetterS)      { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk,  0,  0, -1); }
        if (keyboard->Letters & KeyBoardIDLetterD)      { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk,  1,  0,  0); }
        if (keyboard->Letters & KeyBoardIDSpace)        { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk,  0,  1,  0); }

        pxPlayerMoveInfo.MovementView.X -= mouse->Delta[0];
        pxPlayerMoveInfo.MovementView.Y += mouse->Delta[1];
        pxPlayerMoveInfo.ActionCommit = PXTrue; // Always start with a commit, can be canceled
        pxPlayerMoveInfo.IsWindowInFocus = 1; // PXWindowInteractable(pxWindow);

        PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, &pxPlayerMoveInfo);

        PXMouseInputReset(mouse);

        if (pxPlayerMoveInfo.ActionCommit && pxEngine->CameraCurrent)
        {
            PXCameraMove(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementWalk);
            PXCameraRotate(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementView);
            PXCameraUpdate(pxEngine->CameraCurrent, pxEngine->CounterTimeDelta);

            //printf("[#][OnMouseMove] X:%5.2f Y:%5.2f\n", pxPlayerMoveInfo.MovementView.X, pxPlayerMoveInfo.MovementView.Y);
        }

        pxEngine->CounterTimeUser = PXTimeCounterStampGet() - pxEngine->CounterTimeUser;


        // Extended windows resize check
        if(pxWindow->HasSizeChanged)
        {
            PXViewPort pxViewPort;
            pxViewPort.X = 0;
            pxViewPort.Y = 0;
            pxViewPort.Width = pxWindow->Width;
            pxViewPort.Height = pxWindow->Height;
            pxViewPort.ClippingMinimum = 0;
            pxViewPort.ClippingMaximum = 1;

            pxWindow->HasSizeChanged = PXFalse;

            PXFunctionInvoke(pxEngine->Graphic.ViewPortSet, pxEngine->Graphic.EventOwner, &pxViewPort);

            if (pxEngine->CameraCurrent)
            {
                PXCameraAspectRatioChange(pxEngine->CameraCurrent, pxWindow->Width, pxWindow->Height);
            }
        }
    }

    // Network
    {
        pxEngine->CounterTimeNetwork = PXTimeCounterStampGet();
        PXFunctionInvoke(pxEngine->OnNetworkUpdate, pxEngine->Owner, pxEngine);
        pxEngine->CounterTimeNetwork = PXTimeCounterStampGet() - pxEngine->CounterTimeNetwork;
    }

    // Gameupdate
    {
        pxEngine->CounterTimeCPU = PXTimeCounterStampGet();
        PXFunctionInvoke(pxEngine->OnGameUpdate, pxEngine->Owner, pxEngine);
        pxEngine->CounterTimeCPU = PXTimeCounterStampGet() - pxEngine->CounterTimeCPU;
    }

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
    PXCameraConstruct(&pxEngine->CameraDefault);
    PXCameraViewChangeToPerspective(&pxEngine->CameraDefault, 80, PXCameraAspectRatio(&pxEngine->CameraDefault), 0.05, 10000);

    pxEngine->CameraCurrent = &pxEngine->CameraDefault;


    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Init",
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
            "Init",
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
            "Init",
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
            "Init",
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


    pxEngine->Graphic.Select(pxEngine->Graphic.EventOwner);

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
            "Init",
            "Engine is up and running. Invoking callback for extended load."
        );

        PXFunctionInvoke(pxEngine->OnStartUp, pxEngine->Owner, pxEngine);
    }

#if 0
    PXSize amount = 0;
    PXActionResult res = PXProcessHandleCountGet(PXNull, &amount);

    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Currently <%i> Handles open",
        amount
    );

    //PXActionResult ww = PXProcessHandleListAll(PXNull);
   // printf("");
#endif
}

void PXAPI PXEngineStop(PXEngine* const pxEngine)
{
    PXFunctionInvoke(pxEngine->OnShutDown, pxEngine->Owner, pxEngine);

    PXGraphicRelease(&pxEngine->Graphic);
    PXWindowDestruct(&pxEngine->Window);
}

PXActionResult PXAPI PXSkyBoxCreate(PXEngine* const pxEngine, PXSkyBoxCreateEventData* const pxSkyBoxCreateEventData)
{
    return PXActionInvalid;
}

PXActionResult PXAPI PXSpriteCreate(PXEngine* const pxEngine, PXSpriteCreateEventData* const pxSpriteCreateEventData)
{
    PXLogPrint
    (
        PXLoggingAllocation,
        "PX",
        "Sprite-Create",
        "Use <%s>",
        pxSpriteCreateEventData->TextureName
    );

    PXSprite* pxSprite = pxSpriteCreateEventData->SpriteReference;

    // Clear sprite //     PXGraphicSpriteConstruct(&pxEngine->Graphic, pxSprite);
    {
        PXClear(PXSprite, pxSprite);
        PXModelConstruct(&pxSprite->Model);

        //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
        //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

        PXVector2FSetXY(&pxSprite->TextureScaleOffset, 1, 1);

        //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
    }

    PXGraphicSpriteTextureLoadA(&pxEngine->Graphic, pxSprite, pxSpriteCreateEventData->TextureName);

    pxSprite->Model.ShaderProgramReference = pxSpriteCreateEventData->ShaderProgramCurrent;
    pxSprite->Model.IgnoreViewRotation = 0;
    pxSprite->Model.IgnoreViewPosition = 0;
    //pxSprite->Model.
    pxSprite->Model.RenderBothSides = PXTrue;


    pxSprite->Model.MaterialContaierList = PXNew(PXMaterialContainer);
    pxSprite->Model.MaterialContaierListSize = 1u;

    pxSprite->Model.MaterialContaierList->MaterialList = PXNew(PXMaterial);
    pxSprite->Model.MaterialContaierList->MaterialListSize = 1u;


    PXMaterial* materiial = &pxSprite->Model.MaterialContaierList->MaterialList[0];

    PXClear(PXMaterial, materiial);

    materiial->DiffuseTexture = pxSprite->Texture;

    pxSprite->Model.IndexBuffer.SegmentListSize = 1;
    pxSprite->Model.IndexBuffer.SegmentPrime.Material = materiial;


    PXGraphicSpriteRegister(&pxEngine->Graphic, pxSprite);
}