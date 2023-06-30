#include "PXFont.h"

#include <OS/Memory/PXMemory.h>

#include <OS/File/PXFile.h>
#include <Media/SpriteFont/PXSpriteFont.h>
#include <Media/TTF/PXTTF.h>
#include <Media/PXImage.h>

void PXFontConstruct(PXFont* const font)
{
    PXMemoryClear(font, sizeof(PXFont));
}

void PXFontDestruct(PXFont* const font)
{
    PXMemoryRelease(font->FontElement, font->FontElementSize);
}

PXActionResult PXFontLoad(PXFont* const font, const PXText* const filePath)
{
    PXFile dataStream;

    PXFontConstruct(font);

    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.Text = *filePath;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        const PXActionResult fileLoadingResult = PXFileOpenFromPath(&dataStream, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    //dataStream.FilePath = filePath;

    {
        const FileFormatExtension hint = PXFilePathExtensionDetectTry(filePath);
        const PXActionResult fileParsingResult = PXFontLoadD(font, &dataStream, hint);

        PXActionReturnOnSuccess(fileParsingResult);

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
            font->FontElement = PXMemoryAllocateTypeCleared(PXSpriteFont, 1u);

            {

                PXSize readBytes = 0;
                const PXActionResult filePXActionResult = PXSpriteFontParse(font->FontElement, PXFile);
                const PXBool sucessful = PXActionSuccessful == filePXActionResult;

                if(!sucessful)
                {
                    return filePXActionResult;
                }

                PXSpriteFontPrtinf(font->FontElement);
            }

            break;
        }
        case FileFormatTrueTypeFont:
        {
            PXTTF ttf;

            PXTTFConstruct(&ttf);

            {
               // PXSize readBytes = 0;
                //const PXActionResult filePXActionResult = PXTTFParse(&ttf, data, dataSize, &readBytes);

                //PXActionExitOnError(filePXActionResult);
            }

            PXTTFDestruct(&ttf);

            break;
        }
        default:
        {
            return PXActionRefusedFormatNotSupported;
        }
    }

    return PXActionSuccessful;
}