#include "HardDrive.h"

#include <OS/OSVersion.h>
#include <Memory/Memory.h>

#if OSUnix
#elif OSWindows
#include <direct.h>
#endif

ActionResult HardDriveFetchAll(HardDrive* const hardDriveList, const size_t hardDriveListMaxSize, size_t* hardDriveListSize)
{
    MemorySet(hardDriveList, sizeof(HardDrive) * hardDriveListMaxSize, 0);

#if OSUnix
    return ActionInvalid;

#elif OSWindows
    ULONG uDriveMask = _getdrives();

    size_t hardDriveListIndex = 0;

    for (size_t uDrive = 1; uDrive <= hardDriveListMaxSize && uDrive <= 26u; ++uDrive)
    {
        if (uDriveMask & 1)
        {
            HardDrive* harddrive = &hardDriveList[hardDriveListIndex];
            struct _diskfree_t df = { 0 };
            const unsigned int uErr = _getdiskfree(uDrive, &df);
            const unsigned char successul = 0 == uErr;

            if (successul)
            {
                harddrive->Letter = 'A' + uDrive - 1;
                harddrive->ClustersTotal = df.total_clusters;
                harddrive->ClustersFree = df.avail_clusters;
                harddrive->ClusterSectors = df.sectors_per_cluster;
                harddrive->ClusterSectorSize = df.bytes_per_sector;

                ++hardDriveListIndex;
            }
        }

        uDriveMask >>= 1;
    }

    *hardDriveListSize = (hardDriveListIndex);
#endif

	return ActionSuccessful;
}
