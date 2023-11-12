#include "PXLog.h"

#include <Media/PXText.h>
#include <stdarg.h>
//#include <stdio.h>
//#include <stdarg.h>


void PXAPI PXLogPrint(const PXLoggingType loggingType, const char* const source, const char* const format, ...)
{
	char loggingTypeSymbol;

	switch (loggingType)
	{
		case PXLoggingInfo:
			loggingTypeSymbol = 'i';
			break;

		case PXLoggingWarning:
			loggingTypeSymbol = '!';
			break;

		case PXLoggingQuestion:
			loggingTypeSymbol = '?';
			break;

		case PXLoggingError:
			loggingTypeSymbol = 'E';
			break;

		case PXLoggingFailure:
			loggingTypeSymbol = 'x';
			break;

		case PXLoggingAllocation:
			loggingTypeSymbol = 'A';
			break;

		case PXLoggingReallocation:
			loggingTypeSymbol = 'R';
			break;

		case PXLoggingDeallocation:
			loggingTypeSymbol = 'D';
			break;

		case PXLoggingTypeInvalid:
		default:
			loggingTypeSymbol = '°';
			break;
	}


	char buffer[256];

		// sprintf_s
	PXTextPrintA(buffer, 256, "[%c][%s] %s\n", loggingTypeSymbol, source, format);

	{
		va_list args;
		va_start(args, format);

		vfprintf(stdout, buffer, args);

		va_end(args);
	}
}

void PXAPI PXLogPrintString(const char* const source, PXSize length)
{
	for (PXSize i = 0; i < length; ++i)
	{
		const char character = MakePrintable(source[i]);

		//printf("%c", character);
	}
}

void PXAPI PXLogPrintStringLine(const char* const source, PXSize length)
{
	PXLogPrintString(source, length);

	//printf("\n");
}
