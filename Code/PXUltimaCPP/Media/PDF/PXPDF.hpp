#pragma once

#include <Media/PDF/PXPDF.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class PDF : public PXPDF
	{
		public:
		PX::ActionResult Compile(PX::File& fileStream);
	};
}