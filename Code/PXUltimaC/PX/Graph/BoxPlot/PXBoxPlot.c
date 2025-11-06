#include "PXBoxPlot.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

void PXAPI PXBoxPlotConstruct(PXBoxPlot PXREF boxPlot)
{
    PXClear(PXBoxPlot, boxPlot);
}

void PXAPI PXBoxPlotCalculate(PXBoxPlot PXREF boxPlot, const PXF32 PXREF inputData, const PXSize inputDataSize)
{
    const int iqrFactor = 1;
    PXBool hasEvenLengh = 0;
    int medianIndex = -1;
    int lowerQuartialIndex = -1;
    int higherQuartialIndex = -1;

    boxPlot->Size = inputDataSize;
    boxPlot->Sum = -1;
    boxPlot->Minimum = -50000;
    boxPlot->Maximum = 50000;
    boxPlot->Avrage = -1;
    boxPlot->IQR = -1;
    boxPlot->HigherWhisker = -1;
    boxPlot->HigherQuartial = -1;
    boxPlot->Median = -1;
    boxPlot->LowerQuartial = -1;
    boxPlot->LowerWhisker = -1;

    // Calc
    {
        hasEvenLengh = (inputDataSize % 2) == 0;
        medianIndex = inputDataSize / 2;
        lowerQuartialIndex = medianIndex / 2;
        higherQuartialIndex = medianIndex + lowerQuartialIndex;
    }

    //values.Sort();

    for (PXSize i = 0; i < inputDataSize; ++i)
    {
        const PXF32 value = inputData[i];     

        boxPlot->Maximum = PXMathMaximum(boxPlot->Maximum, value);
        boxPlot->Minimum = PXMathMinimum(boxPlot->Minimum, value);

        boxPlot->Sum += value;
    }

    boxPlot->Avrage = boxPlot->Sum / inputDataSize;

    if (hasEvenLengh)
    {
        PXF32 a;
        PXF32 b;

        a = inputData[medianIndex - 1];
        b = inputData[medianIndex];
        boxPlot->Median = (a + b) / 2.0f;

        a = inputData[lowerQuartialIndex - 1];
        b = inputData[lowerQuartialIndex];
        boxPlot->LowerQuartial = (a + b) / 2.0f;

        a = inputData[higherQuartialIndex - 1];
        b = inputData[higherQuartialIndex];
        boxPlot->HigherQuartial = (a + b) / 2.0f;
    }
    else
    {
        boxPlot->Median = inputData[medianIndex];
        boxPlot->LowerQuartial = inputData[lowerQuartialIndex];
        boxPlot->HigherQuartial = inputData[higherQuartialIndex];
    }

    boxPlot->IQR = boxPlot->HigherQuartial - boxPlot->LowerQuartial;

    boxPlot->HigherWhisker = boxPlot->Median + boxPlot->IQR * iqrFactor;
    boxPlot->LowerWhisker = boxPlot->Median - boxPlot->IQR * iqrFactor;

    // OutOfBounce
    {
        unsigned int index = 0;

        for (PXSize i = 0; i < inputDataSize; ++i)
        {
            const double value = inputData[i];
            const PXBool isStrayBullet = value > boxPlot->HigherWhisker || value < boxPlot->LowerWhisker;

            boxPlot->OutOfBounceListSize += isStrayBullet;
        }
    }
}
