#ifndef PXCPPSpriteFontINCLUDE
#define PXCPPSpriteFontINCLUDE

#include <Media/SpriteFont/PXSpriteFont.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class SpriteFont : public PXSpriteFont
	{
		public:
		SpriteFont();
		~SpriteFont();

		PXSpriteFontCharacter* GetCharacter(const wchar_t character);

		PX::ActionResult Parse(PX::File& file);
	};
}

#endif