#include "PXUltimaInfo.h"

const char* const PXUltimaInfoBuildDate()
{
	const char buildDate[] = __DATE__ " " __TIME__;

	return buildDate;
}