#pragma once

#include <Media/GIF/PXGIF.h>
#include <Media/PXImage.hpp>

namespace PX
{
	class GIF : public PXGIF
	{
		static PXSize FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

		static PX::ActionResult ParseToImage(PX::Image& image, PX::File& file);

		static PX::ActionResult SerializeFromImage(PX::Image& image, PX::File& file);
	};
}