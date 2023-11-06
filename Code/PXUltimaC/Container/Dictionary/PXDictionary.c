#include "PXDictionary.h"

#include <OS/Memory/PXMemory.h>

void PXDictionaryConstruct(PXDictionary* const dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality)
{
	PXMemoryClear(dictionary, sizeof(PXDictionary));

	dictionary->KeyTypeSize = keySize;
	dictionary->ValueTypeSize = valueSize;
	dictionary->ValueLocality = pxDictionaryValueLocality;
}

void PXDictionaryDestruct(PXDictionary* const dictionary)
{
	PXDeleteList(PXByte, dictionary->Data, dictionary->DataSize);
}

PXSize PXDictionaryValueSize(const PXDictionary* const dictionary)
{
	switch (dictionary->ValueLocality)
	{
		default:
		case PXDictionaryValueLocalityInvalid:
			return 0;

		case PXDictionaryValueLocalityInternalEmbedded:
			return dictionary->ValueTypeSize;

		case PXDictionaryValueLocalityExternalReference:
			return sizeof(void*);
	}
}

void PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys)
{
	const PXSize oldPositionOffset = dictionary->EntryAmountMaximal * (dictionary->KeyTypeSize + dictionary->ValueTypeSize);
	const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);

	const PXBool resizeSucessful = PXResizeList(PXByte, &dictionary->Data, &dictionary->DataSize, entrys);

	if (resizeSucessful)
	{
		dictionary->EntryAmountMaximal = entrys;
	}

	const PXSize newPositionOffset = dictionary->DataSize;
	const PXSize difference = newPositionOffset - oldPositionOffset;
	void* newDataPoint = (char*)dictionary->Data + oldPositionOffset;

	PXMemorySet(newDataPoint, 0xFF, difference);
}

PXBool PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value)
{
	const PXBool hasEnoughSpace = (dictionary->EntryAmountCurrent + 1) < dictionary->EntryAmountMaximal;

	if (!hasEnoughSpace)
	{
		PXDictionaryResize(dictionary, dictionary->EntryAmountCurrent + 10);
	}

	for (PXSize i = 0; i < dictionary->EntryAmountMaximal; ++i)
	{
		PXDictionaryEntry pxDictionaryEntry;

		PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

		const PXBool isEmptyKeyField = PXMemoryCompareToByte(pxDictionaryEntry.Key, dictionary->KeyTypeSize, 0xFF);

		if (isEmptyKeyField)
		{
			PXMemoryCopy(key, dictionary->KeyTypeSize, pxDictionaryEntry.Key, dictionary->KeyTypeSize);

			switch (dictionary->ValueLocality)
			{
				default:
				case PXDictionaryValueLocalityInvalid:
				{
					return 0; // Illegal call
				}
				case PXDictionaryValueLocalityInternalEmbedded:
				{
					PXMemoryCopy(value, dictionary->ValueTypeSize, pxDictionaryEntry.Value, dictionary->ValueTypeSize);
					break;
				}
				case PXDictionaryValueLocalityExternalReference:
				{
					PXMemoryCopy(&value, sizeof(void*), pxDictionaryEntry.Value, sizeof(void*));
					break;
				}
			}

			++dictionary->EntryAmountCurrent;;

			return PXTrue;
		}
	}

	return PXFalse;
}

void PXDictionaryRemove(PXDictionary* const dictionary, const void* key)
{
	// Find

	// Remove
}

PXBool PXDictionaryRemoveFound(PXDictionary* const dictionary, const void* key, void* const value)
{
	PXMemorySet(key, 0xFF, dictionary->KeyTypeSize);
	PXMemorySet(value, 0xFF, dictionary->ValueTypeSize);

	--dictionary->EntryAmountCurrent;
}

PXBool PXDictionaryExtract(PXDictionary* const dictionary, const void* const key, void* const value)
{
	const void* valteAdress = 0;

	const PXBool found = PXDictionaryFindEntry(dictionary, key, &valteAdress);

	if (!found)
	{
		PXMemoryClear(value, dictionary->ValueTypeSize);
		return PXFalse;
	}

	const void* data = (char*)valteAdress + dictionary->KeyTypeSize;

	switch (dictionary->ValueLocality)
	{
		default:
		case PXDictionaryValueLocalityInvalid:
		{
			return 0; // Illegal call
		}
		case PXDictionaryValueLocalityInternalEmbedded:
		{
			PXMemoryCopy(data, dictionary->ValueTypeSize, value, dictionary->ValueTypeSize);
			break;
		}
		case PXDictionaryValueLocalityExternalReference:
		{
			PXMemoryCopy(data, sizeof(void*), value, sizeof(void*));
			break;
		}
	}

	PXDictionaryRemoveFound(dictionary, valteAdress, data);

	return PXTrue;
}

void PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry)
{
	const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);
	const PXSize blockSize = dictionary->KeyTypeSize + dataBlockSize;
	const PXByte* blockStart = ((PXByte*)dictionary->Data) + blockSize * index;

	pxDictionaryEntry->Key = blockStart;
	pxDictionaryEntry->Value = blockStart + dictionary->KeyTypeSize;
}

PXBool PXDictionaryFind(PXDictionary* const dictionary, const void* const key, void** const resultKey)
{
	const PXBool found = 0;

	if (!found)
	{
		return PXFalse;
	}	

	return PXTrue;
}

PXBool PXDictionaryFindEntry(PXDictionary* const dictionary, const void* const key, void** const valueResult)
{
	for (PXSize i = 0; i < dictionary->EntryAmountCurrent; ++i)
	{
		PXDictionaryEntry pxDictionaryEntry;

		PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

		const PXBool isTarget = PXMemoryCompare(pxDictionaryEntry.Key, dictionary->KeyTypeSize, key, dictionary->KeyTypeSize);		

		if (isTarget)
		{
			*valueResult = *(void**)pxDictionaryEntry.Value;
			return PXTrue;
		}
	}

	return PXFalse;
}