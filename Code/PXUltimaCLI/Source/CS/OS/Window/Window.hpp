#pragma once

#include <OS/Window/PXWindow.h>

namespace PX
{
	public ref class Window
	{
		private:
		PXWindow* _window;

		public:
		Window();
		~Window();

		property bool IsRunning { bool get() { return _window->IsRunning; }}

		property unsigned int X { unsigned int get() { return _window->X; }}
		property unsigned int Y { unsigned int get() { return _window->Y; }}
		property unsigned int Width { unsigned int get() { return _window->Width; }}
		property unsigned int Height { unsigned int get() { return _window->Height; }}

		property System::String^ Title { System::String^ get() { return gcnew System::String(_window->Title);; }}

		void Create(const unsigned int width, const unsigned int height, const bool async);
		void Create(const unsigned int width, const unsigned int height, const System::String^ title, const bool async);
		void CreateHidden(const unsigned int width, const unsigned int height, const bool async);

		bool BufferSwap();
	};
}