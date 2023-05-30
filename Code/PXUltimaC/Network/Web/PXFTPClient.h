#ifndef PXFTPClientINCLUDE
#define PXFTPClientINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/Network/PXClient.h>

#if PXSocketUSE

#define FTPDefaultPort 21

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXFTPClient_
	{
		PXClient Client;
	}
	PXFTPClient;

	PXPublic PXActionResult PXFTPClientConnectToServer(PXFTPClient* const pxFTPClient, const char* ip, unsigned short port);	


#ifdef __cplusplus
}
#endif

#endif
#endif