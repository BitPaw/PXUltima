#ifndef PXHierarchicalNodeINCLUDE
#define PXHierarchicalNodeINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

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
    PXHierarchicalNode* Parrent;          // First upper parent object
    PXHierarchicalNode* Sibling;          // Like a linked list, contain the next object
    PXHierarchicalNode* ChildFirstborn;   // contain the first child object
}
PXHierarchicalNode;

PXPublic PXActionResult PXAPI PXHierarchicalNodeYourself(PXHierarchicalNode* pxHierarchicalNode, void* const entry);
PXPublic PXActionResult PXAPI PXHierarchicalNodeParent(PXHierarchicalNode* pxHierarchicalNode, PXHierarchicalNode* const pxHierarchicalNodeParent);
PXPublic PXActionResult PXAPI PXHierarchicalNodeSibling(PXHierarchicalNode* pxHierarchicalNode, PXHierarchicalNode* const entry);
PXPublic PXActionResult PXAPI PXHierarchicalNodeChild(PXHierarchicalNode* pxHierarchicalNode, PXHierarchicalNode* const entry);

#endif