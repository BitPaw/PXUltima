#include "AVI.h"

#include <Format/RIFF/RIFF.h>

PXActionResult AVIParse(AVI* avi, const void* data, const PXSize dataSize, PXSize* dataRead)
{
    /*
   PXDataStream dataStream;

    {
        const PXActionResult loadingResult = file.MapToVirtualMemory(filePath, MemoryReadOnly);

        if(loadingResult != PXActionSuccessful)
        {
            return loadingResult;
        }
    }

    // RIFF
    {
        RIFF riff;

        // riff.Parse(fileStream);

        if(!riff.Valid)
        {
            return PXActionFailedFormatNotAsExpected;
        }
    }

    unsigned int size = 0;

    file.Read(size, EndianBig);
    */

    return PXActionInvalid;
}