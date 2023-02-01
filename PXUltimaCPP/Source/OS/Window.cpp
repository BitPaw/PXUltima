#include "Window.hpp"

PX::Window::Window()
{
	PXWindowConstruct(this);
}

PX::Window::~Window()
{
	PXWindowDestruct(this);
}

void PX::Window::Create(const unsigned int width, const unsigned int height, const char* title, bool async)
{
	PXWindowCreateA(this, width, height, title, async);
}

void PX::Window::Destroy()
{
	PXWindowDestruct(this);
}

void PX::Window::IconCorner()
{
	//PXWindowIconCorner();
}

void PX::Window::IconTaskBar()
{
}

void PX::Window::Size(unsigned int& x, unsigned int& y, unsigned int& width, unsigned int& height)
{
	PXWindowSize(this, &x, &y, &width, &height);
}

void PX::Window::SizeChange(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
{
}

void PX::Window::Position(unsigned int& x, unsigned int& y)
{
}

void PX::Window::PositionChange(const unsigned int x, const unsigned int y)
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

void PX::Window::CursorCaptureMode(const PXWindowCursorMode cursorMode)
{
}

int PX::Window::FrameBufferInitialize()
{
	return 0;
}

bool PX::Window::FrameBufferSwap()
{
	return false;
}

bool PX::Window::FrameBufferContextRegister()
{
	return false;
}

bool PX::Window::FrameBufferContextRelease()
{
	return false;
}

bool PX::Window::Interactable()
{
	return false;
}