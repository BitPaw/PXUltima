#include "PXEngine.h"

#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>
#include <OS/Async/PXProcess.h>
#include <Engine/Dialog/PXDialogBox.h>
#include <Media/ADLER/PXAdler32.h>

void PXCDECL PXEngineOnIllegalInstruction(const int signalID)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        "PX",
        "Signal",
        "CPU tryed to exectue illegal instruction!"
    );
#endif
}

void PXCDECL PXEngineOnMemoryViolation(const int signalID)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        "PX",
        "Signal",
        "CPU accessed memory illegally!"
    );
#endif
}

PXInt32U PXAPI PXEngineGenerateUniqeID(PXEngine* const pxEngine)
{
    return ++pxEngine->UniqeIDGeneratorCounter;
   
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
        if (keyboard->Letters & KeyBoardIDLetterF && !pxEngine->InteractionLock)
        {
            pxEngine->InteractionLock = PXTrue;
            PXFunctionInvoke(pxEngine->OnInteract, pxEngine->Owner, pxEngine);
        }
        if (!(keyboard->Letters & KeyBoardIDLetterF))
        {
            pxEngine->InteractionLock = PXFalse;
        }

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

    // Timer
    {
        PXDictionary* const timerList = &pxEngine->TimerLookUp;

        for (PXSize timerIndex = 0; timerIndex < timerList->EntryAmountCurrent; ++timerIndex)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXEngineTimer* pxEngineTimer = PXNull;

            PXDictionaryIndex(timerList, timerIndex, &pxDictionaryEntry);

            pxEngineTimer = *(PXEngineTimer**)pxDictionaryEntry.Value;

            if (!pxEngineTimer->Enabled)
            {
                continue;
            }

            // Check timing, is it time to call yet?
            const PXInt32U timeStamp = PXTimeCounterStampGet();
            const PXInt32U timeerDeltaTime = timeStamp - pxEngineTimer->TimeStampStart;
            const PXBool isTimeDelayToStrong = ((PXInt32S)pxEngineTimer->TimeDeltaTarget + pxEngineTimer->TimeDelayShift) < 0;
            const PXBool isLongEnough = 
                isTimeDelayToStrong ||
                (timeerDeltaTime > (pxEngineTimer->TimeDeltaTarget + pxEngineTimer->TimeDelayShift));

            if (isLongEnough)
            {
                pxEngineTimer->TimeStampStart = timeStamp;

#if 1

                PXText pxText;
                PXTextConstructBufferA(&pxText, 64);

                PXTextFormatTime(&pxText, timeerDeltaTime);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Timer",
                    "Trigger, Waited for %10s",
                    pxText.TextA
                );
#endif

#endif

                if (pxEngineTimer->CallBack)
                {
                    PXEngineTimerEventInfo pxEngineTimerEventInfo;
                    PXClear(PXEngineTimerEventInfo, &pxEngineTimerEventInfo);
                    pxEngineTimerEventInfo.TimerReference = pxEngineTimer;


                    pxEngineTimer->CallBack(pxEngine, &pxEngineTimerEventInfo, pxEngineTimer->Owner);
                }
            }
        }
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

PXActionResult PXAPI PXEngineResourceAction(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfo)
{
    if (!(pxEngine && pxEngineResourceActionInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    switch (pxEngineResourceActionInfo->Type)
    {     
        case PXEngineResourceActionTypeCreate:
        {
            return PXEngineResourceCreate(pxEngine, &pxEngineResourceActionInfo->Create);
        }
        case PXEngineResourceActionTypeDestroy:
        {
            return PXActionRefusedNotImplemented;
        }
        case PXEngineResourceActionTypeUpdate:
        {
            return PXActionRefusedNotImplemented;
        }
        case PXEngineResourceActionTypeFetch:
        {
            return PXActionRefusedNotImplemented;
        }
        case PXEngineResourceActionTypeSelect:
        {
            return PXActionRefusedNotImplemented;
        }
        case PXEngineResourceActionTypeRender:
        {
            return PXEngineResourceRender(pxEngine, &pxEngineResourceActionInfo->Render);
        }
        case PXEngineResourceActionTypePlay:
        {
            return PXActionRefusedNotImplemented;
        }
        case PXEngineResourceActionTypeStateChange:
        {
            PXEngineResourceStateChangeInfo* const pxEngineResourceStateChangeInfo = &pxEngineResourceActionInfo->ChangeInfo;

            if (!pxEngineResourceStateChangeInfo->Object)
            {
                return PXActionRefusedArgumentNull;
            }

            switch (pxEngineResourceStateChangeInfo->Type)
            {
                case PXEngineCreateTypeModel:
                {
                    PXModel* const pxModel = (PXModel*)pxEngineResourceStateChangeInfo->Object;

                    pxModel->Enabled = pxEngineResourceStateChangeInfo->Enable;

                    break;
                }
                case PXEngineCreateTypeSkybox:
                {

                    break;
                }
                case PXEngineCreateTypeSprite:
                {
                    PXSprite* const pxSprite = (PXSprite*)pxEngineResourceStateChangeInfo->Object;

                    pxSprite->Enabled = pxEngineResourceStateChangeInfo->Enable;

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "PX",
                        "Sprite",
                        "Enable:%s",
                        pxEngineResourceStateChangeInfo->Enable ? "Yes" : "No"
                    );
#endif

                    break;
                }
                case PXEngineCreateTypeText:
                {
                    PXEngineText* const pxEngineText = (PXEngineText*)pxEngineResourceStateChangeInfo->Object;

                    pxEngineText->Enabled = pxEngineResourceStateChangeInfo->Enable;

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "PX",
                        "Text",
                        "Enable:%s",
                        pxEngineResourceStateChangeInfo->Enable ? "Yes" : "No"
                    );
#endif

                    break;
                }
                case PXEngineCreateTypeTimer:
                {
                    PXEngineTimer* const pxEngineTimer = (PXEngineTimer*)pxEngineResourceStateChangeInfo->Object;

                    pxEngineTimer->Enabled = pxEngineResourceStateChangeInfo->Enable;
                    pxEngineTimer->TimeStampStart = PXTimeCounterStampGet();

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "PX",
                        "Timer",
                        "Enable:%s",
                        pxEngineResourceStateChangeInfo->Enable ? "Yes" : "No"
                    );
#endif

                    break;
                }
                case PXEngineCreateTypeUIElement:
                {

                    break;
                }
                default:
                    break;
            }


            return PXActionRefusedNotImplemented;
        }
        case PXEngineResourceActionTypeCustom:
        {
            return PXActionRefusedNotImplemented;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXEngineResourceActionBatch(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfoList, const PXSize amount)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Resource",
        "Action Batch <%i>",
        amount
    );
#endif

    for (PXSize i = 0; i < amount; ++i)
    {
        PXEngineResourceActionInfo* const pxEngineResourceAction = &pxEngineResourceActionInfoList[i];

        PXEngineResourceAction(pxEngine, pxEngineResourceAction);
    }

    return PXActionSuccessful;
}

PXInt32U PXAPI PXEngineGenerateRandom(PXEngine* const pxEngine, const PXInt32U limiter)
{
    return PXMathRandomeNumber(&pxEngine->RandomGeneratorSeed) % limiter;
}

PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine)
{
    return pxEngine->IsRunning;
}

void PXAPI PXEngineStart(PXEngine* const pxEngine)
{
    PXCameraConstruct(&pxEngine->CameraDefault);
    PXCameraViewChangeToPerspective(&pxEngine->CameraDefault, 80, PXCameraAspectRatio(&pxEngine->CameraDefault), 0.00, 100000000);

    pxEngine->CameraCurrent = &pxEngine->CameraDefault;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Init",
        "Starting..."
    );
#endif

	PXSignalCallBackRegister(PXSignalTokenIllegalInstruction, PXEngineOnIllegalInstruction);
	PXSignalCallBackRegister(PXSignalTokenMemoryViolation, PXEngineOnMemoryViolation);

    {
        PXText pxText;
        PXTextMakeFixedA(&pxText, "PXEngineMain");

        PXThreadNameSet(PXNull, &pxText);
    }



    //-----------------------------------------------------
    // Create containers
    //-----------------------------------------------------
    PXDictionaryConstruct(&pxEngine->SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->TextLookUp, sizeof(PXInt32U), sizeof(PXEngineText), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->TimerLookUp, sizeof(PXInt32U), sizeof(PXEngineTimer), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->SoundLookUp, sizeof(PXInt32U), sizeof(PXEngineSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->HitBoxLookUp, sizeof(PXInt32U), sizeof(PXHitBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture2D), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->ShaderProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->ImageLookUp, sizeof(PXInt32U), sizeof(PXImage), PXDictionaryValueLocalityExternalReference);
    //-----------------------------------------------------


    PXMathRandomeSeed(&pxEngine->RandomGeneratorSeed);


    // Load all mods now, not fully tho, they may need very early checks before anything happens
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Init",
            "Loading Mods..."
        );
#endif
      
        PXText pxText;
        PXTextMakeFixedA(&pxText, "Mod\\");

        PXModLoaderScan(&pxEngine->ModLoader, &pxText);
    }




    //-----------------------------------------------------
	// Create window
    //-----------------------------------------------------
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Init",
            "Creating Window..."
        );
#endif

        PXWindowConstruct(&pxEngine->Window);
        PXWindowCreate(&pxEngine->Window, 0, 0, -1, -1, PXNull, PXTrue);
        //PXWindowUpdate(&pxEngine->Window);
    }
    //-----------------------------------------------------



    //-----------------------------------------------------
    // Create graphic instance
    //-----------------------------------------------------
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Init",
            "Creating graphical instance..."
        );
#endif

        PXGraphicInitializeInfo pxGraphicInitializeInfo;
        pxGraphicInitializeInfo.WindowReference = &pxEngine->Window;
        pxGraphicInitializeInfo.Width = -1;
        pxGraphicInitializeInfo.Height = -1;
        pxGraphicInitializeInfo.DirectXVersion = PXDirectXVersionNewest;
        pxGraphicInitializeInfo.DirectXDriverType = PXDirectXDriverTypeHardwareDevice;
        pxGraphicInitializeInfo.GraphicSystem = PXGraphicSystemOpenGL;

        PXGraphicInstantiate(&pxEngine->Graphic, &pxGraphicInitializeInfo);

        pxEngine->Graphic.SwapIntervalSet(pxEngine->Graphic.EventOwner, 1);
        pxEngine->Graphic.Select(pxEngine->Graphic.EventOwner);
    }
    //-----------------------------------------------------
  


    //-----------------------------------------------------
    // Create Audio Session
    //-----------------------------------------------------
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Init",
            "Creating audio instance..."
        );
#endif

        const PXActionResult audioInitResult = PXAudioInitialize(&pxEngine->Audio, PXAudioSystemWindowsDirectSound);

        if (PXActionSuccessful == audioInitResult)
        {
            PXInt32U audioDeviceAmountInput = 0;
            PXInt32U audioDeviceAmountOutput = 0;

            pxEngine->Audio.DeviceAmount(&pxEngine->Audio, PXAudioDeviceTypeInput, &audioDeviceAmountInput);
            pxEngine->Audio.DeviceAmount(&pxEngine->Audio, PXAudioDeviceTypeOutput, &audioDeviceAmountOutput);

            for (size_t i = 0; i < audioDeviceAmountInput; i++)
            {
                PXAudioDevice pxAudioDevice;

                pxEngine->Audio.DeviceFetch(&pxEngine->Audio, PXAudioDeviceTypeInput, i, &pxAudioDevice);
            }

            for (size_t i = 0; i < audioDeviceAmountOutput; i++)
            {
                PXAudioDevice pxAudioDevice;

                pxEngine->Audio.DeviceFetch(&pxEngine->Audio, PXAudioDeviceTypeOutput, i, &pxAudioDevice);
            }

            pxEngine->Audio.DeviceFetch(&pxEngine->Audio, PXAudioDeviceTypeOutput, 0, &pxEngine->AudioStandardOutDevice);

            pxEngine->Audio.DeviceOpen(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice, PXAudioDeviceTypeOutput, 0);
        }
    }
    //-----------------------------------------------------



  // PXControllerAttachToWindow(&pxBitFireEngine->Controller, pxBitFireEngine->WindowMain.ID);
  // PXCameraAspectRatioChange(pxBitFireEngine->CameraCurrent, pxBitFireEngine->WindowMain.Width, pxBitFireEngine->WindowMain.Height);

    pxEngine->CounterTimeLast = 0;
    pxEngine->CounterTimeGPU = 0;
    pxEngine->CounterTimeCPU = 0;
    pxEngine->FramesPerSecound = 0;
    pxEngine->FrameTime = 0;
    pxEngine->IsRunning = PXTrue;

    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Init",
            "Engine is up and running. Invoking callback for extended load."
        );
#endif

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


/*


PXActionResult PXAPI PXGraphicLoadImage(PXGraphic* const pxGraphic, PXImage* const pxImage, const PXText* const pxImageFilePath)
{
    // Create checksum
    const PXInt32U checkSum = PXAdler32Create(1, pxImageFilePath->TextA, pxImageFilePath->SizeUsed);


    // Check if already loaded
    {
        PXImage* pxImageFoundEntry = PXNull;

        const PXBool foundEntry = PXDictionaryFindEntry(&pxGraphic->ResourceImageLookUp, &checkSum, &pxImageFoundEntry);

        if (foundEntry) // image is already loaded
        {
            PXImageCopyAsIs(pxImage, pxImageFoundEntry);

            PXLogPrint
            (
                PXLoggingWarning,
                "Graphic",
                "Texture",
                "Load skipped <%s> (Redundant)",
                pxImageFilePath->TextA
            );

            return PXActionSuccessful;
        }
    }

    // Load texture
    {
        const PXActionResult loadResult = PXResourceLoad(pxImage, pxImageFilePath);

        if (PXActionSuccessful != loadResult)
        {
            PXLogPrint
            (
                PXLoggingError,
                "Graphic",
                "Texture",
                "Texture load failed <%s>!",
                pxImageFilePath->TextA
            );
        }

        PXActionReturnOnError(loadResult);

        PXLogPrint
        (
            PXLoggingInfo,
            "Graphic",
            "Texture",
            "Texture load successful <%s>.",
            pxImageFilePath->TextA
        );

        PXDictionaryAdd(&pxGraphic->ResourceImageLookUp, &checkSum, pxImage);
    }

    return PXActionSuccessful;
}


*/

