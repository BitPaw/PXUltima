#ifndef PXWebServerINCLUDE
#define PXWebServerINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXWebServer_
	{
		int x;
	}
	PXWebServer;

	PXPublic ActionResult PXWebServerStart(PXWebServer* const pxWebServer);
	PXPublic ActionResult PXWebServerStop(PXWebServer* const pxWebServer);


#ifdef __cplusplus
}
#endif

#endif