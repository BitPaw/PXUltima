#include "DataStream.hpp"

PX::DataStream::DataStream()
{
	PXFileConstruct(this);
}

PX::DataStream::~DataStream()
{
	PXFileDestruct(this);
}

void PX::DataStream::FromExternal(void* const data, const PXSize dataSize)
{
	return PXFileBufferExternal(this, data, dataSize);
}

ActionResult PX::DataStream::OpenFromPath(const PXTextASCII filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode)
{
	return PXFileOpenFromPathA(this, filePath, fileOpenMode, dataStreamCachingMode);
}

ActionResult PX::DataStream::OpenFromPath(const PXTextUNICODE filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode)
{
	return PXFileOpenFromPathW(this, filePath, fileOpenMode, dataStreamCachingMode);
}

/*
ActionResult PX::DataStream::OpenFromPath(const PXTextUTF8 filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode)
{
	PXFileOpenFromPathU(this, filePath, fileOpenMode, dataStreamCachingMode);
}*/

ActionResult PX::DataStream::Close()
{
	return PXFileClose(this);
}

ActionResult PX::DataStream::MapToMemory(const PXTextASCII filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode)
{
	return PXFileMapToMemoryA(this, filePath, fileSize, protectionMode);
}

ActionResult PX::DataStream::MapToMemory(const PXTextUNICODE filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode)
{
	return PXFileMapToMemoryW(this, filePath, fileSize, protectionMode);
}

/*
ActionResult PX::DataStream::MapToMemory(const PXTextUTF8 filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode)
{
	PXFileMapToMemoryU(this, filePath, fileSize, protectionMode);
}*/

ActionResult PX::DataStream::MapToMemory(const PXSize size, const PXMemoryAccessMode protectionMode)
{
	return PXFileMapToMemory(this, size, protectionMode);
}

ActionResult PX::DataStream::UnmapFromMemory()
{
	return PXFileUnmapFromMemory(this);
}

PXSize PX::DataStream::RemainingSize()
{
	return PXFileRemainingSize(this);
}

bool PX::DataStream::IsAtEnd()
{
	return PXFileIsAtEnd(this);
}

void* PX::DataStream::CursorPosition()
{
	return PXFileCursorPosition(this);
}

void PX::DataStream::CursorToBeginning()
{
	PXFileCursorToBeginning(this);
}

void PX::DataStream::CursorAdvance(const PXSize steps)
{
	PXFileCursorAdvance(this, steps);
}

void PX::DataStream::CursorRewind(const PXSize steps)
{
	PXFileCursorRewind(this, steps);
}

void PX::DataStream::CursorToEnd()
{
	PXFileCursorToEnd(this);
}

PXSize PX::DataStream::SkipEndOfLineCharacters()
{
	return PXFileSkipEndOfLineCharacters(this);
}

PXSize PX::DataStream::SkipEmptySpace()
{
	return PXFileSkipEmptySpace(this);
}

PXSize PX::DataStream::ReadNextLineInto(void* exportBuffer, const PXSize exportBufferSize)
{
	return PXFileReadNextLineInto(this, exportBuffer, exportBufferSize);
}

PXSize PX::DataStream::SkipBlock()
{
	return PXFileSkipBlock(this);
}

PXSize PX::DataStream::SkipToNextBlock()
{
	return PXFileSkipToNextBlock(this);
}

PXSize PX::DataStream::SkipLine()
{
	return PXFileSkipLine(this);
}

PXSize PX::DataStream::ReadTextIU8(PXInt8U* const number)
{
	return PXFileReadTextIU8(this, number);
}

PXSize PX::DataStream::ReadTextI(int* const number)
{
	return PXSize();
}

PXSize PX::DataStream::Read(char* const value)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned char* const value)
{
	return PXSize();
}

PXSize PX::DataStream::Read(short* const value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned short* const value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(int* const value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned int* const value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(long long* const value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned long long* const value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(float* const value)
{
	return PXSize();
}

PXSize PX::DataStream::Read(double* const value)
{
	return PXSize();
}

PXSize PX::DataStream::Read(void* const value, const PXSize length)
{
	return PXSize();
}

PXSize PX::DataStream::Read(const void* const format, const PXSize length, ...)
{
	return PXSize();
}

PXSize PX::DataStream::Read(char* const value, const PXSize length)
{
	return PXSize();
}

PXSize PX::DataStream::Read(wchar_t* const value, const PXSize length)
{
	return PXSize();
}

void PX::DataStream::ReadUntil(void* value, const PXSize length, const char character)
{
}

unsigned char PX::DataStream::ReadAndCompare(const void* value, const PXSize length)
{
	return 0;
}

PXSize PX::DataStream::Write(const char value)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned char value)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const short value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned short value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const int value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned int value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const long long value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned long long value, const PXEndian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const float value)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const double value)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const char* const text, PXSize textSize)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const wchar_t* const text, PXSize textSize)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const void* value, const PXSize length)
{
	return PXSize();
}

PXSize PX::DataStream::WriteFill(const unsigned char value, const PXSize length)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const char* format, ...)
{
	return PXSize();
}

PXSize PX::DataStream::WriteAt(const void* const data, const PXSize dataSize, const PXSize index)
{
	return PXSize();
}

PXSize PX::DataStream::WriteAt(const unsigned char value, const PXSize index)
{
	return PXSize();
}

PXSize PX::DataStream::WriteAt(const unsigned short value, const PXEndian endian, const PXSize index)
{
	return PXSize();
}

PXSize PX::DataStream::WriteAt(const unsigned int value, const PXEndian endian, const PXSize index)
{
	return PXSize();
}