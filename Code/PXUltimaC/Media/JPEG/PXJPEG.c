#include "PXJPEG.h"

#include <Math/PXMath.h>
#include <Media/PXColor.h>
#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

#define JPGDebug 0

#if JPGDebug
#include <stdio.h>
#endif

#define PXJPEGMarkerInvalidID                                             PXInt16Make(0xFF, 0xFF)
#define PXJPEGMarkerStartOfFrameHuffmanBaselineDCTID                      PXInt16Make(0xFF, 0xC0)
#define PXJPEGMarkerStartOfFrameHuffmanSequentialExtendedDCTID            PXInt16Make(0xFF, 0xC1)
#define PXJPEGMarkerStartOfFrameHuffmanProgressiveDCTID                   PXInt16Make(0xFF, 0xC2)
#define PXJPEGMarkerStartOfFrameHuffmanSequentialLosslessID               PXInt16Make(0xFF, 0xC3)
#define PXJPEGMarkerDefineHuffmanTableListID                              PXInt16Make(0xFF, 0xC4)
#define PXJPEGMarkerStartOfFrameHuffmanDifferentialSequentialID           PXInt16Make(0xFF, 0xC5)
#define PXJPEGMarkerStartOfFrameHuffmanDifferentialProgressiveDCTID       PXInt16Make(0xFF, 0xC6)
#define PXJPEGMarkerStartOfFrameHuffmanDifferentialLosslessSequentialID   PXInt16Make(0xFF, 0xC7)
#define PXJPEGMarkerStartOfFrameArithmeticPXJPEGExtensionID                 PXInt16Make(0xFF, 0xC8)
#define PXJPEGMarkerStartOfFrameArithmeticExtendedSequentialDCTID         PXInt16Make(0xFF, 0xC9)
#define PXJPEGMarkerStartOfFrameArithmeticProgressiveDCTID                  PXInt16Make(0xFF, 0xCA)
#define PXJPEGMarkerStartOfFrameArithmeticLosslessSequentialID              PXInt16Make(0xFF, 0xCB)
#define PXJPEGMarkerDefineArithmeticCodingID                                PXInt16Make(0xFF, 0xCC)
#define PXJPEGMarkerStartOfFrameArithmeticDifferentialSequentialDCTID       PXInt16Make(0xFF, 0xCD)
#define PXJPEGMarkerStartOfFrameArithmeticDifferentialProgressiveDCTID      PXInt16Make(0xFF, 0xCE)
#define PXJPEGMarkerStartOfFrameArithmeticDifferentialLosslessSequentialID  PXInt16Make(0xFF, 0xCF)
#define PXJPEGMarkerIntervalTerminationRestart0ID PXInt16Make(0xFF, 0xD0)
#define PXJPEGMarkerIntervalTerminationRestart1ID PXInt16Make(0xFF, 0xD1)
#define PXJPEGMarkerIntervalTerminationRestart2ID PXInt16Make(0xFF, 0xD2)
#define PXJPEGMarkerIntervalTerminationRestart3ID PXInt16Make(0xFF, 0xD3)
#define PXJPEGMarkerIntervalTerminationRestart4ID PXInt16Make(0xFF, 0xD4)
#define PXJPEGMarkerIntervalTerminationRestart5ID PXInt16Make(0xFF, 0xD5)
#define PXJPEGMarkerIntervalTerminationRestart6ID PXInt16Make(0xFF, 0xD6)
#define PXJPEGMarkerIntervalTerminationRestart7ID PXInt16Make(0xFF, 0xD7)
#define PXJPEGMarkerStartOfImageID              PXInt16Make(0xFF, 0xD8)
#define PXJPEGMarkerEndOfImageID            PXInt16Make(0xFF, 0xD9)
#define PXJPEGMarkerStartOfScanID           PXInt16Make(0xFF, 0xDA)
#define PXJPEGMarkerDefineQuantizationTableListID PXInt16Make(0xFF, 0xDB)
#define PXJPEGMarkerDefineNumberOfLinesID PXInt16Make(0xFF, 0xDC)
#define PXJPEGMarkerDefineRestartIntervalID             PXInt16Make(0xFF, 0xDD)
#define PXJPEGMarkerDefineHierarchicalProgressionID     PXInt16Make(0xFF, 0xDE)
#define PXJPEGMarkerExpandReferenceComponentListID      PXInt16Make(0xFF, 0xDF)
#define PXJPEGMarkerApplicationSegment00ID PXInt16Make(0xFF, 0xE0)
#define PXJPEGMarkerApplicationSegment01ID PXInt16Make(0xFF, 0xE1)
#define PXJPEGMarkerApplicationSegment02ID PXInt16Make(0xFF, 0xE2)
#define PXJPEGMarkerApplicationSegment03ID PXInt16Make(0xFF, 0xE3)
#define PXJPEGMarkerApplicationSegment04ID PXInt16Make(0xFF, 0xE4)
#define PXJPEGMarkerApplicationSegment05ID PXInt16Make(0xFF, 0xE5)
#define PXJPEGMarkerApplicationSegment06ID PXInt16Make(0xFF, 0xE6)
#define PXJPEGMarkerApplicationSegment07ID PXInt16Make(0xFF, 0xE7)
#define PXJPEGMarkerApplicationSegment08ID PXInt16Make(0xFF, 0xE8)
#define PXJPEGMarkerApplicationSegment09ID PXInt16Make(0xFF, 0xE9)
#define PXJPEGMarkerApplicationSegment10ID PXInt16Make(0xFF, 0xEA)
#define PXJPEGMarkerApplicationSegment11ID PXInt16Make(0xFF, 0xEB)
#define PXJPEGMarkerApplicationSegment12ID PXInt16Make(0xFF, 0xEC)
#define PXJPEGMarkerApplicationSegment13ID PXInt16Make(0xFF, 0xED)
#define PXJPEGMarkerApplicationSegment14ID PXInt16Make(0xFF, 0xEE)
#define PXJPEGMarkerApplicationSegment15ID PXInt16Make(0xFF, 0xEF)
#define PXJPEGMarkerPXJPEGExtension00ID PXInt16Make(0xFF, 0xF0)
#define PXJPEGMarkerPXJPEGExtension01ID PXInt16Make(0xFF, 0xF1)
#define PXJPEGMarkerPXJPEGExtension02ID PXInt16Make(0xFF, 0xF2)
#define PXJPEGMarkerPXJPEGExtension03ID PXInt16Make(0xFF, 0xF3)
#define PXJPEGMarkerPXJPEGExtension04ID PXInt16Make(0xFF, 0xF4)
#define PXJPEGMarkerPXJPEGExtension05ID PXInt16Make(0xFF, 0xF5)
#define PXJPEGMarkerPXJPEGExtension06ID PXInt16Make(0xFF, 0xF6)
#define PXJPEGMarkerPXJPEGExtension07ID PXInt16Make(0xFF, 0xF7)
#define PXJPEGMarkerPXJPEGExtension08ID PXInt16Make(0xFF, 0xF8)
#define PXJPEGMarkerPXJPEGExtension09ID PXInt16Make(0xFF, 0xF9)
#define PXJPEGMarkerPXJPEGExtension10ID PXInt16Make(0xFF, 0xFA)
#define PXJPEGMarkerPXJPEGExtension11ID PXInt16Make(0xFF, 0xFB)
#define PXJPEGMarkerPXJPEGExtension12ID PXInt16Make(0xFF, 0xFC)
#define PXJPEGMarkerPXJPEGExtension13ID PXInt16Make(0xFF, 0xFD)
#define PXJPEGMarkerCommentID PXInt16Make(0xFF, 0xFE)
#define PXJPEGMarkerTemporaryID PXInt16Make(0xFF, 0x01)
#define PXJPEGMarkerReservedID PXInt16Make(0xFF, 0x02)

