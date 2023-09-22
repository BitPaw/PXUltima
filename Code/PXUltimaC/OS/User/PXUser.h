#ifndef PXUserINCLUDE
#define PXUserINCLUDE

#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXUserNameGet(PXText* const name);

	typedef enum PXUserEnviromentFolderID_
	{
		PXUserEnviromentFolderIDInvalid,

		PXUserEnviromentFolderIDAppData,
		PXUserEnviromentFolderIDDownload,
		PXUserEnviromentFolderIDDocuments,
		PXUserEnviromentFolderIDPictures,
		PXUserEnviromentFolderIDScreenshots,
		PXUserEnviromentFolderIDVideo,
		PXUserEnviromentFolderIDDestop,
		PXUserEnviromentFolderIDFonts,
		PXUserEnviromentFolderIDMusic
	}
	PXUserEnviromentFolderID;

	PXPublic PXBool PXAPI PXUserEnviromentFolderGet(PXText* const name, const PXUserEnviromentFolderID pxUserEnviromentFolderID);

#ifdef __cplusplus
}
#endif

#endif