#include "PXTIFF.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

#define PXTIFFDebug 0

PXTIFFType PXAPI PXTIFFTypeFromID(const unsigned char tiffTypeID)
{
    switch (tiffTypeID)
    {
    case 1u:
        return PXTIFFTypeByteUnsigned;
    case 2u:
        return PXTIFFTypeASCII;
    case 3u:
        return PXTIFFTypeShortUnsigned;
    case 4u:
        return PXTIFFTypeLongUnsigned;
    case 5u:
        return PXTIFFTypeRationalUnsigned;
    case 6u:
        return PXTIFFTypeByteSigned;
    case 7u:
        return PXTIFFTypeByteCustom;
    case 8u:
        return PXTIFFTypeShortSigned;
    case 9u:
        return PXTIFFTypeLongSigned;
    case 10u:
        return PXTIFFTypeRationalSigned;
    case 11u:
        return PXTIFFTypePXF32;
    case 12u:
        return PXTIFFTypeDouble;

    default:
        return PXTIFFTypeInvalid;
    }
}

PXTIFFTagType PXAPI PXTIFFTagTypeFromID(const unsigned short tiffTagTypeID)
{
    switch (tiffTagTypeID)
    {
    case 254u:
        return PXTIFFTagNewSubFileType;
    case 255u:
        return PXTIFFTagSubfileType;
    case 256u:
        return PXTIFFTagImageWidth;
    case 257u:
        return PXTIFFTagImageLength;
    case 258u:
        return PXTIFFTagBitsPerSample;
    case 259u:
        return PXTIFFTagCompression;
    case 262u:
        return PXTIFFTagPhotometricInterpretation;
    case 263u:
        return PXTIFFTagThreshholding;
    case 264u:
        return PXTIFFTagCellWidth;
    case 265u:
        return PXTIFFTagCellLength;
    case 266u:
        return PXTIFFTagFillOrder;
    case 269u:
        return PXTIFFTagDocumentName;
    case 270u:
        return PXTIFFTagImageDescription;
    case 271u:
        return PXTIFFTagMake;
    case 272u:
        return PXTIFFTagModel;
    case 273u:
        return PXTIFFTagStripOffsets;
    case 274u:
        return PXTIFFTagOrientation;
    case 277u:
        return PXTIFFTagSamplesPerPixel;
    case 278u:
        return PXTIFFTagRowsPerStrip;
    case 279u:
        return PXTIFFTagStripByteCounts;
    case 280u:
        return PXTIFFTagMinSampleValue;
    case 281u:
        return PXTIFFTagMaxSampleValue;
    case 282u:
        return PXTIFFTagXResolution;
    case 283u:
        return PXTIFFTagYResolution;
    case 284u:
        return PXTIFFTagPlanarConfiguration;
    case 285u:
        return PXTIFFTagPageName;
    case 286u:
        return PXTIFFTagXPosition;
    case 287u:
        return PXTIFFTagYPosition;
    case 288u:
        return PXTIFFTagFreeOffsets;
    case 289u:
        return PXTIFFTagFreeByteCounts;
    case 290u:
        return PXTIFFTagGrayResponseUnit;
    case 291u:
        return PXTIFFTagGrayResponseCurve;
    case 292u:
        return PXTIFFTagT4Options;
    case 293u:
        return PXTIFFTagT6Options;
    case 296u:
        return PXTIFFTagResolutionUnit;
    case 297u:
        return PXTIFFTagPageNumber;
    case 301u:
        return PXTIFFTagTransferFunction;
    case 305u:
        return PXTIFFTagSoftware;
    case 306u:
        return PXTIFFTagDateTime;
    case 315u:
        return PXTIFFTagArtist;
    case 316u:
        return PXTIFFTagHostComputer;
    case 317u:
        return PXTIFFTagPredictor;
    case 318u:
        return PXTIFFTagWhitePoint;
    case 319u:
        return PXTIFFTagPrimaryChromaticities;
    case 320u:
        return PXTIFFTagColorMap;
    case 321u:
        return PXTIFFTagHalftoneHints;
    case 322u:
        return PXTIFFTagTileWidth;
    case 323u:
        return PXTIFFTagTileLength;
    case 324u:
        return PXTIFFTagTileOffsets;
    case 325u:
        return PXTIFFTagTileByteCounts;
    case 332u:
        return PXTIFFTagInkSet;
    case 333u:
        return PXTIFFTagInkNames;
    case 334u:
        return PXTIFFTagNumberOfInks;
    case 336u:
        return PXTIFFTagDotRange;
    case 337u:
        return PXTIFFTagTargetPrinter;
    case 338u:
        return PXTIFFTagExtraSamples;
    case 339u:
        return PXTIFFTagSampleFormat;
    case 340u:
        return PXTIFFTagSMinSampleValue;
    case 341u:
        return PXTIFFTagSMaxSampleValue;
    case 342u:
        return PXTIFFTagTransferRange;
    case 512u:
        return PXTIFFTagPXJPEGProc;
    case 513u:
        return PXTIFFTagPXJPEGInterchangeFormat;
    case 514u:
        return PXTIFFTagPXJPEGInterchangeFormatLngth;
    case 515u:
        return PXTIFFTagPXJPEGRestartInterval;
    case 517u:
        return PXTIFFTagPXJPEGLosslessPredictors;
    case 518u:
        return PXTIFFTagPXJPEGPointTransforms;
    case 519u:
        return PXTIFFTagPXJPEGQTables;
    case 520u:
        return PXTIFFTagPXJPEGDCTables;
    case 521u:
        return PXTIFFTagPXJPEGACTables;
    case 529u:
        return PXTIFFTagYCbCrCoefficients;
    case 530u:
        return PXTIFFTagYCbCrSubSampling;
    case 531u:
        return PXTIFFTagYCbCrPositioning;
    case 532u:
        return PXTIFFTagReferenceBlackWhite;
    case 33432u:
        return PXTIFFTagCopyright;

    default:
        return PXTIFFTagTypeUnkown;
    }
}

