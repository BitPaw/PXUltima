#pragma once

#ifndef PXHilbertCurveIncluded
#define PXHilbertCurveIncluded

#include <PX/Math/PXVector.h>

// Map N to XY
// space filling spiral
PXPublic PXVector2F32 PXAPI PXHilbertCurveGet(PXSize step);

#endif