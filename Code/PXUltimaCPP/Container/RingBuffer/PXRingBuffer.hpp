#pragma once

#include <Container/RingBuffer/PXRingBuffer.h>

namespace PX
{
	template<typename T>
	class RingBuffer : public PXRingBuffer
	{
		public:
		RingBuffer();
		~RingBuffer();

		void Resize(const PXSize entrys);
		bool Add(const T& element);
	};	
}