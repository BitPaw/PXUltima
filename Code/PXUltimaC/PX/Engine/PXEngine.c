#include "PXEngine.h"

#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/UI/PXButton.h>
#include <PX/Engine/UI/PXECSEntityInfo.h>
#include <PX/Engine/UI/PXECSEntityList.h>
#include <PX/Engine/UI/PXFileDirectory.h>
#include <PX/Engine/UI/PXFooter.h>
#include <PX/Engine/UI/PXSceneView.h>
#include <PX/Engine/UI/PXTransformView.h>
#include <PX/Math/PXCollision.h>
#include <PX/Math/PXMath.h>
#include <PX/Media/ADLER/PXAdler32.h>
#include <PX/Media/D8X/PXD8D.h>
#include <PX/OS/Async/PXProcess.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Audio/Windows/PXDirectSound.h>
#include <PX/OS/Audio/Windows/PXMultiMedia.h>
#include <PX/OS/Audio/Windows/PXWASAPI.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/DataBase/PXDataBase.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/PXOS.h>
#include <PX/Type/PXBrush.h>
#include <PX/Type/PXCamera.h>
#include <PX/Type/PXCPU.h>
#include <PX/Type/PXDialogBox.h>
#include <PX/Type/PXFieldEffect.h>
#include <PX/Type/PXFont.h>
#include <PX/Type/PXMesh.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXScene.h>
#include <PX/Type/PXSkyBox.h>
#include <PX/Type/PXSpaceGrid.h>
#include <PX/Type/PXSprite.h>
#include <PX/Type/PXWindow.h>
#include <PX/OS/Async/PXThread.h>

//-------------------------------------
// Modules
#include <PX/Engine/PXEngineAudio.h>
#include <PX/Engine/PXEngineRender.h>
#include <PX/Engine/PXEngineScene.h>
#include <PX/Engine/PXEngineTimer.h>
#include <PX/Engine/PXEngineInput.h>
#include <PX/Engine/PXEngineNetwork.h>
//-------------------------------------




#if OSWindows
#include <Xinput.h>
#include <stdio.h>

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dinput8.lib")
#endif

#include <assert.h>
#include <math.h>
#include "PXEngineMath.h"
#include "PXEngineModManager.h"

typedef struct PXEngine_
{
    PXECSInfo Info;

    //-------------------------------------------
    PXThread* ThreadConductor; // Orchestrator of the main engine loop
    //-------------------------------------------


    //PXControllerSystem ControllerSystem;
    PXDataBase DataBase;

    PXEngineContext Context;


    //PXKeyBoard KeyBoardCurrentInput;
    //PXMouse MouseCurrentInput;

    //PXAudioDevice* AudioStandardOutDevice;

    void* Owner;
    //PXCamera* CameraCurrent;

    PXEngineResourceAdded ResourceAdded;

    PXEngineStartUpEvent OnStartUp;
    PXEngineShutDownEvent OnShutDown;
    PXEngineUserUpdateEvent OnUserUpdate;
    PXEngineNetworkUpdateEvent OnNetworkUpdate;
    PXEngineGameUpdateEvent OnGameUpdate;
    PXEngineRenderUpdateEvent OnRenderUpdate;
    PXEngineInteractCallBack OnInteract;

    PXBool IsRunning;
    PXBool UpdateUI;
    PXBool DoRendering;

    // Time/Event
    PXBool EventUIAviable;
    PXBool EventEngineThreadDone;


    // Input
    PXI64U TimeInputDeltaMin;
    PXI64U TimeInputLast;

    // Timeing
    PXI64U TimeRenderDeltaMin;
    PXI64U TimeRenderLast;


    // Cached most-common objects
    //PXModel SpriteScaled;
    //PXModel SpriteUnScaled;

    PXMathRandomGeneratorSeed RandomGeneratorSeed;

    PXBool HasGraphicInterface;
    PXBool InteractionLock;

    char ApplicationName[64];

    //---------------------------------
    // Time
    PXBool UseImgameTimer; // A special timer to react every x FPS
    //---------------------------------

    // EventDrivenSystem

}
PXEngine;


const char PXEngineText[] = "Engine-PX";
const char PXRegisterText[] = "Register";
const PXI8U PXEngineTextLength = sizeof(PXEngineText);
const PXECSRegisterInfoStatic PXEngineRegisterInfoStatic =
{
    {sizeof(PXEngineText), sizeof(PXEngineText), PXEngineText, TextFormatASCII},
    sizeof(PXEngine),
    __alignof(PXEngine),
    PXECSTypeSystem,
    PXEngineCreate,
    PXEngineRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineRegisterInfoDynamic;


#if 0
PXEngineUpdateFuntion _pxEngineUpdateFuntionList[] =
{
    PXEngineUpdateAwait,
    PXEngineUpdateTimers,
    PXEngineUpdateInput,
    PXEngineUpdateNetwork,
    PXEngineUpdateWindow,
    PXEngineUpdateTimer,
    PXEngineUpdateCollision,
    PXEngineUpdateSpriteAnimator,
    PXEngineUpdateUser,
    PXEngineUpdateRender
};
#endif

PXEngineModuleCreateFunction pxEngineModuleCreateList[] =
{
    // No dependecy
    PXEngineCreateMath,
    PXEngineCreateTime,
    PXEngineCreateModManager,

    // Minor dependecy
    PXEngineCreateUI,
    PXEngineCreateRender,
    PXEngineCreateAudio,
    PXEngineCreateInput,
    PXEngineCreateScene
};
const PXI8U pxEngineModuleCreateAmount = sizeof(pxEngineModuleCreateList) / sizeof(PXEngineModuleCreateFunction);


PXAudioInitializeFunction _pxAudioInitializeList[] =
{
#if OSUnix
    PXALSAInitialize,
    PXPulseInitialize
#elif OSWindows
    PXMultiMediaInitialize,
    PXDirectSoundInitialize,
    PXWASAPIInitialize
#endif
};
const PXI8U _pxAudioInitializeListAmount = sizeof(_pxAudioInitializeList) / sizeof(PXAudioInitializeFunction);


void PXAPI PXEngineRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXEngineRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXEngineRegisterInfoDynamic;
}

PXResult PXAPI PXEngineCreate(PXEngine** pxEngineREF, PXEngineCreateInfo PXREF pxEngineCreateInfo)
{
    PXEngine* pxEngine = PXNull;
    PXResult pxResult = PXResultInvalid;

    if(!(pxEngineREF && pxEngineCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineCreateInfo->Info.Static = &PXEngineRegisterInfoStatic;
    pxEngineCreateInfo->Info.Dynamic = &PXEngineRegisterInfoDynamic;

    pxResult = PXECSCreate(pxEngineREF, pxEngineCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngine = *pxEngineREF;
    pxEngine->OnStartUp = pxEngineCreateInfo->OnStartUp;
    pxEngine->OnShutDown = pxEngineCreateInfo->OnShutDown;
    pxEngine->OnUserUpdate = pxEngineCreateInfo->OnUserUpdate;
    pxEngine->OnNetworkUpdate = pxEngineCreateInfo->OnNetworkUpdate;
    pxEngine->OnGameUpdate = pxEngineCreateInfo->OnGameUpdate;
    pxEngine->OnRenderUpdate = pxEngineCreateInfo->OnRenderUpdate;
    pxEngine->OnInteract = pxEngineCreateInfo->OnInteract;
    pxEngine->Owner = pxEngineCreateInfo->Owner;

    PXTextCopyA(pxEngineCreateInfo->Name, PXTextLengthUnkown, pxEngine->ApplicationName, 64);

    // Setup core systems
    pxResult = PXDictionaryECSCreate(&pxEngine->Context.SystemLookup);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineText,
        "Create",
        "Loading modules <%i>...",
        pxEngineModuleCreateAmount
    );
#endif

    PXThreadCreateInfo pxThreadCreateInfo;
    PXClear(PXThreadCreateInfo, &pxThreadCreateInfo);
    pxThreadCreateInfo.CreateSuspended = PXTrue;
    pxThreadCreateInfo.Parameter = pxEngine;
    pxThreadCreateInfo.ThreadFunction = PXEngineRootLoop;

    pxResult = PXThreadCreate(&pxEngine->ThreadConductor, &pxThreadCreateInfo);
    
    // We start all threads one after each 
    // other because they rely on a spesific order.

    for(PXSize i = 0; i < pxEngineModuleCreateAmount; ++i)
    {
        PXEngineModuleCreateFunction pxEngineModuleCreate = pxEngineModuleCreateList[i];

        if(!pxEngineModuleCreate)
        {
            continue;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            PXEngineText,
            "Create",
            "Module <%2i/%-2i>",
            i+1,
            pxEngineModuleCreateAmount
        );
#endif
        
        PXResult pxResult = pxEngineModuleCreate(pxEngine, pxEngineCreateInfo);        
    }

    return PXResultOK;
}

PXResult PXAPI PXEngineRelease(PXEngine PXREF pxEngine)
{
    if(!pxEngine)
    {
        return PXResultRefusedParameterNull;
    }

    //PXControllerSystemShutdown(&pxEngine->ControllerSystem);
    //PXGraphicRelease(&pxEngine->Graphic);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineText,
        "Shutdown",
        "Following data might be a sign of a resource leak. Check and advice"
    );
#endif

    // List all open resources, to check for memory and resource leaks
    PXDebug pxDebug;
    PXDebugDebuggerInitialize(&pxDebug);


    // List all open threads
    {
        PXThread* pxThreadList[64];
        PXSize amountResultInput = 64;
        PXSize amountResultOutput = 0;

        PXProcessThreadsListAll(PXNull, pxThreadList, amountResultInput, &amountResultOutput);

        for(PXSize i = 0; i < amountResultOutput; ++i)
        {
            PXThread PXREF pxThread = pxThreadList[i];

            PXText text;
            PXTextConstructBufferA(&text, 256);

            PXThreadNameGet(&pxDebug, pxThread, &text);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXEngineText,
                "Shutdown",
                "Thread %s",
                text.A
            );
#endif

        }
    }

    // list all open handles

    // list all open pointers

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineText,
        "Shutdown",
        "Done"
    );
#endif

    return PXResultRefusedNotImplemented;
}

PXThreadResult PXOSAPI PXEngineRootLoop(PXEngine PXREF pxEngine)
{
    PXResult pxResult = PXResultOK;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineText,
        "ASYNC-Loop",
        "START"
    );
#endif

    PXThreadResult pxThreadResult;
    pxThreadResult.Data = 0;

    PXEngineModuleASYNCActivate(pxEngine);

    for(;;)
    {
        pxResult = PXEngineTimeWaitGAME(pxEngine, pxEngine->Context.Time);

        if(PXResultNoWorkToDo == pxResult)
        {
            // If we did nothing an entire loop, that can mean our 
            // pgysics aswell as rendering system is gone, which 
            // makes this loop not viable and can be closed.
            break; 
        }
    }

    PXEngineModuleASYNCDeactivate(pxEngine);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineText,
        "ASYNC-Loop",
        "STOP"
    );
#endif

    return pxThreadResult;
}

PXThread* PXAPI PXEngineRootThread(const PXEngine PXREF pxEngine)
{
    if(!pxEngine)
    {
        return PXNull;
    }

    return pxEngine->ThreadConductor;
}

void PXAPI PXEngineModuleSerialActivate(PXEngine PXREF pxEngine)
{
    const PXSize moduleAmount = PXEngineModuleAmount(pxEngine);

    for(PXSize i = 0; i < moduleAmount; ++i)
    {
        PXEngineModule PXREF pxEngineModule = PXEngineModuleIndex(pxEngine, i);

        PXEngineModuleActivate(pxEngineModule, pxEngine);
    }
}

void PXAPI PXEngineModuleSerialDeactivate(PXEngine PXREF pxEngine)
{
   
}

void PXAPI PXEngineModuleASYNCActivate(PXEngine PXREF pxEngine)
{
    const PXSize moduleAmount = PXEngineModuleAmount(pxEngine);

    for(PXSize i = 0; i < moduleAmount; ++i)
    {
        PXEngineModule PXREF pxEngineModule = PXEngineModuleIndex(pxEngine, i);

        PXEngineModuleActivate(pxEngineModule, pxEngine);
    }
}

void PXAPI PXEngineModuleASYNCDeactivate(PXEngine PXREF pxEngine)
{
   
}


#define UseOSDelta 0

