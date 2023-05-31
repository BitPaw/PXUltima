#include "PXController.hpp"

bool PX::Controller::ScanDevices(NewControllerDetectedCallback callback)
{
	return false;
}

bool PX::Controller::DataGet()
{
	return PXControllerDataGet(this);
}

bool PX::Controller::AttachToWindow(const PXWindowID pxWindowID)
{
	return PXControllerAttachToWindow(this->ID, pxWindowID);
}

bool PX::Controller::DetachToWindow()
{
	return PXControllerDetachToWindow(this->ID);
}