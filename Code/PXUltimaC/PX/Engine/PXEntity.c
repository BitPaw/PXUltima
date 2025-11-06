#include "PXEntity.h"

#include <PX/Math/PXMath.h>

void PXAPI PXComponentRefCheck(PXComponentRef PXREF pxComponentRef, PXComponentList PXREF pxComponentList)
{
    if(!pxComponentRef) // Is this NULL?
    {
        return; // Illegal call
    }

    if(PXComponentInvalidArrayIndex == pxComponentRef->ArrayIndex)
    {
        return;
    }

    // Is adress in data range?
    const PXBool isInRange = PXListIsIndexValid(&pxComponentList->Lookup.List, pxComponentRef->ArrayIndex);

    if(isInRange)
    {
        // Because adress is valid, we can check now
        PXComponent* pxComponent = PXListItemAtIndexGetT(PXComponent, &pxComponentList->Lookup.List, pxComponentRef->ArrayIndex);

        const PXBool isDirectMatch = pxComponent->ID == pxComponentRef->ExpectedID;

        if(isDirectMatch)
        {
            // Pointer is valid and does not need to be updated
            return; 
        }

        // Pointer is in a valid range but seems to be moved, we need to search..
    }

    // Refered adress is stale! Search to detect valid object
    pxComponentRef->Component = PXNull; // Invalidate pointer now

    for(PXSize i = 0; i < pxComponentList->Lookup.List.EntryAmountUsed; ++i)
    {
        PXComponent* pxComponent = PXListItemAtIndexGetT(PXComponent, pxComponentList, i);
        const PXBool isMatch = pxComponent->ID == pxComponentRef->ExpectedID;

        if(!isMatch)
        {
            continue;
        }

        // We found the object!
        pxComponentRef->Component = pxComponent; // Update reference
        pxComponentRef->ArrayIndex = i; // Update index
    } 

    // Check if we have detected the object at all
    if(!pxComponentRef->Component)
    {
        // We did not detect and com component matching this, array index is stale!
        pxComponentRef->ArrayIndex = PXComponentInvalidArrayIndex;
    }
}

void PXAPI PXComponentManagerAdd(PXComponentManager PXREF pxComponentManager, PXComponentInfo PXREF pxComponentInfo)
{
    ++pxComponentManager->UniqeCounter;

    PXResult pxResult = PXDictionaryEntryCreate
    (
        &pxComponentManager->ComponentLookup,
        &pxComponentManager->UniqeCounter,
        &pxComponentInfo->Component
    );

    // TODO: check pxResult

    // Set ID back to component
    pxComponentInfo->Component->ID = pxComponentManager->UniqeCounter;

    // Update info
    pxComponentInfo->ID = pxComponentManager->UniqeCounter;
}