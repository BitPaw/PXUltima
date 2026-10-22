#include "PXSpiralStep.h"

#include <PX/Math/PXMath.h>

PXVector2F32 PXAPI PXSpiralStepGet(const PXSize step)
{
    PXVector2F32 position = {0,0};

    if(step == 0)
        return position;

    // 1. Find the root/layer of the current step
    PXI32S root = PXMathRootSquareI32U(step); 
    PXI32S square = root * root;
    PXI32S delta = step - square;

#if 1
    // 1. Determine which concentric layer (ring) the step falls into.
    // Ring k has a max index of (2k+1)^2 - 1
    PXSize k = (PXSize)PXMathCeilingF((PXMathRootSquareF32((double)step + 1.0) - 1.0) / 2.0);

    // 2. Find the index relative to the start of the current ring layer
    PXSize prevMax = (2 * k - 1) * (2 * k - 1) - 1;
    PXSize ringIndex = step - (prevMax + 1);

    // 3. Each square ring has 4 sides of length (2 * k)
    PXSize sideLength = 2 * k;
    PXSize side = ringIndex / sideLength;
    PXSize offset = ringIndex % sideLength;

    // 4. Calculate coordinates based on which side of the square ring we are on
    switch(side)
    {
        case 0: // Right vertical edge moving upwards
            position.X = (PXF32)k;
            position.Y = (PXF32)(-(long long)k + 1 + (long long)offset);
            break;

        case 1: // Top horizontal edge moving leftwards
            position.X = (PXF32)((long long)k - 1 - (long long)offset);
            position.Y = (PXF32)k;
            break;

        case 2: // Left vertical edge moving downwards
            position.X = (PXF32)(-(long long)k);
            position.Y = (PXF32)((long long)k - 1 - (long long)offset);
            break;

        case 3: // Bottom horizontal edge moving rightwards
        default:
            position.X = (PXF32)(-(long long)k + 1 + (long long)offset);
            position.Y = (PXF32)(-(long long)k);
            break;
    }

#else
    // 2. Map coordinates based on whether the root layer is even or odd
    if(root % 2 == 0)
    {
        if(delta <= root)
        {
            position.X = -root / 2 + delta;
            position.Y = root / 2;
        }
        else
        {
            position.X = root / 2;
            position.Y = root / 2 - (delta - root);
        }
    }
    else
    {
        if(delta <= root)
        {
            position.X = root / 2 - delta;
            position.Y = -root / 2;
        }
        else
        {
            position.X = -root / 2;
            position.Y = -root / 2 + (delta - root);
        }
    }
#endif

    return position;
}