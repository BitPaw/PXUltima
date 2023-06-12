#ifndef PXCPPPXTGAINCLUDE
#define PXCPPPXTGAINCLUDE

#include <Media/TGA/PXTGA.h>
#include <Media/PXImage.hpp>

namespace PX
{
	class TGA : public PXTGA
	{
		public:
		TGA();
		~TGA();

		static PXSize FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

		PX::ActionResult Parse(PX::File& file);
		static PX::ActionResult ParseToImage(PX::Image& image, PX::File& file);

		static PX::ActionResult SerializeFromImage(PX::Image& image, PX::File& file);
	};
}

#endif