#include "PXPNG.h"

#include <stdlib.h>

#include <PX/Media/ZLIB/PXZLIB.h>
#include <PX/Media/ADAM7/PXADAM7.h>
#include <PX/Media/CRC32/PXCRC32.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Time/PXTime.h>
#include <PX/OS/Console/PXConsole.h>

#define PNGDebugInfo 0

const char PXPNGHeaderSequenz[8] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
const PXInt32U PXPNGChunkHeaderList[] =
{
    PXTypeInt32UBE,
    PXTypeText(4)
};
const PXInt8U PXPNGChunkHeaderListSize = sizeof(PXPNGChunkHeaderList) / sizeof(PXInt32U);


const PXInt32U PXPNGInfoHeaderList[] =
{
    PXTypeInt32UBE,
    PXTypeInt32UBE,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U
};
const PXInt8U PXPNGInfoHeaderListSize = sizeof(PXPNGInfoHeaderList) / sizeof(PXInt32U);


const PXInt32U PXPNGPrimaryChromaticsList[] =
{
     PXTypeInt32UBE,
     PXTypeInt32UBE,
     PXTypeInt32UBE,
     PXTypeInt32UBE,
     PXTypeInt32UBE,
     PXTypeInt32UBE,
     PXTypeInt32UBE,
     PXTypeInt32UBE
};
const PXInt8U PXPNGPrimaryChromaticsListSize = sizeof(PXPNGPrimaryChromaticsList) / sizeof(PXInt32U);



const PXInt32U PXPNGLastModificationTimeList[] =
{
    PXTypeInt16UBE,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U
};
const PXInt8U PXPNGLastModificationTimeListSize = sizeof(PXPNGLastModificationTimeList) / sizeof(PXInt32U);




PXPNGColorType PXAPI PXPNGColorTypeFromID(const PXInt8U colorType)
{
    switch(colorType)
    {
        case 0u:
            return PXPNGColorGrayscale;

        case 2u:
            return PXPNGColorRGB;

        case 3u:
            return PXPNGColorPalette;

        case 4u:
            return PXPNGColorGrayscaleAlpha;

        case 6u:
            return PXPNGColorRGBA;

        default:
            return PXPNGColorInvalid;
    }
}

PXInt8U PXAPI PXPNGColorTypeToID(const PXPNGColorType colorType)
{
    switch(colorType)
    {
        default:
        case PXPNGColorInvalid:
            return -1;

        case PXPNGColorGrayscale:
            return 0u;

        case PXPNGColorRGB:
            return 2u;

        case PXPNGColorPalette:
            return 3u;

        case PXPNGColorGrayscaleAlpha:
            return 4u;

        case PXPNGColorRGBA:
            return 6u;
    }
}

PXPNGInterlaceMethod PXAPI PXPNGInterlaceMethodFromID(const PXInt8U interlaceMethod)
{
    switch(interlaceMethod)
    {
        case 0u:
            return PXPNGInterlaceNone;

        case 1u:
            return PXPNGInterlaceADAM7;

        default:
            return PXPNGInterlaceInvalid;
    }
}

PXInt8U PXAPI PXPNGInterlaceMethodToID(const PXPNGInterlaceMethod interlaceMethod)
{
    switch(interlaceMethod)
    {
        default:
        case PXPNGInterlaceInvalid:
            return (PXInt8U)-1;

        case PXPNGInterlaceNone:
            return 0u;

        case PXPNGInterlaceADAM7:
            return 1u;
    }
}

void PXAPI PXPNGDestruct(PXPNG* const png)
{

    png->PixelData = PXMemoryHeapCallocT(PXByte, png->PixelDataSize, &, &png->PixelDataSize);

    png->PixelDataSize = 0;
    png->PixelData = 0;
}

PXInt8U PXAPI PXPNGColorTypeNumberOfChannels(const PXPNGColorType pngColorType)
{
    switch(pngColorType)
    {
        default:
        case PXPNGColorInvalid:
            return (PXInt8U)-1;

        case PXPNGColorPalette:
        case PXPNGColorGrayscale:
            return 1u;

        case PXPNGColorGrayscaleAlpha:
            return 2u;

        case PXPNGColorRGB:
            return 3u;

        case PXPNGColorRGBA:
            return 4u;
    }
}

PXInt8U PXAPI PXPNGBitsPerPixel(const PXPNG* const png)
{
    const PXInt8U numberOfColorChannels = PXPNGColorTypeNumberOfChannels(png->ImageHeader.ColorType);

    return png->ImageHeader.BitDepth * numberOfColorChannels;
}

PXSize PXAPI PXPNGFilePredictSize(PXImage* const pxImage, PXSize* const fileSize)
{
    const PXSize signature = 8;
    const PXSize header = 25;
    const PXSize time = 19;
    const PXSize end = 12;
    const PXSize idat = 32768;

    const PXSize sum = signature + header + time + end + pxImage->Height * pxImage->Height * 0000000 + 1024u; // bbp

    return sum;
}

PXActionResult PXAPI PXPNGPeekFromFile(PXResourceTransphereInfo* const pxResourceTransphereInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceTransphereInfo->ResourceTarget;
    PXPNG* png = PXNull;

    /*
    PXFile imageDataCache;

    PXSize imageDataCounter = 0;
    PXSize imageDataChunkCacheSizeUSED = 0;
    PXSize imageDataChunkCacheSizeMAX = 0u;
    unsigned char* imageDataChunkCache = 0;
    */

    //---<Parse PNG File>------------------------------------------------------
    {
        PXBool parseFinished = 0;

        //---<Check PNG Header>------------------------------------------------
        {
            const PXBool isValidHeader = PXFileReadAndCompare(pxResourceTransphereInfo->FileReference, PXPNGHeaderSequenz, sizeof(PXPNGHeaderSequenz));

            if(!isValidHeader)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        // Allocate resource
        png = PXMemoryHeapCallocT(PXPNG, 1);
        pxResourceTransphereInfo->ResourceSource = png;
        //---------------------------------------------------------------------

#if PNGDebugInfo
        printf
        (
            "+------+--------------------------------------------+---+---+---+-------+-----+\n"
            "| ID   | Name                                       | E | R | S | Bytes | CRC |\n"
            "+------+--------------------------------------------+---+---+---+-------+-----+\n"
        );
#endif

        // Parse every chunk until finished.
        while(!parseFinished)
        {
            PXPNGChunk chunk;
            PXSize predictedOffset = 0;

            PXFileBinding
            (
                pxResourceTransphereInfo->FileReference,
                &chunk.Header,
                PXPNGChunkHeaderList,
                PXPNGChunkHeaderListSize,
                PXFileBindingRead
            );

            // Check
            {
                chunk.ChunkType = PXInt32FromAdress(chunk.Header.ID);

                // Ancillary bit : bit 5 of first byte
                // 0 (uppercase) = critical, 1 (lowercase) = ancillary.
                chunk.IsEssential = PXTextIsLetterCaseUpper(chunk.Header.ID[0]);

                // Private bit: bit 5 of second byte
                // Must be 0 (uppercase)in files conforming to this version of PNG.
                chunk.IsRegisteredStandard = PXTextIsLetterCaseUpper(chunk.Header.ID[1]);

                // Safe-to-copy bit: bit 5 of fourth byte
                // 0 (uppercase) = unsafe to copy, 1 (lowercase) = safe to copy.
                chunk.IsSafeToCopy = PXTextIsLetterCaseUpper(chunk.Header.ID[3]);

                predictedOffset = pxResourceTransphereInfo->FileReference->DataCursor + chunk.Header.Size;
            }

#if PNGDebugInfo
            printf
            (
                "| %c%c%c%c | %-42s | %c | %c | %c | %5i | Yes |\n",
                chunk.ChunkID.Data[0],
                chunk.ChunkID.Data[1],
                chunk.ChunkID.Data[2],
                chunk.ChunkID.Data[3],
                "---",
                chunk.IsEssential ? 'x' : '-',
                chunk.IsRegisteredStandard ? 'x' : '-',
                chunk.IsSafeToCopy ? 'x' : '-',
                chunk.Lengh
            );
#endif

            //---Get Chunk Data------------------------------------------
            switch(chunk.ChunkType)
            {
                case PXPNGChunkImageHeader:
                {
                    PXFileBinding
                    (
                        pxResourceTransphereInfo->FileReference,
                        &png->ImageHeader,
                        PXPNGInfoHeaderList,
                        PXPNGInfoHeaderListSize,
                        PXFileBindingRead
                    );

                    png->ImageHeader.ColorType = PXPNGColorTypeFromID(png->ImageHeader.ColorTypeID);
                    png->ImageHeader.InterlaceMethod = PXPNGInterlaceMethodFromID(png->ImageHeader.InterlaceMethodID);

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "PNG",
                        "Load",
                        "Header..\n"
                        "%20s : %i\n"
                        "%20s : %i\n"
                        "%20s : %i\n"
                        "%20s : %i\n"
                        "%20s : %i\n"
                        "%20s : %i\n"
                        "%20s : %i\n"
                        "%20s : %i",
                        "Width", png->ImageHeader.Width,
                        "Height", png->ImageHeader.Height,
                        "BitDepth", png->ImageHeader.BitDepth,
                        "ColorTypeID", png->ImageHeader.ColorTypeID,
                        "CompressionMethod", png->ImageHeader.CompressionMethod,
                        "FilterMethod", png->ImageHeader.FilterMethod,
                        "InterlaceMethodID", png->ImageHeader.InterlaceMethodID,
                        "ColorType", png->ImageHeader.ColorType,
                        "InterlaceMethod", png->ImageHeader.InterlaceMethod
                    );
#endif

                    break;
                }
                case PXPNGChunkPalette:
                {
                    const PXSize palettSize = chunk.Header.Size / 3u;
                    const PXBool validSize = palettSize != 0 && palettSize <= 256;

                    if(!validSize)
                        return PXActionFailedFormatNotAsExpected; // palette too small or big

                    png->PaletteSize = palettSize;

                    for(PXSize i = 0; i < palettSize; ++i)
                    {
                        PXInt8U* paletteInsertion = png->Palette + i * 4;

                        PXFileReadB(pxResourceTransphereInfo->FileReference, paletteInsertion, 3u); // Read RGB value

                        paletteInsertion[3] = 0xFF; // Add alpha
                    }

                    break;
                }
                case PXPNGChunkImageData:
                {
                    ++png->DataBlockListAmount;

                    png->DataBlockList = PXMemoryHeapReallocT(PXPNGDataBlock, png->DataBlockList, png->DataBlockListAmount);
                    png->DataBlockTotalSize += chunk.Header.Size;

                    PXPNGDataBlock* pxPNGDataBlock = &png->DataBlockList[png->DataBlockListAmount - 1];
                    pxPNGDataBlock->FileOffset = pxResourceTransphereInfo->FileReference->DataCursor;
                    pxPNGDataBlock->DataSize = chunk.Header.Size;

                    break;
                }
                case PXPNGChunkImageEnd:
                {
                    parseFinished = 1;
                    break;
                }
                case PXPNGChunkTransparency:
                {
                    switch(png->ImageHeader.ColorType)
                    {
                        case PXPNGColorGrayscale:
                        {
                            /*error: this chunk must be 2 bytes for grayscale image*/
                            if(chunk.Header.Size != 2)
                                return PXActionInvalid;

                            unsigned short value;

                            PXFileReadI16UE(pxResourceTransphereInfo->FileReference, &value, PXEndianBig);

                            // color->key_defined = 1;
                            //color->key_r = color->key_g = color->key_b = 256u * data[0] + data[1];

                            break;
                        }
                        case PXPNGColorRGB:
                        {
                            /*error: this chunk must be 6 bytes for RGB image*/
                            if(chunk.Header.Size != 6)
                                return PXActionInvalid;

                            unsigned short red;
                            unsigned short green;
                            unsigned short blue;

                            PXFileReadI16UE(pxResourceTransphereInfo->FileReference, &red, PXEndianBig);
                            PXFileReadI16UE(pxResourceTransphereInfo->FileReference, &green, PXEndianBig);
                            PXFileReadI16UE(pxResourceTransphereInfo->FileReference, &blue, PXEndianBig);

                            //color->key_defined = 1;
                            //color->key_r = 256u * data[0] + data[1];
                            //color->key_g = 256u * data[2] + data[3];
                            //color->key_b = 256u * data[4] + data[5];

                            break;
                        }
                        case PXPNGColorPalette:
                        {
                            /*error: more alpha values given than there are palette entries*/
                            //if(chunkLength > color->palettesize) return 39;

                            //for(PXSize i = 0; i != chunkLength; ++i) color->palette[4 * i + 3] = data[i];

                            for(PXSize i = 0; i < chunk.Header.Size; ++i)
                            {
                                unsigned char value = 0;

                                PXFileReadI8U(pxResourceTransphereInfo->FileReference, &value);

                                png->Palette[i * 4 + 3] = value;
                            }

                            break;
                        }
                        case PXPNGColorGrayscaleAlpha:
                        case PXPNGColorRGBA:
                        case PXPNGColorInvalid:
                        default:
                            return PXActionFailedFormatNotAsExpected; // tRNS chunk not allowed for other color models
                    }

                    break;
                }
                case PXPNGChunkImageGamma:
                {
                    PXFileReadI32UE(pxResourceTransphereInfo->FileReference, &png->Gamma, PXEndianBig);

                    break;
                }
                case PXPNGChunkPrimaryChromaticities:
                {
                    PXFileBinding
                    (
                        pxResourceTransphereInfo->FileReference,
                        &png->PrimaryChromatics,
                        PXPNGPrimaryChromaticsList,
                        PXPNGPrimaryChromaticsListSize,
                        PXFileBindingRead
                    );

                    break;
                }
                case PXPNGChunkStandardRGBColorSpace:
                {
                    // pxFile.Read(RenderingIntent);

                    // COpy array

                    break;
                }
                case PXPNGChunkEmbeddedICCProfile:
                {
                    //  pxFile.DataCursor += chunk.Lengh;

                    break;
                }
                case PXPNGChunkTextualData:
                {
                    //  pxFile.DataCursor += chunk.Lengh;


                    break;
                }
                case PXPNGChunkCompressedTextualData:
                {
                    // Keyword                         1 - 79 bytes(character string)
                    // Null separator                 1 byte(null character)
                    // Compression method             1 byte
                    // Compressed text datastream     n bytes

                    //  pxFile.DataCursor += chunk.Lengh;

                    break;
                }
                case PXPNGChunkBackgroundColor:
                {
                    switch(png->ImageHeader.ColorType)
                    {
                        default:
                        case PXPNGColorInvalid:
                            break; // ERROR

                        case PXPNGColorGrayscale:
                        case PXPNGColorGrayscaleAlpha:
                        {
                            //  pxFile.Read(png.BackgroundColor.GreyScale, PXEndianBig);
                            break;
                        }
                        case PXPNGColorRGB:
                        case PXPNGColorRGBA:
                        {
                            // pxFile.Read(png.BackgroundColor.Red, PXEndianBig);
                            // pxFile.Read(png.BackgroundColor.Green, PXEndianBig);
                            // pxFile.Read(png.BackgroundColor.Blue, PXEndianBig);
                            break;
                        }
                        case PXPNGColorPalette:
                        {
                            //pxFile.Read(png.BackgroundColor.PaletteIndex);
                            break;
                        }
                    }

                    break;
                }
                case PXPNGChunkPhysicalPixelDimensions:
                {
                    unsigned char unitSpecifier = 0;

                    PXFileReadI32UE(pxResourceTransphereInfo->FileReference, &png->PhysicalPixelDimension.PixelsPerUnit[0], PXEndianBig);
                    PXFileReadI32UE(pxResourceTransphereInfo->FileReference, &png->PhysicalPixelDimension.PixelsPerUnit[1], PXEndianBig);
                    PXFileReadI8U(pxResourceTransphereInfo->FileReference, &unitSpecifier);

                    switch(unitSpecifier)
                    {
                        case 0:
                            png->PhysicalPixelDimension.UnitSpecifier = PXPNGUnitSpecifierUnkown;
                            break;

                        case 1:
                            png->PhysicalPixelDimension.UnitSpecifier = PXPNGUnitSpecifierMeter;
                            break;

                        default:
                            png->PhysicalPixelDimension.UnitSpecifier = PXPNGUnitSpecifierInvalid;
                            break;
                    }

                    break;
                }
                case PXPNGChunkSignificantBits:
                {
                    /*
                    unsigned int byteLength = 0;
                    unsigned int result = 0;

                    switch (ColorType)
                    {
                        case PNGColorGrayscale: // single byte,
                            byteLength = 1;
                            break;

                        case PNGColorTruecolor: // three bytes,
                        case PNGColorIndexedColor:
                            byteLength = 3;
                            break;

                        case PNGColorGrayscaleWithAlphaChannel: // two bytes
                            byteLength = 2;
                            break;

                        case PNGColorTruecolorWithAlphaChannel: //  four bytes,
                            byteLength = 4;
                            break;
                    }

                    for (unsigned int i = 0; i < byteLength; i++)
                    {
                        char calcbyte;

                        pxFile.Read(calcbyte);

                        result = (result << (i * 8)) | calcbyte;
                    }

                    SignificantBits = result;*/

                    break;
                }
                case PXPNGChunkSuggestedPalette:
                {
                    break;
                }
                case PXPNGChunkPaletteHistogram:
                {
                    const PXInt32U listSize = chunk.Header.Size / 2;

                    png->PaletteHistogram.ColorFrequencyListSize = listSize;
                    png->PaletteHistogram.ColorFrequencyList = PXMemoryHeapCallocT(PXInt16U, listSize);

                    PXFileReadI16UVE(pxResourceTransphereInfo->FileReference, &png->PaletteHistogram.ColorFrequencyList, listSize, PXEndianBig);

                    break;
                }
                case PXPNGChunkLastModificationTime:
                {
                    PXFileBinding
                    (
                        pxResourceTransphereInfo->FileReference,
                        &png->LastModificationTime,
                        PXPNGLastModificationTimeList,
                        PXPNGLastModificationTimeListSize,
                        PXFileBindingRead
                    );

                    break;
                }
                case PXPNGChunkCustom:
                default:
                {
                    PXFileCursorAdvance(pxResourceTransphereInfo->FileReference, chunk.Header.Size);
                    break;
                }
            }
            //---------------------------------------------------------------

#if PNGDebugInfo
            if(pxFile->DataCursor != predictedOffset)
            {
                printf("[i][PNG] Chunk did not handle all Bytes\n");
            }
#endif
            pxResourceTransphereInfo->FileReference->DataCursor = predictedOffset;

            PXFileReadI32UE(pxResourceTransphereInfo->FileReference, &chunk.CRC, PXEndianBig); // 4 Bytes

            //---<Check CRC>---
            // TODO: Yes
            //-----------------
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXPNGLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceLoadInfo->ResourceTarget;
    PXPNG* const png = (PXPNG*)pxResourceLoadInfo->ResourceSource;

    //---<Allocate>------------------------------------------------------------
    {
        PXColorFormat imageDataFormat;

        switch(png->ImageHeader.ColorType)
        {
            case PXPNGColorGrayscale:
                imageDataFormat = PXColorFormatInvalid;
                break;

            case PXPNGColorRGB:
                imageDataFormat = PXColorFormatRGBI8;
                break;

            case PXPNGColorInvalid:
            case PXPNGColorGrayscaleAlpha:
                imageDataFormat = PXColorFormatInvalid;
                break;

            case PXPNGColorPalette:
            case PXPNGColorRGBA:
                imageDataFormat = PXColorFormatRGBAI8;
                break;

            default:
                imageDataFormat = PXColorFormatInvalid;
                break;
        }

        const PXBool allocateResult = PXImageResize(pxImage, imageDataFormat, png->ImageHeader.Width, png->ImageHeader.Height);

        if(!allocateResult)
        {
            return PXActionFailedMemoryAllocation;
        }
    }
    //-------------------------------------------------------------------------


    PXFile pxZLIBStream;


    PXFileOpenInfo pxFileOpenInfo;
    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
    pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeUseOnce;

    if(png->DataBlockListAmount == 1) // Special behaviour if we only have one chunk
    {
        const PXPNGDataBlock* const pxPNGDataBlock = &png->DataBlockList[0];

        pxFileOpenInfo.BufferData = (PXByte*)pxResourceLoadInfo->FileReference->Data + pxPNGDataBlock->FileOffset;
        pxFileOpenInfo.BufferSize = pxPNGDataBlock->DataSize;
        pxFileOpenInfo.FlagList |= PXFileIOInfoFileMemory;

        PXFileOpen(&pxZLIBStream, &pxFileOpenInfo);
    }
    else
    {
        pxFileOpenInfo.FileSizeRequest = png->DataBlockTotalSize;
        pxFileOpenInfo.FlagList |= PXFileIOInfoFileVirtual;

        PXFileOpen(&pxZLIBStream, &pxFileOpenInfo);

        for(PXSize i = 0; i < png->DataBlockListAmount; i++)
        {
            const PXPNGDataBlock* const pxPNGDataBlock = &png->DataBlockList[i];

            PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxPNGDataBlock->FileOffset);
            PXFileDataCopy(pxResourceLoadInfo->FileReference, &pxZLIBStream, pxPNGDataBlock->DataSize);
        }

        pxZLIBStream.DataCursor = 0;
    }



    //---<Unpack compressed data>----------------------------------------------
    {
        PXSize workingMemorySize = 0;
        const PXInt8U bitsPerPixel = PXPNGBitsPerPixel(png);

        //---------------------------------------------------------------------
        // ZLIB
        //---------------------------------------------------------------------


        PXFile pxZLIBResultStream;

        {
            const PXSize expectedPXZLIBCacheSize = PXZLIBCalculateExpectedSize(png->ImageHeader.Width, png->ImageHeader.Height, bitsPerPixel, png->ImageHeader.InterlaceMethod);

            PXFileOpenInfo pxFileOpenInfo;
            PXClear(PXFileOpenInfo, &pxFileOpenInfo);
            pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
            pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;
            pxFileOpenInfo.FileSizeRequest = expectedPXZLIBCacheSize;

            const PXActionResult pxOpenResult = PXFileOpen(&pxZLIBResultStream, &pxFileOpenInfo);
        }

        const PXActionResult actionResult = PXZLIBDecompress(&pxZLIBStream, &pxZLIBResultStream);
        const PXBool success = PXActionSuccessful == actionResult;

        if(!success)
        {
            return actionResult;
        }
        //---------------------------------------------------------------------



        //---------------------------------------------------------------------
        // ADAM7
        //---------------------------------------------------------------------
        PXFile pxADAM7CacheOutput;

        PXADAM7 pxADAM7;
        PXClear(PXADAM7, &pxADAM7);
        pxADAM7.DataInput = (char*)pxZLIBResultStream.Data;
        //pxADAM7.DataOutput = pxImage->PixelData;
        //pxADAM7.OutputSize = pxImage->PixelDataSize;
        pxADAM7.InputSize = pxZLIBResultStream.DataUsed;
        pxADAM7.Width = png->ImageHeader.Width;
        pxADAM7.Height = png->ImageHeader.Height;
        pxADAM7.BitsPerPixel = bitsPerPixel;
        pxADAM7.InterlaceMethod = png->ImageHeader.InterlaceMethod;

        const PXSize expectedadam7CacheSize = PXADAM7CaluclateExpectedSize(&pxADAM7);

        {
            PXFileOpenInfo pxFileOpenInfo;
            PXClear(PXFileOpenInfo, &pxFileOpenInfo);
            pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
            pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeUseOnce;
            pxFileOpenInfo.FileSizeRequest = expectedadam7CacheSize;
            pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;

            PXFileOpen(&pxADAM7CacheOutput, &pxFileOpenInfo);
        }


        pxADAM7.DataOutput = pxADAM7CacheOutput.Data;
        pxADAM7.OutputSize = pxADAM7CacheOutput.DataAllocated;

        const PXActionResult scanDecodeResult = PXADAM7ScanlinesDecode(&pxADAM7);

        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        // Color Comprerss
        //---------------------------------------------------------------------
        const PXActionResult decompress = PXPNGImageDataDecompress
        (
            png,
            pxADAM7.DataOutput,
            pxImage->PixelData,
            png->ImageHeader.BitDepth,
            png->ImageHeader.ColorType
        );
        //---------------------------------------------------------------------


        PXFileClose(&pxZLIBStream);
        PXFileClose(&pxZLIBResultStream);
        PXFileClose(&pxADAM7CacheOutput);
    }

    //-------------------------------------------------------------------------

    PXPNGDestruct(png);

    return PXActionSuccessful;
}















typedef enum LodePNGFilterStrategy
{
    /*every filter at zero*/
    LFS_ZERO = 0,
    /*every filter at 1, 2, 3 or 4 (paeth), unlike LFS_ZERO not a good choice, but for testing*/
    LFS_ONE = 1,
    LFS_TWO = 2,
    LFS_THREE = 3,
    LFS_FOUR = 4,
    /*Use filter that gives minimum sum, as described in the official PNG filter heuristic.*/
    LFS_MINSUM,
    /*Use the filter type that gives smallest Shannon entropy for this scanline. Depending
    on the image, this is better or worse than minsum.*/
    LFS_ENTROPY,
    /*
    Brute-force-search PNG filters by compressing each filter for each scanline.
    Experimental, very slow, and only rarely gives better compression than MINSUM.
    */
    LFS_BRUTE_FORCE,
    /*use predefined_filters buffer: you specify the filter type for each scanline*/
    LFS_PREDEFINED
} LodePNGFilterStrategy;


/* integer binary logarithm, max return value is 31 */
static PXSize ilog2(PXSize i)
{
    PXSize result = 0;
    if(i >= 65536)
    {
        result += 16;
        i >>= 16;
    }
    if(i >= 256)
    {
        result += 8;
        i >>= 8;
    }
    if(i >= 16)
    {
        result += 4;
        i >>= 4;
    }
    if(i >= 4)
    {
        result += 2;
        i >>= 2;
    }
    if(i >= 2)
    {
        result += 1; /*i >>= 1;*/
    }
    return result;
}

/* integer approximation for i * log2(i), helper function for LFS_ENTROPY */
static PXSize ilog2i(PXSize i)
{
    PXSize l;
    if(i == 0) return 0;
    l = ilog2(i);
    /* approximate i*log2(i): l is integer logarithm, ((i - (1u << l)) << 1u)
    linearly approximates the missing fractional part multiplied by i */
    return i * l + ((i - (1u << l)) << 1u);
}

void filterScanline(unsigned char* out, const unsigned char* scanline, const unsigned char* prevline, PXSize length, PXSize bytewidth, unsigned char filterType)
{
    PXSize i;
    switch(filterType)
    {
        case 0: /*None*/
            for(i = 0; i != length; ++i) out[i] = scanline[i];
            break;
        case 1: /*Sub*/
            for(i = 0; i != bytewidth; ++i) out[i] = scanline[i];
            for(i = bytewidth; i < length; ++i) out[i] = scanline[i] - scanline[i - bytewidth];
            break;
        case 2: /*Up*/
            if(prevline)
            {
                for(i = 0; i != length; ++i) out[i] = scanline[i] - prevline[i];
            }
            else
            {
                for(i = 0; i != length; ++i) out[i] = scanline[i];
            }
            break;
        case 3: /*Average*/
            if(prevline)
            {
                for(i = 0; i != bytewidth; ++i) out[i] = scanline[i] - (prevline[i] >> 1);
                for(i = bytewidth; i < length; ++i) out[i] = scanline[i] - ((scanline[i - bytewidth] + prevline[i]) >> 1);
            }
            else
            {
                for(i = 0; i != bytewidth; ++i) out[i] = scanline[i];
                for(i = bytewidth; i < length; ++i) out[i] = scanline[i] - (scanline[i - bytewidth] >> 1);
            }
            break;
        case 4: /*Paeth*/
            if(prevline)
            {
                /*paethPredictor(0, prevline[i], 0) is always prevline[i]*/
                for(i = 0; i != bytewidth; ++i) out[i] = (scanline[i] - prevline[i]);
                for(i = bytewidth; i < length; ++i)
                {
                    out[i] = (scanline[i] - PXADAM7paethPredictor(scanline[i - bytewidth], prevline[i], prevline[i - bytewidth]));
                }
            }
            else
            {
                for(i = 0; i != bytewidth; ++i) out[i] = scanline[i];
                /*paethPredictor(scanline[i - bytewidth], 0, 0) is always scanline[i - bytewidth]*/
                for(i = bytewidth; i < length; ++i) out[i] = (scanline[i] - scanline[i - bytewidth]);
            }
            break;
        default:
            return; /*invalid filter type given*/
    }
}

/*
Settings for PXZLIB compression. Tweaking these settings tweaks the balance
between speed and compression ratio.
*/
typedef struct LodePNGCompressSettings LodePNGCompressSettings;
struct LodePNGCompressSettings /*deflate = compress*/
{
    /*LZ77 related settings*/
    unsigned btype; /*the block type for LZ (0, 1, 2 or 3, see PXZLIB standard). Should be 2 for proper compression.*/
    unsigned use_lz77; /*whether or not to use LZ77. Should be 1 for proper compression.*/
    unsigned windowsize; /*must be a power of two <= 32768. higher compresses more but is slower. Default value: 2048.*/
    unsigned minmatch; /*minimum lz77 length. 3 is normally best, 6 can be better for some PNGs. Default: 0*/
    unsigned nicematch; /*stop searching if >= this length found. Set to 258 for best compression. Default: 128*/
    unsigned lazymatching; /*use lazy matching: better compression but a bit slower. Default: true*/

    /*use custom PXZLIB encoder instead of built in one (default: null)*/
    unsigned (*custom_PXZLIB)(unsigned char**, PXSize*,
                              const unsigned char*, PXSize,
                              const LodePNGCompressSettings*);
    /*use custom deflate encoder instead of built in one (default: null)
    if custom_PXZLIB is used, custom_deflate is ignored since only the built in
    PXZLIB function will call custom_deflate*/
    unsigned (*custom_deflate)(unsigned char**, PXSize*,
                               const unsigned char*, PXSize,
                               const LodePNGCompressSettings*);

    const void* custom_context; /*optional custom settings for custom functions*/
};


/*in an idat chunk, each scanline is a multiple of 8 bits, unlike the lodepng output buffer,
and in addition has one extra byte per line: the filter byte. So this gives a larger
result than lodepng_get_raw_size. Set h to 1 to get the size of 1 row including filter byte. */
static PXSize lodepng_get_raw_size_idat(unsigned w, unsigned h, unsigned bpp)
{
    /* + 1 for the filter byte, and possibly plus padding bits per line. */
    /* Ignoring casts, the expression is equal to (w * bpp + 7) / 8 + 1, but avoids overflow of w * bpp */
    PXSize line = ((PXSize)(w / 8u) * bpp) + 1u + ((w & 7u) * bpp + 7u) / 8u;
    return (PXSize)h * line;
}

/*Settings for the encoder.*/
typedef struct LodePNGEncoderSettings
{
    LodePNGCompressSettings PXZLIBsettings; /*settings for the PXZLIB encoder, such as window size, ...*/

    unsigned auto_convert; /*automatically choose output PNG color type. Default: true*/

    /*If true, follows the official PNG heuristic: if the PNG uses a palette or lower than
    8 bit depth, set all filters to zero. Otherwise use the filter_strategy. Note that to
    completely follow the official PNG heuristic, filter_palette_zero must be true and
    filter_strategy must be LFS_MINSUM*/
    unsigned filter_palette_zero;
    /*Which filter strategy to use when not using zeroes due to filter_palette_zero.
    Set filter_palette_zero to 0 to ensure always using your chosen strategy. Default: LFS_MINSUM*/
    LodePNGFilterStrategy filter_strategy;
    /*used if filter_strategy is LFS_PREDEFINED. In that case, this must point to a buffer with
    the same length as the amount of scanlines in the image, and each value must <= 5. You
    have to cleanup this buffer, LodePNG will never free it. Don't forget that filter_palette_zero
    must be set to 0 to ensure this is also used on palette or low bitdepth images.*/
    const unsigned char* predefined_filters;

    /*force creating a PLTE chunk if colortype is 2 or 6 (= a suggested palette).
    If colortype is 3, PLTE is always created. If color type is explicitely set
    to a grayscale type (1 or 4), this is not done and is ignored. If enabling this,
    a palette must be present in the info_png.
    NOTE: enabling this may worsen compression if auto_convert is used to choose
    optimal color mode, because it cannot use grayscale color modes in this case*/
    unsigned force_palette;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
    /*add LodePNG identifier and version as a text chunk, for debugging*/
    unsigned add_id;
    /*encode text chunks as zTXt chunks instead of tEXt chunks, and use compression in iTXt chunks*/
    unsigned text_compression;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
} LodePNGEncoderSettings;

static unsigned filter
(
    unsigned char* out,
    const unsigned char* in,
    PXSize width,
    PXSize height,
    PXSize bpp,
    // const LodePNGColorMode* color,
    LodePNGFilterStrategy strategy
)
{

    /*
    For PNG filter method 0
    out must be a buffer with as size: h + (w * h * bpp + 7u) / 8u, because there are
    the scanlines with 1 extra byte per scanline
    */

    //                   unsigned bpp = lodepng_get_bpp(color);
        /*the width of a scanline in bytes, not including the filter type*/
    PXSize linebytes = lodepng_get_raw_size_idat(width, 1, bpp) - 1u;

    /*bytewidth is used for filtering, is 1 when bpp < 8, number of bytes per pixel otherwise*/
    PXSize bytewidth = (bpp + 7u) / 8u;
    const unsigned char* prevline = 0;
    unsigned error = 0;

    /*
    There is a heuristic called the minimum sum of absolute differences heuristic, suggested by the PNG standard:
     *  If the image type is Palette, or the bit depth is smaller than 8, then do not filter the image (i.e.
        use fixed filtering, with the filter None).
     * (The other case) If the image type is Grayscale or RGB (with or without Alpha), and the bit depth is
       not smaller than 8, then use adaptive filtering heuristic as follows: independently for each row, apply
       all five filters and select the filter that produces the smallest sum of absolute values per row.
    This heuristic is used if filter strategy is LFS_MINSUM and filter_palette_zero is true.

    If filter_palette_zero is true and filter_strategy is not LFS_MINSUM, the above heuristic is followed,
    but for "the other case", whatever strategy filter_strategy is set to instead of the minimum sum
    heuristic is used.
    */




    // if(settings->filter_palette_zero &&
    //    (color->colortype == LCT_PALETTE || color->bitdepth < 8)) strategy = LFS_ZERO;





    if(bpp == 0) return 31; /*error: invalid color type*/

    if(strategy >= LFS_ZERO && strategy <= LFS_FOUR)
    {
        unsigned char type = (unsigned char)strategy;
        for(PXSize y = 0; y != height; ++y)
        {
            PXSize outindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
            PXSize inindex = linebytes * y;
            out[outindex] = type; /*filter type byte*/
            filterScanline(&out[outindex + 1], &in[inindex], prevline, linebytes, bytewidth, type);
            prevline = &in[inindex];
        }
    }
    else if(strategy == LFS_MINSUM)
    {
        /*adaptive filtering*/
        unsigned char* attempt[5]; /*five filtering attempts, one for each filter type*/
        PXSize smallest = 0;
        unsigned char type, bestType = 0;

        for(type = 0; type != 5; ++type)
        {
            attempt[type] = PXMemoryHeapCallocT(PXByte, linebytes);

            if(!attempt[type])
                error = 83; /*alloc fail*/
        }

        if(!error)
        {
            for(PXSize y = 0; y != height; ++y)
            {
                /*try the 5 filter types*/
                for(type = 0; type != 5; ++type)
                {
                    PXSize sum = 0;
                    filterScanline(attempt[type], &in[y * linebytes], prevline, linebytes, bytewidth, type);

                    /*calculate the sum of the result*/
                    if(type == 0)
                    {
                        for(PXSize x = 0; x != linebytes; ++x)
                            sum += (unsigned char)(attempt[type][x]);
                    }
                    else
                    {
                        for(PXSize x = 0; x != linebytes; ++x)
                        {
                            /*For differences, each byte should be treated as signed, values above 127 are negative
                            (converted to signed char). Filtertype 0 isn't a difference though, so use unsigned there.
                            This means filtertype 0 is almost never chosen, but that is justified.*/
                            unsigned char s = attempt[type][x];
                            sum += s < 128 ? s : (255U - s);
                        }
                    }

                    /*check if this is smallest sum (or if type == 0 it's the first case so always store the values)*/
                    if(type == 0 || sum < smallest)
                    {
                        bestType = type;
                        smallest = sum;
                    }
                }

                prevline = &in[y * linebytes];

                /*now fill the out values*/
                out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/

                for(PXSize x = 0; x != linebytes; ++x)
                    out[y * (linebytes + 1) + 1 + x] = attempt[bestType][x];
            }
        }

        //for (type = 0; type != 5; ++type) PXResizeList(PXByte, attempt[type], -1);
    }
    else if(strategy == LFS_ENTROPY)
    {
        unsigned char* attempt[5]; /*five filtering attempts, one for each filter type*/
        PXSize bestSum = 0;
        unsigned type, bestType = 0;
        unsigned count[256];

        for(type = 0; type != 5; ++type)
        {
            attempt[type] = PXMemoryHeapCallocT(PXByte, linebytes);

            if(!attempt[type])
                error = 83; /*alloc fail*/
        }

        if(!error)
        {
            for(PXSize y = 0; y != height; ++y)
            {
                /*try the 5 filter types*/
                for(type = 0; type != 5; ++type)
                {
                    PXSize sum = 0;
                    filterScanline(attempt[type], &in[y * linebytes], prevline, linebytes, bytewidth, type);
                    PXMemoryClear(count, 256 * sizeof(*count));

                    for(PXSize x = 0; x != linebytes; ++x)
                        ++count[attempt[type][x]];

                    ++count[type]; /*the filter type itself is part of the scanline*/

                    for(PXSize x = 0; x != 256; ++x)
                    {
                        sum += ilog2i(count[x]);
                    }

                    /*check if this is smallest sum (or if type == 0 it's the first case so always store the values)*/
                    if(type == 0 || sum > bestSum)
                    {
                        bestType = type;
                        bestSum = sum;
                    }
                }

                prevline = &in[y * linebytes];

                // now fill the out values
                out[y * (linebytes + 1)] = bestType; // the first byte of a scanline will be the filter type

                for(PXSize x = 0; x != linebytes; ++x) 
                    out[y * (linebytes + 1) + 1 + x] = attempt[bestType][x];
            }
        }

        for(type = 0; type != 5; ++type)
        {
            PXMemoryHeapFree(PXNull, attempt[type]);
        }
    }
    else if(strategy == LFS_PREDEFINED)
    {
        for(PXSize y = 0; y != height; ++y)
        {
            PXSize outindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
            PXSize inindex = linebytes * y;
            // unsigned char type = settings->predefined_filters[y];
            //   out[outindex] = type; /*filter type byte*/
            //  filterScanline(&out[outindex + 1], &in[inindex], prevline, linebytes, bytewidth, type);
            prevline = &in[inindex];
        }
    }
    else if(strategy == LFS_BRUTE_FORCE)
    {
        /*brute force filter chooser.
        deflate the scanline after every filter attempt to see which one deflates best.
        This is very slow and gives only slightly smaller, sometimes even larger, result*/
        PXSize size[5];
        unsigned char* attempt[5]; /*five filtering attempts, one for each filter type*/
        PXSize smallest = 0;
        unsigned type = 0, bestType = 0;
        unsigned char* dummy;
        LodePNGCompressSettings PXZLIBsettings;
        //                              memcpy(&PXZLIBsettings, &settings->PXZLIBsettings, sizeof(LodePNGCompressSettings));
        /*use fixed tree on the attempts so that the tree is not adapted to the filtertype on purpose,
        to simulate the true case where the tree is the same for the whole image. Sometimes it gives
        better result with dynamic tree anyway. Using the fixed tree sometimes gives worse, but in rare
        cases better compression. It does make this a bit less slow, so it's worth doing this.*/
        PXZLIBsettings.btype = 1;
        /*a custom encoder likely doesn't read the btype setting and is optimized for complete PNG
        images only, so disable it*/
        PXZLIBsettings.custom_PXZLIB = 0;
        PXZLIBsettings.custom_deflate = 0;

        for(type = 0; type != 5; ++type)
        {
            attempt[type] = PXMemoryHeapCallocT(PXByte, linebytes);

            if(!attempt[type])
                error = 83; /*alloc fail*/
        }

        if(!error)
        {
            for(PXSize y = 0; y != height; ++y) /*try the 5 filter types*/
            {
                for(type = 0; type != 5; ++type)
                {
                    unsigned testsize = (unsigned)linebytes;
                    // if(testsize > 8) testsize /= 8;*/ /*it already works good enough by testing a part of the row

                    filterScanline(attempt[type], &in[y * linebytes], prevline, linebytes, bytewidth, type);
                    size[type] = 0;
                    dummy = 0;

                    const PXSize sizeAA = 0xFFFF * 2;
                    dummy = PXMemoryHeapCallocT(PXByte, sizeAA);

                    PXSize written = 0;

                    // fix this: PXZLIBCompress(attempt[type], testsize, &dummy, &size[type], written);
                    // PXZLIB_compress( , &PXZLIBsettings);
                    
                    PXMemoryHeapFree(PXNull, dummy);
                    
                    // check if this is smallest size (or if type == 0 it's the first case so always store the values)
                    if(type == 0 || size[type] < smallest)
                    {
                        bestType = type;
                        smallest = size[type];
                    }
                }
                prevline = &in[y * linebytes];

                out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
                
                for(PXSize x = 0; x != linebytes; ++x) 
                    out[y * (linebytes + 1) + 1 + x] = attempt[bestType][x];
            }
        }
        for(type = 0; type != 5; ++type)
        {
            PXMemoryHeapFree(PXNull, attempt[type]);
        }
    }
    else
        return 88; // unknown filter strategy

    return error;
}

void setBitOfReversedStream(PXSize* bitpointer, unsigned char* bitstream, unsigned char bit)
{
    // the current bit in bitstream may be 0 or 1 for this to work
    if(bit == 0) bitstream[(*bitpointer) >> 3u] &= (unsigned char)(~(1u << (7u - ((*bitpointer) & 7u))));
    else         bitstream[(*bitpointer) >> 3u] |= (1u << (7u - ((*bitpointer) & 7u)));
    ++(*bitpointer);
}

void addPaddingBits(unsigned char* out, const unsigned char* in, PXSize olinebits, PXSize ilinebits, PXSize h)
{
    // The opposite of the removePaddingBits function olinebits must be >= ilinebits

    PXSize diff = olinebits - ilinebits;
    PXSize obp = 0, ibp = 0; // bit pointers

    for(PXSize y = 0; y != h; ++y)
    {
        for(PXSize x = 0; x < ilinebits; ++x)
        {
            unsigned char bit = readBitFromReversedStream(&ibp, in);
            setBitOfReversedStream(&obp, out, bit);
        }

        // obp += diff; --> no, fill in some value in the padding bits too, to avoid
        // "Use of uninitialised value of size ###" warning from valgrind
        for(PXSize x = 0; x != diff; ++x)
        {
            setBitOfReversedStream(&obp, out, 0);
        }
    }
}




// out must be buffer big enough to contain uncompressed IDAT chunk data, and in must contain the full image. return value is error
PXSize preProcessScanlines
(
    PXPNGInterlaceMethod interlaceMethod,
    PXSize width,
    PXSize height,
    PXSize bpp,
    PXPNGColorType colorType,
    PXSize bitDepth,
    PXFile* const pxScanlineStream,
    const unsigned char* in
)
{

    /*
    This function converts the pure 2D image with the PNG's colortype, into filtered-padded-interlaced data. Steps:
    *) if no Adam7: 1) add padding bits (= possible extra bits per scanline if bpp < 8) 2) filter
    *) if adam7: 1) Adam7_interlace 2) 7x add padding bits 3) 7x filter
    */
    unsigned error = 0;

    switch(interlaceMethod)
    {
        case PXPNGInterlaceNone:
        {
            const PXSize outsize = height + (height * ((width * bpp + 7u) / 8u)); /*image size plus an extra byte per scanline + possible padding bits*/
            const PXActionResult allocationResult = PXFileMapToMemory(pxScanlineStream, outsize, PXAccessModeReadAndWrite);

            PXActionReturnOnError(allocationResult);

            // non multiple of 8 bits per scanline, padding bits needed per scanline
            const PXBool paddingBitsNeeded = bpp < 8 && width * bpp != ((width * bpp + 7u) / 8u) * 8u;

            if(paddingBitsNeeded)
            {
                PXByte* padded;

                const PXSize size = height * ((width * bpp + 7u) / 8u);
                padded = PXMemoryHeapCallocT(PXByte, size);

                if(!padded)
                    error = 83; /*alloc fail*/

                if(!error)
                {
                    addPaddingBits(padded, in, ((width * bpp + 7u) / 8u) * 8u, width * bpp, height);
                    error = filter(pxScanlineStream->Data, padded, width, height, bpp, LFS_MINSUM);
                }                

                PXMemoryHeapFree(PXNull, padded);
            }
            else
            {
                // we can immediately filter into the out buffer, no other steps needed
                error = filter(pxScanlineStream->Data, in, width, height, bpp, LFS_MINSUM);
            }

            break;
        }
        case PXPNGInterlaceADAM7:
        {
            unsigned passw[7], passh[7];
            PXSize filter_passstart[8], padded_passstart[8], passstart[8];

            PXADAM7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, width, height, bpp);

            const PXSize outsize = filter_passstart[7]; // image size plus an extra byte per scanline + possible padding bits
            const PXActionResult allocationResult = PXFileMapToMemory(pxScanlineStream, outsize, PXAccessModeReadAndWrite);

            PXActionReturnOnError(allocationResult);

            PXByte* adam7 = PXMemoryHeapCallocT(PXByte, passstart[7]);

            if(!adam7 && passstart[7])
                error = 83; //alloc fail

            if(!error)
            {
                //Adam7_interlace(adam7, in, png->ImageHeader.Width, png->ImageHeader.Height, bpp);
                for(PXInt8U i = 0; i != 7u; ++i)
                {
                    if(bpp < 8)
                    {
                        const PXSize newSize = padded_passstart[i + 1] - padded_passstart[i];
                        PXByte* padded = PXMemoryHeapCallocT(PXByte, newSize);

                        //  if(!padded) ERROR_BREAK(83); //alloc fail
                        addPaddingBits(padded, &adam7[passstart[i]], ((passw[i] * bpp + 7u) / 8u) * 8u, passw[i] * bpp, passh[i]);
                        //  error = filter(&(*out)[filter_passstart[i]], padded, passw[i], passh[i], &info_png->color, settings);
                                                
                        PXMemoryHeapFree(PXNull, padded);
                    }
                    else
                    {
                        //error = filter(&(*out)[filter_passstart[i]], &adam7[padded_passstart[i]], passw[i], passh[i], &info_png->color, settings);
                    }

                    if(error)
                        break;
                }
            }

            PXMemoryHeapFree(PXNull, passstart[7]);

            break;
        }
        default:
        case PXPNGInterlaceInvalid:
        {
            break;
        }
    }

    return error;
}

