#pragma once

#ifndef PXIDIncluded
#define PXIDIncluded

#include <PX/Media/PXType.h>

// Identifier for all elments, will be incremented and thus uniqe untill we overflow.
// We can make a solution at some point.
typedef PXI64U PXID;

PXPublic PXID PXAPI PXIDGenerate(void);

#endif