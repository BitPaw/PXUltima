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

	PXWindowEventTypeElementMove,
	PXWindowEventTypeElementResize,
	PXWindowEventTypeElementDestroy,
	PXWindowEventTypeElementClose,
	PXWindowEventTypeElementCreate,
	PXWindowEventTypeElementSelect,
	PXWindowEventTypeElementClick,
	PXWindowEventTypeElementFocusEnter,
	PXWindowEventTypeElementFocusLeave,

	PXWindowEventTypeInputMouseButton,
	PXWindowEventTypeInputMouseMove,
	PXWindowEventTypeInputKeyboard,


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

	struct PXGUIElement_* UIElementReference;
	struct PXGUIElement_* UIElementSender;
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


#define PXDisplayScreenMonitorLength 32
#define PXDisplayScreenNameLength 32
#define PXDisplayScreenDeviceLength 128

// Container where windows can be created in
typedef struct PXDisplayScreen_
{
	char GraphicDeviceName[PXDisplayScreenDeviceLength];
	char NameMonitor[PXDisplayScreenMonitorLength];
	char NameID[PXDisplayScreenNameLength];

	int Width;
	int Height;
	int Cells;
	int Planes;
	int WidthMM;
	int HeightMM;

	PXBool IsConnected;
	PXBool IsPrimary;
}
PXDisplayScreen;


typedef struct PXDisplay_
{
#if OSUnix
	Display* DisplayHandle;
	Window WindowRootHandle;
	GC GraphicContent;
#elif OSWindows
	int WindowRootHandle;
	void* DisplayHandle;
	int GraphicContent;
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

	// private
	PXGUIElementBrush* BrushBackgroundDark;
	PXGUIElementBrush* BrushTextWhite;
}
PXGUISystem;






// TODO: Global Variable, bad but needed for SYNC with OS. Stupid design
PXPrivate PXGUISystem* PXGUISystemGlobalReference = PXNull;


//---------------------------------------------------------
// GUI system functions
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem);
PXPublic PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem);

#if OSUnix
PXPublic PXActionResult PXAPI PXGUIElementErrorFromXSystem(const int xSysstemErrorID);
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------
PXPublic void PXAPI PXWindowUpdate(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);

PXPublic void PXAPI PXWindowEventConsumer(PXGUISystem* const pxGUISystem, PXWindowEvent* const pxWindowEvent);

#if OSUnix
PXPrivate void PXWindowEventHandler(PXGUIElement* const pxWindow, const XEvent* const xEventData);
#elif PXOSWindowsDestop
PXPublic LRESULT CALLBACK PXWindowEventHandler(const HWND PXWindowsID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);

PXPrivate void PXAPI PXGUIElementChildListEnumerate(PXGUISystem* const pxGUISystem, PXGUIElement* const parent, PXBool visible);
PXPrivate BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam);
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Window utility functions
//---------------------------------------------------------
PXPublic PXBool PXAPI PXGUIElementFind(const PXWindowID pxUIElementID, PXGUIElement* const pxGUIElement);
PXPublic PXThreadResult PXOSAPI PXWindowMessageLoop(PXGUIElement* const pxGUIElement);

PXPublic PXActionResult PXAPI PXGUIFontLoad(PXGUISystem* const pxGUISystem, PXFont* const pxFont, const char* const name);
PXPublic PXActionResult PXAPI PXGUIFontRelease(PXGUISystem* const pxGUISystem, PXFont* const pxFont);
PXPublic PXActionResult PXAPI PXGUIFontSet(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXFont* const pxFont);

PXPublic PXActionResult PXAPI PXGUIElementCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount);
PXPublic PXActionResult PXAPI PXGUIElementUpdate(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount);
PXPublic PXActionResult PXAPI PXGUIElementFetch(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount);
PXPublic PXActionResult PXAPI PXGUIElementRelease(PXGUIElement* const pxGUIElement);
PXPublic PXActionResult PXAPI PXGUIElementDelete(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);

// Use for a seperate window that needs to be merged into a main one.
// Given a spesific window we can try to absorb the contens and underlieing elemetns and move them into your own space.
// Objects shall not be created or destroyed, simply the ownership of those objects should be transphered. (can we do that?)
PXActionResult PXAPI PXGUIElementAbsorb(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);

// Use for draging a window outside it own borders to spawn a new one.
PXActionResult PXAPI PXGUIElementEmit(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);
//---------------------------------------------------------



