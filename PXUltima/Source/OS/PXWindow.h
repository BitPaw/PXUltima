#ifndef PXWindowINCLUDE
#define PXWindowINCLUDE

#include <Format/Type.h>

#include <OS/OSVersion.h>

#if defined(OSUnix)

#include <X11/X.h>
#include <X11/Xlib.h>

#define PXWindowID XID // XID is PXWindow
#define OpenGLConextID GLXContext

#elif defined(OSWindows)
#include <Windows.h>
#define PXWindowID HWND
#define OpenGLConextID HGLRC
#endif

#include <Device/VirtualKey.h>
#include <Async/Thread.h>
#include <Device/Mouse.h>
#include <Device/KeyBoard.h>
#include <Graphic/Graphic.h>

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

	typedef enum ButtonState_
	{
		ButtonStateInvalid,
		ButtonStateDown,
		ButtonStateHold,
		ButtonStateRelease
	}
	ButtonState;

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
		VirtualKey Key;
		ButtonState Mode;
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
	typedef void (*MouseClickEvent)(const void* const receiver, const PXWindow* sender, const MouseButton mouseButton, const ButtonState buttonState);
	typedef void (*MouseClickDoubleEvent)(const void* const receiver, const PXWindow* sender, const MouseButton mouseButton);
	typedef void (*MouseMoveEvent)(const void* const receiver, const PXWindow* sender, const Mouse* mouse);
	typedef void (*MouseEnterEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*MouseLeaveEvent)(const void* const receiver, const PXWindow* sender);

	// Keyboard
	typedef void (*KeyBoardKeyEvent)(const void* const receiver, const PXWindow* sender, const KeyBoardKeyInfo keyBoardKeyInfo);

	typedef void (*FocusEnterEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*FocusLeaveEvent)(const void* const receiver, const PXWindow* sender);

	typedef void (*WindowCreatedEvent)(const void* const receiver, const PXWindow* sender);
	typedef void (*WindowSizeChangedEvent)(const void* const receiver, const PXWindow* sender, const size_t width, const size_t height);
	typedef void (*WindowClosingEvent)(const void* const receiver, const PXWindow* sender, unsigned char* allowClosing);
	typedef void (*WindowClosedEvent)(const void* const receiver, const PXWindow* sender);

	// 	static Dictionary<PXWindowID, PXWindow*> _windowLookup;

	typedef struct PXWindow_
	{
		volatile unsigned char IsRunning;
		PXWindowID ID;

		// Live data
		unsigned char HasSizeChanged;
		PXWindowCursorMode CursorModeCurrent;

		KeyBoard KeyBoardCurrentInput;
		Mouse MouseCurrentInput;

		unsigned int X;
		unsigned int Y;
		unsigned int Width;
		unsigned int Height;

		wchar_t Title[PXWindowTitleSizeMax];

		GraphicContext GraphicInstance;

#if defined(OSUnix)
		Display* DisplayCurrent;
		PXWindowID WindowRoot;
#elif defined(OSWindows)
		HCURSOR CursorID;
		HDC HandleDeviceContext;
#endif

		// Interneal
		ThreadID MessageThreadID;

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

#if defined(OSUnix)
	static void PXWindowEventHandler(PXWindow* const PXWindow, const XEvent* const event);
#elif defined(OSWindows)
	static LRESULT CALLBACK PXWindowEventHandler(HWND PXWindowsID, UINT eventID, WPARAM wParam, LPARAM lParam);
#endif

	static ThreadResult PXWindowCreateThread(void* const PXWindowAdress);

	CPublic void PXWindowConstruct(PXWindow* const PXWindow);
	CPublic void PXWindowCreate(PXWindow* const PXWindow, const unsigned int width, const unsigned int height, const char* title, unsigned char async);
	CPublic void PXWindowCreateHidden(PXWindow* const PXWindow, unsigned char async);
	CPublic void PXWindowDestruct(PXWindow* const PXWindow);

	CPublic void PXWindowIconCorner();
	CPublic void PXWindowIconTaskBar();

	CPublic void PXWindowLookupAdd(const PXWindow* PXWindow);
	CPublic PXWindow* PXWindowLookupFind(const PXWindowID PXWindowID);
	CPublic void PXWindowLookupRemove(const PXWindow* PXWindow);

	CPublic void PXWindowSize(PXWindow* PXWindow, unsigned int* x, unsigned int* y, unsigned int* width, unsigned int* height);
	CPublic void PXWindowSizeChange(PXWindow* PXWindow, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);
	CPublic void PXWindowPosition(PXWindow* PXWindow, unsigned int* x, unsigned int* y);
	CPublic void PXWindowPositionChange(PXWindow* PXWindow, const unsigned int x, const unsigned int y);
	CPublic void PXWindowPositonCenterScreen(PXWindow* PXWindow);
	CPublic void PXWindowCursor(PXWindow* PXWindow);
	//voidPXWindowCursor(const CursorIcon cursorIcon);
	CPublic void PXWindowCursorTexture();
	CPublic void PXWindowCursorCaptureMode(PXWindow* PXWindow, const PXWindowCursorMode cursorMode);
	//voidPXWindowScreenShotTake(Image image);

	CPublic unsigned char PXWindowFrameBufferSwap(PXWindow* PXWindow);

	CPublic unsigned char PXWindowInteractable(PXWindow* PXWindow);

	CPublic unsigned char PXWindowCursorPositionInWindowGet(PXWindow* window, int* x, int* y);
	CPublic unsigned char PXWindowCursorPositionInDestopGet(PXWindow* window, int* x, int* y);



	// Event functions
	CPublic void TriggerOnMouseScrollEvent(const PXWindow* window, const Mouse* mouse);
	CPublic void TriggerOnMouseClickEvent(const PXWindow* window, const MouseButton mouseButton, const ButtonState buttonState);
	CPublic void TriggerOnMouseClickDoubleEvent(const PXWindow* window, const MouseButton mouseButton);
	CPublic void TriggerOnMouseMoveEvent(const PXWindow* window, const int positionX, const int positionY, const int deltaX, const int deltaY);
	CPublic void TriggerOnMouseEnterEvent(const PXWindow* window, const Mouse* mouse);
	CPublic void TriggerOnMouseLeaveEvent(const PXWindow* window, const Mouse* mouse);


	CPublic void TriggerOnKeyBoardKeyEvent(const PXWindow* window, const KeyBoardKeyInfo keyBoardKeyInfo);

	// Keyboard
	//typedef void (*KeyBoardKeyEvent)(const KeyBoardKeyInfo keyBoardKeyInfo);

	//typedef void (*FocusEnterEvent)();
	//typedef void (*FocusLeaveEvent)();

	//typedef void (*WindowCreatedEvent)(const PXWindow* PXWindow);
	//typedef void (*WindowSizeChangedEvent)(const size_t width, const size_t height);
	//typedef void (*WindowClosingEvent)(unsigned char* allowClosing);
	//typedef void (*WindowClosedEvent)();


	unsigned int CursorIconToID(const CursorIcon cursorIcon);

#ifdef __cplusplus
}
#endif

#endif
