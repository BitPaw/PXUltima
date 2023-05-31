#include "PXProcessor.hpp"

void PX::Processor::FetchInfo()
{
	PXProcessorFetchInfo(this);
}

PXInt32U PX::Processor::FrequencyCurrent()
{
	return PXProcessorFrequencyCurrent();
}

PXInt32U PX::Processor::TimeReal()
{
	return PXProcessorTimeReal();
}

PXInt32U PX::Processor::Temperature()
{
	return PXProcessorTemperature();
}