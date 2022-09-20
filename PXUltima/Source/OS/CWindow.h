#ifndef CWindowInclude
#define CWindowInclude

#include <Format/Type.h>

#include <OS/OSVersion.h>

#if defined(OSUnix)

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#define CWindowID XID // XID is CWindow
#define OpenGLConextID GLXContext

#elif defined(OSWindows)
#include <Windows.h>
#define CWindowID HWND
#define OpenGLConextID HGLRC
#endif

#include <Device/VirtualKey.h>
#include <Async/Thread.h>
#include <Device/Mouse.h>
#include <Device/KeyBoard.h>
#include <Graphics/OpenGL/OpenGL.h>

#define UseRawMouseData 1
#define CWindowTitleSizeMax 256
#define CWindowSizeDefault -1

#ifdef __cplusplus
extern "C"
{
#endif

	// The mode in which the mouse pointer will be.
	typedef enum CWindowCursorMode_
	{
		CWindowCursorIgnore, 	// Show the cursor but dont register any Input.
		CWindowCursorShow, 	// Show the cursor and register Input as normal.
		CWindowCursorInvisible, 	// Hide Cursor. You can still use it as normal. Not locked.
		CWindowCursorLock,
		CWindowCursorLockAndHide
	}
	CWindowCursorMode;

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

	typedef struct CWindow_ CWindow;

	// Mouse
	typedef void (*MouseScrollEvent)(const void* const receiver, const CWindow* sender, const MouseScrollDirection mouseScrollDirection);
	typedef void (*MouseClickEvent)(const void* const receiver, const CWindow* sender, const MouseButton mouseButton, const ButtonState buttonState);
	typedef void (*MouseClickDoubleEvent)(const void* const receiver, const CWindow* sender, const MouseButton mouseButton);
	typedef void (*MouseMoveEvent)(const void* const receiver, const CWindow* sender, const Mouse* mouse);
	typedef void (*MouseEnterEvent)(const void* const receiver, const CWindow* sender);
	typedef void (*MouseLeaveEvent)(const void* const receiver, const CWindow* sender);

	// Keyboard
	typedef void (*KeyBoardKeyEvent)(const void* const receiver, const CWindow* sender, const KeyBoardKeyInfo keyBoardKeyInfo);

	typedef void (*FocusEnterEvent)(const void* const receiver, const CWindow* sender);
	typedef void (*FocusLeaveEvent)(const void* const receiver, const CWindow* sender);

	typedef void (*WindowCreatedEvent)(const void* const receiver, const CWindow* sender);
	typedef void (*WindowSizeChangedEvent)(const void* const receiver, const CWindow* sender, const size_t width, const size_t height);
	typedef void (*WindowClosingEvent)(const void* const receiver, const CWindow* sender, unsigned char* allowClosing);
	typedef void (*WindowClosedEvent)(const void* const receiver, const CWindow* sender);

	// 	static Dictionary<CWindowID, CWindow*> _windowLookup;

	typedef struct CWindow_
	{
		volatile unsigned char IsRunning;
		CWindowID ID;

		// Live data
		unsigned char HasSizeChanged;
		CWindowCursorMode CursorModeCurrent;

		KeyBoard KeyBoardCurrentInput;
		Mouse MouseCurrentInput;

		unsigned int X;
		unsigned int Y;
		unsigned int Width;
		unsigned int Height;

		wchar_t Title[CWindowTitleSizeMax];

		OpenGLContext GLContext;

#if defined(OSUnix)
		Display* DisplayCurrent;
		CWindowID WindowRoot;
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
	CWindow;

#if defined(OSUnix)
	static void CWindowEventHandler(CWindow* const cWindow, const XEvent* const event);
#elif defined(OSWindows)
	static LRESULT CALLBACK CWindowEventHandler(HWND cWindowsID, UINT eventID, WPARAM wParam, LPARAM lParam);
#endif

	static ThreadResult CWindowCreateThread(void* const cWindowAdress);

	CPublic void CWindowConstruct(CWindow* const cWindow);
	CPublic void CWindowCreate(CWindow* const cWindow, const unsigned int width, const unsigned int height, const char* title, unsigned char async);
	CPublic void CWindowDestruct(CWindow* const cWindow);

	CPublic void CWindowIconCorner();
	CPublic void CWindowIconTaskBar();

	CPublic void CWindowLookupAdd(const CWindow* CWindow);
	CPublic CWindow* CWindowLookupFind(const CWindowID cWindowID);
	CPublic void CWindowLookupRemove(const CWindow* CWindow);

	CPublic void CWindowSize(CWindow* CWindow, unsigned int* x, unsigned int* y, unsigned int* width, unsigned int* height);
	CPublic void CWindowSizeChange(CWindow* CWindow, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);
	CPublic void CWindowPosition(CWindow* CWindow, unsigned int* x, unsigned int* y);
	CPublic void CWindowPositionChange(CWindow* CWindow, const unsigned int x, const unsigned int y);
	CPublic void CWindowPositonCenterScreen(CWindow* CWindow);
	CPublic void CWindowCursor(CWindow* CWindow);
	//voidCWindowCursor(const CursorIcon cursorIcon);
	CPublic void CWindowCursorTexture();
	CPublic void CWindowCursorCaptureMode(CWindow* CWindow, const CWindowCursorMode cursorMode);
	//voidCWindowScreenShotTake(Image image);
	CPublic int CWindowFrameBufferInitialize(CWindow* CWindow);
	CPublic unsigned char CWindowFrameBufferSwap(CWindow* CWindow);

	CPublic unsigned char CWindowFrameBufferContextRelease(CWindow* CWindow);
	CPublic unsigned char CWindowInteractable(CWindow* CWindow);

	CPublic unsigned char CWindowCursorPositionInWindowGet(CWindow* window, int* x, int* y);
	CPublic unsigned char CWindowCursorPositionInDestopGet(CWindow* window, int* x, int* y);



	// Event functions
	CPublic void TriggerOnMouseScrollEvent(const CWindow* window, const Mouse* mouse);
	CPublic void TriggerOnMouseClickEvent(const CWindow* window, const MouseButton mouseButton, const ButtonState buttonState);
	CPublic void TriggerOnMouseClickDoubleEvent(const CWindow* window, const MouseButton mouseButton);
	CPublic void TriggerOnMouseMoveEvent(const CWindow* window, const int positionX, const int positionY, const int deltaX, const int deltaY);
	CPublic void TriggerOnMouseEnterEvent(const CWindow* window, const Mouse* mouse);
	CPublic void TriggerOnMouseLeaveEvent(const CWindow* window, const Mouse* mouse);


	CPublic void TriggerOnKeyBoardKeyEvent(const CWindow* window, const KeyBoardKeyInfo keyBoardKeyInfo);

	// Keyboard
	//typedef void (*KeyBoardKeyEvent)(const KeyBoardKeyInfo keyBoardKeyInfo);

	//typedef void (*FocusEnterEvent)();
	//typedef void (*FocusLeaveEvent)();

	//typedef void (*WindowCreatedEvent)(const CWindow* cWindow);
	//typedef void (*WindowSizeChangedEvent)(const size_t width, const size_t height);
	//typedef void (*WindowClosingEvent)(unsigned char* allowClosing);
	//typedef void (*WindowClosedEvent)();


#ifdef __cplusplus
}
#endif

#endif
