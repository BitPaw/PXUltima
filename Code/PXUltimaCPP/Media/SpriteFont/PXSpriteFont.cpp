#include "PXSpriteFont.hpp"

PX::SpriteFont::SpriteFont()
{
	PXSpriteFontConstruct(this);
}

PX::SpriteFont::~SpriteFont()
{
	PXSpriteFontDestruct(this);
}

PXSpriteFontCharacter* PX::SpriteFont::GetCharacter(const wchar_t character)
{
	return PXSpriteFontGetCharacter(this, character);
}

PX::ActionResult PX::SpriteFont::Parse(PX::File& file)
{
	return (PX::ActionResult)PXSpriteFontParse(this, &file);
}