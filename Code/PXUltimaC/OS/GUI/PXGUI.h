#ifndef PXGUIINCLUDE
#define PXGUIINCLUDE

#include <Media/PXResource.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Hardware/PXMouse.h>
#include <OS/Hardware/PXKeyBoard.h>
#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>
#include <OS/File/PXFile.h>

#define UseRawMouseData 1
#define PXWindowTitleSizeMax 256
#define PXWindowSizeDefault -1

// The mode in which the mouse pointer will be.
typedef enum PXWindowCursorMode_
{
	PXWindowCursorIgnore, 	// Show the cursor but dont register any Input.
	PXWindowCursorShow, 	// Show the cursor and register Input as normal.
	PXWindowCursorInvisible, 	// Hide Cursor. You can still use it as normal. Not locked.
	PXWindowCursorLock,
	PXWindowCursorLockAndHide
}
PXWindowCursorMode;

typedef enum PXMouseButton_
{
	PXMouseButtonInvalid,
	PXMouseButtonLeft,
	PXMouseButtonMiddle,
	PXMouseButtonRight,
	PXMouseButtonScrollUp,
	PXMouseButtonRightDown,
	PXMouseButtonSpecialA,
	PXMouseButtonSpecialB,
	PXMouseButtonSpecialC,
	PXMouseButtonSpecialD,
	PXMouseButtonSpecialE,
}
PXMouseButton;

typedef enum PXMouseScrollDirection_
{
	PXMouseScrollDirectionInvalid,
	PXMouseScrollDirectionUp,
	PXMouseScrollDirectionDown,
	PXMouseScrollDirectionLeft,
	PXMouseScrollDirectionRight
}
PXMouseScrollDirection;

typedef enum PXCursorIcon_
{
	PXCursorIconInvalid,
	PXCursorIconNormal,
	PXCursorIconIBeam,
	PXCursorIconWait,
	PXCursorIconCross,
	PXCursorIconUp,
	PXCursorIconHand,
	PXCursorIconNotAllowed,
	PXCursorIconAppStarting,
	PXCursorIconResizeHorizontal,
	PXCursorIconResizeVertical,
	PXCursorIconResizeClockwise,
	PXCursorIconResizeClockwiseCounter,
	PXCursorIconResizeAll,
}
PXCursorIcon;

