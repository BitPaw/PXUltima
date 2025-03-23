#ifndef PXFTPServerINCLUDE
#define PXFTPServerINCLUDE

#include <PX/Media/FTP/PXFTP.h>
#include <PX/OS/Network/PXServer.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct PXFTPServer_
{
    int c;
    //PXServer Server;
}
PXFTPServer;

#ifdef __cplusplus
}
#endif

#endif