PXActionResult PXAPI PXPNGSaveToFile(PXResourceTransphereInfo* const pxResourceTransphereInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceTransphereInfo->ResourceTarget;

    //---<Signature>--- 8 Bytes
    {
        const PXInt64U pngFileHeader = PXPNGHeaderSequenz;

        PXFileWriteI64U(pxResourceTransphereInfo->FileReference, pngFileHeader);
    }

    //---<IHDR> (Image Header)--- 21 Bytes
    {
        unsigned char colorType = 0;
        const PXInt8U interlaceMethod = PXPNGInterlaceMethodToID(PXPNGInterlaceNone);
        const unsigned char* chunkStart = PXFileCursorPosition(pxResourceTransphereInfo->FileReference);

        const unsigned char compressionMethod = 0;
        const unsigned char filterMethod = 0;

        switch(pxImage->Format)
        {
            case PXColorFormatInvalid:
                return PXActionRefuedFormatIllegal;

            case PXColorFormatA8:
                colorType = PXPNGColorGrayscaleAlpha;
                break;

            case PXColorFormatBGRI8:
            case PXColorFormatRGBI8:
                colorType = PXPNGColorRGB;
                break;

            case PXColorFormatRGBAI8:
            case PXColorFormatBGRAI8:
                colorType = PXPNGColorRGBA;
                break;
        }
        const unsigned int chunkLength = 13u;

        PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, chunkLength, PXEndianBig);
        PXFileWriteB(pxResourceTransphereInfo->FileReference, "IHDR", 4u);

        PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, pxImage->Width, PXEndianBig);
        PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, pxImage->Height, PXEndianBig);

        {
            const PXInt8U bitDepth = PXColorFormatBitsPerPixel(pxImage->Format);

            PXFileWriteI8U(pxResourceTransphereInfo->FileReference, bitDepth);
        }

        PXFileWriteI8U(pxResourceTransphereInfo->FileReference, colorType);
        PXFileWriteI8U(pxResourceTransphereInfo->FileReference, compressionMethod);
        PXFileWriteI8U(pxResourceTransphereInfo->FileReference, filterMethod);
        PXFileWriteI8U(pxResourceTransphereInfo->FileReference, interlaceMethod);

        {
            const PXInt32U crc = PXCRC32Generate(chunkStart + 4, chunkLength + 4);

            PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, crc, PXEndianBig);
        }



        // Header End

        // [PLTE] Palette

        // 0 = NoShow
        // 1 = Maybe
        // 2 = definitly
        unsigned char shouldPrint = 0;

        switch(colorType)
        {
            default:
            case PXPNGColorInvalid:
                return PXActionInvalid;

            case PXPNGColorGrayscale: // ColorType = 0
            case PXPNGColorGrayscaleAlpha:  // ColorType = 4
                shouldPrint = 0;
                break;

            case PXPNGColorRGB:  // ColorType = 2
            case PXPNGColorRGBA:  // ColorType = 6
                shouldPrint = 1;
                break;

            case PXPNGColorPalette:  // ColorType = 3;
                shouldPrint = 2;
                break;
        }
    }

    // [tRNS] Transparency
    {

    }

    // [gAMA] Image gamma
    {

    }

    // [cHRM] -
    {

    }
    // [sRGB] -
    {

    }
    // [iCCP] -
    {

    }

    // [tEXt] -
    {

    }

    // [zTXt] -
    {

    }

    // [iTXt] -
    {

    }

    // [bKGD] -
    {

    }

    // [pHYs] -
    {

    }

    // [sBIT] -
    {

    }

    // [sPLT] -
    {

    }

    // [hIST] -
    {

    }

