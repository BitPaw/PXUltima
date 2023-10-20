#ifndef PXWindowINCLUDE
#define PXWindowINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXWindowUSE OSDeviceToUse == OSDeviceDestop
#if PXWindowUSE

#include <Media/PXType.h>

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

#include <OS/Graphic/PXGraphic.h>
#include <OS/Hardware/PXMouse.h>
#include <OS/Hardware/PXKeyBoard.h>
#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>

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

	typedef struct PXKeyBoardKeyInfo_
	{
		unsigned char KeyID;
		PXVirtualKey Key;
		PXKeyPressState Mode;
		unsigned short Repeat; // Die Wiederholungsanzahl für die aktuelle Meldung.Der Wert gibt an, wie oft die Tastatureingabe automatisch angezeigt wird, wenn der Benutzer den Schlüssel hält.Die Wiederholungsanzahl ist immer 1 für eine WM _ KEYUP - Nachricht.
		unsigned short ScanCode;// Der Scancode.Der Wert hängt vom OEM ab.
		unsigned short SpecialKey;// Gibt an, ob es sich bei der Taste um eine erweiterte Taste handelt, z.B.die rechte ALT - und STRG - Taste, die auf einer erweiterten Tastatur mit 101 oder 102 Tasten angezeigt werden.Der Wert ist 1, wenn es sich um einen erweiterten Schlüssel handelt.andernfalls ist es 0.
		unsigned short KontextCode; // Der Kontextcode.Der Wert ist für eine WM _ KEYUP - Nachricht immer 0.
		unsigned short PreState; // Der vorherige Schlüsselzustand.Der Wert ist immer 1 für eine WM _ KEYUP - Nachricht.
		unsigned short GapState;
	}
	PXKeyBoardKeyInfo;

	typedef struct PXWindow_ PXWindow;

	// Mouse
	typedef void (PXAPI*MouseScrollEvent)(const void* const receiver, const PXWindow* sender, const PXMouseScrollDirection mouseScrollDirection);
	typedef void (PXAPI*MouseClickEvent)(const void* const receiver, const PXWindow* sender, const PXMouseButton mouseButton, const PXKeyPressState buttonState);
	typedef void (PXAPI*MouseClickDoubleEvent)(const void* const receiver, const PXWindow* sender, const PXMouseButton mouseButton);
	typedef void (PXAPI*MouseMoveEvent)(const void* const receiver, const PXWindow* sender, const PXMouse* mouse);
	typedef void (PXAPI*MouseEnterEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (PXAPI*MouseLeaveEvent)(const void* const receiver, const PXWindow* sender);

	// Keyboard
	typedef void (PXAPI*KeyBoardKeyEvent)(const void* const receiver, const PXWindow* sender, const PXKeyBoardKeyInfo* const keyBoardKeyInfo);

	typedef void (PXAPI*FocusEnterEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (PXAPI*FocusLeaveEvent)(const void* const receiver, const PXWindow* sender);

	typedef void (PXAPI*WindowCreatedEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (PXAPI*WindowSizeChangedEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (PXAPI*WindowClosingEvent)(const void* const receiver, const PXWindow* sender, PXBool* const allowClosing);
	typedef void (PXAPI*WindowClosedEvent)(const void* const receiver, const PXWindow* sender);

	// 	static PXDictionary<PXWindowID, PXWindow*> _windowLookup;

	typedef enum PXWindowMode_
	{
		PXWindowModeInvalid,
		PXWindowModeNormal,
		PXWindowModeHidden
	}
	PXWindowMode;

	typedef struct PXWindow_
	{
		volatile PXBool IsRunning;
		PXWindowID ID;

		PXInt32S X;
		PXInt32S Y;
		PXInt32S Width;
		PXInt32S Height;

		char TitleBuffer[256];
		PXText Title;

		// Live data
		PXBool HasSizeChanged;
		PXWindowMode Mode;
		PXWindowCursorMode CursorModeCurrent;

		PXKeyBoard KeyBoardCurrentInput;
		PXMouse MouseCurrentInput;

#if OSUnix
		Display* DisplayCurrent;
		PXWindowID WindowRoot;
#elif OSWindows
		HCURSOR CursorID;
		HDC HandleDeviceContext;
#endif

		// Interneal
		PXThread MessageThread;

		void* EventReceiver;

		MouseScrollEvent MouseScrollCallBack;
		MouseClickEvent MouseClickCallBack;
		MouseClickDoubleEvent MouseClickDoubleCallBack;
		MouseEnterEvent MouseEnterCallBack;
		MouseLeaveEvent MouseLeaveCallBack;
		MouseMoveEvent MouseMoveCallBack;

		KeyBoardKeyEvent KeyBoardKeyCallBack;

		FocusEnterEvent FocusEnterCallBack;
		FocusLeaveEvent FocusLeaveCallBack;
		WindowCreatedEvent WindowCreatedCallBack;
		WindowSizeChangedEvent WindowSizeChangedCallBack;
		WindowClosingEvent WindowClosingCallBack;
		WindowClosedEvent WindowClosedCallBack;
	}
	PXWindow;

#if OSUnix
	PXPrivate void PXWindowEventHandler(PXWindow* const pxWindow, const XEvent* const xEvent);
#elif PXOSWindowsDestop
	PXPrivate LRESULT CALLBACK PXWindowEventHandler(const HWND PXWindowsID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);
#endif

	PXPrivate PXThreadResult PXOSAPI PXWindowCreateThread(PXWindow* const PXWindowAdress);


	PXPublic PXActionResult PXAPI PXWindowPixelSystemSet(PXWindow* const window);

	PXPublic void PXAPI PXWindowUpdate(PXWindow* const pxWindow);

	PXPublic void PXAPI PXWindowConstruct(PXWindow* const window);
	PXPublic void PXAPI PXWindowDestruct(PXWindow* const window);

	PXPublic float PXAPI PXWindowScreenRatio(const PXWindow* const window);

	PXPublic void PXAPI PXWindowCreateA(PXWindow* const window, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height, const char* const title, const PXBool async);

	// Create a window based on the OS implementation.
	// if a NULL pointer is used as a title, the window will be hidden.
	PXPublic void PXAPI PXWindowCreate(PXWindow* const window, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height, const PXText* const title, const PXBool async);
	PXPublic void PXAPI PXWindowCreateHidden(PXWindow* const window, const PXInt32S width, const PXInt32S height, const PXBool async);


	PXPublic PXProcessThreadID PXAPI PXWindowThreadProcessID(const PXWindowID windowID);

	PXPublic PXBool PXAPI PXWindowTitleSet(PXWindow* const window, const PXText* const title);
	PXPublic PXSize PXAPI PXWindowTitleGet(const PXWindow* const window, PXText* const title);

	PXPublic PXWindowID PXAPI PXWindowFindViaTitle(const PXText* const windowTitle);

	PXPublic void PXAPI PXWindowIconCorner();
	PXPublic void PXAPI PXWindowIconTaskBar();

	PXPublic void PXAPI PXWindowLookupAdd(PXWindow* const PXWindow);
	PXPublic PXWindow* PXAPI PXWindowLookupFind(const PXWindowID PXWindowID);
	PXPublic void PXAPI PXWindowLookupRemove(const PXWindow* PXWindow);

	PXPublic PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindow* const pxWindow);

	PXPublic void PXAPI PXWindowSize(const PXWindow* const pxWindow, PXInt32S* const x, PXInt32S* const y, PXInt32S* const width, PXInt32S* const height);
	PXPublic void PXAPI PXWindowSizeChange(PXWindow* const pxWindow, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height);
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

	PXPublic PXBool PXAPI PXWindowCursorPositionInWindowGet(PXWindow* const window, PXInt32S* const x, PXInt32S* const y);
	PXPublic PXBool PXAPI PXWindowCursorPositionInDestopGet(PXWindow* const window, PXInt32S* const x, PXInt32S* const y);


	// Checks if the current window is the one in focus.
	PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindow* const window);


	// Event functions
	PXPublic void PXAPI PXWindowTriggerOnMouseScrollEvent(const PXWindow* window, const PXMouse* mouse);
	PXPublic void PXAPI PXWindowTriggerOnMouseClickEvent(PXWindow* const window, const PXMouseButton mouseButton, const PXKeyPressState buttonState);
	PXPublic void PXAPI PXWindowTriggerOnMouseClickDoubleEvent(const PXWindow* window, const PXMouseButton mouseButton);
	PXPublic void PXAPI PXWindowTriggerOnMouseMoveEvent(PXWindow* const window, const PXInt32S positionX, const PXInt32S positionY, const PXInt32S deltaX, const PXInt32S deltaY);
	PXPublic void PXAPI PXWindowTriggerOnMouseEnterEvent(const PXWindow* window, const PXMouse* mouse);
	PXPublic void PXAPI PXWindowTriggerOnMouseLeaveEvent(const PXWindow* window, const PXMouse* mouse);


	PXPublic void PXAPI PXWindowTriggerOnKeyBoardKeyEvent(PXWindow* const window, const PXKeyBoardKeyInfo* const keyBoardKeyInfo);

	// Keyboard
	//typedef void (*KeyBoardKeyEvent)(const KeyBoardKeyInfo keyBoardKeyInfo);

	//typedef void (*FocusEnterEvent)();
	//typedef void (*FocusLeaveEvent)();

	//typedef void (*WindowCreatedEvent)(const PXWindow* PXWindow);
	//typedef void (*WindowSizeChangedEvent)(const PXSize width, const PXSize height);
	//typedef void (*WindowClosingEvent)(unsigned char* allowClosing);
	//typedef void (*WindowClosedEvent)();


	PXPrivate PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);

#ifdef __cplusplus
}
#endif

#endif
#endif
