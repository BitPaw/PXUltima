#include "PXDictionary.h"

#include <Memory/PXMemory.h>

void PXDictionaryConstruct(PXDictionary* const dictionary)
{
	MemorySet(dictionary, sizeof(PXDictionary), 0);
}

void PXDictionaryDestruct(PXDictionary* const dictionary)
{

}

void PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys)
{

}

void PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value)
{

}

void PXDictionaryRemove(PXDictionary* const dictionary, const void* key)
{

}

void PXDictionaryFind(PXDictionary* const dictionary, const void* key, void* value)
{

}