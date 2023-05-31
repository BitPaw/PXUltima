#ifndef PXCPPClipBoardINCLUDE
#define PXCPPClipBoardINCLUDE

#include <OS/ClipBoard/PXClipBoard.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	enum class ClipBoardFormat
	{
		ClipBoardFormatInvalid,
		ClipBoardFormatText,
		ClipBoardFormatBITMAP,
		ClipBoardFormatMETAFILEPICT,
		ClipBoardFormatSYLK,
		ClipBoardFormatDIF,
		ClipBoardFormatPXTIFF,
		ClipBoardFormatOEMTEXT,
		ClipBoardFormatDIB,
		ClipBoardFormatPALETTE,
		ClipBoardFormatPENDATA,
		ClipBoardFormatPXRIFF,
		ClipBoardFormatWAVE,
		ClipBoardFormatUNICODETEXT,
		ClipBoardFormatENHMETAFILE,
		ClipBoardForPXMathDROP,
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
	};

	class ClipBoard : public PXClipBoard
	{
		public:
		PX::ActionResult Open();
		PX::ActionResult Close();
		PX::ActionResult Set(const ClipBoardFormat format, const void* data);
		PX::ActionResult Clear();
	};
}

#endif