#ifndef PXXMLINCLUDE
#define PXXMLINCLUDE

#include "../PXResource.h"

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

	PXPublic PXXMLSymbol PXAPI PXXMLPeekLine(const char* const text, const PXSize textSize);

	PXPublic PXActionResult PXAPI PXXMLLoadFromFile(PXFile* const inputStream, PXFile* const outputStream);
	PXPublic PXActionResult PXAPI PXXMLSaveToFile(PXFile* const inputStream, PXFile* const outputStream);

#ifdef __cplusplus
}
#endif

#endif