#include "PXConsole.h"

#include <stdarg.h>
#include <Media/PXText.h>
#include <OS/Async/PXThread.h>
#include <OS/Time/PXTime.h>
#include <OS/File/PXFile.h>
#include <OS/Debug/PXDebug.h>

#if OSUnix || OSForcePOSIXForWindows
#include <stdio.h>
#endif

#define PXConsoleColorEnable 1


PXActionResult PXAPI PXConsoleTextColorSetFromID(const PXInt16U coliorID)
{
	PXConsoleTextColor pxConsoleTextColor = 0;

	if('0' <= coliorID && coliorID <= '9') // Number
	{
		pxConsoleTextColor = coliorID + 1 - '0';
	}
	else if('a' <= coliorID && coliorID <= 'z')
	{
		pxConsoleTextColor = (coliorID - 'a') + 11;
	}
	else if('A' <= coliorID && coliorID <= 'Z')
	{
		pxConsoleTextColor = (coliorID - 'A') + +11;
	}

	return PXConsoleTextColorSet(pxConsoleTextColor);
}

PXActionResult PXAPI PXConsoleTextColorSet(const PXConsoleTextColor pxConsoleTextColor)
{
	const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorID = 0;

	switch(pxConsoleTextColor)
	{

		case PXConsoleTextColorBLACK: 
		{ colorID = 0;  break; }
		case PXConsoleTextColorDARKBLUE:
		{ colorID = FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorDARKGREEN:
		{ colorID = FOREGROUND_GREEN;  break; }
		case PXConsoleTextColorDARKCYAN:
		{ colorID = FOREGROUND_GREEN | FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorDARKRED: 
		{ colorID = FOREGROUND_RED;  break; }
		case PXConsoleTextColorDARKMAGENTA: 
		{ colorID = FOREGROUND_RED | FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorDARKYELLOW:
		{ colorID = FOREGROUND_RED | FOREGROUND_GREEN;  break; }
		case PXConsoleTextColorDARKGRAY: 
		{ colorID = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorGRAY:
		{ colorID = FOREGROUND_INTENSITY;  break; }
		case PXConsoleTextColorBLUE:
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorGREEN:
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_GREEN;  break; }
		case PXConsoleTextColorCYAN: 
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorRED: 
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_RED;  break; }
		case PXConsoleTextColorMAGENTA: 
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;  break; }
		case PXConsoleTextColorYELLOW:
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;  break; }

		default:
		case PXConsoleTextColorWHITE: 
		{ colorID = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;  break; }



			//return PXActionRefusedArgumentInvalid;
	}

	SetConsoleTextAttribute(consoleHandle, colorID);
}

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
	const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const COORD position = {x, y};

	SetConsoleCursorPosition(consoleHandle, position);
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
	const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	const BOOL success = WriteConsoleA(consoleHandle, source, length, 0, 0);


	PXText pxText;
	PXTextConstructFromAdressA(&pxText, source, length, length);

	PXDebugLogMessage(&pxText);
}

void PXAPI PXConsoleWriteFV(const PXSize length, const char* const source, va_list va_list)
{
#if 1
	PXSize xx = vprintf(source, va_list);
#endif
}

void PXAPI PXConsoleWriteWithColorCodes(PXText* const bufferInput)
{
	PXConsoleTextColorSet(PXConsoleTextColorWHITE);

	for (PXSize i = 0; i < bufferInput->SizeUsed; i++)
	{
		PXByte isColorKey = bufferInput->TextA[i] == '§';

		if (!isColorKey)
		{
			PXSize offset = PXTextFindFirstCharacterA(&bufferInput->TextA[i], bufferInput->SizeUsed -i, '§');

			if (offset == (PXSize)-1)
			{
				PXConsoleWrite(bufferInput->SizeUsed - i, &bufferInput->TextA[i]);				

				return; // DONE
			}

			PXConsoleWrite(offset, &bufferInput->TextA[i]);

			i += offset - 1;

			continue;
		}

		++i;

#if PXConsoleColorEnable
		PXConsoleTextColorSetFromID(bufferInput->TextA[i]);

		//PXConsoleTextColorSet(color);
#else
		// Nothing
#endif
	
	}
}

