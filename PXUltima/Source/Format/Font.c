#include "Font.h"

#include <Memory/PXMemory.h>
#include <Text/Text.h>
#include <File/PXDataStream.h>

#include <Format/FNT/FNT.h>
#include <Format/TTF/TTF.h>
#include <Format/Image.h>

void PXFontConstruct(PXFont* const font)
{
    MemoryClear(font, sizeof(PXFont));
}

void PXFontDestruct(PXFont* const font)
{
    MemoryRelease(font->FontElement, font->FontElementSize);
}

PXActionResult FontLoadA(PXFont* const font, const char* filePath)
{
    wchar_t filePathW[PathMaxSize];

    TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

    PXActionResult actionResult = FontLoadW(font, filePathW);

    return actionResult;
}

PXActionResult FontLoadW(PXFont* const font, const wchar_t* filePath)
{
    PXDataStream dataStream;

    PXDataStreamConstruct(&dataStream);
    PXFontConstruct(font);

    {
        const PXActionResult fileLoadingResult = PXDataStreamMapToMemoryW(&dataStream, filePath, 0, MemoryReadOnly);
        const PXBool sucessful = PXActionSuccessful == fileLoadingResult;

        if(!sucessful)
        {
            return fileLoadingResult;
        }
    }

    {
        wchar_t filePathDirectory[PathMaxSize];

        PXSize index = TextFindLastW(filePath, PathMaxSize, '/');

        if (index == -1)
        {
            index = 0;
        }
        else
        {
            index += 1;
        }

        TextCopyW(filePath, index, filePathDirectory, 256);


        const FileFormatExtension hint = FilePathExtensionDetectTryW(filePath, PathMaxSize);
        const PXActionResult fileParsingResult = FontLoadD(font, hint, dataStream.Data, dataStream.DataSize, filePathDirectory);
        const unsigned char success = fileParsingResult == PXActionSuccessful;

        if(success)
        {
            return PXActionSuccessful;
        }

        PXActionResult fileGuessResult = PXActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = FontLoadD(font, imageFileFormat, dataStream.Data, dataStream.DataSize, filePathDirectory);

            fileFormatID++;
        }
        while(fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXDataStreamDestruct(&dataStream);

        return fileGuessResult;
    }
}

PXActionResult FontLoadD(PXFont* const font, const FileFormatExtension guessedFormat, const void* data, const PXSize dataSize, const wchar_t* const sourcePath)
{
    PXFontConstruct(font);

    switch(guessedFormat)
    {
        case FileFormatSpriteFont:
        {
            font->FontElementSize = 1u;
            font->FontElement = MemoryAllocateClear(sizeof(FNT) * 1u);

            {

                PXSize readBytes = 0;
                const PXActionResult filePXActionResult = FNTParse(font->FontElement, data, dataSize, &readBytes, sourcePath);
                const unsigned char sucessful = PXActionSuccessful == filePXActionResult;

                if(!sucessful)
                {
                    return filePXActionResult;
                }
            }

            break;
        }
        case FileFormatTrueTypeFont:
        {
            TTF ttf;

            TTFConstruct(&ttf);

            {
               // PXSize readBytes = 0;
                //const PXActionResult filePXActionResult = TTFParse(&ttf, data, dataSize, &readBytes);

                //PXActionExitOnError(filePXActionResult);
            }

            TTFDestruct(&ttf);

            break;
        }
        default:
        {
            return PXActionRefusedFormatNotSupported;
        }
    }

    return PXActionSuccessful;
}
