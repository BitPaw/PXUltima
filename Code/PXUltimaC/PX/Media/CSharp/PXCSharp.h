#ifndef PXCSharpIncluded
#define PXCSharpIncluded

#include <PX/Engine/PXResource.h>

PXPrivate void PXAPI PXCSharpWriteFile(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXCSharpContainerWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXCSharpFunctionWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXCSharpDefinitionWrite(PXCompiler* const pxCompiler);

PXPublic PXResult PXAPI PXCSharpLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCSharpSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
