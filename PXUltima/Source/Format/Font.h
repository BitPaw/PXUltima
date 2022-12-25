#ifndef PXFontInclude
#define PXFontInclude

#include <Format/Type.h>
#include <Error/PXActionResult.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef Image_
	typedef struct Image_ Image;
#endif

#ifndef FNT_
	typedef struct PXFNT_ PXFNT;
#endif

	typedef struct PXFont_
	{
		unsigned short CharacterSize;
		unsigned short SizeBetweenCharacters;
		unsigned short SizeBetweenLines;

		PXFNT* FontElement;
		PXSize FontElementSize;
	}
	PXFont;

	//---[ Public-Functions ]----------------------------------------------

	PXPublic void PXFontConstruct(PXFont* const font);
	PXPublic void PXFontDestruct(PXFont* const font);

	PXPublic PXActionResult FontLoadA(PXFont* const font, const PXTextASCII filePath);
	PXPublic PXActionResult FontLoadW(PXFont* const font, const PXTextUNICODE filePath);
	PXPublic PXActionResult FontLoadU(PXFont* const font, const PXTextUTF8 filePath);
	PXPublic PXActionResult FontLoadD(PXFont* const font, PXDataStream* const pxDataStream, const FileFormatExtension guessedFormat);


#ifdef __cplusplus
}
#endif

#endif
