#ifndef PXListDEFINED
#define PXListDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXList_
	{
		unsigned int __Dummy__;
	}
	PXList;

	PXPublic void PXListConstruct(PXList* const list);
	PXPublic void PXListDestruct(PXList* const list);

#ifdef __cplusplus
}
#endif

#endif