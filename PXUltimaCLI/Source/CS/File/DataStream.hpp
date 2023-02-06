#pragma once

#include <File/PXDataStream.h>
#include <CS/Error/ActionResult.hpp>

namespace PX
{
	public ref class DataStream
	{
		private:
		PXDataStream* _dataStream;

		public:
		DataStream()// : _dataSteam
		{
			_dataStream = new PXDataStream();
			PXDataStreamConstruct(_dataStream);
		}

		~DataStream()
		{
			PXDataStreamDestruct(_dataStream);
			delete _dataStream;
		}

		property PXSize CursorPositionCurrent
		{
			PXSize get()
			{
				return _dataStream->DataCursor;
			}
		}

		property PXSize CursorPositionMaximum
		{
			PXSize get()
			{
				return _dataStream->DataSize;
			}
		}

		PX::ActionResult OpenFromPathReadOnly(System::String^ filePath);
		PX::ActionResult OpenFromPath(System::String^ filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode);
		PX::ActionResult Close();

		PX::ActionResult MapToMemoryReadOnly(System::String^ filePath);
		PX::ActionResult MapToMemory(System::String^ filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
		PX::ActionResult MapToMemory(const PXSize size, const MemoryProtectionMode protectionMode);
		PX::ActionResult UnmapFromMemory();
	};
}