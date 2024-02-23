#ifndef PXDirectX10INCLUDE
#define PXDirectX10INCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXDirectX10Initialize(PXDirectX10* const pxDirectX);
	PXPublic PXActionResult PXAPI PXDirectX10Release(PXDirectX10* const pxDirectX);

#ifdef __cplusplus
}
#endif

#endif