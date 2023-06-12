#pragma once

#include <Media/LZ77/PXLZ77.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class LZ77
	{
		public:
		static PX::ActionResult Encode();
		static PX::ActionResult Decode();
	};
}