PXSize PXAPI PXTIFFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

PXResult PXAPI PXTIFFLoadFromFile(PXFile PXREF pxFile, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXTIFF pxTIFFOBject;
    PXTIFF* tiff = &pxTIFFOBject;

    PXClear(PXTIFF, tiff);

    // Check Header
    {
        // Get engianess
        {
            PXI16UCluster headerTag;

            PXFileReadB(pxResourceLoadInfo->FileCurrent, headerTag.Data, 2u);

            switch (headerTag.Value)
            {
            case PXI16Make('M', 'M'):
                tiff->Endianness = PXEndianBig;
                break;

            case PXI16Make('I', 'I'):
                tiff->Endianness = PXEndianLittle;
                break;

            default:
                return PXActionRefusedInvalidHeaderSignature;
            }

            PXFileEndianessSet(pxResourceLoadInfo->FileCurrent, tiff->Endianness);

        }

        {
            const PXTypeEntry pxDataStreamElementList[] =
            {
                {&tiff->Version, PXTypeI16U},// Version, expect this to be "42"
                {&tiff->OffsetToIFD, PXTypeI32U}
            };

            PXFileReadMultible(pxResourceLoadInfo->FileCurrent, pxDataStreamElementList, sizeof(pxDataStreamElementList));
        }

        PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, tiff->OffsetToIFD); // Jump to adress

        // PredictSize


        // Now we are at IFD page 0
        while (!PXFileIsAtEnd(pxResourceLoadInfo->FileCurrent))
        {
            PXTIFFPage tiffPage;

            PXClear(PXTIFFPage, &tiffPage);

            PXFileReadI16UE(pxResourceLoadInfo->FileCurrent, &tiffPage.NumberOfTags, tiff->Endianness); // 2-Bytes

            tiffPage.PredictedEndPosition = PXFileDataPosition(pxResourceLoadInfo->FileCurrent) + 12u * tiffPage.NumberOfTags;

            for (PXI16U i = 0; i < tiffPage.NumberOfTags; ++i) // Read 12-Bytes
            {
                PXTIFFTag tiffTag;

                {
                    const PXTypeEntry pxDataStreamElementList[] =
                    {
                        {&tiffTag.TypeID, PXTypeI16U},// Version, expect this to be "42"
                        {&tiffTag.DataTypeID, PXTypeI16U},
                        {&tiffTag.NumberOfValues, PXTypeI32U},
                        {&tiffTag.ImageFileDataOffset, PXTypeI32U},
                    };

                    PXFileReadMultible(pxResourceLoadInfo->FileCurrent, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                }

                tiffTag.Type = PXTIFFTagTypeFromID(tiffTag.TypeID);
                tiffTag.DataType = PXTIFFTypeFromID(tiffTag.DataTypeID);

                switch (tiffTag.Type)
                {
                case PXTIFFTagNewSubFileType:
                case PXTIFFTagSubfileType:
                case PXTIFFTagImageWidth:
                {
                    tiff->Width = tiffTag.ImageFileDataOffset;

#if PXTIFFDebug
                    printf("[TIFF] Width <%i>\n", tiff->Width);
#endif

                    break;
                }
                case PXTIFFTagImageLength:
                {
                    tiff->Height = tiffTag.ImageFileDataOffset;

#if PXTIFFDebug
                    printf("[TIFF] Height <%i>\n", tiff->Height);
#endif

                    break;
                }
                case PXTIFFTagBitsPerSample:
                {
                    for (PXSize i = 0; i < tiffTag.NumberOfValues; ++i)
                    {
                        tiff->BitsPerSample[i] = tiffTag.ImageFileDataOffset; // Is this correct?
                    }

                    break;
                }
                case PXTIFFTagCompression:
                {
                    tiff->Compression = PXTIFFCompressionFromID(tiffTag.ImageFileDataOffset);
                    break;
                }
                case PXTIFFTagPhotometricInterpretation:
                {
                    tiff->PhotometricInterpretation = PXTIFFColorFormatFromID(tiffTag.ImageFileDataOffset);
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
                    break;
                case PXTIFFTagSamplesPerPixel:
                {
                    tiff->SamplesPerPixel = tiffTag.ImageFileDataOffset;
                    break;
                }
                case PXTIFFTagRowsPerStrip:
                {
                    tiff->RowsPerStrip = tiffTag.ImageFileDataOffset;
                    break;
                }
                case PXTIFFTagStripByteCounts:
                {
                    const PXI32U amount = tiffTag.ImageFileDataOffset;

                    break;
                }
                case PXTIFFTagMinSampleValue:
                    tiff->MinSampleValue = tiffTag.ImageFileDataOffset;
                    break;

                case PXTIFFTagMaxSampleValue:
                    tiff->MaxSampleValue = tiffTag.ImageFileDataOffset;
                    break;

                case PXTIFFTagXResolution:
                    tiff->MaxSampleValue = tiffTag.ImageFileDataOffset;
                    break;

                case PXTIFFTagYResolution:
                    tiff->MaxSampleValue = tiffTag.ImageFileDataOffset;
                    break;

                case PXTIFFTagPlanarConfiguration:
                {
                    switch (tiffTag.ImageFileDataOffset)
                    {
                    case 1u:
                        tiff->PlanarConfiguration = PXTIFFPlanarConfigurationChunky;
                        break;
                    case 2u:
                        tiff->PlanarConfiguration = PXTIFFPlanarConfigurationPlanar;
                        break;
                    default:
                        tiff->PlanarConfiguration = PXTIFFPlanarConfigurationInvalid;
                        break;
                    }

                    break;
                }
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
                    break;

                case PXTIFFTagSoftware:
                {
                    const PXSize oldPosition = PXFileDataPosition(pxResourceLoadInfo->FileCurrent);

                    PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, tiffTag.ImageFileDataOffset);
                    PXFileReadB(pxResourceLoadInfo->FileCurrent, tiff->Software, tiffTag.NumberOfValues);
                    PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, oldPosition);

                    break;
                }
                case PXTIFFTagDateTime:
                {
                    const PXSize oldPosition = PXFileDataPosition(pxResourceLoadInfo->FileCurrent);

                    PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, tiffTag.ImageFileDataOffset);
                    PXFileReadB(pxResourceLoadInfo->FileCurrent, tiff->DateTimeStamp, tiffTag.NumberOfValues);
                    PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, oldPosition);

                    break;
                }
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
                    break;
                case PXTIFFTagCopyright:
                {
                    if (tiffTag.ImageFileDataOffset != 0)
                    {
                        const PXSize oldPosition = PXFileDataPosition(pxResourceLoadInfo->FileCurrent);

                        PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, tiffTag.ImageFileDataOffset);
                        PXFileReadB(pxResourceLoadInfo->FileCurrent, tiff->CopyRight, tiffTag.NumberOfValues);
                        PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, oldPosition);
                    }
                    else
                    {
#if PXLogEnable
                        printf("[TIFF] Error: offset is invalid\n");
#endif
                    }

                    break;
                }

                default:
                    break;
                }
            }

            PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, tiffPage.PredictedEndPosition);

            PXFileReadI32UE(pxResourceLoadInfo->FileCurrent, &tiffPage.OffsetToNextImageFileDirectory, tiff->Endianness); // 4-Bytes
            PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, tiffPage.OffsetToNextImageFileDirectory);
        }





        // Parse pages

        //read next stuff


    }


    return PXResultOK;
}

