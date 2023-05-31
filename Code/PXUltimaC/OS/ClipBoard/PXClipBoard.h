#ifndef PXPXClipBoardINCLUDE
#define PXPXClipBoardINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

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

	PXPrivate PXInt32U PXClipBoardFormatToID(const PXClipBoardFormat PXClipBoardFormat);
	PXPrivate PXClipBoardFormat PXClipBoardFormatFromID(const PXInt32U PXClipBoardFormat);

	typedef struct PXClipBoard_
	{
		unsigned int __Dummy_;
	}
	PXClipBoard;

	// Open PXClipBoard, close it imedidly after using!
	// Fails if another window has the PXClipBoard open.
	PXPublic PXActionResult PXClipBoardOpen(PXClipBoard* const PXClipBoard);

	// Use this right after using!
	PXPublic PXActionResult PXClipBoardClose(PXClipBoard* const PXClipBoard);

	PXPublic PXActionResult PXClipBoardSet(PXClipBoard* const PXClipBoard, const PXClipBoardFormat format, const void* data);

	PXPublic PXActionResult PXClipBoardClear(PXClipBoard* const PXClipBoard);


#ifdef __cplusplus
}
#endif

#endif
