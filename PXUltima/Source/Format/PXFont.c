#include "PXFont.h"

#include <OS/Memory/PXMemory.h>
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

PXActionResult PXFontLoadA(PXFont* const font, const PXTextASCII filePath)
{
    PXByte filePathU[PathMaxSize];

    TextCopyAU(filePath, PathMaxSize, filePathU, PathMaxSize);

    PXActionResult actionResult = PXFontLoadU(font, filePathU);

    return actionResult;
}

PXActionResult PXFontLoadW(PXFont* const font, const PXTextUNICODE filePath)
{
    PXByte filePathU[PathMaxSize];

    TextCopyWU(filePath, PathMaxSize, filePathU, PathMaxSize);

    PXActionResult actionResult = PXFontLoadU(font, filePathU);

    return actionResult;
}

PXActionResult PXFontLoadU(PXFont* const font, const PXTextUTF8 filePath)
{
    PXDataStream dataStream;

    PXDataStreamConstruct(&dataStream);
    PXFontConstruct(font);

    {
        const PXActionResult fileLoadingResult = PXDataStreamMapToMemoryU(&dataStream, filePath, 0, MemoryReadOnly);

        PXActionExitOnError(fileLoadingResult);
    }

    dataStream.FilePath = filePath;

    {
        const FileFormatExtension hint = FilePathExtensionDetectTryA(filePath, PathMaxSize);
        const PXActionResult fileParsingResult = PXFontLoadD(font, &dataStream, hint);
        const unsigned char success = fileParsingResult == PXActionSuccessful;

        if (success)
        {
            return PXActionSuccessful;
        }

        PXActionResult fileGuessResult = PXActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = PXFontLoadD(font, &imageFileFormat, hint);

            fileFormatID++;
        } while (fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXDataStreamDestruct(&dataStream);

        return fileGuessResult;
    }
}

PXActionResult PXFontLoadD(PXFont* const font, PXDataStream* const pxDataStream, const FileFormatExtension guessedFormat)
{
    PXFontConstruct(font);

    switch(guessedFormat)
    {
        case FileFormatSpriteFont:
        {
            font->FontElementSize = 1u;
            font->FontElement = MemoryAllocateClear(sizeof(PXFNT) * 1u);

            {

                PXSize readBytes = 0;
                const PXActionResult filePXActionResult = FNTParse(font->FontElement, pxDataStream);
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