PXResult PXAPI PXTIFFSaveToFile(PXFile PXREF pxFile, PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}

PXTIFFCompression PXAPI PXTIFFCompressionFromID(const unsigned short tiffCompressionID)
{
    switch (tiffCompressionID)
    {
    case 1u:
        return PXTIFFCompressionUncompressed;
    case 2u:
        return PXTIFFCompressionCCITT1D;
    case 3u:
        return PXTIFFCompressionGroup3Fax;
    case 4u:
        return PXTIFFCompressionGroup4Fax;

    case 5u:
        return PXTIFFCompressionLZW;
    case 6u:
        return PXTIFFCompressionPXJPEGCompression;
    case 32773u:
        return PXTIFFCompressionPackBits;

    default:
        return PXTIFFCompressionInvalid;
    }
}

PXTIFFColorFormat PXAPI PXTIFFColorFormatFromID(const unsigned short tiffColorFormatID)
{
    switch (tiffColorFormatID)
    {
    case 0u:
        return PXTIFFColorMonochromeWhiteIsZero;
    case 1u:
        return PXTIFFColorMonochromeBlackIsZero;
    case 2u:
        return PXTIFFColorRGB;
    case 3u:
        return PXTIFFColorRGBPalette;
    case 4u:
        return PXTIFFColorTransparencyMask;
    case 5u:
        return PXTIFFColorCMYK;
    case 6u:
        return PXTIFFColorYCbCr;
    case 8u:
        return PXTIFFColorCIELab;

    default:
        return PXTIFFColorInvalid;
    }
}
