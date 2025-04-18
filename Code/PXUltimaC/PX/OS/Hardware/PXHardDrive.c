#include "PXHardDrive.h"

#include <PX/OS/Memory/PXMemory.h>

#if OSUnix
#elif OSWindows
#include <direct.h>
#endif

PXSize PXAPI PXHardDriveListSize()
{
#if OSUnix
    const PXSize numberOfDrives = 0;

    return numberOfDrives;

#elif PXOSWindowsDestop

    // GetLogicalDriveStringsA();


    DWORD bitmaskDrives = GetLogicalDrives(); //_getdrives(); -> Not supported in windows runtime
    DWORD numberOfDrives = 0;

    for (PXSize indexDrive = 1; indexDrive <= 26u; ++indexDrive)
    {
        numberOfDrives += bitmaskDrives & 1;

        bitmaskDrives >>= 1;
    }

    return numberOfDrives;

#else
    return 0;
#endif
}

PXActionResult PXAPI PXHardDriveFetchAll(PXHardDrive* const hardDriveList, const PXSize hardDriveListMaxSize, PXSize* hardDriveListSize)
{
    const PXSize numberOfDrives = PXHardDriveListSize();

    PXClearList(PXHardDrive, hardDriveList, hardDriveListMaxSize);

    // Has enough space?
    {
        const PXBool hasEnoughSpace = numberOfDrives <= hardDriveListMaxSize;

        if (!hasEnoughSpace)
        {
            return PXActionRefuedInputBufferTooSmal;
        }
    }

#if OSUnix
    return PXActionInvalid;

#elif PXOSWindowsDestop

    // GetDriveTypeA()
    // GetDiskFreeSpaceA

#if 0

    DWORD bitmaskDrives = GetLogicalDrives();

    for (PXSize indexDrive = 1; numberOfDrives; ++indexDrive)
    {
        const PXBool isUsed = bitmaskDrives & 1;

        if (isUsed)
        {
            PXHardDrive* harddrive = &hardDriveList[numberOfDrives-1];
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
#endif

    *hardDriveListSize = (numberOfDrives);

    return PXActionSuccessful;
}
