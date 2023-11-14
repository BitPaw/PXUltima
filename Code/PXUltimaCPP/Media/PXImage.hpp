#ifndef PXCPPImageINCLUDE
#define PXCPPImageINCLUDE

#include <Media/PXImage.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class Image : public PXImage
	{
		public:
		Image();
		~Image();

		bool Resize(const PXColorFormat format, const PXSize width, const PXSize height);
		void FlipHorizontal();
		void FlipVertical();
		void RemoveColor(unsigned char red, unsigned char green, unsigned char blue);
		void FillColorRGBA8(const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);

		void* DataPoint( const PXSize x, const PXSize y);

		PXSize PixelPosition
		(
			const PXSize x,
			const PXSize y
		);
		void PixelSetRGB8
		(
			const PXSize x,
			const PXSize y,
			const unsigned char red,
			const unsigned char green,
			const unsigned char blue
		);
	};
}

#endif