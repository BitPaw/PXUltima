#pragma once

#ifndef PXJavaScriptIncludedd
#define PXJavaScriptIncludedd

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXJavaScriptLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJavaScriptSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif