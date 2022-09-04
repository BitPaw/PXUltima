#ifndef DictionaryDEFINED
#define DictionaryDEFINED

#include <stddef.h>

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

	extern void DictionaryConstruct(Dictionary* const dictionary);
	extern void DictionaryDestruct(Dictionary* const dictionary);

	extern void DictionaryResize(Dictionary* const dictionary, const size_t entrys);
	extern void DictionaryAdd(Dictionary* const dictionary, const void* key, const void* value);
	extern void DictionaryRemove(Dictionary* const dictionary, const void* key);
	extern void DictionaryFind(Dictionary* const dictionary, const void* key, void* value);

#ifdef __cplusplus
}
#endif

#endif