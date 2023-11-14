#include "PXBTree.hpp"

template<typename Key, typename Value>
PX::BTree<Key, Value>::BTree(const PXInt8U keyOrder)
{
	PXBTreeInitialize(this, sizeof(Key), sizeof(Value), keyOrder);
}

template<typename Key, typename Value>
PX::BTree<Key, Value>::~BTree()
{
}

template<typename Key, typename Value>
PX::ActionResult PX::BTree<Key, Value>::Resize(const PXSize amount)
{
	return (PX::ActionResult)PXBTreeResize(this, amount);
}

template<typename Key, typename Value>
PX::ActionResult PX::BTree<Key, Value>::Insert(const Key& key, const Value& value)
{
	return (PX::ActionResult)PXBTreeInsert(this, &key, &value);
}

template<typename Key, typename Value>
PX::ActionResult PX::BTree<Key, Value>::Find(const Key& key, Value& value)
{
	return (PX::ActionResult)PXBTreeFind(this, &key, &value);
}