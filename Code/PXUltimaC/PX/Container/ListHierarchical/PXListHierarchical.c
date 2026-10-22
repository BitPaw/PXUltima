#include "PXListHierarchical.h"

#include <PX/Engine/PXResource.h>

PXResult PXAPI PXHierarchicalNodeCreate(PXHierarchicalNode PXREF pxHierarchicalNode, void PXREF ourself)
{
    if(!(pxHierarchicalNode && ourself))
    {
        return PXResultRefusedParameterNull;
    }

    pxHierarchicalNode->Yourself = ourself;

    return PXResultOK;
}

PXResult PXAPI PXHierarchicalNodeSetParent(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF pxHierarchicalNodeParent)
{
    if(!(pxHierarchicalNode && pxHierarchicalNodeParent))
    {
        return PXResultRefusedParameterNull;
    }

    // We want to register a parent, we let the parent register us as a child
    pxHierarchicalNode->Parent = pxHierarchicalNodeParent;

    // Let parent know, i am a child now
    PXHierarchicalNodeAddChild(pxHierarchicalNodeParent, pxHierarchicalNode);

    return PXResultOK;
}

PXResult PXAPI PXHierarchicalNodeAddSibling(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry)
{
    PXHierarchicalNode* nodeCurrent = pxHierarchicalNode;

    // Loop until a sibling is NULL
    for(; nodeCurrent->Sibling; nodeCurrent = nodeCurrent->Sibling);

    // Add new sibling
    nodeCurrent->Sibling = entry;

    return PXResultOK;
}

PXResult PXAPI PXHierarchicalNodeAddChild(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry)
{
    if(!(pxHierarchicalNode && entry))
    {
        return PXResultRefusedParameterNull;
    }

    // If this is the first born, add and exit.
    PXBool hasFirstBorn = pxHierarchicalNode->ChildFirstborn;

    if(!hasFirstBorn)
    {
        pxHierarchicalNode->ChildFirstborn = entry;
        
        return PXResultOK;
    }

    // We have a child, it has a sibling now.
    PXResult pxResult = PXHierarchicalNodeAddSibling(pxHierarchicalNode->ChildFirstborn, entry);

    return pxResult;
}

PXResult PXAPI PXHierarchicalNodeRemoveSibling(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry)
{
    return PXResultOK;
}

PXResult PXAPI PXHierarchicalNodeRemoveChild(PXHierarchicalNode PXREF pxHierarchicalNode, PXHierarchicalNode PXREF entry)
{
    return PXResultOK;
}

PXSize PXAPI PXHierarchicalNodeSiblingAmount(const PXHierarchicalNode PXREF pxHierarchicalNode)
{
    PXHierarchicalNode* nodeCurrent = pxHierarchicalNode->Sibling;

    PXSize counter = 0;

    // Loop until a sibling is NULL
    for(; nodeCurrent; nodeCurrent = nodeCurrent->Sibling)
    {
        ++counter;
    }

    return counter;
}

void* PXAPI PXHierarchicalNodeGetSiblingViaIndex(const PXHierarchicalNode PXREF pxHierarchicalNode, const PXSize index)
{
    PXHierarchicalNode* nodeCurrent = pxHierarchicalNode->Sibling;

    PXSize counter = 0;

    // Loop until a sibling is NULL
    for(; nodeCurrent; nodeCurrent = nodeCurrent->Sibling)
    {
        PXBool isTargetIndex = counter == index;        

        if(isTargetIndex)
        {
            return nodeCurrent->Yourself;
        }

        ++counter;
    }

    return PXNull;
}

PXSize PXAPI PXHierarchicalNodeChildAmount(const PXHierarchicalNode PXREF pxHierarchicalNode)
{
    PXHierarchicalNode* nodeCurrent = pxHierarchicalNode->ChildFirstborn;

    PXSize counter = 0;

    // Loop until a sibling is NULL
    for(; nodeCurrent; nodeCurrent = nodeCurrent->ChildFirstborn)
    {
        ++counter;
    }

    return counter;
}

void* PXAPI PXHierarchicalNodeGetChild(const PXHierarchicalNode PXREF pxHierarchicalNode)
{
    return pxHierarchicalNode->ChildFirstborn->Yourself;
}

void* PXAPI PXHierarchicalNodeGetChildViaIndex(const PXHierarchicalNode PXREF pxHierarchicalNode, const PXSize index)
{
    PXHierarchicalNode* nodeCurrent = pxHierarchicalNode->ChildFirstborn;

    void* target = 0;
    PXSize counter = 0;

    // Loop until a sibling is NULL
    for(; nodeCurrent; nodeCurrent = nodeCurrent->ChildFirstborn)
    {
        if(counter == index)
        {
            target = nodeCurrent->Yourself;
        }

        ++counter;
    }

    return target;
}

void* PXAPI PXHierarchicalNodeGetParent(const PXHierarchicalNode PXREF pxHierarchicalNode)
{
    if(!pxHierarchicalNode)
    {
        return PXNull; // FAIL, Invalid call.
    }

    if(!pxHierarchicalNode->Parent)
    {
        return PXNull; // OK, We dont have a parent.
    }

    void* parent = pxHierarchicalNode->Parent->Yourself;

    PXAssert(parent, "Forgot to init this");

    return parent;
}