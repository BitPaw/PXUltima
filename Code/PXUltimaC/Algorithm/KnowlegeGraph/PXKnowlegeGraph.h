#ifndef PXKnowlegeGraphINCLUDE
#define PXKnowlegeGraphINCLUDE

#include <Media/PXResource.h>
#include <Media/PXDocument.h>

typedef struct PXKnowlegeGraphNode_
{
	float Position[3];
	float Radius;
	float Value;
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