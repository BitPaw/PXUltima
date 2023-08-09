#include "PXGIF.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

#define PXGIFHeader {'G','I','F'}
#define PXGIFVersionA {'8','7','a'}
#define PXGIFVersionB {'8','9','a'}
#define PXGIFEOF 0x3B

PXSize PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXActionResult PXGIFParseToImage(PXImage* const image, PXFile* const pxFile)
{
    PXGIF gif;

    PXMemoryClear(&gif, sizeof(PXGIF));

    // Check Header
    {
        {
            const char headerTag[3] = PXGIFHeader;
            const PXSize headerTagSize = sizeof(headerTag);
            const PXBool validHeader = PXFileReadAndCompare(pxFile, headerTag, headerTagSize);

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
        PXFileReadI16UE(pxFile, &gif.Width, PXEndianLittle);
        PXFileReadI16UE(pxFile, &gif.Height, PXEndianLittle);

        unsigned char packedFields = 0;

        PXFileReadI8U(pxFile, &packedFields);
        PXFileReadI8U(pxFile, &gif.BackgroundColorIndex);
        PXFileReadI8U(pxFile, &gif.PixelAspectRatio);

        gif.GlobalColorTableSize = packedFields & 0b00000111;
        gif.IsSorted = (packedFields & 0b00001000) >> 3;
        gif.ColorResolution = (packedFields & 0b01110000) >> 4;
        gif.IsGlobalColorTablePresent = (packedFields & 0b10000000) >> 7;

        if (gif.IsGlobalColorTablePresent)
        {
            //---<Image Descriptor>--------------------------------------------

            // 3 x 2^(Size of Global Color Table+1).
            PXSize size = 3 * PXMathPowerOfTwo(gif.GlobalColorTableSize + 1); // ???

            PXGIFImageDescriptor imageDescriptor;

            unsigned char packedFields = 0;

            PXFileReadI8U(pxFile, &imageDescriptor.Separator);
            PXFileReadI16UE(pxFile, &imageDescriptor.LeftPosition, PXEndianLittle);
            PXFileReadI16UE(pxFile, &imageDescriptor.TopPosition, PXEndianLittle);
            PXFileReadI16UE(pxFile, &imageDescriptor.Width, PXEndianLittle);
            PXFileReadI16UE(pxFile, &imageDescriptor.Height, PXEndianLittle);
            PXFileReadI8U(pxFile, &packedFields);

            imageDescriptor.LocalColorTableSize = (packedFields & 0b00000111);
            imageDescriptor.Reserved = (packedFields & 0b00011000) >> 3;
            imageDescriptor.SortFlag = (packedFields & 0b00100000) >> 5;
            imageDescriptor.InterlaceFlag = (packedFields & 0b01000000) >> 6;
            imageDescriptor.LocalColorTableFlag = (packedFields & 0b10000000) >> 7;

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

PXActionResult PXGIFSerializeFromImage(const PXImage* const image, PXFile* const pxFile)
{
    return PXActionInvalid;
}