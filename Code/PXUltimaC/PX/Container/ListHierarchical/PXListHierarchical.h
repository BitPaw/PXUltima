#pragma once

#ifndef PXHierarchicalNodeIncluded
#define PXHierarchicalNodeIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>

typedef enum PXHierarchicalNodeDirection_
{
    PXHierarchicalNodeDirectionInvalid,
    PXHierarchicalNodeDirectionSelf,
    PXHierarchicalNodeDirectionParent,
    PXHierarchicalNodeDirectionSibling,
    PXHierarchicalNodeDirectionChild
}
PXHierarchicalNodeDirection;

typedef struct PXHierarchicalNode_ PXHierarchicalNode;

typedef struct PXHierarchicalNode_
{
    void* Yourself;         // Payload of your own data
    PXHierarchicalNode* Parent;          // First upper parent object
    PXHierarchicalNode* Sibling;          // Linked list, contain the next object in the same depth.
    PXHierarchicalNode* ChildFirstborn;   // Contain the first child object
}
PXHierarchicalNode;

PXPublic PXResult PXAPI PXHierarchicalNodeCreate(PXHierarchicalNode PXREF pxHierarchicalNode, void PXREF ourself);

PXPublic PXResult PXAPI PXHierarchicalNodeSetParent(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF pxHierarchicalNodeParent);

PXPublic PXResult PXAPI PXHierarchicalNodeAddSibling(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry);
PXPublic PXResult PXAPI PXHierarchicalNodeAddChild(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry);

PXPublic PXResult PXAPI PXHierarchicalNodeRemoveSibling(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry);
PXPublic PXResult PXAPI PXHierarchicalNodeRemoveChild(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry);


PXPublic PXSize PXAPI PXHierarchicalNodeSiblingAmount(const PXHierarchicalNode PXREF pxHierarchicalNode);
PXPublic void* PXAPI PXHierarchicalNodeGetSiblingViaIndex(const PXHierarchicalNode PXREF pxHierarchicalNode, const PXSize index);


PXPublic PXSize PXAPI PXHierarchicalNodeChildAmount(const PXHierarchicalNode PXREF pxHierarchicalNode);
PXPublic void* PXAPI PXHierarchicalNodeGetChild(const PXHierarchicalNode PXREF pxHierarchicalNode);
PXPublic void* PXAPI PXHierarchicalNodeGetChildViaIndex(const PXHierarchicalNode PXREF pxHierarchicalNode, const PXSize index);

PXPublic void* PXAPI PXHierarchicalNodeGetParent(const PXHierarchicalNode PXREF pxHierarchicalNode);

#endif