#ifndef ImageTestDEFINE
#define ImageTestDEFINE

#include <stddef.h>

#include <Error/PXActionResult.h>
#include <Format/Image.h>
#include <File/File.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void TestImageAll();

    extern PXActionResult ImageLoadTest(Image* const image, const char* const filePath);
    extern void TestSaveImageSmal();
    extern void TestSaveImage();
    extern void ImageWriteText();

    PXActionResult ImageLoadTest(Image* const image, const char* const filePath);
    PXActionResult ImageSaveTest(Image* const image, const char* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat);

#ifdef __cplusplus
}
#endif

#endif
