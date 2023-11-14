#include "PXConsole.h"

#include <Media/PXText.h>
#include <stdarg.h>
//#include <stdio.h>
//#include <stdarg.h>


void PXAPI PXConsoleClear()
{
	printf("\033[H\033[J");
}

void PXAPI PXConsoleGoToXY(const PXInt32U x, const PXInt32U y)
{
	printf("\033[%d;%dH", y, x);
}

void PXAPI PXConsoleTranlateColors(PXText* const bufferInput, PXText* const bufferOuput)
{
	PXTextClear(bufferOuput);

	for (PXSize i = 0; i < bufferInput->SizeUsed; i++)
	{
		PXByte isColorKey = bufferInput->TextA[i] == '§';

		if (!isColorKey)
		{
			PXSize offset = PXTextFindFirstCharacterA(&bufferInput->TextA[i], bufferInput->SizeUsed -i, '§');

			if (offset == (PXSize)-1)
			{
				PXTextAppendA
				(
					bufferOuput,
					&bufferInput->TextA[i],
					bufferInput->SizeUsed - i
				);

				return; // DONE
			}

			PXTextAppendA
			(
				bufferOuput,
				&bufferInput->TextA[i],
				offset
			);

			i += offset - 1;

			continue;
		}

		++i;

		const PXInt8U colorID = bufferInput->TextA[i] - '0';

		char buffer[128];
		PXSize bufferSize = sprintf_s(buffer, 128, "\x1b[38;5;%im", colorID);

		PXTextAppendA
		(
			bufferOuput,
			buffer,
			bufferSize
		);	
	}
}

#define PXConsoleColorRed 1
#define PXConsoleColorGreen 10
#define PXConsoleColorYellow 11
#define PXConsoleColorCyan 12
#define PXConsoleColorGray 7


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


	PXText formattedText;
	PXTextConstructNamedBufferA(&formattedText, formattedTextBuffer, 512);

	PXText bufferColor;
	PXTextConstructNamedBufferA(&bufferColor, bufferColorBuffer, 1024);

		// sprintf_s
	formattedText.SizeUsed = PXTextPrintA
	(
		formattedText.TextA,
		formattedText.SizeAllocated, 
		"§7[§1%c§7][§3%s§7] §6%s\n", 
		loggingTypeSymbol, 
		source, 
		format
	);

	PXConsoleTranlateColors(&formattedText, &bufferColor);


	{
		va_list args;
		va_start(args, format);

		vfprintf(stdout, bufferColor.TextA, args);

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
