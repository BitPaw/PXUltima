#include "PXConsole.h"

#include <stdarg.h>
#include <Media/PXText.h>
#include <OS/Async/PXThread.h>
#include <OS/Time/PXTime.h>
#include <OS/File/PXFile.h>

#if OSUnix || OSForcePOSIXForWindows
#include <stdio.h>
#endif

#define PXConsoleColorEnable 0


void PXAPI PXConsoleClear()
{
#if OSUnix || OSForcePOSIXForWindows
	printf("\033[H\033[J");
#elif OSWindows
	//SetConsoleCursorPosition();
	// TODO
#endif	
}

void PXAPI PXConsoleGoToXY(const PXInt32U x, const PXInt32U y)
{
#if OSUnix || OSForcePOSIXForWindows
	printf("\033[%d;%dH", y, x);
#elif OSWindows
	HWND console = GetConsoleWindow();

	const COORD position = {x, y};

	SetConsoleCursorPosition(console, position);
#endif	
}

void PXAPI PXConsoleWriteF(const PXSize length, const char* const source, ...)
{
#if 1
	va_list args;
	va_start(args, source);

	PXSize xx = vprintf(source, args);

	va_end(args);
#endif
}

void PXAPI PXConsoleWrite(const PXSize length, const char* const source)
{
	HWND console = GetConsoleWindow();

	WriteConsoleA(console, source, length, 0, 0);
}

void PXAPI PXConsoleWriteFV(const PXSize length, const char* const source, va_list va_list)
{
#if 1
	PXSize xx = vprintf(source, va_list);
#endif
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

#if PXConsoleColorEnable && 1
		char buffer[128];
		PXSize bufferSize = sprintf_s(buffer, 128, "\x1b[38;5;%im", colorID);


		PXTextAppendA
		(
			bufferOuput,
			buffer,
			bufferSize
		);
#endif
	
	}
}

#define PXConsoleColorRed 1
#define PXConsoleColorGreen 10
#define PXConsoleColorYellow 11
#define PXConsoleColorCyan 12
#define PXConsoleColorGray 7

void PXAPI PXLogPrintInvoke(PXLoggingEventData* const pxLoggingEventData, ...)
{
	PXText textPreFormatted;
	PXTextConstructNamedBufferA(&textPreFormatted, formattedTextBuffer, 512);

	PXText textColored;
	PXTextConstructNamedBufferA(&textColored, bufferColorBuffer, 1024);

	PXText textExtra;
	PXTextConstructBufferA(&textExtra, 512);

	PXTextClear(&textExtra);

	char loggingTypeSymbol;

	int symbolColor = 7;
	int nameColor = 6;

	switch (pxLoggingEventData->Type)
	{
		case PXLoggingInfo:
			loggingTypeSymbol = 'i';
			symbolColor = 6;
			nameColor = 6;
			break;

		case PXLoggingWarning:
			loggingTypeSymbol = '!';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingQuestion:
			loggingTypeSymbol = '?';
			symbolColor = 6;
			break;

		case PXLoggingError:
			loggingTypeSymbol = 'E';
			symbolColor = 1;
			nameColor = 1;
			break;

		case PXLoggingFailure:
			loggingTypeSymbol = 'x';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingAllocation:
			loggingTypeSymbol = '+';
			symbolColor = 9;
			nameColor = 9;
			break;

		case PXLoggingReallocation:
			loggingTypeSymbol = '*';
			symbolColor = 9;
			nameColor = 9;
			break;

		case PXLoggingDeallocation:
			loggingTypeSymbol = '-';
			symbolColor = 2;
			nameColor = 2;
			break;

		case PXLoggingTypeInvalid:
		default:
			loggingTypeSymbol = '°';
			break;
	}

	switch (pxLoggingEventData->Target)
	{
		case PXLoggingTypeTargetFile:
		{
			PXFile* pxFile = pxLoggingEventData->FileReference;
			//PXFilePathGet();

			PXText pxTextFilePath;
			PXTextConstructNamedBufferA(&pxTextFilePath, pxTextFilePathBuffer, PXPathSizeMax);

			PXFilePathGet(pxFile, &pxTextFilePath);

			PXText pxText;
			PXTextConstructNamedBufferA(&pxText, pxTextBuffer, 32);

			PXTextFormatSize(&pxText, pxFile->DataSize);

			PXTextPrint
			(
				&textExtra,
				"%8s  ID:%i <%s>",
				pxText.TextA,
				(int)pxFile->ID,
				pxTextFilePath.TextA
			);

			break;
		}
		case PXLoggingTypeTargetMemory:
		{
			PXLoggingMemoryData* pxLoggingMemoryData = &pxLoggingEventData->MemoryData;

			PXText pxText;
			PXTextConstructBufferA(&pxText, 32);

			PXTextFormatSize(&pxText, pxLoggingMemoryData->Amount * pxLoggingMemoryData->TypeSize);

			if (pxLoggingMemoryData->NameFile)
			{
				PXTextPrint
				(
					&textExtra,
					"%8s  %s::%s::%i",
					pxText.TextA,
					pxLoggingMemoryData->NameFile,
					pxLoggingMemoryData->NameFunction,
					pxLoggingMemoryData->NumberLine
				);
			}
			else
			{
				PXTextPrint
				(
					&textExtra,
					"%8s",
					pxText.TextA
				);
			}		

			break;
		}

		default:
			break;
	}

	PXThread pxThread;
	PXThreadCurrentGet(&pxThread);

	PXTime pxTime;
	PXTimeNow(&pxTime);

	// sprintf_s
	textPreFormatted.SizeUsed = PXTextPrintA
	(
		textPreFormatted.TextA,
		textPreFormatted.SizeAllocated,
		"[%2.2i:%2.2i:%2.2i] §3%11s §%i%c %-14s §%i%s%s\n",
		pxTime.Hour,
		pxTime.Minute,
		pxTime.Second,
		//(int)pxThread.ThreadID,
		pxLoggingEventData->ModuleSource,
		symbolColor,
		loggingTypeSymbol,
		pxLoggingEventData->ModuleAction,
		nameColor,
		textExtra.TextA,
		pxLoggingEventData->PrintFormat
	);

	PXConsoleTranlateColors(&textPreFormatted, &textColored);



	va_list args;
	va_start(args, pxLoggingEventData);

	PXConsoleWriteFV(0, textColored.TextA, args);

	// vfprintf(stdout, textColored.TextA, args);

	va_end(args);
}

