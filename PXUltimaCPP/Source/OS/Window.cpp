#include "Window.hpp"

BF::Window::Window()
{
}

BF::Window::~Window()
{
}

void BF::Window::Create(const unsigned int width, const unsigned int height, const char* title, bool async)
{
	PXWindowCreateA(this, width, height, title, async);
}

void BF::Window::Destroy()
{
	PXWindowDestruct(this);
}

void BF::Window::IconCorner()
{
	//PXWindowIconCorner();
}

void BF::Window::IconTaskBar()
{
}

void BF::Window::Size(unsigned int& x, unsigned int& y, unsigned int& width, unsigned int& height)
{
	PXWindowSize(this, &x, &y, &width, &height);
}

void BF::Window::SizeChange(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
{
}

void BF::Window::Position(unsigned int& x, unsigned int& y)
{
}

void BF::Window::PositionChange(const unsigned int x, const unsigned int y)
{
}

void BF::Window::PositonCenterScreen()
{
}

void BF::Window::Cursor()
{
}

void BF::Window::CursorTexture()
{
}

void BF::Window::CursorCaptureMode(const PXWindowCursorMode cursorMode)
{
}

int BF::Window::FrameBufferInitialize()
{
	return 0;
}

bool BF::Window::FrameBufferSwap()
{
	return false;
}

bool BF::Window::FrameBufferContextRegister()
{
	return false;
}

bool BF::Window::FrameBufferContextRelease()
{
	return false;
}

bool BF::Window::Interactable()
{
	return false;
}