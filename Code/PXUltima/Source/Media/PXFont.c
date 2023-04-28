#include "PXFont.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <OS/File/PXFile.h>
#include <Media/SpriteFont/PXSpriteFont.h>
#include <Media/TTF/TTF.h>
#include <Media/PXImage.h>

void PXFontConstruct(PXFont* const font)
{
    MemoryClear(font, sizeof(PXFont));
}

void PXFontDestruct(PXFont* const font)
{
    MemoryRelease(font->FontElement, font->FontElementSize);
}

PXActionResult PXFontLoadA(PXFont* const font, const PXTextASCII filePath)
{
    PXByte filePathU[PathMaxSize];

    PXTextCopyAU(filePath, PathMaxSize, filePathU, PathMaxSize);

    PXActionResult actionResult = PXFontLoadU(font, filePathU);

    return actionResult;
}

PXActionResult PXFontLoadW(PXFont* const font, const PXTextUNICODE filePath)
{
    PXByte filePathU[PathMaxSize];

    PXTextCopyWU(filePath, PathMaxSize, filePathU, PathMaxSize);

    PXActionResult actionResult = PXFontLoadU(font, filePathU);

    return actionResult;
}

PXActionResult PXFontLoadU(PXFont* const font, const PXTextUTF8 filePath)
{
    PXFile dataStream;

    PXFontConstruct(font);

    {
        const PXActionResult fileLoadingResult = PXFileMapToMemoryU(&dataStream, filePath, 0, PXMemoryAccessModeReadOnly);

        PXActionExitOnError(fileLoadingResult);
    }

    //dataStream.FilePath = filePath;

    {
        const FileFormatExtension hint = PXFilePathExtensionDetectTryA(filePath, PathMaxSize);
        const PXActionResult fileParsingResult = PXFontLoadD(font, &dataStream, hint);

        PXActionExitOnSuccess(fileParsingResult);

        PXActionResult fileGuessResult = PXActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = PXFontLoadD(font, &imageFileFormat, hint);

            fileFormatID++;
        } 
        while (fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXFileDestruct(&dataStream);

        return fileGuessResult;
    }
}

PXActionResult PXFontLoadD(PXFont* const font, PXFile* const PXFile, const FileFormatExtension guessedFormat)
{
    PXFontConstruct(font);

    switch(guessedFormat)
    {
        case FileFormatSpriteFont:
        {
            font->FontElementSize = 1u;
            font->FontElement = MemoryAllocateClear(sizeof(PXSpriteFont) * 1u);

            {

                PXSize readBytes = 0;
                const PXActionResult filePXActionResult = PXSpriteFontParse(font->FontElement, PXFile);
                const PXBool sucessful = PXActionSuccessful == filePXActionResult;

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