#include "PXEngine.h"

#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>
#include <OS/Async/PXProcess.h>
#include <OS/File/PXFile.h>
#include <Engine/Dialog/PXDialogBox.h>
#include <Media/ADLER/PXAdler32.h>
#include <OS/Hardware/PXProcessor.h>

const char* PXAPI PXEngineCreateTypeToString(const PXEngineCreateType pxEngineCreateType)
{
    switch(pxEngineCreateType)
    {
        case PXEngineCreateTypeCustom: return "Custom";
        case PXEngineCreateTypeModel: return "Model";
        case PXEngineCreateTypeFont: return "Font";
        case PXEngineCreateTypeTexture2D: return "Texture2D";
        case PXEngineCreateTypeImage: return "Image";
        case PXEngineCreateTypeTextureCube: return "TetxureCube";
        case PXEngineCreateTypeShaderProgram: return "ShaderProgram";
        case PXEngineCreateTypeSkybox: return "SkyBox";
        case PXEngineCreateTypeSprite: return "Sprite";
        case PXEngineCreateTypeText: return "Text";
        case PXEngineCreateTypeTimer: return "Timer";
        case PXEngineCreateTypeSound: return "Sound";
        case PXEngineCreateTypeEngineSound: return "PXSound";
        case PXEngineCreateTypeUIElement: return "UI";
        case PXEngineCreateTypeHitBox: return "HitBox";
        case PXEngineCreateTypeDialogBox: return "DialogBox";

        default:
            return PXNull;
    }
}

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

#define UseOSDelta 0

