#pragma once

#ifndef PXKnowlegeGraphIncluded
#define PXKnowlegeGraphIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Media/PXDocument.h>

typedef struct PXKnowlegeGraphNode_
{
    PXF32 Position[3];
    PXF32 Radius;
    PXF32 Value;
    PXCodeDocumentElement DocumentElement;
}
PXKnowlegeGraphNode;

typedef struct PXKnowlegeGraph_
{
    PXI32U NodeMinValue;
    PXI32U NodeMaxValue;
    PXI32U SizeMaxWidth;
    PXI32U SizeMaxHeight;
}
PXKnowlegeGraph;

PXPublic PXResult PXAPI PXKnowlegeGraphNodeInsert(PXKnowlegeGraph PXREF pxKnowlegeGraph, PXKnowlegeGraphNode PXREF pxKnowlegeGraphNode);

PXPublic PXResult PXAPI PXKnowlegeGraphLoadAndBuild(PXKnowlegeGraph PXREF pxKnowlegeGraph, PXCodeDocument PXREF pxDocument, PXTexture PXREF PXTexture);

#endif
