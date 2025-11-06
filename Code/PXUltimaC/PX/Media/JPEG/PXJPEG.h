#pragma once

#ifndef PXJPEGIncluded
#define PXJPEGIncluded

#include <PX/Engine/PXResource.h>

#define PXJPEGQuantizationTableLuminanceIndex   0
#define PXJPEGQuantizationTableChrominanceIndex 1




#define PXJPEGChunckStartOfFrameHuffmanBaselineDCT                      0xC0 // [0xFFC0] SOF0
#define PXJPEGChunckStartOfFrameHuffmanSequentialExtendedDCT            0xC1 // [0xFFC1] SOF1
#define PXJPEGChunckStartOfFrameHuffmanProgressiveDCT                   0xC2 // [0xFFC2] SOF2
#define PXJPEGChunckStartOfFrameHuffmanSequentialLossless               0xC3 // [0xFFC3] SOF3
#define PXJPEGChunckDefineHuffmanTableList                              0xC4 // [0xFFC4] DHT
#define PXJPEGChunckStartOfFrameHuffmanDifferentialSequential           0xC5 // [0xFFC5] SOF5
#define PXJPEGChunckStartOfFrameHuffmanDifferentialProgressiveDCT       0xC6 // [0xFFC6] SOF6
#define PXJPEGChunckStartOfFrameHuffmanDifferentialLosslessSequential   0xC7 // [0xFFC7] SOF7
#define PXJPEGChunckStartOfFrameArithmeticPXJPEGExtension               0xC8 // [0xFFC8] JPG
#define PXJPEGChunckStartOfFrameArithmeticExtendedSequentialDCT         0xC9 // [0xFFC9] SOF09
#define PXJPEGChunckStartOfFrameArithmeticProgressiveDCT                0xC4 // [0xFFCA] SOF10
#define PXJPEGChunckStartOfFrameArithmeticLosslessSequential            0xCB // [0xFFCB] SOF11
#define PXJPEGChunckDefineArithmeticCoding                              0xCC // [0xFFCC] DAC
#define PXJPEGChunckStartOfFrameArithmeticDifferentialSequentialDCT     0xCD, // [0xFFCD] SOF13
#define PXJPEGChunckStartOfFrameArithmeticDifferentialProgressiveDCT    0xCE, // [0xFFCE] SOF14
#define PXJPEGChunckStartOfFrameArithmeticDifferentialLosslessSequential 0xCF, // [0xFFCF] SOF15

#define PXJPEGChunckIntervalTerminationRestart0     0xD0 // [0xFFD0] RST0
#define PXJPEGChunckIntervalTerminationRestart1     0xD1 // [0xFFD1] RST1
#define PXJPEGChunckIntervalTerminationRestart2     0xD2 // [0xFFD2] RST2
#define PXJPEGChunckIntervalTerminationRestart3     0xD3 // [0xFFD3] RST3
#define PXJPEGChunckIntervalTerminationRestart4     0xD4 // [0xFFD4] RST4
#define PXJPEGChunckIntervalTerminationRestart5     0xD5 // [0xFFD5] RST5
#define PXJPEGChunckIntervalTerminationRestart6     0xD6 // [0xFFD6] RST6
#define PXJPEGChunckIntervalTerminationRestart7     0xD7 // [0xFFD7] RST7
#define PXJPEGChunckStartOfImage                    0xD8 // [0xFFD8] SOI
#define PXJPEGChunckEndOfImage                      0xD9 // [0xFFD9] EOI
#define PXJPEGChunckStartOfScan                     0xDA // [0xFFDA] SOS
#define PXJPEGChunckDefineQuantizationTableList     0xDB // [0xFFDB] DQT
#define PXJPEGChunckDefineNumberOfLines             0xDC // [0xFFDC] DNL
#define PXJPEGChunckDefineRestartInterval           0xDD // [0xFFDD] DRI
#define PXJPEGChunckDefineHierarchicalProgression   0xDE // [0xFFDE] DHP
#define PXJPEGChunckExpandReferenceComponentList    0xDF // [0xFFDF] EXP
#define PXJPEGChunckApplicationSegment00            0xE0 // [0xFFE0] APP0
#define PXJPEGChunckApplicationSegment01            0xE1 // [0xFFE1] APP1
#define PXJPEGChunckApplicationSegment02            0xE2 // [0xFFE2] APP2
#define PXJPEGChunckApplicationSegment03            0xE3 // [0xFFE3] APP3
#define PXJPEGChunckApplicationSegment04            0xE4 // [0xFFE4] APP4
#define PXJPEGChunckApplicationSegment05            0xE5 // [0xFFE5] APP5
#define PXJPEGChunckApplicationSegment06            0xE6 // [0xFFE6] APP6
#define PXJPEGChunckApplicationSegment07            0xE7 // [0xFFE7] APP7
#define PXJPEGChunckApplicationSegment08            0xE8 // [0xFFE8] APP8
#define PXJPEGChunckApplicationSegment09            0xE9 // [0xFFE9] APP9
#define PXJPEGChunckApplicationSegment10            0xEA // [0xFFEA] APP10
#define PXJPEGChunckApplicationSegment11            0xEB // [0xFFEB] APP11
#define PXJPEGChunckApplicationSegment12            0xEC // [0xFFEC] APP12
#define PXJPEGChunckApplicationSegment13            0xED // [0xFFED] APP13
#define PXJPEGChunckApplicationSegment14            0xEE // [0xFFEE] APP14
#define PXJPEGChunckApplicationSegment15            0xEF // [0xFFEF] APP15
#define PXJPEGChunckExtension00                     0xF0 // [0xFFF0] JPG0
#define PXJPEGChunckExtension01                     0xF1 // [0xFFF1] JPG1
#define PXJPEGChunckExtension02                     0xF2 // [0xFFF2] JPG2
#define PXJPEGChunckExtension03                     0xF3 // [0xFFF3] JPG3
#define PXJPEGChunckExtension04                     0xF4 // [0xFFF4] JPG4
#define PXJPEGChunckExtension05                     0xF5 // [0xFFF5] JPG5
#define PXJPEGChunckExtension06                     0xF6 // [0xFFF6] JPG6
#define PXJPEGChunckExtension07                     0xF7 // [0xFFF7] JPG7
#define PXJPEGChunckExtension08                     0xF8 // [0xFFF8] JPG8
#define PXJPEGChunckExtension09                     0xF9 // [0xFFF9] JPG9
#define PXJPEGChunckExtension10                     0xFA // [0xFFFA] JPG10
#define PXJPEGChunckExtension11                     0xFB // [0xFFFB] JPG11
#define PXJPEGChunckExtension12                     0xFC // [0xFFFC] JPG12
#define PXJPEGChunckExtension13                     0xFD // [0xFFFD] JPG13
#define PXJPEGChunckComment                         0xFE // [0xFFFE] COM

