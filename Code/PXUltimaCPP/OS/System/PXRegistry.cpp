#include "PXRegistry.hpp"

PX::Registry::Registry()
{
	
}

PX::Registry::~Registry()
{
}

PX::ActionResult PX::Registry::ConnectSpace(const PXRegistrySpace registrySpace)
{
	return (PX::ActionResult)PXRegistryConnectSpace(this, registrySpace);
}

PX::ActionResult PX::Registry::ConnectRemote(const PX::Text& computerName, const PXRegistrySpace registrySpace)
{
	return (PX::ActionResult)PXRegistryConnectRemote(this, &computerName, registrySpace);
}

void PX::Registry::Close()
{
	PXRegistryClose(this);
}

void PX::Registry::KeyCreate()
{
	
}

void PX::Registry::KeyLoad(PX::Text& file)
{
}

void PX::Registry::KeySave()
{
}

void PX::Registry::KeyDelete()
{
}
