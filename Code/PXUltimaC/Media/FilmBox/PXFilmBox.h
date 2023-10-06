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

	PXPublic PXActionResult PXAPI PXFilmBoxLoadFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXFilmBoxSaveToFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
