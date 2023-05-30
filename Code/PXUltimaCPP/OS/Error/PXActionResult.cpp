#include "PXActionResult.hpp"

PX::ActionResult PX::ErrorCurrent()
{
	return (PX::ActionResult)PXErrorCurrent();
}