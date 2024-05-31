#ifndef PXJavaIncluded
#define PXJavaIncluded

#include "../PXResource.h"

PXPrivate void PXAPI PXJavaWriteFile(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaContainerWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaFunctionWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaDefinitionWrite(PXCompiler* const pxCompiler);

PXPublic PXActionResult PXAPI PXJavaLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXJavaSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif