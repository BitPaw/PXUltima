#pragma once

#include <Container/BTree/PXBTree.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	template<typename Key, typename Value>
	class BTree : public PXBTree
	{
		public:
		BTree(const PXInt8U keyOrder);
		~BTree();

        PX::ActionResult Resize(const PXSize amount);

        PX::ActionResult Insert(const Key& key, const Value& value);
        PX::ActionResult Find(const Key& key, Value& value);
	};	
}