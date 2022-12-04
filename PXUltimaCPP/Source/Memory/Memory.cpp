#include "Memory.hpp"

template<typename T>
T* PX::Memory::Allocate(const PXSize size)
{
    const PXSize requestedBytes = size * sizeof(T);
    T* adress = (T*)MemoryAllocate(requestedBytes);

    return adress;
}

template<typename T>
T* PX::Memory::Reallocate(const T* const adress, const PXSize size)
{
    const PXSize requestedBytes = size * sizeof(T);
    T* adressReallocated = (T*)MemoryReallocate(adress, requestedBytes);

    return adressReallocated;
}