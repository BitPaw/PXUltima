#include "PXConsole.h"

#include <stdarg.h>
#include <stdio.h>

#include <PX/Media/PXText.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Time/PXTime.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/OS/PXOS.h>

PXLock* _GLOBALCosolePrintLock = 0;
PXI32U* _GLOBALSourceThreadID;

#define PXConsoleColorEnable 1

PXResult PXAPI PXConsoleTextColorSetFromID(const PXI16U coliorID)
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

PXResult PXAPI PXConsoleTextColorSet(const PXConsoleTextColor pxConsoleTextColor)
{
#if OSUnix

#define PXUnixConsoleColorreset 0
#define PXUnixConsoleColorbright 1
#define PXUnixConsoleColordim 2
#define PXUnixConsoleColorunderscore 4
#define PXUnixConsoleColorblink 5
#define PXUnixConsoleColorreverse 7
#define PXUnixConsoleColorhidden 8

#define PXUnixConsoleColorBlack     8
#define PXUnixConsoleColorred       9
#define PXUnixConsoleColorgreen     10
#define PXUnixConsoleColoryellow    11
#define PXUnixConsoleColorblue      12
#define PXUnixConsoleColormagenta   13
#define PXUnixConsoleColorcyan      14
#define PXUnixConsoleColorwhite     15

#define PXUnixConsoleColorBGblack 40
#define PXUnixConsoleColorBGred 41
#define PXUnixConsoleColorBGgreen 42
#define PXUnixConsoleColorBGyellow 43
#define PXUnixConsoleColorBGblue 44
#define PXUnixConsoleColorBGmagenta 45
#define PXUnixConsoleColorBGcyan 46
#define PXUnixConsoleColorBGwhite 47


    int colorID = 0;
    int brightness = 0;

    switch(pxConsoleTextColor)
    {
    case PXConsoleTextColorBLACK:
    {
        colorID = PXUnixConsoleColorBlack;
        //brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKBLUE:
    {
        colorID = PXUnixConsoleColorblue;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKGREEN:
    {
        colorID = PXUnixConsoleColorgreen;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKCYAN:
    {
        colorID = PXUnixConsoleColorcyan;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKRED:
    {
        colorID = PXUnixConsoleColorred;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKMAGENTA:
    {
        colorID = PXUnixConsoleColormagenta;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKYELLOW:
    {
        colorID = PXUnixConsoleColoryellow;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorDARKGRAY:
    {
        colorID = PXUnixConsoleColorBGwhite;
        brightness = PXUnixConsoleColordim;
        break;
    }
    case PXConsoleTextColorGRAY:
    {
        colorID = PXUnixConsoleColorBGwhite;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    case PXConsoleTextColorBLUE:
    {
        colorID = PXUnixConsoleColorblue;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    case PXConsoleTextColorGREEN:
    {
        colorID = PXUnixConsoleColorgreen;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    case PXConsoleTextColorCYAN:
    {
        colorID = PXUnixConsoleColorcyan;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    case PXConsoleTextColorRED:
    {
        colorID = PXUnixConsoleColorred;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    case PXConsoleTextColorMAGENTA:
    {
        colorID = PXUnixConsoleColormagenta;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    case PXConsoleTextColorYELLOW:
    {
        colorID = PXUnixConsoleColoryellow;
        brightness = PXUnixConsoleColorbright;
        break;
    }

    default:
    case PXConsoleTextColorWHITE:
    {
        colorID = PXUnixConsoleColorwhite;
        brightness = PXUnixConsoleColorbright;
        break;
    }
    }

    // \x1b[0m

    // Write color as a excape sequence

    // 48 for background

    //printf("\x1b[%im", colorID);

    // Works?
   // printf("\x1b[38;5;%im", colorID);

    printf("\033[38;5;%dm", colorID, colorID);

    // %3d\033[m,   this is the reset code?

    //printf("\x1b[38;5;%im\x1b[%im", brightness, colorID);




#elif OSWindows
    const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD colorID = 0;

    switch(pxConsoleTextColor)
    {

    case PXConsoleTextColorBLACK:
    {
        colorID = 0;
        break;
    }
    case PXConsoleTextColorDARKBLUE:
    {
        colorID = FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorDARKGREEN:
    {
        colorID = FOREGROUND_GREEN;
        break;
    }
    case PXConsoleTextColorDARKCYAN:
    {
        colorID = FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorDARKRED:
    {
        colorID = FOREGROUND_RED;
        break;
    }
    case PXConsoleTextColorDARKMAGENTA:
    {
        colorID = FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorDARKYELLOW:
    {
        colorID = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    }
    case PXConsoleTextColorDARKGRAY:
    {
        colorID = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorGRAY:
    {
        colorID = FOREGROUND_INTENSITY;
        break;
    }
    case PXConsoleTextColorBLUE:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorGREEN:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        break;
    }
    case PXConsoleTextColorCYAN:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorRED:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_RED;
        break;
    }
    case PXConsoleTextColorMAGENTA:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    }
    case PXConsoleTextColorYELLOW:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    }

    default:
    case PXConsoleTextColorWHITE:
    {
        colorID = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }



        //return PXResultRefusedParameterInvalid;
    }
 
    const BOOL result = SetConsoleTextAttribute(consoleHandle, colorID);
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
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

void PXAPI PXConsoleGoToXY(const PXI32U x, const PXI32U y)
{
#if OSUnix || OSForcePOSIXForWindows
    printf("\033[%d;%dH", y, x);
#elif OSWindows
    const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    const COORD position = {x, y};

    SetConsoleCursorPosition(consoleHandle, position);
#endif
}

void PXAPI PXConsoleWriteF(const PXSize length, const char PXREF source, ...)
{
#if PXDefaultLibraryEnable
    va_list args;
    va_start(args, source);

    PXSize xx = vprintf(source, args); // stdio.h

    va_end(args);
#endif
}

void PXAPI PXConsoleWrite(const PXSize length, const char PXREF source)
{
#if OSUnix

    int writtenBytes = 0;

    for(size_t i = 0; i < length; i++)
    {
        char text[4] = { source[i],0,0,0};

        writtenBytes += printf(text);
    }

#elif OSWindows
    const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const BOOL success = WriteConsoleA(consoleHandle, source, length, 0, 0);

    PXText pxText;
    PXTextFromAdressA(&pxText, source, length, length);

    PXDebugLogMessage(&pxText);
#endif
}

void PXAPI PXConsoleWriteFV(const PXSize length, const char PXREF source, va_list va_list)
{
#if PXDefaultLibraryEnable
    const PXSize xx = vprintf(source, va_list);
#endif
}

void PXAPI PXConsoleWriteWithColorCodes(PXText PXREF bufferInput)
{
    PXLockEngage(_GLOBALCosolePrintLock, PXTrue);

    PXConsoleTextColorSet(PXConsoleTextColorWHITE);

    for (PXSize i = 0; i < bufferInput->SizeUsed; i++)
    {
        PXByte isColorKey = bufferInput->A[i] == '§';

        if (!isColorKey)
        {
            PXSize offset = PXTextFindFirstCharacterA(&bufferInput->A[i], bufferInput->SizeUsed -i, '§');

            if (offset == (PXSize)-1)
            {
                PXConsoleWrite(bufferInput->SizeUsed - i, &bufferInput->A[i]);

                break; // DONE
            }

            PXConsoleWrite(offset, &bufferInput->A[i]);

            i += offset - 1;

            continue;
        }

        ++i;

#if PXConsoleColorEnable
        PXConsoleTextColorSetFromID(bufferInput->A[i]);

        //PXConsoleTextColorSet(color);
#else
        // Nothing
#endif

    }

    PXLockRelease(_GLOBALCosolePrintLock);
}

void PXAPI PXConsoleWriteTablePXF32(const PXF32 PXREF data, const PXSize amount, const PXSize width)
{
    const PXSize rows = amount / width;

    for(PXSize y = 0; y < rows; ++y)
    {
        for(PXSize x = 0; x < width; ++x)
        {
            PXF32 number = data[x + y * width];

            PXConsoleWriteF(260, "%5.2f ", number);
        }

        PXConsoleWriteF(260, "\n");
    }
}

void PXAPI PXConsoleWriteTableInt(const PXI8U PXREF data, const PXSize amount, const PXSize width)
{
    const PXSize rows = amount / width;

    for(PXSize y = 0; y < rows; ++y)
    {
        for(PXSize x = 0; x < width; ++x)
        {
            PXI8U number = data[x + y * width];

            PXConsoleWriteF(260, "%3i ", number);
        }

        PXConsoleWriteF(260, "\n");
    }
}


void PXAPI PXLogPrintInvoke(PXLoggingEventData PXREF pxLoggingEventData, ...)
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
#if 0
        PXFile* pxFile = pxLoggingEventData->FileReference;
        //PXFilePath();

        PXText pxTextFilePath;
        PXTextConstructNamedBufferA(&pxTextFilePath, pxTextFilePathBuffer, PXPathSizeMax);

        PXFilePath(pxFile, &pxTextFilePath);

        PXText pxText;
        PXTextConstructNamedBufferA(&pxText, pxTextBuffer, 32);

        PXTextFormatSize(&pxText, pxFile->Buffer.DataSize);

        PXTextPrint
        (
            &textExtra,
            "%8s  ID:%i <%s>",
            pxText.A,
            (int)pxFile->FileID,
            pxTextFilePath.A
        );
#endif

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
                pxText.A,
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
                pxText.A
            );
        }

        break;
    }

    default:
        break;
    }

    //PXThread pxThread;
   // PXThreadCurrent(&pxThread);

    PXTime pxTime;
    PXTimeNow(&pxTime);

    textPreFormatted.SizeUsed = PXTextPrintA
    (
        textPreFormatted.A,
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
        textExtra.A,
        pxLoggingEventData->PrintFormat
    );

    PXConsoleWriteWithColorCodes(&textPreFormatted);


    va_list args;
    va_start(args, pxLoggingEventData);

    PXConsoleWriteFV(0, textColored.A, args);

    // vfprintf(stdout, textColored.A, args);

    va_end(args);
}

void PXAPI PXLogEnableASYNC()
{
    PXLockCreateInfo pxLockCreateInfo;
    PXClear(PXLockCreateInfo, &pxLockCreateInfo);
    pxLockCreateInfo.Type = PXLockTypeGlobal;

    PXLockCreate(_GLOBALCosolePrintLock, &pxLockCreateInfo);

    _GLOBALSourceThreadID = PXThreadCurrentID();
}


char _formattedText[1024];
char _exportTextBuffer[1024];

void PXAPI PXLogPrint(const PXLoggingType loggingType, const char PXREF source, const char PXREF action, const char PXREF format, ...)
{
    char loggingTypeSymbol;

    char symbolColor = '7';
    char nameColor = '6';

    switch(loggingType)
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

        case PXLoggingEvent:
            loggingTypeSymbol = '#';
            symbolColor = '8';
            nameColor = '8';
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
            loggingTypeSymbol = '>';
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
    PXText exportText;

    PXTextFromAdressA(&formattedText, _formattedText, 0, sizeof(_formattedText));
    PXTextFromAdressA(&exportText, _exportTextBuffer, 0, sizeof(_exportTextBuffer));

    PXTime pxTime;
    PXTimeNow(&pxTime);  

    const DWORD threadID = GetCurrentThreadId();
    char threadIDName[64];

    if(_GLOBALSourceThreadID == threadID)
    {
        PXTextPrintA(threadIDName, 64, "§6#Main");
    }
    else
    {
        PXTextPrintA(threadIDName, 64, "§9%5.5i", threadID);
    }

    // Pre formatting
    formattedText.SizeUsed = PXTextPrintA
    (
        formattedText.A,
        formattedText.SizeAllocated,
        "§8[%s§8]§8[§9%2.2i§8:§9%2.2i§8:§9%2.2i§8] §6%12s §%c%c %-14s §%c%s§f\n", //
        //"§0# §1# §2# §3# §4# §5# §6# §7# §8# §9# §a# §b# §c# §d# §e# §f#\n",
        threadIDName,
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

        PXTextPrintV(&exportText, formattedText.A, args);

        // vfprintf(stdout, exportText.A, args);

        va_end(args);

        //OutputDebugStringA();
    }



#if 0
    if (PXLoggingError == loggingType)
    {
        for (PXSize i = 1; i < 80; ++i)
        {
            PXConsoleWrite(1, "-");
        }

        PXConsoleWrite(1, "\n");
    }
#endif

    PXConsoleWriteWithColorCodes(&exportText);


#if 0
    if (PXLoggingError == loggingType)
    {
        for (PXSize i = 1; i < 80; ++i)
        {
            PXConsoleWrite(1, "-");
        }

        PXConsoleWrite(1, "\n");
    }
#endif
}
