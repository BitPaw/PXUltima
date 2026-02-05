#include "PXD8X.h"

#include <PX/OS/PXOS.h>
#include <PX/Media/ZLIB/PXZLIB.h>
#include <PX/OS/Console/PXConsole.h>
#include <compressapi.h>
#include <PX/Media/DDS/PXDDS.h>
#include <ddraw.h>

const char PXD8XName[] = "D8X";
const char PXD8WName[] = "D8W";




const PXI32U PXDDSHeaderHeaderBindingAA[] =
{
PXTypeI32ULE,//  Size
PXTypeI32ULE,//  Flags
PXTypeI32ULE,//  Height
PXTypeI32ULE,//  Width
PXTypeI32ULE,//  PitchOrLinearSize
PXTypeI32ULE,//  Depth
PXTypeI32ULE,//  MipMapCount
PXTypeText(44),// Name
PXTypeI32ULE,//  PixelFormatSize
PXTypeI32ULE,//  PixelFormatFlags
PXTypeText(4),// PixelFormatID[4]
PXTypeI32ULE,// RGBBitCount
PXTypeI32ULE,// RBitMask
PXTypeI32ULE,// GBitMask
PXTypeI32ULE,// BBitMask
PXTypeI32ULE,// ABitMask
PXTypeI32ULE,// Caps1
PXTypeI32ULE,// Caps2
PXTypeI32ULE,// Caps3
PXTypeI32ULE,// Caps4
PXTypeI32ULE // Reserved2
};
const PXI8U PXDDSHeaderHeaderBindingSizeAA = sizeof(PXDDSHeaderHeaderBindingAA) / sizeof(PXI32U);

PXResult PXAPI PXD8XLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXD8X pxD8X;

    return PXResultOK;
}

PXResult PXAPI PXD8DatLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
  // PXPNG* png = PXNull;
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXD8XDat dummy;
    PXClear(PXD8XDat, &dummy);

    PXD8XDat* pxD8XDat = &dummy;



    // a .dat can contain a .d8w. Those need a .d8t. Lets seek if we got one
    PXFile* texturePool = 0;

    PXFileCreateInfo pxFileOpenInfo;
    PXClear(PXFileCreateInfo, &pxFileOpenInfo);
    pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenInfo.FlagList = PXFileIOInfoFilePhysical;

    char text[260];
    PXTextFromAdressA(&pxFileOpenInfo.FilePath, text, 0, sizeof(text));
    PXTextPrint(&pxFileOpenInfo.FilePath, "N:/NAS/Games/PC/Juiced/tracks/Angel North Central/angel north central.d8t");

    PXFileCreate(&texturePool, &pxFileOpenInfo);








    PXFileReadI32U(pxFile, &pxD8XDat->TableEntryAmount);
    PXFileReadI32U(pxFile, &pxD8XDat->TableOffset);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXD8XName,
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
            PXD8XName,
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
        pxFileOpenInfo.FileSizeRequest = pxD8XDatEntry->DecompressedSize+4096*10;
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

            PXD8WLoadFromFile(&pxECSCreateInfoSub, texturePool);

            PXFileCursorMoveTo(pxD8XDatEntry->FileData, oldPos);
        }


        char fileName[64];
        PXTextFromAdressA(&pxFileOpenInfo.FilePath, fileName, 0, sizeof(fileName));
        PXTextPrint(&pxFileOpenInfo.FilePath, "J/Unnamed_%8x8.%s", pxD8XDatEntry->Hash, extension);

        PXFileStoreOnDiskA(pxD8XDatEntry->FileData, fileName);
        PXFileClose(pxD8XDatEntry->FileData);
    }

    return PXResultOK;
}

