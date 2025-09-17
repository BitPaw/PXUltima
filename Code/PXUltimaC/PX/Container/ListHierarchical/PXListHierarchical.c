#include "PXListHierarchical.h"

PXResult PXAPI  PXHierarchicalNodeYourself(PXHierarchicalNode* const pxHierarchicalNode, void* const entry)
{
	pxHierarchicalNode->Yourself = entry;

	return PXActionSuccessful;
}

PXResult PXAPI  PXHierarchicalNodeParent(PXHierarchicalNode* const pxHierarchicalNode, PXHierarchicalNode* const pxHierarchicalNodeParent)
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

PXResult PXAPI  PXHierarchicalNodeSibling(PXHierarchicalNode* const pxHierarchicalNode, PXHierarchicalNode* const entry)
{
	PXHierarchicalNode* nodeCurrent = pxHierarchicalNode;

	// Loop until a sibling is NULL
	for(; nodeCurrent->Sibling; nodeCurrent = nodeCurrent->Sibling);

	// Add new sibling
	nodeCurrent->Sibling = entry;

	return PXActionSuccessful;
}

PXResult PXAPI  PXHierarchicalNodeChild(PXHierarchicalNode* const pxHierarchicalNode, PXHierarchicalNode* const entry)
{
	PXHierarchicalNode* nodeCurrent = pxHierarchicalNode;

	// Loop until a sibling is NULL
	for(; nodeCurrent->ChildFirstborn; nodeCurrent = nodeCurrent->ChildFirstborn);

	PXHierarchicalNodeSibling(nodeCurrent, entry);

	return PXActionSuccessful;
}