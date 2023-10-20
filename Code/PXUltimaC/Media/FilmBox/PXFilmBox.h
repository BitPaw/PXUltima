#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXFilmBox_
	{
		unsigned int __dummy__;
	}
	PXFilmBox;

	PXPublic PXActionResult PXAPI PXFilmBoxLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXFilmBoxSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