void PXAPI PXD8GGeometryVertexBlock(PXD8GeometryVertexBlock PXREF pxD8GeometryVertexBlock, PXFile PXREF pxFile)
{
    PXD8GGeometryVertexFVFParse(&pxD8GeometryVertexBlock->vertices, pxFile);
    PXFileReadI32U(pxFile, &pxD8GeometryVertexBlock->definition_count);

    for(PXI32U i = 0; i < pxD8GeometryVertexBlock->definition_count; ++i)
    {
        PXD8GeometryVertexFVF* pxD8GeometryVertexFVFsSub = &pxD8GeometryVertexBlock->definitions[i];

        PXD8GGeometryVertexFVFParse(pxD8GeometryVertexFVFsSub, pxFile);
    }

    PXFileReadI32U(pxFile, &pxD8GeometryVertexBlock->BytesBetweenVertices);
    PXFileReadI32U(pxFile, &pxD8GeometryVertexBlock->NumberOfVertices);
}

void PXAPI PXD8GGeometryVertexFVFParse(PXD8GeometryVertexFVF PXREF pxD8GeometryVertexFVF, PXFile PXREF pxFile)
{
    PXFileReadI16U(pxFile, &pxD8GeometryVertexFVF->Flag);
    PXFileReadI16U(pxFile, &pxD8GeometryVertexFVF->Offset);
    PXFileReadI16U(pxFile, &pxD8GeometryVertexFVF->Format);
    PXFileReadI8U(pxFile, &pxD8GeometryVertexFVF->VertexType);
    PXFileReadI8U(pxFile, &pxD8GeometryVertexFVF->UVChannelIndex);
}

PXResult PXAPI PXD8GLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXD8Geometry dummy;
    PXClear(PXD8XDat, &dummy);

    PXD8Geometry* pxD8Geometry = &dummy;

    PXFileReadI32U(pxFile, &pxD8Geometry->Header.VertexType);
    PXFileReadI32U(pxFile, &pxD8Geometry->Header.FaceType);
    PXFileReadI32U(pxFile, &pxD8Geometry->Header.MeshAmount);

    pxD8Geometry->MeshList = PXMemoryHeapCallocT(PXD8GeometryMesh, pxD8Geometry->Header.MeshAmount);

    for(PXSize i = 0; i < pxD8Geometry->Header.MeshAmount; ++i)
    {
        PXD8GeometryMesh* mesh = &pxD8Geometry->MeshList[i];

        PXFileReadI32U(pxFile, &mesh->TextureID);
        PXFileReadB(pxFile, &mesh->Position, sizeof(float) * 4);
        PXFileReadI32U(pxFile, &mesh->RadiusBoundingSphere);
        PXFileReadI32U(pxFile, &mesh->DistanceLODMin);
        PXFileReadI32U(pxFile, &mesh->DistanceLODMax);
        PXFileReadI32U(pxFile, &mesh->UnknownA);

        if(0 == mesh->UnknownA)
        {
            PXFileReadI32U(pxFile, &mesh->UnknownB);
            PXFileReadI32U(pxFile, &mesh->UnknownC);
            PXFileReadI32U(pxFile, &mesh->UnknownD);
        }
        else
        {
            PXFileCursorRewind(pxFile, sizeof(PXI32U));
        }

        PXD8GGeometryVertexBlock(&mesh->VertexBlock, pxFile);

        PXFileReadI32U(pxFile, &mesh->face_count);
        mesh->IndexListArray = PXFileDataPosition(pxFile);
        mesh->IndexListSize = sizeof(PXI16U) * mesh->face_count;
        PXFileCursorAdvance(pxFile, mesh->IndexListSize);

        PXFileReadI32U(pxFile, &mesh->node_count);
        mesh->NodeListAdress = PXFileDataPosition(pxFile);
        mesh->NodeListSize = sizeof(PXMatrix4x4F) * mesh->node_count;
        PXFileCursorAdvance(pxFile, mesh->NodeListSize);

    }

    return PXResultOK;
}

int counter = 0;

