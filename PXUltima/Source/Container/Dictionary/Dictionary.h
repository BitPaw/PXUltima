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

	CPublic void DictionaryConstruct(Dictionary* const dictionary);
	CPublic void DictionaryDestruct(Dictionary* const dictionary);

	CPublic void DictionaryResize(Dictionary* const dictionary, const size_t entrys);
	CPublic void DictionaryAdd(Dictionary* const dictionary, const void* key, const void* value);
	CPublic void DictionaryRemove(Dictionary* const dictionary, const void* key);
	CPublic void DictionaryFind(Dictionary* const dictionary, const void* key, void* value);

#ifdef __cplusplus
}
#endif

#endif