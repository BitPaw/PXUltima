#ifndef PXDictionaryDEFINED
#define PXDictionaryDEFINED

#include <Media/PXType.h>

typedef enum PXDictionaryValueLocality_
{
	PXDictionaryValueLocalityInvalid,
	PXDictionaryValueLocalityInternalEmbedded,
	PXDictionaryValueLocalityExternalReference
}
PXDictionaryValueLocality;

typedef struct PXDictionary_
{
	PXSize EntryAmountGrowth; // How much to grow if an allocation if space is insufficent

	PXSize EntryAmountCurrent;
	PXSize EntryAmountMaximal;

	//---<Need to be predefined>------------
	PXSize KeyTypeSize;
	PXSize ValueTypeSize;
	//--------------------------------------

	PXSize DataSize;
	void* Data;

	PXDictionaryValueLocality ValueLocality;
}
PXDictionary;

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
PXPublic PXBool PXAPI PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value);
PXPublic PXBool PXAPI PXDictionaryAddMultible(PXDictionary* const dictionary, const void** key, const void** value, const PXSize amount);
PXPublic void PXAPI PXDictionaryRemove(PXDictionary* const dictionary, const void* key);
PXPublic PXBool PXAPI PXDictionaryRemoveFound(PXDictionary* const dictionary, const void* key, void* const value);

// Removed the object and returns it 
PXPublic PXBool PXAPI PXDictionaryExtract(PXDictionary* const dictionary, const void* const key, void* const value);
PXPublic void PXAPI PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry);
PXPublic PXBool PXAPI PXDictionaryFind(PXDictionary* const dictionary, const void* const key, void** const value);

PXPublic PXBool PXAPI PXDictionaryFindEntry(PXDictionary* const dictionary, const void* const key, void** const valueResult);

#endif