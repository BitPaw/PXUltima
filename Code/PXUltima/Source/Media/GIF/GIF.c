#include "GIF.h"

#include <File/PXDataStream.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

#define GIFHeader {'G','I','F'}
#define GIFVersionA {'8','7','a'}
#define GIFVersionB {'8','9','a'}
#define GIFEOF 0x3B

PXSize GIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXActionResult GIFParseToImage(PXImage* const image, PXDataStream* const dataStream)
{
    GIF gif;

    MemoryClear(&gif, sizeof(GIF));

    // Check Header
    {
        {
            const char headerTag[3] = GIFHeader;
            const PXSize headerTagSize = sizeof(headerTag);
            const PXBool validHeader = PXDataStreamReadAndCompare(dataStream, headerTag, headerTagSize);

            if (!validHeader)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        {
            const char versionA[3] = GIFVersionA;
            const char versionB[3] = GIFVersionB;
            const char* const versionDataList[2] = { versionA, versionB };
            const PXSize versionSizeList[2] = { sizeof(versionA),  sizeof(versionB) };
            const PXSize versionSizeListSize = sizeof(versionDataList) / sizeof(void*);
            const PXBool validVersion = PXDataStreamReadAndCompareV(dataStream, versionDataList, versionSizeList, versionSizeListSize);

            if (!validVersion)
            {
                return PXActionRefusedInvalidVersion;
            }
        }
    }

    // Logical Screen Descriptor.
    {
        PXDataStreamReadI16UE(dataStream, &gif.Width, EndianLittle);
        PXDataStreamReadI16UE(dataStream, &gif.Height, EndianLittle);

        unsigned char packedFields = 0;

        PXDataStreamReadI8U(dataStream, &packedFields);
        PXDataStreamReadI8U(dataStream, &gif.BackgroundColorIndex);
        PXDataStreamReadI8U(dataStream, &gif.PixelAspectRatio);

        gif.GlobalColorTableSize = packedFields & 0b00000111;
        gif.IsSorted = (packedFields & 0b00001000) >> 3;
        gif.ColorResolution = (packedFields & 0b01110000) >> 4;
        gif.IsGlobalColorTablePresent = (packedFields & 0b10000000) >> 7;

        if (gif.IsGlobalColorTablePresent)
        {
            //---<Image Descriptor>--------------------------------------------

            // 3 x 2^(Size of Global Color Table+1).
            PXSize size = 3 * PXMathPowerOfTwo(gif.GlobalColorTableSize + 1); // ???

            GIFImageDescriptor imageDescriptor;

            unsigned char packedFields = 0;

            PXDataStreamReadI8U(dataStream, &imageDescriptor.Separator);
            PXDataStreamReadI16UE(dataStream, &imageDescriptor.LeftPosition, EndianLittle);
            PXDataStreamReadI16UE(dataStream, &imageDescriptor.TopPosition, EndianLittle);
            PXDataStreamReadI16UE(dataStream, &imageDescriptor.Width, EndianLittle);
            PXDataStreamReadI16UE(dataStream, &imageDescriptor.Height, EndianLittle);
            PXDataStreamReadI8U(dataStream, &packedFields);

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

PXActionResult GIFSerializeFromImage(const PXImage* const image, PXDataStream* const dataStream)
{
    return PXActionInvalid;
}