#if 1

    // [tIME] - 19 Bytes
    {
        PXTime time;
        PXPNGLastModificationTime pngLastModificationTime;

        PXTimeNow(&time);

        pngLastModificationTime.Year = time.Year;
        pngLastModificationTime.Month = time.Month;
        pngLastModificationTime.Day = time.Day;
        pngLastModificationTime.Hour = time.Hour;
        pngLastModificationTime.Minute = time.Minute;
        pngLastModificationTime.Second = time.Second;

        const unsigned char* chunkStart = PXFileCursorPosition(pxResourceTransphereInfo->FileReference);
        const PXSize chunkLength = 7u;

        const PXTypeEntry pxFileDataElementType[] =
        {
            {&chunkLength, PXTypeInt32UBE},
            {"tIME", PXTypeDatax4},
            {&pngLastModificationTime.Year, PXTypeInt16UBE},
            {&pngLastModificationTime.Month, PXTypeInt08U},
            {&pngLastModificationTime.Day, PXTypeInt08U},
            {&pngLastModificationTime.Hour, PXTypeInt08U},
            {&pngLastModificationTime.Minute, PXTypeInt08U},
            {&pngLastModificationTime.Second, PXTypeInt08U},
        };

        PXFileWriteMultible(pxResourceTransphereInfo->FileReference, pxFileDataElementType, sizeof(pxFileDataElementType));

        {
            const PXInt32U crc = PXCRC32Generate(chunkStart + 4, chunkLength + 4);

            PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, crc, PXEndianBig);
        }
    }
