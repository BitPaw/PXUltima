#ifndef KeyBoardInclude
#define KeyBoardInclude

#include <Media/PXType.h>

#define KeyBoardIDCommandEscape (1 << 0)
#define KeyBoardIDCommandEnter (1 << 1)
#define KeyBoardIDCommandTab (1 << 2)
#define KeyBoardIDCommandShift (1 << 3)
#define KeyBoardIDBACKSPACE (1 << 4)
#define KeyBoardIDINSERT (1 << 5)
#define KeyBoardIDDELETE (1 << 6)
#define KeyBoardIDRIGHT (1 << 7)
#define KeyBoardIDLEFT (1 << 8)
#define KeyBoardIDDOWN (1 << 9)
#define KeyBoardIDUP (1 << 10)
#define KeyBoardIDPAGE_UP (1 << 11)
#define KeyBoardIDPAGE_DOWN (1 << 12)
#define KeyBoardIDHOME (1 << 13)
#define KeyBoardIDEND (1 << 14)
#define KeyBoardIDCAPS_LOCK (1 << 15)
#define KeyBoardIDSCROLL_LOCK (1 << 16)
#define KeyBoardIDNUM_LOCK (1 << 17)
#define KeyBoardIDPRINT_SCREEN (1 << 18)
#define KeyBoardIDPAUSE (1 << 19)
#define KeyBoardIDPadENTER (1 << 20)
#define KeyBoardIDShiftLeft (1 << 21)
#define KeyBoardIDShiftRight (1 << 22)
#define KeyBoardIDCONTROLLEFT (1 << 23)
#define KeyBoardIDALTLEFT (1 << 24)
#define KeyBoardIDSUPERLEFT (1 << 25)
#define KeyBoardIDCONTROLRIGHT (1 << 26)
#define KeyBoardIDALTRIGHT (1 << 27)
#define KeyBoardIDSUPERRIGHT (1 << 28)
#define KeyBoardIDMENU (1 << 29)
#define KeyBoardIDWORLD_1  (1 << 30)
#define KeyBoardIDWORLD_2  (1 << 31)


#define KeyBoardIDF01 (1u << 0u)
#define KeyBoardIDF02 (1u << 1u)
#define KeyBoardIDF03 (1u << 2u)
#define KeyBoardIDF04 (1u << 3u)
#define KeyBoardIDF05 (1u << 4u)
#define KeyBoardIDF06 (1u << 5u)
#define KeyBoardIDF07 (1u << 6u)
#define KeyBoardIDF08 (1u << 7u)
#define KeyBoardIDF09 (1u << 8u)
#define KeyBoardIDF10 (1u << 9u)
#define KeyBoardIDF11 (1u << 10u)
#define KeyBoardIDF12 (1u << 11u)
#define KeyBoardIDF13 (1u << 12u)
#define KeyBoardIDF14 (1u << 13u)
#define KeyBoardIDF15 (1u << 14u)
#define KeyBoardIDF16 (1u << 15u)
#define KeyBoardIDF17 (1u << 16u)
#define KeyBoardIDF18 (1u << 17u)
#define KeyBoardIDF19 (1u << 18u)
#define KeyBoardIDF20 (1u << 19u)
#define KeyBoardIDF21 (1u << 20u)
#define KeyBoardIDF22 (1u << 21u)
#define KeyBoardIDF23 (1u << 22u)
#define KeyBoardIDF24 (1u << 23u)
#define KeyBoardIDF25 (1u << 24u)


#define KeyBoardIDLetterA (1u << 0)
#define KeyBoardIDLetterB (1u << 1)
#define KeyBoardIDLetterC (1u << 2)
#define KeyBoardIDLetterD (1u << 3)
#define KeyBoardIDLetterE (1u << 4)
#define KeyBoardIDLetterF (1u << 5)
#define KeyBoardIDLetterG (1u << 6)
#define KeyBoardIDLetterH (1u << 7)
#define KeyBoardIDLetterI (1u << 8)
#define KeyBoardIDLetterJ (1u << 9)
#define KeyBoardIDLetterK (1u << 10)
#define KeyBoardIDLetterL (1u << 11)
#define KeyBoardIDLetterM (1u << 12)
#define KeyBoardIDLetterN (1u << 13)
#define KeyBoardIDLetterO (1u << 14)
#define KeyBoardIDLetterP (1u << 15)
#define KeyBoardIDLetterQ (1u << 16)
#define KeyBoardIDLetterR (1u << 17)
#define KeyBoardIDLetterS (1u << 18)
#define KeyBoardIDLetterT (1u << 19)
#define KeyBoardIDLetterU (1u << 20)
#define KeyBoardIDLetterV (1u << 21)
#define KeyBoardIDLetterW (1u << 22)
#define KeyBoardIDLetterX (1u << 23)
#define KeyBoardIDLetterY (1u << 24)
#define KeyBoardIDLetterZ (1u << 25)
#define KeyBoardIDSpace	  (1u << 26)
#define KeyBoardIDAPOSTROPHE  (1u << 27)  /* ' */
#define KeyBoardIDComma       (1u << 28)  /* , */
#define KeyBoardIDGRAVE_ACCENT (1u << 29)  /* ` */
#define KeyBoardIDSemicolon   (1u << 30)  /* ; */
#define KeyBoardIDDECIMAL (1u << 31)


#define KeyBoardIDNumber0 (1 << 0)
#define KeyBoardIDNumber1 (1 << 1)
#define KeyBoardIDNumber2 (1 << 2)
#define KeyBoardIDNumber3 (1 << 3)
#define KeyBoardIDNumber4 (1 << 4)
#define KeyBoardIDNumber5 (1 << 5)
#define KeyBoardIDNumber6 (1 << 6)
#define KeyBoardIDNumber7 (1 << 7)
#define KeyBoardIDNumber8 (1 << 8)
#define KeyBoardIDNumber9 (1 << 9)
#define KeyBoardIDNumberBlock0 (1 << 10)
#define KeyBoardIDNumberBlock1 (1 << 11)
#define KeyBoardIDNumberBlock2 (1 << 12)
#define KeyBoardIDNumberBlock3 (1 << 13)
#define KeyBoardIDNumberBlock4 (1 << 14)
#define KeyBoardIDNumberBlock5 (1 << 15)
#define KeyBoardIDNumberBlock6 (1 << 16)
#define KeyBoardIDNumberBlock7 (1 << 17)
#define KeyBoardIDNumberBlock8 (1 << 18)
#define KeyBoardIDNumberBlock9 (1 << 19)
#define KeyBoardIDNumberKeyMinus (1 << 20) 
#define KeyBoardIDNumberKeyPeriod (1 << 21) 
#define KeyBoardIDNumberKeySlash (1 << 22) 
#define KeyBoardIDNumberKeyEqual (1 << 23) 
#define KeyBoardIDNumberKeyLEFT_BRACKET (1 << 24)  
#define KeyBoardIDNumberKeyBACKSLASH (1 << 25)  
#define KeyBoardIDNumberKeyRIGHT_BRACKET (1 << 26) 
#define KeyBoardIDNumberKeyPadDIVIDE (1 << 27)
#define KeyBoardIDNumberKeyPadMULTIPLY (1 << 28)
#define KeyBoardIDNumberKeyPadSUBTRACT (1 << 29)
#define KeyBoardIDNumberKeyPadADD (1 << 30)		
#define KeyBoardIDNumberKeyPadEQUAL (1 << 31)


