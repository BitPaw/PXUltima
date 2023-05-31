#ifndef PXUserINCLUDE
#define PXUserINCLUDE

#include <Media/PXText.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool PXUserNameGet(PXText* const name);

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

	PXPublic PXBool PXUserEnviromentFolderGet(PXText* const name, const PXUserEnviromentFolderID pxUserEnviromentFolderID);

#ifdef __cplusplus
}
#endif

#endif