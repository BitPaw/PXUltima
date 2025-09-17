#include "PXSoftwareRender.h"

#include <PX/Math/PXMath.h>

void PXAPI PXSoftwareRenderDrawRectangle(PXTexture* const image, const PXSize x, const PXSize y, const PXSize width, const PXSize height, const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha)
{
    //unsigned char* data = ImageDataPoint(image, x, y);

    const PXSize mimimumInBoundsX = PXMathMinimum(x + width, image->Width);
    const PXSize mimimumInBoundsY = PXMathMinimum(y + height, image->Height);
    const PXSize bytePerPixel = PXColorFormatBytePerPixel(image->Format);

    for (PXSize cy = y; cy < mimimumInBoundsY; ++cy)
    {
        for (PXSize cx = x; cx < mimimumInBoundsX; ++cx)
        {
            const PXSize index = cx * bytePerPixel + cy * image->Width * bytePerPixel;

            ((unsigned char*)image->PixelData)[index + 0] = red;
            ((unsigned char*)image->PixelData)[index + 1] = green;
            ((unsigned char*)image->PixelData)[index + 2] = blue;
        }
    }
}

void PXAPI PXTextureDrawA(PXTexture* const image, const PXSize x, const PXSize y, const PXSize width, const PXSize height, const PXFont* const font, const char* text)
{
    wchar_t W[1024];

    PXTextCopyAW(text, 1024, W, 1024);

    PXTextureDrawW(image, x, y, width, height, font, W);
}

void PXAPI PXTextureDrawW(PXTexture* const image, const PXSize x, const PXSize y, const PXSize width, const PXSize height, const PXFont* const font, const wchar_t* text)
{
#if 0
    PXF32 fontSize = 0.002;
    PXF32 lastPositionX = x;

    for (PXSize i = 0; (i < 1024u) && (text[i] != '\0'); ++i)
    {
        const wchar_t character = text[i];
        const PXSpriteFontCharacter* PXSpriteFontCharacter = PXSpriteFontGetCharacter(&font->FontElement[0], character);

        if (!PXSpriteFontCharacter)
        {
            // Add red box?

            continue;
        }

        PXF32 offsetX = PXSpriteFontCharacter->Offset[0];
        PXF32 offsetY = PXSpriteFontCharacter->Offset[1];
        PXF32 positionX = PXSpriteFontCharacter->Position[0];
        PXF32 positionY = PXSpriteFontCharacter->Position[1];
        PXF32 sizeX = PXSpriteFontCharacter->Size[0];
        PXF32 sizeY = PXSpriteFontCharacter->Size[1];

#if 0
        ImageDrawRectangle
        (
            image,
            lastPositionX + offsetX,
            y + offsetY,
            sizeX,
            sizeY,
            0xFF,
            0,
            0xFF,
            0
        );
#endif
        PXTextureMerge
        (
            image,
            lastPositionX + offsetX,
            y + offsetY,
            positionX,
            positionY,
            sizeX,
            sizeY,
            &font->FontElement[0].FontPageList[0].FontTextureMap
        );

        lastPositionX += PXSpriteFontCharacter->XAdvance;
        //lastPositionX += 15;
    }
#endif
}

void PXAPI PXTextureMerge(PXTexture* const image, const PXSize x, const PXSize y, const PXSize insertX, const PXSize insertY, const PXSize insertWidth, const PXSize insertHeight, const PXTexture* const imageInsert)
{
    //unsigned char* data = ImageDataPoint(image, x, y);

    const PXSize sourceMimimumInBoundsX = PXMathMinimum(x + insertWidth, image->Width);
    const PXSize sourceMimimumInBoundsY = PXMathMinimum(y + insertHeight, image->Height);
    const PXSize targetMimimumInBoundsX = PXMathMinimum(insertX + insertWidth, imageInsert->Width);
    const PXSize targetMimimumInBoundsY = PXMathMinimum(insertY + insertHeight, imageInsert->Height);

    const PXSize sourceBytesPerPixel = PXColorFormatBytePerPixel(image->Format);
    const PXSize targetBytesPerPixel = PXColorFormatBytePerPixel(imageInsert->Format);
    const PXSize bytesPerPixel = PXMathMinimum(sourceBytesPerPixel, targetBytesPerPixel);

    for (PXSize cy = 0u; cy < insertHeight; ++cy)
    {
        for (PXSize cx = 0u; cx < insertWidth; ++cx)
        {
            const PXSize indexSource = (cx + x) * sourceBytesPerPixel + (cy + y) * image->Width * sourceBytesPerPixel;
            const PXSize indexTarget = (cx + insertX) * targetBytesPerPixel + (cy + insertY) * imageInsert->Width * targetBytesPerPixel;

            unsigned char* source = ((unsigned char*)image->PixelData) + indexSource;
            unsigned char* target = ((unsigned char*)imageInsert->PixelData) + indexTarget;

            if (targetBytesPerPixel == 4)
            {
                if (target[3] == 0)
                {
                    continue;
                }
            }

            for (PXSize i = 0; i < bytesPerPixel; ++i)
            {
                source[i] = target[i];
            }
        }
    }
}
