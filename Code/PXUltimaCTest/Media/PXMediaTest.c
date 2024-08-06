#include "PXMediaTest.h"

#include <Media/TTF/PXTTF.h>

void PXAPI PXMediaTestAll()
{
    PXMediaTestTTF("C:\\Windows\\Fonts\\arial.ttf");
}

void PXAPI PXMediaTestTTF(char* filePath)
{
    /*
    PXTTF ttf;

    PXFile pxFile;

    PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
    PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, filePath);

    pxFileOpenFromPathInfo.FileSize = 0;
    pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
    pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenFromPathInfo.AllowMapping = PXTrue;
    pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
    pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

    PXFileOpenFromPath(&pxFile, &pxFileOpenFromPathInfo);

    PXActionResult xx = PXTTFParse(&ttf, &pxFile);

    printf("");
    */
}