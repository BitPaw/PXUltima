#pragma once

#include <Container/LinkedList/PXLinkedList.h>

namespace PX
{
	template<typename T>
	class LinkedList : public PXLinkedList
	{
		public:
		LinkedList();
		~LinkedList();

		void Resize(const PXSize entrys);
		bool Add(const T& element);
		bool Remove(const T& element);
	};
}