PXJPEGMarker PXAPI PXJPEGMarkerFromID(const PXInt16U jpegMarker)
{
    switch(jpegMarker)
    {
        case  PXJPEGMarkerStartOfFrameHuffmanBaselineDCTID: return PXJPEGMarkerStartOfFrameHuffmanBaselineDCT;
        case  PXJPEGMarkerStartOfFrameHuffmanSequentialExtendedDCTID: return PXJPEGMarkerStartOfFrameHuffmanSequentialExtendedDCT;
        case  PXJPEGMarkerStartOfFrameHuffmanProgressiveDCTID: return PXJPEGMarkerStartOfFrameHuffmanProgressiveDCT;
        case  PXJPEGMarkerStartOfFrameHuffmanSequentialLosslessID: return PXJPEGMarkerStartOfFrameHuffmanSequentialLossless;
        case  PXJPEGMarkerDefineHuffmanTableListID: return PXJPEGMarkerDefineHuffmanTableList;
        case  PXJPEGMarkerStartOfFrameHuffmanDifferentialSequentialID: return PXJPEGMarkerStartOfFrameHuffmanDifferentialSequential;
        case  PXJPEGMarkerStartOfFrameHuffmanDifferentialProgressiveDCTID: return PXJPEGMarkerStartOfFrameHuffmanDifferentialProgressiveDCT;
        case  PXJPEGMarkerStartOfFrameHuffmanDifferentialLosslessSequentialID: return PXJPEGMarkerStartOfFrameHuffmanDifferentialLosslessSequential;
        case  PXJPEGMarkerStartOfFrameArithmeticPXJPEGExtensionID: return PXJPEGMarkerStartOfFrameArithmeticPXJPEGExtension;
        case  PXJPEGMarkerStartOfFrameArithmeticExtendedSequentialDCTID: return PXJPEGMarkerStartOfFrameArithmeticExtendedSequentialDCT;
        case  PXJPEGMarkerStartOfFrameArithmeticProgressiveDCTID: return PXJPEGMarkerStartOfFrameArithmeticProgressiveDCT;
        case  PXJPEGMarkerStartOfFrameArithmeticLosslessSequentialID: return PXJPEGMarkerStartOfFrameArithmeticLosslessSequential;
        case  PXJPEGMarkerDefineArithmeticCodingID: return PXJPEGMarkerDefineArithmeticCoding;
        case  PXJPEGMarkerStartOfFrameArithmeticDifferentialSequentialDCTID: return PXJPEGMarkerStartOfFrameArithmeticDifferentialSequentialDCT;
        case  PXJPEGMarkerStartOfFrameArithmeticDifferentialProgressiveDCTID: return PXJPEGMarkerStartOfFrameArithmeticDifferentialProgressiveDCT;
        case  PXJPEGMarkerStartOfFrameArithmeticDifferentialLosslessSequentialID: return PXJPEGMarkerStartOfFrameArithmeticDifferentialLosslessSequential;
        case  PXJPEGMarkerIntervalTerminationRestart0ID: return PXJPEGMarkerIntervalTerminationRestart0;
        case  PXJPEGMarkerIntervalTerminationRestart1ID: return PXJPEGMarkerIntervalTerminationRestart1;
        case  PXJPEGMarkerIntervalTerminationRestart2ID: return PXJPEGMarkerIntervalTerminationRestart2;
        case  PXJPEGMarkerIntervalTerminationRestart3ID: return PXJPEGMarkerIntervalTerminationRestart3;
        case  PXJPEGMarkerIntervalTerminationRestart4ID: return PXJPEGMarkerIntervalTerminationRestart4;
        case  PXJPEGMarkerIntervalTerminationRestart5ID: return PXJPEGMarkerIntervalTerminationRestart5;
        case  PXJPEGMarkerIntervalTerminationRestart6ID: return PXJPEGMarkerIntervalTerminationRestart6;
        case  PXJPEGMarkerIntervalTerminationRestart7ID: return PXJPEGMarkerIntervalTerminationRestart7;
        case  PXJPEGMarkerStartOfImageID: return PXJPEGMarkerStartOfImage;
        case  PXJPEGMarkerEndOfImageID: return PXJPEGMarkerEndOfImage;
        case  PXJPEGMarkerStartOfScanID: return PXJPEGMarkerStartOfScan;
        case  PXJPEGMarkerDefineQuantizationTableListID: return PXJPEGMarkerDefineQuantizationTableList;
        case  PXJPEGMarkerDefineNumberOfLinesID: return PXJPEGMarkerDefineNumberOfLines;
        case  PXJPEGMarkerDefineRestartIntervalID: return PXJPEGMarkerDefineRestartInterval;
        case  PXJPEGMarkerDefineHierarchicalProgressionID: return PXJPEGMarkerDefineHierarchicalProgression;
        case  PXJPEGMarkerExpandReferenceComponentListID: return PXJPEGMarkerExpandReferenceComponentList;
        case  PXJPEGMarkerApplicationSegment00ID: return PXJPEGMarkerApplicationSegment00;
        case  PXJPEGMarkerApplicationSegment01ID: return PXJPEGMarkerApplicationSegment01;
        case  PXJPEGMarkerApplicationSegment02ID: return PXJPEGMarkerApplicationSegment02;
        case  PXJPEGMarkerApplicationSegment03ID: return PXJPEGMarkerApplicationSegment03;
        case  PXJPEGMarkerApplicationSegment04ID: return PXJPEGMarkerApplicationSegment04;
        case  PXJPEGMarkerApplicationSegment05ID: return PXJPEGMarkerApplicationSegment05;
        case  PXJPEGMarkerApplicationSegment06ID: return PXJPEGMarkerApplicationSegment06;
        case  PXJPEGMarkerApplicationSegment07ID: return PXJPEGMarkerApplicationSegment07;
        case  PXJPEGMarkerApplicationSegment08ID: return PXJPEGMarkerApplicationSegment08;
        case  PXJPEGMarkerApplicationSegment09ID: return PXJPEGMarkerApplicationSegment09;
        case  PXJPEGMarkerApplicationSegment10ID: return PXJPEGMarkerApplicationSegment10;
        case  PXJPEGMarkerApplicationSegment11ID: return PXJPEGMarkerApplicationSegment11;
        case  PXJPEGMarkerApplicationSegment12ID: return PXJPEGMarkerApplicationSegment12;
        case  PXJPEGMarkerApplicationSegment13ID: return PXJPEGMarkerApplicationSegment13;
        case  PXJPEGMarkerApplicationSegment14ID: return PXJPEGMarkerApplicationSegment14;
        case  PXJPEGMarkerApplicationSegment15ID: return PXJPEGMarkerApplicationSegment15;
        case  PXJPEGMarkerPXJPEGExtension00ID: return PXJPEGMarkerPXJPEGExtension00;
        case  PXJPEGMarkerPXJPEGExtension01ID: return PXJPEGMarkerPXJPEGExtension01;
        case  PXJPEGMarkerPXJPEGExtension02ID: return PXJPEGMarkerPXJPEGExtension02;
        case  PXJPEGMarkerPXJPEGExtension03ID: return PXJPEGMarkerPXJPEGExtension03;
        case  PXJPEGMarkerPXJPEGExtension04ID: return PXJPEGMarkerPXJPEGExtension04;
        case  PXJPEGMarkerPXJPEGExtension05ID: return PXJPEGMarkerPXJPEGExtension05;
        case  PXJPEGMarkerPXJPEGExtension06ID: return PXJPEGMarkerPXJPEGExtension06;
        case  PXJPEGMarkerPXJPEGExtension07ID: return PXJPEGMarkerPXJPEGExtension07;
        case  PXJPEGMarkerPXJPEGExtension08ID: return PXJPEGMarkerPXJPEGExtension08;
        case  PXJPEGMarkerPXJPEGExtension09ID: return PXJPEGMarkerPXJPEGExtension09;
        case  PXJPEGMarkerPXJPEGExtension10ID: return PXJPEGMarkerPXJPEGExtension10;
        case  PXJPEGMarkerPXJPEGExtension11ID: return PXJPEGMarkerPXJPEGExtension11;
        case  PXJPEGMarkerPXJPEGExtension12ID: return PXJPEGMarkerPXJPEGExtension12;
        case  PXJPEGMarkerPXJPEGExtension13ID: return PXJPEGMarkerPXJPEGExtension13;
        case  PXJPEGMarkerCommentID: return PXJPEGMarkerComment;
        case  PXJPEGMarkerTemporaryID: return PXJPEGMarkerTemporary;
        case  PXJPEGMarkerReservedID: return PXJPEGMarkerReserved;

        default:
            return PXJPEGMarkerInvalid;
    }
}

PXInt16U PXAPI PXJPEGMarkerToID(const PXJPEGMarker jpegMarker)
{
    switch(jpegMarker)
    {
        case  PXJPEGMarkerStartOfFrameHuffmanBaselineDCT: return PXJPEGMarkerStartOfFrameHuffmanBaselineDCTID;
        case  PXJPEGMarkerStartOfFrameHuffmanSequentialExtendedDCT: return PXJPEGMarkerStartOfFrameHuffmanSequentialExtendedDCTID;
        case  PXJPEGMarkerStartOfFrameHuffmanProgressiveDCT: return PXJPEGMarkerStartOfFrameHuffmanProgressiveDCTID;
        case  PXJPEGMarkerStartOfFrameHuffmanSequentialLossless: return PXJPEGMarkerStartOfFrameHuffmanSequentialLosslessID;
        case  PXJPEGMarkerDefineHuffmanTableList: return PXJPEGMarkerDefineHuffmanTableListID;
        case  PXJPEGMarkerStartOfFrameHuffmanDifferentialSequential: return PXJPEGMarkerStartOfFrameHuffmanDifferentialSequentialID;
        case  PXJPEGMarkerStartOfFrameHuffmanDifferentialProgressiveDCT: return PXJPEGMarkerStartOfFrameHuffmanDifferentialProgressiveDCTID;
        case  PXJPEGMarkerStartOfFrameHuffmanDifferentialLosslessSequential: return PXJPEGMarkerStartOfFrameHuffmanDifferentialLosslessSequentialID;
        case  PXJPEGMarkerStartOfFrameArithmeticPXJPEGExtension: return PXJPEGMarkerStartOfFrameArithmeticPXJPEGExtensionID;
        case  PXJPEGMarkerStartOfFrameArithmeticExtendedSequentialDCT: return PXJPEGMarkerStartOfFrameArithmeticExtendedSequentialDCTID;
        case  PXJPEGMarkerStartOfFrameArithmeticProgressiveDCT: return PXJPEGMarkerStartOfFrameArithmeticProgressiveDCTID;
        case  PXJPEGMarkerStartOfFrameArithmeticLosslessSequential: return PXJPEGMarkerStartOfFrameArithmeticLosslessSequentialID;
        case  PXJPEGMarkerDefineArithmeticCoding: return PXJPEGMarkerDefineArithmeticCodingID;
        case  PXJPEGMarkerStartOfFrameArithmeticDifferentialSequentialDCT: return PXJPEGMarkerStartOfFrameArithmeticDifferentialSequentialDCTID;
        case  PXJPEGMarkerStartOfFrameArithmeticDifferentialProgressiveDCT: return PXJPEGMarkerStartOfFrameArithmeticDifferentialProgressiveDCTID;
        case  PXJPEGMarkerStartOfFrameArithmeticDifferentialLosslessSequential: return PXJPEGMarkerStartOfFrameArithmeticDifferentialLosslessSequentialID;
        case  PXJPEGMarkerIntervalTerminationRestart0: return PXJPEGMarkerIntervalTerminationRestart0ID;
        case  PXJPEGMarkerIntervalTerminationRestart1: return PXJPEGMarkerIntervalTerminationRestart1ID;
        case  PXJPEGMarkerIntervalTerminationRestart2: return PXJPEGMarkerIntervalTerminationRestart2ID;
        case  PXJPEGMarkerIntervalTerminationRestart3: return PXJPEGMarkerIntervalTerminationRestart3ID;
        case  PXJPEGMarkerIntervalTerminationRestart4: return PXJPEGMarkerIntervalTerminationRestart4ID;
        case  PXJPEGMarkerIntervalTerminationRestart5: return PXJPEGMarkerIntervalTerminationRestart5ID;
        case  PXJPEGMarkerIntervalTerminationRestart6: return PXJPEGMarkerIntervalTerminationRestart6ID;
        case  PXJPEGMarkerIntervalTerminationRestart7: return PXJPEGMarkerIntervalTerminationRestart7ID;
        case  PXJPEGMarkerStartOfImage: return PXJPEGMarkerStartOfImageID;
        case  PXJPEGMarkerEndOfImage: return PXJPEGMarkerEndOfImageID;
        case  PXJPEGMarkerStartOfScan: return PXJPEGMarkerStartOfScanID;
        case  PXJPEGMarkerDefineQuantizationTableList: return PXJPEGMarkerDefineQuantizationTableListID;
        case  PXJPEGMarkerDefineNumberOfLines: return PXJPEGMarkerDefineNumberOfLinesID;
        case  PXJPEGMarkerDefineRestartInterval: return PXJPEGMarkerDefineRestartIntervalID;
        case  PXJPEGMarkerDefineHierarchicalProgression: return PXJPEGMarkerDefineHierarchicalProgressionID;
        case  PXJPEGMarkerExpandReferenceComponentList: return PXJPEGMarkerExpandReferenceComponentListID;
        case  PXJPEGMarkerApplicationSegment00: return PXJPEGMarkerApplicationSegment00ID;
        case  PXJPEGMarkerApplicationSegment01: return PXJPEGMarkerApplicationSegment01ID;
        case  PXJPEGMarkerApplicationSegment02: return PXJPEGMarkerApplicationSegment02ID;
        case  PXJPEGMarkerApplicationSegment03: return PXJPEGMarkerApplicationSegment03ID;
        case  PXJPEGMarkerApplicationSegment04: return PXJPEGMarkerApplicationSegment04ID;
        case  PXJPEGMarkerApplicationSegment05: return PXJPEGMarkerApplicationSegment05ID;
        case  PXJPEGMarkerApplicationSegment06: return PXJPEGMarkerApplicationSegment06ID;
        case  PXJPEGMarkerApplicationSegment07: return PXJPEGMarkerApplicationSegment07ID;
        case  PXJPEGMarkerApplicationSegment08: return PXJPEGMarkerApplicationSegment08ID;
        case  PXJPEGMarkerApplicationSegment09: return PXJPEGMarkerApplicationSegment09ID;
        case  PXJPEGMarkerApplicationSegment10: return PXJPEGMarkerApplicationSegment10ID;
        case  PXJPEGMarkerApplicationSegment11: return PXJPEGMarkerApplicationSegment11ID;
        case  PXJPEGMarkerApplicationSegment12: return PXJPEGMarkerApplicationSegment12ID;
        case  PXJPEGMarkerApplicationSegment13: return PXJPEGMarkerApplicationSegment13ID;
        case  PXJPEGMarkerApplicationSegment14: return PXJPEGMarkerApplicationSegment14ID;
        case  PXJPEGMarkerApplicationSegment15: return PXJPEGMarkerApplicationSegment15ID;
        case  PXJPEGMarkerPXJPEGExtension00: return PXJPEGMarkerPXJPEGExtension00ID;
        case  PXJPEGMarkerPXJPEGExtension01: return PXJPEGMarkerPXJPEGExtension01ID;
        case  PXJPEGMarkerPXJPEGExtension02: return PXJPEGMarkerPXJPEGExtension02ID;
        case  PXJPEGMarkerPXJPEGExtension03: return PXJPEGMarkerPXJPEGExtension03ID;
        case  PXJPEGMarkerPXJPEGExtension04: return PXJPEGMarkerPXJPEGExtension04ID;
        case  PXJPEGMarkerPXJPEGExtension05: return PXJPEGMarkerPXJPEGExtension05ID;
        case  PXJPEGMarkerPXJPEGExtension06: return PXJPEGMarkerPXJPEGExtension06ID;
        case  PXJPEGMarkerPXJPEGExtension07: return PXJPEGMarkerPXJPEGExtension07ID;
        case  PXJPEGMarkerPXJPEGExtension08: return PXJPEGMarkerPXJPEGExtension08ID;
        case  PXJPEGMarkerPXJPEGExtension09: return PXJPEGMarkerPXJPEGExtension09ID;
        case  PXJPEGMarkerPXJPEGExtension10: return PXJPEGMarkerPXJPEGExtension10ID;
        case  PXJPEGMarkerPXJPEGExtension11: return PXJPEGMarkerPXJPEGExtension11ID;
        case  PXJPEGMarkerPXJPEGExtension12: return PXJPEGMarkerPXJPEGExtension12ID;
        case  PXJPEGMarkerPXJPEGExtension13: return PXJPEGMarkerPXJPEGExtension13ID;
        case  PXJPEGMarkerComment: return PXJPEGMarkerCommentID;
        case  PXJPEGMarkerTemporary: return PXJPEGMarkerTemporaryID;
        case  PXJPEGMarkerReserved: return PXJPEGMarkerReservedID;

        case PXJPEGMarkerInvalid:
        default:
            return PXJPEGMarkerInvalidID;
    }
}

PXSize PXAPI PXJPEGFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    const PXSize beginning = 2;
    const PXSize end = 2;
    const PXSize app0 = 20;

    const PXSize sum = beginning + end + app0 + (height * width * 3u) + 0xFFFF * 2;

    return sum;
}

void PXAPI PXJPEGDestruct(PXJPEG* const jpeg)
{

}

