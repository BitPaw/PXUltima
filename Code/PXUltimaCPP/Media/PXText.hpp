#ifndef PXCPPTextINLCUDE
#define PXCPPTextINLCUDE

#include <Media/PXText.h>

namespace PX
{
	enum class TextFormat
	{
		FormatInvalid,
		FormatASCII,
		FormatUNICODE,
		FormatUTF8,
		FormatUTF16
	};

	class Text : public PXText
	{
		public:
		PXDLLExport Text();

		PXDLLExport Text(const char* adress, PXSize size);

		template<PXSize stringLength>
		PXDLLExport Text(const char(&stringAdress)[stringLength]);

		template<PXSize stringLength>
		PXDLLExport Text(char(&stringAdress)[stringLength]);
	};
}

#endif