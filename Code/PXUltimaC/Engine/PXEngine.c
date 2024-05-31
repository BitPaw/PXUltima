#include "PXEngine.h"

#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>
#include <OS/Async/PXProcess.h>
#include <OS/File/PXFile.h>
#include <Engine/Dialog/PXDialogBox.h>
#include <Media/ADLER/PXAdler32.h>
#include <OS/Hardware/PXProcessor.h>

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

#define UseOSDelta 0

void PXAPI PXEngineWindowEvent(PXEngine* const pxEngine, PXWindowEvent* const pxWindowEvent)
{
    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeResize:
        {
            pxEngine->UpdateUI = PXTrue;            
            break;
        }

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

    pxEngine->TimeData.CounterTimeDelta = timeNow - pxEngine->TimeData.CounterTimeLast;
    pxEngine->TimeData.CounterTimeLast = timeNow;
    pxEngine->TimeData.TimeFrequency = PXTimeCounterFrequencyGet();
    pxEngine->TimeData.FramesPerSecound = PXMathFloor(pxEngine->TimeData.TimeFrequency / (float)pxEngine->TimeData.CounterTimeDelta);
    pxEngine->TimeData.FrameTime = PXMathCeilingF((1000000 * pxEngine->TimeData.CounterTimeDelta) / (float)pxEngine->TimeData.TimeFrequency);

    pxEngine->TimeData.CounterTimeWindow = PXTimeCounterStampGet();
    //PXWindowUpdate(&pxEngine->Window);
    pxEngine->TimeData.CounterTimeWindow = PXTimeCounterStampGet() - pxEngine->TimeData.CounterTimeWindow;

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

        pxEngine->TimeData.CounterTimeUser = PXTimeCounterStampGet();


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

            // If we have a target, we want to move the camera to it to track it
            if(pxEngine->CameraCurrent->Target)
            {
                PXMatrix4x4FMove3F(pxEngine->CameraCurrent->Target, &pxPlayerMoveInfo.MovementWalk);

                PXCameraFollow(pxEngine->CameraCurrent, pxEngine->TimeData.CounterTimeDelta / 1000000.0f);
            }
            else
            {
                PXCameraMove(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementWalk);         
            }

           // PXCameraMove(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementWalk);
       
            PXCameraRotate(pxEngine->CameraCurrent, &pxPlayerMoveInfo.MovementView);
            PXCameraUpdate(pxEngine->CameraCurrent, pxEngine->TimeData.CounterTimeDelta);


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
                    PXCameraUpdate(pxEngine->CameraCurrent, pxEngine->TimeData.CounterTimeDelta);

                    // printf("[#][OnMouseMove] X:%5.2f Y:%5.2f, %s\n", pxPlayerMoveInfo.MovementView.X, pxPlayerMoveInfo.MovementView.Y, pxEngine->ApplicationName);
                }

                pxEngine->TimeData.CounterTimeUser = PXTimeCounterStampGet() - pxEngine->TimeData.CounterTimeUser;
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
        pxEngine->TimeData.CounterTimeNetwork = PXTimeCounterStampGet();
        PXFunctionInvoke(pxEngine->OnNetworkUpdate, pxEngine->Owner, pxEngine);
        pxEngine->TimeData.CounterTimeNetwork = PXTimeCounterStampGet() - pxEngine->TimeData.CounterTimeNetwork;
    }

    // Timer
    {
        PXDictionary* const timerList = &pxEngine->ResourceManager.TimerLookUp;

        for (PXSize timerIndex = 0; timerIndex < timerList->EntryAmountCurrent; ++timerIndex)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXEngineTimer* pxEngineTimer = PXNull;

            PXDictionaryIndex(timerList, timerIndex, &pxDictionaryEntry);

            pxEngineTimer = *(PXEngineTimer**)pxDictionaryEntry.Value;

            if (!(pxEngineTimer->Info.Flags & PXEngineResourceInfoEnabled))
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
        pxEngine->TimeData.CounterTimeCPU = PXTimeCounterStampGet();
        PXFunctionInvoke(pxEngine->OnGameUpdate, pxEngine->Owner, pxEngine);
        pxEngine->TimeData.CounterTimeCPU = PXTimeCounterStampGet() - pxEngine->TimeData.CounterTimeCPU;
    }


    if ((timeNow - pxEngine->TimeData.CounterTimeRenderLast) > 0.2)
    {
        pxEngine->TimeData.CounterTimeRenderLast = timeNow;

        const PXColorRGBAF color = {0.01,0.01,0.01,1};

        {
            PXText pxText;
            PXTextConstructBufferA(&pxText, 128);

            PXTextClear(&pxText);

            const char* date = __DATE__;
            const char* time = __TIME__;

            PXInt32U cpuTemp = 0;

            PXProcessorTemperature(&cpuTemp);

            PXTextPrint(&pxText, "[%s] (Build:%s %s) FPS:%-3i CPU:%i°C", pxEngine->ApplicationName, date, time, pxEngine->TimeData.FramesPerSecound, cpuTemp);


            PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
            PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);

            pxGUIElementUpdateInfo.UIElement = pxEngine->Window;
            pxGUIElementUpdateInfo.Property = PXUIElementPropertyTextContent;
            pxGUIElementUpdateInfo.Data.Text.Content = pxText.TextA;

            PXGUIElementUpdate(&pxEngine->GUISystem, &pxGUIElementUpdateInfo, 1u);
        }

        if(pxEngine->HasGraphicInterface && pxEngine->Graphic.WindowReference)
        {
            if(IsWindowEnabled(pxEngine->Graphic.WindowReference->Info.WindowID))
            {
#if 1
                pxEngine->Graphic.Clear(pxEngine->Graphic.EventOwner, &color);
                pxEngine->TimeData.CounterTimeGPU = PXTimeCounterStampGet();

                PXEngineResourceRenderDefault(pxEngine);

                PXFunctionInvoke(pxEngine->OnRenderUpdate, pxEngine->Owner, pxEngine);
                pxEngine->TimeData.CounterTimeGPU = PXTimeCounterStampGet() - pxEngine->TimeData.CounterTimeGPU;
                pxEngine->Graphic.SceneDeploy(pxEngine->Graphic.EventOwner);        
#endif
            }


        }
    }


    ++(pxEngine->TimeData.CounterTimeWindow);
    ++(pxEngine->TimeData.CounterTimeUser);
    ++(pxEngine->TimeData.CounterTimeNetwork);
    ++(pxEngine->TimeData.CounterTimeCPU);
    ++(pxEngine->TimeData.CounterTimeGPU);

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

    Sleep(1);
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
            /*
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

                    if(pxEngineResourceStateChangeInfo->Enable)
                    {
                        pxEngineText->Info.Flags |= PXEngineResourceInfoEnabled;
                    }                 

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

                    if(pxEngineResourceStateChangeInfo->Enable)
                    {
                        pxEngineTimer->Info.Flags |= PXEngineResourceInfoEnabled;
                    }

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
                */
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
    pxEngine->TimeData.CounterTimeLast = 0;
    pxEngine->TimeData.CounterTimeGPU = 0;
    pxEngine->TimeData.CounterTimeCPU = 0;
    pxEngine->TimeData.FramesPerSecound = 0;
    pxEngine->TimeData.FrameTime = 0;
    pxEngine->IsRunning = PXFalse;
    pxEngine->HasGraphicInterface = PXFalse;

    pxEngine->CameraCurrent->ViewSpeed = 1;
    PXCameraRotateXYZ(pxEngine->CameraCurrent, 80, 0, 0);

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

    PXResourceManagerInit(&pxEngine->ResourceManager);

    PXMathRandomeSeed(&pxEngine->RandomGeneratorSeed);

    PXGUISystemInitialize(&pxEngine->GUISystem);
    pxEngine->GUISystem.ResourceManager = &pxEngine->ResourceManager;

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
        PXResourceCreateInfo pxResourceCreateInfo;
        PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.ObjectReference = &pxEngine->Window;
        pxResourceCreateInfo.Type = PXResourceTypeGUIElement;
        pxResourceCreateInfo.UIElement.Type = PXUIElementTypeWindow;
        pxResourceCreateInfo.UIElement.StyleFlagList = PXGUIElementStyleDefault;
        pxResourceCreateInfo.UIElement.InteractOwner = pxEngine;
        pxResourceCreateInfo.UIElement.InteractCallBack = PXEngineWindowEvent;
        pxResourceCreateInfo.UIElement.Data.Window.EventOwner = pxEngine;
        pxResourceCreateInfo.UIElement.Data.Window.IsVisible = PXTrue;
        pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Red = 38;
        pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Green = 38;
        pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Blue = 38;
        pxResourceCreateInfo.UIElement.Data.Window.BackGroundColor.Alpha = 0xFF;
        pxResourceCreateInfo.UIElement.Data.Window.Width = pxEngineStartInfo->Width;
        pxResourceCreateInfo.UIElement.Data.Window.Height = pxEngineStartInfo->Height;
        pxResourceCreateInfo.UIElement.Data.Window.Title = "[N/A]";

        switch(pxEngineStartInfo->Mode)
        {
            case PXGraphicInitializeModeWindowless:
            {
                pxResourceCreateInfo.UIElement.Data.Window.IsVisible = PXFalse;

                PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo, 1u);
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
                PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo, 1u);
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

        PXWindowMouseMovementEnable(pxEngine->Window->Info.WindowID);

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


    if(pxEngineStartInfo->Mode != PXGraphicInitializeModeOSGUI)
    {
        PXEngineResourceDefaultElements(pxEngine);
    }




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

