#pragma once

#include <Media/LAME/PXLAME.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class LAME : public PXLAME
	{
		public:
		PX::ActionResult Parse(PX::File& file);
	};
}