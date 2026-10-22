#include "PXHilbertCurve.h"

PXVector2F32 PXAPI PXHilbertCurveGet(PXSize step)
{
    PXVector2F32 result = { 0, 0 };

    // We loop through the bit-pairs of a standard 32-bit integer scope
    for(int s = 1; s < 65536; s <<= 1)
    {
        int rx = 1 & (step >> 1);
        int ry = 1 & (step ^ rx);

        // Rotate and flip the coordinate system based on the current quadrant
        if(ry == 0)
        {
            if(rx == 1)
            {
                result.X = s - 1 - result.X;
                result.Y = s - 1 - result.Y;
            }
            // Swap X and Y
            int t = result.X;
            result.X = result.Y;
            result.Y = t;
        }

        result.X += s * rx;
        result.Y += s * ry;
        step >>= 2;
    }

    return result;
}