#include "PXD8D.h"

#include <PX/OS/PXOS.h>
#include <PX/Media/ZLIB/PXZLIB.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/D8X/PXD8W.h>
#include <PX/Media/D8X/PXD8G.h>
#include <PX/Engine/ECS/Entity/Model/PXModelPool.h>

const char PXD8DText[] = "D8D";

PXResult PXAPI PXD8DatLoadFromFile(PXScene PXREF pxScene, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    // PXPNG* png = PXNull;
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXD8XDat dummy;
    PXClear(PXD8XDat, &dummy);

    PXD8XDat* pxD8XDat = &dummy;



    // a .dat can contain a .d8w. Those need a .d8t. Lets seek if we got one
    PXFile* texturePool = 0;
    PXFile* mesh = 0;

    PXFileCreateInfo pxFileOpenInfo;
    PXClear(PXFileCreateInfo, &pxFileOpenInfo);
    pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenInfo.FlagList = PXFileIOInfoFilePhysical;

    char text[260];
    PXTextFromAdressA(&pxFileOpenInfo.FilePath, text, 0, sizeof(text));
    PXTextPrint(&pxFileOpenInfo.FilePath, "N:/NAS/Games/PC/Juiced/tracks/Angel North Central/angel north central.d8t");


    PXFileCreate(&texturePool, &pxFileOpenInfo);



    PXClear(PXFileCreateInfo, &pxFileOpenInfo);
    pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenInfo.FlagList = PXFileIOInfoFilePhysical;

    PXTextFromAdressA(&pxFileOpenInfo.FilePath, text, 0, sizeof(text));
    //PXTextPrint(&pxFileOpenInfo.FilePath, "N:/NAS/Games/PC/Juiced/tracks/Angel North Central/angel north central_anc_trees.d8g");
    PXTextPrint(&pxFileOpenInfo.FilePath, "N:/NAS/Games/PC/Juiced/tracks/Angel North Central/angel north central_ANC_building.d8g");
    //PXTextPrint(&pxFileOpenInfo.FilePath, "N:/NAS/Games/PC/Juiced/tracks/Angel North Central/angel north central_anc_hills.d8g");
    

    PXFileCreate(&mesh, &pxFileOpenInfo);


    PXECSCreateInfo pxECSCreateInfoSub;
    PXClear(PXECSCreateInfo, &pxECSCreateInfoSub);
    pxECSCreateInfoSub.FileCurrent = mesh;
    
    PXModel* pxModel = 0;

    PXD8GLoadFromFile(&pxModel, &pxECSCreateInfoSub);

    PXSceneAddModel(pxScene, pxModel);



    PXFileReadI32U(pxFile, &pxD8XDat->TableEntryAmount);
    PXFileReadI32U(pxFile, &pxD8XDat->TableOffset);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXD8DText,
        "Header",
        "\n"
        "%20s : %8i\n"
        "%20s : %8i\n",
        "TableEntryAmount", pxD8XDat->TableEntryAmount,
        "TableOffset", pxD8XDat->TableOffset
    );
#endif

    PXBool invalidLength = PXFileIsIndexInRegion(pxFile, pxD8XDat->TableOffset);

    if(!invalidLength)
    {
        return PXActionRefusedIndexOutOfBounce;
    }

    pxD8XDat->PXD8XDatEntry = PXMemoryHeapCallocT(PXD8XArchiveEntry, pxD8XDat->TableEntryAmount);

    PXFileCursorMoveTo(pxFile, pxD8XDat->TableOffset);

    for(PXI32U i = 0; i < pxD8XDat->TableEntryAmount; ++i)
    {
        PXD8XArchiveEntry* pxD8XDatEntry = &pxD8XDat->PXD8XDatEntry[i];

        PXFileReadI32U(pxFile, &pxD8XDatEntry->Hash);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->Offset);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->ChunksCount);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->CompressedSize);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->DecompressedSize);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXD8DText,
            "Entry",
            "\n"
            "%20s : %8.8x\n"
            "%20s : %8i\n"
            "%20s : %8i\n"
            "%20s : %8i\n"
            "%20s : %8i\n",
            "Hash", pxD8XDatEntry->Hash,
            "Offset", pxD8XDatEntry->Offset,
            "ChunksCount", pxD8XDatEntry->ChunksCount,
            "CompressedSize", pxD8XDatEntry->CompressedSize,
            "DecompressedSize", pxD8XDatEntry->DecompressedSize
        );
