#include "Dictionary.hpp"

template<typename Key, typename Value>
PX::Dictionary<Key, Value>::Dictionary()
{
	PXDictionaryConstruct(_pxDictionary, sizeof(Key), sizeof(Value));
}

template<typename Key, typename Value>
PX::Dictionary<Key, Value>::~Dictionary()
{
	PXDictionaryDestruct(_pxDictionary);
}

template<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Resize(const PXSize entrys)
{
	PXDictionaryResize(_pxDictionary, entrys);
}

template<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Add(const Key& key, const Value& value)
{
	throw gcnew System::NotImplementedException();
}

template<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Remove(const Key& key)
{
	PXDictionaryRemove(_pxDictionary, &key);
}

template<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Index(const PXSize index, PXDictionaryEntry* const pxDictionaryEntry)
{
	throw gcnew System::NotImplementedException();
}

template<typename Key, typename Value>
PXBool PX::Dictionary<Key, Value>::Find(const Key& const key, Value& const value)
{
	PXDictionaryFind(_pxDictionary, &key, &value);
}