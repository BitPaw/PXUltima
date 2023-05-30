#include "PXFile.hpp"

PX::File::File()
{
	PXFileConstruct(this);
}

PX::File::~File()
{
	PXFileDestruct(this);
}

bool PX::File::DoesExist(const Text& filePath)
{
	return PXFileDoesExist(&filePath);
}

PX::ActionResult PX::File::Remove(const Text& const filePath)
{
	return (PX::ActionResult)PXFileRemove(&filePath);
}

PX::ActionResult PX::File::Rename(const Text& const oldName, const Text& const newName)
{
	return (PX::ActionResult)PXFileRename(&oldName, &newName);
}

PX::ActionResult PX::File::Copy(const Text& const sourceFilePath, const PXText& const destinationFilePath, const bool overrideIfExists)
{
	return (PX::ActionResult)PXFileCopy(&sourceFilePath, &destinationFilePath, overrideIfExists);
}

PX::ActionResult PX::File::Name(Text& const fileName)
{
	return (PX::ActionResult)PXFileName(this, &fileName);
}

void PX::File::BufferAllocate(const PXSize dataSize)
{
	PXFileBufferAllocate(this, dataSize);
}

void PX::File::BufferExternal(void* const data, const PXSize dataSize)
{
	PXFileBufferExternal(this, data, dataSize);
}

PX::ActionResult PX::File::OpenFromPath(const PXFileOpenFromPathInfo& const OpenFromPathInfo)
{
	return (PX::ActionResult)PXFileOpenFromPath(this, &OpenFromPathInfo);
}

PX::ActionResult PX::File::Close()
{
	return (PX::ActionResult)PXFileClose(this);
}

PX::ActionResult PX::File::MapToMemory(const PXSize size, const PXMemoryAccessMode protectionMode)
{
	return (PX::ActionResult)PXFileMapToMemory(this, size, protectionMode);
}

PX::ActionResult PX::File::UnmapFromMemory()
{
	return (PX::ActionResult)PXFileUnmapFromMemory(this);
}

PXSize PX::File::RemainingSize()
{
	return PXFileRemainingSize(this);
}

PXSize PX::File::RemainingSizeRelativeFromAddress(const void* const adress)
{
	return PXFileRemainingSizeRelativeFromAddress(this, adress);
}

bool PX::File::IsAtEnd()
{
	return PXFileIsAtEnd(this);
}

void* PX::File::CursorPosition()
{
	return PXFileCursorPosition(this);
}

void PX::File::CursorMoveTo(const PXSize position)
{
	PXFileCursorMoveTo(this, position);
}

void PX::File::CursorToBeginning()
{
	PXFileCursorToBeginning(this);
}

PXSize PX::File::CursorAdvance(const PXSize steps)
{
	return PXFileCursorAdvance(this, steps);
}

void PX::File::CursorRewind(const PXSize steps)
{
	PXFileCursorRewind(this, steps);
}

void PX::File::CursorToEnd()
{
	PXFileCursorToEnd(this);
}

PXSize PX::File::SkipEndOfLineCharacters()
{
	return PXFileSkipEndOfLineCharacters(this);
}

PXSize PX::File::SkipEmptySpace()
{
	return PXFileSkipEmptySpace(this);
}

PXSize PX::File::ReadNextLineInto(void* exportBuffer, const PXSize exportBufferSize)
{
	return PXFileReadNextLineInto(this, exportBuffer, exportBufferSize);
}

PXSize PX::File::SkipBlock()
{
	return PXFileSkipBlock(this);
}

PXSize PX::File::SkipToNextBlock()
{
	return PXFileSkipToNextBlock(this);
}

PXSize PX::File::SkipLine()
{
	return PXFileSkipLine(this);
}