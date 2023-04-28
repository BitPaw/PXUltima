#ifndef XMLINCLUDE
#define XMLINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/File/PXFile.h>

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

	PXPublic PXActionResult XMLFileCompile(PXFile* const inputStream, PXFile* const outputStream);

#ifdef __cplusplus
}
#endif

#endif