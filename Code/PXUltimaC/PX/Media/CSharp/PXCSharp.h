#pragma once

#ifndef PXCSharpIncluded
#define PXCSharpIncluded

#include <PX/Engine/PXResource.h>

PXPrivate void PXAPI PXCSharpWriteFile(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXCSharpContainerWrite(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXCSharpFunctionWrite(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXCSharpDefinitionWrite(PXCompiler PXREF pxCompiler);

PXPublic PXResult PXAPI PXCSharpLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCSharpSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
