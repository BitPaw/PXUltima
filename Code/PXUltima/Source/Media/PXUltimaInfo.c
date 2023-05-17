#include "PXUltimaInfo.h"

void PXUltimaInfoBuildDate(PXText* const text)
{
	const char buildDate[] = __DATE__ " " __TIME__;

	text->Format = TextFormatASCII;
	text->SizeAllocated = sizeof(buildDate);
	text->SizeUsed = text->SizeAllocated;
	text->NumberOfCharacters = text->SizeAllocated;
	text->TextA = buildDate;
}