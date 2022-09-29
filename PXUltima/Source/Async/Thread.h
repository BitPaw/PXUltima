#ifndef ThreadInclude
#define ThreadInclude

#include <OS/OSVersion.h>
#include <Format/Type.h>

#if OSUnix
#include <pthread.h>
#include <unistd.h>
#define ThreadResult void*
#define ThreadSucessful 0 // Adress
#define ThreadID pthread_t
#define ThreadIDUnused 0
#elif OSWindows
#include <windows.h>
#define ThreadResult unsigned long
#define ThreadSucessful 0
#define ThreadID HANDLE
#define ThreadIDUnused nullptr
#if OSWindowsXP
typedef struct IUnknown IUnknown;
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef ThreadResult(*ThreadFunction)(void* data);

	CPublic ThreadID ThreadRun(const ThreadFunction threadFunction, const void* parameter);

	CPublic size_t ThreadCurrentID();
	CPublic ThreadID ThreadCurrentGet();

	CPublic void ThreadWaitForFinish(const ThreadID threadID);

#ifdef __cplusplus
}
#endif

#endif