PXActionResult PXAPI PXEngineResourceCreate(PXEngine* const pxEngine, PXEngineResourceCreateInfo* const pxEngineResourceCreateInfo)
{
    if (!(pxEngine && pxEngineResourceCreateInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    switch (pxEngineResourceCreateInfo->CreateType)
    {
        case PXEngineCreateTypeCustom:
        {


            break;
        }
        case PXEngineCreateTypeImage:
        {
            //PXImageCreateData* const pxImageCreateData = &pxEngineResourceCreateInfo->Image;
            PXImage* pxImage = *(PXImage**)pxEngineResourceCreateInfo->ObjectReference;

            PXText pxText;
            PXTextConstructFromAdressA(&pxText, pxEngineResourceCreateInfo->FilePath, PXTextLengthUnkown, PXTextLengthUnkown);
            
            // Create checksum
            const PXInt32U checkSum = PXAdler32Create(1, pxText.TextA, pxText.SizeUsed);

            // Check if already loaded
            {
                PXImage* pxImageFoundEntry = PXNull;

                const PXBool foundEntry = PXDictionaryFindEntry(&pxEngine->ImageLookUp, &checkSum, (void**)&pxImageFoundEntry);

                if (foundEntry) // image is already loaded
                {
                    pxImage = pxImageFoundEntry;
                    *pxEngineResourceCreateInfo->ObjectReference = pxImage;

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingWarning,
                        "PX",
                        "Image-Load",
                        "skipped <%s> (Redundant)",
                        pxText.TextA
                    );
#endif

                    return PXActionSuccessful;
                }
            }

            if (!pxImage)
            {
                pxImage = PXNew(PXImage);
                *pxEngineResourceCreateInfo->ObjectReference = pxImage;
            }

            PXImageConstruct(pxImage);

            // Load texture
            {
                const PXActionResult loadResult = PXResourceLoad(pxImage, &pxText);


                if (PXActionSuccessful != loadResult)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        "PX",
                        "Image-Load",
                        "Load failed <%s>!",
                        pxText.TextA
                    );
#endif

                    return loadResult;
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Image-Load",
                    "Load successful <%s>.",
                    pxText.TextA
                );
#endif

                PXDictionaryAdd(&pxEngine->ImageLookUp, &checkSum, pxImage);
            }

            break;
        }
        case PXEngineCreateTypeTextureCube:
        {
            PXTextureCubeCreateData* const pxTextureCubeCreateData = &pxEngineResourceCreateInfo->TextureCube;
            PXTextureCube* pxTextureCube = *(PXTextureCube**)pxEngineResourceCreateInfo->ObjectReference;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "TextureCube",
                "Create"
            );
#endif

            if (!pxTextureCube)
            {
                pxTextureCube = PXNew(PXTextureCube);
                *pxEngineResourceCreateInfo->ObjectReference = pxTextureCube;
            }

            PXClear(PXTextureCube, pxTextureCube);
            pxTextureCube->Format = PXColorFormatRGBI8;

            PXEngineResourceActionInfo pxEngineResourceActionInfoList[6];
            PXClearList(PXEngineResourceActionInfo, &pxEngineResourceActionInfoList, 6);

            pxEngineResourceActionInfoList[0].Type = PXEngineResourceActionTypeCreate;
            pxEngineResourceActionInfoList[0].Create.CreateType = PXEngineCreateTypeImage;
            pxEngineResourceActionInfoList[0].Create.ObjectReference = (void**)&pxTextureCube->ImageA;
            pxEngineResourceActionInfoList[0].Create.FilePath = pxTextureCubeCreateData->FilePathA;
          
            pxEngineResourceActionInfoList[1].Type = PXEngineResourceActionTypeCreate;
            pxEngineResourceActionInfoList[1].Create.CreateType = PXEngineCreateTypeImage;
            pxEngineResourceActionInfoList[1].Create.ObjectReference = (void**)&pxTextureCube->ImageB;
            pxEngineResourceActionInfoList[1].Create.FilePath = pxTextureCubeCreateData->FilePathB;

            pxEngineResourceActionInfoList[2].Type = PXEngineResourceActionTypeCreate;
            pxEngineResourceActionInfoList[2].Create.CreateType = PXEngineCreateTypeImage;
            pxEngineResourceActionInfoList[2].Create.ObjectReference = (void**)&pxTextureCube->ImageC;
            pxEngineResourceActionInfoList[2].Create.FilePath = pxTextureCubeCreateData->FilePathC;

            pxEngineResourceActionInfoList[3].Type = PXEngineResourceActionTypeCreate;
            pxEngineResourceActionInfoList[3].Create.CreateType = PXEngineCreateTypeImage;
            pxEngineResourceActionInfoList[3].Create.ObjectReference = (void**)&pxTextureCube->ImageD;
            pxEngineResourceActionInfoList[3].Create.FilePath = pxTextureCubeCreateData->FilePathD;

            pxEngineResourceActionInfoList[4].Type = PXEngineResourceActionTypeCreate;
            pxEngineResourceActionInfoList[4].Create.CreateType = PXEngineCreateTypeImage;
            pxEngineResourceActionInfoList[4].Create.ObjectReference = (void**)&pxTextureCube->ImageE;
            pxEngineResourceActionInfoList[4].Create.FilePath = pxTextureCubeCreateData->FilePathE;

            pxEngineResourceActionInfoList[5].Type = PXEngineResourceActionTypeCreate;
            pxEngineResourceActionInfoList[5].Create.CreateType = PXEngineCreateTypeImage;
            pxEngineResourceActionInfoList[5].Create.ObjectReference = (void**)&pxTextureCube->ImageF;
            pxEngineResourceActionInfoList[5].Create.FilePath = pxTextureCubeCreateData->FilePathF;

            PXEngineResourceActionBatch(pxEngine, pxEngineResourceActionInfoList, 6);
            
            pxEngine->Graphic.TextureCubeRegister(pxEngine->Graphic.EventOwner, pxTextureCube);

            break;
        }
        case PXEngineCreateTypeModel:
        {
            PXModelCreateInfo* const pxModelCreateInfo = &pxEngineResourceCreateInfo->Model;
            PXModel* pxModel = *(PXModel**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxModel)
            {
                pxModel = PXNew(PXModel);
                *pxEngineResourceCreateInfo->ObjectReference = pxModel;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Model-Create",
                "load <%s>.",
                pxEngineResourceCreateInfo->FilePath
            );
#endif
            // Init
            PXModelConstruct(pxModel);

            // Register
            pxModel->ResourceID.PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->FontLookUp, &pxModel->ResourceID.PXID, pxModel);

            // Load
            {
                PXResourceLoadA(pxModel, pxEngineResourceCreateInfo->FilePath);
            }

            // Setup    
            PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);
         
            pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

            // Register
            pxEngine->Graphic.ModelRegister(pxEngine->Graphic.EventOwner, pxModel);

            break;
        }
        case PXEngineCreateTypeTexture2D:
        {
            //PXEngineTexture2DCreateData* const pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;
            PXTexture2D* pxTexture2D = *(PXTexture2D**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxTexture2D)
            {
                pxTexture2D = PXNew(PXTexture2D);
                *pxEngineResourceCreateInfo->ObjectReference = pxTexture2D;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Texture-Create",
                "load <%s>.",
                pxEngineResourceCreateInfo->FilePath
            );
#endif

            PXTextureConstruct(pxTexture2D);

            {
                PXEngineResourceActionInfo pxEngineResourceActionInfoList;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfoList);

                pxEngineResourceActionInfoList.Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfoList.Create.CreateType = PXEngineCreateTypeImage;
                pxEngineResourceActionInfoList.Create.ObjectReference = (void**)&pxTexture2D->Image;
                pxEngineResourceActionInfoList.Create.FilePath = pxEngineResourceCreateInfo->FilePath;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfoList);
            }       

            pxTexture2D->ResourceID.PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->FontLookUp, &pxTexture2D->ResourceID.PXID, pxTexture2D);

            pxEngine->Graphic.Texture2DRegister(pxEngine->Graphic.EventOwner, pxTexture2D);


            break;
        }
        case PXEngineCreateTypeFont:
        {
            PXEngineFontCreateData* const pxEngineFontCreateData = &pxEngineResourceCreateInfo->Font;
            PXFont* pxFont = *(PXFont**)pxEngineResourceCreateInfo->ObjectReference;

            // create if not exist
            if(!pxFont)
            {
                pxFont = PXNew(PXFont);
                *pxEngineResourceCreateInfo->ObjectReference = pxFont;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Font-Create",
                "load <%s>.",
                pxEngineResourceCreateInfo->FilePath
            );
#endif

            PXClear(PXFont, pxFont);

            // Load font
            {
                PXText pxText;
                PXTextConstructFromAdressA(&pxText, pxEngineResourceCreateInfo->FilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                const PXActionResult loadResult = PXResourceLoad(pxFont, &pxText);

                PXActionReturnOnError(loadResult);
            }

            pxFont->PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->FontLookUp, &pxFont->PXID, pxFont);

            // Register as normal
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Font",
                    "Registering..."
                );
