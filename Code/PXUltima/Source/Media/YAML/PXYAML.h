#ifndef PXYAMLINCLUDE
#define PXYAMLINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/File/PXFile.h>
#include <Serialization/Serialization.h>

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

	PXPublic PXActionResult PXYAMLFileCompile(PXFile* const inputStream, PXFile* const outputStream);

	PXPublic PXActionResult PXYAMLFileParse(PXFile* const ymlTokenInputStream, SerializationTypeInfo* const serializationTypeInfoList, const PXSize serializationTypeInfoListSize);

#ifdef __cplusplus
}
#endif

#endif
