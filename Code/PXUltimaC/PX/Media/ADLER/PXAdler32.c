#include "PXAdler32.h"

PXSize PXAPI PXAdler32Check(const void PXREF data, const PXSize length)
{
    const PXSize adlerModolo = 65521u;
    PXSize a = 1;
    PXSize b = 0;

    for (PXSize index = 0; index < length; ++index)
    {
        a = (a + ((PXAdress)data)[index]) % adlerModolo;
        b = (b + a) % adlerModolo;
    }

    return (b << 16) | a;
}

PXSize PXAPI PXAdler32Create(const PXSize adler, const void PXREF data, PXSize length)
{
    char* dataCursor = (char*)data;

    PXSize s1 = adler & 0xffffu;
    PXSize s2 = (adler >> 16u) & 0xffffu;

    while (length != 0u)
    {
        // at least 5552 sums can be done before the sums overflow, saving a lot of module divisions
        const PXSize amount = length > 5552u ? 5552u : length;

        length -= amount;

        for (PXSize i = 0; i != amount; ++i)
        {
            s1 += (*dataCursor++);
            s2 += s1;
        }

        s1 %= 65521u;
        s2 %= 65521u;
    }

    return (s2 << 16u) | s1;
}