void PXAPI PXEngineWindowEvent(PXEngine* const pxEngine, PXWindowEvent* const pxWindowEvent)
{
    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeInputMouseButton:
        {
            PXWindowEventInputMouseButton* const inputMouseButton = &pxWindowEvent->InputMouseButton;

            PXMouse* const mouse = &pxEngine->MouseCurrentInput;

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

#if 0
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
            PXWindowEventInputMouseMove* const inputMouseMove = &pxWindowEvent->InputMouseMove;
            PXMouse* const mouse = &pxEngine->MouseCurrentInput;

            const PXInt32S mousePositionOld[2] =
            {
                mouse->Position[0],
                mouse->Position[1]
            };
            const PXInt32S mousePositionNew[2] =
            {
        #if UseOSDelta
                mousePositionOld + deltaX,
                mousePositionOld - deltaY
        #else
                // PXMathLimit(inputMouseMove->AxisX, 0, window->Width),
                // window->Height - PXMathLimit(inputMouseMove->AxisY, 0, window->Height)
                0,
                0
        #endif
            };

            const PXInt32S mousePositionDeltaNew[2] =
            {
                #if UseOSDelta
                mousePositionNew[0] - mousePositionOld[0],
                mousePositionNew[1] - mousePositionOld[1]
                #else
                inputMouseMove->DeltaX,
                inputMouseMove->DeltaY
                #endif
            };

            const PXBool hasDelta = (mousePositionDeltaNew[0] != 0 && mousePositionDeltaNew[1] != 0) || 1;

            if(hasDelta)
            {
                //mouse->Delta[0] = mousePositionDeltaNew[0];
                //mouse->Delta[1] = mousePositionDeltaNew[1];
                mouse->Delta[0] += inputMouseMove->DeltaX;
                mouse->Delta[1] += inputMouseMove->DeltaY;
               // mouse->DeltaNormalisized[0] = (mousePositionDeltaNew[0] / ((float)window->Width / 2.0f)) - 1.0f;
              //  mouse->DeltaNormalisized[1] = (mousePositionDeltaNew[1] / ((float)window->Height / 2.0f)) - 1.0f;
                mouse->Position[0] = mousePositionNew[0];
                mouse->Position[1] = mousePositionNew[1];
             //   mouse->PositionNormalisized[0] = (mousePositionNew[0] / ((float)window->Width / 2.0f)) - 1.0f;
             //   mouse->PositionNormalisized[1] = (mousePositionNew[1] / ((float)window->Height / 2.0f)) - 1.0f;
            }
            else
            {
                mouse->Delta[0] = 0;
                mouse->Delta[1] = 0;
            }

            PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, PXNull);

            break;
        }
        case PXWindowEventTypeInputKeyboard:
        {
            PXWindowEventInputKeyboard* const inputKeyboard = &pxWindowEvent->InputKeyboard;
            PXKeyBoard* const keyBoard = &pxEngine->KeyBoardCurrentInput;

            PXInt32U mask = 0;
            PXInt32U data = 0;
            

            // printf("[#][Event][Key] ID:%-3i Name:%-3i State:%i\n", keyBoardKeyInfo->KeyID, keyBoardKeyInfo->Key, keyBoardKeyInfo->Mode);

            if(PXKeyPressStateDown == inputKeyboard->PressState)
            {
                switch(inputKeyboard->VirtualKey)
                {
                    case KeyA: keyBoard->Letters |= KeyBoardIDLetterA; break;
                    case KeyB: keyBoard->Letters |= KeyBoardIDLetterB; break;
                    case KeyC: keyBoard->Letters |= KeyBoardIDLetterC; break;
                    case KeyD: keyBoard->Letters |= KeyBoardIDLetterD; break;
                    case KeyE: keyBoard->Letters |= KeyBoardIDLetterE; break;
                    case KeyF: keyBoard->Letters |= KeyBoardIDLetterF; break;
                    case KeyG: keyBoard->Letters |= KeyBoardIDLetterG; break;
                    case KeyH: keyBoard->Letters |= KeyBoardIDLetterH; break;
                    case KeyI: keyBoard->Letters |= KeyBoardIDLetterI; break;
                    case KeyJ: keyBoard->Letters |= KeyBoardIDLetterJ; break;
                    case KeyK: keyBoard->Letters |= KeyBoardIDLetterK; break;
                    case KeyL: keyBoard->Letters |= KeyBoardIDLetterL; break;
                    case KeyM: keyBoard->Letters |= KeyBoardIDLetterM; break;
                    case KeyN: keyBoard->Letters |= KeyBoardIDLetterN; break;
                    case KeyO: keyBoard->Letters |= KeyBoardIDLetterO; break;
                    case KeyP: keyBoard->Letters |= KeyBoardIDLetterP; break;
                    case KeyQ: keyBoard->Letters |= KeyBoardIDLetterQ; break;
                    case KeyR: keyBoard->Letters |= KeyBoardIDLetterR; break;
                    case KeyS: keyBoard->Letters |= KeyBoardIDLetterS; break;
                    case KeyT: keyBoard->Letters |= KeyBoardIDLetterT; break;
                    case KeyU: keyBoard->Letters |= KeyBoardIDLetterU; break;
                    case KeyV: keyBoard->Letters |= KeyBoardIDLetterV; break;
                    case KeyW: keyBoard->Letters |= KeyBoardIDLetterW; break;
                    case KeyX: keyBoard->Letters |= KeyBoardIDLetterX; break;
                    case KeyY: keyBoard->Letters |= KeyBoardIDLetterY; break;
                    case KeyZ: keyBoard->Letters |= KeyBoardIDLetterZ; break;
                    case KeySpace: keyBoard->Letters |= KeyBoardIDSpace; break;
                    case KeyApostrophe: keyBoard->Letters |= KeyBoardIDAPOSTROPHE; break;
                    case KeyComma: keyBoard->Letters |= KeyBoardIDComma; break;
                    case KeyGraveAccent: keyBoard->Letters |= KeyBoardIDGRAVE_ACCENT; break;
                    case KeySemicolon: keyBoard->Letters |= KeyBoardIDSemicolon; break;
                    case KeyPeriod: keyBoard->Letters |= KeyBoardIDDECIMAL; break;


                    case KeyEscape: keyBoard->Commands |= KeyBoardIDCommandEscape; break;
                    case KeyEnter: keyBoard->Commands |= KeyBoardIDCommandEnter; break;
                    case KeyTab: keyBoard->Commands |= KeyBoardIDCommandTab; break;
                        //case : keyBoard->Commands |= KeyBoardIDCommandShift; break;
                    case KeyBackspace: keyBoard->Commands |= KeyBoardIDBACKSPACE; break;
                    case KeyInsert: keyBoard->Commands |= KeyBoardIDINSERT; break;
                    case KeyDelete: keyBoard->Commands |= KeyBoardIDDELETE; break;
                    case KeyRight: keyBoard->Commands |= KeyBoardIDRIGHT; break;
                    case KeyLeft: keyBoard->Commands |= KeyBoardIDLEFT; break;
                    case KeyDown: keyBoard->Commands |= KeyBoardIDDOWN; break;
                    case KeyUp: keyBoard->Commands |= KeyBoardIDUP; break;
                    case KeyPageUp: keyBoard->Commands |= KeyBoardIDPAGE_UP; break;
                    case KeyPageDown: keyBoard->Commands |= KeyBoardIDPAGE_DOWN; break;
                    case KeyHome: keyBoard->Commands |= KeyBoardIDHOME; break;
                    case KeyEnd: keyBoard->Commands |= KeyBoardIDEND; break;
                    case KeyCapsLock: keyBoard->Commands |= KeyBoardIDCAPS_LOCK; break;
                    case KeyScrollLock: keyBoard->Commands |= KeyBoardIDSCROLL_LOCK; break;
                    case KeyNumLock: keyBoard->Commands |= KeyBoardIDNUM_LOCK; break;
                    case KeyPrintScreen: keyBoard->Commands |= KeyBoardIDPRINT_SCREEN; break;
                    case KeyPause: keyBoard->Commands |= KeyBoardIDPAUSE; break;
                    case KeyPadEnter: keyBoard->Commands |= KeyBoardIDPadENTER; break;
                    case KeyShiftLeft: keyBoard->Commands |= KeyBoardIDShiftLeft; break;
                    case KeyShiftRight: keyBoard->Commands |= KeyBoardIDShiftRight; break;
                    case KeyControlLeft: keyBoard->Commands |= KeyBoardIDCONTROLLEFT; break;
                    case KeyAltLeft: keyBoard->Commands |= KeyBoardIDALTLEFT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDSUPERLEFT; break;
                    case KeyControlRight: keyBoard->Commands |= KeyBoardIDCONTROLRIGHT; break;
                    case KeyAltRight: keyBoard->Commands |= KeyBoardIDALTRIGHT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDSUPERRIGHT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDMENU; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDWORLD_1; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDWORLD_2; break;


                            // Numbers
                    case Key0: keyBoard->Numbers |= KeyBoardIDNumber0; break;
                    case Key1: keyBoard->Numbers |= KeyBoardIDNumber1; break;
                    case Key2: keyBoard->Numbers |= KeyBoardIDNumber2; break;
                    case Key3: keyBoard->Numbers |= KeyBoardIDNumber3; break;
                    case Key4: keyBoard->Numbers |= KeyBoardIDNumber4; break;
                    case Key5: keyBoard->Numbers |= KeyBoardIDNumber5; break;
                    case Key6: keyBoard->Numbers |= KeyBoardIDNumber6; break;
                    case Key7: keyBoard->Numbers |= KeyBoardIDNumber7; break;
                    case Key8: keyBoard->Numbers |= KeyBoardIDNumber8; break;
                    case Key9: keyBoard->Numbers |= KeyBoardIDNumber9; break;
                    case KeyPad0: keyBoard->Numbers |= KeyBoardIDNumberBlock0; break;
                    case KeyPad1: keyBoard->Numbers |= KeyBoardIDNumberBlock1; break;
                    case KeyPad2: keyBoard->Numbers |= KeyBoardIDNumberBlock2; break;
                    case KeyPad3: keyBoard->Numbers |= KeyBoardIDNumberBlock3; break;
                    case KeyPad4: keyBoard->Numbers |= KeyBoardIDNumberBlock4; break;
                    case KeyPad5: keyBoard->Numbers |= KeyBoardIDNumberBlock5; break;
                    case KeyPad6: keyBoard->Numbers |= KeyBoardIDNumberBlock6; break;
                    case KeyPad7: keyBoard->Numbers |= KeyBoardIDNumberBlock7; break;
                    case KeyPad8: keyBoard->Numbers |= KeyBoardIDNumberBlock8; break;
                    case KeyPad9: keyBoard->Numbers |= KeyBoardIDNumberBlock9; break;
                    case KeyMinus: keyBoard->Numbers |= KeyBoardIDNumberKeyMinus; break;
                        // case KeyPeriod: keyBoard->Numbers |= KeyBoardIDNumberKeyPeriod; break;
                    case KeySlash: keyBoard->Numbers |= KeyBoardIDNumberKeySlash; break;
                    case KeyEqual: keyBoard->Numbers |= KeyBoardIDNumberKeyEqual; break;
                    case KeyBrackedLeft: keyBoard->Numbers |= KeyBoardIDNumberKeyLEFT_BRACKET; break;
                    case KeyBackSlash: keyBoard->Numbers |= KeyBoardIDNumberKeyBACKSLASH; break;
                    case KeyBrackedRight: keyBoard->Numbers |= KeyBoardIDNumberKeyRIGHT_BRACKET; break;
                    case KeyPadDivide: keyBoard->Numbers |= KeyBoardIDNumberKeyPadDIVIDE; break;
                    case KeyPadMultiply: keyBoard->Numbers |= KeyBoardIDNumberKeyPadMULTIPLY; break;
                    case KeyPadSubtract: keyBoard->Numbers |= KeyBoardIDNumberKeyPadSUBTRACT; break;
                    case KeyPadAdd: keyBoard->Numbers |= KeyBoardIDNumberKeyPadADD; break;
                    case KeyPadEqual: keyBoard->Numbers |= KeyBoardIDNumberKeyPadEQUAL; break;

                        // Function key

                    case KeyF1: keyBoard->Actions |= KeyBoardIDF01; break;
                    case KeyF2: keyBoard->Actions |= KeyBoardIDF02; break;
                    case KeyF3: keyBoard->Actions |= KeyBoardIDF03; break;
                    case KeyF4: keyBoard->Actions |= KeyBoardIDF04; break;
                    case KeyF5: keyBoard->Actions |= KeyBoardIDF05; break;
                    case KeyF6: keyBoard->Actions |= KeyBoardIDF06; break;
                    case KeyF7: keyBoard->Actions |= KeyBoardIDF07; break;
                    case KeyF8: keyBoard->Actions |= KeyBoardIDF08; break;
                    case KeyF9: keyBoard->Actions |= KeyBoardIDF09; break;
                    case KeyF10: keyBoard->Actions |= KeyBoardIDF10; break;
                    case KeyF11: keyBoard->Actions |= KeyBoardIDF11; break;
                    case KeyF12: keyBoard->Actions |= KeyBoardIDF12; break;
                    case KeyF13: keyBoard->Actions |= KeyBoardIDF13; break;
                    case KeyF14: keyBoard->Actions |= KeyBoardIDF14; break;
                    case KeyF15: keyBoard->Actions |= KeyBoardIDF15; break;
                    case KeyF16: keyBoard->Actions |= KeyBoardIDF16; break;
                    case KeyF17: keyBoard->Actions |= KeyBoardIDF17; break;
                    case KeyF18: keyBoard->Actions |= KeyBoardIDF18; break;
                    case KeyF19: keyBoard->Actions |= KeyBoardIDF19; break;
                    case KeyF20: keyBoard->Actions |= KeyBoardIDF20; break;
                    case KeyF21: keyBoard->Actions |= KeyBoardIDF21; break;
                    case KeyF22: keyBoard->Actions |= KeyBoardIDF22; break;
                    case KeyF23: keyBoard->Actions |= KeyBoardIDF23; break;
                    case KeyF24: keyBoard->Actions |= KeyBoardIDF24; break;
                    case KeyF25: keyBoard->Actions |= KeyBoardIDF25; break;

                    default:
                        break;
                }
            }
            else
            {
                switch(inputKeyboard->VirtualKey)
                {
                    case KeyA: keyBoard->Letters &= ~KeyBoardIDLetterA; break;
                    case KeyB: keyBoard->Letters &= ~KeyBoardIDLetterB; break;
                    case KeyC: keyBoard->Letters &= ~KeyBoardIDLetterC; break;
                    case KeyD: keyBoard->Letters &= ~KeyBoardIDLetterD; break;
                    case KeyE: keyBoard->Letters &= ~KeyBoardIDLetterE; break;
                    case KeyF: keyBoard->Letters &= ~KeyBoardIDLetterF; break;
                    case KeyG: keyBoard->Letters &= ~KeyBoardIDLetterG; break;
                    case KeyH: keyBoard->Letters &= ~KeyBoardIDLetterH; break;
                    case KeyI: keyBoard->Letters &= ~KeyBoardIDLetterI; break;
                    case KeyJ: keyBoard->Letters &= ~KeyBoardIDLetterJ; break;
                    case KeyK: keyBoard->Letters &= ~KeyBoardIDLetterK; break;
                    case KeyL: keyBoard->Letters &= ~KeyBoardIDLetterL; break;
                    case KeyM: keyBoard->Letters &= ~KeyBoardIDLetterM; break;
                    case KeyN: keyBoard->Letters &= ~KeyBoardIDLetterN; break;
                    case KeyO: keyBoard->Letters &= ~KeyBoardIDLetterO; break;
                    case KeyP: keyBoard->Letters &= ~KeyBoardIDLetterP; break;
                    case KeyQ: keyBoard->Letters &= ~KeyBoardIDLetterQ; break;
                    case KeyR: keyBoard->Letters &= ~KeyBoardIDLetterR; break;
                    case KeyS: keyBoard->Letters &= ~KeyBoardIDLetterS; break;
                    case KeyT: keyBoard->Letters &= ~KeyBoardIDLetterT; break;
                    case KeyU: keyBoard->Letters &= ~KeyBoardIDLetterU; break;
                    case KeyV: keyBoard->Letters &= ~KeyBoardIDLetterV; break;
                    case KeyW: keyBoard->Letters &= ~KeyBoardIDLetterW; break;
                    case KeyX: keyBoard->Letters &= ~KeyBoardIDLetterX; break;
                    case KeyY: keyBoard->Letters &= ~KeyBoardIDLetterY; break;
                    case KeyZ: keyBoard->Letters &= ~KeyBoardIDLetterZ; break;
                    case KeySpace: keyBoard->Letters &= ~KeyBoardIDSpace; break;
                    case KeyApostrophe: keyBoard->Letters &= ~KeyBoardIDAPOSTROPHE; break;
                    case KeyComma: keyBoard->Letters &= ~KeyBoardIDComma; break;
                    case KeyGraveAccent: keyBoard->Letters &= ~KeyBoardIDGRAVE_ACCENT; break;
                    case KeySemicolon: keyBoard->Letters &= ~KeyBoardIDSemicolon; break;
                    case KeyPeriod: keyBoard->Letters &= ~KeyBoardIDDECIMAL; break;


                    case KeyEscape: keyBoard->Commands &= ~KeyBoardIDCommandEscape; break;
                    case KeyEnter: keyBoard->Commands &= ~KeyBoardIDCommandEnter; break;
                    case KeyTab: keyBoard->Commands &= ~KeyBoardIDCommandTab; break;
                        //case : keyBoard->Commands &= ~KeyBoardIDCommandShift; break;
                    case KeyBackspace: keyBoard->Commands &= ~KeyBoardIDBACKSPACE; break;
                    case KeyInsert: keyBoard->Commands &= ~KeyBoardIDINSERT; break;
                    case KeyDelete: keyBoard->Commands &= ~KeyBoardIDDELETE; break;
                    case KeyRight: keyBoard->Commands &= ~KeyBoardIDRIGHT; break;
                    case KeyLeft: keyBoard->Commands &= ~KeyBoardIDLEFT; break;
                    case KeyDown: keyBoard->Commands &= ~KeyBoardIDDOWN; break;
                    case KeyUp: keyBoard->Commands &= ~KeyBoardIDUP; break;
                    case KeyPageUp: keyBoard->Commands &= ~KeyBoardIDPAGE_UP; break;
                    case KeyPageDown: keyBoard->Commands &= ~KeyBoardIDPAGE_DOWN; break;
                    case KeyHome: keyBoard->Commands &= ~KeyBoardIDHOME; break;
                    case KeyEnd: keyBoard->Commands &= ~KeyBoardIDEND; break;
                    case KeyCapsLock: keyBoard->Commands &= ~KeyBoardIDCAPS_LOCK; break;
                    case KeyScrollLock: keyBoard->Commands &= ~KeyBoardIDSCROLL_LOCK; break;
                    case KeyNumLock: keyBoard->Commands &= ~KeyBoardIDNUM_LOCK; break;
                    case KeyPrintScreen: keyBoard->Commands &= ~KeyBoardIDPRINT_SCREEN; break;
                    case KeyPause: keyBoard->Commands &= ~KeyBoardIDPAUSE; break;
                    case KeyPadEnter: keyBoard->Commands &= ~KeyBoardIDPadENTER; break;
                    case KeyShiftLeft: keyBoard->Commands &= ~KeyBoardIDShiftLeft; break;
                    case KeyShiftRight: keyBoard->Commands &= ~KeyBoardIDShiftRight; break;
                    case KeyControlLeft: keyBoard->Commands &= ~KeyBoardIDCONTROLLEFT; break;
                    case KeyAltLeft: keyBoard->Commands &= ~KeyBoardIDALTLEFT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDSUPERLEFT; break;
                    case KeyControlRight: keyBoard->Commands &= ~KeyBoardIDCONTROLRIGHT; break;
                    case KeyAltRight: keyBoard->Commands &= ~KeyBoardIDALTRIGHT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDSUPERRIGHT; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDMENU; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDWORLD_1; break;
                        //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDWORLD_2; break;


                            // Numbers
                    case Key0: keyBoard->Numbers &= ~KeyBoardIDNumber0; break;
                    case Key1: keyBoard->Numbers &= ~KeyBoardIDNumber1; break;
                    case Key2: keyBoard->Numbers &= ~KeyBoardIDNumber2; break;
                    case Key3: keyBoard->Numbers &= ~KeyBoardIDNumber3; break;
                    case Key4: keyBoard->Numbers &= ~KeyBoardIDNumber4; break;
                    case Key5: keyBoard->Numbers &= ~KeyBoardIDNumber5; break;
                    case Key6: keyBoard->Numbers &= ~KeyBoardIDNumber6; break;
                    case Key7: keyBoard->Numbers &= ~KeyBoardIDNumber7; break;
                    case Key8: keyBoard->Numbers &= ~KeyBoardIDNumber8; break;
                    case Key9: keyBoard->Numbers &= ~KeyBoardIDNumber9; break;
                    case KeyPad0: keyBoard->Numbers &= ~KeyBoardIDNumberBlock0; break;
                    case KeyPad1: keyBoard->Numbers &= ~KeyBoardIDNumberBlock1; break;
                    case KeyPad2: keyBoard->Numbers &= ~KeyBoardIDNumberBlock2; break;
                    case KeyPad3: keyBoard->Numbers &= ~KeyBoardIDNumberBlock3; break;
                    case KeyPad4: keyBoard->Numbers &= ~KeyBoardIDNumberBlock4; break;
                    case KeyPad5: keyBoard->Numbers &= ~KeyBoardIDNumberBlock5; break;
                    case KeyPad6: keyBoard->Numbers &= ~KeyBoardIDNumberBlock6; break;
                    case KeyPad7: keyBoard->Numbers &= ~KeyBoardIDNumberBlock7; break;
                    case KeyPad8: keyBoard->Numbers &= ~KeyBoardIDNumberBlock8; break;
                    case KeyPad9: keyBoard->Numbers &= ~KeyBoardIDNumberBlock9; break;
                    case KeyMinus: keyBoard->Numbers &= ~KeyBoardIDNumberKeyMinus; break;
                        // case KeyPeriod: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPeriod; break;
                    case KeySlash: keyBoard->Numbers &= ~KeyBoardIDNumberKeySlash; break;
                    case KeyEqual: keyBoard->Numbers &= ~KeyBoardIDNumberKeyEqual; break;
                    case KeyBrackedLeft: keyBoard->Numbers &= ~KeyBoardIDNumberKeyLEFT_BRACKET; break;
                    case KeyBackSlash: keyBoard->Numbers &= ~KeyBoardIDNumberKeyBACKSLASH; break;
                    case KeyBrackedRight: keyBoard->Numbers &= ~KeyBoardIDNumberKeyRIGHT_BRACKET; break;
                    case KeyPadDivide: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadDIVIDE; break;
                    case KeyPadMultiply: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadMULTIPLY; break;
                    case KeyPadSubtract: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadSUBTRACT; break;
                    case KeyPadAdd: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadADD; break;
                    case KeyPadEqual: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadEQUAL; break;

                        // Function key

                    case KeyF1: keyBoard->Actions &= ~KeyBoardIDF01; break;
                    case KeyF2: keyBoard->Actions &= ~KeyBoardIDF02; break;
                    case KeyF3: keyBoard->Actions &= ~KeyBoardIDF03; break;
                    case KeyF4: keyBoard->Actions &= ~KeyBoardIDF04; break;
                    case KeyF5: keyBoard->Actions &= ~KeyBoardIDF05; break;
                    case KeyF6: keyBoard->Actions &= ~KeyBoardIDF06; break;
                    case KeyF7: keyBoard->Actions &= ~KeyBoardIDF07; break;
                    case KeyF8: keyBoard->Actions &= ~KeyBoardIDF08; break;
                    case KeyF9: keyBoard->Actions &= ~KeyBoardIDF09; break;
                    case KeyF10: keyBoard->Actions &= ~KeyBoardIDF10; break;
                    case KeyF11: keyBoard->Actions &= ~KeyBoardIDF11; break;
                    case KeyF12: keyBoard->Actions &= ~KeyBoardIDF12; break;
                    case KeyF13: keyBoard->Actions &= ~KeyBoardIDF13; break;
                    case KeyF14: keyBoard->Actions &= ~KeyBoardIDF14; break;
                    case KeyF15: keyBoard->Actions &= ~KeyBoardIDF15; break;
                    case KeyF16: keyBoard->Actions &= ~KeyBoardIDF16; break;
                    case KeyF17: keyBoard->Actions &= ~KeyBoardIDF17; break;
                    case KeyF18: keyBoard->Actions &= ~KeyBoardIDF18; break;
                    case KeyF19: keyBoard->Actions &= ~KeyBoardIDF19; break;
                    case KeyF20: keyBoard->Actions &= ~KeyBoardIDF20; break;
                    case KeyF21: keyBoard->Actions &= ~KeyBoardIDF21; break;
                    case KeyF22: keyBoard->Actions &= ~KeyBoardIDF22; break;
                    case KeyF23: keyBoard->Actions &= ~KeyBoardIDF23; break;
                    case KeyF24: keyBoard->Actions &= ~KeyBoardIDF24; break;
                    case KeyF25: keyBoard->Actions &= ~KeyBoardIDF25; break;

                    default:
                        break;
                }
            }

            PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, PXNull);

            break;
        }
        
        default:
            break;
    }
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

    // Fetch Window input if SYNC
    {
        const PXBool isRunningASYNC = 0;// pxEngine->Window->Win.MessageThread.ThreadID != 0;

        if(!isRunningASYNC)
        {
            PXWindowUpdate(pxEngine->Window);

            PXControllerSystemDevicesDataUpdate(&pxEngine->ControllerSystem);
        }
    }

    // User input
    {
        PXUIElement* pxWindow = pxEngine->Window;
        PXKeyBoard* keyboard = &pxEngine->KeyBoardCurrentInput;
        PXMouse* mouse = &pxEngine->MouseCurrentInput;

        pxEngine->CounterTimeUser = PXTimeCounterStampGet();       


        PXPlayerMoveInfo pxPlayerMoveInfo;    
        PXClear(PXPlayerMoveInfo, &pxPlayerMoveInfo);


        // Solve controller
        PXController* const pxController = &pxEngine->ControllerSystem.DeviceListData[0];

        if(pxEngine->CameraCurrent)
        {
            pxEngine->CameraCurrent->WalkSpeed = 1;
            pxEngine->CameraCurrent->ViewSpeed = 1;

            PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, pxController->AxisNormalised[0] * pxEngine->CameraCurrent->WalkSpeed, 0, -pxController->AxisNormalised[1] * pxEngine->CameraCurrent->WalkSpeed);
            PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementView, -pxController->AxisNormalised[2] * pxEngine->CameraCurrent->ViewSpeed, pxController->AxisNormalised[3] * pxEngine->CameraCurrent->ViewSpeed, 0);


            // Up
            if(pxController->ButtonPressedBitList & PXControllerButton1) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, -1, 0); }

            // Down
            if(pxController->ButtonPressedBitList & PXControllerButton2) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 1, 0); }

            // Update
            if(pxController->ButtonPressedBitList & PXControllerButton3)
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



            PXCameraMove(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementWalk);
            PXCameraRotate(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementView);
            PXCameraUpdate(pxEngine->CameraCurrent, pxEngine->CounterTimeDelta);

            // PXControllerSystemDebugPrint(pxController);





             //---------------------------------------------------------------------------


            if(keyboard->Commands & KeyBoardIDShiftLeft) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, -1, 0); }
            if(keyboard->Letters & KeyBoardIDLetterW) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 0, 1); }
            if(keyboard->Letters & KeyBoardIDLetterA) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, -1, 0, 0); }
            if(keyboard->Letters & KeyBoardIDLetterS) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 0, -1); }
            if(keyboard->Letters & KeyBoardIDLetterD) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 1, 0, 0); }
            if(keyboard->Letters & KeyBoardIDSpace) { PXVector3FAddXYZ(&pxPlayerMoveInfo.MovementWalk, 0, 1, 0); }
            if(keyboard->Letters & KeyBoardIDLetterF && !pxEngine->InteractionLock)
            {
                pxEngine->InteractionLock = PXTrue;
                PXFunctionInvoke(pxEngine->OnInteract, pxEngine->Owner, pxEngine);
            }
            if(!(keyboard->Letters & KeyBoardIDLetterF))
            {
                pxEngine->InteractionLock = PXFalse;
            }


            const PXBool hasViewChanged = mouse->Delta[0] != 0 || mouse->Delta[1] != 0;
            const PXBool hasMoveChanged = mouse->Position[0] != 0 || mouse->Position[1] != 0;
            const PXBool hasAnyChanged = hasViewChanged || hasMoveChanged;

            if(hasAnyChanged)
            {
                pxPlayerMoveInfo.MovementView.X -= mouse->Delta[0];
                pxPlayerMoveInfo.MovementView.Y += mouse->Delta[1];
                pxPlayerMoveInfo.ActionCommit = PXTrue; // Always start with a commit, can be canceled
                pxPlayerMoveInfo.IsWindowInFocus = 1; // PXWindowInteractable(pxWindow);

                PXFunctionInvoke(pxEngine->OnUserUpdate, pxEngine->Owner, pxEngine, &pxPlayerMoveInfo);

                PXMouseInputReset(mouse);

                if(pxPlayerMoveInfo.ActionCommit && pxEngine->CameraCurrent)
                {
                    PXCameraMove(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementWalk);
                    PXCameraRotate(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementView);
                    PXCameraUpdate(pxEngine->CameraCurrent, pxEngine->CounterTimeDelta);

                    // printf("[#][OnMouseMove] X:%5.2f Y:%5.2f, %s\n", pxPlayerMoveInfo.MovementView.X, pxPlayerMoveInfo.MovementView.Y, pxEngine->ApplicationName);
                }

                pxEngine->CounterTimeUser = PXTimeCounterStampGet() - pxEngine->CounterTimeUser;
        }

        
        }

      


        // Extended windows resize check
        if(pxEngine->UpdateUI)
        {
            PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
            PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);
            pxGUIElementUpdateInfo.UIElement = pxWindow;
            pxGUIElementUpdateInfo.Property = PXUIElementPropertySize;

            PXGUIElementFetch(&pxEngine->GUISystem, &pxGUIElementUpdateInfo, 1);



            PXViewPort pxViewPort;
            pxViewPort.X = 0;
            pxViewPort.Y = 0;
            pxViewPort.Width = pxGUIElementUpdateInfo.Data.Size.Width;
            pxViewPort.Height = pxGUIElementUpdateInfo.Data.Size.Height;
            pxViewPort.ClippingMinimum = 0;
            pxViewPort.ClippingMaximum = 1;

            pxEngine->UpdateUI = PXFalse;

            PXFunctionInvoke(pxEngine->Graphic.ViewPortSet, pxEngine->Graphic.EventOwner, &pxViewPort);

            if (pxEngine->CameraCurrent)
            {
                PXCameraAspectRatioChange(pxEngine->CameraCurrent, pxViewPort.Width, pxViewPort.Height);
            }   

            PXGUIElementhSizeRefresAll(&pxEngine->GUISystem);
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


    if ((timeNow - pxEngine->CounterTimeRenderLast) > 0.2)
    {
        pxEngine->CounterTimeRenderLast = timeNow;

        const PXColorRGBAF color = {0.01,0.01,0.01,1};

        {
            PXText pxText;
            PXTextConstructBufferA(&pxText, 128);

            PXTextClear(&pxText);

            const char* date = __DATE__;
            const char* time = __TIME__;

            PXInt32U cpuTemp = 0;

            PXProcessorTemperature(&cpuTemp);

            PXTextPrint(&pxText, "[%s] (Build:%s %s) FPS:%-3i CPU:%i°C", pxEngine->ApplicationName, date, time, pxEngine->FramesPerSecound, cpuTemp);


            PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
            PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);

            pxGUIElementUpdateInfo.UIElement = pxEngine->Window;
            pxGUIElementUpdateInfo.Property = PXUIElementPropertyTextContent;
            pxGUIElementUpdateInfo.Data.Text.Content = pxText.TextA;

            PXGUIElementUpdate(&pxEngine->GUISystem, &pxGUIElementUpdateInfo, 1u);
        }

        if(pxEngine->HasGraphicInterface && pxEngine->Graphic.WindowReference)
        {
            if(IsWindowEnabled(pxEngine->Graphic.WindowReference->ID))
            {
#if 1
                pxEngine->Graphic.Clear(pxEngine->Graphic.EventOwner, &color);
                pxEngine->CounterTimeGPU = PXTimeCounterStampGet();

                PXEngineResourceRenderDefault(pxEngine);

                PXFunctionInvoke(pxEngine->OnRenderUpdate, pxEngine->Owner, pxEngine);
                pxEngine->CounterTimeGPU = PXTimeCounterStampGet() - pxEngine->CounterTimeGPU;
                pxEngine->Graphic.SceneDeploy(pxEngine->Graphic.EventOwner);        
#endif
            }


        }
    }


    ++(pxEngine->CounterTimeWindow);
    ++(pxEngine->CounterTimeUser);
    ++(pxEngine->CounterTimeNetwork);
    ++(pxEngine->CounterTimeCPU);
    ++(pxEngine->CounterTimeGPU);

    //Sleep(20);

    PXThreadYieldToOtherThreads();

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

   // Sleep(1);
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

