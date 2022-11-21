#include "HardDrive.h"

#include <OS/OSVersion.h>
#include <Memory/PXMemory.h>

#if OSUnix
#elif OSWindows
#include <direct.h>
#endif

size_t HardDriveListSize()
{
#if OSUnix
    const size_t numberOfDrives = 0;

#elif OSWindows

    size_t bitmaskDrives = _getdrives();
    size_t numberOfDrives = 0;

    for (size_t indexDrive = 1; indexDrive <= 26u; ++indexDrive)
    {
        numberOfDrives += bitmaskDrives & 1;

        bitmaskDrives >>= 1;
    }

#endif

    return numberOfDrives;
}

ActionResult HardDriveFetchAll(HardDrive* const hardDriveList, const size_t hardDriveListMaxSize, size_t* hardDriveListSize)
{
    const size_t numberOfDrives = HardDriveListSize();

    MemoryClear(hardDriveList, sizeof(HardDrive) * hardDriveListMaxSize);

    // Has enough space?
    {
        const PXBool hasEnoughSpace = numberOfDrives <= hardDriveListMaxSize;

        if (!hasEnoughSpace)
        {
            return ActionFailedInputBufferTooSmal;
        }
    }

#if OSUnix
    return ActionInvalid;

#elif OSWindows   

    size_t bitmaskDrives = _getdrives();

    for (size_t indexDrive = 1; numberOfDrives; ++indexDrive)
    {
        const PXBool isUsed = bitmaskDrives & 1;

        if (isUsed)
        {
            HardDrive* harddrive = &hardDriveList[numberOfDrives-1];
            struct _diskfree_t df = { 0 };
            const unsigned int uErr = _getdiskfree(numberOfDrives, &df);
            const unsigned char successul = 0 == uErr;

            if (successul)
            {
                harddrive->Letter = 'A' + numberOfDrives - 1;
                harddrive->ClustersTotal = df.total_clusters;
                harddrive->ClustersFree = df.avail_clusters;
                harddrive->ClusterSectors = df.sectors_per_cluster;
                harddrive->ClusterSectorSize = df.bytes_per_sector;
            }
        }

        bitmaskDrives >>= 1;
    }
#endif

    *hardDriveListSize = (numberOfDrives);

	return ActionSuccessful;
}