PXActionResult PXAPI PXEngineResourceCreate(PXEngine* const pxEngine, PXResourceCreateInfo* const pxResourceCreateInfo)
{
    if (!(pxEngine && pxResourceCreateInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    PXResourceManagerAdd(&pxEngine->ResourceManager, pxResourceCreateInfo, 1);

    switch (pxResourceCreateInfo->Type)
    {
        case PXResourceTypeImage:
        {
          
            break;
        }
        case PXResourceTypeTextureCube:
        {
            PXTextureCubeCreateInfo* const pxTextureCubeCreateData = &pxResourceCreateInfo->TextureCube;
            PXTextureCube* pxTextureCube = *(PXTextureCube**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.TextureAction)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "Resource",
                    "TextureCube-Create",
                    "Not possible"
                );
#endif

                return PXActionFailedNoSystemToHandleData;
            }

            PXGraphicTexturInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = pxTextureCube;
            pxGraphicTexturInfo.Amount = 1u;
            pxGraphicTexturInfo.Type = PXGraphicTextureTypeCubeContainer;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;
                
            pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);    

            break;
        }
        case PXResourceTypeModel:
        {
            PXModelCreateInfo* const pxModelCreateInfo = &pxResourceCreateInfo->Model;
            PXModel* pxModel = *(PXModel**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.ModelRegister)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "Engine",
                    "Model-Create",
                    "Not possible"
                );
#endif

                return PXActionFailedNoSystemToHandleData;
            }

            // Register
            pxEngine->Graphic.ModelRegister(pxEngine->Graphic.EventOwner, pxModel);

            break;
        }
        case PXResourceTypeTexture2D:
        {
            //PXEngineTexture2DCreateData* const pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;
            PXTexture2D* pxTexture2D = *(PXTexture2D**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngine->Graphic.TextureAction)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Texture-Create",
                    "Not possible",
                    pxResourceCreateInfo->FilePath
                );