//---------------------------------------------------------
// Window propertys - Getter / Setter
//---------------------------------------------------------
PXPublic PXBool PXAPI PXGUIElementIsEnabled(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);
PXPublic PXActionResult PXAPI PXGUIElementMove(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXGUIElementResize(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementMoveAndResize(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);

PXPublic PXActionResult PXAPI PXGUIElementTextSet(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, char* text);
PXPublic PXActionResult PXAPI PXGUIElementTextGet(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, char* text);
PXPublic PXActionResult PXAPI PXGUIElementStyleUpdate(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);
PXPublic PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindowID pxWindowID);


PXPublic PXBool PXAPI PXWindowInteractable(const PXWindowID pxWindowID);

PXPublic PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y);
PXPublic PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y);

PXPublic PXBool PXAPI PXGUIElementValueFetch
(
	PXGUIElement* const pxUIElementList, //
	const PXSize dataListAmount,
	const PXUIElementProperty pxUIElementProperty,
	void* const dataList // The given data
);



PXPublic PXActionResult PXAPI PXGUIScreenDeviceAmount(PXSize* const amount);

PXPublic void PXAPI PXGUIScreenFetchAll(PXMonitor* const monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize);

PXPublic void PXAPI PXGUIScreenGetSize(PXInt32S* const width, PXInt32S* const height);

// QueryDisplayConfig 

PXPublic PXActionResult PXAPI PXGUIDisplayScreenListRefresh(PXGUISystem* const pxGUISystem);







//---------------------------------------------------------
// Default rendering functions
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXGUIElementDrawCustomText(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXGUIElementDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXGUIElementDrawCustomButton(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXGUIElementDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXGUIElementDrawCustomComboBox(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXGUIElementDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXGUIElementDrawCustomHexView(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXGUIElementDrawInfo* const pxGUIElementDrawInfo);
//---------------------------------------------------------




//---------------------------------------------------------
// Window Drag & Drop
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXGUIElementDragStart(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);
//---------------------------------------------------------



//---------------------------------------------------------
// Window draw functions
//---------------------------------------------------------

// Swaps the color buffer. Can prevent flickering.
PXPublic PXActionResult PXAPI PXGUIElementBufferSwap(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);

PXActionResult PXAPI PXGUIDrawClear(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);
PXActionResult PXAPI PXGUIDrawForegroundColorSetRGB(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, char red, char green, char blue);
PXActionResult PXAPI PXGUIDrawBackgroundColorSetRGB(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, char red, char green, char blue);
PXActionResult PXAPI PXGUIElementDrawBegin(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);
PXActionResult PXAPI PXGUIElementDrawEnd(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement);

// Draw text into a given window
// Example: Text for a button
//PXPublic PXActionResult PXAPI PXGUIElementDrawText(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXText* const pxText);
PXPublic PXActionResult PXAPI PXGUIElementDrawTextA(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const char* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXGUIElementDrawTextW(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const wchar_t* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXGUIElementDrawPoint(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXGUIElementDrawPoints(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementDrawLine(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x1, const int y1, const int x2, const int y2);
PXPublic PXActionResult PXAPI PXGUIElementDrawLines(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementDrawRectangle(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXGUIElementDrawRectangleRounded(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height);

//---------------------------------------------------------





















PXPublic void PXAPI PXGUIElementhSizeRefresAll(PXGUISystem* const pxGUISystem);




PXPublic PXActionResult PXAPI PXWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo);




PXPublic PXProcessThreadID PXAPI PXWindowThreadProcessID(const PXWindowID windowID);

PXPublic PXWindowID PXAPI PXWindowFindViaTitle(const PXText* const windowTitle);

PXPublic void PXAPI PXWindowIconCorner();
PXPublic void PXAPI PXWindowIconTaskBar();






PXPublic PXActionResult PXAPI PXWindowMouseMovementEnable(const PXWindowID pxWindow);

PXPublic PXActionResult PXAPI PXWindowPosition(const PXWindowID pxWindowID, PXInt32S* x, PXInt32S* y);

//voidPXWindowCursor(const CursorIcon cursorIcon);
PXPublic void PXAPI PXWindowCursorTexture();
PXPublic void PXAPI PXWindowCursorCaptureMode(const PXWindowID pxWindowID, const PXWindowCursorMode cursorMode);
//voidPXWindowScreenShotTake(Image image);






// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindowID pxWindowID);

PXPrivate PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);

#endif
