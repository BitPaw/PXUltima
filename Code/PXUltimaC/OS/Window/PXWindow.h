#ifndef PXWindowINCLUDE
#define PXWindowINCLUDE

#include <Media/PXResource.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Hardware/PXMouse.h>
#include <OS/Hardware/PXKeyBoard.h>
#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>

#if OSUnix

#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

typedef XID PXWindowID;// XID is PXWindow
typedef GLXContext PXOpenGLConextID;

#elif OSWindows
#include <Windows.h>
typedef HWND PXWindowID;
typedef HGLRC PXOpenGLConextID;
#endif

#define UseRawMouseData 1
#define PXWindowTitleSizeMax 256
#define PXWindowSizeDefault -1

#ifdef __cplusplus
extern "C"
{
#endif

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


	typedef struct PXWindow_
	{
		volatile PXBool IsRunning;
		PXWindowID ID;

		// Live data
		PXBool HasSizeChanged;
		PXWindowCursorMode CursorModeCurrent;
	

#if OSUnix
		Display* DisplayCurrent;
		PXWindowID WindowRoot;
#elif OSWindows
		HCURSOR CursorID;
		HDC HandleDeviceContext;
#endif

		// Interneal
		PXThread MessageThread;
	}
	PXWindow;


	


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
		PXUIElement* UIElementSelected;
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

		union PXWindowEventUnion
		{
			PXWindowEventClose Close;
			PXWindowEventResize Resize;
			PXWindowEventInputMouseButton InputMouseButton;
			PXWindowEventInputMouseMove InputMouseMove;
			PXWindowEventInputKeyboard InputKeyboard;
			PXWindowEventSelect Select;
		};	
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


	// Manages library calls to the operating system window manager
	typedef struct PXGUISystem_
	{
		PXLibrary LibraryWindowsUser32DLL;


		PXBool AreOSUIElementsDefined;


		// Storage of all known handles
		PXDictionary UIElementLookUp;

	}
	PXGUISystem;






	typedef enum PXUIElementProperty_
	{
		PXUIElementPropertyInvalid,
		PXUIElementPropertyTextContent,
		PXUIElementPropertyTextAllign,
		PXUIElementPropertyTextColor,
		PXUIElementPropertySize,
		PXUIElementPropertyProgressbarPercentage,
		PXUIElementPropertyProgressbarBarColor,


		PXUIElementPropertyItemAdd,
		PXUIElementPropertyItemDelete,
		PXUIElementPropertyItemUpdate,

		// Tree view

		PXUIElementPropertyTreeView_CreateDragImage,
		PXUIElementPropertyTreeView_DeleteAllItems,
		PXUIElementPropertyTreeView_DeleteItem,
		PXUIElementPropertyTreeView_EditLabel,
		PXUIElementPropertyTreeView_EndEditLabelNow,
		PXUIElementPropertyTreeView_EnsureVisible,
		PXUIElementPropertyTreeView_Expand,
		PXUIElementPropertyTreeView_GetBkColor,
		PXUIElementPropertyTreeView_GetCheckState,
		PXUIElementPropertyTreeView_GetChild,
		PXUIElementPropertyTreeView_GetCount,
		PXUIElementPropertyTreeView_GetDropHilight,
		PXUIElementPropertyTreeView_GetEditControl,
		PXUIElementPropertyTreeView_GetExtendedStyle,
		PXUIElementPropertyTreeView_GetFirstVisible,
		PXUIElementPropertyTreeView_GetImageList,
		PXUIElementPropertyTreeView_GetIndent,
		PXUIElementPropertyTreeView_GetInsertMarkColor,
		PXUIElementPropertyTreeView_GetISearchString,
		PXUIElementPropertyTreeView_GetItem,
		PXUIElementPropertyTreeView_GetItemHeight,
		PXUIElementPropertyTreeView_GetItemPartRect,
		PXUIElementPropertyTreeView_GetItemRect,
		PXUIElementPropertyTreeView_GetItemState,
		PXUIElementPropertyTreeView_GetLastVisible,
		PXUIElementPropertyTreeView_GetLineColor,
		PXUIElementPropertyTreeView_GetNextItem,
		PXUIElementPropertyTreeView_GetNextSelected,
		PXUIElementPropertyTreeView_GetNextSibling,
		PXUIElementPropertyTreeView_GetNextVisible,
		PXUIElementPropertyTreeView_GetParent,
		PXUIElementPropertyTreeView_GetPrevSibling,
		PXUIElementPropertyTreeView_GetPrevVisible,
		PXUIElementPropertyTreeView_GetRoot,
		PXUIElementPropertyTreeView_GetScrollTime,
		PXUIElementPropertyTreeView_GetSelectedCount,
		PXUIElementPropertyTreeView_GetSelection,
		PXUIElementPropertyTreeView_GetTextColor,
		PXUIElementPropertyTreeView_GetToolTips,
		PXUIElementPropertyTreeView_GetUnicodeFormat,
		PXUIElementPropertyTreeView_GetVisibleCount,
		PXUIElementPropertyTreeView_HitTest,
		PXUIElementPropertyTreeView_InsertItem,
		PXUIElementPropertyTreeView_MapAccIDToHTREEITEM,
		PXUIElementPropertyTreeView_MapHTREEITEMtoAccID,
		PXUIElementPropertyTreeView_Select,
		PXUIElementPropertyTreeView_SelectDropTarget,
		PXUIElementPropertyTreeView_SelectItem,
		PXUIElementPropertyTreeView_SelectSetFirstVisible,
		PXUIElementPropertyTreeView_SetAutoScrollInfo,
		PXUIElementPropertyTreeView_SetBkColor,
		PXUIElementPropertyTreeView_SetBorder,
		PXUIElementPropertyTreeView_SetCheckState,
		PXUIElementPropertyTreeView_SetExtendedStyle,
		PXUIElementPropertyTreeView_SetHot,
		PXUIElementPropertyTreeView_SetImageList,
		PXUIElementPropertyTreeView_SetIndent,
		PXUIElementPropertyTreeView_SetInsertMark,
		PXUIElementPropertyTreeView_SetInsertMarkColor,
		PXUIElementPropertyTreeView_SetItem,
		PXUIElementPropertyTreeView_SetItemHeight,
		PXUIElementPropertyTreeView_SetItemState,
		PXUIElementPropertyTreeView_SetLineColor,
		PXUIElementPropertyTreeView_SetScrollTime,
		PXUIElementPropertyTreeView_SetTextColor,
		PXUIElementPropertyTreeView_SetToolTips,
		PXUIElementPropertyTreeView_SetUnicodeFormat,
		PXUIElementPropertyTreeView_ShowInfoTip,
		PXUIElementPropertyTreeView_SortChildren,
		PXUIElementPropertyTreeView_SortChildrenCB,

	}
	PXUIElementProperty;



	typedef struct PXGUIElementCreateWindowInfo_
	{
		//void* EventFunction;
		void* EventOwner;

#if OSUnix
		XID WindowHandle;
		XID WindowRootHandle;
		Display* WindowDisplay;
#elif OSWindows
		HWND WindowHandle;
#endif

		PXColorRGBAI8 BackGroundColor;
		
		PXInt32S X;
		PXInt32S Y;
		PXInt32S Width;
		PXInt32S Height;
		char* Title;

		PXBool IsVisible;
		PXBool CreateMessageThread; // Run events in another thread
		PXBool MaximizeOnStart;

		PXBool RegisterMouse;
	}
	PXGUIElementCreateWindowInfo;

	typedef enum PXUIElementTreeViewItemInsertMode_
	{
		PXUIElementTreeViewItemInsertModeROOT,
		PXUIElementTreeViewItemInsertModeFIRST,
		PXUIElementTreeViewItemInsertModeLAST,
		PXUIElementTreeViewItemInsertModeSORT
	}
	PXUIElementTreeViewItemInsertMode;

	typedef struct PXUIElementTreeViewItemInfo_
	{
		char* TextDataOverride;
		PXSize TextSizeOverride;

		struct PXUIElement_* ItemParent;
		struct PXUIElement_* TreeView;

		struct PXUIElement_* ElementSource;

		PXUIElementTreeViewItemInsertMode InsertMode;

		// Result
		struct _TREEITEM* ItemHandle;
	}
	PXUIElementTreeViewItemInfo;


// Behaviour
#define PXGUIElementBehaviourDrawOverrride	0b00000001
#define PXGUIElementBehaviourUseOS			0b00000010 
#define PXGUIElementBehaviourCanBeTabbed	0b00000100 

// Design
#define PXGUIElementStyleFlagVisible		0b00000000000000000000000000000001
#define PXGUIElementStyleFlagBorder			0b00000000000000000000000000000010
#define PXGUIElementStyleFlagScrollBarHor	0b00000000000000000000000000000100
#define PXGUIElementStyleFlagScrollBarVer	0b00000000000000000000000000001000

#define PXGUIElementStyleDefault PXGUIElementStyleFlagVisible | PXGUIElementStyleFlagBorder


	typedef union PXGUIElementCreateInfoData_
	{
		PXGUIElementCreateWindowInfo Window;
		PXUIElementTextInfo Text;
		PXUIElementButtonInfo Button;
		PXUIElementTreeViewItemInfo TreeViewItem;
		PXUIElementSceneRenderInfo SceneRender;
	}
	PXGUIElementCreateInfoData;

	typedef struct PXGUIElementCreateInfo_
	{
		PXUIElement** UIElement;
		PXUIElement* UIElementParent;
		PXUIElement* UIElementWindow;

		PXInt32U FlagList;

		PXUIElementType Type;
		PXInt32U BehaviourFlags;
		PXColorRGBAF* ColorTintReference;

		PXColorRGBAF Color;

		PXUIElementPosition Position;

		PXInt32U BehaviourFlagList;
		PXInt32U StyleFlagList;

#if OSUnix
#elif OSWindows
		PXInt32U WindowsWindowsStyleFlagsExtended;
		PXInt32U WindowsStyleFlags;
		char* WindowsTextContent;
		char* WindowsClassName;
#endif

		// Additions
		PXBool AvoidCreation;

		char* Name;

		void* InteractOwner;
		PXWindowEventFunction InteractCallBack;
				
		PXGUIElementCreateInfoData Data;
	}
	PXGUIElementCreateInfo;


	typedef struct PXGUIElementUpdateInfo_
	{
		PXUIElement* UIElement;
		PXWindow* WindowReference;
		PXUIElementProperty Property;
		PXGUIElementCreateInfoData Data;
	}
	PXGUIElementUpdateInfo;

	typedef struct PXWindowSizeInfo_
	{
		PXInt32S X;
		PXInt32S Y;
		PXInt32S Width;
		PXInt32S Height;
	}
	PXWindowSizeInfo;


	// Global Variable, bad but needed for SYNC with OS. Stupid design

	PXPrivate PXGUISystem* PXGUISystemGlobalReference = PXNull;


	PXPublic PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem);
	PXPublic PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem);


	PXPrivate void PXAPI PXWindowEventConsumer(PXGUISystem* const pxGUISystem, PXWindowEvent* const pxWindowEvent);

