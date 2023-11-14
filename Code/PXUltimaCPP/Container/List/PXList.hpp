#pragma once

#include <Container/List/PXList.h>

namespace PX
{
	template<typename T>
	class List : public PXList
	{
		public:
		List();
		~List();

		void Resize(const PXSize entrys);
		bool Add(const T& element);
		bool Remove(const T& element);
	};
}