#pragma once

#ifndef PXYAMLIncluded
#define PXYAMLIncluded

#include <PX/Type/PXDOM.h>

PXPublic PXResult PXAPI PXYAMLLoadFromFile(PXDOM PXREF pxDOM, PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXYAMLSaveToFile(PXDOM PXREF pxDOM, PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
