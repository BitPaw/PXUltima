#include "PXHardDrive.hpp"

PXSize PX::HardDrive::ListSize()
{
    return PXHardDriveListSize();
}

PX::ActionResult PX::HardDrive::FetchAll(PXHardDrive* const hardDriveList, const PXSize hardDriveListMaxSize, PXSize* hardDriveListSize)
{
    return (PX::ActionResult)PXHardDriveFetchAll(hardDriveList, hardDriveListMaxSize, hardDriveListSize);
}
