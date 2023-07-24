#include "PXSound.h"

#include <OS/Memory/PXMemory.h>

void PXSoundConstruct(PXSound* const pxSound)
{
    PXObjectClear(PXSound, pxSound);
}

void PXSoundDestruct(PXSound* const pxSound)
{
    
}

PXActionResult PXSoundLoad(PXSound* const pxSound, const PXText* const filePath)
{
    return PXActionNotImplemented;
}

PXActionResult PXSoundLoadAs(PXSound* const pxSound, const PXFile* const pxFile, const FileFormatExtension guessedFormat)
{
    return PXActionNotImplemented;
}

PXActionResult PXSoundSave(PXSound* const pxSound, const PXText* const filePath, const FileFormatExtension fileFormat)
{
    return PXActionNotImplemented;
}

PXActionResult PXSoundSaveAs(PXSound* const pxSound, const PXFile* const pxFile, const FileFormatExtension fileFormat)
{
    return PXActionNotImplemented;
}