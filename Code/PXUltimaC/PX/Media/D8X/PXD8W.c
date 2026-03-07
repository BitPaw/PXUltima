#include "PXD8W.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/DDS/PXDDS.h>

#define PXD8WDebugLog 0

const char PXD8WName[] = "D8W";




const PXI32U PXD8WHeaderList[] =
{
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
};
const PXI8U PXD8WHeaderListSize = sizeof(PXD8WHeaderList) / sizeof(PXI32U);

const PXI32U PXD8WTextureTableEntryList[] =
{
    PXTypeI32ULE,
    PXTypeText(4),
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
    PXTypeI32ULE,
};
const PXI8U PXD8WTextureTableEntryListSize = sizeof(PXD8WTextureTableEntryList) / sizeof(PXI32U);









int counter = 0;

#include <intrin.h>

PXResult PXAPI PXD8WLoadFromFile(PXTexturePool PXREF pxTexturePool, PXECSCreateInfo PXREF pxResourceLoadInfo, PXFile* pxTexturePoolFile)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;
    PXSize poolStartIndex = PXTexturePoolAmountGet(pxTexturePool);
        

    PXD8TextureTable dummy;
    PXClear(PXD8TextureTable, &dummy);

    PXD8TextureTable* textureTable = &dummy;


    PXFileBinding
    (
        pxFile,
        textureTable,
        PXD8WHeaderList,
        PXD8WHeaderListSize,
        PXFileBindingRead
    );

    textureTable->SetList = PXMemoryHeapCallocT(PXD8TextureSet, textureTable->TextureSetAmount);
    textureTable->EntryList = PXMemoryHeapCallocT(PXD8TextureTableEntry, textureTable->TextureAmount);


    // Create textures to store into
    PXTexturePoolReserveAmount(pxTexturePool, poolStartIndex + textureTable->TextureSetAmount);

    PXI32U totalOffset = 0;
    PXI32U totalIndex = 0;

    // For every set..
    // Start at index 12
    for(PXI32U setIndex = 0; setIndex < textureTable->TextureSetAmount; ++setIndex)
    {
        PXD8TextureSet* pxD8TextureSet = &textureTable->SetList[setIndex];
        PXTextureSet PXREF pxTextureSet = PXTexturePoolSetGet(pxTexturePool, poolStartIndex + setIndex);

        PXFileBinding
        (
            pxFile,
            pxD8TextureSet,
            PXD8WHeaderList, // We reuse the header for this subheader!
            PXD8WHeaderListSize,
            PXFileBindingRead
        );

        PXTextureSetReserveAmount(pxTextureSet, pxD8TextureSet->TextureEntryAmount);

#if PXLogEnable && PXD8WDebugLog
        PXLogPrint
        (
            PXLoggingInfo,
            PXD8WName,
            "Texture",
            "Set (%i/%i), Amount:<%i>",
            setIndex + 1,
            textureTable->TextureSetAmount,
            pxD8TextureSet->TextureEntryAmount
        );
#endif

        // In each set, list our list
        for(PXI32U textureIndex = 0; textureIndex < pxD8TextureSet->TextureEntryAmount; ++textureIndex)
        {
            PXD8TextureTableEntry* pxD8TextureTableEntry = &textureTable->EntryList[totalIndex];

            ++totalIndex;

            PXFileBinding
            (
                pxFile,
                pxD8TextureSet,
                PXD8WTextureTableEntryList, // We reuse the header for this subheader!
                PXD8WTextureTableEntryListSize,
                PXFileBindingRead
            );

            pxD8TextureTableEntry->Offset = totalOffset;
            totalOffset += pxD8TextureTableEntry->RawDataSize;

#if PXLogEnable && PXD8WDebugLog
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
                pxD8TextureTableEntry->Offset * 100 / sizeMax
            );
#endif
        }
    }

#if 1
    for(PXI32U setIndex = 0; setIndex < textureTable->TextureSetAmount; ++setIndex)
    {
        PXTextureSet PXREF pxTextureSet = PXTexturePoolSetGet(pxTexturePool, poolStartIndex + setIndex);
        PXD8TextureSet* pxD8TextureSet = &textureTable->SetList[setIndex];

        for(PXI32U textureIndex = 0; textureIndex < pxD8TextureSet->TextureEntryAmount; ++textureIndex)
        {
            PXD8TextureTableEntry PXREF pxD8TextureTableEntry = &textureTable->EntryList[textureIndex];

            PXTexture* pxTexture = 0;

            //-------------------------------------------------
            // Create texture
            PXTextureCreateInfo pxTextureCreateInfo;
            PXClear(PXTextureCreateInfo, &pxTextureCreateInfo);
            pxTextureCreateInfo.Type = PXTextureType2D;
            pxTextureCreateInfo.Format = PXDDSTypeDetect(pxD8TextureTableEntry->PixelFormat);
            pxTextureCreateInfo.T2D.Height = pxD8TextureTableEntry->TextureHeight;
            pxTextureCreateInfo.T2D.Width = pxD8TextureTableEntry->TextureWidth;
            pxTextureCreateInfo.T2D.Minmaps = pxD8TextureTableEntry->MinmapCount;

            PXTextureCreate(&pxTexture, &pxTextureCreateInfo);
            //-------------------------------------------------

            PXTextureSetEntryAdd(pxTextureSet, pxTexture);

            //-------------------------------------------------
            // Fill texture with data
            PXAssert(pxTexture->PixelData.SizeAllocated >= pxD8TextureTableEntry->RawDataSize, "Should be equal");

            PXFileCursorMoveTo(pxTexturePoolFile, pxD8TextureTableEntry->Offset);
            PXFileReadB(pxTexturePoolFile, pxTexture->PixelData.Data, pxD8TextureTableEntry->RawDataSize);
            //-------------------------------------------------
        }
    }
#endif

    return PXResultOK;
}



/*
* 
* 

#if 0
    DECOMPRESSOR_HANDLE h;
    CreateDecompressor(COMPRESS_ALGORITHM_DEFLATE, NULL, &h);

    Decompress(h, input, inputSize, output, outputSize, &bytesDecompressed);

    CloseDecompressor(h);
#endif
* 
PXFileCreateInfo pxFileCreateInfo;
        PXClear(PXFileCreateInfo, &pxFileCreateInfo);
        pxFileCreateInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileCreateInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCreateInfo.FlagList = PXFileIOInfoFileVirtual;
        pxFileCreateInfo.FileSizeRequest = pxD8TextureTableEntry->RawDataSize + 128;

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




        PXText pxText;
        char fileName[64];
        PXTextFromAdressA(&pxText, fileName, 0, sizeof(fileName));
        PXTextPrint(&pxText, "J/Q/Unnamed_UUPx%i.dds", ++counter);

        PXFileStoreOnDiskA(pxFileOUT, fileName);
        PXFileClose(pxFileOUT);

        //PXFileCursorAdvance(pxFile, &pxD8TextureTableEntry->RawDataSize);

*/