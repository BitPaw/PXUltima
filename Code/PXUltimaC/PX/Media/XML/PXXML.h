#ifndef PXXMLIncluded
#define PXXMLIncluded

#include <PX/Engine/PXResource.h>

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

PXPublic PXResult PXAPI PXXMLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXXMLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