typedef enum PXWindowEventType_
{
	PXWindowEventTypeInvalid,

	PXWindowEventTypeMove,
	PXWindowEventTypeResize,
	PXWindowEventTypeDestroy,
	PXWindowEventTypeClose,
	PXWindowEventTypeCreate,
	PXWindowEventTypeSelect,

	PXWindowEventTypeClick,

	PXWindowEventTypeInputMouseButton,
	PXWindowEventTypeInputMouseMove,
	PXWindowEventTypeInputKeyboard,

	PXWindowEventTypeFocusEnter,
	PXWindowEventTypeFocusLeave,


	WindowEventActivate,
	WindowEventRedrawSet,
	WindowEventFocusSet,
	WindowEventFocusKill,
	WindowEventEnable,
	WindowEventTextSet,
	WindowEventTextGet,
	WindowEventTextGetLength,
	WindowEventPaint,
	WindowEventClose,
	WindowEventSessionQuerryEnd,
	WindowEventSessionEnd,
	WindowEventQuerryOpen,
	WindowEventQuit,
	WindowEventBackgroundErase,
	WindowEventSystemColorChange,
	WindowEventShowWindow,
	WindowEventIconChange,
	WindowEventSettingChange,
	WindowEventDeviceModeChange,
	WindowEventActivateApp,
	WindowEventFontChange,
	WindowEventTimeChange,
	WindowEventCancelMode,
	WindowEventCursorSet,
	WindowEventMouseActivate,
	WindowEventChildActivate,
	WindowEventQueueSync,
	WindowEventSizeChange,
	WindowEventIconPaint,
	WindowEventIconBackgroundErase,
	WindowEventDialogControlNext,
	WindowEventSPOOLERSTATUS,
	WindowEventItemDraw,
	WindowEventItemMeasure,
	WindowEventItemDelete,
	WindowEventVKEYTOITEM,
	WindowEventCHARTOITEM,
	WindowEventFontSet,
	WindowEventFontGet,
	WindowEventSETHOTKEY,
	WindowEventGETHOTKEY,
	WindowEventQUERYDRAGICON,
	WindowEventCOMPAREITEM,
	WindowEventGETOBJECT,
	WindowEventCOMPACTING,
	WindowEventCOMMNOTIFY,
	WindowEventWINDOWPOSCHANGING,
	WindowEventWINDOWPOSCHANGED,
	WindowEventPOWER,
	WindowEventCOPYDATA,
	WindowEventCANCELJOURNAL,
	WindowEventNOTIFY,
	WindowEventINPUTLANGCHANGEREQUEST,
	WindowEventINPUTLANGCHANGE,
	WindowEventTCARD,
	WindowEventHELP,
	WindowEventUSERCHANGED,
	WindowEventNOTIFYFORMAT,
	WindowEventCONTEXTMENU,
	WindowEventSTYLECHANGING,
	WindowEventSTYLECHANGED,
	WindowEventDISPLAYCHANGE,
	WindowEventGETICON,
	WindowEventSETICON,
	WindowEventNCCREATE,
	WindowEventNCDESTROY,
	WindowEventNCCALCSIZE,
	WindowEventNCHITTEST,
	WindowEventNCPAINT,
	WindowEventNCACTIVATE,
	WindowEventGETDLGCODE,
	WindowEventSYNCPAINT,
	WindowEventNCMOUSEMOVE,
	WindowEventNCLBUTTONDOWN,
	WindowEventNCLBUTTONUP,
	WindowEventNCLBUTTONDBLCLK,
	WindowEventNCRBUTTONDOWN,
	WindowEventNCRBUTTONUP,
	WindowEventNCRBUTTONDBLCLK,
	WindowEventNCMBUTTONDOWN,
	WindowEventNCMBUTTONUP,
	WindowEventNCMBUTTONDBLCLK,
	WindowEventNCXBUTTONDOWN,
	WindowEventNCXBUTTONUP,
	WindowEventNCXBUTTONDBLCLK,
	WindowEventINPUT_DEVICE_CHANGE,
	WindowEventINPUT,
	WindowEventKEYFIRST,
	WindowEventKEYDOWN,
	WindowEventKEYUP,
	WindowEventCHAR,
	WindowEventDEADCHAR,
	WindowEventSYSKEYDOWN,
	WindowEventSYSKEYUP,
	WindowEventSYSCHAR,
	WindowEventSYSDEADCHAR,
	WindowEventUNICHAR,
	WindowEventKEYLAST,
	//UNICODE_NOCHAR,
	WindowEventIME_STARTCOMPOSITION,
	WindowEventIME_ENDCOMPOSITION,
	WindowEventIME_COMPOSITION,
	WindowEventIME_KEYLAST,
	WindowEventINITDIALOG,
	WindowEventCOMMAND,
	WindowEventSYSCOMMAND,
	WindowEventTIMER,
	WindowEventHSCROLL,
	WindowEventVSCROLL,
	WindowEventINITMENU,
	WindowEventINITMENUPOPUP,
	WindowEventGESTURE,
	WindowEventGESTURENOTIFY,
	WindowEventMENUSELECT,
	WindowEventMENUCHAR,
	WindowEventENTERIDLE,
	WindowEventMENURBUTTONUP,
	WindowEventMENUDRAG,
	WindowEventMENUGETOBJECT,
	WindowEventUNINITMENUPOPUP,
	WindowEventMENUCOMMAND,
	WindowEventCHANGEUISTATE,
	WindowEventUPDATEUISTATE,
	WindowEventQUERYUISTATE,
	WindowEventCTLCOLORMSGBOX,
	WindowEventCTLCOLOREDIT,
	WindowEventCTLCOLORLISTBOX,
	WindowEventCTLCOLORBTN,
	WindowEventCTLCOLORDLG,
	WindowEventCTLCOLORSCROLLBAR,
	WindowEventCTLCOLORSTATIC,
	WindowEventGETHMENU,
	WindowEventMOUSEFIRST,
	WindowEventMOUSEMOVE,
	WindowEventLBUTTONDOWN,
	WindowEventLBUTTONUP,
	WindowEventLBUTTONDBLCLK,
	WindowEventRBUTTONDOWN,
	WindowEventRBUTTONUP,
	WindowEventRBUTTONDBLCLK,
	WindowEventMBUTTONDOWN,
	WindowEventMBUTTONUP,
	WindowEventMBUTTONDBLCLK,
	WindowEventMOUSEWHEEL,
	WindowEventXBUTTONDOWN,
	WindowEventXBUTTONUP,
	WindowEventXBUTTONDBLCLK,
	WindowEventMOUSEHWHEEL,
	WindowEventMOUSELAST,
	WindowEventPARENTNOTIFY,
	WindowEventENTERMENULOOP,
	WindowEventEXITMENULOOP,
	WindowEventNEXTMENU,
	WindowEventSIZING,
	WindowEventCAPTURECHANGED,
	WindowEventMOVING,
	WindowEventPOWERBROADCAST,
	WindowEventDEVICECHANGE,
	WindowEventMDICREATE,
	WindowEventMDIDESTROY,
	WindowEventMDIACTIVATE,
	WindowEventMDIRESTORE,
	WindowEventMDINEXT,
	WindowEventMDIMAXIMIZE,
	WindowEventMDITILE,
	WindowEventMDICASCADE,
	WindowEventMDIICONARRANGE,
	WindowEventMDIGETACTIVE,
	WindowEventMDISETMENU,
	WindowEventENTERSIZEMOVE,
	WindowEventEXITSIZEMOVE,
	WindowEventDROPFILES,
	WindowEventMDIREFRESHMENU,
	WindowEventPOINTERDEVICECHANGE,
	WindowEventPOINTERDEVICEINRANGE,
	WindowEventPOINTERDEVICEOUTOFRANGE,
	WindowEventTOUCH,
	WindowEventNCPOINTERUPDATE,
	WindowEventNCPOINTERDOWN,
	WindowEventNCPOINTERUP,
	WindowEventPOINTERUPDATE,
	WindowEventPOINTERDOWN,
	WindowEventPOINTERUP,
	WindowEventPOINTERENTER,
	WindowEventPOINTERLEAVE,
	WindowEventPOINTERACTIVATE,
	WindowEventPOINTERCAPTURECHANGED,
	WindowEventTOUCHHITTESTING,
	WindowEventPOINTERWHEEL,
	WindowEventPOINTERHWHEEL,
	WindowEventPOINTERROUTEDTO,
	WindowEventPOINTERROUTEDAWAY,
	WindowEventPOINTERROUTEDRELEASED,
	WindowEventIME_SETCONTEXT,
	WindowEventIME_NOTIFY,
	WindowEventIME_CONTROL,
	WindowEventIME_COMPOSITIONFULL,
	WindowEventIME_SELECT,
	WindowEventIME_CHAR,
	WindowEventIME_REQUEST,
	WindowEventIME_KEYDOWN,
	WindowEventIME_KEYUP,
	WindowEventMOUSEHOVER,
	WindowEventMOUSELEAVE,
	WindowEventNCMOUSEHOVER,
	WindowEventNCMOUSELEAVE,
	WindowEventWTSSESSION_CHANGE,
	WindowEventTABLET_FIRST,
	WindowEventTABLET_LAST,
	WindowEventDPICHANGED,
	WindowEventDPICHANGED_BEFOREPARENT,
	WindowEventDPICHANGED_AFTERPARENT,
	WindowEventGETDPISCALEDSIZE,
	WindowEventCUT,
	WindowEventCOPY,
	WindowEventPASTE,
	WindowEventCLEAR,
	WindowEventUNDO,
	WindowEventRENDERFORMAT,
	WindowEventRENDERALLFORMATS,
	WindowEventDESTROYCLIPBOARD,
	WindowEventDRAWCLIPBOARD,
	WindowEventPAINTCLIPBOARD,
	WindowEventVSCROLLCLIPBOARD,
	WindowEventSIZECLIPBOARD,
	WindowEventASKCBFORMATNAME,
	WindowEventCHANGECBCHAIN,
	WindowEventHSCROLLCLIPBOARD,
	WindowEventQUERYNEWPALETTE,
	WindowEventPALETTEISCHANGING,
	WindowEventPALETTECHANGED,
	WindowEventHOTKEY,
	WindowEventPRINT,
	WindowEventPRINTCLIENT,
	WindowEventAPPCOMMAND,
	WindowEventTHEMECHANGED,
	WindowEventCLIPBOARDUPDATE,
	WindowEventDWMCOMPOSITIONCHANGED,
	WindowEventDWMNCRENDERINGCHANGED,
	WindowEventDWMCOLORIZATIONCOLORCHANGED,
	WindowEventDWMWINDOWMAXIMIZEDCHANGE,
	WindowEventDWMSENDICONICTHUMBNAIL,
	WindowEventDWMSENDICONICLIVEPREVIEWBITMAP,
	WindowEventGETTITLEBARINFOEX,
	WindowEventHANDHELDFIRST,
	WindowEventHANDHELDLAST,
	WindowEventAFXFIRST,
	WindowEventAFXLAST,
	WindowEventPENWINFIRST,
	WindowEventPENWINLAST,
	WindowEventAPP
}
PXWindowEventType;



