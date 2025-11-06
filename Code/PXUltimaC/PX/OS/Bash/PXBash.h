#pragma once

#ifndef PXBashIncluded
#define PXBashIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXBashExecute(const PXText PXREF programPath, const PXBool syncronous);
PXPublic PXResult PXAPI PXBashExecuteA
(
    const char PXREF commandText,
    const PXSize commandTextSize,
    char* PXREF outBuffer,
    const PXSize outBufferSizeMax,
    PXSize PXREF outBufferSizeCurrent,
    const PXBool syncronous
);

#endif