#endif

                // Load Textures
                {
                    PXFontPage* const pxFontPage = &pxFont->MainPage;

                    PXEngineResourceActionInfo pxEngineResourceActionInfoList;
                    PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfoList);

                    pxEngineResourceActionInfoList.Type = PXEngineResourceActionTypeCreate;
                    pxEngineResourceActionInfoList.Create.CreateType = PXEngineCreateTypeTexture2D;
                    pxEngineResourceActionInfoList.Create.ObjectReference = (void**)&pxFontPage->Texture;
                    pxEngineResourceActionInfoList.Create.FilePath = pxFontPage->TextureFilePath;

                    PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfoList);
                }

                for (PXSize i = 0; i < pxFont->AdditionalPageListSize; ++i)
                {
                    PXFontPage* const pxFontPage = &pxFont->AdditionalPageList[i];

                    pxEngine->Graphic.Texture2DRegister(pxEngine->Graphic.EventOwner, pxFontPage->Texture);
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Font",
                    "Registerd",
                    PXNull
                );
#endif
            }

            break;
        }
        case PXEngineCreateTypeSkybox:
        {
            PXSkyBoxCreateEventData* const pxSkyBoxCreateEventData = &pxEngineResourceCreateInfo->SkyBox;
            PXSkyBox* pxSkyBox = *(PXSkyBox**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxSkyBox)
            {
                pxSkyBox = PXNew(PXSkyBox);
                *pxEngineResourceCreateInfo->ObjectReference = pxSkyBox;
            }

            PXClear(PXSkyBox, pxSkyBox);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "SkyBox",
                "Create"
            );
#endif

            // Load Textures
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfoList[2];
                PXClearList(PXEngineResourceActionInfo, &pxEngineResourceActionInfoList, 2);

                // Skybox CubeTexture
                pxEngineResourceActionInfoList[0].Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfoList[0].Create.CreateType = PXEngineCreateTypeTextureCube;
                pxEngineResourceActionInfoList[0].Create.SpawnEnabled = PXTrue;
                pxEngineResourceActionInfoList[0].Create.ObjectReference = (void**)&pxSkyBox->TextureCube; // Request object
                pxEngineResourceActionInfoList[0].Create.TextureCube.FilePathA = pxSkyBoxCreateEventData->SkyBoxTextureA;
                pxEngineResourceActionInfoList[0].Create.TextureCube.FilePathB = pxSkyBoxCreateEventData->SkyBoxTextureB;
                pxEngineResourceActionInfoList[0].Create.TextureCube.FilePathC = pxSkyBoxCreateEventData->SkyBoxTextureC;
                pxEngineResourceActionInfoList[0].Create.TextureCube.FilePathD = pxSkyBoxCreateEventData->SkyBoxTextureD;
                pxEngineResourceActionInfoList[0].Create.TextureCube.FilePathE = pxSkyBoxCreateEventData->SkyBoxTextureE;
                pxEngineResourceActionInfoList[0].Create.TextureCube.FilePathF = pxSkyBoxCreateEventData->SkyBoxTextureF;

                // Skybox Shader
                pxEngineResourceActionInfoList[1].Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfoList[1].Create.CreateType = PXEngineCreateTypeShaderProgram;
                pxEngineResourceActionInfoList[1].Create.ObjectReference = (void**)&pxSkyBox->ShaderProgramReference; // Request object
                pxEngineResourceActionInfoList[1].Create.ShaderProgram.VertexShaderFilePath = pxSkyBoxCreateEventData->SkyBoxShaderVertex;
                pxEngineResourceActionInfoList[1].Create.ShaderProgram.PixelShaderFilePath = pxSkyBoxCreateEventData->SkyBoxShaderPixel;

                // Skybox model
                //pxEngineResourceActionInfoList[1].Type = PXEngineResourceActionTypeCreate;
                //pxEngineResourceActionInfoList[1].Create.CreateType = PXEngineCreateTypeModel;
                //pxEngineResourceActionInfoList[1].Create.SpawnEnabled = PXTrue;
                //pxEngineResourceActionInfoList[1].Create.Model.ModelReference = &pxSkyBox->Model; // Request object
                //pxEngineResourceActionInfoList[1].Create.Model.ModelFilePath = pxSkyBoxCreateEventData->;       

                PXEngineResourceActionBatch(pxEngine, pxEngineResourceActionInfoList, 2);              
            }

            // Model create
            {
                PXModel* const pxModel = PXNew(PXModel);
                pxSkyBox->Model = pxModel;

                const float vertexData[] =
                {
                    -1, -1, -1,
                     1, -1, -1,
                    -1,  1, -1,
                     1,  1, -1,
                    -1, -1,  1,
                     1, -1,  1,
                    -1,  1,  1,
                     1,  1,  1,
                };

                const PXInt8U indexList[] =
                {
                    #if QuadSkybox
                    0,1,2,3, // Left OK
                    4,5,6,7, // Right  OK
                    4,5,1,0, // Top OK
                    2,3,7,6, // Bot OK
                    0,2,6,4, // Front OK
                    5,7,3,1 // Back OK
                    #else

                    // Left
                    0,1,2,
                    2,3,1,

                    // Right
                    4,5,6,
                    6,7,5,

                    // Back
                    0,2,4,
                    4,6,2,

                    // Font
                    1,3,5,
                    5,7,3,

                    // Bottom
                    0,1,4,
                    4,5,1,

                    // Top
                    2,3,7,
                    7,6,2

                    #endif
                };


                PXClear(PXModel, pxModel);
                pxModel->VertexBuffer.VertexData = (void*)vertexData;
                pxModel->VertexBuffer.VertexDataSize = sizeof(vertexData);
                pxModel->VertexBuffer.VertexDataRowSize = 3;
                pxModel->VertexBuffer.Format = PXVertexBufferFormatXYZ; // PXVertexBufferFormatXYZC  PXVertexBufferFormatXYZHWC

                pxModel->IndexBuffer.DataType = PXDataTypeInt08U;
                pxModel->IndexBuffer.IndexData = (void*)indexList;
                pxModel->IndexBuffer.IndexDataSize = sizeof(indexList);
                pxModel->IndexBuffer.IndexDataAmount = sizeof(indexList) / sizeof(PXInt8U);
                pxModel->IndexBuffer.DrawModeID = 0 |// PXDrawModeIDPoint | PXDrawModeIDLineLoop |
#if QuadSkybox
                    PXDrawModeIDSquare
#else
                    PXDrawModeIDTriangle
#endif
                    ;

                pxEngine->Graphic.ModelRegister(pxEngine->Graphic.EventOwner, pxModel);
            }


            break;
        }
        case PXEngineCreateTypeSprite:
        {
            PXSpriteCreateEventData* const pxSpriteCreateEventData = &pxEngineResourceCreateInfo->Sprite;
            PXSprite* pxSprite = *(PXSprite**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxSprite)
            {
                pxSprite = PXNew(PXSprite);
                *pxEngineResourceCreateInfo->ObjectReference = pxSprite;
            }

            PXClear(PXSprite, pxSprite);

            pxSprite->PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->SpritelLookUp, &pxSprite->PXID, pxSprite);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingAllocation,
                "PX",
                "Sprite-Create",
                "Use <%s>",
                pxEngineResourceCreateInfo->FilePath
            );   
#endif

            // Clear sprite //     PXGraphicSpriteConstruct(&pxEngine->Graphic, pxSprite);
            {         
                PXModelConstruct(&pxSprite->Model);

                //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
                //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

                PXVector2FSetXY(&pxSprite->TextureScalePositionOffset, 1, 1);
                PXVector2FSetXY(&pxSprite->TextureScalePointOffset, 1, 1);

                //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
            }

            PXTextCopyA(pxEngineResourceCreateInfo->FilePath, 20, pxSprite->Name, 50);

            pxSprite->TextureScalePositionOffset.X = pxSpriteCreateEventData->TextureScalingPoints[0].X;
            pxSprite->TextureScalePositionOffset.Y = pxSpriteCreateEventData->TextureScalingPoints[0].Y;
            pxSprite->TextureScalePointOffset.X = pxSpriteCreateEventData->TextureScalingPoints[1].X;
            pxSprite->TextureScalePointOffset.Y = pxSpriteCreateEventData->TextureScalingPoints[1].Y;


            pxSprite->Model.ShaderProgramReference = pxSpriteCreateEventData->ShaderProgramCurrent;
            pxSprite->Model.IgnoreViewRotation = pxSpriteCreateEventData->ViewRotationIgnore;
            pxSprite->Model.IgnoreViewPosition = pxSpriteCreateEventData->ViewPositionIgnore;
            //pxSprite->Model.
            pxSprite->Model.RenderBothSides = PXTrue;


