#include "PXDiagram.h"

#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

void PXBoxPlotConstruct(PXBoxPlot* const boxPlot)
{
    MemoryClear(boxPlot, sizeof(PXBoxPlot));
}

void PXBoxPlotCalculate(PXBoxPlot* const boxPlot, const float* const inputData, const PXSize inputDataSize)
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
        const float value = inputData[i];     

        boxPlot->Maximum = MathMaximum(boxPlot->Maximum, value);
        boxPlot->Minimum = MathMinimum(boxPlot->Minimum, value);

        boxPlot->Sum += value;
    }

    boxPlot->Avrage = boxPlot->Sum / inputDataSize;

    if (hasEvenLengh)
    {
        float a;
        float b;

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
