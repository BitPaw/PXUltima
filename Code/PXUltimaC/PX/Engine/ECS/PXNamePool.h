#pragma once

#ifndef PXNamePoolIncluded
#define PXNamePoolIncluded

#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/ID/PXID.h>

typedef struct PXNamePool_ PXNamePool;

PXPublic PXResult PXAPI PXNamePoolInit();

PXPublic PXResult PXAPI PXNamePoolStore(const PXID pxID, const PXText* pxText, PXText* pxTextResult);
PXPublic PXResult PXAPI PXNamePoolStoreW(const PXID pxID, const wchar_t* nameAdress, const PXSize nameSize, wchar_t** stored);
PXPublic PXResult PXAPI PXNamePoolStoreA(const PXID pxID, const char* nameAdress, const PXSize nameSize);

#endif