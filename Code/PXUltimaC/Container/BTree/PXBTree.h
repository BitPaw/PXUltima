#ifndef PXBTreeInclude
#define PXBTreeInclude

#include <Media/PXResource.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Default order
#define ORDER 3

    typedef struct record
    {
        int value;
    } record;

    // Node
    typedef struct PXBTreeNode_
    {
        void* Data;

        PXInt8U KeyListAmount; // How many keys do we have
        void* KeyList; // The list of keys, 

        PXInt8U NodeListAmount; // How many keys do we have
        void* NodeList; // The list of keys, 

        void** pointers;
        int* keys;
        struct PXBTreeNode_* parent;
        bool is_leaf;
        int num_keys;
        struct PXBTreeNode_* next;
    } 
    PXBTreeNode;

    bool verbose_output = false;

    // Multilevel indexing tree.
    typedef struct PXBTree_
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
    PXBTree;

    PXPublic void PXAPI PXBTreeInitialize(PXBTree* const pxBTree, const PXSize keySize, const PXSize valueSize, const PXInt8U keyOrder);

    PXPublic PXActionResult PXAPI PXBTreeResize(PXBTree* const pxBTree, const PXSize amount);

    PXPublic PXActionResult PXAPI PXBTreeInsert(PXBTree* const pxBTree, const void* const key, const void* const value);

    PXPublic PXActionResult PXAPI PXBTreeFindIndexed(PXBTree* const pxBTree, const void* const key, void** const value);
    PXPublic PXActionResult PXAPI PXBTreeFindUnindexed(PXBTree* const pxBTree, const void* const key, void** const value);

    PXPublic void PXAPI PXBTreeValueIndex(PXBTree* const pxBTree, const PXSize index, void** const key, void** const value);

    PXPrivate PXActionResult PXAPI PXBTreeNodeExtractFromAdress(PXBTree* const pxBTree, PXBTreeNode* const pxBTreeNode, void* adress);
    PXPrivate PXActionResult PXAPI PXBTreeNodeInsert(PXBTree* const pxBTree, PXBTreeNode* const pxBTreeNode, const void* const key, const void* const value);


#ifdef __cplusplus
}
#endif

#endif