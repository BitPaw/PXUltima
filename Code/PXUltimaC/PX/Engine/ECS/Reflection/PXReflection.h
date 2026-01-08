#pragma once

#ifndef PXReflectionIncluded
#define PXReflectionIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>

typedef struct PXField_
{
    const PXSize FieldSize;
    const PXSize Allignment;
}
PXField;

typedef struct PXType_
{
    PXTextFormat TextFormat;

    union
    {
        char* NameA;
        wchar_t* NameW;
    };

    PXSize NameLength;
    PXSize TypeSize;
    PXSize Allignment;

    PXField* FieldList;
    PXSize FieldListAmount;
}
PXType;

#endif