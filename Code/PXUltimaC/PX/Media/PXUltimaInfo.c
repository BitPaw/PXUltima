#include "PXUltimaInfo.h"

void PXAPI PXUltimaInfoBuildDate(PXText PXREF pxText)
{
    PXText buildDateText;
    PXTextMakeFixedA(&buildDateText, __DATE__ " " __TIME__);

    PXTextCopy(&buildDateText, pxText);
}
