#include "PXGIF.h"

#include <Math/PXMath.h>

const char PXGIFHeader[3] = { 'G','I','F' };
const char PXGIFVersionA[3] = {'8','7','a' };
const char PXGIFVersionB[3] = {'8','9','a'};
#define PXGIFEOF 0x3B

PXSize PXAPI PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXActionResult PXAPI PXGIFLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXGIF gif;

    PXClear(PXGIF, &gif);

    // Check Header
    {
        {
            const PXBool validHeader = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXGIFHeader, sizeof(PXGIFHeader));

            if (!validHeader)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        {
            const char** const versionDataList[2] = { PXGIFVersionA, PXGIFVersionB };
            const PXSize versionSizeList[2] = { sizeof(PXGIFVersionA),  sizeof(PXGIFVersionB) };
            const PXSize versionSizeListSize = sizeof(versionDataList) / sizeof(void*);
            const PXBool validVersion = PXFileReadAndCompareV(pxResourceLoadInfo->FileReference, versionDataList, versionSizeList, versionSizeListSize);

            if (!validVersion)
            {
                return PXActionRefusedVersionInvalid;
            }
        }
    }

    // Logical Screen Descriptor.
    {
        {
            PXInt8U packedFields = 0;

            const PXTypeEntry pxDataStreamElementList[] =
            {
                {&gif.Width,PXTypeInt16ULE},
                {&gif.Height,PXTypeInt16ULE},
                {&packedFields,PXTypeInt08U},
                {&gif.BackgroundColorIndex, PXTypeInt08U},
                {&gif.PixelAspectRatio, PXTypeInt08U}
            };

            PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&imageDescriptor.Separator,PXTypeInt08U},
                    {&imageDescriptor.LeftPosition,PXTypeInt16ULE},
                    {&imageDescriptor.TopPosition,PXTypeInt16ULE},
                    {&imageDescriptor.Width,PXTypeInt16ULE},
                    {&imageDescriptor.Height,PXTypeInt16ULE},
                    {&packedFields,PXTypeInt08U}
                };

                PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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

PXActionResult PXAPI PXGIFSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXFileWriteB(pxResourceSaveInfo->FileReference, PXGIFHeader, sizeof(PXGIFHeader));

    return PXActionRefusedNotImplemented;
}
