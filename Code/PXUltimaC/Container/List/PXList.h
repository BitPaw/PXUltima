#ifndef PXListDEFINED
#define PXListDEFINED

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXList_
	{
		void* Data; // Contains the whole data.
		PXSize DataTypeSize; // Size of a single object.
		PXSize SizeUsed;  // Size currently used by data.
		PXSize SizeAllocated; // Size currently allocated and usable
	}
	PXList;

	PXPublic void PXAPI PXListConstruct(PXList* const list);
	PXPublic void PXAPI PXListDestruct(PXList* const list);

	PXPublic void PXAPI PXListInitialize(PXList* const list, void* const data, const PXSize dataTypeSize, const PXSize sizeAllocated);

	PXPublic PXBool PXAPI PXListAdd(PXList* const list, void* const dataElement);

#ifdef __cplusplus
}
#endif

#endif