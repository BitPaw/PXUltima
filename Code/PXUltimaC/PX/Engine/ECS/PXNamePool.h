#pragma once

#ifndef PXNamePoolIncluded
#define PXNamePoolIncluded

#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/PXID.h>
#include <PX/Type/PXText.h>


typedef struct PXNamePool_ PXNamePool;

typedef struct PXNamePoolEntry_
{
    union
    {
        void* Data;
        PXASCII* A;
        PXUNICODE* W;
    };

    PXSize Length;
}
PXNamePoolEntry;

typedef struct PXNamePoolEntryRow_
{
    PXText Name;
    PXID ID;
}
PXNamePoolEntryRow;


PXPublic PXResult PXAPI PXNamePoolInit();

PXPublic PXResult PXAPI PXNamePoolStore(const PXID pxID, const PXText* pxText, PXText* pxTextResult);
PXPublic PXResult PXAPI PXNamePoolGet(const PXID pxID, PXText PXREF pxText);

PXPublic PXID PXAPI PXNamePoolFindIDViaName(const PXText PXREF pxText);

PXPublic PXSize PXAPI PXNamePoolEntryAmount();
PXPublic PXResult PXAPI PXNamePoolEntryGet(PXNamePoolEntryRow PXREF pxNamePoolEntryRow, const PXSize index);
PXPublic PXResult PXAPI PXNamePoolPrintAll();

PXPublic PXResult PXAPI PXNamePoolStoreW(const PXID pxID, const wchar_t* nameAddress, const PXSize nameSize, wchar_t** stored);
PXPublic PXResult PXAPI PXNamePoolStoreA(const PXID pxID, const char* nameAddress, const PXSize nameSize);

#endif