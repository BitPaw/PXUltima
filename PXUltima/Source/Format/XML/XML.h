#ifndef XMLINCLUDE
#define XMLINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File/PXDataStream.h>

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

		XMLSymbolRawData
	}
	XMLSymbol;

	PXPublic XMLSymbol XMLPeekLine(const char* const text, const PXSize textSize);

	PXPublic ActionResult XMLFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream);

#ifdef __cplusplus
}
#endif

#endif