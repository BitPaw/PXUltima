#pragma once

#include <Media/CS/PXCS.h>
#include <OS/File/PXFile.hpp>

namespace PX
{	
	class CS
	{
		public:

		static void Serialize(PX::File& inputSteam, PX::File& outputStream, PXCTranslateStruct* const pxCTranslateStruct);

		static void CreateWrapperFromCSource(PX::File& inputSteam, PX::File& outputStream);
	};
}