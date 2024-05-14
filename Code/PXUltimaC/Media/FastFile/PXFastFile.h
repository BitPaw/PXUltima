#ifndef PXFastFileINCLUDE
#define PXFastFileINCLUDE

#include "../PXResource.h"

typedef struct PXFastFile_
{
	PXInt32U Version;
}
PXFastFile;

PXPublic PXActionResult PXAPI PXFastFileLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXFastFileSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif