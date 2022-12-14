#ifndef ClipBoardINCLUDE
#define ClipBoardINCLUDE

#include <Format/Type.h>
#include <Error/PXActionResult.h>

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
	PXPublic PXActionResult ClipBoardOpen(ClipBoard* const clipBoard);

	// Use this right after using!
	PXPublic PXActionResult ClipBoardClose(ClipBoard* const clipBoard);

	PXPublic PXActionResult ClipBoardSet(ClipBoard* const clipBoard, const ClipBoardFormat format, const void* data);

	PXPublic PXActionResult ClipBoardClear(ClipBoard* const clipBoard);


#ifdef __cplusplus
}
#endif

#endif
