#pragma once

#include <Container/Queue/PXQueue.h>

namespace PX
{
	template<typename T>
	class Queue : public PXQueue
	{
		public:
		Queue();
		~Queue();

		void Resize(const PXSize entrys);
		bool Add(const T& element);
		bool Remove(const T& element);
	};
}