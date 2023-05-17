#include "PXFTPClient.h"

#include <Media/FTP/FTP.h>

PXActionResult PXFTPClientConnectToServer(PXFTPClient* const pxFTPClient, const char* ip, unsigned short port)
{
    PXActionResult pxActionResult = PXClientConnectToServer(&pxFTPClient->Client, ip, port, &pxFTPClient->Client.SocketPXClient, PXClientCommunicationThread);



    return pxActionResult;
}