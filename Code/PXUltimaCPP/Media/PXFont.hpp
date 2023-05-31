#ifndef PXCPPFontINCLUDE
#define PXCPPFontINCLUDE

#include <Media/PXFont.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class Font : public PXFont
	{
		public:
		Font();
		~Font();

		PX::ActionResult Load(const PX::Text& const filePath);
		PX::ActionResult Load(PX::File& const PXFile, const FileFormatExtension guessedFormat);
	};
}

#endif