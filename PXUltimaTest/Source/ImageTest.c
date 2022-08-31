#include "ImageTest.h"

#include <stdio.h>

#include <File/Image.h>
#include <File/Font.h>
#include <File/File.h>

void TestSaveImageSmal()
{
    Image image;

    DirectoryCreateA("_TEST_DATA_");


    ImageConstruct(&image);

    // Write image data
    {
        const size_t width = 2u * 1u;
        const size_t height = 2u * 1u;

        ImageResize(&image, ImageDataFormatRGB8, width, height);

        int dimension = 3;

        ImagePixelSetRGB8(&image, 0, 0, 0xFF, 0x00, 0x00);
        ImagePixelSetRGB8(&image, 1, 0, 0x00, 0xFF, 0x00);
        ImagePixelSetRGB8(&image, 0, 1, 0x00, 0x00, 0xFF);
        ImagePixelSetRGB8(&image, 1, 1, 0xFF, 0xFF, 0xFF);
    }

    const ActionResult saveBMP = ImageSaveA(&image, "_TEST_DATA_/ImageBMP_Smal.bmp", ImageFileFormatBitMap, ImageDataFormatRGB8);

    if(saveBMP != ResultSuccessful)
    {
        return saveBMP;
    }

    const ActionResult savePNG = ImageSaveA(&image, "_TEST_DATA_/ImagePNG_Smal.png", ImageFileFormatPNG, ImageDataFormatRGB8);

    if(savePNG != ResultSuccessful)
    {
        return savePNG;
    }

    const ActionResult saveJPG = ImageSaveA(&image, "_TEST_DATA_/ImageJPG_Smal.jpg", ImageFileFormatJPEG, ImageDataFormatRGB8);

    if(saveJPG != ResultSuccessful)
    {
        return saveJPG;
    }

    ImageDestruct(&image);
}

void TestSaveImage()
{
    Image image;
    
    DirectoryCreateA("_TEST_DATA_");


    ImageConstruct(&image);

    // Write image data
    {
        const size_t width = 512u * 1;
        const size_t height = 512u * 1;

        ImageResize(&image, ImageDataFormatRGB8, width, height);

        int dimension = 3;

        for(size_t y = 0; y < height; y++)
        {
            for(size_t x = 0; x < width; x++)
            {
                const unsigned char red = x | y;
                const unsigned char green = x ^ y;
                const unsigned char blue = 0xFF * !(x & y);

                ((unsigned char*)image.PixelData)[dimension * width * y + dimension * x + 2] = blue;
                ((unsigned char*)image.PixelData)[dimension * width * y + dimension * x + 1] = green;
                ((unsigned char*)image.PixelData)[dimension * width * y + dimension * x + 0] = red;


                // image.PixelData[4 * width * y + 4 * x + 3] = 255;
            }
        }
    }

    const ActionResult saveBMP = ImageSaveA(&image, "_TEST_DATA_/ImageBMP.bmp", ImageFileFormatBitMap, ImageDataFormatRGB8);
    
    if(saveBMP != ResultSuccessful)
    {
        return saveBMP;
    }
    
    const ActionResult savePNG = ImageSaveA(&image, "_TEST_DATA_/ImagePNG.png", ImageFileFormatPNG, ImageDataFormatRGB8);
    
    if(savePNG != ResultSuccessful)
    {
        return savePNG;
    }
    
    const ActionResult saveJPG = ImageSaveA(&image, "_TEST_DATA_/ImageJPG.jpg", ImageFileFormatJPEG, ImageDataFormatRGB8);

    if(saveJPG != ResultSuccessful)
    {
        return saveJPG;
    }

    ImageDestruct(&image);
}

void ImageWriteText()
{
    Image image;
    CFont font;

    Image imageAA;
    //ActionResult dfsdsfdsf = ImageLoadA(&imageAA, "D:/_Data/A.png");

    ImageConstruct(&image);

    ActionResult actionResult = FontLoadA(&font, "_TEST_DATA_/A.fnt");

    if(actionResult != ResultSuccessful)
    {
        return actionResult;
    }

    ActionResult loadResult = ImageLoadA(&image, "_TEST_DATA_/ImageInput.bmp");

    if(loadResult != ResultSuccessful)
    {
        return loadResult;
    }

    // Write image data
    {
        const size_t width = 512u * 1;
        const size_t height = 512u * 1;

        ImageResize(&image, ImageDataFormatRGB8, width, height);

        //ImageDrawRectangle(&image, 0,0, width, height, 0xFF, 0xFF, 0xFF, 0xFF);
        ImageDrawRectangle(&image, 10, 10, 10, 10, 0xFF, 0x00, 0x00, 0x00);
        ImageDrawRectangle(&image, 10, 30, 10, 10, 0x00, 0xFF, 0x00, 0x00);
        ImageDrawRectangle(&image, 10, 50, 10, 10, 0x00, 0x00, 0xFF, 0x00);

        ImageDrawTextA(&image, 10, 70, 100, 30, &font, "Hello World");
    }

    {
        const char* path = "_TEST_DATA_/TEST_TextWrite.bmp";
        const ActionResult saveBMP = ImageLoadTest(&image, path, ImageFileFormatBitMap, ImageDataFormatRGB8);

        if(saveBMP != ResultSuccessful)
        {
            return saveBMP;
        }
    }

  
    {
        const char* path = "_TEST_DATA_/TEST_TextWrite.jpeg";
        const ActionResult saveJPEG = ImageLoadTest(&image, path, ImageFileFormatJPEG, ImageDataFormatRGB8);

        if(saveJPEG != ResultSuccessful)
        {
            return saveJPEG;
        }
    }

    {
        const char* path = "_TEST_DATA_/TEST_TextWrite.png";
        const ActionResult savePNG = ImageLoadTest(&image, path, ImageFileFormatPNG, ImageDataFormatRGBA8);

        if(savePNG != ResultSuccessful)
        {
            return savePNG;
        }
    }
  

    ImageDestruct(&image);  
}

ActionResult ImageLoadTest(Image* const image, const char* const filePath, const ImageFileFormat fileFormat, const ImageDataFormat dataFormat)
{
    const ActionResult savePNG = ImageLoadA(image, filePath, fileFormat, dataFormat);

    if(savePNG != ResultSuccessful)
    {
        printf("[x] Image saving FAILED! ErrorID:%i Path:%s\n", savePNG, filePath);
    }
    else
    {
        printf("[i] Image saving SUCCESS! Path:%s\n", filePath);
    }


    return savePNG;
}


