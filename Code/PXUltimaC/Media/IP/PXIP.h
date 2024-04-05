#ifndef PXIPINCLUDE
#define PXIPINCLUDE

#include "../PXResource.h"

typedef enum PXIPVersion_
{
    IPVersionInvalid,
    IPVersionUnknownDomain,
    IPVersion4,
    IPVersion6
}
PXIPVersion;

PXPublic PXIPVersion PXAPI PXIPAnalyseVersion(const char* ip);
PXPublic PXByte PXAPI PXIPIsValid(const char* ipAdress);
/*
Check if the given IPv4 is Valid

Returns the following:
0 - Valid IPv4
1 - Nullpointer as Parameter
2 - Invalid Character (only 0-9 or .)
3 - Octet too large (>255)
4 - Too long (>15)
5 - Too many Octets (more that 4)
*/
PXPublic PXByte PXAPI PXIPv4CheckIfValid(const char* ipv4Adress);
PXPublic PXByte PXAPI PXIPv6CheckIfValid(const char* ipv6Adress);

#endif