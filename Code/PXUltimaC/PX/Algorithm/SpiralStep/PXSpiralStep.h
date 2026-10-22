#pragma once

#ifndef PXSpiralStepIncluded
#define PXSpiralStepIncluded

#include <PX/Math/PXVector.h>

// Map N to XY
// space filling spiral
PXPublic PXVector2F32 PXAPI PXSpiralStepGet(const PXSize step);

#endif