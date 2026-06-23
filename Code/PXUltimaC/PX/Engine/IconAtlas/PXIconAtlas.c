#include "PXIconAtlas.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

const char PXIconAtlasText[] = "IconAtlas";

PXResult PXAPI PXIconAtlasCreate(PXIconAtlas PXREF pxIconAtlas, PXIconAtlasCreateInfo PXREF pxIconAtlasCreateInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIconAtlasText,
        "Create",
        "ID:%i <%s> - CallSize:%i",
        pxIconAtlas->Info.ID,
        pxIconAtlasCreateInfo->Info.FilePath.A,
        pxIconAtlasCreateInfo->CellSize
    );
#endif


    // Load image
    {
        PXECSCreateInfo pxResourceCreateInfoSub;

        PXClear(PXECSCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.ObjectReference = (PXECSInfo**)&pxIconAtlas->IconTexture2D;
        pxResourceCreateInfoSub.ObjectAmount = 1;
        pxResourceCreateInfoSub.FilePath = pxIconAtlasCreateInfo->Info.FilePath;
        pxResourceCreateInfoSub.Type = PXResourceTypeTexture2D;

        ////PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

    pxIconAtlasCreateInfo->CellAmountX = pxIconAtlas->IconTexture2D->Width / pxIconAtlasCreateInfo->CellSize;
    pxIconAtlasCreateInfo->CellAmountY = pxIconAtlas->IconTexture2D->Height / pxIconAtlasCreateInfo->CellSize;
    pxIconAtlasCreateInfo->CellAmountTotal = pxIconAtlasCreateInfo->CellAmountX * pxIconAtlasCreateInfo->CellAmountY;


    // Preallocate icons
    pxIconAtlas->IconListAmount = pxIconAtlasCreateInfo->CellAmountTotal;
    pxIconAtlas->IconList = PXMemoryHeapCallocT(PXIcon, pxIconAtlasCreateInfo->CellAmountTotal);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIconAtlasText,
        "IconAtlas",
        "PXID:%i, CellMap:<%i/%i> from Texture:<%i/%i>. Total:%i icons",
        pxIconAtlas->Info.ID,
        pxIconAtlasCreateInfo->CellAmountX,
        pxIconAtlasCreateInfo->CellAmountY,
        pxIconAtlas->IconTexture2D->Width,
        pxIconAtlas->IconTexture2D->Height,
        pxIconAtlasCreateInfo->CellAmountTotal
    );
#endif

    if(!pxIconAtlas->IconList)
    {
        return PXResultInvalid;
    }

    // Register icons
    for(PXSize y = 0; y < pxIconAtlasCreateInfo->CellAmountY; ++y)
    {
        const PXSize pixelPositionY = y * pxIconAtlasCreateInfo->CellSize;

        for(PXSize x = 0; x < pxIconAtlasCreateInfo->CellAmountX; ++x)
        {
            const PXSize pixelPositionX = x * pxIconAtlasCreateInfo->CellSize;

            PXIcon PXREF pxIcon = &pxIconAtlas->IconList[x + y * pxIconAtlasCreateInfo->CellAmountX];

            PXIconCreateInfo pxIconCreateInfo;
            PXClear(PXIconCreateInfo, &pxIconCreateInfo);
            pxIconCreateInfo.Info.ObjectReference = (PXECSInfo**)&pxIcon;
            pxIconCreateInfo.Info.ObjectAmount = 1;
            pxIconCreateInfo.Info.Type = PXResourceTypeIcon;
            pxIconCreateInfo.IconImage = pxIconAtlas->IconTexture2D;
            pxIconCreateInfo.OffsetX = pixelPositionX;
            pxIconCreateInfo.OffsetY = pixelPositionY;
            pxIconCreateInfo.Width = pxIconAtlasCreateInfo->CellSize;
            pxIconCreateInfo.Height = pxIconAtlasCreateInfo->CellSize;
            pxIconCreateInfo.RowSize = pxIconAtlasCreateInfo->CellSize * 4;
            pxIconCreateInfo.BitPerPixel = 8 * 4;

            PXIconCreate(&pxIcon, &pxIconCreateInfo);
        }
    }

    return PXResultOK;
}
