#pragma once

#include <OS/Hardware/PXMouse.h>

namespace PX
{
	class Mouse : public PXMouse
	{
		public:
		void InputReset();
		void InputPrint();
	};
}