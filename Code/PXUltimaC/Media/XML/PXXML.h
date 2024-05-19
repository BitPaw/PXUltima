#ifndef PXXMLINCLUDE
#define PXXMLINCLUDE

#include "../PXResource.h"

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
PXPrivate void PXAPI PXXMLBlockParse(PXCodeDocument* const pxDocument, PXCompiler* const pxCompiler);

PXPublic PXActionResult PXAPI PXXMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXXMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif