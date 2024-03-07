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

	PXPrivate PXXMLSymbol PXAPI PXXMLPeekLine(const char* const text, const PXSize textSize);
	PXPrivate void PXAPI PXXMLBlockParse(PXDocument* const pxDocument, PXFile* const pxFile);

	PXPublic PXActionResult PXAPI PXXMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXXMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif