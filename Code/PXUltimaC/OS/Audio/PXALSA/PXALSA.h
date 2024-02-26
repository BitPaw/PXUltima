#ifndef PXALSAInclude
#define PXALSAInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>


#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXALSAInitialize(PXAudio* const pxAudio);

#ifdef __cplusplus
}
#endif

#endif