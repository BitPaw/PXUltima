#ifndef PXUserINCLUDE
#define PXUserINCLUDE

#include <PX/Media/PXText.h>
#include <PX/OS/Error/PXActionResult.h>

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

PXPublic PXActionResult PXAPI PXUserNameGet(PXText* const name);
PXPublic PXBool PXAPI PXUserEnviromentFolderGet(PXText* const name, const PXUserEnviromentFolderID pxUserEnviromentFolderID);

#endif
