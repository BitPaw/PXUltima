#include "DataStream.hpp"

#include <vcclr.h>

PX::DataStream::DataStream()
{
	_dataStream = new PXDataStream();
	PXDataStreamConstruct(_dataStream);
}

PX::DataStream::~DataStream()
{
	PXDataStreamDestruct(_dataStream);
	delete _dataStream;
}

PX::ActionResult  PX::DataStream::OpenFromPathReadOnly(System::String^ filePath)
{
	return OpenFromPath(filePath, MemoryProtectionMode::MemoryReadAndWrite, PXDataStreamCachingMode::FileCachingSequential);
}

PX::ActionResult PX::DataStream::OpenFromPath(System::String^ filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	pin_ptr<const wchar_t> filePathW = PtrToStringChars(filePath);
	wchar_t* data = (wchar_t*)filePathW;

	const ActionResult actionResult = (ActionResult)PXDataStreamOpenFromPathW(this->_dataStream, data, fileOpenMode, dataStreamCachingMode);

	printf("Path:%ls, %i Bytes\n", data, _dataStream->DataSize);

	for (size_t i = 0; i < _dataStream->DataSize; i++)
	{
		printf("%c", ((unsigned char*)_dataStream->Data)[i]);
	}

	return actionResult;
}

PX::ActionResult PX::DataStream::Close()
{
	return ActionResult::Successful;
}

PX::ActionResult PX::DataStream::MapToMemoryReadOnly(System::String^ filePath)
{
	return MapToMemory(filePath, 0, MemoryProtectionMode::MemoryReadOnly);
}

PX::ActionResult PX::DataStream::MapToMemory(System::String^ filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	pin_ptr<const wchar_t> filePathW = PtrToStringChars(filePath);
	wchar_t* data = (wchar_t*)filePathW;

	const ActionResult actionResult = (ActionResult)PXDataStreamMapToMemoryW(this->_dataStream, data, fileSize, protectionMode);

	printf("Path:%ls, %i Bytes\n", data, _dataStream->DataSize);

	for (size_t i = 0; i < _dataStream->DataSize; i++)
	{
		printf("%c", ((unsigned char*)_dataStream->Data)[i]);
	}

	return actionResult;
}

PX::ActionResult PX::DataStream::MapToMemory(const PXSize size, const MemoryProtectionMode protectionMode)
{
	return ActionResult::Successful;
}

PX::ActionResult PX::DataStream::UnmapFromMemory()
{
	return ActionResult::Successful;
}