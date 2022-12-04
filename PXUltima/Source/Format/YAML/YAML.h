#ifndef YAMLINCLUDE
#define YAMLINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File/DataStream.h>
#include <Serialization/Serialization.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum YAMLLineType_
	{
		YAMLLineTypeInvalid,
		YAMLLineTypeUnkown,
		YAMLLineTypeComment,
		YAMLLineTypeKeyDeclare,
		YAMLLineTypeKeyValueSeperator,
		YAMLLineTypeKeyValueDeclare,

		YAMLLineTypeListElement,
		YAMLLineTypeSeperator,
		YAMLLineTypeNotationStyleBlock,
		YAMLLineTypeNotationStyleExtendedLine
	}
	YAMLLineType;

	PXPrivate YAMLLineType YAMLPeekLine(const void* line, const PXSize size);

	PXPublic ActionResult YAMLFileCompile(DataStream* const inputStream, DataStream* const outputStream);

	PXPublic ActionResult YAMLFileParse(DataStream* const ymlTokenInputStream, SerializationTypeInfo* const serializationTypeInfoList, const PXSize serializationTypeInfoListSize);

#ifdef __cplusplus
}
#endif

#endif
