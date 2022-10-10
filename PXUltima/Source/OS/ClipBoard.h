#ifndef ClipBoardINCLUDE
#define ClipBoardINCLUDE

#include <Format/Type.h>
#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum ClipBoardFormat_
	{
		ClipBoardFormatInvalid,
		ClipBoardFormatText,
		ClipBoardFormatBITMAP,
		ClipBoardFormatMETAFILEPICT,
		ClipBoardFormatSYLK,
		ClipBoardFormatDIF,
		ClipBoardFormatTIFF,
		ClipBoardFormatOEMTEXT,
		ClipBoardFormatDIB,
		ClipBoardFormatPALETTE,
		ClipBoardFormatPENDATA,
		ClipBoardFormatRIFF,
		ClipBoardFormatWAVE,
		ClipBoardFormatUNICODETEXT,
		ClipBoardFormatENHMETAFILE,
		ClipBoardFormatHDROP,
		ClipBoardFormatLOCALE,
		ClipBoardFormatDIBV5,
		ClipBoardFormatMAX,
		ClipBoardFormatOWNERDISPLAY,
		ClipBoardFormatDSPTEXT,
		ClipBoardFormatDSPBITMAP,
		ClipBoardFormatDSPMETAFILEPICT,
		ClipBoardFormatDSPENHMETAFILE,
		ClipBoardFormatPRIVATEFIRST,
		ClipBoardFormatPRIVATELAST,
		ClipBoardFormatGDIOBJFIRST,
		ClipBoardFormatGDIOBJLAST,
	}
	ClipBoardFormat;

	unsigned int ClipBoardFormatToID(const ClipBoardFormat clipBoardFormat);
	ClipBoardFormat ClipBoardFormatFromID(const unsigned int clipBoardFormat);

	typedef struct ClipBoard_
	{
		unsigned int __Dummy_;
	}
	ClipBoard;

	// Open clipboard, close it imedidly after using!
	// Fails if another window has the clipboard open.
	PXPublic ActionResult ClipBoardOpen(ClipBoard* const clipBoard);

	// Use this right after using!
	PXPublic ActionResult ClipBoardClose(ClipBoard* const clipBoard);

	PXPublic ActionResult ClipBoardSet(ClipBoard* const clipBoard, const ClipBoardFormat format, const void* data);

	PXPublic ActionResult ClipBoardClear(ClipBoard* const clipBoard);


#ifdef __cplusplus
}
#endif

#endif
