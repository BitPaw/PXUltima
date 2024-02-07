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
    PXCameraViewChangeToPerspective(&pxEngine->CameraDefault, 80, PXCameraAspectRatio(&pxEngine->CameraDefault), 0.00, 100000000);

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



    //-----------------------------------------------------
    // Create containers
    //-----------------------------------------------------
    PXDictionaryConstruct(&pxEngine->SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxEngine->TextLookUp, sizeof(PXInt32U), sizeof(PXEngineText), PXDictionaryValueLocalityExternalReference);
    //-----------------------------------------------------



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
        case PXEngineCreateTypeModel:
        {
            PXModelCreateEventData* const pxModelCreateEventData = &pxEngineResourceCreateInfo->Model;


            break;
        }
        case PXEngineCreateTypeFont:
        {
            PXEngineFontCreateData* const pxEngineFontCreateData = &pxEngineResourceCreateInfo->Font;
            PXFont* const pxFont = pxEngineFontCreateData->FontReference;

            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Font-Create",
                "load <%s>.",
                pxEngineFontCreateData->FontFilePath
            );

            // Load texture
            {
                PXText pxText;
                PXTextConstructFromAdressA(&pxText, pxEngineFontCreateData->FontFilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                const PXActionResult loadResult = PXResourceLoad(pxFont, &pxText);

                PXActionReturnOnError(loadResult);
            }

            // Register as normal
            {
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Font",
                    "Registering..."
                );

               // PXLockEngage(&pxGraphic->_resourceLock);
                //pxFont->ID = PXGraphicGenerateUniqeID(pxGraphic);
                //PXDictionaryAdd(&pxGraphic->FontLookUp, &pxFont->ID, pxFont);
                //PXLockRelease(&pxGraphic->_resourceLock);

                pxEngine->Graphic.Texture2DRegister(pxEngine->Graphic.EventOwner, &pxFont->MainPage.Texture);

                for (PXSize i = 0; i < pxFont->AdditionalPageListSize; ++i)
                {
                    PXFontPage* const pxFontPage = &pxFont->AdditionalPageList[i];

                    pxEngine->Graphic.Texture2DRegister(pxEngine->Graphic.EventOwner, &pxFontPage->Texture);
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Font",
                    "Registerd",
                    PXNull
                );
            }

            break;
        }
        case PXEngineCreateTypeSkybox:
        {
            PXSkyBoxCreateEventData* const pxSkyBoxCreateEventData = &pxEngineResourceCreateInfo->SkyBox;

            PXGraphicSkyboxRegisterA
            (
                &pxEngine->Graphic,
                pxSkyBoxCreateEventData->SkyboxReference,
                pxSkyBoxCreateEventData->SkyBoxShaderVertex,
                pxSkyBoxCreateEventData->SkyBoxShaderPixel,
                pxSkyBoxCreateEventData->SkyBoxTextureA,
                pxSkyBoxCreateEventData->SkyBoxTextureB,
                pxSkyBoxCreateEventData->SkyBoxTextureC,
                pxSkyBoxCreateEventData->SkyBoxTextureD,
                pxSkyBoxCreateEventData->SkyBoxTextureE,
                pxSkyBoxCreateEventData->SkyBoxTextureF
            );
            //bitFireEngine->DefaultSkyBox = &pxSkyBoxCreateEventData->SkyboxReference;

            break;
        }
        case PXEngineCreateTypeSprite:
        {
            PXSpriteCreateEventData* const pxSpriteCreateEventData = &pxEngineResourceCreateInfo->Sprite;
            PXSprite* const pxSprite = pxSpriteCreateEventData->SpriteReference;

            PXClear(PXSprite, pxSprite);
            pxSprite->PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->SpritelLookUp, &pxSprite->PXID, pxSprite);


            PXLogPrint
            (
                PXLoggingAllocation,
                "PX",
                "Sprite-Create",
                "Use <%s>",
                pxSpriteCreateEventData->TextureName
            );      

            // Clear sprite //     PXGraphicSpriteConstruct(&pxEngine->Graphic, pxSprite);
            {         
                PXModelConstruct(&pxSprite->Model);

                //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
                //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

                PXVector2FSetXY(&pxSprite->TextureScalePositionOffset, 1, 1);
                PXVector2FSetXY(&pxSprite->TextureScalePointOffset, 1, 1);

                //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
            }

            PXTextCopyA(pxSpriteCreateEventData->TextureName, 20, pxSprite->Name, 50);

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

            PXGraphicSpriteTextureLoadA(&pxEngine->Graphic, pxSprite, pxSpriteCreateEventData->TextureName);

            PXMaterial* materiial = PXNew(PXMaterial);
            PXClear(PXMaterial, materiial);

            materiial->DiffuseTexture = pxSprite->Texture;

            pxSprite->Model.IndexBuffer.SegmentListSize = 1;
            pxSprite->Model.IndexBuffer.SegmentPrime.Material = materiial;            

    

            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Sprite",
                "Register ID:%i from %i",
                pxSprite->PXID,
                pxEngine->SpritelLookUp.EntryAmountCurrent
            );

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

            if (pxSprite->Texture->Image.Width && pxSprite->Texture->Image.Height)
            {
                aspectRationX = (float)pxSprite->Texture->Image.Width / (float)pxSprite->Texture->Image.Height;
            }

            PXMatrix4x4FScaleSetXY(&pxSprite->Model.ModelMatrix, aspectRationX, 1);

            PXMatrix4x4FScaleByXY
            (
                &pxSprite->Model.ModelMatrix,
                pxSpriteCreateEventData->Scaling.X,
                pxSpriteCreateEventData->Scaling.Y
            );


            PXMatrix4x4FPositionSet(&pxSprite->Model.ModelMatrix, &pxSpriteCreateEventData->Position);

            break;
        }
        case PXEngineCreateTypeText:
        {
            PXEngineTextCreateData* const pxEngineTextCreateData = &pxEngineResourceCreateInfo->Text;
            PXEngineText* const pxEngineText = pxEngineTextCreateData->EngineTextReference;

            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Text",
                "Register",
                PXNull
            );

            pxEngineText->PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->TextLookUp, &pxEngineText->PXID, pxEngineText);

            break;
        }
        case PXEngineCreateTypeUIElement:
        {
            PXEngineUIElementCreateData* const pxEngineUIElementCreateData = &pxEngineResourceCreateInfo->UIElement;
            PXUIElement* const pxUIElement = pxEngineUIElementCreateData->UIElementReference;

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

            switch (pxUIElement->Type)
            {
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

            PXGraphicSpriteDraw(&pxEngine->Graphic, pxSprite, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXEngineCreateTypeText:
        {
            PXGraphic* const pxGraphic = &pxEngine->Graphic;
            PXEngineText* const pxEngineText = pxEngineResourceRenderInfo->TextRender.TextReference;
            PXText* const pxText = &pxEngineText->Text;
            PXFont* const pxFont = pxEngineText->Font;


            PXVector2F offsetShadowCurrent = { 0.0f, 0.0f };
            const PXVector2F shadowOffset = { 0.005f, -0.005f };

            for (PXInt8U j = 0; j < 2u; ++j)
            {
                pxEngineText->FontScaling = 1.5;

                float offsetX = 0;
                float offsetY = 0;

                pxGraphic->ShaderProgramSelect(pxGraphic->EventOwner, PXNull);
                PXOpenGLBlendingMode(pxGraphic, PXBlendingModeOneToOne);
                pxGraphic->Texture2DSelect(pxGraphic->EventOwner, &pxFont->MainPage.Texture);

                for (PXSize i = 0; i < pxText->SizeUsed; ++i)
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
                        textureWidth = pxFont->MainPage.Texture.Image.Width;
                        textureHeight = pxFont->MainPage.Texture.Image.Height;

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
                            offsetY -= 0.2f;
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

                        if (pxFontPageCharacter)
                        {
                            pxGraphic->RectangleDrawTx(pxGraphic->EventOwner, x1, y1, x2, y2, tx1, ty1, tx2, ty2, 0x01);
                        }
                        else
                        {
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, PXNull);
                            pxGraphic->RectangleDraw(pxGraphic->EventOwner, x1, y1, x2, y2, 0x01);
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, &pxFont->MainPage.Texture);
                        }
                    }
                    else
                    {
                        pxGraphic->DrawColorRGBAF // Text color
                        (
                            pxGraphic->EventOwner,
                            0.8f,
                            0.2f,
                            0.2f,
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
                            pxGraphic->Texture2DSelect(pxGraphic->EventOwner, &pxFont->MainPage.Texture);
                        }
                    }


#endif // Text debug

                }


                offsetShadowCurrent.X += shadowOffset.X;
                offsetShadowCurrent.Y += shadowOffset.Y;

                pxGraphic->Texture2DSelect(pxGraphic->EventOwner, PXNull);
                PXOpenGLBlendingMode(pxGraphic, PXBlendingModeNone);

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