#endif

    // [IDAT] Image data
    {
        const PXSize offsetSizeofChunk = pxResourceTransphereInfo->FileReference->DataCursor;

        const void* chunkStart = PXFileCursorPosition(pxResourceTransphereInfo->FileReference);

        PXSize chunkLength = 0;

        PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, 0u, PXEndianBig); // Length
        PXFileWriteB(pxResourceTransphereInfo->FileReference, "IDAT", 4u);

        PXFile pxScanlineStream;
        // PXFileConstruct(&pxScanlineStream);

        // Preprocess scanlines
        {
            // check if colormodes are equal
            if(0)
            {

            }
            else
            {
                preProcessScanlines
                (
                    PXPNGInterlaceNone,
                    pxImage->Width,
                    pxImage->Height,
                    PXColorFormatBitsPerPixel(pxImage->Format),
                    PXPNGColorRGB,
                    8,
                    &pxScanlineStream,
                    pxImage->PixelData
                );
            }
        }

        // PXZLIB
        {
            PXSize cursorBefore = pxResourceTransphereInfo->FileReference->DataCursor;

            const PXActionResult PXZLIBResult = PXZLIBCompress(&pxScanlineStream, pxResourceTransphereInfo->FileReference);

            chunkLength = pxResourceTransphereInfo->FileReference->DataCursor - cursorBefore;

            PXFileWriteAtI32UE(pxResourceTransphereInfo->FileReference, chunkLength, PXEndianBig, offsetSizeofChunk); // override length
        }

        PXFileClose(&pxScanlineStream);

        {
            const PXInt32U crc = PXCRC32Generate(&((PXByte*)chunkStart)[4], chunkLength + 4);

            PXFileWriteI32UE(pxResourceTransphereInfo->FileReference, crc, PXEndianBig);
        }
    }

    //---<IEND>---------------------------------------------------------------- 12 Bytes
    {
        const PXInt8U imageEndChunk[8] = { 'I','E','N','D','\xAE','\x42','\x60','\x82' }; // Combined write, as this is constand

        const PXTypeEntry pxFileDataElementType[] =
        {
            {PXNull, PXTypePadding(4)},
            {(void*)imageEndChunk, sizeof(imageEndChunk)}
        };

        PXFileWriteMultible(pxResourceTransphereInfo->FileReference, pxFileDataElementType, sizeof(pxFileDataElementType));
    }

    return PXActionSuccessful;
}

































































