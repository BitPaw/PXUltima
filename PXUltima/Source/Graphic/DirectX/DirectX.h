#ifndef DirectXINCLUDE
#define DirectXINCLUDE

#include <Format/Type.h>

#include <d3d9.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct DirectXContext_
	{
		IDirect3D9* ContextD9;

		char Driver[MAX_DEVICE_IDENTIFIER_STRING];
		char Description[MAX_DEVICE_IDENTIFIER_STRING];
		char DeviceName[32];
	}
	DirectXContext;

	PXPublic void DirectXContextConstruct(DirectXContext* const directXContext);
	PXPublic void DirectXContextDestruct(DirectXContext* const directXContext);

	PXPublic void DirectXContextCreate(DirectXContext* const directXContext);

#ifdef __cplusplus
}
#endif

#endif