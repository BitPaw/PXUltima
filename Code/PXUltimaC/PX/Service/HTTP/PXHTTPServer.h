#ifndef PXHTTPServerINCLUDE
#define PXHTTPServerINCLUDE

#include <PX/OS/Error/PXActionResult.h>
#include <PX/Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct PXHTTPServer_
{
    int x;
}
PXHTTPServer;

PXPublic PXActionResult PXHTTPServerStart(PXHTTPServer* const pxHTTPServer);
PXPublic PXActionResult PXHTTPServerStop(PXHTTPServer* const pxHTTPServer);

#ifdef __cplusplus
}
#endif

#endif
