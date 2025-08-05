#ifndef PXHTTPINCLUDE
#define PXHTTPINCLUDE

#include <PX/Media/PXResource.h>

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

PXPublic PXActionResult PXHTTPServerStart(PXHTTPServer* const pxHTTPServer);
PXPublic PXActionResult PXHTTPServerStop(PXHTTPServer* const pxHTTPServer);

#endif