typedef struct PXKeyBoardKeyInfo_
{
	PXVirtualKey Key;
	PXKeyPressState Mode;

	unsigned short Repeat; // Die Wiederholungsanzahl für die aktuelle Meldung.Der Wert gibt an, wie oft die Tastatureingabe automatisch angezeigt wird, wenn der Benutzer den Schlüssel hält.Die Wiederholungsanzahl ist immer 1 für eine WM _ KEYUP - Nachricht.
	unsigned short ScanCode;// Der Scancode.Der Wert hängt vom OEM ab.
	unsigned short SpecialKey;// Gibt an, ob es sich bei der Taste um eine erweiterte Taste handelt, z.B.die rechte ALT - und STRG - Taste, die auf einer erweiterten Tastatur mit 101 oder 102 Tasten angezeigt werden.Der Wert ist 1, wenn es sich um einen erweiterten Schlüssel handelt.andernfalls ist es 0.
	unsigned short KontextCode; // Der Kontextcode.Der Wert ist für eine WM _ KEYUP - Nachricht immer 0.
	unsigned short PreState; // Der vorherige Schlüsselzustand.Der Wert ist immer 1 für eine WM _ KEYUP - Nachricht.
	unsigned short GapState;

	unsigned char KeyID;
}
PXKeyBoardKeyInfo;


