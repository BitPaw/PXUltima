#pragma once

#include <File/PXDataStream.h>

namespace PX
{
	public ref class DataStream
	{
		private:
		PXDataStream* _dataStream;

		public:
		delegate void PXDataStreamEventHandler(Object^ sender, unsigned int e);
		event PXDataStreamEventHandler^ E;

		DataStream()// : _dataSteam
		{
			_dataStream = new PXDataStream();
			PXDataStreamConstruct(_dataStream);
		}

		~DataStream()
		{
			PXDataStreamDestruct(_dataStream);
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

		int OpenFromPathReadOnly(System::String^ filePath);
		ActionResult OpenFromPath(System::String^ filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode);
		ActionResult Close();

		int MapToMemoryReadOnly(System::String^ filePath);
		ActionResult MapToMemory(System::String^ filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
		ActionResult MapToMemory(const PXSize size, const MemoryProtectionMode protectionMode);
		ActionResult UnmapFromMemory();
	};
}