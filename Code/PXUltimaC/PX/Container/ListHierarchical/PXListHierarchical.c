#include "PXListHierarchical.h"

PXResult PXAPI PXHierarchicalNodeYourself(PXHierarchicalNode PXREF pxHierarchicalNode, void PXREF entry)
{
	pxHierarchicalNode->Yourself = entry;

	return PXActionSuccessful;
}

PXResult PXAPI PXHierarchicalNodeParent(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF pxHierarchicalNodeParent)
{
	if(!(pxHierarchicalNode && pxHierarchicalNodeParent))
	{
		return PXActionRefusedArgumentNull;
	}

	// We want to register a parent, we let the parent register us as a child
	pxHierarchicalNode->Parrent = pxHierarchicalNodeParent;

	// Let parent know, i am a child now
	PXHierarchicalNodeChild(pxHierarchicalNodeParent, pxHierarchicalNode);

	return PXActionSuccessful;
}

PXResult PXAPI PXHierarchicalNodeSibling(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry)
{
	PXHierarchicalNode* nodeCurrent = pxHierarchicalNode;

	// Loop until a sibling is NULL
	for(; nodeCurrent->Sibling; nodeCurrent = nodeCurrent->Sibling);

	// Add new sibling
	nodeCurrent->Sibling = entry;

	return PXActionSuccessful;
}

PXResult PXAPI PXHierarchicalNodeChild(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry)
{
	PXHierarchicalNode* nodeCurrent = pxHierarchicalNode;

	// Loop until a sibling is NULL
	for(; nodeCurrent->ChildFirstborn; nodeCurrent = nodeCurrent->ChildFirstborn);

	PXHierarchicalNodeSibling(nodeCurrent, entry);

	return PXActionSuccessful;
}