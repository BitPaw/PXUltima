#pragma once

#include <Media/DEFLATE/PXDEFLATE.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class Deflate
	{
		public:
		static PX::ActionResult Parse(PX::File& pxInputStream, PX::File& pxOutputStream);
		static PX::ActionResult Serialize(PX::File& pxInputStream, PX::File& pxOutputStream);
	};
}