#include "PXUltimaInfo.h"

const char PXUltimaBuildDate[] = __DATE__ " " __TIME__;
const PXI8U PXUltimaBuildDateLength = sizeof(PXUltimaBuildDate);

void PXAPI PXUltimaInfoBuildDate(PXText PXREF pxText)
{
    PXTextFromAdressA(pxText, PXUltimaBuildDate, PXUltimaBuildDateLength, PXUltimaBuildDateLength);
}
