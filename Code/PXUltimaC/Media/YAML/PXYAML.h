#ifndef PXYAMLINCLUDE
#define PXYAMLINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/File/PXFile.h>

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

	PXPrivate PXYAMLLineType PXYAMLPeekLine(const void* line, const PXSize size);

	PXPublic PXActionResult PXYAMLLoadFromFile(PXFile* const inputStream, PXFile* const pxFile);
	PXPublic PXActionResult PXYAMLSaveToFile(PXFile* const inputStream, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
