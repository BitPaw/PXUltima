#include "PXDataStream.hpp"

BF::DataStream::DataStream()
{
	PXDataStreamConstruct(this);
}

BF::DataStream::~DataStream()
{
	PXDataStreamDestruct(this);
}

void BF::DataStream::FromExternal(void* const data, const PXSize dataSize)
{
	PXDataStreamFromExternal(this, data, dataSize);
}

ActionResult BF::DataStream::OpenFromPath(const PXTextASCII filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	PXDataStreamOpenFromPathA(this, filePath, fileOpenMode, dataStreamCachingMode);
}

ActionResult BF::DataStream::OpenFromPath(const PXTextUNICODE filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	PXDataStreamOpenFromPathW(this, filePath, fileOpenMode, dataStreamCachingMode);
}

ActionResult BF::DataStream::OpenFromPath(const PXTextUTF8 filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	PXDataStreamOpenFromPathU(this, filePath, fileOpenMode, dataStreamCachingMode);
}

ActionResult BF::DataStream::Close()
{
	PXDataStreamClose(this);
}

ActionResult BF::DataStream::MapToMemory(const PXTextASCII filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXDataStreamMapToMemoryA(this, filePath, fileSize, protectionMode);
}

ActionResult BF::DataStream::MapToMemory(const PXTextUNICODE filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXDataStreamMapToMemoryW(this, filePath, fileSize, protectionMode);
}

ActionResult BF::DataStream::MapToMemory(const PXTextUTF8 filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXDataStreamMapToMemoryU(this, filePath, fileSize, protectionMode);
}

ActionResult BF::DataStream::MapToMemory(const PXSize size, const MemoryProtectionMode protectionMode)
{
	PXDataStreamMapToMemory(this, size, protectionMode);
}

ActionResult BF::DataStream::UnmapFromMemory()
{
	PXDataStreamUnmapFromMemory(this);
}

PXSize BF::DataStream::RemainingSize()
{
	return PXDataStreamRemainingSize(this);
}

bool BF::DataStream::IsAtEnd()
{
	return PXDataStreamIsAtEnd(this);
}

void* BF::DataStream::CursorPosition()
{
	return PXDataStreamCursorPosition(this);
}

void BF::DataStream::CursorToBeginning()
{
	PXDataStreamCursorToBeginning(this);
}

void BF::DataStream::CursorAdvance(const PXSize steps)
{
	PXDataStreamCursorAdvance(this, steps);
}

void BF::DataStream::CursorRewind(const PXSize steps)
{
	PXDataStreamCursorRewind(this, steps);
}

void BF::DataStream::CursorToEnd()
{
	PXDataStreamCursorToEnd(this);
}

PXSize BF::DataStream::SkipEndOfLineCharacters()
{
	return PXDataStreamSkipEndOfLineCharacters(this);
}

PXSize BF::DataStream::SkipEmptySpace()
{
	return PXDataStreamSkipEmptySpace(this);
}

PXSize BF::DataStream::ReadNextLineInto(void* exportBuffer, const PXSize exportBufferSize)
{
	return PXDataStreamReadNextLineInto(this, exportBuffer, exportBufferSize);
}

PXSize BF::DataStream::SkipBlock()
{
	return PXDataStreamSkipBlock(this);
}

PXSize BF::DataStream::SkipToNextBlock()
{
	return PXDataStreamSkipToNextBlock(this);
}

PXSize BF::DataStream::SkipLine()
{
	return PXDataStreamSkipLine(this);
}

PXSize BF::DataStream::ReadTextIU8(PXInt8U* const number)
{
	return PXDataStreamReadTextIU8();
}

PXSize BF::DataStream::ReadTextI(int* const number)
{
	return PXSize();
}

PXSize BF::DataStream::Read(char* const value)
{
	return PXSize();
}

PXSize BF::DataStream::Read(unsigned char* const value)
{
	return PXSize();
}

PXSize BF::DataStream::Read(short* const value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Read(unsigned short* const value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Read(int* const value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Read(unsigned int* const value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Read(long long* const value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Read(unsigned long long* const value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Read(float* const value)
{
	return PXSize();
}

PXSize BF::DataStream::Read(double* const value)
{
	return PXSize();
}

PXSize BF::DataStream::Read(void* const value, const PXSize length)
{
	return PXSize();
}

PXSize BF::DataStream::Read(const void* const format, const PXSize length, ...)
{
	return PXSize();
}

PXSize BF::DataStream::Read(char* const value, const PXSize length)
{
	return PXSize();
}

PXSize BF::DataStream::Read(wchar_t* const value, const PXSize length)
{
	return PXSize();
}

void BF::DataStream::ReadUntil(void* value, const PXSize length, const char character)
{
}

unsigned char BF::DataStream::ReadAndCompare(const void* value, const PXSize length)
{
	return 0;
}

PXSize BF::DataStream::Write(const char value)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const unsigned char value)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const short value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const unsigned short value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const int value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const unsigned int value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const long long value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const unsigned long long value, const Endian endian)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const float value)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const double value)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const char* const text, PXSize textSize)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const wchar_t* const text, PXSize textSize)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const void* value, const PXSize length)
{
	return PXSize();
}

PXSize BF::DataStream::WriteFill(const unsigned char value, const PXSize length)
{
	return PXSize();
}

PXSize BF::DataStream::Write(const char* format, ...)
{
	return PXSize();
}

PXSize BF::DataStream::WriteAt(const void* const data, const PXSize dataSize, const PXSize index)
{
	return PXSize();
}

PXSize BF::DataStream::WriteAt(const unsigned char value, const PXSize index)
{
	return PXSize();
}

PXSize BF::DataStream::WriteAt(const unsigned short value, const Endian endian, const PXSize index)
{
	return PXSize();
}

PXSize BF::DataStream::WriteAt(const unsigned int value, const Endian endian, const PXSize index)
{
	return PXSize();
}