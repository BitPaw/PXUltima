#ifndef PXJavaIncluded
#define PXJavaIncluded

#include "../PXResource.h"

PXPrivate void PXAPI PXJavaWriteFile(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaContainerWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaFunctionWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaDefinitionWrite(PXCompiler* const pxCompiler);

PXPublic PXActionResult PXAPI PXJavaLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXJavaSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif