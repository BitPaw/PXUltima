#ifndef PXFontINCLUDE
#define PXFontINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <File/PXDataStream.h>
#include <Media/SpriteFont/PXSpriteFont.h>
#include <Graphic/PXGraphic.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef	struct PXFontPage_
	{
		PXTexture Texture;
	}
	PXFontPage;

	typedef struct PXFont_
	{
		PXInt32U ID;

		PXInt16U CharacterSize;
		PXInt16U SizeBetweenCharacters;
		PXInt16U SizeBetweenLines;

		PXSpriteFont* FontElement;
		PXSize FontElementSize;
	}
	PXFont;

	//---[ Public-Functions ]----------------------------------------------

	PXPublic void PXFontConstruct(PXFont* const pxFont);
	PXPublic void PXFontDestruct(PXFont* const pxFont);

	PXPublic PXActionResult PXFontLoadA(PXFont* const pxFont, const PXTextASCII filePath);
	PXPublic PXActionResult PXFontLoadW(PXFont* const pxFont, const PXTextUNICODE filePath);
	PXPublic PXActionResult PXFontLoadU(PXFont* const pxFont, const PXTextUTF8 filePath);
	PXPublic PXActionResult PXFontLoadD(PXFont* const pxFont, PXDataStream* const pxDataStream, const FileFormatExtension guessedFormat);

#ifdef __cplusplus
}
#endif

#endif