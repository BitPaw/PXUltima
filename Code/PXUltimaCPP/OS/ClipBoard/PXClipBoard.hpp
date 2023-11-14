#pragma once

#include <OS/ClipBoard/PXClipBoard.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	enum class ClipBoardFormat
	{
		Invalid,
		Text,
		BITMAP,
		METAFILEPICT,
		SYLK,
		DIF,
		PXTIFF,
		OEMTEXT,
		DIB,
		PALETTE,
		PENDATA,
		PXRIFF,
		WAVE,
		UNICODETEXT,
		ENHMETAFILE,
		ClipBoardForPXMathDROP,
		LOCALE,
		DIBV5,
		MAX,
		OWNERDISPLAY,
		DSPTEXT,
		DSPBITMAP,
		DSPMETAFILEPICT,
		DSPENHMETAFILE,
		PRIVATEFIRST,
		PRIVATELAST,
		GDIOBJFIRST,
		GDIOBJLAST,
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