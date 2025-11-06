#include "PXCollatzConjecture.h"

PXSize PXAPI PXCollatzConjectureGenerate16(PXI16U PXREF list, const PXSize amount, const PXI16U start)
{
    list[0] = start;

    PXSize i = 0;
    PXI16U val = 0;

    for(;;)
    {
        const PXBool isOdd = (list[i] % 2) == 0; 

        if(isOdd)
        {
            val = list[i] / 2;
        }
        else 
        {
            // n = n * 3 + 1
            val = list[i] * 3 + 1;
        }

        list[i+1] = val;

        ++i;

        if(val == 1)
        {
            ++i;
            break;
        }
    }

    return i; 
}