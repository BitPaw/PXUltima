#ifndef PXPXClipBoardINCLUDE
#define PXPXClipBoardINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

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

PXPrivate PXInt32U PXAPI PXClipBoardFormatToID(const PXClipBoardFormat PXClipBoardFormat);
PXPrivate PXClipBoardFormat PXAPI PXClipBoardFormatFromID(const PXInt32U PXClipBoardFormat);

// Open PXClipBoard, close it imedidly after using!
// Fails if another window has the PXClipBoard open.
PXPublic PXActionResult PXAPI PXClipBoardOpen(PXClipBoard* const PXClipBoard);

// Use this right after using!
PXPublic PXActionResult PXAPI PXClipBoardClose(PXClipBoard* const PXClipBoard);

PXPublic PXActionResult PXAPI PXClipBoardSet(PXClipBoard* const PXClipBoard, const PXClipBoardFormat format, const void* data);

PXPublic PXActionResult PXAPI PXClipBoardClear(PXClipBoard* const PXClipBoard);

#endif