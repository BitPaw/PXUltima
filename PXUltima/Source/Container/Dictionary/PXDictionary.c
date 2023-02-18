#include "PXDictionary.h"

#include <Memory/PXMemory.h>

void PXDictionaryConstruct(PXDictionary* const dictionary, const PXSize keySize, const PXSize valueSize)
{
	MemoryClear(dictionary, sizeof(PXDictionary));

	dictionary->KeyTypeSize = keySize;
	dictionary->ValueTypeSize = valueSize;
}

void PXDictionaryDestruct(PXDictionary* const dictionary)
{

}

void PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys)
{

}

void PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value)
{
	// Is enough space?


	for (PXSize i = 0; i < dictionary->EntryAmountCurrent; ++i)
	{
		PXDictionaryEntry pxDictionaryEntry;

		PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

		const PXBool isTarget = MemoryCompare(pxDictionaryEntry.Key, dictionary->KeyTypeSize, pxDictionaryEntry.Value, dictionary->ValueTypeSize);

		if (isTarget)
		{
			MemoryCopy(pxDictionaryEntry.Value, dictionary->ValueTypeSize, value, dictionary->ValueTypeSize);
			break;
		}
	}

	// Add space if needed

	// Add to space
}

void PXDictionaryRemove(PXDictionary* const dictionary, const void* key)
{
	// Find

	// Remove
}

void PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry)
{
	const PXSize blockSize = dictionary->KeyTypeSize + dictionary->ValueTypeSize;
	const PXByte* blockStart = ((PXByte*)dictionary->Data) + blockSize * index;

	pxDictionaryEntry->Key = blockStart;
	pxDictionaryEntry->Value = blockStart + sizeof(dictionary->KeyTypeSize);
}

PXBool PXDictionaryFind(PXDictionary* const dictionary, const void* const key, void* const value)
{
	for (PXSize i = 0; i < dictionary->EntryAmountCurrent; ++i)
	{
		PXDictionaryEntry pxDictionaryEntry;

		PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

		const PXBool isTarget = MemoryCompare(pxDictionaryEntry.Key, dictionary->KeyTypeSize, pxDictionaryEntry.Value, dictionary->ValueTypeSize);

		if (isTarget)
		{
			MemoryCopy(pxDictionaryEntry.Value, dictionary->ValueTypeSize, value, dictionary->ValueTypeSize);
			break;
		}
	}
}