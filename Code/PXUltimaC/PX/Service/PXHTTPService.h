#ifndef PXHTTPIncluded
#define PXHTTPIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXHTTPServer_
{
    int x;
}
PXHTTPServer;

typedef struct PXHTTPClient_
{
    int x;
}
PXHTTPClient;

PXPublic PXResult PXAPI PXHTTPServerStart(PXHTTPServer* const pxHTTPServer);
PXPublic PXResult PXAPI PXHTTPServerStop(PXHTTPServer* const pxHTTPServer);

#endif