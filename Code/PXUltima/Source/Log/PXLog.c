#include "PXLog.h"

#include <Text/PXText.h>
#include <stdio.h>
#include <stdarg.h>

void PXLogPrintf(const LoggingType loggingType, const char* const source, const char* const input,  ...)
{
	char loggingTypeSymbol;

	switch (loggingType)
	{
		case LoggingInfo:
			loggingTypeSymbol = 'i';
			break;

		case LoggingWarning:
			loggingTypeSymbol = '!';
			break;

		case LoggingQuestion:
			loggingTypeSymbol = '?';
			break;

		case LoggingError:
			loggingTypeSymbol = 'E';
			break;

		case LoggingFailure:
			loggingTypeSymbol = 'x';
			break;

		case LoggingAllocation:
			loggingTypeSymbol = 'A';
			break;

		case LoggingReallocation:
			loggingTypeSymbol = 'R';
			break;

		case LoggingDeallocation:
			loggingTypeSymbol = 'D';
			break;

		case LoggingTypeInvalid:
		default:
			loggingTypeSymbol = '°';
			break;
	}


	//char buffer[256];

	//sprintf(buffer, "[%c][%s] %s", loggingTypeSymbol, source, input);

	{
		va_list args;
		va_start(args, input);

		vfprintf(stdout, source, args);

		va_end(args);
	}
}

void PXLogPrintString(const char* const source, PXSize length)
{
	for (size_t i = 0; i < length; ++i)
	{
		const char character = MakePrintable(source[i]);

		printf("%c", character);
	}
}

void PXLogPrintStringLine(const char* const source, PXSize length)
{
	PXLogPrintString(source, length);

	printf("\n");
}