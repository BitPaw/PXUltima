#include "PXZLIB.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Media/DEFLATE/PXDEFLATE.h>
#include <PX/Media/ADLER/PXAdler32.h>

PXZLIBCompressionLevel PXAPI PXZLIBCompressionLevelFromID(const PXI8U compressionLevel)
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

PXI8U PXAPI PXZLIBCompressionLevelToID(const PXZLIBCompressionLevel compressionLevel)
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

PXZLIBCompressionMethod PXAPI PXZLIBCompressionMethodFromID(const PXI8U compressionMethod)
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

PXI8U PXAPI PXZLIBCompressionMethodToID(const PXZLIBCompressionMethod compressionMethod)
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

PXResult PXAPI PXZLIBDecompress(PXFile PXREF pxInputSteam, PXFile PXREF pxOutputSteam)
{
    PXZLIB PXZLIB;

    const PXSize headerSize = 2u;
    const PXSize adlerSize = 4u;

    // Parse header ->     Header.Parse(compressionFormatByte, flagByte);
    {
        PXI8U compressionFormatByte = 0;
        PXI8U flagByte = 0;

        PXFileReadI8U(pxInputSteam, &compressionFormatByte);
        PXFileReadI8U(pxInputSteam, &flagByte);

        // Valid Check
        {
            const PXBool validFlags = PXI16MakeEndianBig(compressionFormatByte, flagByte) % 31u == 0;

            if(!validFlags)
            {
                return PXActionRefusedInvalidHeaderSignature;// assert(validFlags);
            }
        }

        //---<Parse First Byte__ - Compression Info>---------------------------------
        {
            const PXI8U compressionMethodValue = (compressionFormatByte & 0b00001111);

            PXZLIB.Header.CompressionMethod = PXZLIBCompressionMethodFromID(compressionMethodValue);
            PXZLIB.Header.CompressionInfo = (compressionFormatByte & 0b11110000) >> 4;

            // log_2(WindowSize) - 8 = CompressionInfo
            // log_2(32768) - 8 = 7
            // 2^(CompressionInfo + 8)

            const PXBool isCompressionInfoValid = PXZLIB.Header.CompressionInfo <= 7u;

            if(!isCompressionInfoValid)
            {
                return PXActionRefusedVersionInvalid;
            }

            //assert(isCompressionInfoValid);

            PXZLIB.Header.WindowSize = PXMathPower(2, PXZLIB.Header.CompressionInfo + 8u);
        }
        //-------------------------------------------------------------------------

        //---<Parse Second Byte - Flags>-------------------------------------------
        {
            const PXI8U compressionLevelValue = (flagByte & 0b11000000) >> 6;

            PXZLIB.Header.CheckFlag = (flagByte & 0b00011111);
            PXZLIB.Header.DictionaryPresent = ((flagByte & 0b00100000) >> 5) == 1;
            PXZLIB.Header.CompressionLevel = PXZLIBCompressionLevelFromID(compressionLevelValue);
        }
        //-------------------------------------------------------------------------
    }


    //---<PXDictionary Parse>----------------------------------------------------
    if(PXZLIB.Header.DictionaryPresent)
    {
        // Parse DICT dictionary identifier

    }
    //-------------------------------------------------------------------------

    /* Allocate
    if (!(*outputData))
    {
        (*outputData) = (unsigned char*)malloc(PXZLIBHeader.WindowSize * 4 * sizeof(unsigned char));
    }*/


    PXZLIB.CompressedDataSize = PXFileRemainingSize(pxInputSteam) - adlerSize;
    PXZLIB.CompressedData = PXFileDataAtCursor(pxInputSteam);

    switch(PXZLIB.Header.CompressionMethod)
    {
        case PXZLIBCompressionMethodDeflate:
        {
            const PXResult deflateResult = PXDEFLATEParse(pxInputSteam, pxOutputSteam);

            if(PXActionSuccessful != deflateResult)
                return deflateResult;

            break;
        }
        default:
        case PXZLIBCompressionMethodReserved:
        case PXZLIBCompressionMethodInvalid:
        {
            return PXActionFailedFormatNotAsExpected;
        }
    }

    PXFileReadI32UE(pxInputSteam, &PXZLIB.AdlerChecksum, PXEndianBig);

    return PXActionSuccessful;
}

PXResult PXAPI PXZLIBCompress(PXFile PXREF pxInputSteam, PXFile PXREF pxOutputSteam)
{
    // Write PXZLIB Header
    {
        const PXByte compressionMethod = PXZLIBCompressionMethodToID(PXZLIBCompressionMethodDeflate);
        const PXByte compressionInfo = 7u; // 1-7

        const PXByte dictionary = 0;
        const PXByte level = PXZLIBCompressionLevelToID(PXZLIBCompressionLevelFastest);

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
            const PXI16U checksum = PXI16Make(buffer[0], buffer[1]);
            const PXI8U multble = 31-checksum % 31;

            buffer[1] += multble;
        }

        PXFileWriteB(pxOutputSteam, buffer, 2u);
    }

    // Write DEFLATE
    {
        const PXResult delfateResult = PXDEFLATESerialize(pxInputSteam, pxOutputSteam);

        if(PXActionSuccessful != delfateResult) 
            return delfateResult;;
    }

    // Write ADLER
    {
        const PXI32U adler = (PXI32U)PXAdler32Create
        (
            1,
            PXFileDataAtCursor(pxInputSteam),
            PXFileDataPosition(pxInputSteam)
        );

        PXFileWriteI32UE(pxOutputSteam, adler, PXEndianBig);
    }

    return PXActionSuccessful;
}

PXSize PXAPI PXZLIBCalculateExpectedSize(const PXSize width, const PXSize height, const PXSize bpp, const PXPNGInterlaceMethod interlaceMethod)
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
        expected_size = PXZLIBCalculateRawSizeIDAT(width, height, bpp);
        break;
    }
    case PXPNGInterlaceADAM7:
    {
        // Adam-7 interlaced: expected size is the sum of the 7 sub-images sizes
        expected_size = 0;
        expected_size += PXZLIBCalculateRawSizeIDAT((width + 7) >> 3, (height + 7) >> 3, bpp);
        if(width > 4) expected_size += PXZLIBCalculateRawSizeIDAT((width + 3) >> 3, (height + 7) >> 3, bpp);
        expected_size += PXZLIBCalculateRawSizeIDAT((width + 3) >> 2, (height + 3) >> 3, bpp);
        if(width > 2) expected_size += PXZLIBCalculateRawSizeIDAT((width + 1) >> 2, (height + 3) >> 2, bpp);
        expected_size += PXZLIBCalculateRawSizeIDAT((width + 1) >> 1, (height + 1) >> 2, bpp);
        if(width > 1) expected_size += PXZLIBCalculateRawSizeIDAT((width + 0) >> 1, (height + 1) >> 1, bpp);
        expected_size += PXZLIBCalculateRawSizeIDAT((width + 0), (height + 0) >> 1, bpp);

        break;
    }
    }

    return expected_size;
}

PXSize PXAPI PXZLIBCalculateRawSizeIDAT(const PXSize w, const PXSize h, const PXSize bpp)
{
    // + 1 for the filter byte, and possibly plus padding bits per line.
    // Ignoring casts, the expression is equal to (w * bpp + 7) / 8 + 1, but avoids overflow of w * bpp
    return (((w / 8u) * bpp) + 1u + ((w & 7u) * bpp + 7u) / 8u) * h;
}
