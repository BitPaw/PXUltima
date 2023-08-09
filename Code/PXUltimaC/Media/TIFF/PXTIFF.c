#include "PXTIFF.h"

#define PXTIFFormatA {'I','I'}
#define PXTIFFormatB {'M','M'}

PXTIFFType PXTIFFTypeFromID(const unsigned char tiffTypeID)
{
    switch (tiffTypeID)
    {
        case 1u: return PXTIFFTypeByteUnsigned;
        case 2u: return PXTIFFTypeASCII;
        case 3u: return PXTIFFTypeShortUnsigned;
        case 4u: return PXTIFFTypeLongUnsigned;
        case 5u: return PXTIFFTypeRationalUnsigned;
        case 6u: return PXTIFFTypeByteSigned;
        case 7u: return PXTIFFTypeByteCustom;
        case 8u: return PXTIFFTypeShortSigned;
        case 9u: return PXTIFFTypeLongSigned;
        case 10u: return PXTIFFTypeRationalSigned;
        case 11u: return PXTIFFTypeFloat;
        case 12u: return PXTIFFTypeDouble;

        default:
            return PXTIFFTypeInvalid;
    }
}

PXTIFFTagType PXTIFFTagTypeFromID(const unsigned short tiffTagTypeID)
{
    switch (tiffTagTypeID)
    {
        case 254u: return PXTIFFTagNewSubFileType;
        case 255u: return PXTIFFTagSubfileType;
        case 256u: return PXTIFFTagImageWidth;
        case 257u: return PXTIFFTagImageLength;
        case 258u: return PXTIFFTagBitsPerSample;
        case 259u: return PXTIFFTagCompression;
        case 262u: return PXTIFFTagPhotometricInterpretation;
        case 263u: return PXTIFFTagThreshholding;
        case 264u: return PXTIFFTagCellWidth;
        case 265u: return PXTIFFTagCellLength;
        case 266u: return PXTIFFTagFillOrder;
        case 269u: return PXTIFFTagDocumentName;
        case 270u: return PXTIFFTagImageDescription;
        case 271u: return PXTIFFTagMake;
        case 272u: return PXTIFFTagModel;
        case 273u: return PXTIFFTagStripOffsets;
        case 274u: return PXTIFFTagOrientation;
        case 277u: return PXTIFFTagSamplesPerPixel;
        case 278u: return PXTIFFTagRowsPerStrip;
        case 279u: return PXTIFFTagStripByteCounts;
        case 280u: return PXTIFFTagMinSampleValue;
        case 281u: return PXTIFFTagMaxSampleValue;
        case 282u: return PXTIFFTagXResolution;
        case 283u: return PXTIFFTagYResolution;
        case 284u: return PXTIFFTagPlanarConfiguration;
        case 285u: return PXTIFFTagPageName;
        case 286u: return PXTIFFTagXPosition;
        case 287u: return PXTIFFTagYPosition;
        case 288u: return PXTIFFTagFreeOffsets;
        case 289u: return PXTIFFTagFreeByteCounts;
        case 290u: return PXTIFFTagGrayResponseUnit;
        case 291u: return PXTIFFTagGrayResponseCurve;
        case 292u: return PXTIFFTagT4Options;
        case 293u: return PXTIFFTagT6Options;
        case 296u: return PXTIFFTagResolutionUnit;
        case 297u: return PXTIFFTagPageNumber;
        case 301u: return PXTIFFTagTransferFunction;
        case 305u: return PXTIFFTagSoftware;
        case 306u: return PXTIFFTagDateTime;
        case 315u: return PXTIFFTagArtist;
        case 316u: return PXTIFFTagHostComputer;
        case 317u: return PXTIFFTagPredictor;
        case 318u: return PXTIFFTagWhitePoint;
        case 319u: return PXTIFFTagPrimaryChromaticities;
        case 320u: return PXTIFFTagColorMap;
        case 321u: return PXTIFFTagHalftoneHints;
        case 322u: return PXTIFFTagTileWidth;
        case 323u: return PXTIFFTagTileLength;
        case 324u: return PXTIFFTagTileOffsets;
        case 325u: return PXTIFFTagTileByteCounts;
        case 332u: return PXTIFFTagInkSet;
        case 333u: return PXTIFFTagInkNames;
        case 334u: return PXTIFFTagNumberOfInks;
        case 336u: return PXTIFFTagDotRange;
        case 337u: return PXTIFFTagTargetPrinter;
        case 338u: return PXTIFFTagExtraSamples;
        case 339u: return PXTIFFTagSampleFormat;
        case 340u: return PXTIFFTagSMinSampleValue;
        case 341u: return PXTIFFTagSMaxSampleValue;
        case 342u: return PXTIFFTagTransferRange;
        case 512u: return PXTIFFTagPXJPEGProc;
        case 513u: return PXTIFFTagPXJPEGInterchangeFormat;
        case 514u: return PXTIFFTagPXJPEGInterchangeFormatLngth;
        case 515u: return PXTIFFTagPXJPEGRestartInterval;
        case 517u: return PXTIFFTagPXJPEGLosslessPredictors;
        case 518u: return PXTIFFTagPXJPEGPointTransforms;
        case 519u: return PXTIFFTagPXJPEGQTables;
        case 520u: return PXTIFFTagPXJPEGDCTables;
        case 521u: return PXTIFFTagPXJPEGACTables;
        case 529u: return PXTIFFTagYCbCrCoefficients;
        case 530u: return PXTIFFTagYCbCrSubSampling;
        case 531u: return PXTIFFTagYCbCrPositioning;
        case 532u: return PXTIFFTagReferenceBlackWhite;
        case 33432u: return PXTIFFTagCopyright;

        default:
            return PXTIFFTagTypeUnkown;
    }
}

