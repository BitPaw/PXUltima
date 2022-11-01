#ifndef PXThreadInclude
#define PXThreadInclude

#include <OS/OSVersion.h>
#include <Format/Type.h>
#include <Error/ActionResult.h>

#if OSUnix
#include <pthread.h>
#include <unistd.h>
#define PXThreadResult void*
#define PXThreadSucessful 0 // Adress
#define PXThreadIDType pthread_t
#define PXThreadIDUnused 0
#elif OSWindows
#include <windows.h>
#define PXThreadResult unsigned long
#define PXThreadSucessful 0
#define PXThreadIDType HANDLE
#define PXThreadIDUnused nullptr
#if OSWindowsXP
typedef struct IUnknown IUnknown;
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef PXThreadResult(*ThreadFunction)(void* data);

	typedef struct PXThread_
	{
		PXThreadIDType ThreadID;
	}
	PXThread;	

	PXPublic ActionResult PXThreadRun(PXThread* const pxThread, const ThreadFunction threadFunction, const void* parameter);

	PXPublic size_t PXThreadCurrentID();
	PXPublic void PXThreadCurrentGet(PXThread* const pxThread);

	PXPublic void PXThreadWaitForFinish(PXThread* const pxThread);

#ifdef __cplusplus
}
#endif

#endif
