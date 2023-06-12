#include "PXCS.hpp"

void PX::CS::Serialize(PX::File& inputSteam, PX::File& outputStream, PXCTranslateStruct* const pxCTranslateStruct)
{
	PXCSSerialize(&inputSteam, &outputStream, pxCTranslateStruct);
}

void PX::CS::CreateWrapperFromCSource(PX::File& inputSteam, PX::File& outputStream)
{
	PXCSCreateWrapperFromCSource(&inputSteam, &outputStream);
}