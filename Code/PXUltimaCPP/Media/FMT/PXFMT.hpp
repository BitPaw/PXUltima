#pragma once

#include <Media/FMT/PXFMT.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	// Format header
	class FMT : public PXFMT
	{
		public:
		FMT();

		PX::ActionResult Parse(PX::File& file, const PXEndian endian);
		PX::ActionResult Serialize(PX::File& file, const PXEndian endian);
	};
}
