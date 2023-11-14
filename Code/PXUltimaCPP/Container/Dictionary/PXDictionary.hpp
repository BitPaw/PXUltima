#pragma once

#include <Container/Dictionary/PXDictionary.h>

namespace PX
{
	template<typename Key, typename Value>
	class Dictionary : public PXDictionary
	{
		public:
		Dictionary();
		~Dictionary();

		void Resize(const PXSize entrys);
		bool Add(const Key& key, const Value& value);
		bool Remove(const Key& key);
		bool RemoveFound(PXDictionary* const dictionary, const void* key, void* const value);
		bool Find(const Key& key, Value& value);	
	};	
}