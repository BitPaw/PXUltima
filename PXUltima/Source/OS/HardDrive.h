#ifndef HardDriveINCLUDE
#define HardDriveINCLUDE

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct HardDrive_
	{
		unsigned char Letter; // Windows Only

		size_t ClustersTotal;      // The total number of clusters, both used and available, on the disk.
		size_t ClustersFree;      // The number of unused clusters on the disk.
		size_t ClusterSectors; // The number of sectors in each cluster.
		size_t ClusterSectorSize;    // The size of each sector in bytes.
	}
	HardDrive;

	CPublic ActionResult HardDriveFetchAll(HardDrive* const hardDriveList, const size_t hardDriveListMaxSize, size_t* hardDriveListSize);

#ifdef __cplusplus
}
#endif

#endif