unsigned int color_tree_add(PNGColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned index)
{
    for(PXInt8U bit = 0; bit < 8; ++bit)
    {
        const int index = 8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1) + 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1);
        if(!tree->children[index])
        {
            tree->children[index] = PXMemoryHeapCallocT(PNGColorTree, 1);

            if(!tree->children[index])
                return 83; // alloc fail
        }
        tree = tree->children[index];
    }
    tree->index = (int)index;
    return 0;
}

PXActionResult PXAPI PXPNGImageDataDecompress(const PXPNG* const png, const void* pixelDataIn, void* pixelDataOut, unsigned char bitDepth, PXPNGColorType colorType)
{
    LodePNGColorMode colorModeIn;
    LodePNGColorMode colorModeOut;

    PXMemoryClear(&colorModeIn, sizeof(LodePNGColorMode));
    PXMemoryClear(&colorModeOut, sizeof(LodePNGColorMode));

    colorModeIn.bitdepth = bitDepth;
    colorModeIn.colortype = LCT_RGBA;

    colorModeOut.bitdepth = 8;
    colorModeOut.colortype = LCT_RGBA;

    switch(colorType)
    {
        default:
        case PXPNGColorInvalid:
            return PXActionRefusedArgumentInvalid;

        case PXPNGColorGrayscale:
            colorModeIn.colortype = LCT_GREY;
            break;

        case PXPNGColorRGB:
            colorModeIn.colortype = LCT_RGB;
            colorModeOut.colortype = LCT_RGB;
            break;

        case PXPNGColorPalette:
            colorModeIn.colortype = LCT_PALETTE;
            colorModeOut.colortype = LCT_RGBA;
            break;

        case PXPNGColorGrayscaleAlpha:
            colorModeIn.colortype = LCT_GREY_ALPHA;
            break;

        case PXPNGColorRGBA:
            colorModeIn.colortype = LCT_RGBA;
            colorModeOut.colortype = LCT_RGBA;
            break;
    }

    //colorModeOut.bitdepth = colorModeIn.bitdepth;
    // colorModeOut.colortype = colorModeIn.colortype;

    PNGColorTree tree;
    const PXSize width = png->ImageHeader.Width;
    const PXSize height = png->ImageHeader.Height;
    const PXSize numpixels = width * height;
    unsigned error = 0;

    colorModeIn.palettesize = png->PaletteSize;
    colorModeIn.palette = png->Palette;

    if(colorModeIn.colortype == LCT_PALETTE && !colorModeIn.palette)
    {
        return PXActionRefusedArgumentInvalid; /* error: must provide palette if input mode is palette */
    }

    if(lodepng_color_mode_equal(&colorModeOut, &colorModeIn))
    {
        PXSize numbytes = lodepng_get_raw_size(width, height, &colorModeIn);
        PXMemoryCopy(pixelDataIn, numbytes, pixelDataOut, numbytes);
        return PXActionSuccessful;
    }

    if(colorModeOut.colortype == LCT_PALETTE)
    {
        PXSize palettesize = colorModeOut.palettesize;
        const unsigned char* palette = colorModeOut.palette;
        PXSize palsize = (PXSize)1u << colorModeOut.bitdepth;
        // if the user specified output palette but did not give the values, assume
        // they want the values of the input color type (assuming that one is palette).
        // Note that we never create a new palette ourselves.
        if(palettesize == 0)
        {
            palettesize = colorModeIn.palettesize;
            palette = colorModeIn.palette;
            // if the input was also palette with same bitdepth, then the color types are also
            // equal, so copy literally. This to preserve the exact indices that were in the PNG
            // even in case there are duplicate colors in the palette.
            if(colorModeIn.colortype == LCT_PALETTE && colorModeIn.bitdepth == colorModeOut.bitdepth)
            {
                const PXSize numbytes = lodepng_get_raw_size(width, height, &colorModeIn);

                PXMemoryCopy(pixelDataIn, numbytes, pixelDataOut, numbytes);

                return PXActionSuccessful;
            }
        }
        if(palettesize < palsize)
            palsize = palettesize;

        // color_tree_init(&tree);

        for(PXSize i = 0; i != palsize; ++i)
        {
            const unsigned char* p = &palette[i * 4];
            error = color_tree_add(&tree, p[0], p[1], p[2], p[3], i);
            if(error) break;
        }
    }

    if(!error)
    {
        if(colorModeIn.bitdepth == 16 && colorModeOut.bitdepth == 16)
        {
            for(PXSize i = 0; i != numpixels; ++i)
            {
                unsigned short r = 0, g = 0, b = 0, a = 0;
                getPixelColorRGBA16(&r, &g, &b, &a, pixelDataIn, i, &colorModeIn);
                rgba16ToPixel(pixelDataOut, i, &colorModeOut, r, g, b, a);
            }
        }
        else if(colorModeOut.bitdepth == 8 && colorModeOut.colortype == LCT_RGBA)
        {
            getPixelColorsRGBA8(pixelDataOut, numpixels, pixelDataIn, &colorModeIn);
        }
        else if(colorModeOut.bitdepth == 8 && colorModeOut.colortype == LCT_RGB)
        {
            getPixelColorsRGB8(pixelDataOut, numpixels, pixelDataIn, &colorModeIn);
        }
        else
        {
            PXColorRGBAI8 color;

            for(PXSize i = 0; i != numpixels; ++i)
            {
                getPixelColorRGBA8(&color, pixelDataIn, i, &colorModeIn);

                const PXActionResult result = rgba8ToPixel(pixelDataOut, i, &colorModeOut, &tree, &color);

                PXActionReturnOnError(result);
            }
        }
    }

    /*
    if (colorModeOut.colortype == LCT_PALETTE)
    {
       // color_tree_cleanup(&tree);
    }*/

    return error;
}

