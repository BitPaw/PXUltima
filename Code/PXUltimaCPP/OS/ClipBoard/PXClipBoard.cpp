#include "PXClipBoard.hpp"

PX::ActionResult PX::ClipBoard::Open()
{
	return (PX::ActionResult)PXClipBoardOpen(this);
}

PX::ActionResult PX::ClipBoard::Close()
{
	return (PX::ActionResult)PXClipBoardClose(this);;
}

PX::ActionResult PX::ClipBoard::Set(const ClipBoardFormat format, const void* data)
{
	return (PX::ActionResult)PXClipBoardSet(this, (PXClipBoardFormat)format, data);
}

PX::ActionResult PX::ClipBoard::Clear()
{
	return (PX::ActionResult)PXClipBoardClear(this);
}
