#include "PXFTPClient.h"

#if PXSocketUSE

#include <Media/FTP/PXFTP.h>

PXActionResult PXFTPClientConnectToServer(PXFTPClient* const pxFTPClient, const char* ip, unsigned short port)
{
    PXActionResult pxActionResult = PXClientConnectToServer(&pxFTPClient->Client, ip, port, &pxFTPClient->Client.SocketClient, PXClientCommunicationThread);



    return pxActionResult;
}
#endif