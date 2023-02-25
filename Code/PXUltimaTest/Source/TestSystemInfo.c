#include "TestSystemInfo.h"

#include <stdio.h>

#include <OS/HardDrive/HardDrive.h>

void TestSystemInfoAll()
{
	TestSystemInfoHardDrive();
}

void TestSystemInfoHardDrive()
{
	HardDrive hardDrive[26];
	PXSize hardDriveSize = 0;

	HardDriveFetchAll(&hardDrive, 26u, &hardDriveSize);

	for (PXSize i = 0; i < hardDriveSize; i++)
	{
		HardDrive* harddrive = &hardDrive[i];

		printf
		(
			"\nDrive: %c\n"
			"\tTotal clusters:      %11u\n"
			"\tAvailable clusters:  %11u\n"
			"\tSectors per cluster: %11u\n"
			"\tBytes per sector:    %11u\n",
			harddrive->Letter,
			harddrive->ClustersTotal,
			harddrive->ClustersFree,
			harddrive->ClusterSectors,
			harddrive->ClusterSectorSize
		);
	}
}
