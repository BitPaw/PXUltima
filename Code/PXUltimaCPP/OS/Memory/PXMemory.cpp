#include "PXMemory.hpp"

template<typename T>
T* PX::Memory::Allocate(const PXSize size)
{
	return PXMemoryAllocateType(T, size);
}

template<typename T>
T* PX::Memory::AllocateCleared(const PXSize amount)
{
	return PXMemoryAllocateTypeCleared(T, amount);
}

template<typename T>
T* PX::Memory::Reallocate(const T* const adress, const PXSize size)
{
	const PXSize requestedBytes = size * sizeof(T);
	T* adressReallocated = (T*)PXMemoryReallocate(adress, requestedBytes);

	return adressReallocated;
}

bool PX::Memory::Scan(PXMemoryUsage& memoryUsage)
{
	return PXMemoryScan(&memoryUsage);
}

void PX::Memory::Clear(void* const buffer, const PXSize bufferSize)
{
	PXMemoryClear(buffer, bufferSize);
}

void PX::Memory::Set(void* const buffer, const PXByte value, const PXSize bufferSize)
{
	PXMemorySet(buffer, value, bufferSize);
}

int PX::Memory::CompareThreeWay(const void* bufferA, const PXSize bufferASize, const void* bufferB, const PXSize bufferBSize)
{
	return PXMemoryCompareThreeWay(bufferA, bufferASize, bufferB, bufferBSize);
}