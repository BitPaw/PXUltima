#pragma once

#include <Format/Type.h>
#include <Container/Dictionary/PXDictionary.h>

namespace PX
{
	template <typename Key, typename Value>
	public ref class Dictionary
	{
		private:
		PXDictionary* _pxDictionary;

		public:
		Dictionary();
		~Dictionary();

		void Resize(const PXSize entrys);
		void Add(const Key& key, const Value& value);
		void Remove(const Key& key);
		void Index(const PXSize index, PXDictionaryEntry* const pxDictionaryEntry);
		PXBool Find(const Key& const key, Value& const value);
	};	
}