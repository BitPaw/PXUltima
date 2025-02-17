#ifndef PXFractalINCLUDE
#define PXFractalINCLUDE

#include <Media/PXResource.h>

typedef struct PXFractalIterator_
{
    double cx;
    double cy;
    int ItterationMaximal;
    float Scale;
}
PXFractalIterator;

PXPublic int PXAPI PXFractalGenerateBurningShip(PXFractalIterator* const pxFractalIterator);
PXPublic int PXAPI PXFractalGenerateJulia(PXFractalIterator* const pxFractalIterator);
PXPublic int PXAPI PXFractalGenerateMandelbrot(PXFractalIterator* const pxFractalIterator);

#endif
