#ifndef PXDictionaryDEFINED
#define PXDictionaryDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXDictionary_
	{
		PXSize EntryAmountCurrent;
		PXSize EntryAmountMaximal;

		PXSize KeyTypeSize;
		PXSize ValueTypeSize;

		PXSize DataSize;
		void* Data;
	}
	PXDictionary;

	PXPublic void PXDictionaryConstruct(PXDictionary* const dictionary);
	PXPublic void PXDictionaryDestruct(PXDictionary* const dictionary);

	PXPublic void PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys);
	PXPublic void PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value);
	PXPublic void PXDictionaryRemove(PXDictionary* const dictionary, const void* key);
	PXPublic void PXDictionaryFind(PXDictionary* const dictionary, const void* key, void* value);

#ifdef __cplusplus
}
#endif

#endif