#ifndef PXBTreeInclude
#define PXBTreeInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Node
    typedef struct PXBTreeNode_
    {
        void* Data;

        void* KeyList; // The list of keys,  
        void* NodeList; // The list of keys, 

        PXInt8U KeyListAmount; // How many keys do we have
        PXInt8U NodeListAmount; // How many keys do we have
    } 
    PXBTreeNode;

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