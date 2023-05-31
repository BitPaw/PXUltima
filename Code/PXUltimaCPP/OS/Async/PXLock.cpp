#include "PXLock.hpp"

PX::Lock::Lock()
{
	PXLockClear(this);
}

PX::Lock::~Lock()
{
	PXLockDelete(this);
}

void PX::Lock::Clear()
{
	PXLockClear(this);
}

PX::ActionResult PX::Lock::Create(const LockType type)
{
	return (PX::ActionResult)PXLockCreate(this, (PXLockType)type);
}

PX::ActionResult PX::Lock::Delete()
{
	return (PX::ActionResult)PXLockDelete(this);
}

PX::ActionResult PX::Lock::Engage()
{
	return	(PX::ActionResult)PXLockEngage(this);
}

PX::ActionResult PX::Lock::Release()
{
	return (PX::ActionResult)PXLockRelease(this);
}
