#ifndef PXKnowlegeGraphINCLUDE
#define PXKnowlegeGraphINCLUDE

#include <Media/PXResource.h>
#include <Media/PXDocument.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXKnowlegeGraphNode_
	{
		float Position[3];
		float Radius;
		float Value;
		PXDocumentElement DocumentElement;
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

	PXPublic PXActionResult PXAPI PXKnowlegeGraphLoadAndBuild(PXKnowlegeGraph* const pxKnowlegeGraph, PXDocument* const pxDocument, PXImage* const pxImage);

#ifdef __cplusplus
}
#endif

#endif