#endif
    }

    for(PXI32U i = 0; i < pxD8XDat->TableEntryAmount; ++i)
    {
        PXD8XArchiveEntry* pxD8XDatEntry = &pxD8XDat->PXD8XDatEntry[i];

        // Jump to data
        PXFileCursorMoveTo(pxFile, pxD8XDatEntry->Offset);

        // 0 means we dont have compression, data is "as is"?
        PXBool isCompressed = pxD8XDatEntry->CompressedSize != 0;


        PXFileCreateInfo pxFileOpenInfo;
        PXClear(PXFileCreateInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeWriteOnly;
        pxFileOpenInfo.FileSizeRequest = pxD8XDatEntry->DecompressedSize + 4096 * 10;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;

        PXFileCreate(&pxD8XDatEntry->FileData, &pxFileOpenInfo);


        if(isCompressed)
        {
            // We dont have one single continius ZLIB stream, it is chunked.. for some reason.
            PXI32U offset = 0;
            PXI32S compressedSize = 0;
            PXFile* pxFileChunk = PXNull;
            PXFile* pxFileChunkOut = PXNull;

            for(PXI32U chunkIndex = 0; chunkIndex < pxD8XDatEntry->ChunksCount; ++chunkIndex)
            {
                PXFileReadI32U(pxFile, &offset);
                PXFileReadI32S(pxFile, &compressedSize);

                PXSize returnPosition = PXFileDataPosition(pxFile);

                // Jump to location
                PXFileCursorMoveTo(pxFile, offset);

                {
                    PXFileCreateInfo pxFileOpenInfo;
                    PXClear(PXFileCreateInfo, &pxFileOpenInfo);
                    pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
                    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                    pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
                    PXBufferSet
                    (
                        &pxFileOpenInfo.Data,
                        (PXSize)PXFileDataAtCursor(pxFile), // +2??
                        compressedSize// - 2
                    );
                    PXFileCreate(&pxFileChunk, &pxFileOpenInfo);

                    PXClear(PXFileCreateInfo, &pxFileOpenInfo);
                    pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
                    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                    pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
                    PXBufferSet
                    (
                        &pxFileOpenInfo.Data,
                        PXFileDataAtCursor(pxD8XDatEntry->FileData),
                        PXFileRemainingSize(pxD8XDatEntry->FileData)
                    );
                    PXFileCreate(&pxFileChunkOut, &pxFileOpenInfo);
                }

                //PXFileCursorMoveTo(pxFileChunk, 2);                

                PXZLIBDecompress(pxFileChunk, pxFileChunkOut);

                PXFileCursorAdvance(pxD8XDatEntry->FileData, PXFileDataPosition(pxFileChunkOut));

                // We go back
                PXFileCursorMoveTo(pxFile, returnPosition);
            }

            //PXFileRelese(pxFileChunk);
        }
        else
        {
            PXFileReadB(pxFile, pxD8XDatEntry->FileData, pxD8XDatEntry->DecompressedSize);
        }

        char* extension = "unkown";

        PXBuffer* pxBuffer = PXFileBufferGET(pxD8XDatEntry->FileData);
        PXBool isD8W = PXTextCompareA
        (
            &pxBuffer->Data[28],
            3,
            "DXT",
            3,
            PXTextCompareRequireSameLength
        );
        PXBool isJet = PXTextCompareA
        (
            pxBuffer->Data,
            18,
            ";; Jet Spline File",
            18,
            PXTextCompareRequireSameLength
        );

        PXBool isAtm = PXTextCompareA
        (
            pxBuffer->Data,
            17,
            "[BeginAtmosphere]",
            17,
            PXTextCompareRequireSameLength
        );



        if(isJet)
        {
            extension = "spl";
        }
        if(isJet)
        {
            extension = "atm";
        }

        if(isD8W)
        {
            isD8W = PXTextCompareA
            (
                &pxBuffer->Data[76],
                3,
                "DXT",
                3,
                PXTextCompareRequireSameLength
            );
            extension = "d8w";
        }

        if(isD8W)
        {
            PXSize oldPos = PXFileDataPosition(pxD8XDatEntry->FileData);

            PXFileCursorToBeginning(pxD8XDatEntry->FileData);

            PXECSCreateInfo pxECSCreateInfoSub;
            PXClear(PXECSCreateInfo, &pxECSCreateInfoSub);
            pxECSCreateInfoSub.FileCurrent = pxD8XDatEntry->FileData;

            //PXD8WLoadFromFile(pxScene->TexturePool, &pxECSCreateInfoSub, texturePool);

            PXFileCursorMoveTo(pxD8XDatEntry->FileData, oldPos);         
        }


        char fileName[64];
        PXTextFromAdressA(&pxFileOpenInfo.FilePath, fileName, 0, sizeof(fileName));
        PXTextPrint(&pxFileOpenInfo.FilePath, "J/Unnamed_%8x8.%s", pxD8XDatEntry->Hash, extension);

        //PXFileStoreOnDiskA(pxD8XDatEntry->FileData, fileName);
        //PXFileClose(pxD8XDatEntry->FileData);
    }


    // We need to bind texture data with model material IDs       
    PXSize amount = PXModelPoolModelAmount(pxScene->ModelPool);

    for(PXSize i = 0; i < amount; i++)
    {
        PXModel* pxModel = PXModelPoolModelGet(pxScene->ModelPool, i);
                
        PXSize meshAmount = PXModelMeshAmountGet(pxModel);

        for(PXSize meshIndex = 0; meshIndex < meshAmount; ++meshIndex)
        {
            PXMesh PXREF pxMesh = PXModelMeshGet(pxModel, meshIndex);;

            pxMesh->Texture = PXTexturePoolTextureGet(pxScene->TexturePool, pxMesh->TextureID);
        }
    }
            


    return PXResultOK;
}


