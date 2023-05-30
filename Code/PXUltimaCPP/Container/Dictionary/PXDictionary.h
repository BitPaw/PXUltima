#ifndef PXDictionaryDEFINED
#define PXDictionaryDEFINED

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXDictionaryValueLocality_
	{
		PXDictionaryValueLocalityInvalid,
		PXDictionaryValueLocalityInternalEmbedded,
		PXDictionaryValueLocalityExternalReference
	}
	PXDictionaryValueLocality;

	typedef struct PXDictionary_
	{
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

	PXPublic void PXDictionaryConstruct(PXDictionary* const dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality);
	PXPublic void PXDictionaryDestruct(PXDictionary* const dictionary);

	PXPublic PXSize PXDictionaryValueSize(const PXDictionary* const dictionary);

	PXPublic void PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys);
	PXPublic PXBool PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value);
	PXPublic void PXDictionaryRemove(PXDictionary* const dictionary, const void* key);
	PXPublic PXBool PXDictionaryRemoveFound(PXDictionary* const dictionary, const void* key, void* const value);

	// Removed the object and returns it 
	PXPublic PXBool PXDictionaryExtract(PXDictionary* const dictionary, const void* const key, void* const value);
	PXPublic void PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry);
	PXPublic PXBool PXDictionaryFind(PXDictionary* const dictionary, const void* const key, void** const value);
	PXPublic PXBool PXDictionaryFindEntry(PXDictionary* const dictionary, const void* const key, void** const keyResult);

#ifdef __cplusplus
}
#endif

#endif