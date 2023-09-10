#include "PXGIF.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

const char PXGIFHeader[3] = { 'G','I','F' };
const char PXGIFVersionA[3] = { '8','7','a' };
const char PXGIFVersionB[3] = {'8','9','a'};
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
            const char** const versionDataList[2] = { PXGIFVersionA, PXGIFVersionB };
            const PXSize versionSizeList[2] = { sizeof(PXGIFVersionA),  sizeof(PXGIFVersionB) };
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
                {&gif.Width,PXDataTypeInt16ULE},
                {&gif.Height,PXDataTypeInt16ULE},
                {&packedFields,PXDataTypeInt08U},
                {&gif.BackgroundColorIndex, PXDataTypeInt08U},
                {&gif.PixelAspectRatio, PXDataTypeInt08U}
            };

            PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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
                PXInt8U packedFields = 0;

                const PXFileDataElementType pxDataStreamElementList[] =
                {
                    {&imageDescriptor.Separator,PXDataTypeInt08U},
                    {&imageDescriptor.LeftPosition,PXDataTypeInt16ULE},
                    {&imageDescriptor.TopPosition,PXDataTypeInt16ULE},
                    {&imageDescriptor.Width,PXDataTypeInt16ULE},
                    {&imageDescriptor.Height,PXDataTypeInt16ULE},
                    {&packedFields,PXDataTypeInt08U}
                };

                PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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