#if 0
            pxSprite->Model.MaterialContaierList = PXNew(PXMaterialContainer);
            pxSprite->Model.MaterialContaierListSize = 1u;

            pxSprite->Model.MaterialContaierList->MaterialList = PXNew(PXMaterial);
            pxSprite->Model.MaterialContaierList->MaterialListSize = 1u;

            PXMaterial* materiial = &pxSprite->Model.MaterialContaierList->MaterialList[0];

            PXClear(PXMaterial, materiial);

            materiial->DiffuseTexture = pxSprite->Texture;
#endif

            if (pxEngineResourceCreateInfo->FilePath)
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfoList;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfoList);

                pxEngineResourceActionInfoList.Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfoList.Create.CreateType = PXEngineCreateTypeTexture2D;
                pxEngineResourceActionInfoList.Create.ObjectReference = (void**)&pxSprite->Texture;
                pxEngineResourceActionInfoList.Create.FilePath = pxEngineResourceCreateInfo->FilePath;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfoList);

                PXEngineSpriteTextureSet(pxEngine, pxSprite, pxSprite->Texture);           
            }    

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Sprite",
                "Register ID:%i from %i",
                pxSprite->PXID,
                pxEngine->SpritelLookUp.EntryAmountCurrent
            );
#endif

            const PXBool hasScaling = pxSprite->TextureScalePositionOffset.X != 0 || pxSprite->TextureScalePositionOffset.Y != 0;

            if (hasScaling)
            {
                if (pxEngine->SpriteScaled.ResourceID.OpenGLID == 0)
                {
                    PXOpenGLSpriteRegister(&pxEngine->Graphic.OpenGLInstance, pxSprite);
                }
                else
                {
                    pxSprite->Model.StructureOverride = &pxEngine->SpriteScaled;
                }
            }
            else
            {
                if (pxEngine->SpriteUnScaled.ResourceID.OpenGLID == 0)
                {
                    PXOpenGLSpriteRegister(&pxEngine->Graphic.OpenGLInstance, pxSprite);
                }
                else
                {
                    pxSprite->Model.StructureOverride = &pxEngine->SpriteUnScaled;
                }
            }

            if (pxSpriteCreateEventData->Scaling.X == 0)
            {
                pxSpriteCreateEventData->Scaling.X = 1;
            }

            if (pxSpriteCreateEventData->Scaling.Y == 0)
            {
                pxSpriteCreateEventData->Scaling.Y = 1;
            }



            float aspectRationX = 1;

            if (pxSprite->Texture)
            {
                if (pxSprite->Texture->Image->Width && pxSprite->Texture->Image->Height)
                {
                    aspectRationX = (float)pxSprite->Texture->Image->Width / (float)pxSprite->Texture->Image->Height;
                }
            }

         

            PXMatrix4x4FScaleSetXY(&pxSprite->Model.ModelMatrix, aspectRationX, 1);

            PXMatrix4x4FScaleByXY
            (
                &pxSprite->Model.ModelMatrix,
                pxSpriteCreateEventData->Scaling.X,
                pxSpriteCreateEventData->Scaling.Y
            );


            PXMatrix4x4FPositionSet(&pxSprite->Model.ModelMatrix, &pxSpriteCreateEventData->Position);


            //---------------------------------------------
            // Trigger enable
            //---------------------------------------------
            if (pxEngineResourceCreateInfo->SpawnEnabled)
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
                pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
                pxEngineResourceActionInfo.ChangeInfo.Type = PXEngineCreateTypeSprite;
                pxEngineResourceActionInfo.ChangeInfo.Object = pxSprite;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
            }
            //---------------------------------------------


            break;
        }
        case PXEngineCreateTypeText:
        {
            //PXEngineTextCreateData* const pxEngineTextCreateData = &pxEngineResourceCreateInfo->Text;
            PXEngineText* pxEngineText = *(PXEngineText**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxEngineText)
            {
                pxEngineText = PXNew(PXEngineText);
                *pxEngineResourceCreateInfo->ObjectReference = pxEngineText;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Text",
                "Register",
                PXNull
            );
#endif

            pxEngineText->PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->TextLookUp, &pxEngineText->PXID, pxEngineText);

            //---------------------------------------------
            // Trigger enable
            //---------------------------------------------
            if (pxEngineResourceCreateInfo->SpawnEnabled)
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
                pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
                pxEngineResourceActionInfo.ChangeInfo.Type = PXEngineCreateTypeText;
                pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineText;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
            }
            //---------------------------------------------

            break;
        }
        case PXEngineCreateTypeTimer:
        {
            PXEngineTimer* pxEngineTimer = *(PXEngineTimer**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxEngineTimer)
            {
                pxEngineTimer = PXNew(PXEngineTimer);
                *pxEngineResourceCreateInfo->ObjectReference = pxEngineTimer;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Timer",
                "Register",
                PXNull
            );
#endif

            pxEngineTimer->PXID = PXEngineGenerateUniqeID(pxEngine);
            pxEngineTimer->TimeStampStart = PXTimeCounterStampGet();

            PXDictionaryAdd(&pxEngine->TimerLookUp, &pxEngineTimer->PXID, pxEngineTimer);

            //---------------------------------------------
            // Trigger enable
            //---------------------------------------------
            if (pxEngineResourceCreateInfo->SpawnEnabled)
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
                pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
                pxEngineResourceActionInfo.ChangeInfo.Type = PXEngineCreateTypeTimer;
                pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineTimer;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
            }
            //---------------------------------------------

            break;
        }
        case PXEngineCreateTypeSound:
        {
            PXSound* pxSound = *(PXSound**)pxEngineResourceCreateInfo->ObjectReference;

            PXText pxTextFilePath;
            PXTextConstructFromAdressA(&pxTextFilePath, pxEngineResourceCreateInfo->FilePath, PXTextLengthUnkown, PXTextLengthUnkown);

            if (!pxSound)
            {
                pxSound = PXNew(PXSound);
                *pxEngineResourceCreateInfo->ObjectReference = pxSound;
            }

            const PXActionResult loadResult = PXResourceLoad(pxSound, &pxTextFilePath);

            if (PXActionSuccessful != loadResult)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "PX",
                    "Sound-Load",
                    "failed <%s>!",
                    pxTextFilePath.TextA
                );
#endif
            }

            PXActionReturnOnError(loadResult);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Sound-Load",
                "successful <%s>.",
                pxTextFilePath.TextA
            );
#endif

            break;
        }
        case PXEngineCreateTypeEngineSound:
        {
            PXEngineSoundCreateInfo* const pxEngineSoundCreateInfo = &pxEngineResourceCreateInfo->Sound;
            PXEngineSound* pxEngineSound = *(PXEngineSound**)pxEngineResourceCreateInfo->ObjectReference;

            // Create if not exists
            if (!pxEngineSound)
            {
                pxEngineSound = PXNew(PXEngineSound);
                *pxEngineResourceCreateInfo->ObjectReference = pxEngineSound;
            }

            // Register
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Sound",
                    "Register",
                    PXNull
                );