PXActionResult PXAPI PXEngineStart(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo)
{
    PXClear(PXEngine, pxEngine);

    PXCameraConstruct(&pxEngine->CameraDefault);

    pxEngine->CameraCurrent = &pxEngine->CameraDefault;
    pxEngine->CounterTimeLast = 0;
    pxEngine->CounterTimeGPU = 0;
    pxEngine->CounterTimeCPU = 0;
    pxEngine->FramesPerSecound = 0;
    pxEngine->FrameTime = 0;
    pxEngine->IsRunning = PXFalse;
    pxEngine->HasGraphicInterface = PXFalse;

    // Fetch from start info
    {
        pxEngine->OnStartUp = pxEngineStartInfo->OnStartUp;
        pxEngine->OnShutDown = pxEngineStartInfo->OnShutDown;
        pxEngine->OnUserUpdate = pxEngineStartInfo->OnUserUpdate;
        pxEngine->OnNetworkUpdate = pxEngineStartInfo->OnNetworkUpdate;
        pxEngine->OnGameUpdate = pxEngineStartInfo->OnGameUpdate;
        pxEngine->OnRenderUpdate = pxEngineStartInfo->OnRenderUpdate;
        pxEngine->OnInteract = pxEngineStartInfo->OnInteract;
        pxEngine->Owner = pxEngineStartInfo->Owner;

        PXTextCopyA(pxEngineStartInfo->Name, PXTextLengthUnkown, pxEngine->ApplicationName, 32);
    }


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

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "PX",
        "Instantiate",
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


    PXGUISystemInitialize(&pxEngine->GUISystem);


    // Load all mods now, not fully tho, they may need very early checks before anything happens
    if(pxEngineStartInfo->UseMods)
    {      
        PXText pxText;
        PXTextMakeFixedA(&pxText, "Mod\\");

        PXModLoaderScan(&pxEngine->ModLoader, &pxText);
    }

    //-----------------------------------------------------
	// Create window
    //-----------------------------------------------------
    {
        PXGUIElementCreateInfo pxGUIElementCreateInfo;
        PXClear(PXGUIElementCreateInfo, &pxGUIElementCreateInfo);
        pxGUIElementCreateInfo.UIElement = &pxEngine->Window;
        pxGUIElementCreateInfo.Type = PXUIElementTypeWindow;
        pxGUIElementCreateInfo.StyleFlagList = PXGUIElementStyleDefault;
        pxGUIElementCreateInfo.InteractOwner = pxEngine;
        pxGUIElementCreateInfo.InteractCallBack = PXEngineWindowEvent;
        pxGUIElementCreateInfo.Data.Window.EventOwner = pxEngine;
        pxGUIElementCreateInfo.Data.Window.IsVisible = PXTrue;
        pxGUIElementCreateInfo.Data.Window.BackGroundColor.Red = 38;
        pxGUIElementCreateInfo.Data.Window.BackGroundColor.Green = 38;
        pxGUIElementCreateInfo.Data.Window.BackGroundColor.Blue = 38;
        pxGUIElementCreateInfo.Data.Window.BackGroundColor.Alpha = 0xFF;
        pxGUIElementCreateInfo.Data.Window.Width = pxEngineStartInfo->Width;
        pxGUIElementCreateInfo.Data.Window.Height = pxEngineStartInfo->Height;
        pxGUIElementCreateInfo.Data.Window.Title = "[N/A]";

        switch(pxEngineStartInfo->Mode)
        {
            case PXGraphicInitializeModeWindowless:
            {
                pxGUIElementCreateInfo.Data.Window.IsVisible = PXFalse;

                PXGUIElementCreate(&pxEngine->GUISystem, &pxGUIElementCreateInfo, 1u);
                break;
            }
            case PXGraphicInitializeModeOSGUIElement:
            {
                // ... ?

                // We dont need to create any UI resource, it has been created already

                pxEngine->Window = pxEngineStartInfo->UIElement;

                break;
            }
            case PXGraphicInitializeModeOSGUI:
            case PXGraphicInitializeModeWindowfull:
            {
                PXGUIElementCreate(&pxEngine->GUISystem, &pxGUIElementCreateInfo, 1u);
                break;
            }
        }    
    }
    //-----------------------------------------------------



    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    if(pxEngineStartInfo->UseMouseInput)
    {
        PXWindowUpdate(pxEngine->Window);

        PXWindowMouseMovementEnable(pxEngine->Window->ID);

        PXControllerSystemInitilize(&pxEngine->ControllerSystem);
        PXControllerSystemDevicesListRefresh(&pxEngine->ControllerSystem);
    }
    //-----------------------------------------------------



    //-----------------------------------------------------
    // Create graphic instance
    //-----------------------------------------------------
    if(pxEngineStartInfo->Mode != PXGraphicInitializeModeOSGUI) // if we have GDI, we init this later
    {
        PXGraphicInitializeInfo pxGraphicInitializeInfo;
        PXClear(PXGraphicInitializeInfo, &pxGraphicInitializeInfo);
        pxGraphicInitializeInfo.Mode = pxEngineStartInfo->Mode;
        pxGraphicInitializeInfo.GraphicSystem = pxEngineStartInfo->System;      
        pxGraphicInitializeInfo.WindowReference = pxEngine->Window;
        pxGraphicInitializeInfo.Width = -1;
        pxGraphicInitializeInfo.Height = -1;
        pxGraphicInitializeInfo.DirectXVersion = PXGraphicSystemOpenGL;
        pxGraphicInitializeInfo.DirectXDriverType = PXDirectXDriverTypeHardwareDevice;

        const PXActionResult graphicInit = PXGraphicInstantiate(&pxEngine->Graphic, &pxGraphicInitializeInfo);

        if (PXActionSuccessful != graphicInit)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "PX",
                "Instantiate",
                "failed to create graphical instance!\n"
                "Graphics card driver is not able to provide a rendering context."
                "Either the card itself is not capable or a driver is missing and needs to be installed."
            );
#endif

            if(0) // If its really important that the graphic API exists, we can exit now.
            {
                return graphicInit;
            }           
        }

        pxEngine->HasGraphicInterface = PXTrue;


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Instantiate",
            "Created graphical instance"
        );
