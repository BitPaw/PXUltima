#ifndef PXFontINCLUDE
#define PXFontINCLUDE

#include <OS/File/PXFile.h>
#include <Media/SpriteFont/PXSpriteFont.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef	struct PXFontPage_
	{
		PXInt32U Texture;
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

	PXPublic PXActionResult PXFontLoad(PXFont* const pxFont, const PXText* const filePath);
	PXPublic PXActionResult PXFontLoadD(PXFont* const pxFont, PXFile* const PXFile, const FileFormatExtension guessedFormat);

#ifdef __cplusplus
}
#endif

#endif