#include "PXWindow.hpp"
#include <Container/Dictionary/PXDictionary.hpp>

PX::Window::Window()
{
	PXWindowConstruct(this);
}

PX::Window::~Window()
{
	PXWindowDestruct(this);
}

float PX::Window::ScreenRatio()
{
	return PXWindowScreenRatio(this);
}

void PX::Window::Create(const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height, const Text& const title, const bool async)
{
	PXWindowCreate(this, x, y, width, height, &title, async);
}

void PX::Window::CreateHidden(const PXInt32S width, const PXInt32S height, const bool async)
{
	PXWindowCreateHidden(this, width, height, async);
}

PXProcessThreadID PX::Window::ThreadProcessID(const PXWindowID windowID)
{
	return PXWindowThreadProcessID(this->ID);
}

bool PX::Window::TitleSet(const Text& const title)
{
	return PXWindowTitleSet(this, &title);
}

PXSize PX::Window::TitleGet(Text& const title)
{
	return PXWindowTitleGet(this, &title);
}

PXWindowID PX::Window::FindViaTitle(const Text& const windowTitle)
{
	return PXWindowID();
}

void PX::Window::IconCorner()
{
}

void PX::Window::IconTaskBar()
{
}

void PX::Window::Size(PXInt32S& x, PXInt32S& y, PXInt32S& width, PXInt32S& height)
{
}

void PX::Window::SizeChange(const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height)
{
}

void PX::Window::Position(PXInt32S& x, PXInt32S& y)
{
}

void PX::Window::PositionChange(const PXInt32S x, const PXInt32S y)
{
}

void PX::Window::PositonCenterScreen()
{
}

void PX::Window::Cursor()
{
}

void PX::Window::CursorTexture()
{
}

void PX::Window::CursorCaptureMode(const WindowCursorMode cursorMode)
{
}

bool PX::Window::FrameBufferSwap()
{
	return PXWindowFrameBufferSwap(this);
}

bool PX::Window::Interactable()
{
	return false;
}

bool PX::Window::CursorPositionInWindowGet(PXInt32S& x, PXInt32S& y)
{
	return false;
}

bool PX::Window::CursorPositionInDestopGet(PXInt32S& x, PXInt32S& y)
{
	return false;
}

bool PX::Window::IsInFocus()
{
	return false;
}