#endif

        PXFunctionInvoke(pxEngine->Graphic.SwapIntervalSet, pxEngine->Graphic.EventOwner, 1);
        PXFunctionInvoke(pxEngine->Graphic.Select, pxEngine->Graphic.EventOwner);
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Instantiate",
            "Creation of graphical skipped for now"
        );
#endif
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
            "Instantiate",
            "Creating audio instance..."
        );
#endif

        const PXActionResult audioInitResult = PXAudioInitialize(&pxEngine->Audio, PXAudioSystemWindowsDirectSound);

        if (PXActionSuccessful == audioInitResult)
        {
            PXAudioDeviceAmountInfo pxAudioDeviceAmountInfo;

            pxEngine->Audio.DeviceAmount(&pxEngine->Audio, &pxAudioDeviceAmountInfo);

            for (size_t i = 0; i < pxAudioDeviceAmountInfo.DeviceInput; i++)
            {
                PXAudioDevice pxAudioDevice;

                pxEngine->Audio.DeviceFetch(&pxEngine->Audio, PXAudioDeviceTypeInput, i, &pxAudioDevice);
            }

            for (size_t i = 0; i < pxAudioDeviceAmountInfo.DeviceOutput; i++)
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


    pxEngine->IsRunning = PXTrue;

    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "PX",
            "Instantiate",
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




    // Post rendering fixup, camera needs to be ajusted and also the window itself
    PXWindowSizeInfo pxWindowSizeInfo;

    //PXWindowSizeGet(pxEngine->Window->ID, &pxWindowSizeInfo);

   // PXCameraAspectRatioChange(pxEngine->CameraCurrent, pxWindowSizeInfo.Width, pxWindowSizeInfo.Width);
   // PXCameraViewChangeToPerspective(&pxEngine->CameraDefault, 90, PXCameraAspectRatio(&pxEngine->CameraDefault), 0.00, 100000000);
  //  PXCameraUpdate(pxEngine->CameraCurrent, 1);



    return PXActionSuccessful;
}

