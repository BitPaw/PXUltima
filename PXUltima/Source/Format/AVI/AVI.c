#include "AVI.h"

#include <Format/RIFF/RIFF.h>

ActionResult AVIParse(AVI* avi, const void* data, const PXSize dataSize, PXSize* dataRead)
{
    /*
   DataStream dataStream;

    {
        const ActionResult loadingResult = file.MapToVirtualMemory(filePath, MemoryReadOnly);

        if(loadingResult != ActionSuccessful)
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
            return ResultFormatNotAsExpected;
        }
    }

    unsigned int size = 0;

    file.Read(size, EndianBig);
    */

    return ActionInvalid;
}