#ifndef PXFTPServerINCLUDE
#define PXFTPServerINCLUDE

#include <Media/FTP/PXFTP.h>
#include <OS/Network/PXServer.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXFTPServer_
	{
		PXServer Server;
	}
	PXFTPServer;

#ifdef __cplusplus
}
#endif

#endif