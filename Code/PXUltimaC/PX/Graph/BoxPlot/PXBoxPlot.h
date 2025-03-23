#ifndef PXDiagramINCLUDE
#define PXDiagramINCLUDE

#include <PX/Media/PXResource.h>

typedef struct PXBoxPlot_
{
    int Size;
    PXF32 Sum;
    PXF32 Minimum;
    PXF32 Maximum;
    PXF32 Avrage;
    PXF32 IQR;

    PXF32 HigherWhisker;
    PXF32 HigherQuartial;
    PXF32 Median;
    PXF32 LowerQuartial;
    PXF32 LowerWhisker;

    PXF32 OutOfBounceList;
    PXSize OutOfBounceListSize;
}
PXBoxPlot;

PXPublic void PXAPI PXBoxPlotConstruct(PXBoxPlot* const boxPlot);

PXPublic void PXAPI PXBoxPlotCalculate(PXBoxPlot* const boxPlot, const PXF32* const inputData, const PXSize inputDataSize);

#endif