#ifndef DictionaryDEFINED
#define DictionaryDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct Dictionary_
	{
		size_t EntryAmountCurrent;
		size_t EntryAmountMaximal;

		size_t KeyTypeSize;
		size_t ValueTypeSize;

		size_t DataSize;
		void* Data;
	}
	Dictionary;

	PXPublic void DictionaryConstruct(Dictionary* const dictionary);
	PXPublic void DictionaryDestruct(Dictionary* const dictionary);

	PXPublic void DictionaryResize(Dictionary* const dictionary, const size_t entrys);
	PXPublic void DictionaryAdd(Dictionary* const dictionary, const void* key, const void* value);
	PXPublic void DictionaryRemove(Dictionary* const dictionary, const void* key);
	PXPublic void DictionaryFind(Dictionary* const dictionary, const void* key, void* value);

#ifdef __cplusplus
}
#endif

#endif