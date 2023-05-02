#include "TestImage.h"

#include <stdio.h>

#include <Media/PXImage.h>
#include <Media/PXFont.h>
#include <OS/File/PXFile.h>
#include <Graphic/SoftwareRender/PXSoftwareRender.h>

void TestSaveImageSmal()
{
    PXImage image;
    PXText pxText;

    PXTextMakeFixedA(&pxText, "_TEST_DATA_OUTPUT_");

    PXDirectoryCreate(&pxText);

    PXImageConstruct(&image);

    // Write image data
    {
        const PXSize width = 2u * 1u;
        const PXSize height = 2u * 1u;

        PXImageResize(&image, PXColorFormatRGBI8, width, height);

        int dimension = 3;

        PXImagePixelSetRGB8(&image, 0, 0, 0xFF, 0x00, 0x00);
        PXImagePixelSetRGB8(&image, 1, 0, 0x00, 0xFF, 0x00);
        PXImagePixelSetRGB8(&image, 0, 1, 0x00, 0x00, 0xFF);
        PXImagePixelSetRGB8(&image, 1, 1, 0xFF, 0xFF, 0xFF);
    }

    const char* pathBMP = "_TEST_DATA_OUTPUT_/Smal_ImageBMP.bmp";
    const PXActionResult saveBMP = ImageSaveTest(&image, pathBMP, FileFormatBitMap, PXColorFormatRGBI8);

    if(saveBMP != PXActionSuccessful)
    {
        return saveBMP;
    }
    

    const char* pathPNG = "_TEST_DATA_OUTPUT_/Smal_ImagePNG.png";
    const PXActionResult savePNG = ImageSaveTest(&image, pathPNG, FileFormatPNG, PXColorFormatRGBI8);

    if(savePNG != PXActionSuccessful)
    {
        return savePNG;
    }

    const char* pathJPG = "_TEST_DATA_OUTPUT_/Smal_ImageJPG.jpg";
    const PXActionResult saveJPG = ImageSaveTest(&image, pathJPG, FileFormatJPEG, PXColorFormatRGBI8);

    if(saveJPG != PXActionSuccessful)
    {
        return saveJPG;
    }

    PXImageDestruct(&image);
}

void TestSaveImage()
{
    PXImage image;
    PXText pxText;

    PXTextMakeFixedA(&pxText, "_TEST_DATA_OUTPUT_");

    PXDirectoryCreate(&pxText);


    PXImageConstruct(&image);

    // Write image data
    {
        const PXSize width = 512u * 1;
        const PXSize height = 512u * 1;

        PXImageResize(&image, PXColorFormatRGBI8, width, height);

        int dimension = 3;

        for(PXSize y = 0; y < height; y++)
        {
            for(PXSize x = 0; x < width; x++)
            {
                const unsigned char a = 0xFF * !(x & y); // Triangle part
                const unsigned char b = x | y; // pri
                const unsigned char c = x ^ y; // Sec

                unsigned char* data = (unsigned char*)image.PixelData + dimension * width * y + dimension * x;

                *(data + 0) = a;
                *(data + 1) = c;
                *(data + 2) = b;
            }
        }
    }

    const PXActionResult saveBMP = ImageSaveTest(&image, "_TEST_DATA_OUTPUT_/Complex_ImageBMP.bmp", FileFormatBitMap, PXColorFormatRGBI8);

    if(saveBMP != PXActionSuccessful)
    {
        return saveBMP;
    }

    const PXActionResult savePNG = ImageSaveTest(&image, "_TEST_DATA_OUTPUT_/Complex_ImagePNG.png", FileFormatPNG, PXColorFormatRGBI8);

    if(savePNG != PXActionSuccessful)
    {
        return savePNG;
    }

    const PXActionResult saveJPG = ImageSaveTest(&image, "_TEST_DATA_OUTPUT_/Complex_ImageJPG.jpg", FileFormatJPEG, PXColorFormatRGBI8);

    if(saveJPG != PXActionSuccessful)
    {
        return saveJPG;
    }

    PXImageDestruct(&image);
}