#endif


                pxEngineSound->PXID = PXEngineGenerateUniqeID(pxEngine);

                PXDictionaryAdd(&pxEngine->SoundLookUp, &pxEngineSound->PXID, pxEngineSound);
            }

            // Load
            {           
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfo.Create.CreateType = PXEngineCreateTypeSound;
                pxEngineResourceActionInfo.Create.ObjectReference = &pxEngineSound->Sound;
                pxEngineResourceActionInfo.Create.FilePath = pxEngineResourceCreateInfo->FilePath;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);               
            }



            {
                const PXActionResult pxActionResult = pxEngine->Audio.DeviceLoad(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice, pxEngineSound->Sound);

#if PXLogEnable
                if (PXActionSuccessful == pxActionResult)
                {

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "PX",
                        "Sound-Upload",
                        "Success <%s>!",
                        pxEngineResourceCreateInfo->FilePath
                    );                  
                }
                else
                {
                    PXLogPrint
                    (
                        PXLoggingError,
                        "PX",
                        "Sound-Upload",
                        "Failed <%s>!",
                        pxEngineResourceCreateInfo->FilePath
                    );
                }
#endif

            }


            break;
        }
        case PXEngineCreateTypeUIElement:
        {
            PXUIElementCreateData* const pxUIElementCreateData = &pxEngineResourceCreateInfo->UIElement;
            PXUIElement* pxUIElement = *(PXUIElement**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxUIElement)
            {
                pxUIElement = PXNew(PXUIElement);
                *pxEngineResourceCreateInfo->ObjectReference = pxUIElement;
            }

            // Is Registerd
            {
                const PXBool isRegisterd = pxUIElement->ID != -1;

                if (!isRegisterd)
                {
                    return PXActionInvalidRedundandInteraction;
                }

                pxUIElement->ID = PXEngineGenerateUniqeID(pxEngine);
                PXDictionaryAdd(&pxEngine->UIElementLookUp, &pxUIElement->ID, pxUIElement);      
            }

            // General stuff
            {
                PXGraphicUIElementCreate(&pxEngine->Graphic, &pxUIElement, 1, pxUIElementCreateData->Paranet);
                PXGraphicUIElementTypeSet(&pxEngine->Graphic, pxUIElement, PXUIElementTypePanel);
                PXGraphicUIElementFlagSet(pxUIElement, PXUIElementDecorative);
                pxUIElement->ColorTintReference = pxUIElementCreateData->ColorTintReference;
                PXUIElementSizeSet(pxUIElement, 0.00, 1.95, 0.0, 0.00, PXUIElementPositionRelative);


                PXUIElementColorSet4F(pxUIElement, 0.40f, 0.15f, 0.15f, 1);

 
                pxUIElement->OnClickCallback = pxUIElementCreateData->OnClickCallback;

            }


            switch (pxUIElement->Type)
            {
                case PXUIElementTypeText:
                {
                    pxUIElement->TextInfo.FontID = pxUIElementCreateData->FontReference;
                    PXGraphicPXUIElementTextSetA(pxUIElement, pxUIElementCreateData->Text);
                    break;
                }

                case PXUIElementTypeRenderFrame:
                {
                    //PXOpenGL* const pxOpenGL = &pxGraphic->OpenGLInstance;

#if 0
                    // Texture
                   // PXTexture2DSet(pxUIElement->FrameRenderTextureReference, PXColorFormatRGBI8, pxUIElement->FrameRenderWidth, pxUIElement->Height);
                    PXOpenGLTexture2DCreate(pxOpenGL, pxUIElement->FrameRenderTextureReference);


                    // Framebuffer
                    PXOpenGLFrameBufferCreate(pxOpenGL, 1, &pxUIElement->FrameBufferID);
                    PXOpenGLFrameBufferBind(pxOpenGL, PXOpenGLFrameBufferModeDrawAndRead, pxUIElement->FrameBufferID);

                    // Renderbuffer as depthbuffer
                    PXOpenGLRenderBufferCreate(pxOpenGL, 1, &pxUIElement->FrameRenderID);
                    PXOpenGLRenderBufferBind(pxOpenGL, pxUIElement->FrameRenderID);
                    PXOpenGLRenderBufferStorage(pxOpenGL, PXOpenGLRenderBufferFormatDepthComponent, pxUIElement->FrameRenderWidth, pxUIElement->FrameRenderHeight);

                    // Link buffer
                    PXOpenGLFrameBufferLinkRenderBuffer(pxOpenGL, PXOpenGLRenderBufferAttachmentPointDepth, pxUIElement->FrameRenderID);



                    PXOpenGLFrameBufferLinkTexture2D(pxOpenGL, PXOpenGLRenderBufferAttachmentPointColor, PXGraphicTextureType2D, pxUIElement->FrameRenderTextureReference->ResourceID.OpenGLID, 0);

                    // Set the list of draw buffers.
                   // GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
                    //glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

                   // // Always check that our framebuffer is ok
                    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                     //   return false;


                    PXOpenGLFrameBufferBind(pxOpenGL, PXOpenGLFrameBufferModeDrawAndRead, 0);
                    PXOpenGLRenderBufferBind(pxOpenGL, 0);
#endif
                    break;
                }

                default:
                    break;
            }




            /*
            switch (pxUIElement->Type)
            {
                case PXUIElementTypePanel:
                {
                    const float vertexData[] =
                    {
                         -1,  -1,  0, 0,1,
                        1,  -1,  0,  1,1,
                         1, 1,  0,   1,0,
                        -1, 1,  0,   0,0
                    };
                    const PXSize vertexDataSize = sizeof(vertexData) / sizeof(float);

                    PXByte bufferData[32];
                    PXModel model;

                    ModelConstruct(&model);

                    model.Data = bufferData;

                    MemoryClear(bufferData, sizeof(bufferData));
                    ModelSegmentsAdd(&model, 4u, vertexDataSize, -1);

                    model.DataVertexList = vertexData;
                    model.DataVertexListSize = vertexDataSize;

                    model.DataVertexWidth = 3u;
                    model.DataVertexSize = vertexDataSize;
                    model.DataTextureWidth = 2u;
                    model.DataTextureSize = vertexDataSize;

                    {
                        const PXActionResult actionResult = PXGraphicModelRegisterFromModel(pxGraphic, &pxUIPanel->UIElement.Renderable, &model);

                        PXActionExitOnError(actionResult);
                    }

                    PXGraphicRenderableRegister(pxGraphic, &pxUIPanel->UIElement.Renderable);

                    break;
                }
                case PXUIElementTypeLabel:
                {
                    const PXSize textSize = PXTextLengthA(text, 256);
                    const PXSize vertexDataSize = textSize * 4u * (3u + 2u);

                    float* vertexData = MemoryAllocate(vertexDataSize * sizeof(float));

                    PXSize index = 0;
                    float xoffset = 0;

                    float imgwidth = pxGraphicUIText->TextFont->FontElement->FontPageList[0].FontTextureMap.Width;
                    float imgheight = pxGraphicUIText->TextFont->FontElement->FontPageList[0].FontTextureMap.Height;

                    for (size_t i = 0; i < textSize; i++)
                    {
                        char character = text[i];

                        PXSpriteFontCharacter* PXSpriteFontChar = PXSpriteFontGetCharacter(pxGraphicUIText->TextFont->FontElement, character);

                        float texturePositionX = PXSpriteFontChar->Position[0] / imgwidth;
                        float texturePositionY = PXSpriteFontChar->Position[1] / imgheight;
                        float texturePositionWidth = PXSpriteFontChar->Size[0] / imgwidth;
                        float texturePositionHeight = PXSpriteFontChar->Size[1] / imgheight;

                        vertexData[index++] = xoffset;
                        vertexData[index++] = 0;
                        vertexData[index++] = 0;

                        vertexData[index++] = texturePositionX;
                        vertexData[index++] = texturePositionY + texturePositionHeight;

                        vertexData[index++] = PXSpriteFontChar->Size[0] + xoffset;
                        vertexData[index++] = 0;
                        vertexData[index++] = 0;

                        vertexData[index++] = texturePositionX + texturePositionWidth;
                        vertexData[index++] = texturePositionY + texturePositionHeight;

                        vertexData[index++] = PXSpriteFontChar->Size[0] + xoffset;
                        vertexData[index++] = PXSpriteFontChar->Size[1];
                        vertexData[index++] = 0;

                        vertexData[index++] = texturePositionX + texturePositionWidth;
                        vertexData[index++] = texturePositionY;

                        vertexData[index++] = xoffset;
                        vertexData[index++] = PXSpriteFontChar->Size[1];
                        vertexData[index++] = 0;

                        vertexData[index++] = texturePositionX;
                        vertexData[index++] = texturePositionY;



                        xoffset += PXSpriteFontChar->XAdvance + 10;


                        //vertexData[index++] = PXSpriteFontChar->Position[0];
                        //vertexData[index++] = PXSpriteFontChar->Position[1];

                        //vertexData[index++] = PXSpriteFontChar->Position[0];
                        //vertexData[index++] = PXSpriteFontChar->Position[1];
                        //vertexData[index++] = PXSpriteFontChar->Position[0];
                        //vertexData[index++] = PXSpriteFontChar->Position[1];

                    }

                    PXByte bufferData[32];
                    PXModel model;

                    ModelConstruct(&model);

                    model.Data = bufferData;

                    MemoryClear(bufferData, sizeof(bufferData));
                    ModelSegmentsAdd(&model, 4u, vertexDataSize, -1);

                    model.DataVertexList = vertexData;
                    model.DataVertexListSize = vertexDataSize;

                    model.DataVertexWidth = 3u;
                    model.DataVertexSize = vertexDataSize;
                    model.DataTextureWidth = 2u;
                    model.DataTextureSize = vertexDataSize;

                    {
                        const PXActionResult actionResult = PXGraphicModelRegisterFromModel(pxGraphic, &pxGraphicUIText->UIElement.Renderable, &model);

                        PXActionExitOnError(actionResult);
                    }

                    PXGraphicRenderableRegister(pxGraphic, &pxGraphicUIText->UIElement.Renderable);



                    PXTexture pxTexture;

                    PXTextureConstruct(&pxTexture);

                    MemoryCopy(&pxGraphicUIText->TextFont->FontElement[0].FontPageList[0].FontTextureMap, sizeof(Image), &pxTexture.Image, sizeof(Image));

                    pxTexture.Type = PXGraphicTextureType2D;
                    pxTexture.Filter = PXGraphicRenderFilterNoFilter;
                    pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
                    pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
                    pxTexture.WrapHeight = PXGraphicImageWrapStrechEdges;
                    pxTexture.WrapWidth = PXGraphicImageWrapStrechEdges;

                    PXGraphicTextureRegister(pxGraphic, &pxTexture);

                    pxGraphicUIText->UIElement.Renderable.MeshSegmentList[0].TextureID = pxTexture.ID;

                    break;
                }
                case PXUIElementTypeButton:
                {
                    PXRenderable* renderable = &pxButton->UIElement.Renderable;

                    pxButton->TextFont = pxFont;
                    PXGraphicUITextRegister(pxGraphic, renderable, 0, 0, 1, 1, text);
                    PXGraphicModelShaderSet(pxGraphic, renderable, shader);
                    PXMatrix4x4FScaleSet(0.0017, 0.002, 1, &renderable->MatrixModel);
                    PXMatrix4x4FMoveToScaleXY(&renderable->MatrixModel, -0.9, -0.9, &renderable->MatrixModel);
                    renderable->MeshSegmentList[0].RenderMode = PXGraphicDrawModeSquare;

                    PXLockEngage(&pxGraphic->_pxUIElements);
                    PXLinkedListFixedNodeAdd(&pxGraphic->_pxUIElements, &pxButton->UIElement);
                    PXLockRelease(&pxGraphic->_pxUIElements);

                    break;
                }
                case PXUIElementTypeImage:

                case PXUIElementTypeCustom:

                default:
                    break;
            }*/


            break;
        }
        case PXEngineCreateTypeShaderProgram:
        {
            PXShaderProgramCreateData* const pxShaderProgramCreateData = &pxEngineResourceCreateInfo->ShaderProgram;
            PXShaderProgram* pxShaderProgram = *(PXShaderProgram**)pxEngineResourceCreateInfo->ObjectReference;

            // Make sure object exists
            {
                if (!pxShaderProgram)
                {
                    pxShaderProgram = PXNew(PXShaderProgram);
                    *pxEngineResourceCreateInfo->ObjectReference = pxShaderProgram;
                }
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Shader-Create",
                "load..."
            );
#endif

            PXResourceIDMarkAsUnused(&pxShaderProgram->ResourceID);

            pxEngine->Graphic.ShaderProgramCreateFromFileVPA
            (
                pxEngine->Graphic.EventOwner,
                pxShaderProgram,
                pxShaderProgramCreateData->VertexShaderFilePath,
                pxShaderProgramCreateData->PixelShaderFilePath
            );

            break;
        }
        case PXEngineCreateTypeDialogBox:
        {





            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXEngineResourceRender(PXEngine* const pxEngine, PXEngineResourceRenderInfo* const pxEngineResourceRenderInfo)
{
    if (!(pxEngine && pxEngineResourceRenderInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    switch (pxEngineResourceRenderInfo->Type)
    {
        case PXEngineCreateTypeModel:
        {
            PXModel* const pxModel = pxEngineResourceRenderInfo->ModelRender.ModelReference;

            if (!pxModel->Enabled)
            {
                break;
            }

            break;
        }
        case PXEngineCreateTypeSkybox:
        {
            PXSkyBox* const pxSkyBox = pxEngineResourceRenderInfo->SkyBoxRender.SkyBoxReference;

            PXOpenGLSkyboxDraw(&pxEngine->Graphic.OpenGLInstance, pxSkyBox, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXEngineCreateTypeSprite:
        {
            PXSprite* const pxSprite = pxEngineResourceRenderInfo->SpriteRender.SpriteReference;

            if (!pxSprite->Enabled)
            {
                break;
            }

            PXGraphicSpriteDraw(&pxEngine->Graphic, pxSprite, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXEngineCreateTypeText:
        {
      

            PXGraphic* const pxGraphic = &pxEngine->Graphic;
            PXEngineText* const pxEngineText = pxEngineResourceRenderInfo->TextRender.TextReference;

            if (!pxEngineText->Text)
            {
                break; // No text, no content to render, done
            }

            PXText* const pxText = pxEngineText->Text;
            PXFont* const pxFont = pxEngineText->Font;

            if (!pxEngineText->Enabled)
            {
                break;
            }

            PXVector2F offsetShadowCurrent = { 0.0f, 0.0f };
            const PXVector2F shadowOffset = { 0.0045f, -0.005f };
            PXBlendingMode blendingMode[2] = { PXBlendingModeSoureAlphaOnly, PXBlendingModeSoureAlphaOnly };

            for (PXInt8U j = 0; j < 2u; ++j)
            {
                pxEngineText->FontScaling = 1.5;

                float offsetX = 0;
                float offsetY = 0;

                pxGraphic->ShaderProgramSelect(pxGraphic->EventOwner, PXNull);
                PXOpenGLBlendingMode(&pxGraphic->OpenGLInstance, blendingMode[j]);
                pxGraphic->Texture2DSelect(pxGraphic->EventOwner, pxFont->MainPage.Texture);

                for (PXSize i = 0; i < pxText->SizeUsed && i < pxEngineText->TextRenderAmount; ++i)
                {
                    const char character = pxText->TextA[i];
                    PXFontPageCharacter* const pxFontPageCharacter = PXFontPageCharacterFetch(&pxFont->MainPage, character);

                    float textureWidth;
                    float textureHeight;
                    float charWidth;
                    float charHeight;
                    float charWidthSpacing;
                    float tx1;
                    float ty1;
                    float tx2;
                    float ty2;

                    if (pxFontPageCharacter)
                    {
                        textureWidth = pxFont->MainPage.Texture->Image->Width;
                        textureHeight = pxFont->MainPage.Texture->Image->Height;

                        charWidth = pxFontPageCharacter->Size[0];
                        charHeight = pxFontPageCharacter->Size[1];
                        charWidthSpacing = pxFontPageCharacter->XAdvance;

                        tx1 = pxFontPageCharacter->Position[0] / textureWidth;
                        ty1 = pxFontPageCharacter->Position[1] / textureHeight;
                        tx2 = ((pxFontPageCharacter->Position[0] + pxFontPageCharacter->Size[0]) / textureWidth);
                        ty2 = ((pxFontPageCharacter->Position[1] + pxFontPageCharacter->Size[1]) / textureHeight);
                    }
                    else
                    {
                        textureWidth = 20;
                        textureHeight = 20;

                        charWidth = 40;
                        charHeight = 60;
                        charWidthSpacing = 45;

                        tx1 = 0;
                        ty1 = 0;
                        tx2 = 1;
                        ty2 = 1;
                    }

                    //pxUIElement->TextInfo.Scale = 0.35;

                    float sclaingWidth = (9.0f / 16.0f) * pxEngineText->FontScaling * pxEngineText->Scaling.X;
                    float scalingHeight = pxEngineText->Scaling.Y;

                    float x1 = -1 + offsetX + pxEngineText->Position.X + offsetShadowCurrent.X;// currentOffset.Left + offsetX; // offset // currentOffset.Left  
                    float y1 = -1 + offsetY + pxEngineText->Position.Y + offsetShadowCurrent.Y;// currentOffset.Bottom;

                    // Add offset
                    x1 += 0;// currentOffset.Left;
                    y1 += 0;//currentOffset.Top;

                    float x2 = (x1 + ((charWidth / textureWidth) * sclaingWidth));
                    float y2 = (y1 + ((charHeight / textureHeight) * scalingHeight));

                    offsetX += ((charWidthSpacing / textureWidth) * sclaingWidth);



                    //x1 -= 1.0;
                    //y1 += 0.9;
                    //x2 -= 1.0;
                    //y2 += 0.9;

                    // Handle special characters
                    switch (character)
                    {
                        case '\n':
                        {
                            // Reset X and go to next Y line
                            offsetY -= 0.2f * scalingHeight;
                            offsetX = 0;

                            continue;
                        }
                        case ' ':
                        {
                            continue;
                        }
                    }


#if 0
                    pxGraphic->DrawColorRGBAF // Text color
                    (
                        pxGraphic->EventOwner,
                        pxUIElement->ColorTintReference->Red,
                        pxUIElement->ColorTintReference->Green,
                        pxUIElement->ColorTintReference->Blue,
                        pxUIElement->ColorTintReference->Alpha
                    );
#endif



                    if (j == 0)
                    {
#if 0 // Text debug
                        pxGraphic->Texture2DSelect(pxGraphic->EventOwner, 0);
                        pxGraphic->DrawModeSet(pxGraphic->EventOwner, PXGraphicDrawFillModeFill);
                        pxGraphic->DrawColorRGBAF(pxGraphic->EventOwner, 0, 1, 0, 1);
                        pxGraphic->RectangleDraw(pxGraphic->EventOwner, x1, y1, x2, y2, 0x01);
#else

                        pxGraphic->DrawColorRGBAF // Text color
                        (
                            pxGraphic->EventOwner,
                            0.0f,
                            0.0f,
                            0.0f,
                            1.0f
                        );


                        if (pxFontPageCharacter)
                        {
                            pxGraphic->RectangleDrawTx(pxGraphic->EventOwner, x1, y1, x2, y2, tx1, ty1, tx2, ty2, 0x01);
                        }
                        else
                        {
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, PXNull);
                            pxGraphic->RectangleDraw(pxGraphic->EventOwner, x1, y1, x2, y2, 0x01);
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, pxFont->MainPage.Texture);
                        }
                    }
                    else
                    {
                        pxGraphic->DrawColorRGBAF // Text color
                        (
                            pxGraphic->EventOwner,
                            0.95f,
                            0.95f,
                            0.95f,
                            1.0f
                        );

                        if (pxFontPageCharacter)
                        {
                            pxGraphic->RectangleDrawTx(pxGraphic->EventOwner, x1, y1, x2, y2, tx1, ty1, tx2, ty2, 0x01);
                        }
                        else
                        {
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, PXNull);
                            pxGraphic->RectangleDraw(pxGraphic->EventOwner, x1, y1, x2, y2, 0x01);
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, pxFont->MainPage.Texture);
                        }
                    }


#endif // Text debug

                }


                offsetShadowCurrent.X -= shadowOffset.X;
                offsetShadowCurrent.Y -= shadowOffset.Y;

                pxGraphic->Texture2DSelect(pxGraphic->EventOwner, PXNull);
                PXOpenGLBlendingMode(&pxGraphic->OpenGLInstance, PXBlendingModeNone);

#if 0 // Text debug
                pxGraphic->DrawModeSet(pxGraphic->EventOwner, PXGraphicDrawFillModeFill);
                pxGraphic->DrawColorRGBAF(pxGraphic->EventOwner, 0, 1, 0, 1);
                pxGraphic->RectangleDraw(pxGraphic->EventOwner, currentOffset.Left, currentOffset.Top, currentOffset.Right, currentOffset.Bottom, 0x02);
#endif // Text debug
            }

            break;
        }
        default:
        {
            return PXActionRefusedArgumentInvalid;
        }
    }
}

void PXAPI PXEngineCollsisionSolve(PXEngine* const pxEngine)
{
    PXDictionary* const hitBoxLookUp = &pxEngine->HitBoxLookUp;

    for (PXSize indexA = 0; indexA < hitBoxLookUp->EntryAmountCurrent; ++indexA)
    {
        PXDictionaryEntry pxDictionaryEntryA;
        PXHitBox* pxHitBoxA = PXNull;

        PXDictionaryIndex(hitBoxLookUp, indexA, &pxDictionaryEntryA);

        pxHitBoxA = *(PXHitBox**)pxDictionaryEntryA.Value;

        if (!pxHitBoxA->Enabled)
        {
            continue;
        }

        for (PXSize indexB = 0; indexB < hitBoxLookUp->EntryAmountCurrent; ++indexB)
        {
            PXDictionaryEntry pxDictionaryEntryB;
            PXHitBox* hitBoxB = PXNull;

            PXDictionaryIndex(hitBoxLookUp, indexB, &pxDictionaryEntryB);

            hitBoxB = *(PXHitBox**)pxDictionaryEntryB.Value;

            if (!pxHitBoxA->Enabled)
            {
                continue;
            }



            /*
            const PXBool isCollising = PXCollisionAABB
            (
                currentOffset.Left,
                currentOffset.Top,
                currentOffset.Right,
                currentOffset.Bottom,
                bfEngine->CollisionCheckInfo.MousePosition[0],
                bfEngine->CollisionCheckInfo.MousePosition[1],
                bfEngine->CollisionCheckInfo.MousePosition[2],
                bfEngine->CollisionCheckInfo.MousePosition[3]
            );*/
        }
    }
}

PXActionResult PXAPI PXEngineSpriteTextureSet(PXEngine* const pxEngine, PXSprite* const pxSprite, PXTexture2D* const pxTexture2D)
{
    if (pxSprite->Model.IndexBuffer.SegmentListSize > 0)
    {
        PXMaterial* materiial = pxSprite->Model.IndexBuffer.SegmentPrime.Material;

        materiial->DiffuseTexture = pxTexture2D;
    }
    else
    {
        PXMaterial* materiial = PXNew(PXMaterial);
        PXClear(PXMaterial, materiial);

        materiial->DiffuseTexture = pxSprite->Texture;

        pxSprite->Model.IndexBuffer.SegmentListSize = 1;
        pxSprite->Model.IndexBuffer.SegmentPrime.Material = materiial;
    }

    return PXActionSuccessful;
}