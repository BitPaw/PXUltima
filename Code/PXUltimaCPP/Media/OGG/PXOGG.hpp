#pragma once

#include <Media/OGG/PXOGG.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class OGG : public PXOGG
	{
		public:
		PX::ActionResult Parse(PX::File& file);
	};
}