PXActionResult PXAPI PXJPEGLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXJPEG jpeXg;
    PXJPEG* jpeg = &jpeXg;

    PXClear(PXJPEG, &jpeg);

    // Check Start of Image
    {
        PXInt16UCluster startBlock;

        PXFileReadB(pxResourceLoadInfo->FileReference, startBlock.Data, 2u);

        const PXJPEGMarker marker = PXJPEGMarkerFromID(startBlock.Value);
        const PXBool validStart = PXJPEGMarkerStartOfImage == marker;

        if(!validStart)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

#if JPGDebug
        printf("[i][JPG] Start of Image\n");
#endif
    }

    while(!PXFileIsAtEnd(pxResourceLoadInfo->FileReference))
    {
        PXJPEGMarker chunkMarker = PXJPEGMarkerInvalid;
        PXInt16U chunkLength = -1;
        PXSize expectedOffset = -1;

        // Parse info
        {
            PXInt16UCluster markerData;

            PXFileReadB(pxResourceLoadInfo->FileReference, markerData.Data, 2u);

            chunkMarker = PXJPEGMarkerFromID(markerData.Value);

            if(chunkMarker == PXJPEGMarkerEndOfImage)
            {
#if JPGDebug
                printf("[i][JPG] End of Image\n");
#endif

                return PXActionSuccessful;
            }

            PXFileReadI16UE(pxResourceLoadInfo->FileReference, &chunkLength, PXEndianBig);

            chunkLength -= 2u; // dont count header

            expectedOffset = pxResourceLoadInfo->FileReference->DataCursor + chunkLength;

#if JPGDebug

            unsigned int percentage = (pxFile->DataCursor / (float)pxFile->DataSize) * 100;

            printf("\n[i][JPG] Chunk <%2x%2x> deteced. Size:%i Bytes (Parsed : %i%%)\n", markerData.A, markerData.B, chunkLength, percentage);
#endif
        }

        switch(chunkMarker)
        {
            case PXJPEGMarkerStartOfImage:
            {
                // We read the start tag already. Reading it again is not valid.
                return PXActionFailedFormatNotAsExpected;
            }

            case PXJPEGMarkerStartOfFrameHuffmanBaselineDCT:
            {
                PXJPEGFrame frame;

                // Read frame
                {
                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {&frame.Precision,PXDataTypeInt08U},
                        {&frame.Height,PXDataTypeInt16UBE},
                        {&frame.Width,PXDataTypeInt16UBE},
                        {&frame.ComponentListSize,PXDataTypeInt08U}
                    };

                    PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                }

                for(PXSize i = 0; i < frame.ComponentListSize; ++i)
                {
                    PXJPEGFrameComponent* frameComponent = &frame.ComponentList[i];
                    unsigned char samplingFactor = 0;

                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {&frameComponent->ID,PXDataTypeInt08U},
                        {&samplingFactor,PXDataTypeInt08U},
                        {&frameComponent->QuantizationTableID,PXDataTypeInt08U}
                    };

                    PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                    frameComponent->SamplingFactorHorizonal = ((samplingFactor & 0b11110000) >> 4u);
                    frameComponent->SamplingFactorVertical = (samplingFactor & 0b00001111);
                }

#if JPGDebug
                printf
                (
                    "[i][JPG] Start of Frame (DCT Baseline)\n"
                    " | Precision  : %3i |\n"
                    " | Size       : %i, %i |\n"
                    " | Components : %3i |\n",
                    frame.Precision,
                    frame.Width,
                    frame.Height,
                    frame.ComponentListSize
                );
#endif

                break;
            }
            case PXJPEGMarkerDefineQuantizationTableList:
            {
                PXSize remainingBytes = chunkLength;

                while(remainingBytes)
                {
                    PXInt8U precision = 0;
                    PXInt8U matixID = 0;

                    {
                        PXInt8U cluster = 0;

                        remainingBytes -= PXFileReadI8U(pxResourceLoadInfo->FileReference, &cluster);

                        precision = (cluster & 0b11110000) >> 4;
                        matixID = (cluster & 0b00001111);
                    }

                    PXInt8U* const matrixAdress = (PXInt8U*)&jpeg->QuantizationTable[matixID];

                    remainingBytes -= PXFileReadB(pxResourceLoadInfo->FileReference, matrixAdress, sizeof(PXInt8U) * 64u);

#if JPGDebug
                    printf("[i][JPG] Define Quantization Table <%i>\n", matixID);



                    for(PXSize y = 0; y < 8u; ++y)
                    {
                        printf("|");

                        for(PXSize x = 0; x < 8u; ++x)
                        {
                            printf("%3i |", matrixAdress[y * 8u + x]);
                        }

                        printf("\n");
                    }
#endif
                }

                break;
            }
            case PXJPEGMarkerDefineHuffmanTableList:
            {
                PXSize remainingBytes = chunkLength;

                while(remainingBytes)
                {
                    PXJPEGHuffmanTable jpegHuffmanTable;

                    {
                        PXInt8U huffmanTableInfo;

                        remainingBytes -= PXFileReadI8U(pxResourceLoadInfo->FileReference, &huffmanTableInfo);

                        jpegHuffmanTable.ID = (huffmanTableInfo & 0b00001111);
                        jpegHuffmanTable.Type = (huffmanTableInfo & 0b00010000) >> 4u;
                        // unused 0b11100000
                    }
#if JPGDebug
                    printf
                    (
                        "[i][JPG] Define Huffman table\n"
                        " | ID   : %2i \n"
                        " | Type : %2i \n",
                        jpegHuffmanTable.ID,
                        jpegHuffmanTable.Type
                    );
#endif

                    PXInt16U symbolSum = 0;

                    // 16 Bytes symbopls
                    for(PXInt8U i = 0; i < 16u; ++i)
                    {
                        PXInt8U symbol = 0;

                        remainingBytes -= PXFileReadI8U(pxResourceLoadInfo->FileReference, &symbol);

#if JPGDebug
                        printf
                        (
                            " | Symbol length <%zi>\n",
                            symbol
                        );
#endif

                        symbolSum += symbol;
                    }

#if JPGDebug
                    printf
                    (
                        " | Symbol sum <%zi>\n",
                        symbolSum
                    );
#endif


                    // n bytes from that data
                    for(PXInt16U i = 0; i < symbolSum; ++i)
                    {
                        PXInt8U symbol = 0;

                        remainingBytes -= PXFileReadI8U(pxResourceLoadInfo->FileReference, &symbol);

#if JPGDebug
                        printf
                        (
                            " | Symbol <%i>\n",
                            symbol
                        );
#endif

                        // Save?
                    }

                }
                break;
            }
            case PXJPEGMarkerStartOfScan:
            {
                PXFileReadI8U(pxResourceLoadInfo->FileReference, &jpeg->ScanStart.ScanSelectorSize);

                for(PXInt8U i = 0; i < jpeg->ScanStart.ScanSelectorSize; ++i)
                {
                    PXJPEGScanSelector* scanSelector = &jpeg->ScanStart.ScanSelector[i];
                    PXInt8U huffmanTableUsed = 0;

                    PXFileReadI8U(pxResourceLoadInfo->FileReference, &scanSelector->ID);
                    PXFileReadI8U(pxResourceLoadInfo->FileReference, &huffmanTableUsed);

                    scanSelector->DC = ((huffmanTableUsed & 0b11110000) >> 4u);
                    scanSelector->ACTable = (huffmanTableUsed & 0b00001111);
                }

                PXFileCursorAdvance(pxResourceLoadInfo->FileReference, 3u); // mandatorily to skip these, why?

                // Compressed image data starts here --------------------------------

                //PXFileReadI8U(&PXFile, &jpeg->ScanStart.SpectralSelectFrom);
                //PXFileReadI8U(&PXFile, &jpeg->ScanStart.SpectralSelectTo);
                //PXFileReadI8U(&PXFile, &jpeg->ScanStart.SuccessiveAproximation);

                //jpeg->CompressedImageDataSize = PXFileRemainingSize(&PXFile) - 2u;
                //jpeg->CompressedImageData = MemoryAllocate(sizeof(unsigned char) * jpeg->CompressedImageDataSize);

                //PXFileReadB(&PXFile, jpeg->CompressedImageData, jpeg->CompressedImageDataSize);

                const PXSize imageDataSize = PXFileRemainingSize(pxResourceLoadInfo->FileReference) - 2u;

                // Correct expected offset, as the "chunk length" seems to be only considering the data iself and not the whole chunk.

                expectedOffset += imageDataSize;

#if JPGDebug
                printf
                (
                    "[i][JPG] Start of Scan...\n"
                    " - Image Data <%i> Bytes\n",
                    imageDataSize
                );
#endif
                PXFileCursorAdvance(pxResourceLoadInfo->FileReference, imageDataSize);

                break;
            }
            case PXJPEGMarkerApplicationSegment00:
            {
                char identifier[5];

                PXFileReadB(pxResourceLoadInfo->FileReference, identifier, 5u);

                {
                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {&jpeg->FileInfo.VersionMajor, PXDataTypeInt08U},
                        {&jpeg->FileInfo.VersionMinor, PXDataTypeInt08U},
                        {&jpeg->FileInfo.DensityUnits, PXDataTypeInt08U},
                        {&jpeg->FileInfo.DensityX,PXDataTypeInt16UBE},
                        {&jpeg->FileInfo.DensityY,PXDataTypeInt16UBE},
                        {&jpeg->FileInfo.ThumbnailX, PXDataTypeInt08U},
                        {&jpeg->FileInfo.ThumbnailY, PXDataTypeInt08U},
                    };

                    PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                }