unsigned getNumColorChannels(LodePNGColorType colortype)
{
    switch(colortype)
    {
        case LCT_GREY:
            return 1;
        case LCT_RGB:
            return 3;
        case LCT_PALETTE:
            return 1;
        case LCT_GREY_ALPHA:
            return 2;
        case LCT_RGBA:
            return 4;
        case LCT_MAX_OCTET_VALUE:
            return 0; /* invalid color type */
        default:
            return 0; /*invalid color type*/
    }
}

PXSize lodepng_get_bpp_lct(LodePNGColorType colortype, PXSize bitdepth)
{
    /*bits per pixel is amount of channels * bits per channel*/
    return getNumColorChannels(colortype) * bitdepth;
}

int lodepng_color_mode_equal(const LodePNGColorMode* a, const LodePNGColorMode* b)
{
    PXSize i;
    if(a->colortype != b->colortype) return 0;
    if(a->bitdepth != b->bitdepth) return 0;
    if(a->key_defined != b->key_defined) return 0;
    if(a->key_defined)
    {
        if(a->key_r != b->key_r) return 0;
        if(a->key_g != b->key_g) return 0;
        if(a->key_b != b->key_b) return 0;
    }
    if(a->palettesize != b->palettesize) return 0;
    for(i = 0; i != a->palettesize * 4; ++i)
    {
        if(a->palette[i] != b->palette[i]) return 0;
    }
    return 1;
}

void getPixelColorRGBA16(unsigned short* r, unsigned short* g, unsigned short* b, unsigned short* a, const unsigned char* in, PXSize i, const LodePNGColorMode* mode)
{
    if(mode->colortype == LCT_GREY)
    {
        *r = *g = *b = 256 * in[i * 2 + 0] + in[i * 2 + 1];
        if(mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r) *a = 0;
        else *a = 65535;
    }
    else if(mode->colortype == LCT_RGB)
    {
        *r = 256u * in[i * 6 + 0] + in[i * 6 + 1];
        *g = 256u * in[i * 6 + 2] + in[i * 6 + 3];
        *b = 256u * in[i * 6 + 4] + in[i * 6 + 5];
        if(mode->key_defined
           && 256u * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
           && 256u * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
           && 256u * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b) *a = 0;
        else *a = 65535;
    }
    else if(mode->colortype == LCT_GREY_ALPHA)
    {
        *r = *g = *b = 256u * in[i * 4 + 0] + in[i * 4 + 1];
        *a = 256u * in[i * 4 + 2] + in[i * 4 + 3];
    }
    else if(mode->colortype == LCT_RGBA)
    {
        *r = 256u * in[i * 8 + 0] + in[i * 8 + 1];
        *g = 256u * in[i * 8 + 2] + in[i * 8 + 3];
        *b = 256u * in[i * 8 + 4] + in[i * 8 + 5];
        *a = 256u * in[i * 8 + 6] + in[i * 8 + 7];
    }
}

void rgba16ToPixel(unsigned char* out, PXSize i, const LodePNGColorMode* mode, unsigned short r, unsigned short g, unsigned short b, unsigned short a)
{
    if(mode->colortype == LCT_GREY)
    {
        unsigned short gray = r; /*((unsigned)r + g + b) / 3u;*/
        out[i * 2 + 0] = (gray >> 8) & 255;
        out[i * 2 + 1] = gray & 255;
    }
    else if(mode->colortype == LCT_RGB)
    {
        out[i * 6 + 0] = (r >> 8) & 255;
        out[i * 6 + 1] = r & 255;
        out[i * 6 + 2] = (g >> 8) & 255;
        out[i * 6 + 3] = g & 255;
        out[i * 6 + 4] = (b >> 8) & 255;
        out[i * 6 + 5] = b & 255;
    }
    else if(mode->colortype == LCT_GREY_ALPHA)
    {
        unsigned short gray = r; /*((unsigned)r + g + b) / 3u;*/
        out[i * 4 + 0] = (gray >> 8) & 255;
        out[i * 4 + 1] = gray & 255;
        out[i * 4 + 2] = (a >> 8) & 255;
        out[i * 4 + 3] = a & 255;
    }
    else if(mode->colortype == LCT_RGBA)
    {
        out[i * 8 + 0] = (r >> 8) & 255;
        out[i * 8 + 1] = r & 255;
        out[i * 8 + 2] = (g >> 8) & 255;
        out[i * 8 + 3] = g & 255;
        out[i * 8 + 4] = (b >> 8) & 255;
        out[i * 8 + 5] = b & 255;
        out[i * 8 + 6] = (a >> 8) & 255;
        out[i * 8 + 7] = a & 255;
    }
}

void getPixelColorsRGBA8(unsigned char* buffer, PXSize numpixels, const unsigned char* in, const LodePNGColorMode* mode)
{
    unsigned num_channels = 4;
    PXSize i;
    if(mode->colortype == LCT_GREY)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i];
                buffer[3] = 255;
            }
            if(mode->key_defined)
            {
                buffer -= numpixels * num_channels;
                for(i = 0; i != numpixels; ++i, buffer += num_channels)
                {
                    if(buffer[0] == mode->key_r) buffer[3] = 0;
                }
            }
        }
        else if(mode->bitdepth == 16)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i * 2];
                buffer[3] = mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r ? 0 : 255;
            }
        }
        else
        {
            unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
            PXSize j = 0;
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
                buffer[0] = buffer[1] = buffer[2] = (value * 255) / highest;
                buffer[3] = mode->key_defined && value == mode->key_r ? 0 : 255;
            }
        }
    }
    else if(mode->colortype == LCT_RGB)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                PXMemoryCopy(&in[i * 3], 3, buffer, 3);
                buffer[3] = 255;
            }
            if(mode->key_defined)
            {
                buffer -= numpixels * num_channels;
                for(i = 0; i != numpixels; ++i, buffer += num_channels)
                {
                    if(buffer[0] == mode->key_r && buffer[1] == mode->key_g && buffer[2] == mode->key_b) buffer[3] = 0;
                }
            }
        }
        else
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = in[i * 6 + 0];
                buffer[1] = in[i * 6 + 2];
                buffer[2] = in[i * 6 + 4];
                buffer[3] = mode->key_defined
                    && 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
                    && 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
                    && 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b ? 0 : 255;
            }
        }
    }
    else if(mode->colortype == LCT_PALETTE)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                unsigned index = in[i];
                /*out of bounds of palette not checked: see lodepng_color_mode_alloc_palette.*/
                PXMemoryCopy(&mode->palette[index * 4], 4, buffer, 4u);
            }
        }
        else
        {
            PXSize j = 0;
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                unsigned index = readBitsFromReversedStream(&j, in, mode->bitdepth);
                /*out of bounds of palette not checked: see lodepng_color_mode_alloc_palette.*/
                PXMemoryCopy(&mode->palette[index * 4], 4, buffer, 4u);
            }
        }
    }
    else if(mode->colortype == LCT_GREY_ALPHA)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i * 2 + 0];
                buffer[3] = in[i * 2 + 1];
            }
        }
        else
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i * 4 + 0];
                buffer[3] = in[i * 4 + 2];
            }
        }
    }
    else if(mode->colortype == LCT_RGBA)
    {
        if(mode->bitdepth == 8)
        {
            const PXSize size = numpixels * 4u;

            PXMemoryCopy(in, size, buffer, size);
        }
        else
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = in[i * 8 + 0];
                buffer[1] = in[i * 8 + 2];
                buffer[2] = in[i * 8 + 4];
                buffer[3] = in[i * 8 + 6];
            }
        }
    }
}

