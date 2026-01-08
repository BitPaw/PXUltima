#pragma once

#ifndef PXJavaIncludedd
#define PXJavaIncludedd

#include <PX/Engine/PXResource.h>

PXPrivate void PXAPI PXJavaWriteFile(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXJavaContainerWrite(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXJavaFunctionWrite(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXJavaDefinitionWrite(PXCompiler PXREF pxCompiler);

PXPublic PXResult PXAPI PXJavaLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJavaSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
