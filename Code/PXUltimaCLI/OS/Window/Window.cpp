#include "Window.hpp"

#include <atlstr.h>

PX::Window::Window()
{
	_window = new PXWindow();
	PXWindowConstruct(_window);
}

PX::Window::~Window()
{
	PXWindowDestruct(_window);
	delete _window;
}

void PX::Window::Create(const unsigned int width, const unsigned int height, const bool async)
{
	PXWindowCreate(_window, width, height, async);
}

void PX::Window::Create(const unsigned int width, const unsigned int height, const System::String^ title, const bool async)
{
	pin_ptr<const wchar_t> titleAdress = PtrToStringChars(title);

	PXWindowCreateW(_window, width, height, titleAdress, async);
}

void PX::Window::CreateHidden(const unsigned int width, const unsigned int height, const bool async)
{
	PXWindowCreateHidden(_window, width, height, async);
}

bool PX::Window::BufferSwap()
{
	return PXWindowFrameBufferSwap(_window);
}