#if JPGDebug
                printf
                (
                    "[i][JPG] Header Info\n"
                    " | Identifier   : %8s |\n"
                    " | Version      : %3i, %3i |\n"
                    " | Density Unit : %8i |\n"
                    " | Density      : %3i, %3i |\n"
                    " | Thumbnail    : %3i, %3i |\n",
                    identifier,
                    jpeg->FileInfo.VersionMajor,
                    jpeg->FileInfo.VersionMinor,
                    jpeg->FileInfo.DensityUnits,
                    jpeg->FileInfo.DensityX,
                    jpeg->FileInfo.DensityY,
                    jpeg->FileInfo.ThumbnailX,
                    jpeg->FileInfo.ThumbnailY
                );
#endif

                // Load Thumbnail
                {
                    const PXBool hasThumbnail = jpeg->FileInfo.ThumbnailX > 0 && jpeg->FileInfo.ThumbnailY > 0;

                    if(hasThumbnail)
                    {
                        const PXSize size = jpeg->FileInfo.ThumbnailX * jpeg->FileInfo.ThumbnailY * 3u;

                        PXNewList(PXByte, size, &jpeg->FileInfo.ThumbnailData, &jpeg->FileInfo.ThumbnailDataSize);

                        PXFileReadB(pxResourceLoadInfo->FileReference, jpeg->FileInfo.ThumbnailData, jpeg->FileInfo.ThumbnailDataSize);
                    }
                }

                break;
            }
            case PXJPEGMarkerComment:
            {        
                PXNewList(char, chunkLength, &jpeg->Comment, &jpeg->CommentSize);

                PXFileReadB(pxResourceLoadInfo->FileReference, jpeg->Comment, chunkLength);

                break;
            }
        }

#if JPGDebug
        if(pxFile->DataCursor != expectedOffset)
        {
            printf("[!][JPG] Chunk has unhandled data! Skipping <%zi> Bytes\n", expectedOffset - pxFile->DataCursor);
        }
#endif
        //--<Allign>----
        pxResourceLoadInfo->FileReference->DataCursor = expectedOffset;
        //--------------
    }

    return PXActionSuccessful;
}


// represent a few bits, typically a Huffman code
typedef struct BitCode_
{
    // BitCode() {}       // undefined state, must be initialized at a later time
    // BitCode(PXInt16U code_, PXInt8U numBits_) : code(code_), numBits(numBits_) {}
    PXInt16U code;     // PXJPEG's Huffman codes are limited to 16 bits
    PXInt8U  numBits;  // actual number of bits
}
BitCode;

// store the most recently encoded bits that are not written yet
typedef struct BitBuffer_
{
    // BitBuffer()        // actually, there will be only one instance of this object
     //    : bits(0), numBits(0) {}
    PXInt32S bits;      // actually only at most 24 bits are used
    PXInt8U numBits;   // number of valid bits (the right-most bits)
}
BitBuffer;

// quantization tables from PXJPEG Standard, Annex K
// there are a few experts proposing slightly more efficient values, e.g. https://www.imagemagick.org/discourse-server/viewtopic.php?t=20333
// btw: Google's Guetzli project optimizes the quantization tables per image
const unsigned char DefaultQuantLuminance[8 * 8] =
{ 16, 11, 10, 16, 24, 40, 51, 61,
  12, 12, 14, 19, 26, 58, 60, 55,
  14, 13, 16, 24, 40, 57, 69, 56,
  14, 17, 22, 29, 51, 87, 80, 62,
  18, 22, 37, 56, 68,109,103, 77,
  24, 35, 55, 64, 81,104,113, 92,
  49, 64, 78, 87,103,121,120,101,
  72, 92, 95, 98,112,100,103, 99 };
const unsigned char DefaultQuantChrominance[8 * 8] =
{ 17, 18, 24, 47, 99, 99, 99, 99,
  18, 21, 26, 66, 99, 99, 99, 99,
  24, 26, 56, 99, 99, 99, 99, 99,
  47, 66, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99 };

// 8x8 blocks are processed in zig-zag order
// most encoders use a zig-zag table, I switched to its inverse for performance reasons
// note: ZigZagInv[ZigZag[i]] = i
const PXInt8U ZigZagInv[8 * 8] =
{ 
    0, 1, 8,16, 9, 2, 3,10,     // ZigZag[] =  0, 1, 5, 6,14,15,27,28,
    17,24,32,25,18,11, 4, 5,    //             2, 4, 7,13,16,26,29,42,
    12,19,26,33,40,48,41,34,    //             3, 8,12,17,25,30,41,43,
    27,20,13, 6, 7,14,21,28,    //             9,11,18,24,31,40,44,53,
    35,42,49,56,57,50,43,36,    //            10,19,23,32,39,45,52,54,
    29,22,15,23,30,37,44,51,    //            20,22,33,38,46,51,55,60,
    58,59,52,45,38,31,39,46,    //            21,34,37,47,50,56,59,61,
    53,60,61,54,47,55,62,63     //            35,36,48,49,57,58,62,63
}; 

// some constants for our DCT
#define SqrtHalfSqrt 1.306562965f // sqrt((2 + sqrt(2)) / 2)  = cos(pi * 1 / 8) * sqrt(2)
#define HalfSqrtSqrt  0.382683432f // sqrt( 2 - sqrt(2)) / 2   = cos(pi * 3 / 8)
#define InvSqrt 0.707106781f // 1 / sqrt(2)              = cos(pi * 2 / 8)
#define InvSqrtSqrt 0.541196100f // 1 / sqrt(2 - sqrt(2))    = cos(pi * 3 / 8) * sqrt(2)
    // scaling constants for AAN DCT algorithm: AanScaleFactors[0] = 1, AanScaleFactors[k=1..7] = cos(k*PI/16) * sqrt(2)
const float AanScaleFactors[8] = { 1, 1.387039845f, SqrtHalfSqrt, 1.175875602f, 1, 0.785694958f, InvSqrtSqrt, 0.275899379f };

// use Huffman code tables recommended by the PXJPEG standard Annex K
// - they work quite well for most images but are not optimal
// - CodesPerBitsize tables define how many Huffman codes will have a certain bitsize (plus 1),
//   e.g. DcLuminanceCodesPerBitsize[2] = 5 because there are 5 Huffman codes being 2+1=3 bits long
// - Values tables are a list of values ordered by their Huffman code bitsize,
//   e.g. AcLuminanceValues => Huffman(0x01,0x02,0x03) will have 2 bits, Huffman(0x00) will have 3 bits, Huffman(0x04,0x11,0x05) will have 4 bits, ...

// Huffman definitions for first DC/AC tables (luminance / Y channel)
const PXInt8U DcLuminanceCodesPerBitsize[16] = { 0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };   // sum = 12
const PXInt8U DcLuminanceValues[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };         // => 12 codes
const PXInt8U AcLuminanceCodesPerBitsize[16] = { 0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,125 }; // sum = 162
const PXInt8U AcLuminanceValues[162] =                                        // => 162 codes
{ 0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08, // 16*10+2 because
  0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28, // upper 4 bits can be 0..F
  0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59, // while lower 4 bits can be 1..A
  0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89, // plus two special codes 0x00 and 0xF0
  0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6, // order of these symbols was determined empirically by PXJPEG committee
  0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,
  0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };
// Huffman definitions for second DC/AC tables (chrominance / Cb and Cr channels)
const PXInt8U DcChrominanceCodesPerBitsize[16] = { 0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };   // sum = 12
const PXInt8U DcChrominanceValues[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };         // => 12 codes (identical to DcLuminanceValues)
const PXInt8U AcChrominanceCodesPerBitsize[16] = { 0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,119 }; // sum = 162
const PXInt8U AcChrominanceValues[162] =                                        // => 162 codes
{ 0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91, // same number of symbol, just different order
  0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26, // (which is more efficient for AC coding)
  0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,
  0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,
  0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,
  0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,
  0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };

// ////////////////////////////////////////
// helper functions / templates


