#pragma once

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

PXPrivate PXXMLSymbol PXAPI PXXMLPeekLine(const char PXREF text, const PXSize textSize);
PXPrivate void PXAPI PXXMLBlockParse(PXCodeDocument PXREF pxDocument, PXCompiler PXREF pxCompiler);

PXPublic PXResult PXAPI PXXMLLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXXMLSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
