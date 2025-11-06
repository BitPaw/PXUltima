#include "PXFractal.h"

#include <PX/Math/PXMath.h>

int PXAPI PXFractalGenerateBurningShip(PXFractalIterator PXREF pxFractalIterator)
{
    double zx = 0.0, zy = 0.0;
    int iter = 0;
    while(zx * zx + zy * zy < 4.0 && iter < pxFractalIterator->ItterationMaximal)
    {
        double tmp = zx * zx - zy * zy + pxFractalIterator->cx;
        zy = PXMathAbsoluteD(2.0 * zx * zy) + pxFractalIterator->cy;
        zx = PXMathAbsoluteD(tmp);
        iter++;
    }
    return iter;
}

int PXAPI PXFractalGenerateJulia(PXFractalIterator PXREF pxFractalIterator)
{
    double zx, zy;
    zx = zy = 0;
    int iter = 0;

    while(zx * zx + zy * zy < 4.0 && iter < pxFractalIterator->ItterationMaximal)
    {
        double tmp = zx * zx - zy * zy + pxFractalIterator->cx;
        zy = 2.0 * zx * zy + pxFractalIterator->cy;
        zx = tmp;
        iter++;
    }

    return iter;
}

int PXAPI PXFractalGenerateMandelbrot(PXFractalIterator PXREF pxFractalIterator)
{
    double zx, zy;
    zx = zy = 0;
    int iter = pxFractalIterator->ItterationMaximal;

    while(zx * zx + zy * zy < 4.0 && iter > 0)
    {
        double tmp = zx * zx - zy * zy + pxFractalIterator->cx;
        zy = 2.0 * zx * zy + pxFractalIterator->cy;
        zx = tmp;
        iter--;
    }

    return iter;
}
