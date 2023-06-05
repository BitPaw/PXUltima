#include "PXClipBoard.h"

#include <OS/System/PXOSVersion.h>

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <WinUser.h>

#define PXClipBoardFormatTEXTID CF_TEXT
#define PXClipBoardFormatBITMAPID CF_BITMAP
#define PXClipBoardFormatMETAFILEPICTID CF_METAFILEPICT
#define PXClipBoardFormatSYLKID CF_SYLK
#define PXClipBoardFormatDIFID CF_DIF
#define PXClipBoardFormatPXTIFFID   CF_TIFF
#define PXClipBoardFormatOEMTEXTID CF_OEMTEXT
#define PXClipBoardFormatDIBID CF_DIB
#define PXClipBoardFormatPALETTEID  CF_PALETTE
#define PXClipBoardFormatPENDATAID CF_PENDATA
#define PXClipBoardFormatPXRIFFID CF_RIFF
#define PXClipBoardFormatWAVEID CF_WAVE
#define PXClipBoardFormatUNICODETEXTID CF_UNICODETEXT
#define PXClipBoardFormatENHMETAFILEID CF_ENHMETAFILE
#define PXClipBoardFormatOWNERDISPLAYID CF_OWNERDISPLAY
#define PXClipBoardFormatDSPTEXTID   CF_DSPTEXT
#define PXClipBoardFormatDSPBITMAPID CF_DSPBITMAP
#define PXClipBoardFormatDSPMETAFILEPICTID CF_DSPMETAFILEPICT
#define PXClipBoardFormatDSPENHMETAFILEID  CF_DSPENHMETAFILE
#define PXClipBoardFormatPRIVATEFIRSTID CF_PRIVATEFIRST
#define PXClipBoardFormatPRIVATELASTID CF_PRIVATELAST
#define PXClipBoardFormatGDIOBJFIRSTID CF_GDIOBJFIRST
#define PXClipBoardFormatGDIOBJLASTID CF_GDIOBJLAST

#if WindowsAtleastNT
#define PXClipBoardForPXMathDROPID CF_HDROP
#define PXClipBoardFormatLOCALEID CF_LOCALE
#else
#define PXClipBoardForPXMathDROP 15
#define PXClipBoardFormatLOCALE 16
#endif

#if WindowsAtleast2000
#define PXClipBoardFormatDIBV5ID CF_DIBV5
#else
#define PXClipBoardFormatDIBV5 17
#endif

#if WindowsAtleast2000
#define PXClipBoardFormatMAXID CF_MAX
#elif WindowsAtleastNT
#define PXClipBoardFormatMAX 17
#else
#define PXClipBoardFormatMAX 15
#endif

#endif

PXInt32U PXClipBoardFormatToID(const PXClipBoardFormat clipBoardFormat)
{

#if OSUnix
	return -1;

#elif OSWindows
	switch (clipBoardFormat)
	{
	default:
	case PXClipBoardFormatInvalid:
		return -1;

	case PXClipBoardFormatText:
		return PXClipBoardFormatTEXTID;

	case PXClipBoardFormatPictureBitmap:
		return PXClipBoardFormatBITMAPID;

	case PXClipBoardFormatMetaFilePICT:
		return PXClipBoardFormatMETAFILEPICTID;

	case PXClipBoardFormatSYLK:
		return PXClipBoardFormatSYLKID;

	case PXClipBoardFormatDIF:
		return PXClipBoardFormatDIFID;

	case PXClipBoardFormatPictureTIFF:
		return PXClipBoardFormatPXTIFFID;

	case PXClipBoardFormatOEMTEXT:
		return PXClipBoardFormatOEMTEXTID;

	case PXClipBoardFormatDIB:
		return PXClipBoardFormatDIBID;

	case PXClipBoardFormatPALETTE:
		return PXClipBoardFormatPALETTEID;

	case PXClipBoardFormatPENDATA:
		return PXClipBoardFormatPENDATAID;

	case PXClipBoardFormatRIFF:
		return PXClipBoardFormatPXRIFFID;

	case PXClipBoardFormatWAVE:
		return PXClipBoardFormatWAVEID;

	case PXClipBoardFormatUNICODETEXT:
		return PXClipBoardFormatUNICODETEXTID;

	case PXClipBoardFormatENHMETAFILE:
		return PXClipBoardFormatENHMETAFILEID;

//	case PXClipBoardForPXMathDROP:
//		return PXClipBoardForPXMathDROPID;

//	case PXClipBoardFormatLOCALE:
//		return PXClipBoardFormatLOCALEID;

//	case PXClipBoardFormatDIBV5:
//		return PXClipBoardFormatDIBV5ID;

//	case PXClipBoardFormatMAX:
//		return PXClipBoardFormatMAXID;

	case PXClipBoardFormatOWNERDISPLAY:
		return PXClipBoardFormatOWNERDISPLAYID;

	case PXClipBoardFormatDSPTEXT:
		return PXClipBoardFormatDSPTEXTID;

	case PXClipBoardFormatDSPBITMAP:
		return PXClipBoardFormatDSPBITMAPID;

	case PXClipBoardFormatDSPMETAFILEPICT:
		return PXClipBoardFormatDSPMETAFILEPICTID;

	case PXClipBoardFormatDSPENHMETAFILE:
		return PXClipBoardFormatDSPENHMETAFILEID;

	case PXClipBoardFormatPRIVATEFIRST:
		return PXClipBoardFormatPRIVATEFIRSTID;

	case PXClipBoardFormatPRIVATELAST:
		return PXClipBoardFormatPRIVATELASTID;

	case PXClipBoardFormatGDIOBJFIRST:
		return PXClipBoardFormatGDIOBJFIRSTID;

	case PXClipBoardFormatGDIOBJLAST:
		return PXClipBoardFormatGDIOBJLASTID;
	}
#endif
}

