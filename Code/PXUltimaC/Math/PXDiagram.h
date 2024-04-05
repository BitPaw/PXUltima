#ifndef PXDiagramINCLUDE
#define PXDiagramINCLUDE

#include <Media/PXResource.h>

typedef struct PXBoxPlot_
{
    int Size;
    float Sum;
    float Minimum;
    float Maximum;
    float Avrage;
    float IQR;

    float HigherWhisker;
    float HigherQuartial;
    float Median;
    float LowerQuartial;
    float LowerWhisker;

    float OutOfBounceList;
    PXSize OutOfBounceListSize;
}
PXBoxPlot;

PXPublic void PXAPI PXBoxPlotConstruct(PXBoxPlot* const boxPlot);

PXPublic void PXAPI PXBoxPlotCalculate(PXBoxPlot* const boxPlot, const float* const inputData, const PXSize inputDataSize);

#endif