// write bits stored in BitCode, keep excess bits in BitBuffer
void writeBits(PXFile* PXFile, BitBuffer* buffer, BitCode data)
{
    // append the new bits to those bits leftover from previous call(s)
    buffer->numBits += data.numBits;
    buffer->bits <<= data.numBits;
    buffer->bits |= data.code;

    // write all "full" bytes
    while(buffer->numBits >= 8)
    {
        // extract highest 8 bits
        buffer->numBits -= 8;
        const PXInt8U oneByte = (buffer->bits >> buffer->numBits) & 0xFF;
        PXFileWriteI8U(PXFile, oneByte);

        if(oneByte == 0xFF) // 0xFF has a special meaning for PXJPEGs (it's a block marker)
            PXFileWriteI8U(PXFile, 0);         // therefore pad a zero to indicate "nope, this one ain't a marker, it's just a coincidence"

          // note: I don't clear those written bits, therefore buffer.bits contains garbage in the high bits
          //       if you really want to "clean up" (e.g. for debugging purposes) then uncomment the following line
          //buffer.bits &= (1 << buffer.numBits) - 1;
    }
}

// convert to a PXJPEG codeword
void convertCode(BitCode* bitCode, int value)
{
    // positive value: code = value,     numBits = position of highest set bit
    // negative value: ignore sign, then numBits = position of highest set bit, and code = (2^numBits) - 1 + value
    int absolute = value >= 0 ? +value : -value; // by the way: value is never zero
    int mask = 0;       // will be 2^numBits - 1
    // find position of highest set bit, fast way for GCC: result.numBits = 32 - __builtin_clz(value);
    int numBits = 0;
    while(absolute > mask)
    {
        numBits++;
        mask = 2 * mask + 1; // append a set bit (numBits increased by one, so we need to update 2^numBits - 1)
    }

    // remember: mask = 2^numBits - 1, so if the original value was negative we can cheaply compute code = (2^numBits) - 1 + value = mask + value
    bitCode->code = value >= 0 ? value : value + mask;
    bitCode->numBits = numBits;;
}

// forward DCT computation "in one dimension" (fast AAN algorithm: Arai, Agui and Nakajima: "A fast DCT-SQ scheme for images")
// stride must be 1 (=horizontal) or 8 (=vertical)
void DCT(int stride, float block[8 * 8])
{
    // modify in-place
    float* block0 = &block[0 * stride];
    float* block1 = &block[1 * stride];
    float* block2 = &block[2 * stride];
    float* block3 = &block[3 * stride];
    float* block4 = &block[4 * stride];
    float* block5 = &block[5 * stride];
    float* block6 = &block[6 * stride];
    float* block7 = &block[7 * stride];

    // based on https://dev.w3.org/Amaya/libjpeg/jfdctflt.c , the original variable names can be found in my comments
    float add07 = *block0 + *block7; float sub07 = *block0 - *block7; // tmp0, tmp7
    float add16 = *block1 + *block6; float sub16 = *block1 - *block6; // tmp1, tmp6
    float add25 = *block2 + *block5; float sub25 = *block2 - *block5; // tmp2, tmp5
    float add34 = *block3 + *block4; float sub34 = *block3 - *block4; // tmp3, tmp4

    float add0347 = add07 + add34; float sub07_34 = add07 - add34; // tmp10, tmp13 ("even part" / "phase 2")
    float add1256 = add16 + add25; float sub16_25 = add16 - add25; // tmp11, tmp12

    *block0 = add0347 + add1256; *block4 = add0347 - add1256; // "phase 3"

    float z1 = (sub16_25 + sub07_34) * InvSqrt; // all temporary z-variables kept their original names
    *block2 = sub07_34 + z1; *block6 = sub07_34 - z1; // "phase 5"

    float sub23_45 = sub25 + sub34; // tmp10 ("odd part" / "phase 2")
    float sub12_56 = sub16 + sub25; // tmp11
    float sub01_67 = sub16 + sub07; // tmp12

    float z5 = (sub23_45 - sub01_67) * HalfSqrtSqrt;
    float z2 = sub23_45 * InvSqrtSqrt + z5;
    float z3 = sub12_56 * InvSqrt;
    float z4 = sub01_67 * SqrtHalfSqrt + z5;
    float z6 = sub07 + z3; // z11 ("phase 5")
    float z7 = sub07 - z3; // z13
    *block1 = z6 + z4; *block7 = z6 - z4; // "phase 6"
    *block5 = z7 + z2; *block3 = z7 - z2;
}

// run DCT, quantize and write Huffman bit codes
short PXJPEGEncodeBlock
(
    PXFile* PXFile,
    BitBuffer* buffer,
    float block[8][8],
    const float scaled[8 * 8],
    PXInt16S lastDC,
    const BitCode huffmanDC[256],
    const BitCode huffmanAC[256]
)
{

    // "linearize" the 8x8 block, treat it as a flat array of 64 floats
    float* block64 = (float*)block;

    // DCT: rows
    for(PXInt8U offset = 0; offset < 8u; ++offset)
        DCT(1u, block64 + offset * 8);
    // DCT: columns
    for(PXInt8U offset = 0; offset < 8u; ++offset)
        DCT(8u, block64 + offset * 1);

    // scale
    for(PXInt8U i = 0; i < 8 * 8; i++)
        block64[i] *= scaled[i];

    // encode DC (the first coefficient is the "average color" of the 8x8 block)
    // convert to an integer
    PXInt16S DC = (PXInt16S)(block64[0] + (block64[0] >= 0 ? +0.5f : -0.5f)); // C++11's nearbyint() achieves a similar effect
    // same "average color" as previous block ?
    if(DC == lastDC)
        writeBits(PXFile, buffer, huffmanDC[0x00]); // yes, write a special short symbol
    else
    {
        BitCode bitCode;
        convertCode(&bitCode, DC - lastDC);       // nope, encode the difference to previous block's average color
        writeBits(PXFile, buffer, huffmanDC[bitCode.numBits]);
        writeBits(PXFile, buffer, bitCode);
    }

    // quantize and zigzag the other 63 coefficients
    PXSize posNonZero = 0; // find last coefficient which is not zero (because trailing zeros are encoded very efficiently)
    PXInt16S quantized[8 * 8];
    for(PXSize i = 1; i < 8 * 8; i++) // start at 1 because block64[0]=DC was already processed
    {
        float value = block64[ZigZagInv[i]];
        // round to nearest integer (actually, rounding is performed by casting from float to int16)
        quantized[i] = (PXInt16S)(value + (value >= 0 ? +0.5f : -0.5f)); // C++11's nearbyint() achieves a similar effect
        // remember offset of last non-zero coefficient
        if(quantized[i] != 0)
            posNonZero = i;
    }

    // encode ACs (Q[1..63])
    for(PXSize i = 1; i <= posNonZero; i++) // Q[0] was already written, start at Q[1] and skip all trailing zeros
    {
        // zeros are encoded in a special way
        PXSize offset = 0; // upper 4 bits count the number of consecutive zeros
        while(quantized[i] == 0) // found a few zeros, let's count them
        {
            i++;
            offset += 1 << 4; // add 1 to the upper 4 bits
            // split into blocks of at most 16 consecutive zeros
            if(offset > 15 << 4) // remember, the counter is in the upper 4 bits
            {
                offset = 0;
                writeBits(PXFile, buffer, huffmanAC[0xF0]); // 0xF0 is a special code for "16 zeros"
            }
        }

        // merge number of zeros with the number of bits of the next non-zero value
        BitCode bitCode;
        convertCode(&bitCode, quantized[i]);
        offset += bitCode.numBits;
        writeBits(PXFile, buffer, huffmanAC[offset]);
        writeBits(PXFile, buffer, bitCode);
    }

    // send end-of-block code (0x00), only needed if there are trailing zeros
    if(posNonZero < 8 * 8 - 1) // = 63
        writeBits(PXFile, buffer, huffmanAC[0x00]);

    return DC;
}

// Jon's code includes the pre-generated Huffman codes
// I don't like these "magic constants" and compute them on my own :-)
void generateHuffmanTable(const PXInt8U numCodes[16], const PXInt8U* values, BitCode result[256])
{
    PXInt16U huffmanCode = 0; // no PXJPEG Huffman code exceeds 16 bits
    // process all bitsizes 1 thru 16
    for(PXInt8U numBits = 1; numBits <= 16; numBits++)
    {
        // ... and each code of these bitsizes
        for(PXInt8U i = 0; i < numCodes[numBits - 1]; i++) // note: numCodes array starts at zero, but smallest bitsize is 1
        {
            const PXInt8U current = *values++;
            result[current].code = huffmanCode++;
            result[current].numBits = numBits;
        }
        // next Huffman code needs to be one bit wider
        huffmanCode <<= 1;
    }
}

