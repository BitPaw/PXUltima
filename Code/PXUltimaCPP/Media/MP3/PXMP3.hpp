#pragma once

#include <Media/MP3/PXMP3.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class MP3 : public PXMP3
	{
		public:
		PX::ActionResult Parse(PX::File& file);
	};
}