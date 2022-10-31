#include "Memory.hpp"

template<typename T>
T* PX::Memory::Allocate(const size_t size)
{
    const size_t requestedBytes = size * sizeof(T);
    T* adress = (T*)MemoryAllocate(requestedBytes);

    return adress;
}

template<typename T>
T* PX::Memory::Reallocate(const T* const adress, const size_t size)
{
    const size_t requestedBytes = size * sizeof(T);
    T* adressReallocated = (T*)MemoryReallocate(adress, requestedBytes);

    return adressReallocated;
}