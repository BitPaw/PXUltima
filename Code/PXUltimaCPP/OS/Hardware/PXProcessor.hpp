#pragma once

#include <OS/Hardware/PXProcessor.h>

namespace PX
{
	typedef struct Processor : public PXProcessor
	{
        public:
        void FetchInfo();

        static PXInt32U FrequencyCurrent();
        static PXInt32U TimeReal();

        static PXInt32U Temperature();
	};
}