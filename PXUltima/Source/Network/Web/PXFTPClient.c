#include "PXFTPClient.h"

#include <Format/FTP/FTP.h>

PXActionResult PXFTPClientConnectToServer(PXFTPClient* const pxFTPClient, const char* ip, unsigned short port)
{
    PXActionResult pxActionResult = PXClientConnectToServer(&pxFTPClient->Client, ip, port, &pxFTPClient->Client.SocketPXClient, CommunicationFunctionAsync);



    return pxActionResult;
}