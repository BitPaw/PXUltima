#include "PXDictionary.hpp"

template<typename Key, typename Value>
PX::Dictionary<Key, Value>::Dictionary()
{
}

template<typename Key, typename Value>
PX::Dictionary<Key, Value>::~Dictionary()
{
}

template<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Resize(const PXSize entrys)
{
}

template<typename Key, typename Value>
bool PX::Dictionary<Key, Value>::Add(const Key& key, const Value& value)
{
	return false;
}

template<typename Key, typename Value>
bool PX::Dictionary<Key, Value>::Remove(const Key& key)
{
	return false;
}

template<typename Key, typename Value>
bool PX::Dictionary<Key, Value>::RemoveFound(PXDictionary* const dictionary, const void* key, void* const value)
{
	return false;
}

template<typename Key, typename Value>
bool PX::Dictionary<Key, Value>::Find(const Key& key, Value& value)
{
	return false;
}