void PXAPI PXEngineStop(PXEngine* const pxEngine)
{
    PXFunctionInvoke(pxEngine->OnShutDown, pxEngine->Owner, pxEngine);

    PXGraphicRelease(&pxEngine->Graphic);
    //PXWindowDestruct(&pxEngine->Window);
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
                PXNewZerod(PXImage, &pxImage);
                *pxEngineResourceCreateInfo->ObjectReference = pxImage;
            }

            PXImageConstruct(pxImage);

            // Load texture
            {
                PXResourceLoadInfo pxResourceLoadInfo;
                PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.Target = pxImage;
                pxResourceLoadInfo.Type = PXGraphicResourceTypeImage;

                const PXActionResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxText);


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

            if(!pxEngine->Graphic.TextureAction)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "PX",
                    "TextureCube",
                    "Create not possible"
                );
#endif

                return PXActionRefusedAPIIsNotLinked;
            }

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
                PXNewZerod(PXTextureCube, &pxTextureCube);
                *pxEngineResourceCreateInfo->ObjectReference = pxTextureCube;
            }

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


            PXGraphicTexturInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = pxTextureCube;
            pxGraphicTexturInfo.Amount = 1u;
            pxGraphicTexturInfo.Type = PXGraphicTextureTypeCubeContainer;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;
                
            pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);          

            break;
        }
        case PXEngineCreateTypeModel:
        {
            PXModelCreateInfo* const pxModelCreateInfo = &pxEngineResourceCreateInfo->Model;
            PXModel* pxModel = *(PXModel**)pxEngineResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.ModelRegister)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "PX",
                    "Model-Create",
                    "Not possible"
                );