#if OSUnix
	PXPrivate void PXWindowEventHandler(PXWindow* const pxWindow, const XEvent* const xEvent);
#elif PXOSWindowsDestop
	PXPrivate LRESULT CALLBACK PXWindowEventHandler(const HWND PXWindowsID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);
#endif


	PXPrivate PXThreadResult PXOSAPI PXWindowMessageLoop(PXWindow* const pxWindow);


	PXPublic PXActionResult PXAPI PXGUIElementCreate(PXGUISystem* const pxGUISystem, PXGUIElementCreateInfo* const pxGUIElementCreateInfo, const PXSize amount);
	PXPublic PXActionResult PXAPI PXGUIElementUpdate(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfo, const PXSize amount);
	PXPublic void PXAPI PXGUIElementhSizeRefresAll(PXGUISystem* const pxGUISystem, const PXUIElement* pxUIElement);




	PXPublic PXActionResult PXAPI PXWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo);

	PXPublic void PXAPI PXWindowUpdate(PXUIElement* const pxUIElement);

	PXPublic void PXAPI PXWindowDestruct(PXWindow* const window);


	PXPublic PXProcessThreadID PXAPI PXWindowThreadProcessID(const PXWindowID windowID);

	PXPublic PXBool PXAPI PXWindowTitleSet(PXWindow* const window, const PXText* const title);
	PXPublic PXSize PXAPI PXWindowTitleGet(const PXWindow* const window, PXText* const title);

	PXPublic PXWindowID PXAPI PXWindowFindViaTitle(const PXText* const windowTitle);

	PXPublic void PXAPI PXWindowIconCorner();
	PXPublic void PXAPI PXWindowIconTaskBar();

	PXPublic PXWindow* PXAPI PXWindowLookupFind(const PXWindowID PXWindowID);
	PXPublic void PXAPI PXWindowLookupRemove(const PXWindow* PXWindow);

	PXPublic PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindowID pxWindowID);




	PXPublic PXActionResult PXAPI PXWindowSizeGet(const PXWindowID pxWindow, PXWindowSizeInfo* const pxWindowSizeInfo);
	PXPublic PXActionResult PXAPI PXWindowSizeSet(const PXWindowID pxWindow, PXWindowSizeInfo* const pxWindowSizeInfo);
	PXPublic PXActionResult PXAPI PXWindowPosition(PXWindow* const pxWindow, PXInt32S* x, PXInt32S* y);
	PXPublic PXActionResult PXAPI PXWindowMove(PXWindow* const pxWindow, const PXInt32S x, const PXInt32S y);
	PXPublic void PXAPI PXWindowPositonCenterScreen(PXWindow* const pxWindow);
	PXPublic void PXAPI PXWindowCursor(PXWindow* const pxWindow);
	//voidPXWindowCursor(const CursorIcon cursorIcon);
	PXPublic void PXAPI PXWindowCursorTexture();
	PXPublic void PXAPI PXWindowCursorCaptureMode(PXWindow* const pxWindow, const PXWindowCursorMode cursorMode);
	//voidPXWindowScreenShotTake(Image image);

	PXPublic PXBool PXAPI PXWindowFrameBufferSwap(const PXWindow* const pxWindow);

	PXPublic PXBool PXAPI PXWindowInteractable(PXWindow* const pxWindow);

	PXPublic PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y);
	PXPublic PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y);


	// Checks if the current window is the one in focus.
	PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindow* const window);


	PXPrivate PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);

#ifdef __cplusplus
}
#endif

#endif