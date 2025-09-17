#ifndef PXPXClipBoardIncluded
#define PXPXClipBoardIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>

typedef enum PXClipBoardFormat_
{
    PXClipBoardFormatInvalid,
    PXClipBoardFormatText,
    PXClipBoardFormatPictureBitmap,
    PXClipBoardFormatMetaFilePICT,
    PXClipBoardFormatSYLK,
    PXClipBoardFormatDIF,
    PXClipBoardFormatPictureTIFF,
    PXClipBoardFormatOEMTEXT,
    PXClipBoardFormatDIB,
    PXClipBoardFormatPALETTE,
    PXClipBoardFormatPENDATA,
    PXClipBoardFormatRIFF,
    PXClipBoardFormatWAVE,
    PXClipBoardFormatUNICODETEXT,
    PXClipBoardFormatENHMETAFILE,
    PXClipBoardForPXMathDROP,
    PXClipBoardFormatLOCALE,
    PXClipBoardFormatDIBV5,
    PXClipBoardFormatMAX,
    PXClipBoardFormatOWNERDISPLAY,
    PXClipBoardFormatDSPTEXT,
    PXClipBoardFormatDSPBITMAP,
    PXClipBoardFormatDSPMETAFILEPICT,
    PXClipBoardFormatDSPENHMETAFILE,
    PXClipBoardFormatPRIVATEFIRST,
    PXClipBoardFormatPRIVATELAST,
    PXClipBoardFormatGDIOBJFIRST,
    PXClipBoardFormatGDIOBJLAST,
}
PXClipBoardFormat;

typedef struct PXClipBoard_
{
    unsigned int __Dummy_;
}
PXClipBoard;

PXPrivate PXI32U PXAPI PXClipBoardFormatToID(const PXClipBoardFormat PXClipBoardFormat);
PXPrivate PXClipBoardFormat PXAPI PXClipBoardFormatFromID(const PXI32U PXClipBoardFormat);

// Open PXClipBoard, close it imedidly after using!
// Fails if another window has the PXClipBoard open.
PXPublic PXResult PXAPI PXClipBoardOpen(PXClipBoard* const PXClipBoard);

// Use this right after using!
PXPublic PXResult PXAPI PXClipBoardClose(PXClipBoard* const PXClipBoard);

PXPublic PXResult PXAPI PXClipBoardSet(PXClipBoard* const PXClipBoard, const PXClipBoardFormat format, const void* data);

PXPublic PXResult PXAPI PXClipBoardClear(PXClipBoard* const PXClipBoard);

#endif