void PXAPI PXEngineWindowEvent(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXEngine PXREF pxEngine = (PXEngine*)pxWindowEvent->Owner;

    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeElementDestroy:
        {
            //pxEngine->WindowMain = PXNull; // Window is beeing destroyed, reference illegal
            break;
        }
        case PXWindowEventTypeElementResize:
        {
            pxEngine->UpdateUI = PXTrue;
            break;
        }
        case PXWindowEventTypeInputMouseButton:
        {
            PXWindowEventMouseButtonData PXREF inputMouseButton = &pxWindowEvent->MouseButton;



#if 0
            PXMouse PXREF mouse = &pxEngine->MouseCurrentInput;

            switch(inputMouseButton->PressState)
            {
                case PXKeyPressStateDown:
                {
                    switch(inputMouseButton->Button)
                    {
                        case PXMouseButtonLeft:
                        {
                            mouse->ButtonsDelta |= ButtonLeft;
                            break;
                        }
                        case PXMouseButtonMiddle:
                        {
                            mouse->ButtonsDelta |= ButtonMiddle;
                            break;
                        }
                        case PXMouseButtonRight:
                        {
                            mouse->ButtonsDelta |= ButtonRight;
                            break;
                        }
                        case PXMouseButtonSpecialA:
                        {
                            mouse->ButtonsDelta |= ButtonCustomA;
                            break;
                        }
                        case PXMouseButtonSpecialB:
                        {
                            mouse->ButtonsDelta |= ButtonCustomB;
                            break;
                        }
                    }

                    mouse->Buttons |= mouse->ButtonsDelta;

                    break;
                }
                case PXKeyPressStateUp:
                {
                    switch(inputMouseButton->Button)
                    {
                        case PXMouseButtonLeft:
                        {
                            mouse->Buttons &= ~ButtonLeft;
                            break;
                        }
                        case PXMouseButtonMiddle:
                        {
                            mouse->Buttons &= ~ButtonMiddle;
                            break;
                        }
                        case PXMouseButtonRight:
                        {
                            mouse->Buttons &= ~ButtonRight;
                            break;
                        }
                        case PXMouseButtonSpecialA:
                        {
                            mouse->Buttons &= ~ButtonCustomA;
                            break;
                        }
                        case PXMouseButtonSpecialB:
                        {
                            mouse->Buttons &= ~ButtonCustomB;
                            break;
                        }
                    }
                    break;
                }

                default:
                    break;
            }

            const char* buttonStateText = 0;
            const char* mouseButtonText = 0;

            switch(buttonState)
            {
                case PXKeyPressStateInvalid:
                    buttonStateText = "Invalid";
                    break;

                case PXKeyPressStateDown:
                    buttonStateText = "Down";
                    break;

                case PXKeyPressStateHold:
                    buttonStateText = "Hold";
                    break;

                case PXKeyPressStateUp:
                    buttonStateText = "Release";
                    break;
            }

            switch(mouseButton)
            {
                case PXMouseButtonInvalid:
                    mouseButtonText = "Invalid";
                    break;

                case PXMouseButtonLeft:
                    mouseButtonText = "Left";
                    break;

                case PXMouseButtonMiddle:
                    mouseButtonText = "Middle";
                    break;

                case PXMouseButtonRight:
                    mouseButtonText = "Right";
                    break;

                case PXMouseButtonSpecialA:
                    mouseButtonText = "Special A";
                    break;

                case PXMouseButtonSpecialB:
                    mouseButtonText = "Special B";
                    break;

                case PXMouseButtonSpecialC:
                    mouseButtonText = "Special C";
                    break;

                case PXMouseButtonSpecialD:
                    mouseButtonText = "Special D";
                    break;

                case PXMouseButtonSpecialE:
                    mouseButtonText = "Special E";
                    break;
            }

            printf("[#][Event][Mouse] Button:%-10s State:%-10s\n", mouseButtonText, buttonStateText);

#endif

            PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, PXNull);

            break;
        }
        case PXWindowEventTypeInputMouseMove:
        {
            PXWindowEventMouseMoveData PXREF inputMouseMove = &pxWindowEvent->MouseMove;

            PXEngineUpdateMouse(pxEngine, inputMouseMove);

            PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, PXNull);

            break;
        }
        case PXWindowEventTypeInputKeyboard:
        {
            PXWindowEventKeyboardData PXREF inputKeyboard = &pxWindowEvent->Keyboard;

            PXEngineUpdateKeyBoard(pxEngine, inputKeyboard);


            // printf("[#][Event][Key] ID:%-3i Name:%-3i State:%i\n", keyBoardKeyInfo->KeyID, keyBoardKeyInfo->Key, keyBoardKeyInfo->Mode);
#if 0
            //PXKeyBoard PXREF keyBoard = &pxEngine->KeyBoardCurrentInput;

            PXI32U mask = 0;
            PXI32U data = 0;

            if(PXKeyPressStateDown == inputKeyboard->PressState)
            {
                switch(inputKeyboard->VirtualKey)
                {
                    case KeyA:
                        keyBoard->Letters |= KeyBoardIDLetterA;
                        break;
                    case KeyB:
                        keyBoard->Letters |= KeyBoardIDLetterB;
                        break;
                    case KeyC:
                        keyBoard->Letters |= KeyBoardIDLetterC;
                        break;
                    case KeyD:
                        keyBoard->Letters |= KeyBoardIDLetterD;
                        break;
                    case KeyE:
                        keyBoard->Letters |= KeyBoardIDLetterE;
                        break;
                    case KeyF:
                        keyBoard->Letters |= KeyBoardIDLetterF;
                        break;
                    case KeyG:
                        keyBoard->Letters |= KeyBoardIDLetterG;
                        break;
                    case KeyH:
                        keyBoard->Letters |= KeyBoardIDLetterH;
                        break;
                    case KeyI:
                        keyBoard->Letters |= KeyBoardIDLetterI;
                        break;
                    case KeyJ:
                        keyBoard->Letters |= KeyBoardIDLetterJ;
                        break;
                    case KeyK:
                        keyBoard->Letters |= KeyBoardIDLetterK;
                        break;
                    case KeyL:
                        keyBoard->Letters |= KeyBoardIDLetterL;
                        break;
                    case KeyM:
                        keyBoard->Letters |= KeyBoardIDLetterM;
                        break;
                    case KeyN:
                        keyBoard->Letters |= KeyBoardIDLetterN;
                        break;
                    case KeyO:
                        keyBoard->Letters |= KeyBoardIDLetterO;
                        break;
                    case KeyP:
                        keyBoard->Letters |= KeyBoardIDLetterP;
                        break;
                    case KeyQ:
                        keyBoard->Letters |= KeyBoardIDLetterQ;
                        break;
                    case KeyR:
                        keyBoard->Letters |= KeyBoardIDLetterR;
                        break;
                    case KeyS:
                        keyBoard->Letters |= KeyBoardIDLetterS;
                        break;
                    case KeyT:
                        keyBoard->Letters |= KeyBoardIDLetterT;
                        break;
                    case KeyU:
                        keyBoard->Letters |= KeyBoardIDLetterU;
                        break;
                    case KeyV:
                        keyBoard->Letters |= KeyBoardIDLetterV;
                        break;
                    case KeyW:
                        keyBoard->Letters |= KeyBoardIDLetterW;
                        break;
                    case KeyX:
                        keyBoard->Letters |= KeyBoardIDLetterX;
                        break;
                    case KeyY:
                        keyBoard->Letters |= KeyBoardIDLetterY;
                        break;
                    case KeyZ:
                        keyBoard->Letters |= KeyBoardIDLetterZ;
                        break;
                    case KeySpace:
                        keyBoard->Letters |= KeyBoardIDSpace;
                        break;
                    case KeyApostrophe:
                        keyBoard->Letters |= KeyBoardIDAPOSTROPHE;
                        break;
                    case KeyComma:
                        keyBoard->Letters |= KeyBoardIDComma;
                        break;
                    case KeyGraveAccent:
                        keyBoard->Letters |= KeyBoardIDGRAVE_ACCENT;
                        break;
                    case KeySemicolon:
                        keyBoard->Letters |= KeyBoardIDSemicolon;
                        break;
                    case KeyPeriod:
                        keyBoard->Letters |= KeyBoardIDDECIMAL;
                        break;


                    case KeyEscape:
                        keyBoard->Commands |= KeyBoardIDCommandEscape;
                        break;
                    case KeyEnter:
                        keyBoard->Commands |= KeyBoardIDCommandEnter;
                        break;
                    case KeyTab:
                        keyBoard->Commands |= KeyBoardIDCommandTab;
                        break;
                        //case : keyBoard->Commands |= KeyBoardIDCommandShift; break;
                    case KeyBackspace:
                        keyBoard->Commands |= KeyBoardIDBACKSPACE;
                        break;
                    case KeyInsert:
                        keyBoard->Commands |= KeyBoardIDINSERT;
                        break;
                    case KeyDelete:
                        keyBoard->Commands |= KeyBoardIDDELETE;
                        break;
                    case KeyRight:
                        keyBoard->Commands |= KeyBoardIDRIGHT;
                        break;
                    case KeyLeft:
                        keyBoard->Commands |= KeyBoardIDLEFT;
                        break;
                    case KeyDown:
                        keyBoard->Commands |= KeyBoardIDDOWN;
                        break;
                    case KeyUp:
                        keyBoard->Commands |= KeyBoardIDUP;
                        break;
                    case KeyPageUp:
                        keyBoard->Commands |= KeyBoardIDPAGE_UP;
                        break;
                    case KeyPageDown:
                        keyBoard->Commands |= KeyBoardIDPAGE_DOWN;
                        break;
                    case KeyHome:
                        keyBoard->Commands |= KeyBoardIDHOME;
                        break;
                    case KeyEnd:
                        keyBoard->Commands |= KeyBoardIDEND;
                        break;
                    case KeyCapsLock:
                        keyBoard->Commands |= KeyBoardIDCAPS_LOCK;
                        break;
                    case KeyScrollLock:
                        keyBoard->Commands |= KeyBoardIDSCROLL_LOCK;
                        break;
                    case KeyNumLock:
                        keyBoard->Commands |= KeyBoardIDNUM_LOCK;
                        break;
                    case KeyPrintScreen:
                        keyBoard->Commands |= KeyBoardIDPRINT_SCREEN;
                        break;
                    case KeyPause:
                        keyBoard->Commands |= KeyBoardIDPAUSE;
                        break;
                    case KeyPadEnter:
                        keyBoard->Commands |= KeyBoardIDPadENTER;
                        break;
                    case KeyShiftLeft:
                        keyBoard->Commands |= KeyBoardIDShiftLeft;
                        break;
                    case KeyShiftRight:
                        keyBoard->Commands |= KeyBoardIDShiftRight;
                        break;
                    case KeyControlLeft:
                        keyBoard->Commands |= KeyBoardIDCONTROLLEFT;
                        break;
                    case KeyAltLeft:
                        keyBoard->Commands |= KeyBoardIDALTLEFT;
                        break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDSUPERLEFT; break;
                    case KeyControlRight:
                        keyBoard->Commands |= KeyBoardIDCONTROLRIGHT;
                        break;
                    case KeyAltRight:
                        keyBoard->Commands |= KeyBoardIDALTRIGHT;
                        break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDSUPERRIGHT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDMENU; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDWORLD_1; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDWORLD_2; break;


                        // Numbers
                    case Key0:
                        keyBoard->Numbers |= KeyBoardIDNumber0;
                        break;
                    case Key1:
                        keyBoard->Numbers |= KeyBoardIDNumber1;
                        break;
                    case Key2:
                        keyBoard->Numbers |= KeyBoardIDNumber2;
                        break;
                    case Key3:
                        keyBoard->Numbers |= KeyBoardIDNumber3;
                        break;
                    case Key4:
                        keyBoard->Numbers |= KeyBoardIDNumber4;
                        break;
                    case Key5:
                        keyBoard->Numbers |= KeyBoardIDNumber5;
                        break;
                    case Key6:
                        keyBoard->Numbers |= KeyBoardIDNumber6;
                        break;
                    case Key7:
                        keyBoard->Numbers |= KeyBoardIDNumber7;
                        break;
                    case Key8:
                        keyBoard->Numbers |= KeyBoardIDNumber8;
                        break;
                    case Key9:
                        keyBoard->Numbers |= KeyBoardIDNumber9;
                        break;
                    case KeyPad0:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock0;
                        break;
                    case KeyPad1:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock1;
                        break;
                    case KeyPad2:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock2;
                        break;
                    case KeyPad3:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock3;
                        break;
                    case KeyPad4:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock4;
                        break;
                    case KeyPad5:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock5;
                        break;
                    case KeyPad6:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock6;
                        break;
                    case KeyPad7:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock7;
                        break;
                    case KeyPad8:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock8;
                        break;
                    case KeyPad9:
                        keyBoard->Numbers |= KeyBoardIDNumberBlock9;
                        break;
                    case KeyMinus:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyMinus;
                        break;
                        // case KeyPeriod: keyBoard->Numbers |= KeyBoardIDNumberKeyPeriod; break;
                    case KeySlash:
                        keyBoard->Numbers |= KeyBoardIDNumberKeySlash;
                        break;
                    case KeyEqual:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyEqual;
                        break;
                    case KeyBrackedLeft:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyLEFT_BRACKET;
                        break;
                    case KeyBackSlash:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyBACKSLASH;
                        break;
                    case KeyBrackedRight:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyRIGHT_BRACKET;
                        break;
                    case KeyPadDivide:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyPadDIVIDE;
                        break;
                    case KeyPadMultiply:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyPadMULTIPLY;
                        break;
                    case KeyPadSubtract:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyPadSUBTRACT;
                        break;
                    case KeyPadAdd:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyPadADD;
                        break;
                    case KeyPadEqual:
                        keyBoard->Numbers |= KeyBoardIDNumberKeyPadEQUAL;
                        break;

                        // Function key

                    case KeyF1:
                        keyBoard->Actions |= KeyBoardIDF01;
                        break;
                    case KeyF2:
                        keyBoard->Actions |= KeyBoardIDF02;
                        break;
                    case KeyF3:
                        keyBoard->Actions |= KeyBoardIDF03;
                        break;
                    case KeyF4:
                        keyBoard->Actions |= KeyBoardIDF04;
                        break;
                    case KeyF5:
                        keyBoard->Actions |= KeyBoardIDF05;
                        break;
                    case KeyF6:
                        keyBoard->Actions |= KeyBoardIDF06;
                        break;
                    case KeyF7:
                        keyBoard->Actions |= KeyBoardIDF07;
                        break;
                    case KeyF8:
                        keyBoard->Actions |= KeyBoardIDF08;
                        break;
                    case KeyF9:
                        keyBoard->Actions |= KeyBoardIDF09;
                        break;
                    case KeyF10:
                        keyBoard->Actions |= KeyBoardIDF10;
                        break;
                    case KeyF11:
                        keyBoard->Actions |= KeyBoardIDF11;
                        break;
                    case KeyF12:
                        keyBoard->Actions |= KeyBoardIDF12;
                        break;
                    case KeyF13:
                        keyBoard->Actions |= KeyBoardIDF13;
                        break;
                    case KeyF14:
                        keyBoard->Actions |= KeyBoardIDF14;
                        break;
                    case KeyF15:
                        keyBoard->Actions |= KeyBoardIDF15;
                        break;
                    case KeyF16:
                        keyBoard->Actions |= KeyBoardIDF16;
                        break;
                    case KeyF17:
                        keyBoard->Actions |= KeyBoardIDF17;
                        break;
                    case KeyF18:
                        keyBoard->Actions |= KeyBoardIDF18;
                        break;
                    case KeyF19:
                        keyBoard->Actions |= KeyBoardIDF19;
                        break;
                    case KeyF20:
                        keyBoard->Actions |= KeyBoardIDF20;
                        break;
                    case KeyF21:
                        keyBoard->Actions |= KeyBoardIDF21;
                        break;
                    case KeyF22:
                        keyBoard->Actions |= KeyBoardIDF22;
                        break;
                    case KeyF23:
                        keyBoard->Actions |= KeyBoardIDF23;
                        break;
                    case KeyF24:
                        keyBoard->Actions |= KeyBoardIDF24;
                        break;
                    case KeyF25:
                        keyBoard->Actions |= KeyBoardIDF25;
                        break;

                    default:
                        break;
                }
            }
            else
            {
                switch(inputKeyboard->VirtualKey)
                {
                    case KeyA:
                        keyBoard->Letters &= ~KeyBoardIDLetterA;
                        break;
                    case KeyB:
                        keyBoard->Letters &= ~KeyBoardIDLetterB;
                        break;
                    case KeyC:
                        keyBoard->Letters &= ~KeyBoardIDLetterC;
                        break;
                    case KeyD:
                        keyBoard->Letters &= ~KeyBoardIDLetterD;
                        break;
                    case KeyE:
                        keyBoard->Letters &= ~KeyBoardIDLetterE;
                        break;
                    case KeyF:
                        keyBoard->Letters &= ~KeyBoardIDLetterF;
                        break;
                    case KeyG:
                        keyBoard->Letters &= ~KeyBoardIDLetterG;
                        break;
                    case KeyH:
                        keyBoard->Letters &= ~KeyBoardIDLetterH;
                        break;
                    case KeyI:
                        keyBoard->Letters &= ~KeyBoardIDLetterI;
                        break;
                    case KeyJ:
                        keyBoard->Letters &= ~KeyBoardIDLetterJ;
                        break;
                    case KeyK:
                        keyBoard->Letters &= ~KeyBoardIDLetterK;
                        break;
                    case KeyL:
                        keyBoard->Letters &= ~KeyBoardIDLetterL;
                        break;
                    case KeyM:
                        keyBoard->Letters &= ~KeyBoardIDLetterM;
                        break;
                    case KeyN:
                        keyBoard->Letters &= ~KeyBoardIDLetterN;
                        break;
                    case KeyO:
                        keyBoard->Letters &= ~KeyBoardIDLetterO;
                        break;
                    case KeyP:
                        keyBoard->Letters &= ~KeyBoardIDLetterP;
                        break;
                    case KeyQ:
                        keyBoard->Letters &= ~KeyBoardIDLetterQ;
                        break;
                    case KeyR:
                        keyBoard->Letters &= ~KeyBoardIDLetterR;
                        break;
                    case KeyS:
                        keyBoard->Letters &= ~KeyBoardIDLetterS;
                        break;
                    case KeyT:
                        keyBoard->Letters &= ~KeyBoardIDLetterT;
                        break;
                    case KeyU:
                        keyBoard->Letters &= ~KeyBoardIDLetterU;
                        break;
                    case KeyV:
                        keyBoard->Letters &= ~KeyBoardIDLetterV;
                        break;
                    case KeyW:
                        keyBoard->Letters &= ~KeyBoardIDLetterW;
                        break;
                    case KeyX:
                        keyBoard->Letters &= ~KeyBoardIDLetterX;
                        break;
                    case KeyY:
                        keyBoard->Letters &= ~KeyBoardIDLetterY;
                        break;
                    case KeyZ:
                        keyBoard->Letters &= ~KeyBoardIDLetterZ;
                        break;
                    case KeySpace:
                        keyBoard->Letters &= ~KeyBoardIDSpace;
                        break;
                    case KeyApostrophe:
                        keyBoard->Letters &= ~KeyBoardIDAPOSTROPHE;
                        break;
                    case KeyComma:
                        keyBoard->Letters &= ~KeyBoardIDComma;
                        break;
                    case KeyGraveAccent:
                        keyBoard->Letters &= ~KeyBoardIDGRAVE_ACCENT;
                        break;
                    case KeySemicolon:
                        keyBoard->Letters &= ~KeyBoardIDSemicolon;
                        break;
                    case KeyPeriod:
                        keyBoard->Letters &= ~KeyBoardIDDECIMAL;
                        break;


                    case KeyEscape:
                        keyBoard->Commands &= ~KeyBoardIDCommandEscape;
                        break;
                    case KeyEnter:
                        keyBoard->Commands &= ~KeyBoardIDCommandEnter;
                        break;
                    case KeyTab:
                        keyBoard->Commands &= ~KeyBoardIDCommandTab;
                        break;
                        //case : keyBoard->Commands &= ~KeyBoardIDCommandShift; break;
                    case KeyBackspace:
                        keyBoard->Commands &= ~KeyBoardIDBACKSPACE;
                        break;
                    case KeyInsert:
                        keyBoard->Commands &= ~KeyBoardIDINSERT;
                        break;
                    case KeyDelete:
                        keyBoard->Commands &= ~KeyBoardIDDELETE;
                        break;
                    case KeyRight:
                        keyBoard->Commands &= ~KeyBoardIDRIGHT;
                        break;
                    case KeyLeft:
                        keyBoard->Commands &= ~KeyBoardIDLEFT;
                        break;
                    case KeyDown:
                        keyBoard->Commands &= ~KeyBoardIDDOWN;
                        break;
                    case KeyUp:
                        keyBoard->Commands &= ~KeyBoardIDUP;
                        break;
                    case KeyPageUp:
                        keyBoard->Commands &= ~KeyBoardIDPAGE_UP;
                        break;
                    case KeyPageDown:
                        keyBoard->Commands &= ~KeyBoardIDPAGE_DOWN;
                        break;
                    case KeyHome:
                        keyBoard->Commands &= ~KeyBoardIDHOME;
                        break;
                    case KeyEnd:
                        keyBoard->Commands &= ~KeyBoardIDEND;
                        break;
                    case KeyCapsLock:
                        keyBoard->Commands &= ~KeyBoardIDCAPS_LOCK;
                        break;
                    case KeyScrollLock:
                        keyBoard->Commands &= ~KeyBoardIDSCROLL_LOCK;
                        break;
                    case KeyNumLock:
                        keyBoard->Commands &= ~KeyBoardIDNUM_LOCK;
                        break;
                    case KeyPrintScreen:
                        keyBoard->Commands &= ~KeyBoardIDPRINT_SCREEN;
                        break;
                    case KeyPause:
                        keyBoard->Commands &= ~KeyBoardIDPAUSE;
                        break;
                    case KeyPadEnter:
                        keyBoard->Commands &= ~KeyBoardIDPadENTER;
                        break;
                    case KeyShiftLeft:
                        keyBoard->Commands &= ~KeyBoardIDShiftLeft;
                        break;
                    case KeyShiftRight:
                        keyBoard->Commands &= ~KeyBoardIDShiftRight;
                        break;
                    case KeyControlLeft:
                        keyBoard->Commands &= ~KeyBoardIDCONTROLLEFT;
                        break;
                    case KeyAltLeft:
                        keyBoard->Commands &= ~KeyBoardIDALTLEFT;
                        break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDSUPERLEFT; break;
                    case KeyControlRight:
                        keyBoard->Commands &= ~KeyBoardIDCONTROLRIGHT;
                        break;
                    case KeyAltRight:
                        keyBoard->Commands &= ~KeyBoardIDALTRIGHT;
                        break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDSUPERRIGHT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDMENU; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDWORLD_1; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDWORLD_2; break;


                        // Numbers
                    case Key0:
                        keyBoard->Numbers &= ~KeyBoardIDNumber0;
                        break;
                    case Key1:
                        keyBoard->Numbers &= ~KeyBoardIDNumber1;
                        break;
                    case Key2:
                        keyBoard->Numbers &= ~KeyBoardIDNumber2;
                        break;
                    case Key3:
                        keyBoard->Numbers &= ~KeyBoardIDNumber3;
                        break;
                    case Key4:
                        keyBoard->Numbers &= ~KeyBoardIDNumber4;
                        break;
                    case Key5:
                        keyBoard->Numbers &= ~KeyBoardIDNumber5;
                        break;
                    case Key6:
                        keyBoard->Numbers &= ~KeyBoardIDNumber6;
                        break;
                    case Key7:
                        keyBoard->Numbers &= ~KeyBoardIDNumber7;
                        break;
                    case Key8:
                        keyBoard->Numbers &= ~KeyBoardIDNumber8;
                        break;
                    case Key9:
                        keyBoard->Numbers &= ~KeyBoardIDNumber9;
                        break;
                    case KeyPad0:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock0;
                        break;
                    case KeyPad1:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock1;
                        break;
                    case KeyPad2:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock2;
                        break;
                    case KeyPad3:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock3;
                        break;
                    case KeyPad4:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock4;
                        break;
                    case KeyPad5:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock5;
                        break;
                    case KeyPad6:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock6;
                        break;
                    case KeyPad7:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock7;
                        break;
                    case KeyPad8:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock8;
                        break;
                    case KeyPad9:
                        keyBoard->Numbers &= ~KeyBoardIDNumberBlock9;
                        break;
                    case KeyMinus:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyMinus;
                        break;
                        // case KeyPeriod: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPeriod; break;
                    case KeySlash:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeySlash;
                        break;
                    case KeyEqual:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyEqual;
                        break;
                    case KeyBrackedLeft:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyLEFT_BRACKET;
                        break;
                    case KeyBackSlash:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyBACKSLASH;
                        break;
                    case KeyBrackedRight:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyRIGHT_BRACKET;
                        break;
                    case KeyPadDivide:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadDIVIDE;
                        break;
                    case KeyPadMultiply:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadMULTIPLY;
                        break;
                    case KeyPadSubtract:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadSUBTRACT;
                        break;
                    case KeyPadAdd:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadADD;
                        break;
                    case KeyPadEqual:
                        keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadEQUAL;
                        break;

                        // Function key

                    case KeyF1:
                        keyBoard->Actions &= ~KeyBoardIDF01;
                        break;
                    case KeyF2:
                        keyBoard->Actions &= ~KeyBoardIDF02;
                        break;
                    case KeyF3:
                        keyBoard->Actions &= ~KeyBoardIDF03;
                        break;
                    case KeyF4:
                        keyBoard->Actions &= ~KeyBoardIDF04;
                        break;
                    case KeyF5:
                        keyBoard->Actions &= ~KeyBoardIDF05;
                        break;
                    case KeyF6:
                        keyBoard->Actions &= ~KeyBoardIDF06;
                        break;
                    case KeyF7:
                        keyBoard->Actions &= ~KeyBoardIDF07;
                        break;
                    case KeyF8:
                        keyBoard->Actions &= ~KeyBoardIDF08;
                        break;
                    case KeyF9:
                        keyBoard->Actions &= ~KeyBoardIDF09;
                        break;
                    case KeyF10:
                        keyBoard->Actions &= ~KeyBoardIDF10;
                        break;
                    case KeyF11:
                        keyBoard->Actions &= ~KeyBoardIDF11;
                        break;
                    case KeyF12:
                        keyBoard->Actions &= ~KeyBoardIDF12;
                        break;
                    case KeyF13:
                        keyBoard->Actions &= ~KeyBoardIDF13;
                        break;
                    case KeyF14:
                        keyBoard->Actions &= ~KeyBoardIDF14;
                        break;
                    case KeyF15:
                        keyBoard->Actions &= ~KeyBoardIDF15;
                        break;
                    case KeyF16:
                        keyBoard->Actions &= ~KeyBoardIDF16;
                        break;
                    case KeyF17:
                        keyBoard->Actions &= ~KeyBoardIDF17;
                        break;
                    case KeyF18:
                        keyBoard->Actions &= ~KeyBoardIDF18;
                        break;
                    case KeyF19:
                        keyBoard->Actions &= ~KeyBoardIDF19;
                        break;
                    case KeyF20:
                        keyBoard->Actions &= ~KeyBoardIDF20;
                        break;
                    case KeyF21:
                        keyBoard->Actions &= ~KeyBoardIDF21;
                        break;
                    case KeyF22:
                        keyBoard->Actions &= ~KeyBoardIDF22;
                        break;
                    case KeyF23:
                        keyBoard->Actions &= ~KeyBoardIDF23;
                        break;
                    case KeyF24:
                        keyBoard->Actions &= ~KeyBoardIDF24;
                        break;
                    case KeyF25:
                        keyBoard->Actions &= ~KeyBoardIDF25;
                        break;

                    default:
                        break;
                }
            }
#endif
            PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, PXNull);

            break;
        }

        default:
            break;
    }
}

