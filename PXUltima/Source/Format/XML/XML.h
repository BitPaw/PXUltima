#ifndef XMLINCLUDE
#define XMLINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File/DataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum XMLSymbol_
	{
		XMLSymbolInvalid,

		XMLSymbolUnkown,

		XMLSymbolTagOpenBegin, // <xxxx...
		XMLSymbolTagOpenFull, // <xxxxx>...

		XMLSymbolTagCloseCurrent, // .../>
		XMLSymbolTagCloseFull, // </>

		XMLSymbolAttribute,
	}
	XMLSymbol;

	PXPublic XMLSymbol XMLPeekLine(const char* const text, const size_t textSize);

	PXPublic ActionResult XMLFileCompile(DataStream* const inputStream, DataStream* const outputStream);

#ifdef __cplusplus
}
#endif

#endif