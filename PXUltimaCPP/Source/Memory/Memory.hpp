#pragma once

#include <Memory/PXMemory.h>

namespace PX
{
    class Memory
    {
        public:
        // Allocates memory on the HEAP.
        // Returns NULL if systems is "out of memory"
        template<typename T>
        static T* Allocate(const size_t size);

        // Changes the size of a given byteblock.knging the poniter.
        // The function will return NULL if the system is "out of memory".
        template<typename T>
        static T* Reallocate(const T* const adress, const size_t size);
    };
}

