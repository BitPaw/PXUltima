#include "Font.h"

#include <Memory/PXMemory.h>
#include <Text/Text.h>
#include <File/DataStream.h>

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

ActionResult FontLoadA(PXFont* const font, const char* filePath)
{
    wchar_t filePathW[PathMaxSize];

    TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

    ActionResult actionResult = FontLoadW(font, filePathW);

    return actionResult;
}

ActionResult FontLoadW(PXFont* const font, const wchar_t* filePath)
{
    DataStream dataStream;

    DataStreamConstruct(&dataStream);
    PXFontConstruct(font);

    {
        const ActionResult fileLoadingResult = DataStreamMapToMemoryW(&dataStream, filePath, 0, MemoryReadOnly);
        const unsigned char sucessful = fileLoadingResult == ActionSuccessful;

        if(!sucessful)
        {
            return fileLoadingResult;
        }
    }

    {
        wchar_t filePathDirectory[PathMaxSize];

        size_t index = TextFindLastW(filePath, PathMaxSize, '/');

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
        const ActionResult fileParsingResult = FontLoadD(font, hint, dataStream.Data, dataStream.DataSize, filePathDirectory);
        const unsigned char success = fileParsingResult == ActionSuccessful;

        if(success)
        {
            return ActionSuccessful;
        }

        ActionResult fileGuessResult = ActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = FontLoadD(font, imageFileFormat, dataStream.Data, dataStream.DataSize, filePathDirectory);

            fileFormatID++;
        }
        while(fileGuessResult == ActionInvalidHeaderSignature);

        return fileGuessResult;
    }

    DataStreamDestruct(&dataStream);
}

ActionResult FontLoadD(PXFont* const font, const FileFormatExtension guessedFormat, const void* data, const size_t dataSize, const wchar_t* const sourcePath)
{
    PXFontConstruct(font);

    switch(guessedFormat)
    {
        case FileFormatSpriteFont:
        {
            font->FontElementSize = 1u;
            font->FontElement = MemoryAllocateClear(sizeof(FNT) * 1u);

            {

                size_t readBytes = 0;
                const ActionResult fileActionResult = FNTParse(font->FontElement, data, dataSize, &readBytes, sourcePath);
                const unsigned char sucessful = ActionSuccessful == fileActionResult;

                if(!sucessful)
                {
                    return fileActionResult;
                }
            }

            break;
        }
        case FileFormatTrueTypeFont:
        {
            TTF ttf;

            TTFConstruct(&ttf);

            {
                size_t readBytes = 0;
                const ActionResult fileActionResult = TTFParse(&ttf, data, dataSize, &readBytes);
                const unsigned char sucessful = fileActionResult == ActionSuccessful;

                if(sucessful)
                {
                   // ttf.ConvertTo(*this);

                    return ActionSuccessful;
                }
            }

            TTFDestruct(&ttf);

            break;
        }
        default:
        {
            return ResultFormatNotSupported;
        }
    }

    return ActionSuccessful;
}
