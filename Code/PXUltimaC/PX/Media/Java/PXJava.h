#ifndef PXJavaIncludedd
#define PXJavaIncludedd

#include <PX/Engine/PXResource.h>

PXPrivate void PXAPI PXJavaWriteFile(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaContainerWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaFunctionWrite(PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXJavaDefinitionWrite(PXCompiler* const pxCompiler);

PXPublic PXResult PXAPI PXJavaLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJavaSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
