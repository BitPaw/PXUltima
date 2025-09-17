#ifndef PXBashIncluded
#define PXBashIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXBashExecute(const PXText* const programPath, const PXBool syncronous);
PXPublic PXResult PXAPI PXBashExecuteA
(
    const char* const commandText,
    const PXSize commandTextSize,
    char** const outBuffer,
    const PXSize outBufferSizeMax,
    PXSize* const outBufferSizeCurrent,
    const PXBool syncronous
);

#endif
