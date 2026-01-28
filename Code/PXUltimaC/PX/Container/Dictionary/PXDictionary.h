#pragma once

#ifndef PXDictionaryDEFINED
#define PXDictionaryDEFINED

#include <PX/Media/PXType.h>
#include <PX/Container/List/PXList.h>
#include <PX/Engine/ECS/PXECS.h>

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

typedef struct PXDictionaryCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize KeySize;
    PXSize ValueSize;
    PXDictionaryValueLocality ValueLocality;
}
PXDictionaryCreateInfo;

PXPublic PXResult PXAPI PXDictionaryRegisterToECS();
PXPublic PXResult PXAPI PXDictionaryCreate(PXDictionary** pxDictionaryREF, PXDictionaryCreateInfo PXREF pxDictionaryCreateInfo);
PXPublic PXResult PXAPI PXDictionaryRelease(PXDictionary PXREF pxDictionary);

PXPublic PXSize PXAPI PXDictionaryValueSize(const PXDictionary PXREF dictionary);
PXPublic PXSize PXAPI PXDictionaryEntryAmount(const PXDictionary PXREF dictionary);

PXPublic void PXAPI PXDictionaryResize(PXDictionary PXREF dictionary, const PXSize entrys);

//-------------------------------------
// Entry
PXPublic PXResult PXAPI PXDictionaryEntryCreate(PXDictionary PXREF pxDictionary, const void* key, void** value);
PXPublic PXResult PXAPI PXDictionaryEntryAdd(PXDictionary PXREF pxDictionary, const void* key, const void* value);
PXPublic PXResult PXAPI PXDictionaryEntryAddRange(PXDictionary PXREF pxDictionary, const void* key, const void* value, const PXSize valueSize);
PXPublic PXResult PXAPI PXDictionaryEntryFind(PXDictionary PXREF pxDictionary, const void PXREF key, void* PXREF valueResult);

PXPublic PXBool PXAPI PXDictionaryEntryAddMultible(PXDictionary PXREF dictionary, const void** key, const void** value, const PXSize amount);
PXPublic void PXAPI PXDictionaryRemove(PXDictionary PXREF dictionary, const void* key);
PXPublic PXBool PXAPI PXDictionaryRemoveFound(PXDictionary PXREF dictionary, const void* key, void PXREF value);
//-------------------------------------

// Removed the object and returns it
PXPublic PXBool PXAPI PXDictionaryExtract(PXDictionary PXREF dictionary, const void PXREF key, void PXREF value);

PXPublic void PXAPI PXDictionaryIndexUnsafe(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry);
PXPublic PXResult PXAPI PXDictionaryIndex(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry);


#endif
