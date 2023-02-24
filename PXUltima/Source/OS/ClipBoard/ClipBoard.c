#include "ClipBoard.h"

#include <OS/System/OSVersion.h>

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <WinUser.h>

#define ClipBoardFormatTEXTID CF_TEXT
#define ClipBoardFormatBITMAPID CF_BITMAP
#define ClipBoardFormatMETAFILEPICTID CF_METAFILEPICT
#define ClipBoardFormatSYLKID CF_SYLK
#define ClipBoardFormatDIFID CF_DIF
#define ClipBoardFormatTIFFID   CF_TIFF
#define ClipBoardFormatOEMTEXTID CF_OEMTEXT
#define ClipBoardFormatDIBID CF_DIB
#define ClipBoardFormatPALETTEID  CF_PALETTE
#define ClipBoardFormatPENDATAID CF_PENDATA
#define ClipBoardFormatRIFFID CF_RIFF
#define ClipBoardFormatWAVEID CF_WAVE
#define ClipBoardFormatUNICODETEXTID CF_UNICODETEXT
#define ClipBoardFormatENHMETAFILEID CF_ENHMETAFILE
#define ClipBoardFormatOWNERDISPLAYID CF_OWNERDISPLAY
#define ClipBoardFormatDSPTEXTID   CF_DSPTEXT
#define ClipBoardFormatDSPBITMAPID CF_DSPBITMAP
#define ClipBoardFormatDSPMETAFILEPICTID CF_DSPMETAFILEPICT
#define ClipBoardFormatDSPENHMETAFILEID  CF_DSPENHMETAFILE
#define ClipBoardFormatPRIVATEFIRSTID CF_PRIVATEFIRST
#define ClipBoardFormatPRIVATELASTID CF_PRIVATELAST
#define ClipBoardFormatGDIOBJFIRSTID CF_GDIOBJFIRST
#define ClipBoardFormatGDIOBJLASTID CF_GDIOBJLAST

#if defined(WindowsAtleastNT)
#define ClipBoardFormatHDROPID CF_HDROP
#define ClipBoardFormatLOCALEID CF_LOCALE
#else
#define ClipBoardFormatHDROP 15
#define ClipBoardFormatLOCALE 16
#endif

#if defined(WindowsAtleast2000)
#define ClipBoardFormatDIBV5ID CF_DIBV5
#else
#define ClipBoardFormatDIBV5 17
#endif

#if defined(WindowsAtleast2000)
#define ClipBoardFormatMAXID CF_MAX
#elif defined(WindowsAtleastNT)
#define ClipBoardFormatMAX 17
#else
#define ClipBoardFormatMAX 15
#endif

#endif

unsigned int ClipBoardFormatToID(const ClipBoardFormat clipBoardFormat)
{

#if OSUnix
	return -1;

#elif OSWindows
	switch (clipBoardFormat)
	{
	default:
	case ClipBoardFormatInvalid:
		return -1;

	case ClipBoardFormatText:
		return ClipBoardFormatTEXTID;

	case ClipBoardFormatBITMAP:
		return ClipBoardFormatBITMAPID;

	case ClipBoardFormatMETAFILEPICT:
		return ClipBoardFormatMETAFILEPICTID;

	case ClipBoardFormatSYLK:
		return ClipBoardFormatSYLKID;

	case ClipBoardFormatDIF:
		return ClipBoardFormatDIFID;

	case ClipBoardFormatTIFF:
		return ClipBoardFormatTIFFID;

	case ClipBoardFormatOEMTEXT:
		return ClipBoardFormatOEMTEXTID;

	case ClipBoardFormatDIB:
		return ClipBoardFormatDIBID;

	case ClipBoardFormatPALETTE:
		return ClipBoardFormatPALETTEID;

	case ClipBoardFormatPENDATA:
		return ClipBoardFormatPENDATAID;

	case ClipBoardFormatRIFF:
		return ClipBoardFormatRIFFID;

	case ClipBoardFormatWAVE:
		return ClipBoardFormatWAVEID;

	case ClipBoardFormatUNICODETEXT:
		return ClipBoardFormatUNICODETEXTID;

	case ClipBoardFormatENHMETAFILE:
		return ClipBoardFormatENHMETAFILEID;

//	case ClipBoardFormatHDROP:
//		return ClipBoardFormatHDROPID;

//	case ClipBoardFormatLOCALE:
//		return ClipBoardFormatLOCALEID;

//	case ClipBoardFormatDIBV5:
//		return ClipBoardFormatDIBV5ID;

//	case ClipBoardFormatMAX:
//		return ClipBoardFormatMAXID;

	case ClipBoardFormatOWNERDISPLAY:
		return ClipBoardFormatOWNERDISPLAYID;

	case ClipBoardFormatDSPTEXT:
		return ClipBoardFormatDSPTEXTID;

	case ClipBoardFormatDSPBITMAP:
		return ClipBoardFormatDSPBITMAPID;

	case ClipBoardFormatDSPMETAFILEPICT:
		return ClipBoardFormatDSPMETAFILEPICTID;

	case ClipBoardFormatDSPENHMETAFILE:
		return ClipBoardFormatDSPENHMETAFILEID;

	case ClipBoardFormatPRIVATEFIRST:
		return ClipBoardFormatPRIVATEFIRSTID;

	case ClipBoardFormatPRIVATELAST:
		return ClipBoardFormatPRIVATELASTID;

	case ClipBoardFormatGDIOBJFIRST:
		return ClipBoardFormatGDIOBJFIRSTID;

	case ClipBoardFormatGDIOBJLAST:
		return ClipBoardFormatGDIOBJLASTID;
	}
#endif
}

