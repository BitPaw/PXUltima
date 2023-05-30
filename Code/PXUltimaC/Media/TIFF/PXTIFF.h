#ifndef PXPXTIFFINCLUDE
#define PXPXTIFFINCLUDE

#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXTIFFType_
	{
		PXTIFFTypeInvalid,

		PXTIFFTypeByteSigned,
		PXTIFFTypeByteUnsigned,
		PXTIFFTypeByteCustom,

		PXTIFFTypeShortSigned,
		PXTIFFTypeShortUnsigned,

		PXTIFFTypeLongSigned,
		PXTIFFTypeLongUnsigned,

		PXTIFFTypeRationalSigned,
		PXTIFFTypeRationalUnsigned,

		PXTIFFTypeFloat,
		PXTIFFTypeDouble,

		PXTIFFTypeASCII
	}
	PXTIFFType;

	typedef enum PXTIFFTagType_
	{
		PXTIFFTagTypeInvalid,
		PXTIFFTagTypeUnkown,
		PXTIFFTagNewSubFileType,
		PXTIFFTagSubfileType,
		PXTIFFTagImageWidth,
		PXTIFFTagImageLength,
		PXTIFFTagBitsPerSample,
		PXTIFFTagCompression,
		PXTIFFTagPhotometricInterpretation,
		PXTIFFTagThreshholding,//	263
		PXTIFFTagCellWidth,//264
		PXTIFFTagCellLength,//265
		PXTIFFTagFillOrder,//266
		PXTIFFTagDocumentName,//269
		PXTIFFTagImageDescription,//270
		PXTIFFTagMake,//271
		PXTIFFTagModel,//272
		PXTIFFTagStripOffsets,//273
		PXTIFFTagOrientation,//274
		PXTIFFTagSamplesPerPixel,//	277
		PXTIFFTagRowsPerStrip,//278
		PXTIFFTagStripByteCounts,//279
		PXTIFFTagMinSampleValue,//280
		PXTIFFTagMaxSampleValue,//281
		PXTIFFTagXResolution,//282
		PXTIFFTagYResolution,//283
		PXTIFFTagPlanarConfiguration,//284
		PXTIFFTagPageName,//285
		PXTIFFTagXPosition,//286
		PXTIFFTagYPosition,//287
		PXTIFFTagFreeOffsets,//288
		PXTIFFTagFreeByteCounts,//289
		PXTIFFTagGrayResponseUnit,//290
		PXTIFFTagGrayResponseCurve,//291
		PXTIFFTagT4Options,//292
		PXTIFFTagT6Options,//293
		PXTIFFTagResolutionUnit,//296
		PXTIFFTagPageNumber,//297
		PXTIFFTagTransferFunction,//301
		PXTIFFTagSoftware,//305
		PXTIFFTagDateTime,//306
		PXTIFFTagArtist,//315
		PXTIFFTagHostComputer,//316
		PXTIFFTagPredictor,//317
		PXTIFFTagWhitePoint,//318
		PXTIFFTagPrimaryChromaticities,//319
		PXTIFFTagColorMap,//320
		PXTIFFTagHalftoneHints,//321
		PXTIFFTagTileWidth,//322
		PXTIFFTagTileLength,//323
		PXTIFFTagTileOffsets,//324
		PXTIFFTagTileByteCounts,//325
		PXTIFFTagInkSet,//332
		PXTIFFTagInkNames,//333
		PXTIFFTagNumberOfInks,//	334
		PXTIFFTagDotRange,//336
		PXTIFFTagTargetPrinter,//	337
		PXTIFFTagExtraSamples,//338
		PXTIFFTagSampleFormat,//339
		PXTIFFTagSMinSampleValue,//340
		PXTIFFTagSMaxSampleValue,//341
		PXTIFFTagTransferRange,//342
		PXTIFFTagPXJPEGProc,//512
		PXTIFFTagPXJPEGInterchangeFormat,//513
		PXTIFFTagPXJPEGInterchangeFormatLngth,//514
		PXTIFFTagPXJPEGRestartInterval,//515
		PXTIFFTagPXJPEGLosslessPredictors,//	517
		PXTIFFTagPXJPEGPointTransforms,//518
		PXTIFFTagPXJPEGQTables,//519
		PXTIFFTagPXJPEGDCTables,//520
		PXTIFFTagPXJPEGACTables,//521
		PXTIFFTagYCbCrCoefficients,//529
		PXTIFFTagYCbCrSubSampling,//530
		PXTIFFTagYCbCrPositioning,//531
		PXTIFFTagReferenceBlackWhite,//	532
		PXTIFFTagCopyright//	33432
	}
	PXTIFFTagType;


	typedef enum PXTIFFColorFormat_
	{
		PXTIFFColorInvalid,
		PXTIFFColorMonochromeWhiteIsZero,
		PXTIFFColorMonochromeBlackIsZero,
		PXTIFFColorRGB,
		PXTIFFColorRGBPalette,
		PXTIFFColorTransparencyMask,
		PXTIFFColorCMYK,
		PXTIFFColorYCbCr,
		PXTIFFColorCIELab
	}
	PXTIFFColorFormat;

	typedef enum PXTIFFCompression_
	{
		PXTIFFCompressionInvalid,
		PXTIFFCompressionUncompressed,
		PXTIFFCompressionCCITT1D,
		PXTIFFCompressionGroup3Fax,
		PXTIFFCompressionGroup4Fax,
		PXTIFFCompressionLZW,
		PXTIFFCompressionPXJPEGCompression,
		PXTIFFCompressionPackBits
	}
	PXTIFFCompression;


	typedef struct PXTIFFHeader_
	{
		PXEndian Endianness;
		unsigned short Version;
		unsigned int OffsetToIFD;
	}
	PXTIFFHeader;

	typedef struct PXTIFFPage_
	{
		unsigned short NumberOfTags;
		void* TagData;
		unsigned int OffsetToNextIFD;
	}
	PXTIFFPage;





	typedef struct PXTIFFTag_
	{
		unsigned short TypeID;
		PXTIFFTagType Type;
		unsigned short DataTypeID;
		PXTIFFType DataType;
		unsigned int NumberOfValues;
		unsigned int DataOffset;
	}
	PXTIFFTag;

	// Tag Image File Format
	typedef struct PXTIFF_
	{
		unsigned int Width;
		unsigned int Height;

		PXTIFFColorFormat PhotometricInterpretation;
		PXTIFFCompression Compression;
		unsigned short SamplesPerPixel;
		unsigned short BitsPerSample[4];
	}
	PXTIFF;



	PXPrivate PXTIFFType PXTIFFTypeFromID(const unsigned char tiffTypeID);
	PXPrivate PXTIFFTagType PXTIFFTagTypeFromID(const unsigned short tiffTagTypeID);
	PXPrivate PXTIFFCompression PXTIFFCompressionFromID(const unsigned short tiffCompressionID);
	PXPrivate PXTIFFColorFormat PXTIFFColorFormatFromID(const unsigned short tiffColorFormatID);

	PXPublic PXSize PXTIFFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

	PXPublic PXActionResult PXTIFFParse(PXTIFF* const tiff, PXFile* const dataStream);
	PXPublic PXActionResult PXTIFFParseToImage(PXImage* const image, PXFile* const dataStream);

	PXPublic PXActionResult PXTIFFSerializeFromImage(const PXImage* const image, PXFile* const dataStream);

#ifdef __cplusplus
}
#endif

#endif