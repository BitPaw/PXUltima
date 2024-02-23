#ifndef PXDirectX10x1INCLUDE
#define PXDirectX10x1INCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXDirectX10x1Initialize(PXDirectX10x1* const pxDirectX10x1);
	PXPublic PXActionResult PXAPI PXDirectX10x1Release(PXDirectX10x1* const pxDirectX10x1);

#ifdef __cplusplus
}
#endif

#endif