PXSize PXTIFFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXActionResult PXTIFFLoadFromFile(PXImage* const pxImage, PXFile* const pxFile)
{
    PXTIFF* tiff = PXNull;

    PXMemoryClear(tiff, sizeof(PXTIFF));


    PXTIFFHeader tIFFHeader;


    // Check Header
    {
        // Get engianess
        {
            const char versionA[2] = PXTIFFormatA;
            const char versionB[2] = PXTIFFormatB;
            char headerTag[2];

            PXFileReadB(pxFile, headerTag, 2u);

            const char select =
                'B' * (headerTag[0] == versionB[0] && headerTag[1] == versionB[1]) + // big
                'L' * (headerTag[0] == versionA[0] && headerTag[1] == versionA[1]); // little

            switch (select)
            {
                case 'B':
                    tIFFHeader.Endianness = PXEndianBig;
                    break;

                case 'L':
                    tIFFHeader.Endianness = PXEndianLittle;
                    break;

                default:
                    return PXActionRefusedInvalidHeaderSignature;
            }
        }

        PXFileReadI16UE(pxFile, &tIFFHeader.Version, tIFFHeader.Endianness); // Version, expect this to be "42"
        PXFileReadI32UE(pxFile, &tIFFHeader.OffsetToIFD, tIFFHeader.Endianness);

        // Jump to adress
        pxFile->DataCursor = tIFFHeader.OffsetToIFD;

        // Now we are at IFD page 0
        while (!PXFileIsAtEnd(pxFile))
        {
            PXTIFFPage tiffPage;

            PXMemoryClear(&tiffPage, sizeof(PXTIFFPage));

            PXFileReadI16UE(pxFile, &tiffPage.NumberOfTags, tIFFHeader.Endianness); // 2-Bytes

            for (unsigned short i = 0; i < tiffPage.NumberOfTags; ++i) // Read 12-Bytes
            {
                PXTIFFTag tiffTag;

                PXFileReadI16UE(pxFile, &tiffTag.TypeID, tIFFHeader.Endianness); // 2-Bytes
                PXFileReadI16UE(pxFile, &tiffTag.DataTypeID, tIFFHeader.Endianness); // 2-Bytes
                PXFileReadI32UE(pxFile, &tiffTag.NumberOfValues, tIFFHeader.Endianness); // 4-Bytes
                PXFileReadI32UE(pxFile, &tiffTag.DataOffset, tIFFHeader.Endianness); // 4-Bytes

                tiffTag.Type = PXTIFFTagTypeFromID(tiffTag.TypeID);
                tiffTag.DataType = PXTIFFTypeFromID(tiffTag.DataTypeID);

                switch (tiffTag.Type)
                {
                    case PXTIFFTagNewSubFileType:
                    case PXTIFFTagSubfileType:
                    case PXTIFFTagImageWidth:
                    {
                        tiff->Width = tiffTag.DataOffset;
                        break;
                    }
                    case PXTIFFTagImageLength:
                    {
                        tiff->Height = tiffTag.DataOffset;
                        break;
                    }
                    case PXTIFFTagBitsPerSample:
                    {
                        for (PXSize i = 0; i < tiffTag.NumberOfValues; ++i)
                        {
                            tiff->BitsPerSample[i] = tiffTag.DataOffset; // Is this correct?
                        }

                        break;
                    }
                    case PXTIFFTagCompression:
                    {
                        tiff->Compression = PXTIFFCompressionFromID(tiffTag.DataOffset);
                        break;
                    }
                    case PXTIFFTagPhotometricInterpretation:
                    {
                        tiff->PhotometricInterpretation = PXTIFFColorFormatFromID(tiffTag.DataOffset);
                        break;
                    }
                    case PXTIFFTagThreshholding:
                    case PXTIFFTagCellWidth:
                    case PXTIFFTagCellLength:
                    case PXTIFFTagFillOrder:
                    case PXTIFFTagDocumentName:
                    case PXTIFFTagImageDescription:
                    case PXTIFFTagMake:
                    case PXTIFFTagModel:
                    case PXTIFFTagStripOffsets:
                    case PXTIFFTagOrientation:
                    case PXTIFFTagSamplesPerPixel:
                    {
                        tiff->SamplesPerPixel = tiffTag.DataOffset;
                        break;
                    }
                    case PXTIFFTagRowsPerStrip:
                    case PXTIFFTagStripByteCounts:
                    case PXTIFFTagMinSampleValue:
                    case PXTIFFTagMaxSampleValue:
                    case PXTIFFTagXResolution:
                    case PXTIFFTagYResolution:
                    case PXTIFFTagPlanarConfiguration:
                    case PXTIFFTagPageName:
                    case PXTIFFTagXPosition:
                    case PXTIFFTagYPosition:
                    case PXTIFFTagFreeOffsets:
                    case PXTIFFTagFreeByteCounts:
                    case PXTIFFTagGrayResponseUnit:
                    case PXTIFFTagGrayResponseCurve:
                    case PXTIFFTagT4Options:
                    case PXTIFFTagT6Options:
                    case PXTIFFTagResolutionUnit:
                    case PXTIFFTagPageNumber:
                    case PXTIFFTagTransferFunction:
                    case PXTIFFTagSoftware:
                    case PXTIFFTagDateTime:
                    case PXTIFFTagArtist:
                    case PXTIFFTagHostComputer:
                    case PXTIFFTagPredictor:
                    case PXTIFFTagWhitePoint:
                    case PXTIFFTagPrimaryChromaticities:
                    case PXTIFFTagColorMap:
                    case PXTIFFTagHalftoneHints:
                    case PXTIFFTagTileWidth:
                    case PXTIFFTagTileLength:
                    case PXTIFFTagTileOffsets:
                    case PXTIFFTagTileByteCounts:
                    case PXTIFFTagInkSet:
                    case PXTIFFTagInkNames:
                    case PXTIFFTagNumberOfInks:
                    case PXTIFFTagDotRange:
                    case PXTIFFTagTargetPrinter:
                    case PXTIFFTagExtraSamples:
                    case PXTIFFTagSampleFormat:
                    case PXTIFFTagSMinSampleValue:
                    case PXTIFFTagSMaxSampleValue:
                    case PXTIFFTagTransferRange:
                    case PXTIFFTagPXJPEGProc:
                    case PXTIFFTagPXJPEGInterchangeFormat:
                    case PXTIFFTagPXJPEGInterchangeFormatLngth:
                    case PXTIFFTagPXJPEGRestartInterval:
                    case PXTIFFTagPXJPEGLosslessPredictors:
                    case PXTIFFTagPXJPEGPointTransforms:
                    case PXTIFFTagPXJPEGQTables:
                    case PXTIFFTagPXJPEGDCTables:
                    case PXTIFFTagPXJPEGACTables:
                    case PXTIFFTagYCbCrCoefficients:
                    case PXTIFFTagYCbCrSubSampling:
                    case PXTIFFTagYCbCrPositioning:
                    case PXTIFFTagReferenceBlackWhite:
                    case PXTIFFTagCopyright:

                    default:
                        break;
                }
            }

            PXFileReadI32UE(pxFile, &tiffPage.OffsetToNextIFD, tIFFHeader.Endianness); // 4-Bytes
        }





        // Parse pages

        //read next stuff


    }


    return PXActionSuccessful;
}

