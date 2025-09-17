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

PXPublic void PXAPI PXDictionaryConstruct(PXDictionary* const dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality);
PXPublic void PXAPI PXDictionaryDestruct(PXDictionary* const dictionary);

PXPublic PXSize PXAPI PXDictionaryValueSize(const PXDictionary* const dictionary);

PXPublic void PXAPI PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys);

//-------------------------------------
// Entry
PXPublic PXResult PXAPI PXDictionaryEntryCreate(PXDictionary* const dictionary, const void* key, const void** value);
PXPublic PXResult PXAPI PXDictionaryEntryAdd(PXDictionary* const dictionary, const void* key, const void* value);
PXPublic PXResult PXAPI PXDictionaryEntryFind(PXDictionary* const dictionary, const void* const key, void** const valueResult);

PXPublic PXBool PXAPI PXDictionaryEntryAddMultible(PXDictionary* const dictionary, const void** key, const void** value, const PXSize amount);
PXPublic void PXAPI PXDictionaryRemove(PXDictionary* const dictionary, const void* key);
PXPublic PXBool PXAPI PXDictionaryRemoveFound(PXDictionary* const dictionary, const void* key, void* const value);
//-------------------------------------

// Removed the object and returns it
PXPublic PXBool PXAPI PXDictionaryExtract(PXDictionary* const dictionary, const void* const key, void* const value);
PXPublic void PXAPI PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry);


#endif
