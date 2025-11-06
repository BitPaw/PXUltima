#pragma once

#ifndef PXDictionaryDEFINED
#define PXDictionaryDEFINED

#include <PX/Media/PXType.h>
#include <PX/Container/List/PXList.h>

typedef enum PXDictionaryValueLocality_
{
    PXDictionaryValueLocalityInvalid,
    PXDictionaryValueLocalityInternalEmbedded,
    PXDictionaryValueLocalityExternalReference
}
PXDictionaryValueLocality;

typedef struct PXDictionary_
{
    PXList List;

    PXSize KeyTypeSize;

    PXDictionaryValueLocality ValueLocality;
}
PXDictionary;

#define PXDictionaryT(typeKey, typeValue) PXDictionary

typedef struct PXDictionaryEntry_
{
    void* Key;
    void* Value;
}
PXDictionaryEntry;

PXPublic void PXAPI PXDictionaryConstruct(PXDictionary PXREF dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality);
PXPublic void PXAPI PXDictionaryDestruct(PXDictionary PXREF dictionary);

PXPublic PXSize PXAPI PXDictionaryValueSize(const PXDictionary PXREF dictionary);

PXPublic void PXAPI PXDictionaryResize(PXDictionary PXREF dictionary, const PXSize entrys);

//-------------------------------------
// Entry
PXPublic PXResult PXAPI PXDictionaryEntryCreate(PXDictionary PXREF pxDictionary, const void* key, void** value);
PXPublic PXResult PXAPI PXDictionaryEntryAdd(PXDictionary PXREF pxDictionary, const void* key, const void* value);
PXPublic PXResult PXAPI PXDictionaryEntryFind(PXDictionary PXREF pxDictionary, const void PXREF key, void* PXREF valueResult);

PXPublic PXBool PXAPI PXDictionaryEntryAddMultible(PXDictionary PXREF dictionary, const void** key, const void** value, const PXSize amount);
PXPublic void PXAPI PXDictionaryRemove(PXDictionary PXREF dictionary, const void* key);
PXPublic PXBool PXAPI PXDictionaryRemoveFound(PXDictionary PXREF dictionary, const void* key, void PXREF value);
//-------------------------------------

// Removed the object and returns it
PXPublic PXBool PXAPI PXDictionaryExtract(PXDictionary PXREF dictionary, const void PXREF key, void PXREF value);
PXPublic void PXAPI PXDictionaryIndex(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry);


#endif
