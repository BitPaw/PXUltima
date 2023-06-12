#pragma once

#include <Media/AVI/PXAVI.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class AVI : public PXAVI
	{
		public:
		PX::ActionResult Parse(PX::File& file);
	};
}