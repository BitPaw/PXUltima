#ifndef PXFontInclude
#define PXFontInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef Image_
	typedef struct Image_ Image;
#endif

#ifndef FNT_
	typedef struct FNT_ FNT;
#endif

	typedef enum FontFileFormat_
	{
		FontFileFormatUnkown,
		FontFileFormatFNT,
		FontFileFormatOFT,
		FontFileFormatTTF
	}
	FontFileFormat;

	typedef struct PXFont_
	{
		unsigned short CharacterSize;
		unsigned short SizeBetweenCharacters;
		unsigned short SizeBetweenLines;

		FNT* FontElement;
		size_t FontElementSize;
	}
	PXFont;

	//---[ Public-Functions ]----------------------------------------------

	CPublic void PXFontConstruct(PXFont* const font);
	CPublic void PXFontDestruct(PXFont* const font);

	CPublic FontFileFormat FontGuessFormat(const wchar_t* filePath);

	CPublic ActionResult FontLoadA(PXFont* const font, const char* filePath);
	CPublic ActionResult FontLoadW(PXFont* const font, const wchar_t* filePath);
	CPublic ActionResult FontLoadD(PXFont* const font, const FontFileFormat guessedFormat, const void* data, const size_t dataSize, const wchar_t* const sourcePath);


#ifdef __cplusplus
}
#endif

#endif
