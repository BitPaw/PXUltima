#include "Dictionary.h"

#include <Memory/Memory.h>

void DictionaryConstruct(Dictionary* const dictionary)
{
	MemorySet(dictionary, sizeof(Dictionary), 0);
}

void DictionaryDestruct(Dictionary* const dictionary)
{

}

void DictionaryResize(Dictionary* const dictionary, const size_t entrys)
{

}

void DictionaryAdd(Dictionary* const dictionary, const void* key, const void* value)
{

}

void DictionaryRemove(Dictionary* const dictionary, const void* key)
{

}

void DictionaryFind(Dictionary* const dictionary, const void* key, void* value)
{

}