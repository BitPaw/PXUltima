#ifndef PXCPPPNGINCLUDE
#define PXCPPPNGINCLUDE

#include <Media/PNG/PXPNG.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXImage.hpp>

namespace PX
{
	class PNG : public PXPNG
	{
		public:
		PNG();
		~PNG();

		PXInt8U BitsPerPixel();

		static PXSize FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

		PX::ActionResult ParseToImage(PX::Image& image, PX::File& dataStream);
		PX::ActionResult SerializeFromImage(PX::Image& image, PX::File& dataStream);
	};
}

#endif