PXResult PXAPI PXEngineUpdate(PXEngine PXREF pxEngine)
{
    PXResult pxResult = PXResultOK;

    if(!pxEngine)
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxEngine->IsRunning)
    {
        return PXResultRefuedServiceNotRunning; // Engine is not running, we don't release.
    }

    pxResult = PXEngineTimeWaitUI(pxEngine, pxEngine->Context.Time);

    pxResult = PXEngineEventOnUI(pxEngine);

#if 0
    PXConsoleGoToXY(0, 0);

    printf("%15s : Hz %-20i\n", "FPS", pxEngine->FramesPerSecound);
    printf("%15s : us %-20i\n", "Frame-Time", pxEngine->FrameTime);
    printf("%15s : us %-20i %3i%%\n", "Window", pxEngine->CounterTimeWindow, (int)((pxEngine->CounterTimeWindow / (PXF32)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "User", pxEngine->CounterTimeUser, (int)((pxEngine->CounterTimeUser / (PXF32)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "Network", pxEngine->CounterTimeNetwork, (int)((pxEngine->CounterTimeNetwork / (PXF32)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "Gamelogic", pxEngine->CounterTimeCPU, (int)((pxEngine->CounterTimeCPU / (PXF32)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i %3i%%\n", "Render", pxEngine->CounterTimeGPU, (int)((pxEngine->CounterTimeGPU / (PXF32)pxEngine->CounterTimeDelta) * 100));
    printf("%15s : us %-20i\n", "Full rotation", pxEngine->CounterTimeDelta);

    printf("\n");

    PXKeyBoardInputPrint(&pxEngine->Window.KeyBoardCurrentInput);

    PXMouseInputPrint(&pxEngine->Window.MouseCurrentInput);
#endif

    return pxResult;
}

PXResult PXAPI PXEngineRun(PXEngine PXREF pxEngine)
{
    if(!pxEngine)
    {
        return PXResultRefusedParameterNull;
    }

    PXEngineModuleSerialActivate(pxEngine);

    PXResult pxResult = PXResultOK;
    PXThread PXREF pxThread = PXEngineRootThread(pxEngine);

    // Forever loop
    for(;;)
    {
        pxResult = PXEngineUpdate(pxEngine);

        PXThreadState threadState = PXThreadStateGet(pxThread);

        PXBool quitLoop =
            (PXThreadStateFinished == threadState) ||
            (PXResultNoWorkToDo == pxResult);

        if(quitLoop)
        {
            break;
        }
    }

    PXEngineModuleSerialDeactivate(pxEngine);

    return PXResultOK;
}

void PXAPI PXEngineDrawSceneDefault(PXEngine PXREF pxEngine, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    PXWindow PXREF pxWindow = pxDrawInfo->Window;

    PXRectangleXYWHI32 spaceDraw;
    PXRectangleXYWHI32 spaceBox;
    PXRectangleXYWHI32 spaceContent;

    PXWindowSpaceDrawGet(pxWindow, &spaceDraw);
    PXWindowSpaceBoxGet(pxWindow, &spaceBox);
    PXWindowSpaceContentGet(pxWindow, &spaceContent);

    PXEngineScene PXREF pxEngineScene = pxEngine->Context.Scene;
    PXEngineRender PXREF pxEngineRender = pxEngine->Context.Render;
    PXBool isRendererAttached = pxEngineRender > 0;

    PXScene PXREF pxScene = PXEngineSceneSceneCurrent(pxEngineScene);
    PXBool canRender = isRendererAttached && pxScene;

    if(!canRender)
    {
        PXWindowDrawHelloWorldTriangle
        (
            pxWindow,
            spaceDraw.X,
            spaceDraw.Y,
            spaceDraw.Width,
            spaceDraw.Height
        );

        return;
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        PXColorRGB_F32 ClearColor = { 0.2, 0.2, 0.4 };

        glClearColor(ClearColor.Red, ClearColor.Green, ClearColor.Blue, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 1
        PXWindowDrawRect
        (
            pxWindow,
            spaceDraw.X,
            spaceDraw.Y,
            spaceDraw.Width,
            spaceDraw.Height
        );
#endif
    }

    pxDrawInfo->Scene = pxScene;
    pxDrawInfo->Camera = PXSceneCameraCurrent(pxScene);

    PXEngineRenderDrawScene(pxEngineRender, pxScene);
    
}

void PXAPI PXEngineUpdateMouse(PXEngine PXREF pxEngine, PXWindowEventMouseMoveData PXREF pxWindowEventInputMouseMove)
{
    PXScene PXREF pxScene = PXEngineSceneSceneCurrent(pxEngine->Context.Scene);
    PXCamera PXREF pxCamera = PXSceneCameraCurrent(pxScene);
    //PXEngineRender PXREF pxEngineRender = pxEngine->Renderer;
    //PXWindow* pxWindow = pxEngine->WindowMain;

    //pxWindowEventInputMouseMove

    PXVector3_F32 mouseInput;
    mouseInput.X = pxWindowEventInputMouseMove->Delta.X;
    mouseInput.Y = pxWindowEventInputMouseMove->Delta.Y;
    mouseInput.Z = 0.0f; // No roll/Z rotation input from a standard mouse

    // 2. Pass the input vector straight to your 4D-Rotor camera function
    PXCameraRotate(pxCamera, &mouseInput);
}

void PXAPI PXEngineUpdateKeyBoard(PXEngine PXREF pxEngine, PXWindowEventKeyboardData PXREF pxWindowEventInputKeyboard)
{
    PXScene PXREF pxScene = PXEngineSceneSceneCurrent(pxEngine->Context.Scene);
    PXCamera PXREF pxCamera = PXSceneCameraCurrent(pxScene);

    if(!pxCamera)
    {
        return;
    }


    // 1. Initialize a unified 4D movement accumulator vector
    PXVector4_F32 move = { 0.0f, 0.0f, 0.0f, 0.0f };

    // 2. Fetch the current directional vectors from the 4D camera rotor
    PXVector3_F32 forward;
    PXVector3_F32 right;
    PXVector3_F32 up = {0,1,0};
    PXCameraForward(pxCamera, &forward);
    PXCameraRight(pxCamera, &right);
    //PXCameraUp(pxCamera, &up);

    // Disable Y 
    right.Y = 0;
    forward.Y = 0;

    PXVector3Normalize_F32(&right);
    PXVector3Normalize_F32(&forward);


    // 3. Accumulate directional inputs into the 4D vector
    // Standard WASD / QE 3D space movement
    if(pxWindowEventInputKeyboard->CharacterW == L's') { move.X -= forward.X; move.Y -= forward.Y; move.Z -= forward.Z; }
    if(pxWindowEventInputKeyboard->CharacterW == L'w') { move.X += forward.X; move.Y += forward.Y; move.Z += forward.Z; }
    if(pxWindowEventInputKeyboard->CharacterW == L'a') { move.X -= right.X;   move.Y -= right.Y;   move.Z -= right.Z; }
    if(pxWindowEventInputKeyboard->CharacterW == L'd') { move.X += right.X;   move.Y += right.Y;   move.Z += right.Z; }
    if(pxWindowEventInputKeyboard->CharacterW == L'q') { move.X += up.X;      move.Y += up.Y;      move.Z += up.Z; }
    if(pxWindowEventInputKeyboard->CharacterW == L'e') { move.X -= up.X;      move.Y -= up.Y;      move.Z -= up.Z; }

    // 4D Ana / Kata movement along the raw W axis
    if(pxWindowEventInputKeyboard->CharacterW == L'o') { move.W -= 1.0f; }
    if(pxWindowEventInputKeyboard->CharacterW == L'p') { move.W += 1.0f; }

    // 4. Calculate the magnitude across all 4 spatial dimensions
    float lengthSq = (move.X * move.X) + (move.Y * move.Y) + (move.Z * move.Z) + (move.W * move.W);

    if(lengthSq > 0.0001f)
    {
        float invLength = 1.0f / sqrtf(lengthSq);

        // Normalize the 4D displacement vector
        move.X *= invLength;
        move.Y *= invLength;
        move.Z *= invLength;
        move.W *= invLength;

        // 5. Scale the 4D movement vector by velocity and your engine's delta time
        float dt = 1;// pxEngine->TimeData.CounterTimeDelta;
        float currentSpeed = pxCamera->WalkSpeed * dt * 0.00000005;

        move.X *= currentSpeed;
        move.Y *= currentSpeed;
        move.Z *= currentSpeed;
        move.W *= currentSpeed;

        // 6. Apply displacement to the 4D spatial transform position
        pxCamera->Transform.Position.X += move.X;
        pxCamera->Transform.Position.Y += move.Y;
        pxCamera->Transform.Position.Z += move.Z;
        pxCamera->Transform.Position.W += move.W;
               
    }

    // 7. Fire a unified single update command after processing all spatial input modifications
    PXCameraUpdate(pxCamera, 1); // pxEngine->TimeData.CounterTimeDelta

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Update",
        "CAM:\n"
        "POS: %8.2f %8.2f %8.2f\n"
        "ROT: %8.2f %8.2f %8.2f\n"
        "MOV: %8.2f %8.2f %8.2f",
        pxCamera->Position.X,
        pxCamera->Position.Y,
        pxCamera->Position.Z,
        pxCamera->CurrentRotation.X,
        pxCamera->CurrentRotation.Y,
        pxCamera->CurrentRotation.Z,
        move.X,
        move.Y,
        move.Z
    );
#endif


    // User input
    {

        PXPlayerMoveInfo pxPlayerMoveInfo;
        PXClear(PXPlayerMoveInfo, &pxPlayerMoveInfo);


        // Solve controller

#if 0
        if(pxEngine->CameraCurrent && pxEngine->ControllerSystem.DeviceListData)
        {
            PXController PXREF pxController = &pxEngine->ControllerSystem.DeviceListData[0];

            // pxEngine->CameraCurrent->WalkSpeed = 1;
            // pxEngine->CameraCurrent->ViewSpeed = 1;

           //  PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, pxController->AxisNormalised[0] * pxEngine->CameraCurrent->WalkSpeed, 0, pxController->AxisNormalised[1] * pxEngine->CameraCurrent->WalkSpeed);
           //  PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementView, pxController->AxisNormalised[2] * pxEngine->CameraCurrent->ViewSpeed, pxController->AxisNormalised[3] * pxEngine->CameraCurrent->ViewSpeed, 0);
            // Update
            const PXBool doUIUpdate = pxWindowEventInputKeyboard->VirtualKey == KeyU;

            if(doUIUpdate)
            {
                PXLogPrint
                (
                    PXLoggingWarning,
                    "PX",
                    "Update",
                    "Triggerd by user input"
                );


                pxEngine->UpdateUI = 1;
            }







#if 0

            if(keyboard->Commands & KeyBoardIDShiftLeft)
            {
                PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, -1, 0);
            }
            if(keyboard->Letters & KeyBoardIDLetterW)
            {
                PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 0, 1);
            }
            if(keyboard->Letters & KeyBoardIDLetterA)
            {
                PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, -1, 0, 0);
            }
            if(keyboard->Letters & KeyBoardIDLetterS)
            {
                PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 0, -1);
            }
            if(keyboard->Letters & KeyBoardIDLetterD)
            {
                PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, 1, 0, 0);
            }
            if(keyboard->Letters & KeyBoardIDSpace)
            {
                PXVector3F32AddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 1, 0);
            }
            if(keyboard->Letters & KeyBoardIDLetterF && !pxEngine->InteractionLock)
            {
                pxEngine->InteractionLock = PXTrue;
                PXFunctionInvoke(pxEngine->OnInteract, pxEngine->Owner, pxEngine);
            }
            if(!(keyboard->Letters & KeyBoardIDLetterF))
            {
                pxEngine->InteractionLock = PXFalse;
            }
#endif
        }

#endif
    }
}

PXI32U PXAPI PXEngineGenerateRandom(PXEngine PXREF pxEngine, const PXI32U limiter)
{
    return PXMathRandomeNumber(&pxEngine->RandomGeneratorSeed) % limiter;
}

PXResult PXAPI PXEngineCreateRender(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXEngineRenderCreateInfo pxEngineRenderCreateInfo;
    PXClear(PXEngineRenderCreateInfo, &pxEngineRenderCreateInfo);
    pxEngineRenderCreateInfo.Info.Engine = pxEngine;
    pxEngineRenderCreateInfo.Mode = PXGraphicInitializeModeWindowfull;
    pxEngineRenderCreateInfo.GraphicSystem = PXGraphicAPIOpenGL;
    pxEngineRenderCreateInfo.WindowReference = PXEngineUIWindowRootGet(pxEngine->Context.UI);
    pxEngineRenderCreateInfo.Width = -1;
    pxEngineRenderCreateInfo.Height = -1;
    pxEngineRenderCreateInfo.DirectXVersion = PXDirectXVersionNewest;
    pxEngineRenderCreateInfo.DirectXDriverType = PXDirectXDriverTypeHardwareDevice;

    PXResult pxResult = PXEngineRenderCreate(&pxEngine->Context.Render, &pxEngineRenderCreateInfo);

    PXEngineRender PXREF pxEngineRender = pxEngine->Context.Render;
   
    PXEngineModuleRegister(pxEngine, pxEngineRender);

    return pxResult;
}

PXResult PXAPI PXEngineCreateAudio(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXResult pxResult = PXResultInvalid;

    PXEngineAudioCreateInfo pxEngineAudioCreateInfo;
    PXClear(PXEngineAudioCreateInfo, &pxEngineAudioCreateInfo);
    pxEngineAudioCreateInfo.Info.Engine = pxEngine;
    pxEngineAudioCreateInfo.AudioInitializeIndexTarget = 1;
    pxEngineAudioCreateInfo.AudioInitializeList = _pxAudioInitializeList;
    pxEngineAudioCreateInfo.AudioInitializeListAmount = _pxAudioInitializeListAmount;

    pxResult = PXEngineAudioCreate(&pxEngine->Context.Audio, &pxEngineAudioCreateInfo);

    if(PXResultOK != pxResult)
    {
        // HANDLE
    }

    PXEngineAudio PXREF pxEngineAudio = pxEngine->Context.Audio;

    PXEngineModuleRegister(pxEngine, pxEngineAudio);
    
    pxEngineAudio->DeviceInputAmount(pxEngineAudio);
    pxEngineAudio->DeviceOutputAmount(pxEngineAudio);

    return pxResult;
}

PXResult PXAPI PXEngineCreateInput(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXResult pxResult = PXResultInvalid;

    PXEngineInputCreateInfo pxEngineInputCreateInfo;
    PXClear(PXEngineInputCreateInfo, &pxEngineInputCreateInfo);
    pxEngineInputCreateInfo.Info.Engine = pxEngine;
    // No settings?

    pxResult = PXEngineInputCreate(&pxEngine->Context.Input, &pxEngineInputCreateInfo);

    PXEngineInput PXREF pxEngineInput = pxEngine->Context.Input;

    PXEngineModuleRegister(pxEngine, pxEngineInput);

    return pxResult;
}

PXResult PXAPI PXEngineCreateScene(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXResult pxResult = PXResultInvalid;

    PXEngineSceneCreateInfo pxEngineSceneCreateInfo;
    PXClear(PXEngineSceneCreateInfo, &pxEngineSceneCreateInfo);
    pxEngineSceneCreateInfo.Info.Engine = pxEngine;
    // No settings?

    pxResult = PXEngineSceneCreate(&pxEngine->Context.Scene, &pxEngineSceneCreateInfo);

    PXEngineScene PXREF pxEngineScene = pxEngine->Context.Scene;
    pxEngineScene->RenderSystem = pxEngine->Context.Render;

    PXEngineModuleRegister(pxEngine, pxEngineScene);

    return pxResult;
}

PXResult PXAPI PXEngineCreateUI(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXResult pxResult = PXResultInvalid;


    PXEngineUIInfo pxUIInfo;
    PXClear(PXEngineUIInfo, &pxUIInfo);
    pxUIInfo.Info.Engine = pxEngine;
    //PXTextFromAddressA(&pxUIInfo.WindowRootName, "pxMainWindow", 13, 13);

    pxResult = PXEngineUICreate(&pxEngine->Context.UI, &pxUIInfo);

    PXEngineUI PXREF pxEngineUI = pxEngine->Context.UI;

    pxResult = PXEngineModuleRegister(pxEngine, pxEngineUI);




    PXWindow* pxWindowRoot = PXNull;

    PXWindowCreateInfo pxWindowCreateInfo;
    PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
    pxWindowCreateInfo.Info.Type = PXResourceTypeGUIElement;
    PXTextFromAddressA(&pxWindowCreateInfo.Info.Name, "pxMainWindow", 13, 13);
    pxWindowCreateInfo.Type = PXEngineUIElementTypeWindow;
    pxWindowCreateInfo.Setting = PXWindowBehaviourBorder;
    pxWindowCreateInfo.SkipHandleCreation = PXTrue;
    pxWindowCreateInfo.EventList.CallBackOwner = pxEngine;
    //pxWindowCreateInfo.EventList.CallBackDraw = PXEngineEventDraw; // This is only drawing the background!
    pxWindowCreateInfo.EventList.CallBackEvent = PXEngineWindowEvent;
    //pxWindowCreateInfo.Layout.abs.Width = pxEngineStartInfo->Width;
    //pxWindowCreateInfo.Layout.abs.Height = pxEngineStartInfo->Height;
    pxWindowCreateInfo.WindowParent = pxEngineStartInfo->WindowRenderParent;
    pxWindowCreateInfo.EngineUI = pxEngineUI;
    pxWindowCreateInfo.BackGroundColor.Red = 0x20;
    pxWindowCreateInfo.BackGroundColor.Green = 0x20;
    pxWindowCreateInfo.BackGroundColor.Blue = 0x20;

    pxResult = PXWindowCreate(&pxWindowRoot, &pxWindowCreateInfo);

    //pxResourceCreateInfo.UIElement.Data.Window.EventOwner = pxEngine;
    //pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Red = 38;
    //pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Green = 38;
    //pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Blue = 38;
    //pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Alpha = 0xFF;

    pxResult = PXWindowMouseMovementEnable(pxWindowRoot);

    //PXCameraAspectRatioChange(pxEngine->CameraCurrent, pxEngineStartInfo->Width, pxEngineStartInfo->Height);

#if OSUnix
#elif OSWindows && 0
    InitDockContainerStyles(pxMainWindow);

    LayoutDockedEx(pxMainWindow, NULL);
#endif

    PXWindowHide(pxWindowRoot);
    PXWindowShow(pxWindowRoot);

    PXEngineUIWindowRootSet(pxEngineUI, pxWindowRoot);


    return pxResult;
}

PXResult PXAPI PXEngineCreateTime(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXEngineTimeCreateInfo pxEngineTimeCreateInfo;
    PXClear(PXEngineTimeCreateInfo, &pxEngineTimeCreateInfo);
    pxEngineTimeCreateInfo.Info.Engine = pxEngine;
    pxEngineTimeCreateInfo.RefreshHz = 60;

    PXResult pxResult = PXEngineTimeCreate(&pxEngine->Context.Time, &pxEngineTimeCreateInfo);

    PXEngineTime PXREF pxEngineTime = pxEngine->Context.Time;

    pxResult = PXEngineModuleRegister(pxEngine, pxEngineTime);

    return pxResult;
}

PXResult PXAPI PXEngineCreateMath(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXEngineMathCreateInfo pxEngineMathCreateInfo;
    PXClear(PXEngineTimeCreateInfo, &pxEngineMathCreateInfo);

    PXResult pxResult = PXEngineMathCreate(&pxEngine->Context.Math, &pxEngineMathCreateInfo);

    return pxResult;
}

PXResult PXAPI PXEngineCreateModManager(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo)
{
    PXEngineModManagerCreateInfo pxEngineModManagerCreateInfo;
    PXClear(PXEngineModManagerCreateInfo, &pxEngineModManagerCreateInfo);
    PXTextFromAddressA(&pxEngineModManagerCreateInfo.FilePathModFolder, "Mod/", 4, 4);
    pxEngineModManagerCreateInfo.AvoidLoadingMods = PXTrue;

    PXResult pxResult = PXEngineModManagerCreate(&pxEngine->Context.ModManager, &pxEngineModManagerCreateInfo);

    return pxResult;
}
PXResult PXAPI PXEngineStart(PXEngine PXREF pxEngine)
{
    if(!(pxEngine))
    {
        return PXResultRefusedParameterNull;
    }

    //   pxEngine->CameraCurrent = &pxEngine->CameraDefault;
    //pxEngine->TimeData.CounterTimeLast = 0;
    //pxEngine->TimeData.CounterTimeGPU = 0;
    //pxEngine->TimeData.CounterTimeCPU = 0;
    //pxEngine->TimeData.FramesPerSecound = 0;
    //pxEngine->TimeData.FrameTime = 0;
    pxEngine->IsRunning = PXFalse;
    pxEngine->HasGraphicInterface = PXFalse;

    // pxEngine->CameraCurrent->ViewSpeed = 1;
    //PXCameraRotateXYZ(pxEngine->CameraCurrent, 80, 0, 0);

#if PXLogEnable
    // Create Banner
    {
        char stampBuffer[256];

        PXTextPrintA(stampBuffer, 256, "%s - %s", __DATE__, __TIME__);

        PXConsoleWriteF
        (
            0,
            "\n"
            "+++-----------------------------------------------------+++\n"
            "||| %51s |||\n"
            "+++-----------------------------------------------------+++\n"
            "|/| __________ .__   __  __________.__                  |/|\n"
            "|/| \\______   \\|__|_/  |_\\_  _____/|__|_______   ____   |/|\n"
            "|/|  |    |  _/|  |\\   __\\|  __)   |  |\\_  __ \\_/ __ \\  |/|\n"
            "|/|  |    |   \\|  | |  |  |  |     |  | |  | \\/\\  ___/  |/|\n"
            "|/|  |________/|__| |__|  \\__|     |__| |__|    \\_____> |/|\n"
            "|/|_____________________________________________________|/|\n"
            "+-+-----------------------------------------------------+-+\n\n",
            stampBuffer
        );
    }

    // Processor
    {
        PXProcessor processor;

        PXProcessorFetchInfo(&processor);

        PXConsoleWriteF
        (
            0,
            "+---------------------------------------------------------+\n"
            "| Processor - Information                                 |\n"
            "+---------------------------------------------------------+\n"
            "| BrandName : %-43s |\n"
            "| Identity  : %-43s |\n"
            "| Cores     : %-43i |\n"
            "| Family    : %-43i |\n"
            "| Model     : %-43i |\n"
            "+---------------------------------------------------------+\n\n",
            processor.BrandName,
            processor.IdentityString,
            processor.NumberOfProcessors,
            processor.Family,
            processor.Model
        );
    }
#endif

    PXMathRandomeSeed(&pxEngine->RandomGeneratorSeed);

    // After creating all systems in a liniar way, we now can let our 
    // system handle ASYNC work. 

    pxEngine->IsRunning = PXTrue;

    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineText,
            "Start",
            "Engine is up and running. Invoking callback for extended load."
        );
#endif

        if(pxEngine->OnStartUp) 
        {
            pxEngine->OnStartUp(pxEngine->Owner, pxEngine);
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineText,
        "Start",
        "----------------DONE----------------"
    );
#endif

    PXThreadResume(pxEngine->ThreadConductor);

    return PXResultOK;
}

PXResult PXAPI PXEngineStop(PXEngine PXREF pxEngine)
{
    if(!pxEngine)
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxEngine->IsRunning)
    {
        return; // Engine is not running, we dont release
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineText,
        "Shutdown",
        "All running systems are being closed down"
    );
#endif


    //-----------------------------------------------------
    // Activate modules
    const PXSize moduleAmount = PXEngineModuleAmount(pxEngine);

    for(PXSize i = 0; i < moduleAmount; ++i)
    {
        PXEngineModule PXREF pxEngineModule = PXEngineModuleIndex(pxEngine, i);

        PXEngineModuleDeactivate(pxEngineModule, pxEngine);
    }
    //-----------------------------------------------------

    pxEngine->IsRunning = PXFalse;

    if(pxEngine->OnShutDown)
    {
        pxEngine->OnShutDown(pxEngine->Owner, pxEngine);
    }
}

