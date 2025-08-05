#ifndef PXFTPINCLUDE
#define PXFTPINCLUDE

#include <PX/Media/PXResource.h>
#include <PX/Media/FTP/PXFTP.h>
#include <PX/OS/Network/PXServer.h>

typedef struct PXFTPServer_
{
    int c;
    //PXServer Server;
}
PXFTPServer;

typedef struct PXFTPClient_
{
    int x;
    //PXClient Client;
}
PXFTPClient;

#endif