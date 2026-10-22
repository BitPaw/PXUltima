#include "PXDictionary.h"

#include <PX/OS/Error/PXResult.hpp>

namespace PX
{
    template<typename Key, typename Value>
    class Dictionary
    {
        private:
        PXDictionary* _pxDictionary;

        public:
        Dictionary(PXDictionaryCreateInfo PXREF pxDictionaryCreateInfo) { PXDictionaryCreate(_pxDictionary); }
        ~Dictionary() { PXDictionaryRelease(_pxDictionary); }

        PXSize ValueSize() { return PXDictionaryValueSize(_pxDictionary); }
        PXSize EntryAmount() { return PXDictionaryEntryAmount(_pxDictionary); }
        PXSize RowSize() { return PXDictionaryRowSize(_pxDictionary); }
        void Resize(const PXSize entrys) { return PXDictionaryResize(_pxDictionary, entrys); }
        PX::Result EntryCreate(const Key* key, Value** value) { return PXDictionaryEntryCreate(_pxDictionary, key, value); }
        PX::Result EntryAdd(const Key* key, const Value* value) { return PXDictionaryEntryAdd(_pxDictionary, key, value); }
        PX::Result EntryAddRange(const Key* key, const Value* value, const PXSize valueSize) { return PXDictionaryEntryAddRange(_pxDictionary, key, value); }
        PX::Result EntryFind(PXDictionaryEntry PXREF pxDictionaryEntry) { return PXDictionaryEntryFind(_pxDictionary, pxDictionaryEntry); }
        PXBool EntryAddMultible(const Key** key, const Value** value, const PXSize amount) { return PXDictionaryEntryAddMultible(_pxDictionary, key, value, amount); }
        void Remove(const Key* key) { return PXDictionaryRemove(_pxDictionary, key); }
        PXBool RemoveFound(const Key* key, Value* value) { return PXDictionaryRemoveFound(_pxDictionary, key, value); }
        PXBool Extract(const Key* key, Value* value) { return PXDictionaryExtract(_pxDictionary, key, value); }
        void IndexUnsafe(const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry) { return PXDictionaryIndexUnsafe(_pxDictionary, index, pxDictionaryEntry); }
        PX::Result Index(const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry) { return PXDictionaryIndex(_pxDictionary, index, pxDictionaryEntry); }
    };
}