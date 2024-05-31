#ifndef PXCSharpINCLUDE
#define PXCSharpINCLUDE

#include "../PXResource.h"

PXPrivate void PXAPI PXCSharpWriteFile(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXCSharpContainerWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXCSharpFunctionWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXCSharpDefinitionWrite(PXCompiler* const pxCompiler);

PXPublic PXActionResult PXAPI PXCSharpLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXCSharpSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif