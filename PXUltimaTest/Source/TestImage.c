#include "TestImage.h"

#include <stdio.h>

#include <Format/Image.h>
#include <Format/Font.h>
#include <File/File.h>

void TestSaveImageSmal()
{
    Image image;

    DirectoryCreateA("_TEST_DATA_OUTPUT_");

    ImageConstruct(&image);

    // Write image data
    {
        const PXSize width = 2u * 1u;
        const PXSize height = 2u * 1u;

        ImageResize(&image, ImageDataFormatRGB8, width, height);

        int dimension = 3;

        ImagePixelSetRGB8(&image, 0, 0, 0xFF, 0x00, 0x00);
        ImagePixelSetRGB8(&image, 1, 0, 0x00, 0xFF, 0x00);
        ImagePixelSetRGB8(&image, 0, 1, 0x00, 0x00, 0xFF);
        ImagePixelSetRGB8(&image, 1, 1, 0xFF, 0xFF, 0xFF);
    }

    const char* pathBMP = "_TEST_DATA_OUTPUT_/Smal_ImageBMP.bmp";
    const PXActionResult saveBMP = ImageSaveTest(&image, pathBMP, FileFormatBitMap, ImageDataFormatRGB8);

    if(saveBMP != PXActionSuccessful)
    {
        return saveBMP;
    }
    

    const char* pathPNG = "_TEST_DATA_OUTPUT_/Smal_ImagePNG.png";
    const PXActionResult savePNG = ImageSaveTest(&image, pathPNG, FileFormatPNG, ImageDataFormatRGB8);

    if(savePNG != PXActionSuccessful)
    {
        return savePNG;
    }

    const char* pathJPG = "_TEST_DATA_OUTPUT_/Smal_ImageJPG.jpg";
    const PXActionResult saveJPG = ImageSaveTest(&image, pathJPG, FileFormatJPEG, ImageDataFormatRGB8);

    if(saveJPG != PXActionSuccessful)
    {
        return saveJPG;
    }

    ImageDestruct(&image);
}

void TestSaveImage()
{
    Image image;

    DirectoryCreateA("_TEST_DATA_OUTPUT_");


    ImageConstruct(&image);

    // Write image data
    {
        const PXSize width = 512u * 1;
        const PXSize height = 512u * 1;

        ImageResize(&image, ImageDataFormatRGB8, width, height);

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

    const PXActionResult saveBMP = ImageSaveTest(&image, "_TEST_DATA_OUTPUT_/Complex_ImageBMP.bmp", FileFormatBitMap, ImageDataFormatRGB8);

    if(saveBMP != PXActionSuccessful)
    {
        return saveBMP;
    }

    const PXActionResult savePNG = ImageSaveTest(&image, "_TEST_DATA_OUTPUT_/Complex_ImagePNG.png", FileFormatPNG, ImageDataFormatRGB8);

    if(savePNG != PXActionSuccessful)
    {
        return savePNG;
    }

    const PXActionResult saveJPG = ImageSaveTest(&image, "_TEST_DATA_OUTPUT_/Complex_ImageJPG.jpg", FileFormatJPEG, ImageDataFormatRGB8);

    if(saveJPG != PXActionSuccessful)
    {
        return saveJPG;
    }

    ImageDestruct(&image);
}

void ImageWriteText()
{
    Image image;
    PXFont font;

    {
        const PXActionResult actionResult = FontLoadA(&font, "_TEST_DATA_INPUT_/A.fnt");
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

        ImageResize(&image, ImageDataFormatRGB8, width, height);

        //ImageDrawRectangle(&image, 0,0, width, height, 0xFF, 0xFF, 0xFF, 0xFF);
        ImageDrawRectangle(&image, 10, 10, 10, 10, 0xFF, 0x00, 0x00, 0x00);
        ImageDrawRectangle(&image, 10, 30, 10, 10, 0x00, 0xFF, 0x00, 0x00);
        ImageDrawRectangle(&image, 10, 50, 10, 10, 0x00, 0x00, 0xFF, 0x00);

        ImageDrawTextA(&image, 10, 70, 100, 30, &font, "Hello World");
    }

    {
        const char* path = "_TEST_DATA_OUTPUT_/TextWrite_TEST_BMP.bmp";
        const PXActionResult saveBMP = ImageSaveTest(&image, path, FileFormatBitMap, ImageDataFormatRGB8);

        if(saveBMP != PXActionSuccessful)
        {
            return saveBMP;
        }
    }


    {
        const char* path = "_TEST_DATA_OUTPUT_/TextWrite_TEST_TextWrite_JPG.jpeg";
        const PXActionResult saveJPEG = ImageSaveTest(&image, path, FileFormatJPEG, ImageDataFormatRGB8);

        if(saveJPEG != PXActionSuccessful)
        {
            return saveJPEG;
        }
    }

    {
        const char* path = "_TEST_DATA_OUTPUT_/TextWrite_TEST_TextWrite_PNG.png";
        const PXActionResult savePNG = ImageSaveTest(&image, path, FileFormatPNG, ImageDataFormatRGBA8);

        if(savePNG != PXActionSuccessful)
        {
            return savePNG;
        }
    }


    ImageDestruct(&image);
}

void TestImageAll()
{
    printf("[#] Test for images...\n");

    //TestSaveImageSmal();
    TestSaveImage();
    //ImageWriteText();

    printf("[i] Test image finished\n");
}

PXActionResult ImageLoadTest(Image* const image, const char* const filePath)
{
    const PXActionResult savePNG = ImageLoadA(image, filePath);

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

PXActionResult ImageSaveTest(Image* const image, const char* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat)
{
    const PXActionResult savePNG = ImageSaveA(image, filePath, fileFormat, dataFormat);

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


