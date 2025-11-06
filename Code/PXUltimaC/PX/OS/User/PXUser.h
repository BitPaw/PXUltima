#pragma once

#ifndef PXUserIncluded
#define PXUserIncluded

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

PXPublic PXResult PXAPI PXUserNameGetAA(PXText PXREF name);
PXPublic PXBool PXAPI PXUserEnviromentFolderGet(PXText PXREF name, const PXUserEnviromentFolderID pxUserEnviromentFolderID);

#endif