PXActionResult PXAPI PXJPEGSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceSaveInfo->FileReference;

    unsigned char isRGB = 1u;
    unsigned char quality = 100u;
    unsigned char downsample = 0;
    const char* comment = 0;

    quality = PXMathLimitCU(quality, 1u, 100u);
    // formula taken from libjpeg
    quality = quality < 50 ? 5000 / quality : 200 - quality * 2;

    // Header Signature
    {
        PXFileWriteI16UE(pxResourceSaveInfo->FileReference, PXJPEGMarkerStartOfImageID, EndianCurrentSystem);
    }

    // APP0
    {
        PXJPEGFileInfo jpegFileInfo;
        jpegFileInfo.VersionMajor = 1u;
        jpegFileInfo.VersionMinor = 1u;
        jpegFileInfo.DensityUnits = 0;
        jpegFileInfo.DensityX = 1;
        jpegFileInfo.DensityY = 1;
        jpegFileInfo.ThumbnailX = 0;
        jpegFileInfo.ThumbnailY = 0;
        jpegFileInfo.ThumbnailDataSize = 0;
        jpegFileInfo.ThumbnailData = 0;

        const char PXJPEGApp0[4] = { 'J', 'F', 'I', 'F' };

        const PXInt16U segmentID = PXJPEGMarkerApplicationSegment00ID;
        const PXInt16U val = 16u;
        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {(void*)&segmentID, PXDataTypeInt16UBE},
            {(void*)&val, PXDataTypeInt16UBE},
            {(void*)PXJPEGApp0, PXDataTypeDatax4},
            {PXNull, PXDataTypePadding(1)},
            {&jpegFileInfo.VersionMajor, PXDataTypeInt08U},
            {&jpegFileInfo.VersionMinor, PXDataTypeInt08U},
            {&jpegFileInfo.DensityUnits, PXDataTypeInt08U},
            {&jpegFileInfo.DensityX, PXDataTypeInt16UBE},
            {&jpegFileInfo.DensityY, PXDataTypeInt16UBE},
            {&jpegFileInfo.ThumbnailX, PXDataTypeInt08U},
            {&jpegFileInfo.ThumbnailY, PXDataTypeInt08U}
        };

        PXFileReadMultible(pxResourceSaveInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    // ////////////////////////////////////////
    // adjust quantization tables to desired quality
    PXInt8U quantLuminance[8 * 8];
    PXInt8U quantChrominance[8 * 8];

    for(PXInt8U i = 0; i < 8 * 8; i++)
    {
        const PXInt8U luminance = (DefaultQuantLuminance[ZigZagInv[i]] * quality + 50u) / 100u;
        const PXInt8U chrominance = (DefaultQuantChrominance[ZigZagInv[i]] * quality + 50u) / 100u;

        // clamp to 1..255
        quantLuminance[i] = (PXMathLimitCU(luminance, 1u, 255u));
        quantChrominance[i] = (PXMathLimitCU(chrominance, 1u, 255u));
    }

    // write quantization tables
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, PXJPEGMarkerDefineQuantizationTableListID, EndianCurrentSystem);
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, 2 + (isRGB ? 2 : 1) * (1 + 8 * 8), PXEndianBig);  // length: 65 bytes per table + 2 bytes for this length field

    // each table has 64 entries and is preceded by an ID byte
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0u); // first  quantization table
    PXFileWriteB(pxResourceSaveInfo->FileReference, quantLuminance, sizeof(PXInt8U) * 64u);

    if(isRGB)// chrominance is only relevant for color images
    {
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, 1u);  // second quantization table
        PXFileWriteB(pxResourceSaveInfo->FileReference, quantChrominance, sizeof(PXInt8U) * 64u);
    }

    // ////////////////////////////////////////
    // write image infos (SOF0 - start of frame)
    // length: 6 bytes general info + 3 per channel + 2 bytes for this length field
    const PXInt8U numComponents = isRGB ? 3 : 1;

    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, PXJPEGMarkerStartOfFrameHuffmanBaselineDCTID, EndianCurrentSystem);
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, 2 + 6 + 3 * numComponents, PXEndianBig);


    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 8u); // 8 bits per channel

    // image dimensions (big-endian)
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, pxImage->Height, PXEndianBig);
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, pxImage->Width, PXEndianBig);

    // sampling and quantization tables for each component
            // 1 component (grayscale, Y only) or 3 components (Y,Cb,Cr)
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, numComponents);


    for(PXInt8U id = 1; id <= numComponents; ++id)
    {
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, id);                    // component ID (Y=1, Cb=2, Cr=3)
        // bitmasks for sampling: highest 4 bits: horizontal, lowest 4 bits: vertical
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, id == 1 && downsample ? 0x22 : 0x11); // 0x11 is default YCbCr 4:4:4 and 0x22 stands for YCbCr 4:2:0
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, id == 1 ? 0 : 1);       // use quantization table 0 for Y, else table 1
    }

    // ////////////////////////////////////////
    // Huffman tables
    // DHT marker - define Huffman tables
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, PXJPEGMarkerDefineHuffmanTableListID, EndianCurrentSystem);
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, isRGB ? (2 + 208 + 208) : (2 + 208), PXEndianBig);


    // 2 bytes for the length field, store chrominance only if needed
    //   1+16+12  for the DC luminance
    //   1+16+162 for the AC luminance   (208 = 1+16+12 + 1+16+162)
    //   1+16+12  for the DC chrominance
    //   1+16+162 for the AC chrominance (208 = 1+16+12 + 1+16+162, same as above)

