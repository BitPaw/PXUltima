#ifndef PXTreeBInclude
#define PXTreeBInclude

#include <PX/Media/PXResource.h>

// Node
typedef struct PXTreeBNode_
{
    void* Data;

    void* KeyList; // The list of keys,  
    void* NodeList; // The list of keys, 

    PXInt8U KeyListAmount; // How many keys do we have
    PXInt8U NodeListAmount; // How many keys do we have
}
PXTreeBNode;

// Multilevel indexing tree.
typedef struct PXTreeB_
{
    //-------------------
    // Data
    //-------------------
    void* ValueDataAdress;
    PXSize ValueAmount;
    PXSize ValueSize;
    //-------------------

    //-------------------
    // Index
    //-------------------
    void* KeyDataAdress;
    PXSize KeyAmount;
    PXSize KeySize;
    PXInt8U KeyOrder;
    //-------------------    
}
PXTreeB;

PXPublic void PXAPI PXTreeBInitialize(PXTreeB* const pxTreeB, const PXSize keySize, const PXSize valueSize, const PXInt8U keyOrder);
PXPublic PXActionResult PXAPI PXTreeBResize(PXTreeB* const pxTreeB, const PXSize amount);
PXPublic PXActionResult PXAPI PXTreeBInsert(PXTreeB* const pxTreeB, const void* const key, const void* const value);
PXPublic PXActionResult PXAPI PXTreeBFindIndexed(PXTreeB* const pxTreeB, const void* const key, void** const value);
PXPublic PXActionResult PXAPI PXTreeBFindUnindexed(PXTreeB* const pxTreeB, const void* const key, void** const value);
PXPublic void PXAPI PXTreeBValueIndex(PXTreeB* const pxTreeB, const PXSize index, void** const key, void** const value);
PXPrivate PXActionResult PXAPI PXTreeBNodeExtractFromAdress(PXTreeB* const pxTreeB, PXTreeBNode* const pxTreeBNode, void* adress);
PXPrivate PXActionResult PXAPI PXTreeBNodeInsert(PXTreeB* const pxTreeB, PXTreeBNode* const pxTreeBNode, const void* const key, const void* const value);

#endif