typedef enum PXVirtualKey_
{
	KeyInvalid,
	KeyUnknown,

	// Command

	KeySpace,

	KeyMinus,  /* - */
	KeySlash,  /* / */
	KeyEscape,
	KeyEnter,
	KeyTab,
	KeyBackspace,
	KeyInsert,
	KeyDelete,
	KeyRight,
	KeyLeft,
	KeyDown,
	KeyUp,
	KeyPageUp,
	KeyPageDown,
	KeyHome,
	KeyEnd,
	KeyCapsLock,
	KeyScrollLock,
	KeyNumLock,
	KeyPrintScreen,
	KeyPause,


	// Numbers
	Key0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeyPad0,
	KeyPad1,
	KeyPad2,
	KeyPad3,
	KeyPad4,
	KeyPad5,
	KeyPad6,
	KeyPad7,
	KeyPad8,
	KeyPad9,
	KeyPadDecimal,
	KeyPadDivide,
	KeyPadMultiply,
	KeyPadSubtract,
	KeyPadAdd,
	KeyPadEnter,
	KeyPadEqual,



	KeySemicolon,  /* ; */
	KeyEqual,  /* = */

	// Letters

	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,

	KeyASmal,
	KeyBSmal,
	KeyCSmal,
	KeyDSmal,
	KeyESmal,
	KeyFSmal,
	KeyGSmal,
	KeyHSmal,
	KeyISmal,
	KeyJSmal,
	KeyKSmal,
	KeyLSmal,
	KeyMSmal,
	KeyNSmal,
	KeyOSmal,
	KeyPSmal,
	KeyQSmal,
	KeyRSmal,
	KeySSmal,
	KeyTSmal,
	KeyUSmal,
	KeyVSmal,
	KeyWSmal,
	KeyXSmal,
	KeyYSmal,
	KeyZSmal,
	KeyApostrophe,  /* ' */
	KeyComma,  /* , */
	KeyPeriod,  /* . */


	KeyBrackedLeft,  /* [ */
	KeyBackSlash,  /* \ */
	KeyBrackedRight,  /* ] */
	KeyGraveAccent,  /* ` */
	KeyWorld1, /* non-US #1 */
	KeyWorld2, /* non-US #2 */



	// Function
	KeyF1,
	KeyF2,
	KeyF3,
	KeyF4,
	KeyF5,
	KeyF6,
	KeyF7,
	KeyF8,
	KeyF9,
	KeyF10,
	KeyF11,
	KeyF12,
	KeyF13,
	KeyF14,
	KeyF15,
	KeyF16,
	KeyF17,
	KeyF18,
	KeyF19,
	KeyF20,
	KeyF21,
	KeyF22,
	KeyF23,
	KeyF24,
	KeyF25,


	KeyShiftLeft,
	KeyShiftRight,

	KeyControlLeft,
	KeyAltLeft,
	KeySuperLeft,

	KeyControlRight,
	KeyAltRight,
	KeySuperRight,
	KeyMenu
}
PXVirtualKey;

typedef enum PXKeyPressState_
{
	PXKeyPressStateInvalid,
	PXKeyPressStateDown,
	PXKeyPressStateUp,
	PXKeyPressStateHold,
	PXKeyPressStateDoubleClick,
}
PXKeyPressState;

typedef struct PXKeyBoard_
{
	PXInt32U Letters; // A, B, C, D, ...
	PXInt32U Numbers; // 0,1,2... and +, -, /, ...
	PXInt32U Commands; // Enter, Escape, ect..
	PXInt32U Actions; // F-01 to F-xx
}
PXKeyBoard;

typedef struct PXKeyBoardVirtualInput_
{
	PXVirtualKey VirtualKey;
	PXKeyPressState KeyStrokeMode;
}
PXKeyBoardVirtualInput;


PXPublic PXVirtualKey PXAPI PXVirtualKeyFromID(const PXInt8U character);
PXPublic PXInt8U PXAPI PXVirtualKeyToID(const PXVirtualKey character);

PXPublic void PXAPI PXKeyBoardInputReset(PXKeyBoard* const keyBoard);
PXPublic void PXAPI PXKeyBoardInputPrint(const PXKeyBoard* const keyBoard);


// Remove this? Find better solution
PXPublic unsigned char PXAPI PXInputButtonReset(unsigned char* value);
PXPublic unsigned char PXAPI PXInputButtonIncrement(unsigned char* value);
PXPublic unsigned char PXAPI PXInputButtonIncrementIfAlreadyPressed(unsigned char* value);
PXPublic unsigned char PXAPI PXInputButtonIsShortPressed(const unsigned char value);
PXPublic unsigned char PXAPI PXInputButtonIsLongPressed(const unsigned charvalue);
PXPublic unsigned char PXAPI PXInputButtonIsPressed(const unsigned char value);

PXPublic PXKeyPressState PXAPI PXKeyPressStateFromID(const PXInt8U pxKeyPressStateID);
PXPublic PXInt8U PXAPI PXKeyPressStateToID(const PXKeyPressState pxKeyPressState);

#define PXKeyBoardVirtualInputSet(adress, pxVirtualKey, PXKeyPressState) (adress)->VirtualKey = pxVirtualKey; (adress)->KeyStrokeMode = PXKeyPressState; 

PXPublic PXBool PXAPI PXKeyBoardVirtualInsertAction(const PXKeyBoardVirtualInput* const inputList, const PXSize inputListSize);

#endif