PXResult PXAPI PXD8WLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo, PXFile* pxTexturePool)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;


    PXD8TextureTable dummy;
    PXClear(PXD8TextureTable, &dummy);

    PXD8TextureTable* textureTable = &dummy;

    PXFileReadI32U(pxFile, &textureTable->TextureAmount);
    PXFileReadI32U(pxFile, &textureTable->TextureSetAmount);
    PXFileReadI32U(pxFile, &textureTable->TotalSizeRequested);

    textureTable->SetList = PXMemoryHeapCallocT(PXD8TextureSet, textureTable->TextureSetAmount);
    textureTable->EntryList = PXMemoryHeapCallocT(PXD8TextureTableEntry, textureTable->TextureAmount);

    PXI32U totalOffset = 0;
    PXI32U totalIndex = 0;

    // For every set..
    // Start at index 12
    for(PXI32U setIndex = 0; setIndex < textureTable->TextureSetAmount; ++setIndex)
    {
        PXD8TextureSet* pxD8TextureSet = &textureTable->SetList[setIndex];

        char* point = PXFileDataAtCursor(pxFile);

        PXFileReadI32U(pxFile, &pxD8TextureSet->TextureSetDataSize);
        PXFileReadI32U(pxFile, &pxD8TextureSet->TextureTotalSize);
        PXFileReadI32U(pxFile, &pxD8TextureSet->TextureEntryAmount);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXD8WName,
            "Texture",
            "Set (%i/%i), Amount:<%i>",
            setIndex +1,
            textureTable->TextureSetAmount,
            pxD8TextureSet->TextureEntryAmount
        );
#endif

        // In each set, list our list
        for(PXI32U textureIndex = 0; textureIndex < pxD8TextureSet->TextureEntryAmount; ++textureIndex)
        {            
            PXD8TextureTableEntry* pxD8TextureTableEntry = &textureTable->EntryList[totalIndex];

            ++totalIndex;

            char* point = PXFileDataAtCursor(pxFile);

            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->RawDataSize);
            PXFileReadB(pxFile, pxD8TextureTableEntry->PixelFormat, 4);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->TextureWidth);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->TextureHeight);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->MinmapCount);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownB);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownC);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownD);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownE);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownF);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownG);
            PXFileReadI32U(pxFile, &pxD8TextureTableEntry->UnknownH);


            pxD8TextureTableEntry->Offset = totalOffset;
            totalOffset += pxD8TextureTableEntry->RawDataSize;

#if PXLogEnable
            PXSize sizeMax = PXFileSizeToRead(pxTexturePool);

            PXLogPrint
            (
                PXLoggingInfo,
                PXD8WName,
                "Texture",
                "%c%c%c%c %4ix%-4i (%8i @ %8i/%8i <%3i>) ",
                pxD8TextureTableEntry->PixelFormat[0],
                pxD8TextureTableEntry->PixelFormat[1],
                pxD8TextureTableEntry->PixelFormat[2],
                pxD8TextureTableEntry->PixelFormat[3],
                pxD8TextureTableEntry->TextureWidth,
                pxD8TextureTableEntry->TextureHeight,
                pxD8TextureTableEntry->RawDataSize,
                pxD8TextureTableEntry->Offset,
                sizeMax,
                pxD8TextureTableEntry->Offset * 100/ sizeMax
            );
#endif

        }
    }





    
#if 0
    DECOMPRESSOR_HANDLE h;
    CreateDecompressor(COMPRESS_ALGORITHM_DEFLATE, NULL, &h);

    Decompress(h, input, inputSize, output, outputSize, &bytesDecompressed);

    CloseDecompressor(h);
#endif


