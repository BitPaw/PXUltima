#ifndef PXJPEGInclude
#define PXJPEGInclude

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>
#include <Media/PXImage.h>

#define PXJPEGQuantizationTableLuminanceIndex 0
#define PXJPEGQuantizationTableChrominanceIndex 1

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXJPEGMarker_
	{
		PXJPEGMarkerInvalid,

		PXJPEGMarkerStartOfFrameHuffmanBaselineDCT, // [0xFFC0] SOF0
		PXJPEGMarkerStartOfFrameHuffmanSequentialExtendedDCT, // [0xFFC1] SOF1
		PXJPEGMarkerStartOfFrameHuffmanProgressiveDCT, // [0xFFC2] SOF2
		PXJPEGMarkerStartOfFrameHuffmanSequentialLossless,// [0xFFC3] SOF3

		PXJPEGMarkerDefineHuffmanTableList, // [0xFFC4] DHT

		PXJPEGMarkerStartOfFrameHuffmanDifferentialSequential,// [0xFFC5] SOF5
		PXJPEGMarkerStartOfFrameHuffmanDifferentialProgressiveDCT,// [0xFFC6] SOF6
		PXJPEGMarkerStartOfFrameHuffmanDifferentialLosslessSequential,// [0xFFC7] SOF7

		PXJPEGMarkerStartOfFrameArithmeticPXJPEGExtension, // [0xFFC8] JPG
		PXJPEGMarkerStartOfFrameArithmeticExtendedSequentialDCT, // [0xFFC9] SOF09
		PXJPEGMarkerStartOfFrameArithmeticProgressiveDCT, // [0xFFCA] SOF10
		PXJPEGMarkerStartOfFrameArithmeticLosslessSequential, // [0xFFCB] SOF11

		PXJPEGMarkerDefineArithmeticCoding, // [0xFFCC] DAC

		PXJPEGMarkerStartOfFrameArithmeticDifferentialSequentialDCT, // [0xFFCD] SOF13
		PXJPEGMarkerStartOfFrameArithmeticDifferentialProgressiveDCT, // [0xFFCE] SOF14
		PXJPEGMarkerStartOfFrameArithmeticDifferentialLosslessSequential, // [0xFFCF] SOF15

		PXJPEGMarkerIntervalTerminationRestart0, // [0xFFD0] RST0
		PXJPEGMarkerIntervalTerminationRestart1, // [0xFFD1] RST1
		PXJPEGMarkerIntervalTerminationRestart2, // [0xFFD2] RST2
		PXJPEGMarkerIntervalTerminationRestart3, // [0xFFD3] RST3
		PXJPEGMarkerIntervalTerminationRestart4, // [0xFFD4] RST4
		PXJPEGMarkerIntervalTerminationRestart5, // [0xFFD5] RST5
		PXJPEGMarkerIntervalTerminationRestart6, // [0xFFD6] RST6
		PXJPEGMarkerIntervalTerminationRestart7, // [0xFFD7] RST7

		PXJPEGMarkerStartOfImage, // [0xFFD8] SOI
		PXJPEGMarkerEndOfImage, // [0xFFD9] EOI
		PXJPEGMarkerStartOfScan, // [0xFFDA] SOS
		PXJPEGMarkerDefineQuantizationTableList, // [0xFFDB] DQT
		PXJPEGMarkerDefineNumberOfLines, // [0xFFDC] DNL
		PXJPEGMarkerDefineRestartInterval, // [0xFFDD] DRI
		PXJPEGMarkerDefineHierarchicalProgression, // [0xFFDE] DHP
		PXJPEGMarkerExpandReferenceComponentList, // [0xFFDF] EXP

		PXJPEGMarkerApplicationSegment00, // [0xFFE0] APP0
		PXJPEGMarkerApplicationSegment01, // [0xFFE1] APP1
		PXJPEGMarkerApplicationSegment02, // [0xFFE2] APP2
		PXJPEGMarkerApplicationSegment03, // [0xFFE3] APP3
		PXJPEGMarkerApplicationSegment04, // [0xFFE4] APP4
		PXJPEGMarkerApplicationSegment05, // [0xFFE5] APP5
		PXJPEGMarkerApplicationSegment06, // [0xFFE6] APP6
		PXJPEGMarkerApplicationSegment07, // [0xFFE7] APP7
		PXJPEGMarkerApplicationSegment08, // [0xFFE8] APP8
		PXJPEGMarkerApplicationSegment09, // [0xFFE9] APP9
		PXJPEGMarkerApplicationSegment10, // [0xFFEA] APP10
		PXJPEGMarkerApplicationSegment11, // [0xFFEB] APP11
		PXJPEGMarkerApplicationSegment12, // [0xFFEC] APP12
		PXJPEGMarkerApplicationSegment13, // [0xFFED] APP13
		PXJPEGMarkerApplicationSegment14, // [0xFFEE] APP14
		PXJPEGMarkerApplicationSegment15, // [0xFFEF] APP15

		PXJPEGMarkerPXJPEGExtension00, // [0xFFF0] JPG0
		PXJPEGMarkerPXJPEGExtension01, // [0xFFF1] JPG1
		PXJPEGMarkerPXJPEGExtension02, // [0xFFF2] JPG2
		PXJPEGMarkerPXJPEGExtension03, // [0xFFF3] JPG3
		PXJPEGMarkerPXJPEGExtension04, // [0xFFF4] JPG4
		PXJPEGMarkerPXJPEGExtension05, // [0xFFF5] JPG5
		PXJPEGMarkerPXJPEGExtension06, // [0xFFF6] JPG6
		PXJPEGMarkerPXJPEGExtension07, // [0xFFF7] JPG7
		PXJPEGMarkerPXJPEGExtension08, // [0xFFF8] JPG8
		PXJPEGMarkerPXJPEGExtension09, // [0xFFF9] JPG9
		PXJPEGMarkerPXJPEGExtension10, // [0xFFFA] JPG10
		PXJPEGMarkerPXJPEGExtension11, // [0xFFFB] JPG11
		PXJPEGMarkerPXJPEGExtension12, // [0xFFFC] JPG12
		PXJPEGMarkerPXJPEGExtension13, // [0xFFFD] JPG13

		PXJPEGMarkerComment, // [0xFFFE] COM

		PXJPEGMarkerTemporary, // [0xFF01] TEM
		PXJPEGMarkerReserved, // [0xFF02] to [0xFFBF] RES
	}
	PXJPEGMarker;

	typedef struct PXJPEGFrameComponent_
	{
		unsigned char ID;
		unsigned char SamplingFactorHorizonal;
		unsigned char SamplingFactorVertical;
		unsigned char QuantizationTableID;
	}
	PXJPEGFrameComponent;

	typedef struct PXJPEGFrame_
	{
		unsigned char Precision;
		unsigned short Height;
		unsigned short Width;
		unsigned char ComponentListSize;
		PXJPEGFrameComponent ComponentList[255];
	}
	PXJPEGFrame;

	typedef struct PXJPEGFileInfo_
	{
		unsigned char VersionMajor;
		unsigned char VersionMinor;
		unsigned char DensityUnits;
		unsigned short DensityX;
		unsigned short DensityY;
		unsigned char ThumbnailX;
		unsigned char ThumbnailY;

		unsigned int ThumbnailDataSize;
		unsigned char* ThumbnailData;
	}
	PXJPEGFileInfo;

	typedef struct PXJPEGHuffmanTable_
	{
		unsigned char ID;
		unsigned char Type;
	}
	PXJPEGHuffmanTable;

	typedef struct PXJPEGScanSelector_
	{
		unsigned char ID;
		unsigned char DC;
		unsigned char ACTable;
	}
	PXJPEGScanSelector;

	typedef struct PXJPEGScanStart_
	{
		unsigned char ScanSelectorSize;
		PXJPEGScanSelector ScanSelector[3];
		unsigned char SpectralSelectFrom;
		unsigned char SpectralSelectTo;
		unsigned char SuccessiveAproximation;
	}
	PXJPEGScanStart;

	typedef struct PXJPEG_
	{
		PXJPEGFileInfo FileInfo;

		unsigned char QuantizationTable[2][64];

		PXSize HuffmanTableSize;
		PXJPEGHuffmanTable* HuffmanTable;
		PXJPEGScanStart ScanStart;

		PXSize CompressedImageDataSize;
		unsigned char* CompressedImageData;

		PXSize CommentSize;
		char* Comment;
	}
	PXJPEG;


	PXPrivate PXJPEGMarker ConvertToPXJPEGMarker(const PXInt16U jpegMarker);
	PXPrivate PXInt16U ConvertFromPXJPEGMarker(const PXJPEGMarker jpegMarker);

	PXPublic void PXJPEGConstruct(PXJPEG* const jpeg);
	PXPublic void PXJPEGDestruct(PXJPEG* const jpeg);

	PXPublic PXSize PXJPEGFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);
	
	PXPublic PXActionResult PXJPEGParseToImage(PXImage* const image, PXFile* const dataStream);
	PXPublic PXActionResult PXJPEGSerializeFromImage(const PXImage* const image, PXFile* const dataStream);

#ifdef __cplusplus
}
#endif

#endif