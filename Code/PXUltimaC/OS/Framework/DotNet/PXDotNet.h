#ifndef PXDotNetIncluded
#define PXDotNetIncluded

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

typedef struct PXDotNet_
{
    PXLibrary Library;

    void* MetaHost;
    void* MetaHostPolicy;
    void* Debugging;

    void* RuntimeInfo;
    void* RuntimeHost;

    void* AppDomain;
}
PXDotNet;

PXPublic PXActionResult PXAPI PXDotNetInit(PXDotNet* const pxDotNet);

#endif