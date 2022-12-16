#include "TIFF.h"

#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>

#define TIFFormatA {'I','I'}
#define TIFFormatB {'M','M'}

TIFFType TIFFTypeFromID(const unsigned char tiffTypeID)
{
    switch (tiffTypeID)
    {
        case 1u: return TIFFTypeByteUnsigned;
        case 2u: return TIFFTypeASCII;
        case 3u: return TIFFTypeShortUnsigned;
        case 4u: return TIFFTypeLongUnsigned;
        case 5u: return TIFFTypeRationalUnsigned;
        case 6u: return TIFFTypeByteSigned;
        case 7u: return TIFFTypeByteCustom;
        case 8u: return TIFFTypeShortSigned;
        case 9u: return TIFFTypeLongSigned;
        case 10u: return TIFFTypeRationalSigned;
        case 11u: return TIFFTypeFloat;
        case 12u: return TIFFTypeDouble;

        default:
            return TIFFTypeInvalid;
    }
}

TIFFTagType TIFFTagTypeFromID(const unsigned short tiffTagTypeID)
{
    switch (tiffTagTypeID)
    {
        case 254u: return TIFFTagNewSubFileType;
        case 255u: return TIFFTagSubfileType;
        case 256u: return TIFFTagImageWidth;
        case 257u: return TIFFTagImageLength;
        case 258u: return TIFFTagBitsPerSample;
        case 259u: return TIFFTagCompression;
        case 262u: return TIFFTagPhotometricInterpretation;
        case 263u: return TIFFTagThreshholding;
        case 264u: return TIFFTagCellWidth;
        case 265u: return TIFFTagCellLength;
        case 266u: return TIFFTagFillOrder;
        case 269u: return TIFFTagDocumentName;
        case 270u: return TIFFTagImageDescription;
        case 271u: return TIFFTagMake;
        case 272u: return TIFFTagModel;
        case 273u: return TIFFTagStripOffsets;
        case 274u: return TIFFTagOrientation;
        case 277u: return TIFFTagSamplesPerPixel;
        case 278u: return TIFFTagRowsPerStrip;
        case 279u: return TIFFTagStripByteCounts;
        case 280u: return TIFFTagMinSampleValue;
        case 281u: return TIFFTagMaxSampleValue;
        case 282u: return TIFFTagXResolution;
        case 283u: return TIFFTagYResolution;
        case 284u: return TIFFTagPlanarConfiguration;
        case 285u: return TIFFTagPageName;
        case 286u: return TIFFTagXPosition;
        case 287u: return TIFFTagYPosition;
        case 288u: return TIFFTagFreeOffsets;
        case 289u: return TIFFTagFreeByteCounts;
        case 290u: return TIFFTagGrayResponseUnit;
        case 291u: return TIFFTagGrayResponseCurve;
        case 292u: return TIFFTagT4Options;
        case 293u: return TIFFTagT6Options;
        case 296u: return TIFFTagResolutionUnit;
        case 297u: return TIFFTagPageNumber;
        case 301u: return TIFFTagTransferFunction;
        case 305u: return TIFFTagSoftware;
        case 306u: return TIFFTagDateTime;
        case 315u: return TIFFTagArtist;
        case 316u: return TIFFTagHostComputer;
        case 317u: return TIFFTagPredictor;
        case 318u: return TIFFTagWhitePoint;
        case 319u: return TIFFTagPrimaryChromaticities;
        case 320u: return TIFFTagColorMap;
        case 321u: return TIFFTagHalftoneHints;
        case 322u: return TIFFTagTileWidth;
        case 323u: return TIFFTagTileLength;
        case 324u: return TIFFTagTileOffsets;
        case 325u: return TIFFTagTileByteCounts;
        case 332u: return TIFFTagInkSet;
        case 333u: return TIFFTagInkNames;
        case 334u: return TIFFTagNumberOfInks;
        case 336u: return TIFFTagDotRange;
        case 337u: return TIFFTagTargetPrinter;
        case 338u: return TIFFTagExtraSamples;
        case 339u: return TIFFTagSampleFormat;
        case 340u: return TIFFTagSMinSampleValue;
        case 341u: return TIFFTagSMaxSampleValue;
        case 342u: return TIFFTagTransferRange;
        case 512u: return TIFFTagJPEGProc;
        case 513u: return TIFFTagJPEGInterchangeFormat;
        case 514u: return TIFFTagJPEGInterchangeFormatLngth;
        case 515u: return TIFFTagJPEGRestartInterval;
        case 517u: return TIFFTagJPEGLosslessPredictors;
        case 518u: return TIFFTagJPEGPointTransforms;
        case 519u: return TIFFTagJPEGQTables;
        case 520u: return TIFFTagJPEGDCTables;
        case 521u: return TIFFTagJPEGACTables;
        case 529u: return TIFFTagYCbCrCoefficients;
        case 530u: return TIFFTagYCbCrSubSampling;
        case 531u: return TIFFTagYCbCrPositioning;
        case 532u: return TIFFTagReferenceBlackWhite;
        case 33432u: return TIFFTagCopyright;

        default:
            return TIFFTagTypeUnkown;
    }
}