void getPixelColorsRGB8(unsigned char* buffer, PXSize numpixels, const unsigned char* in, const LodePNGColorMode* mode)
{
    const unsigned num_channels = 3;
    PXSize i;
    if(mode->colortype == LCT_GREY)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i];
            }
        }
        else if(mode->bitdepth == 16)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i * 2];
            }
        }
        else
        {
            unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
            PXSize j = 0;
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
                buffer[0] = buffer[1] = buffer[2] = (value * 255) / highest;
            }
        }
    }
    else if(mode->colortype == LCT_RGB)
    {
        if(mode->bitdepth == 8)
        {
            PXSize size = numpixels * 3;

            PXMemoryCopy(in, size, buffer, size);
        }
        else
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = in[i * 6 + 0];
                buffer[1] = in[i * 6 + 2];
                buffer[2] = in[i * 6 + 4];
            }
        }
    }
    else if(mode->colortype == LCT_PALETTE)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                unsigned index = in[i];
                /*out of bounds of palette not checked: see lodepng_color_mode_alloc_palette.*/

                const PXSize size = 3;
                PXMemoryCopy(&mode->palette[index * 4], size, buffer, size);
            }
        }
        else
        {
            PXSize j = 0;
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                unsigned index = readBitsFromReversedStream(&j, in, mode->bitdepth);
                /*out of bounds of palette not checked: see lodepng_color_mode_alloc_palette.*/

                const PXSize size = 3;
                PXMemoryCopy(&mode->palette[index * 4], size, buffer, size);
            }
        }
    }
    else if(mode->colortype == LCT_GREY_ALPHA)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i * 2 + 0];
            }
        }
        else
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = buffer[1] = buffer[2] = in[i * 4 + 0];
            }
        }
    }
    else if(mode->colortype == LCT_RGBA)
    {
        if(mode->bitdepth == 8)
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                const PXSize size = 3;

                PXMemoryCopy(&in[i * 4], size, buffer, size);
            }
        }
        else
        {
            for(i = 0; i != numpixels; ++i, buffer += num_channels)
            {
                buffer[0] = in[i * 8 + 0];
                buffer[1] = in[i * 8 + 2];
                buffer[2] = in[i * 8 + 4];
            }
        }
    }
}

void getPixelColorRGBA8(PXColorRGBAI8* const color, const unsigned char* in, PXSize i, const LodePNGColorMode* mode)
{
    if(mode->colortype == LCT_GREY)
    {
        if(mode->bitdepth == 8)
        {
            color->Red = in[i];
            color->Green = color->Red;
            color->Blue = color->Red;

            if(mode->key_defined && color->Red == mode->key_r)
                color->Alpha = 0;
            else
                color->Alpha = 255;
        }
        else if(mode->bitdepth == 16)
        {
            color->Red = in[i * 2 + 0];
            color->Green = color->Red;
            color->Blue = color->Red;

            if(mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r)
                color->Alpha = 0;
            else
                color->Alpha = 255;
        }
        else
        {
            PXSize highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
            PXSize j = i * mode->bitdepth;
            PXSize value = readBitsFromReversedStream(&j, in, mode->bitdepth);

            color->Red = (value * 255) / highest;
            color->Green = color->Red;
            color->Blue = color->Red;

            if(mode->key_defined && value == mode->key_r)
                color->Alpha = 0;

            else
                color->Alpha = 255;
        }
    }
    else if(mode->colortype == LCT_RGB)
    {
        if(mode->bitdepth == 8)
        {
            color->Red = in[i * 3 + 0];
            color->Green = in[i * 3 + 1];
            color->Blue = in[i * 3 + 2];

            if(mode->key_defined && color->Red == mode->key_r && color->Green == mode->key_g && color->Blue == mode->key_b)
                color->Alpha = 0;
            else
                color->Alpha = 255;
        }
        else
        {
            color->Red = in[i * 6 + 0];
            color->Green = in[i * 6 + 2];
            color->Blue = in[i * 6 + 4];

            if(mode->key_defined && 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
               && 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
               && 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b)
                color->Alpha = 0;

            else
                color->Alpha = 255;
        }
    }
    else if(mode->colortype == LCT_PALETTE)
    {
        unsigned index;
        if(mode->bitdepth == 8)
        {
            index = in[i];
        }
        else
        {
            PXSize j = i * mode->bitdepth;
            index = readBitsFromReversedStream(&j, in, mode->bitdepth);
        }
        /*out of bounds of palette not checked: see lodepng_color_mode_alloc_palette.*/
        color->Red = mode->palette[index * 4 + 0];
        color->Green = mode->palette[index * 4 + 1];
        color->Blue = mode->palette[index * 4 + 2];
        color->Alpha = mode->palette[index * 4 + 3];
    }
    else if(mode->colortype == LCT_GREY_ALPHA)
    {
        if(mode->bitdepth == 8)
        {
            color->Red = in[i * 2 + 0];
            color->Green = color->Red;
            color->Blue = color->Red;
            color->Alpha = in[i * 2 + 1];
        }
        else
        {
            color->Red = in[i * 4 + 0];
            color->Green = color->Red;
            color->Blue = color->Red;
            color->Alpha = in[i * 4 + 2];
        }
    }
    else if(mode->colortype == LCT_RGBA)
    {
        if(mode->bitdepth == 8)
        {
            color->Red = in[i * 4 + 0];
            color->Green = in[i * 4 + 1];
            color->Blue = in[i * 4 + 2];
            color->Alpha = in[i * 4 + 3];
        }
        else
        {
            color->Red = in[i * 8 + 0];
            color->Green = in[i * 8 + 2];
            color->Blue = in[i * 8 + 4];
            color->Alpha = in[i * 8 + 6];
        }
    }
}

PXActionResult PXAPI rgba8ToPixel
(
    unsigned char* out,
    PXSize i,
    const LodePNGColorMode* mode,
    PNGColorTree* tree,
    const PXColorRGBAI8* const color
)
{
    switch(mode->colortype)
    {
        case LCT_GREY:
        {
            unsigned char gray = color->Red; /*((unsigned short)r + g + b) / 3u;*/
            if(mode->bitdepth == 8) out[i] = gray;
            else if(mode->bitdepth == 16) out[i * 2 + 0] = out[i * 2 + 1] = gray;
            else
            {
                /*take the most significant bits of gray*/
                gray = ((unsigned)gray >> (8u - mode->bitdepth)) & ((1u << mode->bitdepth) - 1u);
                addColorBits(out, i, mode->bitdepth, gray);
            }
            break;
        }
        case LCT_RGB:
        {
            if(mode->bitdepth == 8)
            {
                out[i * 3 + 0] = color->Red;
                out[i * 3 + 1] = color->Green;
                out[i * 3 + 2] = color->Blue;
            }
            else
            {
                out[i * 6 + 0] = out[i * 6 + 1] = color->Red;
                out[i * 6 + 2] = out[i * 6 + 3] = color->Green;
                out[i * 6 + 4] = out[i * 6 + 5] = color->Blue;
            }
            break;
        }
        case LCT_PALETTE:
        {
            int index = color_tree_get(tree, color);

            if(index < 0)
                return PXActionInvalid; // color not in palette

            if(mode->bitdepth == 8)
                out[i] = index;
            else
                addColorBits(out, i, mode->bitdepth, (unsigned)index);

            break;
        }
        case LCT_GREY_ALPHA:
        {
            unsigned char gray = color->Red; /*((unsigned short)r + g + b) / 3u;*/
            if(mode->bitdepth == 8)
            {
                out[i * 2 + 0] = gray;
                out[i * 2 + 1] = color->Alpha;
            }
            else if(mode->bitdepth == 16)
            {
                out[i * 4 + 0] = out[i * 4 + 1] = gray;
                out[i * 4 + 2] = out[i * 4 + 3] = color->Alpha;
            }
            break;
        }
        case LCT_RGBA:
        {
            if(mode->bitdepth == 8)
            {
                out[i * 4 + 0] = color->Red;
                out[i * 4 + 1] = color->Green;
                out[i * 4 + 2] = color->Blue;
                out[i * 4 + 3] = color->Alpha;
            }
            else
            {
                out[i * 8 + 0] = out[i * 8 + 1] = color->Red;
                out[i * 8 + 2] = out[i * 8 + 3] = color->Green;
                out[i * 8 + 4] = out[i * 8 + 5] = color->Blue;
                out[i * 8 + 6] = out[i * 8 + 7] = color->Alpha;
            }
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXSize lodepng_get_raw_size_lct(PXSize w, PXSize h, LodePNGColorType colortype, PXSize bitdepth)
{
    PXSize bpp = lodepng_get_bpp_lct(colortype, bitdepth);
    PXSize n = w * h;

    return ((n / 8u) * bpp) + ((n & 7u) * bpp + 7u) / 8u;
}

PXSize lodepng_get_raw_size(PXSize w, PXSize h, const LodePNGColorMode* color)
{
    return lodepng_get_raw_size_lct(w, h, color->colortype, color->bitdepth);
}

int color_tree_get(PNGColorTree* tree, const PXColorRGBAI8* const color)
{
    for(PXInt8U bit = 0; bit < 8u; ++bit)
    {
        int i =
            8 * ((color->Red >> bit) & 1) +
            4 * ((color->Green >> bit) & 1) +
            2 * ((color->Blue >> bit) & 1) +
            1 * ((color->Alpha >> bit) & 1);

        if(!tree->children[i])
            return -1;

        else
            tree = tree->children[i];
    }

    return tree ? tree->index : -1;
}

void addColorBits(unsigned char* out, PXSize index, unsigned int bits, unsigned int in)
{
    unsigned int m = bits == 1 ? 7 : bits == 2 ? 3 : 1; /*8 / bits - 1*/
    /*p = the partial index in the byte, e.g. with 4 palettebits it is 0 for first half or 1 for second half*/
    unsigned int p = index & m;

    in &= (1u << bits) - 1u; /*filter out any other bits of the input value*/
    in = in << (bits * (m - p));

    if(p == 0)
    {
        out[index * bits / 8u] = in;
    }
    else
    {
        out[index * bits / 8u] |= in;
    }
}

unsigned char readBitFromReversedStream(PXSize* bitpointer, const char* bitstream)
{
    unsigned char result = (unsigned char)((bitstream[(*bitpointer) >> 3] >> (7 - ((*bitpointer) & 0x7))) & 1);
    ++(*bitpointer);
    return result;
}

unsigned readBitsFromReversedStream(PXSize* bitpointer, const unsigned char* bitstream, PXSize nbits)
{
    unsigned int result = 0;

    for(PXSize i = 0; i < nbits; ++i)
    {
        result <<= 1u;
        result |= (unsigned int)readBitFromReversedStream(bitpointer, bitstream);
    }

    return result;
}