void PXAPI PXLogPrint(const PXLoggingType loggingType, const char* const source, const char* const action, const char* const format, ...)
{
	char loggingTypeSymbol;

	int symbolColor = 7;
	int nameColor = 6;

	switch (loggingType)
	{
		case PXLoggingInfo:
			loggingTypeSymbol = 'i';
			symbolColor = 6;
			break;

		case PXLoggingWarning:
			loggingTypeSymbol = '!';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingQuestion:
			loggingTypeSymbol = '?';
			symbolColor = 6;
			break;

		case PXLoggingError:
			loggingTypeSymbol = 'E';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingFailure:
			loggingTypeSymbol = 'x';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingAllocation:
			loggingTypeSymbol = '+';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingReallocation:
			loggingTypeSymbol = '*';
			symbolColor = 9;
			nameColor = 1;
			break;

		case PXLoggingDeallocation:
			loggingTypeSymbol = '-';
			symbolColor = 2;
			nameColor = 2;
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

	PXTime pxTime;
	PXTimeNow(&pxTime);

	formattedText.SizeUsed = PXTextPrintA
	(
		formattedText.TextA,
		formattedText.SizeAllocated,
		"[%2.2i:%2.2i:%2.2i] §3%11s §%i%c %-14s §%i%s\n",
		pxTime.Hour,
		pxTime.Minute,
		pxTime.Second,
		source,
		symbolColor,
		loggingTypeSymbol,
		action,
		nameColor,
		format
	);

	PXConsoleTranlateColors(&formattedText, &bufferColor);


	if (PXLoggingError == loggingType)
	{
		for (PXSize i = 1; i < 80; ++i)
		{
			PXConsoleWrite("%c", '-', 1);
		}

		PXConsoleWrite("\n", 1);
	}

	{
		va_list args;
		va_start(args, format);

		vfprintf(stdout, bufferColor.TextA, args);

		va_end(args);

		//OutputDebugStringA();
	}

	if (PXLoggingError == loggingType)
	{
		for (PXSize i = 1; i < 80; ++i)
		{
			PXConsoleWrite("%c", '-', 1);
		}

		PXConsoleWrite("\n", 1);
	}
}


/*
* 
* 	PXLogPrintString(source, length);

	//printf("\n");
* 

for (PXSize i = 0; i < length; ++i)
{
	const char character = MakePrintable(source[i]);

	//printf("%c", character);
}
*/