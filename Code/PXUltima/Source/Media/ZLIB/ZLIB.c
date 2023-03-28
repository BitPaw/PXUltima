#include "ZLIB.h"

#include <Container/ClusterValue.h>
#include <Math/PXMath.h>
#include <File/PXDataStream.h>
#include <Media/DEFLATE/DEFLATE.h>
#include <Media/ADLER/Adler32.h>

ZLIBCompressionLevel ConvertToCompressionLevel(const unsigned char compressionLevel)
{
    switch(compressionLevel)
    {
        case 0u:
            return ZLIBCompressionLevelFastest;

        case 1u:
            return ZLIBCompressionLevelFast;

        case 2u:
            return ZLIBCompressionLevelDefault;

        case 3u:
            return ZLIBCompressionLevelSlowest;

        default:
            return ZLIBCompressionLevelInvalid;
    }
}

unsigned char ConvertFromCompressionLevel(const ZLIBCompressionLevel compressionLevel)
{
    switch(compressionLevel)
    {
        default:
        case ZLIBCompressionLevelInvalid:
            return -1;

        case ZLIBCompressionLevelDefault:
            return 2u;

        case ZLIBCompressionLevelSlowest:
            return 3u;

        case ZLIBCompressionLevelFast:
            return 1u;

        case ZLIBCompressionLevelFastest:
            return 0u;
    }
}

ZLIBCompressionMethod ConvertToCompressionMethod(const unsigned char compressionMethod)
{
    switch(compressionMethod)
    {
        case 8u:
            return ZLIBCompressionMethodDeflate;

        case 15u:
            return ZLIBCompressionMethodReserved;

        default:
            return ZLIBCompressionMethodInvalid;
    }
}

unsigned char ConvertFromCompressionMethod(const ZLIBCompressionMethod compressionMethod)
{
    switch(compressionMethod)
    {
        default:
        case ZLIBCompressionMethodInvalid:
            return -1;

        case ZLIBCompressionMethodDeflate:
            return 8u;

        case ZLIBCompressionMethodReserved:
            return 15u;
    }
}

PXActionResult ZLIBDecompress(PXDataStream* const pxInputSteam, PXDataStream* const pxOutputSteam)
{
    ZLIB zlib;

    const PXSize headerSize = 2u;
    const PXSize adlerSize = 4u;

    // Parse header ->     Header.Parse(compressionFormatByte, flagByte);
    {
        unsigned char compressionFormatByte = 0;
        unsigned char flagByte = 0;

        PXDataStreamReadI8U(pxInputSteam, &compressionFormatByte);
        PXDataStreamReadI8U(pxInputSteam, &flagByte);

        // Valid Check
        {
            const PXBool validFlags = MakeShortBE(compressionFormatByte, flagByte) % 31u == 0;

            if(!validFlags)
            {
                return PXActionRefusedInvalidHeaderSignature;// assert(validFlags);
            }
        }

        //---<Parse First Byte__ - Compression Info>---------------------------------
        {
            unsigned char compressionMethodValue = (compressionFormatByte & 0b00001111);

            zlib.Header.CompressionMethod = ConvertToCompressionMethod(compressionMethodValue);
            zlib.Header.CompressionInfo = (compressionFormatByte & 0b11110000) >> 4;

            // log_2(WindowSize) - 8 = CompressionInfo
            // log_2(32768) - 8 = 7
            // 2^(CompressionInfo + 8)

            const PXBool isCompressionInfoValid = zlib.Header.CompressionInfo <= 7u;

            if(!isCompressionInfoValid)
            {
                return PXActionRefusedInvalidVersion;
            }

            //assert(isCompressionInfoValid);

            zlib.Header.WindowSize = MathPower(2, zlib.Header.CompressionInfo + 8u);
        }
        //-------------------------------------------------------------------------

        //---<Parse Second Byte__ - Flags>-------------------------------------------
        {
            unsigned char compressionLevelValue = (flagByte & 0b11000000) >> 6;

            zlib.Header.CheckFlag = (flagByte & 0b00011111);
            zlib.Header.PXDictionaryPresent = ((flagByte & 0b00100000) >> 5) == 1;
            zlib.Header.CompressionLevel = ConvertToCompressionLevel(compressionLevelValue);
        }
        //-------------------------------------------------------------------------        
    }


    //---<PXDictionary Parse>----------------------------------------------------
    if(zlib.Header.PXDictionaryPresent)
    {
        // Parse DICT dictionary identifier 

    }
    //-------------------------------------------------------------------------

    /* Allocate
    if (!(*outputData))
    {
        (*outputData) = (unsigned char*)malloc(zLIBHeader.WindowSize * 4 * sizeof(unsigned char));
    }*/


    zlib.CompressedDataSize = PXDataStreamRemainingSize(pxInputSteam) - adlerSize;
    zlib.CompressedData = PXDataStreamCursorPosition(pxInputSteam);

    switch(zlib.Header.CompressionMethod)
    {
        case ZLIBCompressionMethodDeflate:
        {
            const PXActionResult deflateResult = DEFLATEParse(pxInputSteam, pxOutputSteam);

            PXActionExitOnError(deflateResult);

            break;
        }
        default:
        case ZLIBCompressionMethodReserved:
        case ZLIBCompressionMethodInvalid:
        {
            return PXActionFailedFormatInvalid;
        }
    }

    PXDataStreamReadI32UE(pxInputSteam, &zlib.AdlerChecksum, EndianBig);

    return PXActionSuccessful;
}

