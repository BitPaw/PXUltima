#include "Dictionary.hpp"

generic<typename Key, typename Value>
PX::Dictionary<Key, Value>::Dictionary()
{
	_pxDictionary = new	PXDictionary();
	PXDictionaryConstruct(_pxDictionary, sizeof(Key), sizeof(Value));
}

generic<typename Key, typename Value>
PX::Dictionary<Key, Value>::~Dictionary()
{
	PXDictionaryDestruct(_pxDictionary);
	delete _pxDictionary;
}

generic<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Resize(const PXSize entrys)
{
	PXDictionaryResize(_pxDictionary, entrys);
}

generic<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Add(const Key key, const Value value)
{
	PXDictionaryAdd(_pxDictionary, &key, &value);
}

generic<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Remove(const Key key)
{
	PXDictionaryRemove(_pxDictionary, &key);
}

generic<typename Key, typename Value>
void PX::Dictionary<Key, Value>::Index(const PXSize index, PXDictionaryEntry* const pxDictionaryEntry)
{
	throw gcnew System::NotImplementedException();
}

generic<typename Key, typename Value>
bool PX::Dictionary<Key, Value>::Find(const Key const key, Value const value)
{
	//PXDictionaryFind(_pxDictionary, &key, &value);
	return false;
}