#if 1
    for(PXI32U i = 0; i < textureTable->TextureAmount; ++i)
    {
        PXD8TextureTableEntry* pxD8TextureTableEntry = &textureTable->EntryList[i];


        PXFileCreateInfo pxFileCreateInfo;
        PXClear(PXFileCreateInfo, &pxFileCreateInfo);
        pxFileCreateInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileCreateInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCreateInfo.FlagList = PXFileIOInfoFileVirtual;
        pxFileCreateInfo.FileSizeRequest = pxD8TextureTableEntry->RawDataSize+128;

        PXFileCreate(&pxD8TextureTableEntry->TextureFile, &pxFileCreateInfo);


        PXFile* pxFileOUT = pxD8TextureTableEntry->TextureFile;

        PXFileWriteB(pxFileOUT, "DDS ", 4);

        PXDDSHeader pxDDSHeader;
        PXClear(PXDDSHeader, &pxDDSHeader);
        pxDDSHeader.Size = 124; // sizeof header
        pxDDSHeader.Flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        pxDDSHeader.Height = pxD8TextureTableEntry->TextureHeight;
        pxDDSHeader.Width = pxD8TextureTableEntry->TextureWidth;
        pxDDSHeader.MipMapCount = (pxD8TextureTableEntry->MinmapCount > 1) ? pxD8TextureTableEntry->MinmapCount : 0;

        int bpp = 0;

        if(pxD8TextureTableEntry->PixelFormat[0] != 0) {
            // compressed (e.g. DXT1/3/5)
            pxDDSHeader.Flags |= DDSD_LINEARSIZE;
            pxDDSHeader.PitchOrLinearSize = pxD8TextureTableEntry->RawDataSize; // total size of top level (or whole chain if you prefer)

            pxDDSHeader.PixelFormatSize = 32;
            pxDDSHeader.PixelFormatFlags = DDPF_FOURCC;
            pxDDSHeader.PixelFormatID[0] = pxD8TextureTableEntry->PixelFormat[0];
            pxDDSHeader.PixelFormatID[1] = pxD8TextureTableEntry->PixelFormat[1];
            pxDDSHeader.PixelFormatID[2] = pxD8TextureTableEntry->PixelFormat[2];
            pxDDSHeader.PixelFormatID[3] = pxD8TextureTableEntry->PixelFormat[3];

            // caps
            pxDDSHeader.Caps1 = DDSCAPS_TEXTURE;
            if(pxD8TextureTableEntry->MinmapCount > 1) {
                pxDDSHeader.Caps1 |= DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
                pxDDSHeader.Flags |= DDSD_MIPMAPCOUNT;
            }
        }
        else {
            // uncompressed (e.g. RGBA8)
            pxDDSHeader.Flags |= DDSD_PITCH;
            pxDDSHeader.PitchOrLinearSize = (pxD8TextureTableEntry->TextureWidth * bpp + 7) / 8;

            pxDDSHeader.PixelFormatSize = 32;
            pxDDSHeader.PixelFormatFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
            pxDDSHeader.RGBBitCount = bpp;

            // assume 32-bit RGBA: 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000
            if(bpp == 32) {
                pxDDSHeader.RBitMask = 0x00FF0000;
                pxDDSHeader.GBitMask = 0x0000FF00;
                pxDDSHeader.BBitMask = 0x000000FF;
                pxDDSHeader.ABitMask = 0xFF000000;
            }

            pxDDSHeader.Caps1 = DDSCAPS_TEXTURE;
            if(pxD8TextureTableEntry->MinmapCount > 1) {
                pxDDSHeader.Caps1 |= DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
                pxDDSHeader.Flags |= DDSD_MIPMAPCOUNT;
            }
        }

        const PXSize readAbount = PXFileBinding
        (
            pxFileOUT,
            &pxDDSHeader,
            PXDDSHeaderHeaderBindingAA,
            PXDDSHeaderHeaderBindingSizeAA,
            PXFileBindingWrite
        );

        PXAssert(124 == readAbount, "Cant");

        PXFileCursorMoveTo(pxTexturePool, pxD8TextureTableEntry->Offset);
        PXFileRead(pxTexturePool, pxFileOUT, pxD8TextureTableEntry->RawDataSize);


        PXText pxText;
        char fileName[64];
        PXTextFromAdressA(&pxText, fileName, 0, sizeof(fileName));
        PXTextPrint(&pxText, "J/Q/Unnamed_UUPx%i.dds", ++counter);

        PXFileStoreOnDiskA(pxFileOUT, fileName);
        PXFileClose(pxFileOUT);

        //PXFileCursorAdvance(pxFile, &pxD8TextureTableEntry->RawDataSize);
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXD8TLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXD8MLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    return PXResultOK;
}
