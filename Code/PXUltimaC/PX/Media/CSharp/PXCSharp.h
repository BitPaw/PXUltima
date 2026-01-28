#pragma once

#ifndef PXCSharpIncluded
#define PXCSharpIncluded

#include <PX/Engine/PXResource.h>

PXPrivate void PXAPI PXCSharpWriteFile(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXCSharpContainerWrite(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXCSharpFunctionWrite(PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXCSharpDefinitionWrite(PXCompiler PXREF pxCompiler);

PXPublic PXResult PXAPI PXCSharpLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCSharpSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
