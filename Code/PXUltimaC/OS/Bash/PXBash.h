#ifndef PXBashINCLUDE
#define PXBashINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXBashUSE OSDeviceToUse == OSDeviceDestop
#if PXBashUSE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <Media/PXText.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXBashExecute(const PXText* const programPath, const PXBool syncronous);

#ifdef __cplusplus
}
#endif

#endif
#endif