PXSize TIFFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXActionResult TIFFParse(TIFF* const tiff, PXDataStream* const dataStream)
{
    MemoryClear(tiff, sizeof(TIFF));


    TIFFHeader tIFFHeader;


    // Check Header
    {
        // Get engianess
        {
            const char versionA[2] = TIFFormatA;
            const char versionB[2] = TIFFormatB;
            char headerTag[2];

            PXDataStreamReadB(dataStream, headerTag, 2u);

            const char select =
                'B' * (headerTag[0] == versionB[0] && headerTag[1] == versionB[1]) + // big
                'L' * (headerTag[0] == versionA[0] && headerTag[1] == versionA[1]); // little

            switch (select)
            {
                case 'B':
                    tIFFHeader.Endianness = EndianBig;
                    break;

                case 'L':
                    tIFFHeader.Endianness = EndianLittle;
                    break;

                default:
                    return ;
            }
        }

        PXDataStreamReadI16UE(dataStream, &tIFFHeader.Version, tIFFHeader.Endianness); // Version, expect this to be "42"
        PXDataStreamReadI32UE(dataStream, &tIFFHeader.OffsetToIFD, tIFFHeader.Endianness);

        // Jump to adress
        dataStream->DataCursor = tIFFHeader.OffsetToIFD;

        // Now we are at IFD page 0
        while (!PXDataStreamIsAtEnd(dataStream))
        {
            TIFFPage tiffPage;

            MemoryClear(&tiffPage, sizeof(TIFFPage));

            PXDataStreamReadI16UE(dataStream, &tiffPage.NumberOfTags, tIFFHeader.Endianness); // 2-Bytes

            for (unsigned short i = 0; i < tiffPage.NumberOfTags; ++i) // Read 12-Bytes
            {
                TIFFTag tiffTag;

                PXDataStreamReadI16UE(dataStream, &tiffTag.TypeID, tIFFHeader.Endianness); // 2-Bytes
                PXDataStreamReadI16UE(dataStream, &tiffTag.DataTypeID, tIFFHeader.Endianness); // 2-Bytes
                PXDataStreamReadI32UE(dataStream, &tiffTag.NumberOfValues, tIFFHeader.Endianness); // 4-Bytes
                PXDataStreamReadI32UE(dataStream, &tiffTag.DataOffset, tIFFHeader.Endianness); // 4-Bytes

                tiffTag.Type = TIFFTagTypeFromID(tiffTag.TypeID);
                tiffTag.DataType = TIFFTypeFromID(tiffTag.DataTypeID);

                switch (tiffTag.Type)
                {
                    case TIFFTagNewSubFileType:
                    case TIFFTagSubfileType:
                    case TIFFTagImageWidth:
                    {
                        tiff->Width = tiffTag.DataOffset;
                        break;
                    }
                    case TIFFTagImageLength:
                    {
                        tiff->Height = tiffTag.DataOffset;
                        break;
                    }
                    case TIFFTagBitsPerSample:
                    {
                        for (PXSize i = 0; i < tiffTag.NumberOfValues; ++i)
                        {
                            tiff->BitsPerSample[i] = tiffTag.DataOffset; // Is this correct?
                        }

                        break;
                    }
                    case TIFFTagCompression:
                    {
                        tiff->Compression = TIFFCompressionFromID(tiffTag.DataOffset);
                        break;
                    }
                    case TIFFTagPhotometricInterpretation:
                    {
                        tiff->PhotometricInterpretation = TIFFColorFormatFromID(tiffTag.DataOffset);
                        break;
                    }
                    case TIFFTagThreshholding:
                    case TIFFTagCellWidth:
                    case TIFFTagCellLength:
                    case TIFFTagFillOrder:
                    case TIFFTagDocumentName:
                    case TIFFTagImageDescription:
                    case TIFFTagMake:
                    case TIFFTagModel:
                    case TIFFTagStripOffsets:
                    case TIFFTagOrientation:
                    case TIFFTagSamplesPerPixel:
                    {
                        tiff->SamplesPerPixel = tiffTag.DataOffset;
                        break;
                    }
                    case TIFFTagRowsPerStrip:
                    case TIFFTagStripByteCounts:
                    case TIFFTagMinSampleValue:
                    case TIFFTagMaxSampleValue:
                    case TIFFTagXResolution:
                    case TIFFTagYResolution:
                    case TIFFTagPlanarConfiguration:
                    case TIFFTagPageName:
                    case TIFFTagXPosition:
                    case TIFFTagYPosition:
                    case TIFFTagFreeOffsets:
                    case TIFFTagFreeByteCounts:
                    case TIFFTagGrayResponseUnit:
                    case TIFFTagGrayResponseCurve:
                    case TIFFTagT4Options:
                    case TIFFTagT6Options:
                    case TIFFTagResolutionUnit:
                    case TIFFTagPageNumber:
                    case TIFFTagTransferFunction:
                    case TIFFTagSoftware:
                    case TIFFTagDateTime:
                    case TIFFTagArtist:
                    case TIFFTagHostComputer:
                    case TIFFTagPredictor:
                    case TIFFTagWhitePoint:
                    case TIFFTagPrimaryChromaticities:
                    case TIFFTagColorMap:
                    case TIFFTagHalftoneHints:
                    case TIFFTagTileWidth:
                    case TIFFTagTileLength:
                    case TIFFTagTileOffsets:
                    case TIFFTagTileByteCounts:
                    case TIFFTagInkSet:
                    case TIFFTagInkNames:
                    case TIFFTagNumberOfInks:
                    case TIFFTagDotRange:
                    case TIFFTagTargetPrinter:
                    case TIFFTagExtraSamples:
                    case TIFFTagSampleFormat:
                    case TIFFTagSMinSampleValue:
                    case TIFFTagSMaxSampleValue:
                    case TIFFTagTransferRange:
                    case TIFFTagJPEGProc:
                    case TIFFTagJPEGInterchangeFormat:
                    case TIFFTagJPEGInterchangeFormatLngth:
                    case TIFFTagJPEGRestartInterval:
                    case TIFFTagJPEGLosslessPredictors:
                    case TIFFTagJPEGPointTransforms:
                    case TIFFTagJPEGQTables:
                    case TIFFTagJPEGDCTables:
                    case TIFFTagJPEGACTables:
                    case TIFFTagYCbCrCoefficients:
                    case TIFFTagYCbCrSubSampling:
                    case TIFFTagYCbCrPositioning:
                    case TIFFTagReferenceBlackWhite:
                    case TIFFTagCopyright:

                    default:
                        break;
                }
            }

            PXDataStreamReadI32UE(dataStream, &tiffPage.OffsetToNextIFD, tIFFHeader.Endianness); // 4-Bytes
        }





        // Parse pages

        //read next stuff


    }


    return PXActionSuccessful;
}

