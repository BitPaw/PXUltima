#pragma once

#include <OS/Console/PXConsole.h>

namespace PX
{
	enum class LoggingType
	{
		Invalid,
		Info,
		Warning,
		Question,
		Error,
		Failure,
		Allocation,
		Reallocation,
		Deallocation
	};

	class Console
	{
		public:
		static void PXAPI Clear();
		static void PXAPI GoToXY(const PXInt32U x, const PXInt32U y);

		static void PXAPI Print(const PXLoggingType loggingType, const char* const source, const char* const format, ...);

		static void PXAPI PrintString(const char* const source, PXSize length);
		static void PXAPI PrintStringLine(const char* const source, PXSize length);
	};
}