#pragma once

#ifndef PXXingInfoIncluded
#define PXXingInfoIncluded

#include <PX/Engine/PXResource.h>

#define PXXingInfoTOCBufferSize 100u

typedef enum PXXingInfoVersion_
{
    PXXingInfoVersionInvalid,
    PXXingInfoVersionInfo,
    PXXingInfoVersionXing
}
PXXingInfoVersion;

typedef struct PXXingInfo_
{
    PXXingInfoVersion Version;
    PXI32U NumberOfFrames;
    PXI32U SizeInBytes;
    char TOCBuffer[PXXingInfoTOCBufferSize];
    PXI32U VBRScale; // 0-100
}
PXXingInfo;

PXPublic PXResult PXAPI PXXingInfoParse(PXXingInfo PXREF xingInfo, PXFile PXREF PXFile);

#endif
