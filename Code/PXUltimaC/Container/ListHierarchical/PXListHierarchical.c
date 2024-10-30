#include "PXListHierarchical.h"

PXActionResult PXAPI PXHierarchicalNodeEntryAdd(PXHierarchicalNode* const pxHierarchicalNode, void* const entry, const PXHierarchicalNodeDirection pxHierarchicalNodeDirection)
{
	PXHierarchicalNode* nodeCurrent = pxHierarchicalNode;

	switch(pxHierarchicalNodeDirection)
	{
		case PXHierarchicalNodeDirectionParent:
		{			

			break;
		}
		case PXHierarchicalNodeDirectionSibling:
		{
			// Loop until a sibling is NULL
			for(; nodeCurrent->Sibling; nodeCurrent = (PXHierarchicalNode*)nodeCurrent->Sibling);

			// Add new sibling
			nodeCurrent->Sibling = entry;

			break;
		}

		case PXHierarchicalNodeDirectionChild:
		{
			// Loop until a sibling is NULL
			for(; nodeCurrent->ChildFirstborn; nodeCurrent = (PXHierarchicalNode*)nodeCurrent->ChildFirstborn);

			PXHierarchicalNodeEntryAdd(nodeCurrent, entry, PXHierarchicalNodeDirectionSibling);

			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}


    return PXActionSuccessful;
}

PXActionResult PXAPI PXHierarchicalNodeEntryAmount(PXHierarchicalNode* const pxHierarchicalNode, void* const entry, const PXHierarchicalNodeDirection pxHierarchicalNodeDirection)
{
    return PXActionRefusedNotImplemented;
}