#include "PXUltimaInfo.h"

void PXAPI PXUltimaInfoBuildDate(PXText* const pxText)
{
    PXText buildDateText;
    PXTextMakeFixedA(&buildDateText, __DATE__ " " __TIME__);

    PXTextCopy(&buildDateText, pxText);
}