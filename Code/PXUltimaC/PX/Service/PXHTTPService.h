#pragma once

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

PXPublic PXResult PXAPI PXHTTPServerStart(PXHTTPServer PXREF pxHTTPServer);
PXPublic PXResult PXAPI PXHTTPServerStop(PXHTTPServer PXREF pxHTTPServer);

#endif