#endif

                return PXActionFailedNoSystemToHandleData;
            }         


            PXGraphicTexturInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = &pxTexture2D;
            pxGraphicTexturInfo.Amount = 1u;
            pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;

            pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);

            break;
        }
        case PXResourceTypeFont:
        {
            PXEngineFontCreateInfo* const pxEngineFontCreateData = &pxResourceCreateInfo->Font;
            PXFont* pxFont = *(PXFont**)pxResourceCreateInfo->ObjectReference;

            // Register as normal
            {
                // Load Textures
                {
                    PXFontPage* const pxFontPage = &pxFont->MainPage;                   

                    PXResourceCreateInfo pxResourceCreateInfoSub;
                    PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);

                    pxResourceCreateInfoSub.Type = PXResourceTypeTexture2D;
                    pxResourceCreateInfoSub.ObjectReference = (void**)&pxFontPage->Texture;
                    pxResourceCreateInfoSub.FilePath = pxFontPage->TextureFilePath;

                    PXEngineResourceCreate(pxEngine, &pxResourceCreateInfoSub, 1);
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
            }

            break;
        }
        case PXResourceTypeSkybox:
        {
            PXSkyBoxCreateEventInfo* const pxSkyBoxCreateEventData = &pxResourceCreateInfo->SkyBox;
            PXSkyBox* pxSkyBox = *(PXSkyBox**)pxResourceCreateInfo->ObjectReference;

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

                return PXActionFailedNoSystemToHandleData;
            }

        
            // Regsieter texture
            {
                PXGraphicTexturInfo pxGraphicTexturInfo;
                pxGraphicTexturInfo.TextureReference = &pxSkyBox->TextureCube;
                pxGraphicTexturInfo.Amount = 1u;
                pxGraphicTexturInfo.Type = PXGraphicTextureTypeCubeContainer;
                pxGraphicTexturInfo.Action = PXResourceActionCreate;

                pxEngine->Graphic.TextureAction(pxEngine->Graphic.EventOwner, &pxGraphicTexturInfo);
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
                //pxModel->VertexBuffer.VertexDataAmount = sizeof(vertexData) / sizeof(float);
                //pxModel->VertexBuffer.VertexDataRowSize = 3;
                pxModel->VertexBuffer.Format = PXVertexBufferFormatXYZFloat; // PXVertexBufferFormatXYZC  PXVertexBufferFormatXYZHWC

                pxModel->IndexBuffer.IndexDataType = PXDataTypeInt08U;
                pxModel->IndexBuffer.IndexData = (void*)indexList;
                pxModel->IndexBuffer.IndexDataSize = sizeof(indexList);
                pxModel->IndexBuffer.DrawModeID = 0 |// PXDrawModeIDPoint | PXDrawModeIDLineLoop |
#if QuadSkybox
                    PXDrawModeIDSquare
#else
                    PXDrawModeIDTriangle
#endif
                    ;

                pxEngine->Graphic.ModelRegister(pxEngine->Graphic.EventOwner, pxModel);
            }

            // Shader create
            {
                PXShader pxShader[2];
                PXClearList(PXShader, pxShader, 2);
                pxShader[0].Type = PXShaderTypeVertex;
                pxShader[0].ShaderFile = &pxSkyBoxCreateEventData->ShaderProgramCreateInfo.ShaderVertexFile;

                pxShader[1].Type = PXShaderTypePixel;
                pxShader[1].ShaderFile = &pxSkyBoxCreateEventData->ShaderProgramCreateInfo.ShaderPixelFile;

                pxEngine->Graphic.ShaderProgramCreate
                (
                    pxEngine->Graphic.EventOwner,
                    pxSkyBox->ShaderProgramReference,
                    pxShader,
                    2
                );
            }            

            pxSkyBox->Info.Flags |= PXEngineResourceInfoVisble;

            break;
        }
        case PXResourceTypeSprite:
        {
            PXSpriteCreateInfo* const pxSpriteCreateEventData = &pxResourceCreateInfo->Sprite;
            PXSprite* pxSprite = *(PXSprite**)pxResourceCreateInfo->ObjectReference;

            break;
        }
        case PXResourceTypeText:
        {
            //PXEngineTextCreateData* const pxEngineTextCreateData = &pxEngineResourceCreateInfo->Text;
            PXEngineText* pxEngineText = *(PXEngineText**)pxResourceCreateInfo->ObjectReference;

            break;
        }
        case PXResourceTypeTimer:
        {
            PXEngineTimer* pxEngineTimer = *(PXEngineTimer**)pxResourceCreateInfo->ObjectReference;


            break;
        }
        case PXResourceTypeSound:
        {
            PXSound* pxSound = *(PXSound**)pxResourceCreateInfo->ObjectReference;


            break;
        }
        case PXResourceTypeEngineSound:
        {
            PXEngineSoundCreateInfo* const pxEngineSoundCreateInfo = &pxResourceCreateInfo->Sound;
            PXEngineSound* pxEngineSound = *(PXEngineSound**)pxResourceCreateInfo->ObjectReference;

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


            break;
        }
        case PXResourceTypeGUIElement:
        {            
            PXGUIElementCreateInfo* const pxGUIElementCreateInfo = &pxResourceCreateInfo->UIElement;

           // pxGUIElementCreateInfo->U

            //pxUIElementCreateData.UIElement.Data.SceneRender.StartInfo->UseMouseInput
            pxGUIElementCreateInfo->Name = pxResourceCreateInfo->Name;
            pxGUIElementCreateInfo->UIElementWindow = pxEngine->Window;

            PXGUIElementCreate(&pxEngine->GUISystem, pxResourceCreateInfo, 1);

            PXFunctionInvoke(pxEngine->ResourceAdded, pxEngine->Owner, pxEngine, pxResourceCreateInfo);
            
            break;
        }
        case PXResourceTypeShaderProgram:
        {
            PXShaderProgramCreateInfo* const pxShaderProgramCreateData = &pxResourceCreateInfo->ShaderProgram;
            PXShaderProgram* pxShaderProgram = *(PXShaderProgram**)pxResourceCreateInfo->ObjectReference;

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

                return PXActionFailedNoSystemToHandleData;
            }

            {
                PXShader pxShader[2];
                PXClearList(PXShader, pxShader, 2);
                pxShader[0].Type = PXShaderTypeVertex;
                pxShader[0].ShaderFile = &pxShaderProgramCreateData->ShaderVertexFile;

                pxShader[1].Type = PXShaderTypePixel;
                pxShader[1].ShaderFile = &pxShaderProgramCreateData->ShaderPixelFile;

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
        case PXResourceTypeHitBox:
        {
            PXHitBox* pxHitBox = *(PXHitBox**)pxResourceCreateInfo->ObjectReference;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "HitBox-Create",
                "ID:%i",
                pxHitBox->Info.ID
            );
#endif

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

    if(!pxEngineResourceRenderInfo->ObjectReference)
    {
        return PXActionRefusedArgumentInvalid;
    }

#if 1
    switch (pxEngineResourceRenderInfo->Type)
    {
        case PXResourceTypeModel:
        {
            PXModel* const pxModel = pxEngineResourceRenderInfo->ObjectReference;

            if(!(pxModel->Info.Flags & PXEngineResourceInfoVisble))
            {
                break; // Skip rendering
            }

            pxEngine->Graphic.ModelDraw(pxEngine->Graphic.EventOwner, pxModel, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXResourceTypeSkybox:
        {
            PXSkyBox* const pxSkyBox = pxEngineResourceRenderInfo->ObjectReference;

            if(!(pxSkyBox->Info.Flags & PXEngineResourceInfoVisble))
            {
                break; // Skip rendering
            }

            PXOpenGLSkyboxDraw(&pxEngine->Graphic.OpenGLInstance, pxSkyBox, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXResourceTypeSprite:
        {
            PXSprite* const pxSprite = pxEngineResourceRenderInfo->ObjectReference;

            if(!(pxSprite->Info.Flags & PXEngineResourceInfoVisble))
            {
                break; // Skip rendering
            }

            PXGraphicSpriteDraw(&pxEngine->Graphic, pxSprite, pxEngineResourceRenderInfo->CameraReference);

            break;
        }
        case PXResourceTypeHitBox:
        {
            PXHitBox* const pxHitBox = (PXHitBox*)pxEngineResourceRenderInfo->ObjectReference;

            if(!(pxHitBox->Info.Flags & PXEngineResourceInfoVisble))
            {
                break; // Skip rendering
            }


           // PXOpenGLBlendingMode(&pxEngine->Graphic.OpenGLInstance, PXBlendingModeOneToOne);

            PXBool isEnabled = !(pxHitBox->Info.Flags & PXEngineResourceInfoEnabled);

            float color[4];

            // Store previous state


            PXInt32U drawstateBefore = pxHitBox->Model->IndexBuffer.DrawModeID;

            PXMaterial* material = pxHitBox->Model->IndexBuffer.SegmentPrime.Material;   

            if(material == PXNull)
            {
                PXNewZerod(PXMaterial, &material);
                //PXNewZerod(PXMaterialContainer, &pxHitBox->Model->MaterialContaierList);
             
                pxHitBox->Model->IndexBuffer.SegmentPrime.DataRange = 1;
                pxHitBox->Model->IndexBuffer.SegmentPrime.Material = material;

               // pxHitBox->Model->MaterialContaierList[0].MaterialListAmount = 1;

                PXTextCopyA("Wonk", 4, material->Name, 64);

                //PXCopy(PXMaterial, pxHitBox->Model->MaterialContaierList[0].MaterialList, &materialCopy);

               // pxHitBox->Model->IndexBuffer.SegmentPrime.Material = pxHitBox->Model->MaterialContaierList[0].MaterialList;
            }


          



            //pxHitBox->Model->IndexBuffer.DrawModeID = PXDrawModeIDLineLoop;

            // Draw red box
            if(pxHitBox->Info.Behaviour & PXHitBoxBehaviourKeepOut)
            {
                // Red
                material->Diffuse[0] = 1;
                material->Diffuse[1] = 0;
                material->Diffuse[2] = 0;
                material->Diffuse[3] = 0.4;
      
                PXOpenGLModelDraw(&pxEngine->Graphic.OpenGLInstance, pxHitBox->Model, pxEngineResourceRenderInfo->CameraReference);
            }

            // Red border
            if(pxHitBox->Info.Behaviour & PXHitBoxBehaviourKeepIn)
            {
                material->Diffuse[0] = 1;
                material->Diffuse[1] = 0;
                material->Diffuse[2] = 0;
                material->Diffuse[3] = 0.4;

                PXOpenGLModelDraw(&pxEngine->Graphic.OpenGLInstance, pxHitBox->Model, pxEngineResourceRenderInfo->CameraReference);
            }


            // Blue border
            if(pxHitBox->Info.Behaviour & PXHitBoxBehaviourDetect)
            {
                material->Diffuse[0] = 0;
                material->Diffuse[1] = 0;
                material->Diffuse[2] = 1;
                material->Diffuse[3] = 0.4;

                PXOpenGLModelDraw(&pxEngine->Graphic.OpenGLInstance, pxHitBox->Model, pxEngineResourceRenderInfo->CameraReference);
            }

            material->Diffuse[0] = 1;
            material->Diffuse[1] = 1;
            material->Diffuse[2] = 1;
            material->Diffuse[3] = 1;

            pxHitBox->Model->IndexBuffer.DrawModeID = drawstateBefore;

            break;
        }
        case PXResourceTypeText:
        {
            PXGraphic* const pxGraphic = &pxEngine->Graphic;
            PXEngineText* const pxEngineText = pxEngineResourceRenderInfo->ObjectReference;

            if (!pxEngineText->Text)
            {
                break; // No text, no content to render, done
            }

            PXText* const pxText = pxEngineText->Text;
            PXFont* const pxFont = pxEngineText->Font;

            if (!pxEngineText->Info.Flags & PXEngineResourceInfoEnabled)
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

#endif
}

void PXAPI PXEngineResourceDefaultElements(PXEngine* const pxEngine)
{
    PXResourceCreateInfo pxResourceCreateInfo;

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
        
        PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.Type = PXResourceTypeShaderProgram;
        pxResourceCreateInfo.ObjectReference = &pxEngine->ResourceManager.ShaderFailback;
        pxResourceCreateInfo.ShaderProgram.ShaderVertexText = vertexShaderData;
        pxResourceCreateInfo.ShaderProgram.ShaderVertexTextSize = sizeof(vertexShaderData) - 1;
        pxResourceCreateInfo.ShaderProgram.ShaderPixelText = pixelShaderData;
        pxResourceCreateInfo.ShaderProgram.ShaderPixelTextSize = sizeof(pixelShaderData) - 1;

        PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);
    }

    // Create missing object model
    {
        const float vertexData[] =
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

        PXInt8U indexList[] =
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


        for(size_t i = 0; i < sizeof(indexList); i++)
        {
            --indexList[i];
        }

        PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.Type = PXResourceTypeModel;
        pxResourceCreateInfo.ObjectReference = &pxEngine->ResourceManager.ModelFailback;

        pxResourceCreateInfo.Model.VertexBuffer.VertexData = vertexData;
        pxResourceCreateInfo.Model.VertexBuffer.VertexDataSize = sizeof(vertexData);
        pxResourceCreateInfo.Model.VertexBuffer.Format = PXVertexBufferFormatXYZFloat;

        pxResourceCreateInfo.Model.IndexBuffer.IndexData = indexList;
        pxResourceCreateInfo.Model.IndexBuffer.IndexDataSize = sizeof(indexList);
        pxResourceCreateInfo.Model.IndexBuffer.IndexDataType = PXDataTypeInt08U;
        pxResourceCreateInfo.Model.IndexBuffer.DrawModeID = PXDrawModeIDTriangle;

        pxResourceCreateInfo.Model.ShaderProgramReference = pxEngine->ResourceManager.ShaderFailback;

        PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);

        PXMatrix4x4FScaleBy(&pxEngine->ResourceManager.ModelFailback->ModelMatrix, 20);
    }

    // Create missing texture 
    {
        const PXColorRGBI8 pink = { 165, 5, 250 };
        const PXColorRGBI8 black = { 30, 30, 30 };

        const PXInt8U colorData[4 * 4] =
        {
            pink.Red, pink.Green, pink.Blue,
            black.Red, black.Green, black.Blue,
            black.Red, black.Green, black.Blue,
            pink.Red, pink.Green, pink.Blue
        };

        PXTexture2D* failbackTexture = 0;

        PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
        pxResourceCreateInfo.Type = PXResourceTypeTexture2D;
        pxResourceCreateInfo.ObjectReference = &pxEngine->ResourceManager.Texture2DFailBack;
        pxResourceCreateInfo.Texture2D.Image.Image.PixelData = colorData;
        pxResourceCreateInfo.Texture2D.Image.Image.PixelDataSize = sizeof(colorData);
        pxResourceCreateInfo.Texture2D.Image.Image.Width = 2;
        pxResourceCreateInfo.Texture2D.Image.Image.Height = 2;
        pxResourceCreateInfo.Texture2D.Image.Image.Depth = 1;
        pxResourceCreateInfo.Texture2D.Image.Image.Format = PXColorFormatRGBI8;

        PXEngineResourceCreate(pxEngine, &pxResourceCreateInfo);
    }

    PXMaterial* material;

    PXNew(PXMaterial, &material);

    pxEngine->ResourceManager.ModelFailback->IndexBuffer.SegmentPrime.Material = material;
    material->DiffuseTexture = pxEngine->ResourceManager.Texture2DFailBack;
}

PXActionResult PXAPI PXEngineResourceRenderDefault(PXEngine* const pxEngine)
{
    {
        PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
        pxEngineResourceRenderInfo.Type = PXResourceTypeSkybox;
        pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;    

        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(&pxEngine->ResourceManager.SkyBoxLookUp, 0, &pxDictionaryEntry);

        pxEngineResourceRenderInfo.ObjectReference = *(void**)pxDictionaryEntry.Value;

        PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
    }

    // Model
    {
        PXDictionary* const modelLookup = &pxEngine->ResourceManager.ModelLookUp;

        for(PXSize i = 0; i < modelLookup->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXModel* pxModel = PXNull;

            PXDictionaryIndex(modelLookup, i, &pxDictionaryEntry);

            pxModel = *(PXModel**)pxDictionaryEntry.Value;

            if(!(pxModel->Info.Flags & PXEngineResourceInfoVisble))
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXResourceTypeModel;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxModel;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }


    // Sprite
    {
        PXDictionary* const spirteList = &pxEngine->ResourceManager.SpritelLookUp;

        for(PXSize i = 0; i < spirteList->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXSprite* pxSprite = PXNull;

            PXDictionaryIndex(spirteList, i, &pxDictionaryEntry);

            pxSprite = *(PXSprite**)pxDictionaryEntry.Value;

            if(!(pxSprite->Info.Flags & PXEngineResourceInfoVisble))
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXResourceTypeSprite;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxSprite;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }

    // Text
    {
        PXDictionary* const textList = &pxEngine->ResourceManager.TextLookUp;

        for(PXSize i = 0; i < textList->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXEngineText* pxEngineText = PXNull;

            PXDictionaryIndex(textList, i, &pxDictionaryEntry);

            pxEngineText = *(PXEngineText**)pxDictionaryEntry.Value;

            if(!(pxEngineText->Info.Flags & PXEngineResourceInfoEnabled))
            {
                continue;
            }

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXResourceTypeText;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxEngineText;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }

    // HitBoxes
    {
        PXDictionary* const hitBoxList = &pxEngine->ResourceManager.HitBoxLookUp;

        for(PXSize i = 0; i < hitBoxList->EntryAmountCurrent; ++i)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXHitBox* pxHitBox = PXNull;

            PXDictionaryIndex(hitBoxList, i, &pxDictionaryEntry);

            pxHitBox = *(PXHitBox**)pxDictionaryEntry.Value;

            PXEngineResourceRenderInfo pxEngineResourceRenderInfo;
            pxEngineResourceRenderInfo.Type = PXResourceTypeHitBox;
            pxEngineResourceRenderInfo.CameraReference = pxEngine->CameraCurrent;
            pxEngineResourceRenderInfo.ObjectReference = pxHitBox;

            PXEngineResourceRender(pxEngine, &pxEngineResourceRenderInfo);
        }
    }


    return PXActionSuccessful;
}

void PXAPI PXEngineCollsisionSolve(PXEngine* const pxEngine)
{
    PXDictionary* const hitBoxLookUp = &pxEngine->ResourceManager.HitBoxLookUp;

    for (PXSize indexA = 0; indexA < hitBoxLookUp->EntryAmountCurrent; ++indexA)
    {
        PXDictionaryEntry pxDictionaryEntryA;
        PXHitBox* pxHitBoxA = PXNull;

        PXDictionaryIndex(hitBoxLookUp, indexA, &pxDictionaryEntryA);

        pxHitBoxA = *(PXHitBox**)pxDictionaryEntryA.Value;

        if (!(pxHitBoxA->Info.Flags & PXEngineResourceInfoEnabled))
        {
            continue;
        }

        for (PXSize indexB = 0; indexB < hitBoxLookUp->EntryAmountCurrent; ++indexB)
        {
            PXDictionaryEntry pxDictionaryEntryB;
            PXHitBox* hitBoxB = PXNull;

            PXDictionaryIndex(hitBoxLookUp, indexB, &pxDictionaryEntryB);

            hitBoxB = *(PXHitBox**)pxDictionaryEntryB.Value;

            if (!(pxHitBoxA->Info.Flags & PXEngineResourceInfoEnabled))
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
#if 0
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
#endif

    return PXActionSuccessful;
}