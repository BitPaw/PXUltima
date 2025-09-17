#ifndef PXHierarchicalNodeIncluded
#define PXHierarchicalNodeIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>

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

PXPublic PXResult PXAPI PXHierarchicalNodeYourself(PXHierarchicalNode* pxHierarchicalNode, void* const entry);
PXPublic PXResult PXAPI PXHierarchicalNodeParent(PXHierarchicalNode* pxHierarchicalNode, PXHierarchicalNode* const pxHierarchicalNodeParent);
PXPublic PXResult PXAPI PXHierarchicalNodeSibling(PXHierarchicalNode* pxHierarchicalNode, PXHierarchicalNode* const entry);
PXPublic PXResult PXAPI PXHierarchicalNodeChild(PXHierarchicalNode* pxHierarchicalNode, PXHierarchicalNode* const entry);

#endif