#ifndef PXDirectXINCLUDE
#define PXDirectXINCLUDE

#include <OS/System/OSVersion.h>

#define PXDirectXUSE OSDeviceToUse == OSDeviceDestop
#if PXDirectXUSE

#include <Media/PXType.h>

#if OSUnix
// UNIX has no DirectX support
typedef unsigned int  IDirect3D9; // Dummy value
#define MAX_DEVICE_IDENTIFIER_STRING 32
#elif OSWindows
#include <d3d9.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXDirectXContext_
	{
		IDirect3D9* ContextD9;

		char Driver[MAX_DEVICE_IDENTIFIER_STRING];
		char Description[MAX_DEVICE_IDENTIFIER_STRING];
		char DeviceName[32];
	}
	PXDirectXContext;

	PXPublic void PXDirectXContextConstruct(PXDirectXContext* const directXContext);
	PXPublic void PXDirectXContextDestruct(PXDirectXContext* const directXContext);

	PXPublic void PXDirectXContextCreate(PXDirectXContext* const directXContext);

#ifdef __cplusplus
}
#endif

#endif
#endif