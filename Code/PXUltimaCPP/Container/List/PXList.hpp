#pragma once

#include <Container/List/PXList.h>

namespace PX
{
	template<typename T>
	class List
	{
		protected:
		PXList _list;

		public:
		List(const PXSize startSize);
		~List();

		void Resize(const PXSize entrys);
		bool Add(const T& element);
		bool Remove(const T& element);
	};
}