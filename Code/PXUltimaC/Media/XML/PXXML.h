#ifndef PXXMLINCLUDE
#define PXXMLINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXXMLSymbol_
	{
		PXXMLSymbolInvalid,

		PXXMLSymbolUnkown,

		PXXMLSymbolTagOpenBegin, // <xxxx...
		PXXMLSymbolTagOpenFull, // <xxxxx>...

		PXXMLSymbolTagCloseCurrent, // .../>
		PXXMLSymbolTagCloseFull, // </>

		PXXMLSymbolAttribute,

		PXXMLSymbolRawData
	}
	PXXMLSymbol;

	PXPublic PXXMLSymbol PXXMLPeekLine(const char* const text, const PXSize textSize);

	PXPublic PXActionResult PXXMLLoadFromFile(PXFile* const inputStream, PXFile* const outputStream);
	PXPublic PXActionResult PXXMLSaveToFile(PXFile* const inputStream, PXFile* const outputStream);

#ifdef __cplusplus
}
#endif

#endif