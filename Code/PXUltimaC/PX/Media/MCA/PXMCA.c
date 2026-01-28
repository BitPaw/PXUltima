#include "PXMCA.h"

PXResult PXAPI PXMCALoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileCurrent;

    /*

    for(PXI8U x = 0; x < 32; ++x)
    {
        for(PXI8U z = 0; z < 32; ++z)
        {
     
                DataInputStream regionChunkInputStream = regionSource.getChunkDataInputStream(x, z);
                if(regionChunkInputStream == null) {
                    System.out.println("Failed to fetch input stream");
                    continue;
                }
                CompoundTag chunkData = NbtIo.read(regionChunkInputStream);
                regionChunkInputStream.close();

                CompoundTag compound = chunkData.getCompound("Level");
                {
                    OldLevelChunk oldChunk = OldChunkStorage.load(compound);

                    CompoundTag tag = new CompoundTag();
                    CompoundTag levelData = new CompoundTag();
                    tag.put("Level", levelData);
                    OldChunkStorage.convertToAnvilFormat(oldChunk, levelData, biomeSource);

                    DataOutputStream chunkDataOutputStream = regionDest.getChunkDataOutputStream(x, z);
                    NbtIo.write(tag, chunkDataOutputStream);
                    chunkDataOutputStream.close();
                }
            
        }
        int basePercent = (int)Math.round(100.0d * (double)(currentCount * 1024) / (double)(totalCount * 1024));
        int newPercent = (int)Math.round(100.0d * (double)((x + 1) * 32 + currentCount * 1024) / (double)(totalCount * 1024));

    }
    */


    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMCASaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}