typedef struct PXWindowEventClose_
{
	PXBool CommitToClose;
}
PXWindowEventClose;

typedef struct PXWindowEventResize_
{
	PXInt16S Width;
	PXInt16S Height;
}
PXWindowEventResize;

typedef struct PXWindowEventInputMouseButton_
{
	PXKeyPressState PressState;
	PXMouseButton Button;
}
PXWindowEventInputMouseButton;

typedef struct PXWindowEventSelect_
{
	PXGUIElement* UIElementSelected;
}
PXWindowEventSelect;

typedef struct PXWindowEventInputMouseMove_
{
	PXInt32S AxisX;
	PXInt32S AxisY;
	PXInt32S DeltaX;
	PXInt32S DeltaY;
}
PXWindowEventInputMouseMove;

typedef struct PXWindowEventInputKeyboard_
{
	PXKeyPressState PressState;
	PXVirtualKey VirtualKey;

	PXInt16U CharacterID;
}
PXWindowEventInputKeyboard;


typedef struct PXWindowEvent_
{
	union
	{
		PXWindowEventClose Close;
		PXWindowEventResize Resize;
		PXWindowEventInputMouseButton InputMouseButton;
		PXWindowEventInputMouseMove InputMouseMove;
		PXWindowEventInputKeyboard InputKeyboard;
		PXWindowEventSelect Select;
	};

	//-----------------------------
	// Original data
	//-----------------------------
#if OSUnix
	XEvent* EventData;
#elif OSWindows
	HWND WindowHandle;
	UINT EventID;
	WPARAM ParamW;
	LPARAM ParamL;
#endif

	//-----------------------------
	// Translated Data
	//-----------------------------
	PXWindowEventType Type;

	struct PXUIElement_* UIElementReference;
	struct PXUIElement_* UIElementSender;
}
PXWindowEvent;

typedef struct PXWindowPixelSystemInfo_
{
#if OSUnix

#elif OSWindows
	HDC HandleDeviceContext;
	HWND HandleWindow;
#endif


	PXInt8U BitPerPixel; // 32=8Bit Default

	PXBool OpenGL;
	PXBool DirectX;
	PXBool GDI;
}
PXWindowPixelSystemInfo;


// Container where windows can be created in
typedef struct PXDisplayScreen_
{
	char* Name;

	int Width;
	int Height;
	int Cells;
	int Planes;
	int WidthMM;
	int HeightMM;

}
PXDisplayScreen;


typedef struct PXDisplay_
{
#if OSUnix
	Display* DisplayHandle;
#elif OSWindows
	void* DisplayHandle;
#endif

#if OSUnix
	Window WindowRootHandle;
#elif OSWindows
	int WindowRootHandle;
#endif

	char* Data;
	char* Name;

	int ProtocolVersion;
	int ProtocolRevision;

	char* ServerVendor;
	int VendorRelease;

	int ScreenDefaultID;
	int ScreenListAmount;

	PXDisplayScreen DisplayScreenList[8];
}
PXDisplay;


// Manages library calls to the operating system window manager
typedef struct PXGUISystem_
{
	PXLibrary LibraryWindowsUser32DLL;

	PXBool AreOSUIElementsDefined;

	PXResourceManager* ResourceManager;

	PXDisplay DisplayCurrent;
}
PXGUISystem;






// Global Variable, bad but needed for SYNC with OS. Stupid design

