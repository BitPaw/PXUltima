#ifndef PXFTPClientINCLUDE
#define PXFTPClientINCLUDE

#include <PX/Media/FTP/PXFTP.h>
#include <PX/OS/Network/PXClient.h>

#if PXSocketUSE

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct PXFTPClient_
{
    int x;
    //PXClient Client;
}
PXFTPClient;

PXPublic PXActionResult PXFTPClientConnectToServer(PXFTPClient* const pxFTPClient, const PXText* const ip, const PXInt16U port);


#ifdef __cplusplus
}
#endif

#endif
#endif
