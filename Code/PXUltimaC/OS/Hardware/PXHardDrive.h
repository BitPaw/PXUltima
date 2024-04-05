#ifndef PXHardDriveINCLUDE
#define PXHardDriveINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

typedef struct PXHardDrive_
{
	char Letter; // Windows Only

	PXSize ClustersTotal;      // The total number of clusters, both used and available, on the disk.
	PXSize ClustersFree;      // The number of unused clusters on the disk.
	PXSize ClusterSectors; // The number of sectors in each cluster.
	PXSize ClusterSectorSize;    // The size of each sector in bytes.
}
PXHardDrive;

PXPublic PXSize PXAPI PXHardDriveListSize();

PXPublic PXActionResult PXAPI PXHardDriveFetchAll(PXHardDrive* const hardDriveList, const PXSize hardDriveListMaxSize, PXSize* hardDriveListSize);

#endif