PXPrivate PXGUISystem* PXGUISystemGlobalReference = PXNull;


PXPublic PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem);
PXPublic PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem);


PXPublic void PXAPI PXWindowEventConsumer(PXGUISystem* const pxGUISystem, PXWindowEvent* const pxWindowEvent);

#if OSUnix
PXPrivate void PXWindowEventHandler(PXGUIElement* const pxWindow, const XEvent* const xEventData);
#elif PXOSWindowsDestop
PXPublic LRESULT CALLBACK PXWindowEventHandler(const HWND PXWindowsID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);

PXPrivate void PXAPI PXGUIElementChildListEnumerate(PXGUISystem* const pxGUISystem, PXGUIElement* const parent, PXBool visible);
PXPrivate BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam);
#endif


PXPublic PXThreadResult PXOSAPI PXWindowMessageLoop(PXGUIElement* const pxGUIElement);

PXPublic PXBool PXAPI PXGUIElementIsEnabled(const PXWindowID pxUIElementID);
PXPublic PXBool PXAPI PXGUIElementFind(const PXWindowID pxUIElementID, PXGUIElement* const pxGUIElement);
PXPublic PXActionResult PXAPI PXGUIElementDelete(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);


PXPublic PXBool PXAPI PXGUIElementValueFetch
(
	PXGUIElement* const pxUIElementList, //
	const PXSize dataListAmount,
	const PXUIElementProperty pxUIElementProperty,
	void* const dataList // The given data
);






PXPublic PXActionResult PXAPI PXGUIElementTextSet(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, char* text);



// Draw text into a given window
// Example: Text for a button
PXPublic PXActionResult PXAPI PXGUIElementDrawText(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXText* const pxText);
PXPublic PXActionResult PXAPI PXGUIElementDrawTextA(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const char* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXGUIElementDrawTextW(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const wchar_t* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXGUIElementDrawPoint(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXGUIElementDrawPoints(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementDrawLine(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x1, const int y1, const int x2, const int y2);
PXPublic PXActionResult PXAPI PXGUIElementDrawLines(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementDrawRectangle(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementDrawRectangleRounded(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementMove(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXGUIElementResize(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementMoveAndResize(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);






#if OSUnix
PXPublic PXActionResult PXAPI PXGUIElementErrorFromXSystem(const int xSysstemErrorID);
#endif






PXPublic PXActionResult PXAPI PXGUIElementCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount);
PXPublic PXActionResult PXAPI PXGUIElementUpdate(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount);
PXPublic PXActionResult PXAPI PXGUIElementFetch(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount);

PXPublic PXActionResult PXAPI PXGUIElementRelease(PXGUIElement* const pxGUIElement);


PXPublic void PXAPI PXGUIElementhSizeRefresAll(PXGUISystem* const pxGUISystem);




PXPublic PXActionResult PXAPI PXWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo);

PXPublic void PXAPI PXWindowUpdate(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);


PXPublic PXProcessThreadID PXAPI PXWindowThreadProcessID(const PXWindowID windowID);

PXPublic PXWindowID PXAPI PXWindowFindViaTitle(const PXText* const windowTitle);

PXPublic void PXAPI PXWindowIconCorner();
PXPublic void PXAPI PXWindowIconTaskBar();



PXPublic PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindowID pxWindowID);


PXPublic PXActionResult PXAPI PXWindowMouseMovementEnable(const PXWindowID pxWindow);

PXPublic PXActionResult PXAPI PXWindowPosition(const PXWindowID pxWindowID, PXInt32S* x, PXInt32S* y);
PXPublic PXActionResult PXAPI PXWindowMove(const PXWindowID pxWindow, const PXInt32S x, const PXInt32S y);
PXPublic void PXAPI PXWindowPositonCenterScreen(const PXWindowID pxWindow);
PXPublic void PXAPI PXWindowCursor(const PXWindowID pxWindow);
//voidPXWindowCursor(const CursorIcon cursorIcon);
PXPublic void PXAPI PXWindowCursorTexture();
PXPublic void PXAPI PXWindowCursorCaptureMode(const PXWindowID pxWindowID, const PXWindowCursorMode cursorMode);
//voidPXWindowScreenShotTake(Image image);

PXPublic PXBool PXAPI PXWindowFrameBufferSwap(const PXWindowID pxWindowID);

PXPublic PXBool PXAPI PXWindowInteractable(const PXWindowID pxWindowID);

PXPublic PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y);
PXPublic PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y);


// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindowID pxWindowID);

PXPrivate PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);

#endif
