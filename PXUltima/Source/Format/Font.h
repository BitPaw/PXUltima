#ifndef PXFontInclude
#define PXFontInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <File/File.h>

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

	typedef struct PXFont_
	{
		unsigned short CharacterSize;
		unsigned short SizeBetweenCharacters;
		unsigned short SizeBetweenLines;

		FNT* FontElement;
		PXSize FontElementSize;
	}
	PXFont;

	//---[ Public-Functions ]----------------------------------------------

	PXPublic void PXFontConstruct(PXFont* const font);
	PXPublic void PXFontDestruct(PXFont* const font);

	PXPublic ActionResult FontLoadA(PXFont* const font, const char* filePath);
	PXPublic ActionResult FontLoadW(PXFont* const font, const wchar_t* filePath);
	PXPublic ActionResult FontLoadD(PXFont* const font, const FileFormatExtension guessedFormat, const void* data, const PXSize dataSize, const wchar_t* const sourcePath);


#ifdef __cplusplus
}
#endif

#endif
