#ifndef PXDiagramINCLUDE
#define PXDiagramINCLUDE

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif
   

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
        size_t OutOfBounceListSize;
    }
    PXBoxPlot;
    
    
    PXPublic void PXBoxPlotConstruct(PXBoxPlot* const boxPlot);    

    PXPublic void PXBoxPlotCalculate(PXBoxPlot* const boxPlot, const float* const inputData, const size_t inputDataSize);

#ifdef __cplusplus
}
#endif

#endif