// store luminance's DC+AC Huffman table definitions
    // highest 4 bits: 0 => DC, lowest 4 bits: 0 => Y (baseline)
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0x00);

    PXFileWriteB(pxResourceSaveInfo->FileReference, DcLuminanceCodesPerBitsize, sizeof(DcLuminanceCodesPerBitsize));
    PXFileWriteB(pxResourceSaveInfo->FileReference, DcLuminanceValues, sizeof(DcLuminanceValues));

    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0x10);// highest 4 bits: 1 => AC, lowest 4 bits: 0 => Y (baseline)

    PXFileWriteB(pxResourceSaveInfo->FileReference, AcLuminanceCodesPerBitsize, sizeof(AcLuminanceCodesPerBitsize));
    PXFileWriteB(pxResourceSaveInfo->FileReference, AcLuminanceValues, sizeof(AcLuminanceValues));


    // compute actual Huffman code tables (see Jon's code for precalculated tables)
    BitCode huffmanLuminanceDC[256];
    BitCode huffmanLuminanceAC[256];
    generateHuffmanTable(DcLuminanceCodesPerBitsize, DcLuminanceValues, huffmanLuminanceDC);
    generateHuffmanTable(AcLuminanceCodesPerBitsize, AcLuminanceValues, huffmanLuminanceAC);

    // chrominance is only relevant for color images
    BitCode huffmanChrominanceDC[256];
    BitCode huffmanChrominanceAC[256];
    if(isRGB)
    {
        // store luminance's DC+AC Huffman table definitions
        // highest 4 bits: 0 => DC, lowest 4 bits: 1 => Cr,Cb (baseline)
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0x01);

        PXFileWriteB(pxResourceSaveInfo->FileReference, DcChrominanceCodesPerBitsize, sizeof(DcChrominanceCodesPerBitsize));
        PXFileWriteB(pxResourceSaveInfo->FileReference, DcChrominanceValues, sizeof(DcChrominanceValues));


        // highest 4 bits: 1 => AC, lowest 4 bits: 1 => Cr,Cb (baseline)
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0x11);

        PXFileWriteB(pxResourceSaveInfo->FileReference, AcChrominanceCodesPerBitsize, sizeof(AcChrominanceCodesPerBitsize));
        PXFileWriteB(pxResourceSaveInfo->FileReference, AcChrominanceValues, sizeof(AcChrominanceValues));

        // compute actual Huffman code tables (see Jon's code for precalculated tables)
        generateHuffmanTable(DcChrominanceCodesPerBitsize, DcChrominanceValues, huffmanChrominanceDC);
        generateHuffmanTable(AcChrominanceCodesPerBitsize, AcChrominanceValues, huffmanChrominanceAC);
    }

    // ////////////////////////////////////////
    // start of scan (there is only a single scan for baseline PXJPEGs)
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, PXJPEGMarkerStartOfScanID, EndianCurrentSystem);
    PXFileWriteI16UE(pxResourceSaveInfo->FileReference, 2 + 1 + 2 * numComponents + 3, PXEndianBig);


    // assign Huffman tables to each component
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, numComponents);

    for(PXInt8U id = 1; id <= numComponents; ++id)
    {
        // component ID (Y=1, Cb=2, Cr=3)
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, id);
        // highest 4 bits: DC Huffman table, lowest 4 bits: AC Huffman table
        PXFileWriteI8U(pxResourceSaveInfo->FileReference, id == 1 ? 0x00 : 0x11); // Y: tables 0 for DC and AC; Cb + Cr: tables 1 for DC and AC
    }

    // constant values for our baseline PXJPEGs with a single sequential scan
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0u); // spectral selection: must start at  0
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 63u); // spectral selection: must stop  at 63
    PXFileWriteI8U(pxResourceSaveInfo->FileReference, 0u); // successive approximation: must be  0

    // adjust quantization tables with AAN scaling factors to simplify DCT
    float scaledLuminance[64u];
    float scaledChrominance[64u];

    for(PXInt8U i = 0; i < 64u; ++i)
    {
        const PXInt8U row = ZigZagInv[i] / 8; // same as i >> 3
        const PXInt8U column = ZigZagInv[i] % 8; // same as i &  7
        const float factor = 1 / (AanScaleFactors[row] * AanScaleFactors[column] * 8);
        scaledLuminance[ZigZagInv[i]] = factor / quantLuminance[i];
        scaledChrominance[ZigZagInv[i]] = factor / quantChrominance[i];
        // if you really want PXJPEGs that are bitwise identical to Jon's code then you need slightly different formulas (note: sqrt(8) = 2.828427125f)
        //const float aasf[] = { 1.0f * 2.828427125f, 1.387039845f * 2.828427125f, 1.306562965f * 2.828427125f, 1.175875602f * 2.828427125f, 1.0f * 2.828427125f, 0.785694958f * 2.828427125f, 0.541196100f * 2.828427125f, 0.275899379f * 2.828427125f }; // line 240 of jo_jpeg.cpp
        //scaledLuminance  [ZigZagInv[i]] = 1 / (quantLuminance  [i] * aasf[row] * aasf[column]); // lines 266-267 of jo_jpeg.cpp
        //scaledChrominance[ZigZagInv[i]] = 1 / (quantChrominance[i] * aasf[row] * aasf[column]);
    }

    // all encoded bits pass through this buffer, it writes to output whenever a byte is completed
    BitBuffer buffer;

    buffer.bits = 0;      // actually only at most 24 bits are used
    buffer.numBits = 0;   // number of valid bits (the right-most bits)

    // just convert image data from void*
    const unsigned char* pixels = (unsigned char*)pxImage->PixelData;

    // break down the image into 8x8 blocks and convert from RGB or grayscale to YCbCr color space
    float Y[8][8], Cb[8][8], Cr[8][8];
    // average color of the previous 8x8 block
    PXInt16S lastYDC = 0, lastCbDC = 0, lastCrDC = 0;

    // downsampling of Cb and Cr channels, if sampling = 2 then 2x2 samples are used
    const PXSize sampling = downsample ? 2 : 1;
    const PXSize numSamples = sampling * sampling; // 1 (grayscale, YCbCr444) or 4 (YCbCr420)
    // only for downsampled: sum of four pixels' red, green, blue components
    float red[8][8];
    float green[8][8];
    float blue[8][8]; // PXInt16U works, too, but float is faster

    PXMemoryClear(red, sizeof(float) * 8 * 8);
    PXMemoryClear(green, sizeof(float) * 8 * 8);
    PXMemoryClear(blue, sizeof(float) * 8 * 8);

    // process MCUs (minimum codes units)
    for(PXSize mcuY = 0; mcuY < pxImage->Height; mcuY += 8u * sampling)
        for(PXSize mcuX = 0; mcuX < pxImage->Width; mcuX += 8u * sampling)
        {
            // YCbCr 4:4:4 format: each MCU is a 8x8 block - the same applies to grayscale images, too
            // YCbCr 4:2:0 format: each MCU represents a 16x16 block, stored as 4x 8x8 Y-blocks plus 1x 8x8 Cb and 1x 8x8 Cr blocks)
            for(PXSize block = 0; block < numSamples; block++) // this loop is iterated once (grayscale, 4:4:4) or four times (4:2:0)
            {
                // subdivide into 8x8 blocks where blockX and blockY indicate the minimum x and y of the current block
                PXSize blockX = 8 * (block & 1); // same as 8 * (block % 2) => { 0, 8, 0, 8 }
                PXSize blockY = 4 * (block & 2); // same as 8 * (block / 2) => { 0, 0, 8, 8 }

                // now we finally have an 8x8 block ...
                for(PXSize deltaY = 0; deltaY < 8; ++deltaY)
                    for(PXSize deltaX = 0; deltaX < 8; ++deltaX)
                    {
                        // find actual pixel position within the current image
                        const PXSize column = PXMathMinimumIU(mcuX + blockX + deltaX, pxImage->Width - 1); // must not exceed image borders, replicate last row/column if needed
                        const PXSize row = PXMathMinimumIU(mcuY + blockY + deltaY, pxImage->Height - 1);
                        PXSize pixelPos = row * pxImage->Width + column; // the cast ensures that we don't run into multiplication overflows

                        // grayscale images have solely a Y channel which can be easily derived from the input pixel by shifting it by 128
                        if(!isRGB)
                        {
                            Y[deltaY][deltaX] = pixels[pixelPos] - 128.f;
                            continue;
                        }

                        // RGB: 3 bytes per pixel (whereas grayscale images have only 1 byte per pixel)
                        pixelPos *= numComponents;
                        float r = pixels[pixelPos];
                        float g = pixels[pixelPos + 1];
                        float b = pixels[pixelPos + 2];

                        // compute Y channel
                        Y[deltaY][deltaX] = PXColorRGBToY(r, g, b) - 128; // again, the PXJPEG standard requires Y to be shifted by 128

                        if(downsample)
                        {
                            // defer Cb/Cr computation if YCbCr420 mode: we must average 2x2 pixels, so let's "shrink" a 16x16 area to 8x8
                            PXSize x = (blockX + deltaX) / 2;
                            PXSize y = (blockY + deltaY) / 2;

                            // add red, green, blue (note: each number should be weighted by 1/4, this is done later)
                            red[y][x] += r;
                            green[y][x] += g;
                            blue[y][x] += b;
                        }
                        else
                        {
                            // YCbCr444: computation of Cb and Cr is similar to Y channel but without the need to subtract 128
                            Cb[deltaY][deltaX] = PXColorRGBToCb(r, g, b);
                            Cr[deltaY][deltaX] = PXColorRGBToCr(r, g, b);
                        }
                    }

                // encode Y channel
                lastYDC = PXJPEGEncodeBlock(pxResourceSaveInfo->FileReference, &buffer, Y, scaledLuminance, lastYDC, huffmanLuminanceDC, huffmanLuminanceAC);
            }

            if(!isRGB)
                continue;

            // ////////////////////////////////////////
            // YCbCr420 / downsampled: convert summed RGB values to Cb and Cr
            if(downsample)
            {
                for(PXInt8U y = 0; y < 8u; ++y)
                {
                    for(PXInt8U x = 0; x < 8u; ++x)
                    {
                        // each number in the arrays "red", "green" and "blue" consists of the summed values of four pixels
                        // so I still have to divide them by 4 to get their average value
                        float r = red[y][x] / 4.f; // numSamples = 4
                        float g = green[y][x] / 4.f;
                        float b = blue[y][x] / 4.f;

                        Cb[y][x] = PXColorRGBToCb(r, g, b);
                        Cr[y][x] = PXColorRGBToCr(r, g, b);

                        // reset arrays for next block
                        red[y][x] = 0x00;
                        green[y][x] = 0x00;
                        blue[y][x] = 0x00;
                    }
                }
            }


            // encode Cb + Cr channels
            lastCbDC = PXJPEGEncodeBlock(pxResourceSaveInfo->FileReference, &buffer, Cb, scaledChrominance, lastCbDC, huffmanChrominanceDC, huffmanChrominanceAC);
            lastCrDC = PXJPEGEncodeBlock(pxResourceSaveInfo->FileReference, &buffer, Cr, scaledChrominance, lastCrDC, huffmanChrominanceDC, huffmanChrominanceAC);
        }

    // fill remaining bits with 1s
    BitCode bitCode;
    bitCode.code = 0x7F;
    bitCode.numBits = 7;

    writeBits(pxResourceSaveInfo->FileReference, &buffer, bitCode); // seven set bits: 0x7F = binary 0111 1111

   // PXFileWriteI8U(&PXFile, &buffer);
   // PXFileWriteI16U(&PXFile, );

    // End Tag
    {
        PXFileWriteI16UE(pxResourceSaveInfo->FileReference, PXJPEGMarkerEndOfImageID, PXEndianLittle);
    }

    return PXActionSuccessful;
}
