#pragma once

#ifndef PXIndexIncluded
#define PXIndexIncluded

#include <PX/Type/PXType.h>

typedef struct PXIndexPNT64_
{
    PXI64U Position;
    PXI64U Normal;
    PXI64U Texture;
}
PXIndexPNT64;

typedef struct PXIndexPNT32_
{
    PXI32U Position;
    PXI32U Normal;
    PXI32U Texture;
}
PXIndexPNT32;

typedef struct PXIndexPNT16_
{
    PXI32U Position;
    PXI32U Normal;
    PXI32U Texture;
}
PXIndexPNT16;

typedef struct PXIndexPNT8_
{
    PXI8U Position;
    PXI8U Normal;
    PXI8U Texture;
}
PXIndexPNT8;

typedef struct PXIndexTriangle8_
{
    PXI8U A;
    PXI8U B;
    PXI8U C;
}
PXIndexTriangle8;

typedef struct PXIndexTriangle16_
{
    PXI16U A;
    PXI16U B;
    PXI16U C;
}
PXIndexTriangle16;

typedef struct PXIndexTriangle32_
{
    PXI32U A;
    PXI32U B;
    PXI32U C;
}
PXIndexTriangle32;

typedef struct PXIndexTriangle64_
{
    PXI64U A;
    PXI64U B;
    PXI64U C;
}
PXIndexTriangle64;

#endif