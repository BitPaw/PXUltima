#include "PXFont.hpp"

PX::Font::Font()
{
	PXFontConstruct(this);
}

PX::Font::~Font()
{
	PXFontDestruct(this);
}

PX::ActionResult PX::Font::Load(const PX::Text& const filePath)
{
	return (PX::ActionResult)PXFontLoad(this, &filePath);
}

PX::ActionResult PX::Font::Load(PX::File& const pxFile, const FileFormatExtension guessedFormat)
{
	return (PX::ActionResult)PXFontLoadD(this, &pxFile, guessedFormat);;
}