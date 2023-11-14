#pragma once

#include <Container/BinaryTree/PXBinaryTree.h>

namespace PX
{
	template<typename Key, typename Value>
	class BinaryTree : public PXBinaryTree
	{
		public:
		BinaryTree();
		~BinaryTree();

		void Resize(const PXSize entrys);
		bool Add(const Key& key, const Value& value);
		bool Remove(const Key& key);
		bool Find(const Key& key, Value& value);
	};
}