PXResult PXAPI PXEngineEventOnTick(PXEngine PXREF pxEngine)
{
    PXEngineUpdateInfo pxEngineUpdateInfo;
    PXClear(PXEngineUpdateInfo, &pxEngineUpdateInfo);
    pxEngineUpdateInfo.Engine = pxEngine;

    //PXResult pxResult = PXEnginePhysicsUpdate(pxEngine->Ph, &pxEngineUpdateInfo);

    PXResult pxResult = PXResultNoWorkToDo;

    return pxResult;
}

PXResult PXAPI PXEngineEventOnRender(PXEngine PXREF pxEngine)
{
    PXEngineUpdateInfo pxEngineUpdateInfo;
    PXClear(PXEngineUpdateInfo, &pxEngineUpdateInfo);
    pxEngineUpdateInfo.Engine = pxEngine;

    PXEngineRender PXREF pxEngineRender = pxEngine->Context.Render;

    if(!pxEngineRender->ModuleInfo.IsActive)
    {
        return PXResultNoWorkToDo;
    }   

    PXResult pxResult = PXEngineModuleUpdateASYNC(pxEngine->Context.Render, &pxEngineUpdateInfo);

    return pxResult;
}

PXResult PXAPI PXEngineEventOnUI(PXEngine PXREF pxEngine)
{    
    if(!pxEngine)
    {
        return PXResultRefusedParameterNull;
    }

    PXEngineUpdateInfo pxEngineUpdateInfo;
    PXClear(PXEngineUpdateInfo, &pxEngineUpdateInfo);
    pxEngineUpdateInfo.Engine = pxEngine;

    //PXResult pxResult = PXEngineModuleUpdateSerial(pxEngine->UI, &pxEngineUpdateInfo);

    const PXSize moduleAmount = PXEngineModuleAmount(pxEngine);

    for(PXSize i = 0; i < moduleAmount; i++)
    {
        PXEngineModule PXREF pxEngineModule = PXEngineModuleIndex(pxEngine, i);

        PXEngineModuleUpdateSerial(pxEngineModule, &pxEngineUpdateInfo);
    }

    //pxEngine->TimeData.CounterTimeCPU = PXTimeStampNow();
    //PXFunctionInvoke(pxEngine->OnGameUpdate, pxEngine->Owner, pxEngine);
    //pxEngine->TimeData.CounterTimeCPU = PXTimeStampNow() - pxEngine->TimeData.CounterTimeCPU;


    return PXResultOK;;
}

