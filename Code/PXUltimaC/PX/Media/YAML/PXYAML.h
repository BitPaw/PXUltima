#pragma once

#ifndef PXYAMLIncluded
#define PXYAMLIncluded

#include <PX/Engine/PXResource.h>

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

PXPublic PXResult PXAPI PXYAMLLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXYAMLSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