#endif

                return PXActionRefusedAPIIsNotLinked;
            }

            if (!pxModel)
            {
                PXNewZerod(PXModel, &pxModel);
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
            PXDictionaryAdd(&pxEngine->ModelLookUp, &pxModel->ResourceID.PXID, pxModel);

            // Load model
            {
                PXResourceLoadInfo pxResourceLoadInfo;
                PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.Target = pxModel;
                pxResourceLoadInfo.Type = PXGraphicResourceTypeModel;

                PXText pxText;
                PXTextConstructFromAdressA(&pxText, pxEngineResourceCreateInfo->FilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                PXResourceLoad(&pxResourceLoadInfo, &pxText);


                PXFilePathStructure pxFilePathStructure;

                PXFilePathSplitt(&pxText, &pxFilePathStructure);

                PXTextCopyA(pxFilePathStructure.FileName.TextA, pxFilePathStructure.FileName.SizeUsed, pxModel->ResourceID.Name, ResourceIDNameLength);

            }


            // Load additional resources
            {
                // Load textures

                for(size_t i = 0; i < pxModel->MaterialContaierListAmount; i++)
                {
                    PXMaterialContainer* const pxMaterialContainer = &pxModel->MaterialContaierList[i];

                    for(size_t i = 0; i < pxMaterialContainer->MaterialListAmount; i++)
                    {
                        PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[i];

                        PXEngineResourceCreateInfo pxEngineResourceCreateInfo;
                        PXClear(PXEngineResourceCreateInfo, &pxEngineResourceCreateInfo);
                        pxEngineResourceCreateInfo.CreateType = PXEngineCreateTypeTexture2D;
                        pxEngineResourceCreateInfo.ObjectReference = &pxMaterial->DiffuseTexture;
                        pxEngineResourceCreateInfo.FilePath = pxMaterial->DiffuseTextureFilePath;

                        const PXActionResult resultA = PXEngineResourceCreate(pxEngine, &pxEngineResourceCreateInfo);
                    }
                }


            }



            // Setup    
            PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);     

            pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

            // Register
            pxEngine->Graphic.ModelRegister(pxEngine->Graphic.EventOwner, pxModel);
            pxModel->Enabled = 1;

            break;
        }
        case PXEngineCreateTypeTexture2D:
        {
            //PXEngineTexture2DCreateData* const pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;
            PXTexture2D* pxTexture2D = *(PXTexture2D**)pxEngineResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.TextureAction)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Texture-Create",
                    "Not possible",
                    pxEngineResourceCreateInfo->FilePath
                );