PXSize PXAPI PXEngineModuleAmount(const PXEngine PXREF pxEngine)
{
    if(!pxEngine)
    {
        return 0;
    }

    return PXDictionaryECSAmount(&pxEngine->Context.SystemLookup);
}

PXEngineModule* PXAPI PXEngineModuleIndex(PXEngine PXREF pxEngine, const PXSize index)
{
    if(!pxEngine)
    {
        return PXNull;
    }

    return (PXEngineModule*)PXDictionaryECSGetViaIndex(&pxEngine->Context.SystemLookup, index);;
}

PXResult PXAPI PXEngineModuleRegister
(
    PXEngine PXREF pxEngine,
    PXEngineModule PXREF pxEngineModule
)
{
    PXResult pxResult = PXDictionaryECSAdd(&pxEngine->Context.SystemLookup, pxEngineModule);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    return pxResult;
}

PXResult PXAPI PXEngineModuleRelease(PXEngine PXREF pxEngine)
{
    return PXResultOK;
}

PXEngineContext* PXAPI PXEngineContextGet(const PXEngine PXREF pxEngine)
{
    if(!pxEngine)
    {
        return PXNull;
    }

    return &pxEngine->Context;
}

PXResult PXAPI PXEngineEventUIAviableSet(PXEngine PXREF pxEngine)
{
    pxEngine->EventUIAviable = PXTrue;

    return PXResultOK;
}