void ImageWriteText()
{
    PXImage image;
    PXFont font;

    {
        PXText pxText;

        PXTextMakeFixedA(&pxText, "_TEST_DATA_INPUT_/A.fnt");

        const PXActionResult actionResult = PXFontLoad(&font, &pxText);
        const PXBool successful = PXActionSuccessful == actionResult;

        if (!successful)
        {
            return actionResult;
        }
    }

    {
        const PXActionResult loadResult = ImageLoadTest(&image, "_TEST_DATA_INPUT_/ImageInput.bmp");
        const PXBool successful = PXActionSuccessful == loadResult;

        if (!successful)
        {
            return loadResult;
        }
    }

    // Write image data
    {
        const PXSize width = 512u * 1;
        const PXSize height = 512u * 1;

        PXImageResize(&image, PXColorFormatRGBI8, width, height);

        //PXSoftwareRenderDrawRectangle(&image, 0,0, width, height, 0xFF, 0xFF, 0xFF, 0xFF);
        PXSoftwareRenderDrawRectangle(&image, 10, 10, 10, 10, 0xFF, 0x00, 0x00, 0x00);
        PXSoftwareRenderDrawRectangle(&image, 10, 30, 10, 10, 0x00, 0xFF, 0x00, 0x00);
        PXSoftwareRenderDrawRectangle(&image, 10, 50, 10, 10, 0x00, 0x00, 0xFF, 0x00);

        PXImageDrawTextA(&image, 10, 70, 100, 30, &font, "Hello World");
    }

    {
        const char* path = "_TEST_DATA_OUTPUT_/TextWrite_TEST_BMP.bmp";
        const PXActionResult saveBMP = ImageSaveTest(&image, path, FileFormatBitMap, PXColorFormatRGBI8);

        if(saveBMP != PXActionSuccessful)
        {
            return saveBMP;
        }
    }


    {
        const char* path = "_TEST_DATA_OUTPUT_/TextWrite_TEST_TextWrite_JPG.jpeg";
        const PXActionResult saveJPEG = ImageSaveTest(&image, path, FileFormatJPEG, PXColorFormatRGBI8);

        if(saveJPEG != PXActionSuccessful)
        {
            return saveJPEG;
        }
    }

    {
        const char* path = "_TEST_DATA_OUTPUT_/TextWrite_TEST_TextWrite_PNG.png";
        const PXActionResult savePNG = ImageSaveTest(&image, path, FileFormatPNG, PXColorFormatRGBI8);

        if(savePNG != PXActionSuccessful)
        {
            return savePNG;
        }
    }

    PXImageDestruct(&image);
}

void TestImageAll()
{
    printf("[#] Test for images...\n");

    //TestSaveImageSmal();
    TestSaveImage();
    //ImageWriteText();

    printf("[i] Test image finished\n");
}

PXActionResult ImageLoadTest(PXImage* const image, const char* const filePath)
{
    PXText pxText;
    PXTextMakeExternA(&pxText, filePath);

    const PXActionResult savePNG = PXImageLoad(image, &pxText);

    if(savePNG != PXActionSuccessful)
    {
        printf("[x][Error] Image loading FAILED! ErrorID:%i Path:\"%s\"\n", savePNG, filePath);
    }
    else
    {
        printf("[i][OK] Image loading SUCCESS! Path:\"%s\"\n", filePath);
    }


    return savePNG;
}

PXActionResult ImageSaveTest(PXImage* const image, const char* const filePath, const FileFormatExtension fileFormat, const PXColorFormat dataFormat)
{
    PXText pxText;

    PXTextMakeFixedA(&pxText, "_TEST_DATA_INPUT_/A.fnt");

    const PXActionResult savePNG = PXImageSave(image, &pxText, fileFormat, dataFormat);

    if (savePNG != PXActionSuccessful)
    {
        printf("[x][Error] Image saving FAILED! ErrorID:%i Path:\"%s\"\n", savePNG, filePath);
    }
    else
    {
        printf("[i][OK] Image saving SUCCESS! Path:\"%s\"\n", filePath);
    }


    return savePNG;
}


