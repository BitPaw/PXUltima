#include "Lock.hpp"

void PX::Lock::Clear()
{
	PXLockClear(this);
}

ActionResult PX::Lock::Create()
{
	return PXLockCreate(this);
}

ActionResult PX::Lock::Delete()
{
	return PXLockDelete(this);
}

ActionResult PX::Lock::Engage()
{
	return PXLockEngage(this);
}

ActionResult PX::Lock::Release()
{
	return PXLockRelease(this);
}