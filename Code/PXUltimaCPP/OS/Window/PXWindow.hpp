#ifndef PXCPPWindowINCLUDE
#define PXCPPWindowINCLUDE

#include <OS/Window/PXWindow.h>
#include <Media/PXText.hpp>

namespace PX
{
	// The mode in which the mouse pointer will be.
	enum class WindowCursorMode
	{
		Ignore, 	// Show the cursor but dont register any Input.
		Show, 	// Show the cursor and register Input as normal.
		Invisible, 	// Hide Cursor. You can still use it as normal. Not locked.
		Lock,
		LockAndHide
	};

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

	typedef enum PXWindowMode_
	{
		PXWindowModeInvalid,
		PXWindowModeNormal,
		PXWindowModeHidden
	}
	PXWindowMode;

	class Window : public PXWindow
	{
		public:

		Window();
		~Window();

		float ScreenRatio();

		void Create(const PXInt32S width, const PXInt32S height, const Text& const title, const bool async);
		void CreateHidden(const PXInt32S width, const PXInt32S height, const bool async);


		PXProcessThreadID ThreadProcessID(const PXWindowID windowID);

		bool TitleSet(const Text& const title);
		PXSize TitleGet(Text& const title);

		PXWindowID FindViaTitle(const Text& const windowTitle);

		void IconCorner();
		void IconTaskBar();

		void Size(PXInt32S& x, PXInt32S& y, PXInt32S& width, PXInt32S& height);
		void SizeChange(const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height);
		void Position(PXInt32S& x, PXInt32S& y);
		void PositionChange(const PXInt32S x, const PXInt32S y);
		void PositonCenterScreen();
		void Cursor();
		//voidPXWindowCursor(const CursorIcon cursorIcon);
		void CursorTexture();
		void CursorCaptureMode(const WindowCursorMode cursorMode);
		//voidPXWindowScreenShotTake(Image image);

		bool FrameBufferSwap();

		bool Interactable();

		bool CursorPositionInWindowGet(PXInt32S& x, PXInt32S& y);
		bool CursorPositionInDestopGet(PXInt32S& x, PXInt32S& y);

		bool IsInFocus();
	};
}

#endif