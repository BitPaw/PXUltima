#include "PXLibrary.hpp"

PX::Library::Library()
{
	PXMemoryClear(this, sizeof(PXLibrary));
}

PX::Library::Library(const Text& const filePath)
{
	PXLibraryOpen(this, &filePath);
}

PX::Library::~Library()
{
	PXLibraryClose(this);
}

bool PX::Library::Open(const Text& const filePath)
{
	return PXLibraryOpen(this, &filePath);
}

bool PX::Library::Close()
{
	return PXLibraryClose(this);
}

bool PX::Library::GetSymbol(void** libraryFunction, const Text& symbolName)
{
	return PXLibraryGetSymbol(this, libraryFunction, &symbolName);
}

PX::ActionResult PX::Library::Name(Text& const libraryName)
{
	return (PX::ActionResult)PXLibraryName(this, &libraryName);
}

bool PX::Library::ParseSymbols()
{
	return false;
}