#endif

                return PXActionRefusedAPIIsNotLinked;
            }

            if (!pxTexture2D)
            {
                PXNewZerod(PXTexture2D, &pxTexture2D);
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


            PXGraphicTexturInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = pxTexture2D;
            pxGraphicTexturInfo.Amount = 1u;
            pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;

            pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);

            break;
        }
        case PXEngineCreateTypeFont:
        {
            PXEngineFontCreateData* const pxEngineFontCreateData = &pxEngineResourceCreateInfo->Font;
            PXFont* pxFont = *(PXFont**)pxEngineResourceCreateInfo->ObjectReference;

            // create if not exist
            if(!pxFont)
            {
                PXNewZerod(PXFont, &pxFont);
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

                PXResourceLoadInfo pxResourceLoadInfo;
                PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.Target = pxFont;
                pxResourceLoadInfo.Type = PXGraphicResourceTypeFont;

                const PXActionResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxText);

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

                    PXGraphicTexturInfo pxGraphicTexturInfo;
                    pxGraphicTexturInfo.TextureReference = pxFontPage->Texture;
                    pxGraphicTexturInfo.Amount = 1u;
                    pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                    pxGraphicTexturInfo.Action = PXResourceActionCreate;

                    pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
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

            if(!pxEngine->Graphic.ModelRegister)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "PX",
                    "SkyBox-Create",
                    "Not possible"
                );
#endif

                return PXActionRefusedAPIIsNotLinked;
            }

            if (!pxSkyBox)
            {
                PXNewZerod(PXSkyBox, &pxSkyBox);
                *pxEngineResourceCreateInfo->ObjectReference = pxSkyBox;
            }

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
                PXModel* pxModel = PXNull;                
                PXNewZerod(PXModel, &pxModel);
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
                pxModel->VertexBuffer.VertexDataAmount = sizeof(vertexData) / sizeof(float);
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

            pxEngine->DefaultSkyBox = pxSkyBox;

            break;
        }
        case PXEngineCreateTypeSprite:
        {
            PXSpriteCreateEventData* const pxSpriteCreateEventData = &pxEngineResourceCreateInfo->Sprite;
            PXSprite* pxSprite = *(PXSprite**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxSprite)
            {
                PXNewZerod(PXSprite, &pxSprite);
                *pxEngineResourceCreateInfo->ObjectReference = pxSprite;
            }

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



            // Add hibox if needed
            if (pxSpriteCreateEventData->HitBoxCreate)
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfo.Create.CreateType = PXEngineCreateTypeHitBox;
                pxEngineResourceActionInfo.Create.ObjectReference = &pxSpriteCreateEventData->HitBox;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
            }





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
                PXNewZerod(PXEngineText, &pxEngineText);
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
                PXNewZerod(PXEngineTimer, &pxEngineTimer);
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
                PXNewZerod(PXSound, &pxSound);
                *pxEngineResourceCreateInfo->ObjectReference = pxSound;
            }

            PXResourceLoadInfo pxResourceLoadInfo;
            PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
            pxResourceLoadInfo.Target = pxSound;
            pxResourceLoadInfo.Type = PXGraphicResourceTypeSound;
        
            const PXActionResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxTextFilePath);

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
                PXNewZerod(PXEngineSound, &pxEngineSound);
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
            PXGUIElementCreateInfo* const pxGUIElementCreateInfo = &pxEngineResourceCreateInfo->UIElement;

           // pxGUIElementCreateInfo->U

            //pxUIElementCreateData.UIElement.Data.SceneRender.StartInfo->UseMouseInput
            pxGUIElementCreateInfo->Name = pxEngineResourceCreateInfo->Name;
            pxGUIElementCreateInfo->UIElementWindow = pxEngine->Window;
            pxGUIElementCreateInfo->UIElement = pxEngineResourceCreateInfo->ObjectReference;

            PXGUIElementCreate(&pxEngine->GUISystem, pxGUIElementCreateInfo, 1);   

            PXFunctionInvoke(pxEngine->ResourceAdded, pxEngine->Owner, pxEngine, pxEngineResourceCreateInfo);

            break;
        }
        case PXEngineCreateTypeShaderProgram:
        {
            PXShaderProgramCreateData* const pxShaderProgramCreateData = &pxEngineResourceCreateInfo->ShaderProgram;
            PXShaderProgram* pxShaderProgram = *(PXShaderProgram**)pxEngineResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.ShaderProgramCreate)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "PX",
                    "Shader-Create",
                    "Not possible"
                );
