#ifndef ImageTestDEFINE
#define ImageTestDEFINE

#include <stddef.h>

#include <Error/ActionResult.h>
#include <File/Image.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern ActionResult ImageLoadTest(Image* const image, const char* const filePath, const ImageFileFormat fileFormat, const ImageDataFormat dataFormat);
    extern void TestSaveImageSmal();
    extern void TestSaveImage();
    extern void ImageWriteText();

#ifdef __cplusplus
}
#endif

#endif