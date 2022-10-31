#include "GIF.h"

#include <File/DataStream.h>
#include <Memory/PXMemory.h>
#include <Math/PXMath.h>

#define GIFHeader {'G','I','F'}
#define GIFVersionA {'8','7','a'}
#define GIFVersionB {'8','9','a'}
#define GIFEOF 0x3B

size_t GIFFilePredictSize(const size_t width, const size_t height, const size_t bbp)
{
    return 0;
}

ActionResult GIFLoad(GIF* gif, const void* data, const size_t dataSize, size_t* dataRead)
{
    DataStream dataStream;

    MemorySet(gif, sizeof(GIF), 0);
    DataStreamConstruct(&dataStream);
    DataStreamFromExternal(&dataStream, data, dataSize);
    *dataRead = 0;

    // Check Header
    {
        const char versionA[3] = GIFVersionA;
        const char versionB[3] = GIFVersionB;
        const char headerTag[3] = GIFHeader;
        char version[3] = { '#','#','#' };

        const unsigned char validHeader = DataStreamReadAndCompare(&dataStream, headerTag, sizeof(headerTag));

        DataStreamReadP(&dataStream, version, sizeof(version));

        const unsigned char validVersion =
            version[0] == versionA[0] &&
            version[1] == versionA[1] &&
            version[2] == versionA[2]
            ||
            version[0] == versionB[0] &&
            version[1] == versionB[1] &&
            version[2] == versionB[2];

        if(!validHeader)
        {
            return ActionInvalidHeaderSignature;
        }

        if(!validVersion)
        {
            return ActionInvalidVersion;
        }
    }

    // Logical Screen Descriptor.
    {
        DataStreamReadSU(&dataStream, &gif->Width, EndianLittle);
        DataStreamReadSU(&dataStream, &gif->Height, EndianLittle);

        unsigned char packedFields = 0;

        DataStreamReadCU(&dataStream, &packedFields);
        DataStreamReadCU(&dataStream, &gif->BackgroundColorIndex);
        DataStreamReadCU(&dataStream, &gif->PixelAspectRatio);

        gif->GlobalColorTableSize = packedFields & 0b00000111;
        gif->IsSorted = (packedFields & 0b00001000) >> 3;
        gif->ColorResolution = (packedFields & 0b01110000) >> 4;
        gif->IsGlobalColorTablePresent = (packedFields & 0b10000000) >> 7;

        if(gif->IsGlobalColorTablePresent)
        {
            //---<Image Descriptor>--------------------------------------------

            // 3 x 2^(Size of Global Color Table+1).
            size_t size = 3 * MathPowerOfTwo(gif->GlobalColorTableSize + 1); // ???

            GIFImageDescriptor imageDescriptor;

            unsigned char packedFields = 0;

            DataStreamReadCU(&dataStream, &imageDescriptor.Separator);
            DataStreamReadSU(&dataStream, &imageDescriptor.LeftPosition, EndianLittle);
            DataStreamReadSU(&dataStream, &imageDescriptor.TopPosition, EndianLittle);
            DataStreamReadSU(&dataStream, &imageDescriptor.Width, EndianLittle);
            DataStreamReadSU(&dataStream, &imageDescriptor.Height, EndianLittle);
            DataStreamReadCU(&dataStream, &packedFields);

            imageDescriptor.LocalColorTableSize = (packedFields & 0b00000111);
            imageDescriptor.Reserved = (packedFields & 0b00011000) >> 3;
            imageDescriptor.SortFlag = (packedFields & 0b00100000) >> 5;
            imageDescriptor.InterlaceFlag = (packedFields & 0b01000000) >> 6;
            imageDescriptor.LocalColorTableFlag = (packedFields & 0b10000000) >> 7;

            if(imageDescriptor.LocalColorTableFlag)
            {
                //---<Local Color Table>---------------------------------------

                //-------------------------------------------------------------
            }

            //-----------------------------------------------------------------
        }
    }

    //---<Table Based Image Data>--------------------------------------

    //-----------------------------------------------------------------


    return ActionSuccessful;
}

ActionResult GIFParseToImage(Image* const image, const void* const data, const size_t dataSize, size_t* dataRead)
{
    return ActionInvalid;
}

ActionResult GIFSerializeFromImage(const Image* const image, void* data, const size_t dataSize, size_t* dataWritten)
{
    return ActionInvalid;
}