#endif

                return PXActionRefusedAPIIsNotLinked;
            }

            // Make sure object exists
            {
                if (!pxShaderProgram)
                {
                    PXNewZerod(PXShaderProgram, &pxShaderProgram);
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


            // Load 
            {
                PXFile vertexShaderFile;
                PXFile fragmentShaderFile;

                {
                    PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
                    PXClear(PXFileOpenFromPathInfo, &pxFileOpenFromPathInfo);
                    pxFileOpenFromPathInfo.FileSize = 0;
                    pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
                    pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                    pxFileOpenFromPathInfo.AllowMapping = PXTrue;
                    pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
                    pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

                    PXTextConstructFromAdressA(&pxFileOpenFromPathInfo.Text, pxShaderProgramCreateData->VertexShaderFilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                    const PXActionResult vertexLoadResult = PXFileOpenFromPath(&vertexShaderFile, &pxFileOpenFromPathInfo);

                    PXActionReturnOnError(vertexLoadResult);

                    PXTextConstructFromAdressA(&pxFileOpenFromPathInfo.Text, pxShaderProgramCreateData->PixelShaderFilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                    const PXActionResult fragmentLoadResult = PXFileOpenFromPath(&fragmentShaderFile, &pxFileOpenFromPathInfo);

                    PXActionReturnOnError(fragmentLoadResult);

                    {
                        PXShader pxShader[2];
                        PXClearList(PXShader, pxShader, 2);
                        pxShader[0].Type = PXShaderTypeVertex;
                        pxShader[0].ContentSize = vertexShaderFile.DataSize;
                        pxShader[0].Content = vertexShaderFile.Data;
                        pxShader[0].ShaderFilePath = pxShaderProgramCreateData->VertexShaderFilePath;

                        pxShader[1].Type = PXShaderTypePixel;
                        pxShader[1].ContentSize = fragmentShaderFile.DataSize;
                        pxShader[1].Content = fragmentShaderFile.Data;
                        pxShader[1].ShaderFilePath = pxShaderProgramCreateData->PixelShaderFilePath;
                                  
                        pxEngine->Graphic.ShaderProgramCreate
                        (
                            pxEngine->Graphic.EventOwner,
                            pxShaderProgram,
                            pxShader,
                            2
                        );                      
                    }

                    PXFilePathStructure pxFilePathStructure;

                    PXFilePathSplitt(&pxFileOpenFromPathInfo.Text, &pxFilePathStructure);

                    PXTextCopyA(pxFilePathStructure.FileName.TextA, pxFilePathStructure.FileName.SizeUsed, pxShaderProgram->ResourceID.Name, ResourceIDNameLength);
                }

                PXFileDestruct(&vertexShaderFile);
                PXFileDestruct(&fragmentShaderFile);
            }

            pxShaderProgram->ResourceID.PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->ShaderProgramLookup, &pxShaderProgram->ResourceID.PXID, pxShaderProgram);

            break;
        }
        case PXEngineCreateTypeDialogBox:
        {





            break;
        }
        case PXEngineCreateTypeHitBox:
        {
            PXHitBox* pxHitBox = *(PXHitBox**)pxEngineResourceCreateInfo->ObjectReference;

            if (!pxHitBox)
            {
                PXNewZerod(PXHitBox, &pxHitBox);
                *pxEngineResourceCreateInfo->ObjectReference = pxHitBox;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "HitBox-Create",
                ""
            );
#endif

            pxHitBox->PXID = PXEngineGenerateUniqeID(pxEngine);
            PXDictionaryAdd(&pxEngine->HitBoxLookUp, &pxHitBox->PXID, pxHitBox);

            pxHitBox->Enabled = PXTrue;

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
            PXModel* const pxModel = pxEngineResourceRenderInfo->ObjectReference;

            if (!pxModel->Enabled)
            {
                break;
            }

            pxEngine->Graphic.ModelDraw(pxEngine->Graphic.EventOwner, pxModel, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXEngineCreateTypeSkybox:
        {
            PXSkyBox* const pxSkyBox = pxEngineResourceRenderInfo->ObjectReference;

            PXOpenGLSkyboxDraw(&pxEngine->Graphic.OpenGLInstance, pxSkyBox, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXEngineCreateTypeSprite:
        {
            PXSprite* const pxSprite = pxEngineResourceRenderInfo->ObjectReference;

            if (!pxSprite->Enabled)
            {
                break;
            }

            PXGraphicSpriteDraw(&pxEngine->Graphic, pxSprite, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXEngineCreateTypeHitBox:
        {
            PXHitBox* const pxHitBox = pxEngineResourceRenderInfo->ObjectReference;

            PXOpenGLBlendingMode(&pxEngine->Graphic.OpenGLInstance, PXBlendingModeOneToOne);

            pxEngine->Graphic.DrawColorRGBAF
            (
                pxEngine->Graphic.EventOwner,
                0,
                1,
                0,
                1
            );
            pxEngine->Graphic.RectangleDraw
            (
                pxEngine->Graphic.EventOwner,
                -0.5,
                -0.5,
                0.5,
                0.5,
                0x01
            );

            break;
        }
        case PXEngineCreateTypeText:
        {
            PXGraphic* const pxGraphic = &pxEngine->Graphic;
            PXEngineText* const pxEngineText = pxEngineResourceRenderInfo->ObjectReference;

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

                {
                    PXGraphicTexturInfo pxGraphicTexturInfo;
                    pxGraphicTexturInfo.TextureReference = pxFont->MainPage.Texture;
                    pxGraphicTexturInfo.Amount = 1u;
                    pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                    pxGraphicTexturInfo.Action = PXResourceActionSelect;

                    pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                }

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
                            {
                                PXGraphicTexturInfo pxGraphicTexturInfo;
                                pxGraphicTexturInfo.TextureReference = PXNull;
                                pxGraphicTexturInfo.Amount = 1u;
                                pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                                pxGraphicTexturInfo.Action = PXResourceActionSelect;

                                pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                            }

                            pxGraphic->RectangleDraw(pxGraphic->EventOwner, x1, y1, x2, y2, 0x01);


                            {
                                PXGraphicTexturInfo pxGraphicTexturInfo;
                                pxGraphicTexturInfo.TextureReference = pxFont->MainPage.Texture;
                                pxGraphicTexturInfo.Amount = 1u;
                                pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                                pxGraphicTexturInfo.Action = PXResourceActionSelect;

                                pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                            }
                                            
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
                            {
                                PXGraphicTexturInfo pxGraphicTexturInfo;
                                pxGraphicTexturInfo.TextureReference = PXNull;
                                pxGraphicTexturInfo.Amount = 1u;
                                pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                                pxGraphicTexturInfo.Action = PXResourceActionSelect;

                                pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                            }

                            pxGraphic->RectangleDraw(pxGraphic->EventOwner, x1, y1, x2, y2, 0x01);
  
                            {
                                PXGraphicTexturInfo pxGraphicTexturInfo;
                                pxGraphicTexturInfo.TextureReference = pxFont->MainPage.Texture;
                                pxGraphicTexturInfo.Amount = 1u;
                                pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                                pxGraphicTexturInfo.Action = PXResourceActionSelect;

                                pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                            }
                        }
                    }


#endif // Text debug

                }


                offsetShadowCurrent.X -= shadowOffset.X;
                offsetShadowCurrent.Y -= shadowOffset.Y;

                {
                    PXGraphicTexturInfo pxGraphicTexturInfo;
                    pxGraphicTexturInfo.TextureReference = PXNull;
                    pxGraphicTexturInfo.Amount = 1u;
                    pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
                    pxGraphicTexturInfo.Action = PXResourceActionSelect;

                    pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
                }

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

PXActionResult PXAPI PXEngineResourceRenderDefault(PXEngine* const pxEngine)
{
    {
        PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
        pxEngineResourceRenderInfo.Type = PXEngineCreateTypeSkybox;
        pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
        pxEngineResourceRenderInfo.ObjectReference = pxEngine->DefaultSkyBox;

        PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
    }

    // Sprite
    {
        PXDictionary* const modelLookup = &pxEngine->ModelLookUp;

        for(PXSize i = 0; i < modelLookup->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXModel* pxModel = PXNull;

            PXDictionaryIndex(modelLookup, i, &pxDictionaryEntry);

            pxModel = *(PXModel**)pxDictionaryEntry.Value;

            if(!pxModel->Enabled)
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXEngineCreateTypeModel;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxModel;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }


    // Sprite
    {
        PXDictionary* const spirteList = &pxEngine->SpritelLookUp;

        for(PXSize i = 0; i < spirteList->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXSprite* pxSprite = PXNull;

            PXDictionaryIndex(spirteList, i, &pxDictionaryEntry);

            pxSprite = *(PXSprite**)pxDictionaryEntry.Value;

            if(!pxSprite->Enabled)
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXEngineCreateTypeSprite;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxSprite;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }

    // Text
    {
        PXDictionary* const textList = &pxEngine->TextLookUp;

        for(PXSize i = 0; i < textList->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXEngineText* pxEngineText = PXNull;

            PXDictionaryIndex(textList, i, &pxDictionaryEntry);

            pxEngineText = *(PXEngineText**)pxDictionaryEntry.Value;

            if(!pxEngineText->Enabled)
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXEngineCreateTypeText;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxEngineText;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }

    // HitBoxes
    {
        PXDictionary* const hitBoxList = &pxEngine->HitBoxLookUp;

        for(PXSize i = 0; i < hitBoxList->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXHitBox* pxHitBox = PXNull;

            PXDictionaryIndex(hitBoxList, i, &pxDictionaryEntry);

            pxHitBox = *(PXHitBox**)pxDictionaryEntry.Value;

            if(!pxHitBox->Enabled)
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXEngineCreateTypeHitBox;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxHitBox;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }

    return PXActionSuccessful;
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
    if (pxSprite->Model.IndexBuffer.SegmentListAmount > 0)
    {
        PXMaterial* materiial = pxSprite->Model.IndexBuffer.SegmentPrime.Material;

        materiial->DiffuseTexture = pxTexture2D;
    }
    else
    {
        PXMaterial* materiial = PXNull;
        PXNewZerod(PXMaterial, &materiial);

        materiial->DiffuseTexture = pxSprite->Texture;

        pxSprite->Model.IndexBuffer.SegmentListAmount = 1;
        pxSprite->Model.IndexBuffer.SegmentPrime.Material = materiial;
    }

    return PXActionSuccessful;
}