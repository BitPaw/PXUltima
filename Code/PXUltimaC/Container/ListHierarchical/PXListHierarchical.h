#ifndef PXHierarchicalNodeINCLUDE
#define PXHierarchicalNodeINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

typedef enum PXHierarchicalNodeDirection_
{
    PXHierarchicalNodeDirectionInvalid,
    PXHierarchicalNodeDirectionParent,
    PXHierarchicalNodeDirectionSibling,
    PXHierarchicalNodeDirectionChild
}
PXHierarchicalNodeDirection;

typedef struct PXHierarchicalNode_
{
    void* Parrent;          // First upper parent object
    void* Sibling;          // Like a linked list, contain the next object
    void* ChildFirstborn;   // contain the first child object
}
PXHierarchicalNode;

PXPublic PXActionResult PXAPI PXHierarchicalNodeEntryAdd(PXHierarchicalNode* const pxHierarchicalNode, void* const entry, const PXHierarchicalNodeDirection pxHierarchicalNodeDirection);
PXPublic PXActionResult PXAPI PXHierarchicalNodeEntryAmount(PXHierarchicalNode* const pxHierarchicalNode, void* const entry, const PXHierarchicalNodeDirection pxHierarchicalNodeDirection);

#endif