PXActionResult TIFFParseToImage(Image* const image, PXDataStream* const dataStream)
{
    TIFF tiff;

    TIFFParse(&tiff, dataStream);

    return PXActionInvalid;
}

PXActionResult TIFFSerializeFromImage(const Image* const image, void* data, const PXSize dataSize, PXSize* dataWritten)
{
    return PXActionInvalid;
}

TIFFCompression TIFFCompressionFromID(const unsigned short tiffCompressionID)
{
    switch (tiffCompressionID)
    {
        case 1u: return TIFFCompressionUncompressed;
        case 2u: return TIFFCompressionCCITT1D;
        case 3u: return TIFFCompressionGroup3Fax;
        case 4u: return TIFFCompressionGroup4Fax;

        case 5u: return TIFFCompressionLZW;
        case 6u: return TIFFCompressionJPEGCompression;
        case 32773u: return TIFFCompressionPackBits;

        default:
            return TIFFCompressionInvalid;
    }
}

TIFFColorFormat TIFFColorFormatFromID(const unsigned short tiffColorFormatID)
{
    switch (tiffColorFormatID)
    {
        case 0u: return TIFFColorMonochromeWhiteIsZero;
        case 1u: return TIFFColorMonochromeBlackIsZero;
        case 2u: return TIFFColorRGB;
        case 3u: return TIFFColorRGBPalette;
        case 4u: return TIFFColorTransparencyMask;
        case 5u: return TIFFColorCMYK;
        case 6u: return TIFFColorYCbCr;
        case 8u: return TIFFColorCIELab;

        default:
            return TIFFColorInvalid;
    }
}
