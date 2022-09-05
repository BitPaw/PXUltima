#include "Font.h"

#include <Memory/Memory.h>
#include <Text/Text.h>
#include <File/File.h>

#include <Format/FNT/FNT.h>
#include <Format/TTF/TTF.h>
#include <Format/Image.h>

void FontConstruct(CFont* const font)
{
    MemorySet(font, sizeof(CFont), 0);
}

void FontDestruct(CFont* const font)
{
    MemoryRelease(font->FontElement, font->FontElementSize);
}

FontFileFormat FontGuessFormat(const wchar_t* filePath)
{
    wchar_t extension[ExtensionMaxSize];

    FilePathExtensionGetW(filePath, PathMaxSize, extension, ExtensionMaxSize);

    if(TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "FNT", 3u))  return FontFileFormatFNT;
    if(TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "OTF", 3u))  return FontFileFormatOFT;
    if(TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "TTF", 4u))  return FontFileFormatTTF;

    return FontFileFormatUnkown;
}

ActionResult FontLoadA(CFont* const font, const char* filePath)
{
    wchar_t filePathW[PathMaxSize];

    TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

    ActionResult actionResult = FontLoadW(font, filePathW);

    return actionResult;
}

ActionResult FontLoadW(CFont* const font, const wchar_t* filePath)
{
    File file;

    FileConstruct(&file);
    FontConstruct(font);

    {
        const ActionResult fileLoadingResult = FileMapToVirtualMemoryW(&file, filePath, 0, MemoryReadOnly);
        const unsigned char sucessful = fileLoadingResult == ActionSuccessful;

        if(!sucessful)
        {
            return fileLoadingResult;
        }
    }

    {
        wchar_t filePathDirectory[256];

        size_t index = TextFindLastW(filePath, 256, '/');

        if (index == -1)
        {
            index = 0;
        }
        else
        {
            index += 1;
        }

        TextCopyW(filePath, index, filePathDirectory, 256);


        const FontFileFormat hint = FontGuessFormat(filePath);
        const ActionResult fileParsingResult = FontLoadD(font, hint, file.Data, file.DataSize, filePathDirectory);
        const unsigned char success = fileParsingResult == ActionSuccessful;

        if(success)
        {
            return ActionSuccessful;
        }

        ActionResult fileGuessResult = ActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const ImageFileFormat imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = FontLoadD(font, imageFileFormat, file.Data, file.DataSize, filePathDirectory);

            fileFormatID++;
        }
        while(fileGuessResult == ActionInvalidHeaderSignature);

        return fileGuessResult;
    }

    FileDestruct(&file);
}

ActionResult FontLoadD(CFont* const font, const FontFileFormat guessedFormat, const void* data, const size_t dataSize, const wchar_t* const sourcePath)
{
    FontConstruct(font);

    switch(guessedFormat)
    {
        case FontFileFormatFNT:
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
        case FontFileFormatTTF:
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
        case FontFileFormatUnkown:
        default:
        {
            return ResultFormatNotSupported;
        }
    }

    return ActionSuccessful;
}
