#include "PXGIF.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXGIFHeader[3] = "GIF";
const char PXGIFVersionA[3] = "87a";
const char PXGIFVersionB[3] = "89a";
#define PXGIFEOF 0x3B

PXSize PXAPI PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXResult PXAPI PXGIFLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileCurrent;

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
            const char* PXREF versionDataList[2] = { PXGIFVersionA, PXGIFVersionB };
            const PXSize versionSizeList[2] = { sizeof(PXGIFVersionA),  sizeof(PXGIFVersionB) };
            const PXSize versionSizeListSize = sizeof(versionDataList) / sizeof(void*);
            const PXBool validVersion = PXFileReadAndCompareV(pxFile, versionDataList, versionSizeList, versionSizeListSize);

            if (!validVersion)
            {
                return PXActionRefusedVersionInvalid;
            }
        }
    }

    // Logical Screen Descriptor.
    {
        {
            PXI8U packedFields = 0;

            const PXTypeEntry pxDataStreamElementList[] =
            {
                {&gif.Width,PXTypeI16ULE},
                {&gif.Height,PXTypeI16ULE},
                {&packedFields,PXTypeI08U},
                {&gif.BackgroundColorIndex, PXTypeI08U},
                {&gif.PixelAspectRatio, PXTypeI08U}
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
                PXI8U packedFields = 0;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&imageDescriptor.Separator,PXTypeI08U},
                    {&imageDescriptor.LeftPosition,PXTypeI16ULE},
                    {&imageDescriptor.TopPosition,PXTypeI16ULE},
                    {&imageDescriptor.Width,PXTypeI16ULE},
                    {&imageDescriptor.Height,PXTypeI16ULE},
                    {&packedFields,PXTypeI08U}
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

    return PXResultOK;
}

PXResult PXAPI PXGIFSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    PXFile PXREF pxFile = pxResourceSaveInfo->FileCurrent;

    PXFileWriteB(pxFile, PXGIFHeader, sizeof(PXGIFHeader));

    return PXActionRefusedNotImplemented;
}
