#ifndef PXThreadInclude
#define PXThreadInclude

#include <OS/OSVersion.h>
#include <Format/Type.h>
#include <Error/ActionResult.h>

#if OSUnix
#include <pthread.h>
#include <unistd.h>
typedef void* PXThreadResult;
typedef pthread_t PXThreadIDType;
#define PXThreadSucessful 0 // Adress
#define PXThreadIDUnused 0
#elif OSWindows
#include <windows.h>
typedef unsigned long PXThreadResult;
typedef HANDLE PXThreadIDType;
#define PXThreadSucessful 0
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

	PXPublic PXSize PXThreadCurrentID();
	PXPublic void PXThreadCurrentGet(PXThread* const pxThread);

	PXPublic void PXThreadWaitForFinish(PXThread* const pxThread);

#ifdef __cplusplus
}
#endif

#endif
