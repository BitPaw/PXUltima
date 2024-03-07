#ifndef PXYAMLINCLUDE
#define PXYAMLINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

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

	PXPublic PXActionResult PXAPI PXYAMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXYAMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
