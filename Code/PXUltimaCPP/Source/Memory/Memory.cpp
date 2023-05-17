#include "Memory.hpp"

template<typename T>
T* PX::Memory::Allocate(const PXSize size)
{
    const PXSize requestedBytes = size * sizeof(T);
    T* adress = (T*)PXMemoryAllocate(requestedBytes);

    return adress;
}

template<typename T>
T* PX::Memory::Reallocate(const T* const adress, const PXSize size)
{
    const PXSize requestedBytes = size * sizeof(T);
    T* adressReallocated = (T*)PXMemoryReallocate(adress, requestedBytes);

    return adressReallocated;
}