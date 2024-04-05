#ifndef PXBashINCLUDE
#define PXBashINCLUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/System/PXOSVersion.h>

PXPublic PXActionResult PXAPI PXBashExecute(const PXText* const programPath, const PXBool syncronous);

#endif