#define PXJPEGChunckTemporary           0x01 // [0xFF01] TEM
#define PXJPEGChunckReservedUpper       0x02 // [0xFF02] to [0xFFBF] RES
#define PXJPEGChunckReservedLower       0xBF // [0xFF02] to [0xFFBF] RES

typedef struct PXJPEGFrameComponent_
{
    PXI8U ID;
    PXI8U SamplingFactorHorizonal;
    PXI8U SamplingFactorVertical;
    PXI8U QuantizationTableID;
}
PXJPEGFrameComponent;

typedef struct PXJPEGFrame_
{
    PXJPEGFrameComponent ComponentList[255];
    PXI16U Width;
    PXI16U Height;
    PXI8U Precision;
    PXI8U ComponentListSize;
}
PXJPEGFrame;

typedef struct PXJPEGFileInfo_
{
    void* ThumbnailData;
    PXI32U ThumbnailDataSize;
    PXI16U DensityX;
    PXI16U DensityY;
    PXI8U VersionMajor;
    PXI8U VersionMinor;
    PXI8U DensityUnits;
    PXI8U ThumbnailX;
    PXI8U ThumbnailY;
}
PXJPEGFileInfo;

typedef struct PXJPEGHuffmanTable_
{
    PXI8U ID;
    PXI8U Type;
}
PXJPEGHuffmanTable;

typedef struct PXJPEGScanSelector_
{
    PXI8U ID;
    PXI8U DC;
    PXI8U ACTable;
}
PXJPEGScanSelector;

typedef struct PXJPEGScanStart_
{
    PXJPEGScanSelector ScanSelector[3];
    PXI8U ScanSelectorSize;
    PXI8U SpectralSelectFrom;
    PXI8U SpectralSelectTo;
    PXI8U SuccessiveAproximation;
}
PXJPEGScanStart;

typedef struct PXJPEG_
{
    PXJPEGFileInfo FileInfo;

    PXI8U QuantizationTable[2][64];

    PXSize HuffmanTableSize;
    PXJPEGHuffmanTable* HuffmanTable;
    PXJPEGScanStart ScanStart;

    PXSize CompressedImageDataSize;
    unsigned char* CompressedImageData;

    PXSize CommentSize;
    char* Comment;
}
PXJPEG;

PXPublic void PXAPI PXJPEGDestruct(PXJPEG PXREF jpeg);

PXPublic PXSize PXAPI PXJPEGFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

PXPublic PXResult PXAPI PXJPEGLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJPEGSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
