#include "PXDirectory.hpp"

PX::ActionResult PX::Directory::Create(PX::Text& text)
{
	return (PX::ActionResult)PXDirectoryCreate(&text);
}

PX::ActionResult PX::Directory::Delete(PX::Text& text)
{
	return (PX::ActionResult)PXDirectoryDelete(&text);
}

PX::ActionResult PX::DirectoryIterator::Open(PX::Text& text)
{
	return (PX::ActionResult)PXDirectoryOpen(this, &text);
}

PX::ActionResult PX::DirectoryIterator::Next()
{
	return (PX::ActionResult)PXDirectoryNext(this);
}

PX::ActionResult PX::DirectoryIterator::Close()
{
	return (PX::ActionResult)PXDirectoryClose(this);
}
