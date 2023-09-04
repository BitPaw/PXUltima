#include "PXGIF.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

const char PXGIFHeader[3] = { 'G','I','F' };
#define PXGIFVersionA {'8','7','a'}
#define PXGIFVersionB {'8','9','a'}
#define PXGIFEOF 0x3B

PXSize PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXActionResult PXGIFLoadFromFile(PXVideo* const pxVideo, PXFile* const pxFile)
{
    PXGIF gif;

    PXClear(PXGIF, &gif);

    // Check Header
    {
        {
            const PXBool validHeader = PXFileReadAndCompare(pxFile, PXGIFHeader, sizeof(PXGIFHeader));

            if (!validHeader)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        {
            const char versionA[3] = PXGIFVersionA;
            const char versionB[3] = PXGIFVersionB;
            const char* const versionDataList[2] = { versionA, versionB };
            const PXSize versionSizeList[2] = { sizeof(versionA),  sizeof(versionB) };
            const PXSize versionSizeListSize = sizeof(versionDataList) / sizeof(void*);
            const PXBool validVersion = PXFileReadAndCompareV(pxFile, versionDataList, versionSizeList, versionSizeListSize);

            if (!validVersion)
            {
                return PXActionRefusedInvalidVersion;
            }
        }
    }

    // Logical Screen Descriptor.
    {
        {
            PXInt8U packedFields = 0;

            const PXFileDataElementType pxDataStreamElementList[] =
            {
                {PXDataTypeLEInt16U, &gif.Width},
                {PXDataTypeLEInt16U, &gif.Height},
                {PXDataTypeInt8U, &packedFields},
                {PXDataTypeInt8U, &gif.BackgroundColorIndex},
                {PXDataTypeInt8U, &gif.PixelAspectRatio}
            };
            const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

            PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

            gif.GlobalColorTableSize = packedFields & 0b00000111;
            gif.IsSorted = (packedFields & 0b00001000) >> 3;
            gif.ColorResolution = (packedFields & 0b01110000) >> 4;
            gif.IsGlobalColorTablePresent = (packedFields & 0b10000000) >> 7;
        }  

        if (gif.IsGlobalColorTablePresent)
        {
            //---<Image Descriptor>--------------------------------------------

            // 3 x 2^(Size of Global Color Table+1).
            PXSize size = 3 * PXMathPowerOfTwo(gif.GlobalColorTableSize + 1); // ???

            PXGIFImageDescriptor imageDescriptor;

            {
                unsigned char packedFields = 0;

                const PXFileDataElementType pxDataStreamElementList[] =
                {
                    {PXDataTypeInt8U,       &imageDescriptor.Separator},
                    {PXDataTypeLEInt16U,    &imageDescriptor.LeftPosition},
                    {PXDataTypeLEInt16U,    &imageDescriptor.TopPosition},
                    {PXDataTypeLEInt16U,    &imageDescriptor.Width},
                    {PXDataTypeLEInt16U,    &imageDescriptor.Height},
                    {PXDataTypeInt8U,       &packedFields}
                };
                const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

                PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

                imageDescriptor.LocalColorTableSize = (packedFields & 0b00000111);
                imageDescriptor.Reserved = (packedFields & 0b00011000) >> 3;
                imageDescriptor.SortFlag = (packedFields & 0b00100000) >> 5;
                imageDescriptor.InterlaceFlag = (packedFields & 0b01000000) >> 6;
                imageDescriptor.LocalColorTableFlag = (packedFields & 0b10000000) >> 7;
            }     

            if (imageDescriptor.LocalColorTableFlag)
            {
                //---<Local Color Table>---------------------------------------

                //-------------------------------------------------------------
            }

            //-----------------------------------------------------------------
        }
    }

    //---<Table Based Image Data>--------------------------------------

    //-----------------------------------------------------------------

    return PXActionSuccessful;
}

PXActionResult PXGIFSaveToFile(const PXVideo* const pxVideo, PXFile* const pxFile)
{
    PXFileWriteB(pxFile, PXGIFHeader, sizeof(PXGIFHeader));

    return PXActionRefusedNotImplemented;
}