PXActionResult PXTIFFSaveToFile(PXImage* const pxImage, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}

PXTIFFCompression PXTIFFCompressionFromID(const unsigned short tiffCompressionID)
{
    switch (tiffCompressionID)
    {
        case 1u: return PXTIFFCompressionUncompressed;
        case 2u: return PXTIFFCompressionCCITT1D;
        case 3u: return PXTIFFCompressionGroup3Fax;
        case 4u: return PXTIFFCompressionGroup4Fax;

        case 5u: return PXTIFFCompressionLZW;
        case 6u: return PXTIFFCompressionPXJPEGCompression;
        case 32773u: return PXTIFFCompressionPackBits;

        default:
            return PXTIFFCompressionInvalid;
    }
}

PXTIFFColorFormat PXTIFFColorFormatFromID(const unsigned short tiffColorFormatID)
{
    switch (tiffColorFormatID)
    {
        case 0u: return PXTIFFColorMonochromeWhiteIsZero;
        case 1u: return PXTIFFColorMonochromeBlackIsZero;
        case 2u: return PXTIFFColorRGB;
        case 3u: return PXTIFFColorRGBPalette;
        case 4u: return PXTIFFColorTransparencyMask;
        case 5u: return PXTIFFColorCMYK;
        case 6u: return PXTIFFColorYCbCr;
        case 8u: return PXTIFFColorCIELab;

        default:
            return PXTIFFColorInvalid;
    }
}
