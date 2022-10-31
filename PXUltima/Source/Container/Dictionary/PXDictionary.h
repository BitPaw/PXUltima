#ifndef PXDictionaryDEFINED
#define PXDictionaryDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXDictionary_
	{
		size_t EntryAmountCurrent;
		size_t EntryAmountMaximal;

		size_t KeyTypeSize;
		size_t ValueTypeSize;

		size_t DataSize;
		void* Data;
	}
	PXDictionary;

	PXPublic void PXDictionaryConstruct(PXDictionary* const dictionary);
	PXPublic void PXDictionaryDestruct(PXDictionary* const dictionary);

	PXPublic void PXDictionaryResize(PXDictionary* const dictionary, const size_t entrys);
	PXPublic void PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value);
	PXPublic void PXDictionaryRemove(PXDictionary* const dictionary, const void* key);
	PXPublic void PXDictionaryFind(PXDictionary* const dictionary, const void* key, void* value);

#ifdef __cplusplus
}
#endif

#endif