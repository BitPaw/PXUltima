#ifndef PXCPPPXTIFFINCLUDE
#define PXCPPPXTIFFINCLUDE

#include <Media/TIFF/PXTIFF.h>
#include <OS/File/PXFile.hpp>
#include <Media/PXImage.hpp>

namespace PX
{
	// Tag Image File Format
	class TIFF : public PXTIFF
	{
		public:
		TIFF();
		~TIFF();

		static PXSize FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

		PXActionResult Parse(PX::File& file);
		static PX::ActionResult ParseToImage(PX::Image& image, PX::File& file);

		static PX::ActionResult SerializeFromImage(PX::Image& image, PX::File& file);
	};
}


#endif