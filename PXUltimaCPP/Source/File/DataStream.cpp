#include "DataStream.hpp"

PX::DataStream::DataStream()
{
	PXDataStreamConstruct(this);
}

PX::DataStream::~DataStream()
{
	PXDataStreamDestruct(this);
}

void PX::DataStream::FromExternal(void* const data, const PXSize dataSize)
{
	return PXDataStreamFromExternal(this, data, dataSize);
}

ActionResult PX::DataStream::OpenFromPath(const PXTextASCII filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	return PXDataStreamOpenFromPathA(this, filePath, fileOpenMode, dataStreamCachingMode);
}

ActionResult PX::DataStream::OpenFromPath(const PXTextUNICODE filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	return PXDataStreamOpenFromPathW(this, filePath, fileOpenMode, dataStreamCachingMode);
}

/*
ActionResult PX::DataStream::OpenFromPath(const PXTextUTF8 filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	PXDataStreamOpenFromPathU(this, filePath, fileOpenMode, dataStreamCachingMode);
}*/

ActionResult PX::DataStream::Close()
{
	return PXDataStreamClose(this);
}

ActionResult PX::DataStream::MapToMemory(const PXTextASCII filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	return PXDataStreamMapToMemoryA(this, filePath, fileSize, protectionMode);
}

ActionResult PX::DataStream::MapToMemory(const PXTextUNICODE filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	return PXDataStreamMapToMemoryW(this, filePath, fileSize, protectionMode);
}

/*
ActionResult PX::DataStream::MapToMemory(const PXTextUTF8 filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXDataStreamMapToMemoryU(this, filePath, fileSize, protectionMode);
}*/

ActionResult PX::DataStream::MapToMemory(const PXSize size, const MemoryProtectionMode protectionMode)
{
	return PXDataStreamMapToMemory(this, size, protectionMode);
}

ActionResult PX::DataStream::UnmapFromMemory()
{
	return PXDataStreamUnmapFromMemory(this);
}

PXSize PX::DataStream::RemainingSize()
{
	return PXDataStreamRemainingSize(this);
}

bool PX::DataStream::IsAtEnd()
{
	return PXDataStreamIsAtEnd(this);
}

void* PX::DataStream::CursorPosition()
{
	return PXDataStreamCursorPosition(this);
}

void PX::DataStream::CursorToBeginning()
{
	PXDataStreamCursorToBeginning(this);
}

void PX::DataStream::CursorAdvance(const PXSize steps)
{
	PXDataStreamCursorAdvance(this, steps);
}

void PX::DataStream::CursorRewind(const PXSize steps)
{
	PXDataStreamCursorRewind(this, steps);
}

void PX::DataStream::CursorToEnd()
{
	PXDataStreamCursorToEnd(this);
}

PXSize PX::DataStream::SkipEndOfLineCharacters()
{
	return PXDataStreamSkipEndOfLineCharacters(this);
}

PXSize PX::DataStream::SkipEmptySpace()
{
	return PXDataStreamSkipEmptySpace(this);
}

PXSize PX::DataStream::ReadNextLineInto(void* exportBuffer, const PXSize exportBufferSize)
{
	return PXDataStreamReadNextLineInto(this, exportBuffer, exportBufferSize);
}

PXSize PX::DataStream::SkipBlock()
{
	return PXDataStreamSkipBlock(this);
}

PXSize PX::DataStream::SkipToNextBlock()
{
	return PXDataStreamSkipToNextBlock(this);
}

PXSize PX::DataStream::SkipLine()
{
	return PXDataStreamSkipLine(this);
}

PXSize PX::DataStream::ReadTextIU8(PXInt8U* const number)
{
	return PXDataStreamReadTextIU8(this, number);
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

PXSize PX::DataStream::Read(short* const value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned short* const value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(int* const value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned int* const value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(long long* const value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Read(unsigned long long* const value, const Endian endian)
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

PXSize PX::DataStream::Write(const short value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned short value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const int value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned int value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const long long value, const Endian endian)
{
	return PXSize();
}

PXSize PX::DataStream::Write(const unsigned long long value, const Endian endian)
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

PXSize PX::DataStream::WriteAt(const unsigned short value, const Endian endian, const PXSize index)
{
	return PXSize();
}

PXSize PX::DataStream::WriteAt(const unsigned int value, const Endian endian, const PXSize index)
{
	return PXSize();
}