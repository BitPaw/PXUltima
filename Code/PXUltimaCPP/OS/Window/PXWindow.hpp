#pragma once

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
		PXDLLExport Window();
		PXDLLExport ~Window();

		PXDLLExport float ScreenRatio();

		PXDLLExport void Create(const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height, const Text& const title, const bool async);
		PXDLLExport void CreateHidden(const PXInt32S width, const PXInt32S height, const bool async);


		PXDLLExport PXProcessThreadID ThreadProcessID(const PXWindowID windowID);

		PXDLLExport bool TitleSet(const Text& const title);
		PXDLLExport PXSize TitleGet(Text& const title);

		PXDLLExport PXWindowID FindViaTitle(const Text& const windowTitle);

		PXDLLExport void IconCorner();
		PXDLLExport void IconTaskBar();

		PXDLLExport void Size(PXInt32S& x, PXInt32S& y, PXInt32S& width, PXInt32S& height);
		PXDLLExport void SizeChange(const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height);
		PXDLLExport void Position(PXInt32S& x, PXInt32S& y);
		PXDLLExport void PositionChange(const PXInt32S x, const PXInt32S y);
		PXDLLExport void PositonCenterScreen();
		PXDLLExport void Cursor();
		//voidPXWindowCursor(const CursorIcon cursorIcon);
		PXDLLExport void CursorTexture();
		PXDLLExport void CursorCaptureMode(const WindowCursorMode cursorMode);
		//voidPXWindowScreenShotTake(Image image);

		PXDLLExport bool FrameBufferSwap();

		PXDLLExport bool Interactable();

		PXDLLExport bool CursorPositionInWindowGet(PXInt32S& x, PXInt32S& y);
		PXDLLExport bool CursorPositionInDestopGet(PXInt32S& x, PXInt32S& y);

		PXDLLExport bool IsInFocus();
	};
}