#pragma once

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

PXPublic void PXAPI PXTreeBInitialize(PXTreeB PXREF pxTreeB, const PXSize keySize, const PXSize valueSize, const PXI8U keyOrder);
PXPublic PXResult PXAPI PXTreeBResize(PXTreeB PXREF pxTreeB, const PXSize amount);
PXPublic PXResult PXAPI PXTreeBInsert(PXTreeB PXREF pxTreeB, const void PXREF key, const void PXREF value);
PXPublic PXResult PXAPI PXTreeBFindIndexed(PXTreeB PXREF pxTreeB, const void PXREF key, void* PXREF value);
PXPublic PXResult PXAPI PXTreeBFindUnindexed(PXTreeB PXREF pxTreeB, const void PXREF key, void* PXREF value);
PXPublic void PXAPI PXTreeBValueIndex(PXTreeB PXREF pxTreeB, const PXSize index, void* PXREF key, void* PXREF value);
PXPrivate PXResult PXAPI PXTreeBNodeExtractFromAdress(PXTreeB PXREF pxTreeB, PXTreeBNode PXREF pxTreeBNode, void* adress);
PXPrivate PXResult PXAPI PXTreeBNodeInsert(PXTreeB PXREF pxTreeB, PXTreeBNode PXREF pxTreeBNode, const void PXREF key, const void PXREF value);

#endif