PXActionResult ZLIBCompress(PXDataStream* const pxInputSteam, PXDataStream* const pxOutputSteam)
{
    // Write ZLIB Header
    {
        const PXByte compressionMethod = ConvertFromCompressionMethod(ZLIBCompressionMethodDeflate);
        const PXByte compressionInfo = 7u; // 1-7

        const PXByte dictionary = 0;
        const PXByte level = ConvertFromCompressionLevel(ZLIBCompressionLevelFastest);

        // Byte 1
        // 0b00001111
        // 0b11110000

        // Byte 2
        // 0b11000000
        // 0b00100000

        PXByte buffer[2] = 
        {
            compressionMethod | compressionInfo << 4u,
            (level & 0b11) << 6u | (dictionary & 0b01) << 5u
        };
        
        // Check
        {
            const unsigned short checksum = MakeShortBE(buffer[0], buffer[1]);
            const unsigned char multble = 31-checksum % 31;

            buffer[1] += multble;
        }    

        PXDataStreamWriteB(pxOutputSteam, buffer, 2u);
    }

    // Write DEFLATE
    {
        const PXActionResult delfateResult = DEFLATESerialize(pxInputSteam, pxOutputSteam);

        PXActionExitOnError(delfateResult);
    }

    // Write ADLER
    {
        const unsigned int adler = (unsigned int)Adler32Create(1, pxInputSteam->Data, pxInputSteam->DataSize);

        PXDataStreamWriteI32UE(pxOutputSteam, adler, EndianBig);
    }

    return PXActionSuccessful;
}

PXSize ZLIBCalculateExpectedSize(const PXSize width, const PXSize height, const PXSize bpp, const PNGInterlaceMethod interlaceMethod)
{
    PXSize expected_size = 0;

    switch(interlaceMethod)
    {
        default:
        case PNGInterlaceInvalid:
            break;

        case PNGInterlaceNone:
        {
            // predict output size, to allocate exact size for output buffer to avoid more dynamic allocation.
            // If the decompressed size does not match the prediction, the image must be corrupt.
            expected_size = CalculateRawSizeIDAT(width, height, bpp);
            break;
        }
        case PNGInterlaceADAM7:
        {
            // Adam-7 interlaced: expected size is the sum of the 7 sub-images sizes
            expected_size = 0;
            expected_size += CalculateRawSizeIDAT((width + 7) >> 3, (height + 7) >> 3, bpp);
            if(width > 4) expected_size += CalculateRawSizeIDAT((width + 3) >> 3, (height + 7) >> 3, bpp);
            expected_size += CalculateRawSizeIDAT((width + 3) >> 2, (height + 3) >> 3, bpp);
            if(width > 2) expected_size += CalculateRawSizeIDAT((width + 1) >> 2, (height + 3) >> 2, bpp);
            expected_size += CalculateRawSizeIDAT((width + 1) >> 1, (height + 1) >> 2, bpp);
            if(width > 1) expected_size += CalculateRawSizeIDAT((width + 0) >> 1, (height + 1) >> 1, bpp);
            expected_size += CalculateRawSizeIDAT((width + 0), (height + 0) >> 1, bpp);

            break;
        }
    }

    return expected_size;
}

PXSize CalculateRawSizeIDAT(const PXSize w, const PXSize h, const PXSize bpp)
{
    // + 1 for the filter byte, and possibly plus padding bits per line.
    // Ignoring casts, the expression is equal to (w * bpp + 7) / 8 + 1, but avoids overflow of w * bpp
    return (((w / 8u) * bpp) + 1u + ((w & 7u) * bpp + 7u) / 8u) * h;
}