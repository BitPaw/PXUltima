#include "PXZLIB.h"

#include <Math/PXMath.h>
#include <File/PXDataStream.h>
#include <Media/DEFLATE/DEFLATE.h>
#include <Media/ADLER/Adler32.h>

PXZLIBCompressionLevel ConvertToCompressionLevel(const PXInt8U compressionLevel)
{
    switch(compressionLevel)
    {
        case 0u:
            return PXZLIBCompressionLevelFastest;

        case 1u:
            return PXZLIBCompressionLevelFast;

        case 2u:
            return PXZLIBCompressionLevelDefault;

        case 3u:
            return PXZLIBCompressionLevelSlowest;

        default:
            return PXZLIBCompressionLevelInvalid;
    }
}

unsigned char ConvertFromCompressionLevel(const PXZLIBCompressionLevel compressionLevel)
{
    switch(compressionLevel)
    {
        default:
        case PXZLIBCompressionLevelInvalid:
            return -1;

        case PXZLIBCompressionLevelDefault:
            return 2u;

        case PXZLIBCompressionLevelSlowest:
            return 3u;

        case PXZLIBCompressionLevelFast:
            return 1u;

        case PXZLIBCompressionLevelFastest:
            return 0u;
    }
}

PXZLIBCompressionMethod ConvertToCompressionMethod(const PXInt8U compressionMethod)
{
    switch(compressionMethod)
    {
        case 8u:
            return PXZLIBCompressionMethodDeflate;

        case 15u:
            return PXZLIBCompressionMethodReserved;

        default:
            return PXZLIBCompressionMethodInvalid;
    }
}

unsigned char ConvertFromCompressionMethod(const PXZLIBCompressionMethod compressionMethod)
{
    switch(compressionMethod)
    {
        default:
        case PXZLIBCompressionMethodInvalid:
            return -1;

        case PXZLIBCompressionMethodDeflate:
            return 8u;

        case PXZLIBCompressionMethodReserved:
            return 15u;
    }
}

PXActionResult PXZLIBDecompress(PXDataStream* const pxInputSteam, PXDataStream* const pxOutputSteam)
{
    PXZLIB PXZLIB;

    const PXSize headerSize = 2u;
    const PXSize adlerSize = 4u;

    // Parse header ->     Header.Parse(compressionFormatByte, flagByte);
    {
        PXInt8U compressionFormatByte = 0;
        PXInt8U flagByte = 0;

        PXDataStreamReadI8U(pxInputSteam, &compressionFormatByte);
        PXDataStreamReadI8U(pxInputSteam, &flagByte);

        // Valid Check
        {
            const PXBool validFlags = PXInt16MakeEndianBig(compressionFormatByte, flagByte) % 31u == 0;

            if(!validFlags)
            {
                return PXActionRefusedInvalidHeaderSignature;// assert(validFlags);
            }
        }

        //---<Parse First Byte__ - Compression Info>---------------------------------
        {
            const PXInt8U compressionMethodValue = (compressionFormatByte & 0b00001111);

            PXZLIB.Header.CompressionMethod = ConvertToCompressionMethod(compressionMethodValue);
            PXZLIB.Header.CompressionInfo = (compressionFormatByte & 0b11110000) >> 4;

            // log_2(WindowSize) - 8 = CompressionInfo
            // log_2(32768) - 8 = 7
            // 2^(CompressionInfo + 8)

            const PXBool isCompressionInfoValid = PXZLIB.Header.CompressionInfo <= 7u;

            if(!isCompressionInfoValid)
            {
                return PXActionRefusedInvalidVersion;
            }

            //assert(isCompressionInfoValid);

            PXZLIB.Header.WindowSize = MathPower(2, PXZLIB.Header.CompressionInfo + 8u);
        }
        //-------------------------------------------------------------------------

        //---<Parse Second Byte - Flags>-------------------------------------------
        {
            const PXInt8U compressionLevelValue = (flagByte & 0b11000000) >> 6;

            PXZLIB.Header.CheckFlag = (flagByte & 0b00011111);
            PXZLIB.Header.PXDictionaryPresent = ((flagByte & 0b00100000) >> 5) == 1;
            PXZLIB.Header.CompressionLevel = ConvertToCompressionLevel(compressionLevelValue);
        }
        //-------------------------------------------------------------------------        
    }


    //---<PXDictionary Parse>----------------------------------------------------
    if(PXZLIB.Header.PXDictionaryPresent)
    {
        // Parse DICT dictionary identifier 

    }
    //-------------------------------------------------------------------------

    /* Allocate
    if (!(*outputData))
    {
        (*outputData) = (unsigned char*)malloc(PXZLIBHeader.WindowSize * 4 * sizeof(unsigned char));
    }*/


    PXZLIB.CompressedDataSize = PXDataStreamRemainingSize(pxInputSteam) - adlerSize;
    PXZLIB.CompressedData = PXDataStreamCursorPosition(pxInputSteam);

    switch(PXZLIB.Header.CompressionMethod)
    {
        case PXZLIBCompressionMethodDeflate:
        {
            const PXActionResult deflateResult = DEFLATEParse(pxInputSteam, pxOutputSteam);

            PXActionExitOnError(deflateResult);

            break;
        }
        default:
        case PXZLIBCompressionMethodReserved:
        case PXZLIBCompressionMethodInvalid:
        {
            return PXActionFailedFormatInvalid;
        }
    }

    PXDataStreamReadI32UE(pxInputSteam, &PXZLIB.AdlerChecksum, EndianBig);

    return PXActionSuccessful;
}

PXActionResult PXZLIBCompress(PXDataStream* const pxInputSteam, PXDataStream* const pxOutputSteam)
{
    // Write PXZLIB Header
    {
        const PXByte compressionMethod = ConvertFromCompressionMethod(PXZLIBCompressionMethodDeflate);
        const PXByte compressionInfo = 7u; // 1-7

        const PXByte dictionary = 0;
        const PXByte level = ConvertFromCompressionLevel(PXZLIBCompressionLevelFastest);

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
            const PXInt16U checksum = PXInt16Make(buffer[0], buffer[1]);
            const PXInt8U multble = 31-checksum % 31;

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
        const PXInt32U adler = (PXInt32U)Adler32Create(1, pxInputSteam->Data, pxInputSteam->DataSize);

        PXDataStreamWriteI32UE(pxOutputSteam, adler, EndianBig);
    }

    return PXActionSuccessful;
}

PXSize PXZLIBCalculateExpectedSize(const PXSize width, const PXSize height, const PXSize bpp, const PXPNGInterlaceMethod interlaceMethod)
{
    PXSize expected_size = 0;

    switch(interlaceMethod)
    {
        default:
        case PXPNGInterlaceInvalid:
            break;

        case PXPNGInterlaceNone:
        {
            // predict output size, to allocate exact size for output buffer to avoid more dynamic allocation.
            // If the decompressed size does not match the prediction, the image must be corrupt.
            expected_size = CalculateRawSizeIDAT(width, height, bpp);
            break;
        }
        case PXPNGInterlaceADAM7:
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