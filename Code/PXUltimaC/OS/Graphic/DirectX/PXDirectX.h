#ifndef PXDirectXINCLUDE
#define PXDirectXINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXDirectXUSE PXOSWindowsDestop
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

	typedef struct PXDirectX
	{
		IDirect3D9* ContextD9;

		char Driver[MAX_DEVICE_IDENTIFIER_STRING];
		char Description[MAX_DEVICE_IDENTIFIER_STRING];
		char DeviceName[32];
	}
	PXDirectX;

	PXPublic void PXDirectXContextConstruct(PXDirectX* const pxDirectX);
	PXPublic void PXDirectXContextDestruct(PXDirectX* const pxDirectX);

	PXPublic void PXDirectXContextCreate(PXDirectX* const pxDirectX);

#ifdef __cplusplus
}
#endif

#endif
#endif