PXClipBoardFormat PXClipBoardFormatFromID(const PXInt32U clipBoardFormat)
{
#if OSUnix
    return PXActionInvalid;

#elif OSWindows
	switch (clipBoardFormat)
	{
	case PXClipBoardFormatTEXTID:
		return PXClipBoardFormatText;

	case PXClipBoardFormatBITMAPID:
		return PXClipBoardFormatPictureBitmap;

	case PXClipBoardFormatMETAFILEPICTID:
		return PXClipBoardFormatMetaFilePICT;

	case PXClipBoardFormatSYLKID:
		return PXClipBoardFormatSYLK;

	case PXClipBoardFormatDIFID:
		return PXClipBoardFormatDIF;

	case PXClipBoardFormatPXTIFFID:
		return PXClipBoardFormatPictureTIFF;

	case PXClipBoardFormatOEMTEXTID:
		return PXClipBoardFormatOEMTEXT;

	case PXClipBoardFormatDIBID:
		return PXClipBoardFormatDIB;

	case PXClipBoardFormatPALETTEID:
		return PXClipBoardFormatPALETTE;

	case PXClipBoardFormatPENDATAID:
		return PXClipBoardFormatPENDATA;

	case PXClipBoardFormatPXRIFFID:
		return PXClipBoardFormatRIFF;

	case PXClipBoardFormatWAVEID:
		return PXClipBoardFormatWAVE;

	case PXClipBoardFormatUNICODETEXTID:
		return PXClipBoardFormatUNICODETEXT;

	case PXClipBoardFormatENHMETAFILEID:
		return PXClipBoardFormatENHMETAFILE;

//	case PXClipBoardForPXMathDROPID:
//		return PXClipBoardForPXMathDROP;

//	case PXClipBoardFormatLOCALEID:
//		return PXClipBoardFormatLOCALE;

//	case PXClipBoardFormatDIBV5ID:
//		return PXClipBoardFormatDIBV5;

//	case PXClipBoardFormatMAXID:
//		return PXClipBoardFormatMAX;

	case PXClipBoardFormatOWNERDISPLAYID:
		return PXClipBoardFormatOWNERDISPLAY;

	case PXClipBoardFormatDSPTEXTID:
		return PXClipBoardFormatDSPTEXT;

	case PXClipBoardFormatDSPBITMAPID:
		return PXClipBoardFormatDSPBITMAP;

	case PXClipBoardFormatDSPMETAFILEPICTID:
		return PXClipBoardFormatDSPMETAFILEPICT;

	case PXClipBoardFormatDSPENHMETAFILEID:
		return PXClipBoardFormatDSPENHMETAFILE;

	case PXClipBoardFormatPRIVATEFIRSTID:
		return PXClipBoardFormatPRIVATEFIRST;

	case PXClipBoardFormatPRIVATELASTID:
		return PXClipBoardFormatPRIVATELAST;

	case PXClipBoardFormatGDIOBJFIRSTID:
		return PXClipBoardFormatGDIOBJFIRST;

	case PXClipBoardFormatGDIOBJLASTID:
		return PXClipBoardFormatGDIOBJLAST;

	default:
		return PXClipBoardFormatInvalid;
	}
#endif // defined
}

PXActionResult PXClipBoardOpen(PXClipBoard* const clipBoard)
{
#if OSUnix
    return PXActionNotImplemented;

#elif PXOSWindowsDestop
	HWND pxWindowID = NULL;
	const PXBool success = OpenClipboard(pxWindowID); // Windows 2000, User32.dll, winuser.h

	PXActionOnErrorFetchAndReturn(!success);

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXClipBoardClose(PXClipBoard* const clipBoard)
{
#if OSUnix
    return PXActionNotImplemented;

#elif PXOSWindowsDestop
	return CloseClipboard(); // Windows 2000, User32.dll, winuser.h
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXClipBoardSet(PXClipBoard* const clipBoard, const PXClipBoardFormat format, const void* data)
{
#if OSUnix
	return PXActionNotImplemented;
#elif PXOSWindowsDestop
	// Alloc something with GlobalAlloc()
	// Maybe copy data?

	const HGLOBAL memory = 0;
	const PXInt32U formatID = PXClipBoardFormatToID(format);
	const HANDLE handle = SetClipboardData(formatID, memory); // Windows 2000, User32.dll, winuser.h
	const PXBool successful = handle != 0;

	PXActionOnErrorFetchAndReturn(!successful);

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXClipBoardClear(PXClipBoard* const clipBoard)
{
#if OSUnix
    return PXActionNotImplemented;
#elif PXOSWindowsDestop
	return EmptyClipboard(); // Windows 2000, User32.dll, winuser.h
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}