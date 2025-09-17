#ifndef PXTreeBIncluded
#define PXTreeBIncluded

#include <PX/Engine/PXResource.h>

// Node
typedef struct PXTreeBNode_
{
    void* Data;

    void* KeyList; // The list of keys,  
    void* NodeList; // The list of keys, 

    PXI8U KeyListAmount; // How many keys do we have
    PXI8U NodeListAmount; // How many keys do we have
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
    PXI8U KeyOrder;
    //-------------------    
}
PXTreeB;

PXPublic void PXAPI PXTreeBInitialize(PXTreeB* const pxTreeB, const PXSize keySize, const PXSize valueSize, const PXI8U keyOrder);
PXPublic PXResult PXAPI PXTreeBResize(PXTreeB* const pxTreeB, const PXSize amount);
PXPublic PXResult PXAPI PXTreeBInsert(PXTreeB* const pxTreeB, const void* const key, const void* const value);
PXPublic PXResult PXAPI PXTreeBFindIndexed(PXTreeB* const pxTreeB, const void* const key, void** const value);
PXPublic PXResult PXAPI PXTreeBFindUnindexed(PXTreeB* const pxTreeB, const void* const key, void** const value);
PXPublic void PXAPI PXTreeBValueIndex(PXTreeB* const pxTreeB, const PXSize index, void** const key, void** const value);
PXPrivate PXResult PXAPI  PXTreeBNodeExtractFromAdress(PXTreeB* const pxTreeB, PXTreeBNode* const pxTreeBNode, void* adress);
PXPrivate PXResult PXAPI  PXTreeBNodeInsert(PXTreeB* const pxTreeB, PXTreeBNode* const pxTreeBNode, const void* const key, const void* const value);

#endif