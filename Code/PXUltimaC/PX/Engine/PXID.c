#include "PXID.h"

PXID _globalIDCounter = 1000;

PXID PXAPI PXIDGenerate(void)
{
    ++_globalIDCounter;

    return _globalIDCounter;
}