#ifndef PXYAMLINCLUDE
#define PXYAMLINCLUDE

#include "../PXResource.h"

typedef enum PXYAMLLineType_
{
    PXYAMLLineTypeInvalid,
    PXYAMLLineTypeUnkown,
    PXYAMLLineTypeComment,
    PXYAMLLineTypeKeyDeclare,
    PXYAMLLineTypeKeyValueSeperator,
    PXYAMLLineTypeKeyValueDeclare,

    PXYAMLLineTypeListElement,
    PXYAMLLineTypeSeperator,
    PXYAMLLineTypeNotationStyleBlock,
    PXYAMLLineTypeNotationStyleExtendedLine
}
PXYAMLLineType;

PXPrivate PXYAMLLineType PXAPI PXYAMLPeekLine(const void* line, const PXSize size);

PXPublic PXActionResult PXAPI PXYAMLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXYAMLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif