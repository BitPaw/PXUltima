#pragma once

#include <Media/JPEG/PXJPEG.h>
#include <Media/PXImage.hpp>

namespace PX
{	
	class JPEG : public PXJPEG
	{
		public:
		JPEG();
		~JPEG();
		
		static PXSize FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

		static PX::ActionResult ParseToImage(PX::Image& image, PX::File& file);
		static PX::ActionResult SerializeFromImage(PX::Image& image, PX::File& file);
	};
}