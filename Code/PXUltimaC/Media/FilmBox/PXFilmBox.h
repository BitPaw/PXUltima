#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include <Media/PXModel.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXFilmBox_
	{
		unsigned int __dummy__;
	}
	PXFilmBox;

	PXPublic PXActionResult PXFilmBoxLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	PXPublic PXActionResult PXFilmBoxSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif