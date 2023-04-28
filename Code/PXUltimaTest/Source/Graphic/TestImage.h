#ifndef ImageTestDEFINE
#define ImageTestDEFINE

#include <stddef.h>

#include <Media/PXImage.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void TestImageAll();

    extern void TestSaveImageSmal();
    extern void TestSaveImage();
    extern void ImageWriteText();

    PXActionResult ImageLoadTest(PXImage* const image, const char* const filePath);
    PXActionResult ImageSaveTest(PXImage* const image, const char* const filePath, const FileFormatExtension fileFormat, const PXColorFormat dataFormat);

#ifdef __cplusplus
}
#endif

#endif