PXResult PXAPI PXEngineEventThreadDoneSet(PXEngine PXREF pxEngine)
{
    pxEngine->EventEngineThreadDone = PXTrue;

    return PXResultOK;
}



/*


PXResult PXAPI PXGraphicLoadImage(PXGraphic PXREF pxGraphic, PXTexture2D PXREF pxTexture2D, const PXText PXREF PXTextureFilePath)
{
    // Create checksum
    const PXI32U checkSum = PXAdler32Create(1, PXTextureFilePath->A, PXTextureFilePath->SizeUsed);


    // Check if already loaded
    {
        PXTexture2D* PXTextureFoundEntry = PXNull;

        const PXBool foundEntry = PXDictionaryFindEntry(&pxGraphic->ResourceImageLookUp, &checkSum, &PXTextureFoundEntry);

        if (foundEntry) // image is already loaded
        {
            PXTextureCopyAsIs(PXTexture, PXTextureFoundEntry);

            PXLogPrint
            (
                PXLoggingWarning,
                "Graphic",
                "Texture",
                "Load skipped <%s> (Redundant)",
                PXTextureFilePath->A
            );

            return PXResultOK;
        }
    }

    // Load texture
    {
        const PXResult loadResult = PXResourceLoad(PXTexture, PXTextureFilePath);

        if (PXResultOK != loadResult)
        {
            PXLogPrint
            (
                PXLoggingError,
                "Graphic",
                "Texture",
                "Texture load failed <%s>!",
                PXTextureFilePath->A
            );
        }

        PXResultReturnOnError(loadResult);

        PXLogPrint
        (
            PXLoggingInfo,
            "Graphic",
            "Texture",
            "Texture load successful <%s>.",
            PXTextureFilePath->A
        );

        PXDictionaryEntryAdd(&pxGraphic->ResourceImageLookUp, &checkSum, PXTexture);
    }

    return PXResultOK;
}


*/