void PXAPI PXConsoleWriteTableFloat(const float* const data, const PXSize amount, const PXSize width)
{	
	const PXSize rows = amount / 3;

	for(PXSize y = 0; y < rows; ++y)
	{
		for(PXSize x = 0; x < width; ++x)
		{
			float number = data[x + y * width];

			PXConsoleWriteF(260, "%5.2f ", number);
		}

		PXConsoleWriteF(260, "\n");
	}
}

void PXAPI PXConsoleWriteTableInt(const PXInt8U* const data, const PXSize amount, const PXSize width)
{
	const PXSize rows = amount / 3;

	for(PXSize y = 0; y < rows; ++y)
	{
		for(PXSize x = 0; x < width; ++x)
		{
			PXInt8U number = data[x + y * width];

			PXConsoleWriteF(260, "%3i ", number);
		}

		PXConsoleWriteF(260, "\n");
	}
}


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





	PXConsoleWriteWithColorCodes(&textPreFormatted, &textColored);



	va_list args;
	va_start(args, pxLoggingEventData);

	PXConsoleWriteFV(0, textColored.TextA, args);

	// vfprintf(stdout, textColored.TextA, args);

	va_end(args);
}

void PXAPI PXLogPrint(const PXLoggingType loggingType, const char* const source, const char* const action, const char* const format, ...)
{
	char loggingTypeSymbol;

	char symbolColor = '7';
	char nameColor = '6';

	switch (loggingType)
	{
		case PXLoggingInfo:
			loggingTypeSymbol = 'i';
			symbolColor = 'b';
			nameColor = '7';
			break;

		case PXLoggingWarning:
			loggingTypeSymbol = '!';
			symbolColor = 'c';
			nameColor = 'c';
			break;

		case PXLoggingQuestion:
			loggingTypeSymbol = '?';
			symbolColor = '9';
			break;

		case PXLoggingError:
			loggingTypeSymbol = 'E';
			symbolColor = '4';
			nameColor = 'c';
			break;

		case PXLoggingFailure:
			loggingTypeSymbol = 'x';
			symbolColor = '4';
			nameColor = 'c';
			break;

		case PXLoggingAllocation:
			loggingTypeSymbol = '+';
			symbolColor = 'c';
			nameColor = 'e';
			break;

		case PXLoggingReallocation:
			loggingTypeSymbol = '*';
			symbolColor = 'c';
			nameColor = 'd';
			break;

		case PXLoggingDeallocation:
			loggingTypeSymbol = '-';
			symbolColor = 'a';
			nameColor = 'e';
			break;

		case PXLoggingTypeInvalid:
		default:
			loggingTypeSymbol = '°';
			break;
	}


	PXText formattedText;
	PXTextConstructNamedBufferA(&formattedText, formattedTextBuffer, 512);

	PXText exportText;
	PXTextConstructNamedBufferA(&exportText, bufferColorBuffer, 4000);

	PXTime pxTime;
	PXTimeNow(&pxTime);

	// Pre formatting
	formattedText.SizeUsed = PXTextPrintA
	(
		formattedText.TextA,
		formattedText.SizeAllocated,
		"§8[§9%2.2i§8:§9%2.2i§8:§9%2.2i§8] §6%11s §%c%c %-14s §%c%s§f\n", //  
		//"§0# §1# §2# §3# §4# §5# §6# §7# §8# §9# §a# §b# §c# §d# §e# §f#\n",
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

	// Add content
	{
		va_list args;
		va_start(args, format);

		PXTextPrintV(&exportText, formattedText.TextA, args);

		// vfprintf(stdout, exportText.TextA, args);

		va_end(args);

		//OutputDebugStringA();
	}




	if (PXLoggingError == loggingType)
	{
		for (PXSize i = 1; i < 80; ++i)
		{
			PXConsoleWrite(1, "-");
		}

		PXConsoleWrite(1, "\n");
	}

	PXConsoleWriteWithColorCodes(&exportText);


	if (PXLoggingError == loggingType)
	{
		for (PXSize i = 1; i < 80; ++i)
		{
			PXConsoleWrite(1, "-");
		}

		PXConsoleWrite(1, "\n");
	}
}