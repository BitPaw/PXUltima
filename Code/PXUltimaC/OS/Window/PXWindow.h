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

#include <Graphic/PXGraphic.h>
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

	typedef enum MouseButton_
	{
		MouseButtonInvalid,
		MouseButtonLeft,
		MouseButtonMiddle,
		MouseButtonRight,
		MouseButtonSpecialA,
		MouseButtonSpecialB,
		MouseButtonSpecialC,
		MouseButtonSpecialD,
		MouseButtonSpecialE,
	}
	MouseButton;

	typedef enum MouseScrollDirection_
	{
		MouseScrollDirectionInvalid,
		MouseScrollDirectionUp,
		MouseScrollDirectionDown,
		MouseScrollDirectionLeft,
		MouseScrollDirectionRight
	}
	MouseScrollDirection;

	typedef enum CursorIcon_
	{
		CursorIconInvalid,
		CursorIconNormal,
		CursorIconIBeam,
		CursorIconWait,
		CursorIconCross,
		CursorIconUp,
		CursorIconHand,
		CursorIconNotAllowed,
		CursorIconAppStarting,
		CursorIconResizeHorizontal,
		CursorIconResizeVertical,
		CursorIconResizeClockwise,
		CursorIconResizeClockwiseCounter,
		CursorIconResizeAll,
	}
	CursorIcon;

	typedef struct KeyBoardKeyInfo_
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
	KeyBoardKeyInfo;

	typedef struct PXWindow_ PXWindow;

	// Mouse
	typedef void (*MouseScrollEvent)(const void* const receiver, const PXWindow* sender, const MouseScrollDirection mouseScrollDirection);
	typedef void (*MouseClickEvent)(const void* const receiver, const PXWindow* sender, const MouseButton mouseButton, const PXKeyPressState buttonState);
	typedef void (*MouseClickDoubleEvent)(const void* const receiver, const PXWindow* sender, const MouseButton mouseButton);
	typedef void (*MouseMoveEvent)(const void* const receiver, const PXWindow* sender, const PXMouse* mouse);
	typedef void (*MouseEnterEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*MouseLeaveEvent)(const void* const receiver, const PXWindow* sender);

	// Keyboard
	typedef void (*KeyBoardKeyEvent)(const void* const receiver, const PXWindow* sender, const KeyBoardKeyInfo* const keyBoardKeyInfo);

	typedef void (*FocusEnterEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*FocusLeaveEvent)(const void* const receiver, const PXWindow* sender);

	typedef void (*WindowCreatedEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*WindowSizeChangedEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*WindowClosingEvent)(const void* const receiver, const PXWindow* sender, PXBool* const allowClosing);
	typedef void (*WindowClosedEvent)(const void* const receiver, const PXWindow* sender);

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

		PXGraphicContext GraphicInstance;




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

	PXPublic void PXWindowConstruct(PXWindow* const window);
	PXPublic void PXWindowDestruct(PXWindow* const window);

	PXPublic float PXWindowScreenRatio(const PXWindow* const window);

	// Create a window based on the OS implementation.
	// if a NULL pointer is used as a title, the window will be hidden.
	PXPublic void PXWindowCreate(PXWindow* const window, const PXInt32S width, const PXInt32S height, const PXText* const title, const PXBool async);
	PXPublic void PXWindowCreateHidden(PXWindow* const window, const PXInt32S width, const PXInt32S height, const PXBool async);


	PXPublic PXProcessThreadID PXWindowThreadProcessID(const PXWindowID windowID);

	PXPublic PXBool PXWindowTitleSet(PXWindow* const window, const PXText* const title);
	PXPublic PXSize PXWindowTitleGet(const PXWindow* const window, PXText* const title);

	PXPublic PXWindowID PXWindowFindViaTitle(const PXText* const windowTitle);

	PXPublic void PXWindowIconCorner();
	PXPublic void PXWindowIconTaskBar();

	PXPublic void PXWindowLookupAdd(const PXWindow* PXWindow);
	PXPublic PXWindow* PXWindowLookupFind(const PXWindowID PXWindowID);
	PXPublic void PXWindowLookupRemove(const PXWindow* PXWindow);

	PXPublic PXActionResult PXWindowTitleBarColorSet(const PXWindow* const pxWindow);

	PXPublic void PXWindowSize(const PXWindow* const pxWindow, PXInt32S* const x, PXInt32S* const y, PXInt32S* const width, PXInt32S* const height);
	PXPublic void PXWindowSizeChange(PXWindow* const pxWindow, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height);
	PXPublic PXActionResult PXWindowPosition(PXWindow* const pxWindow, PXInt32S* x, PXInt32S* y);
	PXPublic PXActionResult PXWindowMove(PXWindow* const pxWindow, const PXInt32S x, const PXInt32S y);
	PXPublic void PXWindowPositonCenterScreen(PXWindow* const pxWindow);
	PXPublic void PXWindowCursor(PXWindow* const pxWindow);
	//voidPXWindowCursor(const CursorIcon cursorIcon);
	PXPublic void PXWindowCursorTexture();
	PXPublic void PXWindowCursorCaptureMode(PXWindow* const pxWindow, const PXWindowCursorMode cursorMode);
	//voidPXWindowScreenShotTake(Image image);

	PXPublic PXBool PXWindowFrameBufferSwap(const PXWindow* const pxWindow);

	PXPublic PXBool PXWindowInteractable(PXWindow* const pxWindow);

	PXPublic PXBool PXWindowCursorPositionInWindowGet(PXWindow* const window, PXInt32S* const x, PXInt32S* const y);
	PXPublic PXBool PXWindowCursorPositionInDestopGet(PXWindow* const window, PXInt32S* const x, PXInt32S* const y);


	// Checks if the current window is the one in focus.
	PXPublic PXBool PXWindowIsInFocus(const PXWindow* const window);


	// Event functions
	PXPublic void PXWindowTriggerOnMouseScrollEvent(const PXWindow* window, const PXMouse* mouse);
	PXPublic void PXWindowTriggerOnMouseClickEvent(const PXWindow* window, const MouseButton mouseButton, const PXKeyPressState buttonState);
	PXPublic void PXWindowTriggerOnMouseClickDoubleEvent(const PXWindow* window, const MouseButton mouseButton);
	PXPublic void PXWindowTriggerOnMouseMoveEvent(const PXWindow* window, const PXInt32S positionX, const PXInt32S positionY, const PXInt32S deltaX, const PXInt32S deltaY);
	PXPublic void PXWindowTriggerOnMouseEnterEvent(const PXWindow* window, const PXMouse* mouse);
	PXPublic void PXWindowTriggerOnMouseLeaveEvent(const PXWindow* window, const PXMouse* mouse);


	PXPublic void PXWindowTriggerOnKeyBoardKeyEvent(const PXWindow* window, const KeyBoardKeyInfo* const keyBoardKeyInfo);

	// Keyboard
	//typedef void (*KeyBoardKeyEvent)(const KeyBoardKeyInfo keyBoardKeyInfo);

	//typedef void (*FocusEnterEvent)();
	//typedef void (*FocusLeaveEvent)();

	//typedef void (*WindowCreatedEvent)(const PXWindow* PXWindow);
	//typedef void (*WindowSizeChangedEvent)(const PXSize width, const PXSize height);
	//typedef void (*WindowClosingEvent)(unsigned char* allowClosing);
	//typedef void (*WindowClosedEvent)();


	PXPublic unsigned int PXWindowCursorIconToID(const CursorIcon cursorIcon);

#ifdef __cplusplus
}
#endif

#endif
#endif
