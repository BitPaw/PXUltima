#ifndef PXLinkedListDEFINED
#define PXLinkedListDEFINED

#include <Format/Type.h>

#define PXLinkedListNodeNotUsed (void*)-1
#define PXLinkedListNodeNoNext (void*)0

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXLinkedListNodeFixed_ PXLinkedListNodeFixed;

	typedef struct PXLinkedListNodeFixed_
	{		
		void* BlockData;
		PXLinkedListNodeFixed* NodeNext; // 0=No Next 0xFFF=InvalidData 
	}
	PXLinkedListNodeFixed;

	typedef struct PXLinkedListNodeDynamic_
	{
		void* BlockData;
		size_t BlockSize;
	}
	PXLinkedListNodeDynamic;
	
	typedef struct PXLinkedListFixed_
	{
		void* Data;
		size_t NodeSize; // if size is -1, we will store the data pointer itself
		size_t NodeListSizeCurrent;
		size_t NodeListSizeMaximal;
	}
	PXLinkedListFixed;

	PXPublic void PXLinkedListFixedNodeConstruct(PXLinkedListFixed* const linkedListFixed);
	PXPublic void PXLinkedListFixedNodeDestruct(PXLinkedListFixed* const linkedListFixed);

	PXPublic void PXLinkedListFixedNodeSet(PXLinkedListFixed* const linkedListFixed, void* const data, const size_t dataSize, const size_t nodeSize);

	PXPublic PXBool PXLinkedListFixedNodeAt(PXLinkedListFixed* const linkedListFixed, PXLinkedListNodeFixed* const pxLinkedListNodeFixed, const size_t index);
	PXPublic PXBool PXLinkedListFixedNodeNext(PXLinkedListFixed* const linkedListFixed, PXLinkedListNodeFixed* const pxLinkedListNodeFixed);

	//PXPublic PXBool PXLinkedListFixedNodeFetch(PXLinkedListFixed* const linkedListFixed, void* const element);

	PXPublic PXBool PXLinkedListFixedNodeAdd(PXLinkedListFixed* const linkedListFixed, void* const element);
	PXPublic PXBool PXLinkedListFixedNodeRemoveAt(PXLinkedListFixed* const linkedListFixed, const size_t index);
#ifdef __cplusplus
}
#endif

#endif