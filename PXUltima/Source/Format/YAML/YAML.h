#ifndef YAMLINCLUDE
#define YAMLINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File\DataStream.h>

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
		YAMLLineTypeListElement,
		YAMLLineTypeSeperator,
		YAMLLineTypeNotationStyleBlock,
		YAMLLineTypeNotationStyleExtendedLine
	}
	YAMLLineType;

	PXPrivate YAMLLineType YAMLPeekLine(const void* line, const size_t size);

	PXPublic ActionResult YAMLFileCompile(DataStream* const inputStream, DataStream* const outputStream);

#ifdef __cplusplus
}
#endif

#endif