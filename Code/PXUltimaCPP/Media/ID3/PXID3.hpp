#pragma once

#include <Media/ID3/PXID3.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class ID3 : public PXID3
	{
		public:
		PX::ActionResult Parse(PX::File& file);
	};
}