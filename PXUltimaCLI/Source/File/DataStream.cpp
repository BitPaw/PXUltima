#include "DataStream.hpp"

#include <vcclr.h>

int PX::DataStream::OpenFromPathReadOnly(System::String^ filePath)
{
	return OpenFromPath(filePath, MemoryProtectionMode::MemoryReadAndWrite, PXDataStreamCachingMode::FileCachingSequential);
}

ActionResult PX::DataStream::OpenFromPath(System::String^ filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	pin_ptr<const wchar_t> filePathW = PtrToStringChars(filePath);
	wchar_t* data = (wchar_t*)filePathW;

	const ActionResult actionResult = PXDataStreamOpenFromPathW(this->_dataStream, data, fileOpenMode, dataStreamCachingMode);

	printf("Path:%ls, %i Bytes\n", data, _dataStream->DataSize);

	for (size_t i = 0; i < _dataStream->DataSize; i++)
	{
		printf("%c", ((unsigned char*)_dataStream->Data)[i]);
	}

	return actionResult;
}

ActionResult PX::DataStream::Close()
{
	return ActionSuccessful;
}

int PX::DataStream::MapToMemoryReadOnly(System::String^ filePath)
{
	return MapToMemory(filePath, 0, MemoryProtectionMode::MemoryReadOnly);
}

ActionResult PX::DataStream::MapToMemory(System::String^ filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	pin_ptr<const wchar_t> filePathW = PtrToStringChars(filePath);
	wchar_t* data = (wchar_t*)filePathW;

	const ActionResult actionResult = PXDataStreamMapToMemoryW(this->_dataStream, data, fileSize, protectionMode);

	printf("Path:%ls, %i Bytes\n", data, _dataStream->DataSize);

	for (size_t i = 0; i < _dataStream->DataSize; i++)
	{
		printf("%c", ((unsigned char*)_dataStream->Data)[i]);
	}

	return actionResult;
}

ActionResult PX::DataStream::MapToMemory(const PXSize size, const MemoryProtectionMode protectionMode)
{
	return ActionSuccessful;
}

ActionResult PX::DataStream::UnmapFromMemory()
{
	return ActionSuccessful;
}