ClipBoardFormat ClipBoardFormatFromID(const unsigned int clipBoardFormat)
{
#if OSUnix
    return PXActionInvalid;

#elif OSWindows
	switch (clipBoardFormat)
	{
	case ClipBoardFormatTEXTID:
		return ClipBoardFormatText;

	case ClipBoardFormatBITMAPID:
		return ClipBoardFormatBITMAP;

	case ClipBoardFormatMETAFILEPICTID:
		return ClipBoardFormatMETAFILEPICT;

	case ClipBoardFormatSYLKID:
		return ClipBoardFormatSYLK;

	case ClipBoardFormatDIFID:
		return ClipBoardFormatDIF;

	case ClipBoardFormatTIFFID:
		return ClipBoardFormatTIFF;

	case ClipBoardFormatOEMTEXTID:
		return ClipBoardFormatOEMTEXT;

	case ClipBoardFormatDIBID:
		return ClipBoardFormatDIB;

	case ClipBoardFormatPALETTEID:
		return ClipBoardFormatPALETTE;

	case ClipBoardFormatPENDATAID:
		return ClipBoardFormatPENDATA;

	case ClipBoardFormatRIFFID:
		return ClipBoardFormatRIFF;

	case ClipBoardFormatWAVEID:
		return ClipBoardFormatWAVE;

	case ClipBoardFormatUNICODETEXTID:
		return ClipBoardFormatUNICODETEXT;

	case ClipBoardFormatENHMETAFILEID:
		return ClipBoardFormatENHMETAFILE;

//	case ClipBoardFormatHDROPID:
//		return ClipBoardFormatHDROP;

//	case ClipBoardFormatLOCALEID:
//		return ClipBoardFormatLOCALE;

//	case ClipBoardFormatDIBV5ID:
//		return ClipBoardFormatDIBV5;

//	case ClipBoardFormatMAXID:
//		return ClipBoardFormatMAX;

	case ClipBoardFormatOWNERDISPLAYID:
		return ClipBoardFormatOWNERDISPLAY;

	case ClipBoardFormatDSPTEXTID:
		return ClipBoardFormatDSPTEXT;

	case ClipBoardFormatDSPBITMAPID:
		return ClipBoardFormatDSPBITMAP;

	case ClipBoardFormatDSPMETAFILEPICTID:
		return ClipBoardFormatDSPMETAFILEPICT;

	case ClipBoardFormatDSPENHMETAFILEID:
		return ClipBoardFormatDSPENHMETAFILE;

	case ClipBoardFormatPRIVATEFIRSTID:
		return ClipBoardFormatPRIVATEFIRST;

	case ClipBoardFormatPRIVATELASTID:
		return ClipBoardFormatPRIVATELAST;

	case ClipBoardFormatGDIOBJFIRSTID:
		return ClipBoardFormatGDIOBJFIRST;

	case ClipBoardFormatGDIOBJLASTID:
		return ClipBoardFormatGDIOBJLAST;

	default:
		return ClipBoardFormatInvalid;
	}
#endif // defined
}

PXActionResult ClipBoardOpen(ClipBoard* const clipBoard)
{
#if OSUnix
    return PXActionInvalid;

#elif OSWindows
	HWND PXWindowID = NULL;
	unsigned char success = OpenClipboard(PXWindowID);

	if (!success)
	{
		return GetCurrentError();
	}

	return PXActionSuccessful;
#endif
}

PXActionResult ClipBoardClose(ClipBoard* const clipBoard)
{
#if OSUnix
    return PXActionInvalid;

#elif OSWindows
	return CloseClipboard();
#endif
}

PXActionResult ClipBoardSet(ClipBoard* const clipBoard, const ClipBoardFormat format, const void* data)
{

#if OSUnix
	return PXActionInvalid;

#elif OSWindows
	// Alloc something with GlobalAlloc()
	// Maybe copy data?

	const HGLOBAL memory = 0;
	const unsigned int formatID = ClipBoardFormatToID(format);
	const HANDLE handle = SetClipboardData(formatID, memory);
	const unsigned char successful = handle != 0;

	if (!successful)
	{
		return GetCurrentError();
	}

	return PXActionSuccessful;
#endif
}

PXActionResult ClipBoardClear(ClipBoard* const clipBoard)
{
#if OSUnix
    return PXActionInvalid;

#elif OSWindows
	return EmptyClipboard();
#endif
}