PXResult PXAPI PXEngineResourceCreate(PXEngine PXREF pxEngine, PXECSCreateInfo PXREF pxResourceCreateInfo)
{
    if(!(pxEngine && pxResourceCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    // Primary load
    {
        const PXResult resourceAddResult = PXResultInvalid;// //PXResourceManagerAdd(pxResourceCreateInfo);
        const PXBool success = PXResultOK == resourceAddResult;

        if(!success)
        {
            return resourceAddResult;
        }
    }

    //assert(pxResourceCreateInfo->ObjectReference);
    //   assert(*pxResourceCreateInfo->ObjectReference);
    /*
    switch(pxResourceCreateInfo->Type)
    {
        case PXResourceTypeTexture:
        {

            break;
        }
        case PXResourceTypeTextureCube:
        {
            PXTextureCreateInfo PXREF pxTextureCreateData = 0;//& pxResourceCreateInfo->Texture;
            PXTexture2D* pxTexture = *(PXTexture2D**)pxResourceCreateInfo->ObjectReference;

            assert(pxTexture);

            if(!pxEngine->Graphic.TextureAction)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXEngineText,
                    "TextureCube-Create",
                    "Not possible"
                );
#endif

                return PXResultFailedNoSystemToHandleData;
            }

            PXTextureInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = &pxTexture;
            pxGraphicTexturInfo.Amount = 1u;
            pxGraphicTexturInfo.Type = PXTextureTypeCube;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;

            pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);

            break;
        }
        case PXResourceTypeModel:
        {
            PXModelCreateInfo PXREF pxModelCreateInfo = 0;// &pxResourceCreateInfo->Model;
            PXModel* pxModel = *(PXModel**)pxResourceCreateInfo->ObjectReference;

            assert(pxModel);

            if(!pxEngine->Graphic.ModelRegister)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXEngineText,
                    "Model-Create",
                    "Not possible"
                );
#endif

                return PXResultFailedNoSystemToHandleData;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXEngineText,
                "Model-Register",
                "ID:%i, 0x%p",
                0,//pxModel->Info.ID,
                pxModel
            );
#endif
            PXBool hasASYNCChildren = PXFalse;

            if(!hasASYNCChildren)
            {
                pxEngine->Graphic.ModelRegister(pxEngine->Graphic.EventOwner, pxModel);
            }
            else
            {
                PXThreadPoolQueueWork
                (
                    PXNull,
                    0,
                    pxEngine->Graphic.ModelRegister,
                    pxEngine->Graphic.EventOwner,
                    pxModel,
                    PXTaskExecuteSYNC | PXTaskDepended
                );
            }

            //  pxModel->Info.Behaviour |= PXECSInfoRender | PXECSInfoActive;

            break;
        }
        case PXResourceTypeTexture2D:
        {
            //PXEngineTexture2DCreateData PXREF pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;
            PXTexture2D* pxTexture = *(PXTexture2D**)pxResourceCreateInfo->ObjectReference;

            assert(pxTexture);

            if(!pxEngine->Graphic.TextureAction)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXEngineText,
                    "Texture-Create",
                    "Not possible",
                    pxResourceCreateInfo->FilePath.A
                );
#endif

                return PXResultFailedNoSystemToHandleData;
            }


            PXTextureInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = &pxTexture;
            pxGraphicTexturInfo.Amount = 1u;
            pxGraphicTexturInfo.Type = PXTextureType2D;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;

            pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);

            break;
        }
        case PXResourceTypeFont:
        {
            // PXEngineFontCreateInfo PXREF pxEngineFontCreateData = &pxResourceCreateInfo->Font;
            PXFont* pxFont = *(PXFont**)pxResourceCreateInfo->ObjectReference;

            // Register as normal
            {
                /*
                // Load Textures
                {
                    PXFontPage PXREF pxFontPage = &pxFont->;

                    PXResourceCreateInfo pxResourceCreateInfoSub;
                    PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);

                    pxResourceCreateInfoSub.Type = PXResourceTypeTexture2D;
                    pxResourceCreateInfoSub.ObjectReference = (void**)&pxFontPage->Texture;
                   // pxResourceCreateInfoSub.FilePath = pxFontPage->TextureFilePath;

                    PXEngineResourceCreate(pxEngine, &pxResourceCreateInfoSub);
                }

                for (PXSize i = 0; i < pxFont->AdditionalPageListSize; ++i)
                {
                    PXFontPage PXREF pxFontPage = &pxFont->AdditionalPageList[i];

                    PXTexturInfo pxGraphicTexturInfo;
                    pxGraphicTexturInfo.TextureReference = pxFontPage->Texture;
                    pxGraphicTexturInfo.Amount = 1u;
                    pxGraphicTexturInfo.Type = PXTextureType2D;
                    pxGraphicTexturInfo.Action = PXResourceActionCreate;

                    pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                }* /
            }

            break;
        }
        case PXResourceTypeSkybox:
        {
            //   PXSkyBoxCreateInfo PXREF pxSkyBoxCreateEventData = &pxResourceCreateInfo->SkyBox;
            PXSkyBox* pxSkyBox = *(PXSkyBox**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.ModelRegister)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXEngineText,
                    "SkyBox-Create",
                    "Not possible"
                );
#endif

                return PXResultFailedNoSystemToHandleData;
            }



            // Model create
            {
                PXECSCreateInfo pxResourceCreateInfo;
                PXClear(PXECSCreateInfo, &pxResourceCreateInfo);
                pxResourceCreateInfo.Type = PXResourceTypeModel;
                pxResourceCreateInfo.ObjectReference = (PXECSInfo**)&pxSkyBox->Model;
                //  pxResourceCreateInfo.Model.Form = PXModelFormCube;

                PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);
            }

            // Shader create
            {
                PXShader pxShader[2];
                PXClearList(PXShader, pxShader, 2);
                pxShader[0].Info.ID = PXIDGenerate();
                pxShader[0].Type = PXShaderTypeVertex;
                // pxShader[0].ShaderFile = pxSkyBoxCreateEventData->ShaderProgram.ShaderVertexFile;

                pxShader[1].Info.ID = PXIDGenerate();
                pxShader[1].Type = PXShaderTypePixel;
                //pxShader[1].ShaderFile = pxSkyBoxCreateEventData->ShaderProgram.ShaderPixelFile;

                pxEngine->Graphic.ShaderProgramCreate
                (
                    pxEngine->Graphic.EventOwner,
                    pxSkyBox->ShaderProgramReference,
                    pxShader,
                    2
                );
            }

            // Regsieter texture
            {
                PXTextureInfo* pxGraphicTexturInfo = PXMemoryHeapCallocT(PXTextureInfo, 1);
                pxGraphicTexturInfo->TextureReference = &pxSkyBox->TextureCube;
                pxGraphicTexturInfo->Amount = 1u;
                pxGraphicTexturInfo->Type = PXTextureTypeCube;
                pxGraphicTexturInfo->Action = PXResourceActionCreate;

                PXThreadPoolQueueWork
                (
                    PXNull,
                    0,
                    pxEngine->Graphic.TextureAction,
                    pxEngine->Graphic.EventOwner,
                    pxGraphicTexturInfo,
                    PXTaskExecuteSYNC | PXTaskParameterRelease | PXTaskDepended
                );
            }

            PXECSInfoFlagStateAdd(&pxSkyBox->Info, PXECSFlagBehaviourCanRender);

            break;
        }
        case PXResourceTypeSprite:
        {
            //  PXSpriteCreateInfo PXREF pxSpriteCreateEventData = &pxResourceCreateInfo->Sprite;
            PXSprite* pxSprite = *(PXSprite**)pxResourceCreateInfo->ObjectReference;

            // pxSprite->ShaderProgarm = pxSpriteCreateEventData->ShaderProgramCurrent;

             // Load texture & sprite model
            {
                PXECSCreateInfo pxResourceCreateInfoSub[3];
                PXClearList(PXECSCreateInfo, &pxResourceCreateInfoSub, 3);
                PXSize amount = 2;

                pxResourceCreateInfoSub[0].Type = PXResourceTypeTexture2D;
                pxResourceCreateInfoSub[0].ObjectReference = (PXECSInfo**)&pxSprite->Texture;
                pxResourceCreateInfoSub[0].FilePath = pxResourceCreateInfo->FilePath;

                pxResourceCreateInfoSub[1].Type = PXResourceTypeModel;
                pxResourceCreateInfoSub[1].ObjectReference = (PXECSInfo**)&pxSprite->Model;
                //    pxResourceCreateInfoSub[1].Model.Form = PXModelFormRectangleTX;

                    // Add hibox if needed
                  //  if(pxSpriteCreateEventData->HitboxBehaviour > 0)
                {
                    pxResourceCreateInfoSub[2].Type = PXResourceTypeHitBox;
                    pxResourceCreateInfoSub[2].ObjectReference = (PXECSInfo**)&pxSprite->HitBox;
                    // pxResourceCreateInfoSub[2].HitBox.Behaviour = 0;
                  //   pxResourceCreateInfoSub[2].HitBox.Model = pxSprite->Model;

                     // pxResourceCreateInfo->HitBox.HitBox = pxSprite->HitBox;

                    amount++;
                }

                PXEngineResourceCreate(pxEngine, &pxResourceCreateInfoSub[0]);
                PXEngineResourceCreate(pxEngine, &pxResourceCreateInfoSub[1]);
            }

            PXMaterial* pxMaterial = PXMemoryHeapCallocT(PXMaterial, 1);

            pxMaterial->DiffuseTexture = pxSprite->Texture;

            pxSprite->Material = pxMaterial;
            //    pxSprite->Model->ShaderProgramReference = pxSpriteCreateEventData->ShaderProgramCurrent;


            PXF32 txWidth = pxSprite->Texture->Width;
            PXF32 txHeight = pxSprite->Texture->Height;

            PXVector2F32 aspectScaling = { 1, txHeight / txWidth };

#if 0
            PXMatrix4x4FMove3F(&pxSprite->Model->ModelMatrix, &pxSpriteCreateEventData->Position);
            PXMatrix4x4FScaleSetXY
            (
                &pxSprite->Model->ModelMatrix,
                aspectScaling.X * pxSpriteCreateEventData->Scaling.X,
                aspectScaling.Y * pxSpriteCreateEventData->Scaling.Y
            );

            if(pxSpriteCreateEventData->HitboxBehaviour > 0)
            {
                pxSprite->HitBox->Model = pxSprite->Model;
            }
#endif

            break;
        }
        case PXResourceTypeText:
        {
            //PXEngineTextCreateData PXREF pxEngineTextCreateData = &pxEngineResourceCreateInfo->Text;
            //PXEngineTextEEQ* pxEngineText = *(PXEngineTextEEQ**)pxResourceCreateInfo->ObjectReference;

            break;
        }
        case PXResourceTypeIconAtlas:
        {
            break;
        }
        case PXResourceTypeTimer:
        {
            PXTimer* pxEngineTimer = *(PXTimer**)pxResourceCreateInfo->ObjectReference;


            break;
        }
        case PXResourceTypeSound:
        {
            PXSound* pxSound = *(PXSound**)pxResourceCreateInfo->ObjectReference;


            break;
        }
        case PXResourceTypeEngineSound:
        {
            /*
            PXEngineSoundCreateInfo PXREF pxEngineSoundCreateInfo = &pxResourceCreateInfo->Sound;
            PXEngineSound* pxEngineSound = *(PXEngineSound**)pxResourceCreateInfo->ObjectReference;

            {
                const PXResult pxResult = pxEngine->Audio.DeviceLoad(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice, pxEngineSound->Sound);

#if PXLogEnable
                if(PXResultOK == pxResult)
                {

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "PX",
                        "Sound-Upload",
                        "Success <%s>!",
                        pxResourceCreateInfo->FilePath
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
                        pxResourceCreateInfo->FilePath
                    );
                }
#endif

            }
            * /


            break;
        }
        case PXResourceTypeGUIElement:
        {
            //  PXWindowCreateInfo PXREF pxWindowCreateInfo = &pxResourceCreateInfo->UIElement;



              // pxWindowCreateInfo->U

              //pxUIElementCreateData.UIElement.Data.SceneRender.StartInfo->UseMouseInput
             // pxWindowCreateInfo->Name = pxResourceCreateInfo->NameAddress;

            //  if(pxEngine->Window)
            //  {
            //      pxWindowCreateInfo->WindowCurrent = pxEngine->Window;
           //   }

            //  PXWindowCreate(&pxEngine->GUISystem, pxResourceCreateInfo, 1);

             // PXFunctionInvoke(pxEngine->ResourceAdded, pxEngine->Owner, pxEngine, pxResourceCreateInfo);

             // PXWindow* pxWindow = *(PXWindow**)pxResourceCreateInfo->ObjectReference;

              //pxWindow->NameContent = pxResourceCreateInfo->NameAddress;
              //pxWindow->NameContentSize = PXTextLengthA(pxResourceCreateInfo->NameAddress, 128);

            break;
        }
        case PXResourceTypeShaderProgram:
        {
            // PXShaderProgramCreateInfo PXREF pxShaderProgramCreateData = &pxResourceCreateInfo->ShaderProgram;
            PXShaderProgram* pxShaderProgram = *(PXShaderProgram**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.ShaderProgramCreate)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXEngineText,
                    "Shader-Create",
                    "Not possible"
                );
#endif

                return PXResultFailedNoSystemToHandleData;
            }

            {
                PXShader pxShader[2];
                PXClearList(PXShader, pxShader, 2);
                pxShader[0].Type = PXShaderTypeVertex;
                // pxShader[0].ShaderFile = pxShaderProgramCreateData->ShaderVertexFile;

                pxShader[1].Type = PXShaderTypePixel;
                //  pxShader[1].ShaderFile = pxShaderProgramCreateData->ShaderPixelFile;

                pxEngine->Graphic.ShaderProgramCreate
                (
                    pxEngine->Graphic.EventOwner,
                    pxShaderProgram,
                    pxShader,
                    2
                );
            }

            break;
        }
        default:
            return PXResultRefusedParameterInvalid;
    }
    */
    return PXResultOK;
}

void PXAPI PXEngineResourceDefaultElements(PXEngine PXREF pxEngine)
{
    PXECSCreateInfo pxResourceCreateInfo;

    // Create shader for model
    {
        // TODO: will only work for opengl
        char vertexShaderData[] =
            "#version 300 es\n"
            "layout(location = 0) in vec3 InputVertexPosition;\n"
            "out struct Vertex\n"
            "{\n"
            "  vec3 Position;\n"
            "  vec2 Texture;\n"
            "}\n"
            "vertex;\n"
            "uniform mat4 MatrixModel;\n"
            "uniform mat4 MatrixView;\n"
            "uniform mat4 MatrixProjection;\n"
            "void main()\n"
            "{\n"
            "    vec4 position4x = vec4(InputVertexPosition, 1.0f);\n"
            "    vec4 matrixModelViewProjection = MatrixProjection * MatrixView * MatrixModel * position4x;\n"
            "    vertex.Position = vec3(MatrixModel * position4x);\n"
            "    vertex.Texture =  vertex.Position.yz * 0.05f;\n"
            "    gl_Position = matrixModelViewProjection;\n"
            "}";

        char pixelShaderData[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "layout(location = 0) out vec4 fragcolor;\n"
            "in struct Vertex\n"
            "{\n"
            "  vec3 Position;\n"
            "  vec2 Texture;\n"
            "}\n"
            "vertex;\n"
            "uniform sampler2D MaterialTexture;\n"
            "void main()\n"
            "{\n"
            "   vec4 texturedColor = texture(MaterialTexture, vertex.Texture);\n"
            // "   fragcolor = vec4(0.64f, 0.02f, 0.98f, 0.35f);\n"
            "   fragcolor = vec4(texturedColor.xyz, 1.00f);\n"
            "}";

        PXClear(PXECSCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.Type = PXResourceTypeShaderProgram;
        // pxResourceCreateInfo.ObjectReference = &pxEngine->ResourceManager->ShaderFailback;
         //pxResourceCreateInfo.ShaderProgram.ShaderVertex.A = vertexShaderData;
         //pxResourceCreateInfo.ShaderProgram.ShaderVertex.SizeUsed = sizeof(vertexShaderData) - 1;
         //pxResourceCreateInfo.ShaderProgram.ShaderPixel.A = pixelShaderData;
         //pxResourceCreateInfo.ShaderProgram.ShaderPixel.SizeUsed = sizeof(pixelShaderData) - 1;

        PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);
    }

    // Create missing object model
    {
        const PXF32 vertexData[] =
        {
            -1.0f, 9.0f, -3.0f,
                1.0f, 7.0f, -5.0f,
                1.0f, 9.0f, -3.0f,
                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 3.0f, 1.0f,
                -1.0f, 3.0f, -1.0f,
                1.0f, 3.0f, 1.0f,
                1.0f, 3.0f, -1.0f,
                -1.0f, 9.0f, 1.0f,
                -1.0f, 7.0f, -1.0f,
                1.0f, 9.0f, 1.0f,
                1.0f, 7.0f, -1.0f,
                -1.0f, 15.0f, -3.0f,
                -1.0f, 17.0f, -5.0f,
                1.0f, 15.0f, -3.0f,
                1.0f, 17.0f, -5.0f,
                -1.0f, 7.0f, -5.0f,
                -1.0f, 9.0f, -3.0f,
                -1.0f, 7.0f, -5.0f,
                1.0f, 9.0f, -3.0f,
                1.0f, 7.0f, -5.0f,
                1.0f, 7.0f, -5.0f,
                1.0f, 9.0f, -3.0f,
                -1.0f, 17.0f, 5.0f,
                -1.0f, 15.0f, 3.0f,
                1.0f, 17.0f, 5.0f,
                1.0f, 15.0f, 3.0f,
                -1.0f, 9.0f, -3.0f,
                1.0f, 9.0f, -3.0f,
                -1.0f, 15.0f, -3.0f,
                -1.0f, 15.0f, -3.0f,
                1.0f, 15.0f, -3.0f,
                1.0f, 17.0f, -5.0f,
                -1.0f, 15.0f, -3.0f,
                1.0f, 15.0f, -3.0f,
                1.0f, 17.0f, -5.0f,
                -1.0f, 15.0f, 3.0f,
                1.0f, 15.0f, 3.0f,
                -1.0f, 15.0f, 3.0f,
                1.0f, 15.0f, 3.0f,
                -1.0f, 9.0f, 5.0f,
                -1.0f, 9.0f, 3.0f,
                1.0f, 9.0f, 5.0f,
                1.0f, 9.0f, 3.0f
        };

        PXI8U indexList[] =
        {
            17,1,24,
            3,19,2,
            2,17,24,
            1,18,3,
            5,6,4,
            7,10,6,
            11,8,10,
            9,4,8,
            10,4,6,
            7,9,11,
            12,17,13,
            17,15,13,
            19,14,15,
            18,12,14,
            15,12,13,
            46,47,32,
            21,43,40,
            40,43,22,
            23,21,43,
            24,25,26,
            28,27,2,
            26,2,24,
            37,26,1,
            43,30,29,
            39,1,30,
            35,1,26,
            40,43,23,
            29,30,36,
            36,1,35,
            37,38,21,
            43,40,39,
            39,38,37,
            31,41,46,
            43,47,42,
            42,46,41,
            21,33,43,
            44,31,32,
            47,33,45,
            32,45,44,
            31,46,32,
            32,47,34,
            34,47,33,
            48,32,49,
            32,51,49,
            34,50,51,
            33,48,50,
            51,48,49,
            26,43,29,
            28,26,29,
            17,16,1,
            3,18,19,
            2,19,17,
            1,16,18,
            5,7,6,
            7,11,10,
            11,9,8,
            9,5,4,
            10,8,4,
            7,5,9,
            12,16,17,
            17,19,15,
            19,18,14,
            18,16,12,
            15,14,12,
            40,23,43,
            28,29,27,
            26,28,2,
            37,21,26,
            43,39,30,
            39,37,1,
            36,30,1,
            31,21,41,
            43,33,47,
            42,47,46,
            21,31,33,
            32,47,45,
            32,46,47,
            48,31,32,
            32,34,51,
            34,33,50,
            33,31,48,
            51,50,48,
            26,21,43
        };

#if 0

        for(PXSize i = 0; i < sizeof(indexList); ++i)
        {
            --indexList[i];
        }

        PXClear(PXECSCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.Type = PXResourceTypeModel;
        //    pxResourceCreateInfo.ObjectReference = &pxEngine->ResourceManager->ModelFailback;

        pxResourceCreateInfo.Model.Form = PXModelFormCustom;

        PXBufferSet(&pxResourceCreateInfo.Model.VertexBuffer, vertexData, sizeof(vertexData));

        pxResourceCreateInfo.Model.VertexBuffer.LayoutAmount = 1;
        pxResourceCreateInfo.Model.VertexBuffer.LayoutPrime[0].AmountOfElements = 3;
        pxResourceCreateInfo.Model.VertexBuffer.LayoutPrime[0].Format = PXTypeF32;

        PXBufferSet(&pxResourceCreateInfo.Model.IndexBuffer.Data, indexList, sizeof(indexList));

        pxResourceCreateInfo.Model.IndexBuffer.DataType = PXTypeI08U;
        pxResourceCreateInfo.Model.IndexBuffer.DrawModeID = PXDrawModeIDTriangle;

        // pxResourceCreateInfo.Model.ShaderProgramReference = pxEngine->ResourceManager->ShaderFailback;

        // PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);

         //PXMatrix4x4FScaleBy(&pxEngine->ResourceManager->ModelFailback->ModelMatrix, 20);
    }

    // Create missing texture
    {
        const PXColorRGB_U8 pink = { 165, 5, 250 };
        const PXColorRGB_U8 black = { 30, 30, 30 };

        const PXI8U colorData[4 * 4] =
        {
            pink.Red, pink.Green, pink.Blue,
            black.Red, black.Green, black.Blue,
            black.Red, black.Green, black.Blue,
            pink.Red, pink.Green, pink.Blue
        };

        PXTexture2D* failbackTexture = 0;

        PXClear(PXECSCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.Type = PXResourceTypeTexture2D;
        // pxResourceCreateInfo.ObjectReference = (void**)&pxEngine->ResourceManager->Texture2DFailBack;
        pxResourceCreateInfo.Texture.Texture.PixelData = (void*)colorData;
        pxResourceCreateInfo.Texture.Texture.PixelDataSize = sizeof(colorData);
        pxResourceCreateInfo.Texture.Texture.Width = 2;
        pxResourceCreateInfo.Texture.Texture.Height = 2;
        pxResourceCreateInfo.Texture.Texture.Depth = 1;
        pxResourceCreateInfo.Texture.Texture.Format = PXColorFormatRGBI8;

        PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);


#endif
    }
    PXMaterial* material = PXMemoryHeapCallocT(PXMaterial, 1);

    //pxEngine->ResourceManager->ModelFailback->Mesh.IndexBuffer.SegmentPrime.Material = material;
    //material->DiffuseTexture = pxEngine->ResourceManager->Texture2DFailBack;
}

PXResult PXAPI PXEngineResourceRenderDefault(PXEngine PXREF pxEngine)
{
    assert(pxEngine);

   
    //glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Set background color
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
    {
        PXClear(PXRenderEntity, &pxRenderEntity);
        pxRenderEntity.Type = PXResourceTypeSkybox;
        pxRenderEntity.CameraReference = pxEngine->CameraCurrent;


        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(&pxEngine->ResourceManager->SkyBoxLookUp, 0, &pxDictionaryEntry);

        if(pxDictionaryEntry.Value)
        {
            PXSkyBox PXREF pxSkyBox = *(PXSkyBox**)pxDictionaryEntry.Value;


            pxRenderEntity.ObjectReference = pxSkyBox;

            if(pxSkyBox)
            {
                pxRenderEntity.ShaderProgramReference = pxSkyBox->ShaderProgramReference;
            }

            PXEngineResourceRender(pxEngine, &pxRenderEntity);
        }
    }

    // Model
    {
        PXDictionary PXREF modelLookup = &pxEngine->ResourceManager->ModelLookUp;

        PXShaderProgram* qqq = 0;

        for(PXSize i = 0; i < modelLookup->List.EntryAmountUsed; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXModel* pxModel = PXNull;

            PXDictionaryIndex(modelLookup, i, &pxDictionaryEntry);

            if(!pxDictionaryEntry.Value)
            {
                continue;
            }

            pxModel = *(PXModel**)pxDictionaryEntry.Value;

            if(!(pxModel->Info.Behaviour & PXECSInfoRender))
            {
                continue;
            }


            PXClear(PXRenderEntity, &pxRenderEntity);
            pxRenderEntity.Type = PXResourceTypeModel;
            pxRenderEntity.CameraReference = pxEngine->CameraCurrent;
            pxRenderEntity.ObjectReference = pxModel;
            pxRenderEntity.MatrixModel = pxModel->ModelMatrix;
            pxRenderEntity.ShaderProgramReference = pxModel->ShaderProgramReference;

            pxModel->RenderBothSides = 1;
            // pxModel->Mesh.IndexBuffer.DrawModeID |= PXDrawModeIDPoint; // For debug only, you can see vertex points

#if 0
                // TODO: TEMP FIX
            if(!qqq)
            {
                qqq = pxModel->ShaderProgramReference;
            }
            else
            {
                pxModel->ShaderProgramReference = qqq;
                pxRenderEntity.ShaderProgramReference = qqq;
            }
#endif

            if(pxModel->ShaderProgramReference)
            {
                PXEngineResourceRender(pxEngine, &pxRenderEntity);
            }
        }
    }

    // Sprite
    {
        PXDictionary PXREF spirteList = &pxEngine->ResourceManager->SpritelLookUp;

        for(PXSize i = 0; i < spirteList->List.EntryAmountUsed; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXSprite* pxSprite = PXNull;

            PXDictionaryIndex(spirteList, i, &pxDictionaryEntry);

            if(!pxDictionaryEntry.Value)
            {
                continue;
            }

            pxSprite = *(PXSprite**)pxDictionaryEntry.Value;

            if(!(pxSprite->Info.Behaviour & PXECSInfoRender))
            {
                continue;
            }

            PXClear(PXRenderEntity, &pxRenderEntity);
            pxRenderEntity.Type = PXResourceTypeSprite;
            pxRenderEntity.CameraReference = pxEngine->CameraCurrent;
            pxRenderEntity.ObjectReference = pxSprite->Model;
            pxRenderEntity.ShaderProgramReference = pxSprite->ShaderProgarm;
            pxRenderEntity.MatrixModel = pxSprite->Model->ModelMatrix;
            pxSprite->Model->ShaderProgramReference = pxSprite->ShaderProgarm;
            pxRenderEntity.Texture2DOverride = pxSprite->Texture;
            pxRenderEntity.MaterialOverride = pxSprite->Material;


#if 1 // if show hitbox
            if(pxSprite->HitBox)
            {
                if(pxSprite->HitBox->ColliderChild)
                {
                    pxSprite->Material->Diffuse.Red = 0.0f;
                    pxSprite->Material->Diffuse.Green = 1.0f;
                    pxSprite->Material->Diffuse.Blue = 0.0f;
                    pxSprite->Material->Diffuse.Alpha = 1.0f;
                }
                else if(pxSprite->HitBox->ColliderParent)
                {
                    pxSprite->Material->Diffuse.Red = 1.0f;
                    pxSprite->Material->Diffuse.Green = 1.0f;
                    pxSprite->Material->Diffuse.Blue = 0.0f;
                    pxSprite->Material->Diffuse.Alpha = 1.0f;
                }
                else
                {
                    pxSprite->Material->Diffuse.Red = 0.0f;
                    pxSprite->Material->Diffuse.Green = 0.5f;
                    pxSprite->Material->Diffuse.Blue = 0.5f;
                    pxSprite->Material->Diffuse.Alpha = 1.0f;
                }
            }
#endif

            PXEngineResourceRender(pxEngine, &pxRenderEntity);
        }
    }

    // Text
    {
        PXDictionary PXREF textList = 0;//&pxEngine->ResourceManager->TextLookUp;

        for(PXSize i = 0; i < textList->List.EntryAmountUsed; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            //PXEngineText* pxEngineText = PXNull;

            PXDictionaryIndex(textList, i, &pxDictionaryEntry);

            if(!pxDictionaryEntry.Value)
            {
                continue;
            }

            // pxEngineText = *(PXEngineText**)pxDictionaryEntry.Value;

            // if(!(pxEngineText->Info.Behaviour & PXECSInfoActive))
            {
                continue;
            }

            PXClear(PXRenderEntity, &pxRenderEntity);
            pxRenderEntity.Type = PXResourceTypeText;
            pxRenderEntity.CameraReference = pxEngine->CameraCurrent;
            //  pxRenderEntity.ObjectReference = pxEngineText;
            pxRenderEntity.ShaderProgramReference = 0;; // pxEngineText.->ShaderProgramReference;

            PXEngineResourceRender(pxEngine, &pxRenderEntity);
        }
    }

    // HitBoxes
    {
        PXDictionary PXREF hitBoxList = 0;// &pxEngine->ResourceManager->HitBoxLookUp;

        for(PXSize i = 0; i < hitBoxList->List.EntryAmountUsed; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXHitBox* pxHitBox = PXNull;

            PXDictionaryIndex(hitBoxList, i, &pxDictionaryEntry);

            pxHitBox = *(PXHitBox**)pxDictionaryEntry.Value;

            PXClear(PXRenderEntity, &pxRenderEntity);
            pxRenderEntity.Type = PXResourceTypeHitBox;
            pxRenderEntity.CameraReference = pxEngine->CameraCurrent;
            pxRenderEntity.ObjectReference = pxHitBox;
            pxRenderEntity.ShaderProgramReference = 0;

            PXEngineResourceRender(pxEngine, &pxRenderEntity);
        }
    }
#endif
    return PXResultOK;
}

void PXAPI PXEngineUpdateSpriteAnimator(PXEngine PXREF pxEngine)
{
    /*
    PXDictionary PXREF spriteAnimatorList = &pxEngine->ResourceManager->SpriteAnimator;

    for(PXSize index = 0; index < spriteAnimatorList->EntryAmountCurrent; ++index)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXSpriteAnimator* pxSpriteAnimator = PXNull;

        PXDictionaryIndex(spriteAnimatorList, index, &pxDictionaryEntry);

        pxSpriteAnimator = *(PXSpriteAnimator**)pxDictionaryEntry.Value;

        if(!(pxSpriteAnimator->Info.Behaviour & PXECSInfoActive))
        {
            continue;
        }

        const PXBool proceedAnimate =
            pxSpriteAnimator->TimeStampList &&
            pxSpriteAnimator->TimeStampAmount &&
            pxSpriteAnimator->Info.Behaviour & PXSpriteAnimatorBehaviourAnimationEnable;

        if(!proceedAnimate)
        {
            continue;
        }

        // Get current keyframe
        PXSpriteAnimatorTimeStamp PXREF pxSpriteAnimatorTimeStamp = &pxSpriteAnimator->TimeStampList[pxSpriteAnimator->TimeStampCurrent];

        const PXI32U delta = pxEngine->TimeData.CounterTimeLast - pxSpriteAnimator->LastUpdate;
        const PXF32 deltaTargetms = PXTimeCounterStampToSecoundsF(delta);
        const PXF32 tergetedDelta = pxSpriteAnimatorTimeStamp->DeltaTime;


        // if time is big enough, update. Else, do nothing
        const PXBool shalUpdate = deltaTargetms >= tergetedDelta; // pxSpriteAnimator->RateUpdate

        if(!shalUpdate)
        {
            continue;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "SpriteAnimator",
            "Updated",
            "%8.4f / %8.4f",
            deltaTargetms,
            tergetedDelta
        );
#endif

        pxSpriteAnimator->LastUpdate = pxEngine->TimeData.CounterTimeLast;

        pxSpriteAnimator->SpriteTarget->Texture = pxSpriteAnimatorTimeStamp->Texture;

        ++pxSpriteAnimator->TimeStampCurrent;
        pxSpriteAnimator->TimeStampCurrent %= pxSpriteAnimator->TimeStampAmount;

        // If it ran once, check if we want to stop the animation
        {
            const PXBool isLastFrame = pxSpriteAnimator->TimeStampCurrent == pxSpriteAnimator->TimeStampAmount;
            const PXBool playOnce = (pxSpriteAnimator->Info.Behaviour & PXSpriteAnimatorBehaviourPlayOnce) > 0;
            const PXBool stopAnimation = isLastFrame && playOnce;

            if(stopAnimation)
            {
                pxSpriteAnimator->Info.Behaviour &= ~PXSpriteAnimatorBehaviourAnimationEnable;
            }
        }
    }
    */
}

void PXAPI PXEngineHitBoxHandleAvsB(PXEngine PXREF pxEngine, PXFieldEffect PXREF hitBoxA, PXFieldEffect PXREF hitBoxB)
{
    /*
    pxVector3 positionA;
    pxVector3 positionB;
    pxVector3 positionSizeA;
    pxVector3 positionSizeB;

    PXF32 boundingBox[8] =
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };


    PXMatrix4x4FPositionGet(&hitBoxA->Model->ModelMatrix, &positionA);
    PXMatrix4x4FPositionGet(&hitBoxB->Model->ModelMatrix, &positionB);

    PXMatrix4x4FScaleGet(&hitBoxA->Model->ModelMatrix, &positionSizeA);
    PXMatrix4x4FScaleGet(&hitBoxB->Model->ModelMatrix, &positionSizeB);


    // Get boundingbox
    {

    }


    const PXBool ss = PXCollisionAABB
    (
        positionA.X + -1,
        positionA.Y + -1,
        positionA.X + 1 * positionSizeA.X,
        positionA.Y + 1 * positionSizeA.Y,
        positionB.X + -1,
        positionB.Y + -1,
        positionB.X + 1 * positionSizeB.X,
        positionB.Y + 1 * positionSizeB.Y
    );


    if(ss)
    {

        // We can add them as a linked list.

        PXHitBox* inserz = hitBoxA;

        while(inserz->ColliderChild)
        {
            inserz = inserz->ColliderChild;
        }


        inserz->ColliderChild = hitBoxB;


        hitBoxB->ColliderParent = inserz;



        /*
           PXLogPrint
        (
            PXLoggingInfo,
            "HitBox",
            "Compare",
            "%p vs %p - COLLISION",
            hitBoxA,
            hitBoxB
        );

        * /

    }
    else
    {
        //hitBoxA->Collider = PXNull;

        /*
                PXLogPrint
                (
                    PXLoggingInfo,
                    "HitBox",
                    "Compare",
                    "%p vs %p",
                    hitBoxA,
                    hitBoxB
                );
                    * /
    }
    */
}
