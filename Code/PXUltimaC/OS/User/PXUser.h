#ifndef PXUserINCLUDE
#define PXUserINCLUDE

#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>

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

#endif
