#pragma once

#include <Media/Bitmap/PXBitmap.h>
#include <Media/PXImage.hpp>

namespace PX
{ 
	class Bitmap : public PXBitmap
	{
		public:
		Bitmap();
		~Bitmap();

		static PXSize FilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel);

		static PX::ActionResult ParseToImage(PX::Image& image, PX::File& file);

		static PX::ActionResult SerializeFromImage(PX::Image& image, PX::File& file);
	};
}