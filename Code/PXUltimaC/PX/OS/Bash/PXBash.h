#ifndef PXBashINCLUDE
#define PXBashINCLUDE

#include <PX/Media/PXResource.h>

PXPublic PXActionResult PXAPI PXBashExecute(const PXText* const programPath, const PXBool syncronous);
PXPublic PXActionResult PXAPI PXBashExecuteA
(
    const char* const commandText,
    const PXSize commandTextSize,
    char** const outBuffer,
    const PXSize outBufferSizeMax,
    PXSize* const outBufferSizeCurrent,
    const PXBool syncronous
);

#endif
