#pragma once

#include <OS/File/PXFile.h>
#include <CS/OS/Error/ActionResult.hpp>

namespace PX
{
	public ref class DataStream
	{
		private:
		PXFile* _dataStream;

		public:
		DataStream();
		~DataStream();

		property PXSize CursorPositionCurrent { PXSize get() { return _dataStream->DataCursor; }}
		property PXSize CursorPositionMaximum { PXSize get() { return _dataStream->DataSize; }}

		PX::ActionResult OpenFromPathReadOnly(System::String^ filePath);
		PX::ActionResult OpenFromPath(System::String^ filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode);
		PX::ActionResult Close();

		PX::ActionResult MapToMemoryReadOnly(System::String^ filePath);
		PX::ActionResult MapToMemory(System::String^ filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
		PX::ActionResult MapToMemory(const PXSize size, const PXMemoryAccessMode protectionMode);
		PX::ActionResult UnmapFromMemory();
	};
}