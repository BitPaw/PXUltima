#ifndef PXKnowlegeGraphINCLUDE
#define PXKnowlegeGraphINCLUDE

#include <PX/Media/PXResource.h>
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
    PXInt32U NodeMinValue;
    PXInt32U NodeMaxValue;
    PXInt32U SizeMaxWidth;
    PXInt32U SizeMaxHeight;
}
PXKnowlegeGraph;

PXPublic PXActionResult PXAPI PXKnowlegeGraphNodeInsert(PXKnowlegeGraph* const pxKnowlegeGraph, PXKnowlegeGraphNode* const pxKnowlegeGraphNode);

PXPublic PXActionResult PXAPI PXKnowlegeGraphLoadAndBuild(PXKnowlegeGraph* const pxKnowlegeGraph, PXCodeDocument* const pxDocument, PXImage* const pxImage);

#endif
