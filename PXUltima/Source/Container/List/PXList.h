#ifndef PXListDEFINED
#define PXListDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXList_
	{
		void* Data; // Contains the whole data.
		size_t DataTypeSize; // Size of a single object.
		size_t SizeUsed;  // Size currently used by data.
		size_t SizeAllocated; // Size currently allocated and usable
	}
	PXList;

	PXPublic void PXListConstruct(PXList* const list);
	PXPublic void PXListDestruct(PXList* const list);

	PXPublic void PXListInitialize(PXList* const list, void* const data, const size_t dataTypeSize, const size_t sizeAllocated);

	PXPublic PXBool PXListAdd(PXList* const list, void* const dataElement);

#ifdef __cplusplus
}
#endif

#endif