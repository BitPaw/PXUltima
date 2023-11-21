#ifndef PXSQLINCLUDE
#define PXSQLINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXSQLKeyWord_
	{
		PXSQLKeyWordInvalid,
		PXSQLKeyWordSelect,
		PXSQLKeyWordFrom,
		PXSQLKeyWordOrder,
		PXSQLKeyWordJoin,
		PXSQLKeyWordOuter,
		PXSQLKeyWordFull,
		PXSQLKeyWordCross,
		PXSQLKeyWordNatural,
		PXSQLKeyWordWhere,
		PXSQLKeyWordBetween,
		PXSQLKeyWordExists,
		PXSQLKeyWordLike,
		PXSQLKeyWordUnion,
		PXSQLKeyWordIntersect,
		PXSQLKeyWordExcept,
		PXSQLKeyWordAnd,
		PXSQLKeyWordAll,
		PXSQLKeyWordAny,
		PXSQLKeyWordOr,
		PXSQLKeyWordRight,
		PXSQLKeyWordOn,
		PXSQLKeyWordIn,
		PXSQLKeyWordAs,
		PXSQLKeyWordIs,
		PXSQLKeyWordNot,
		PXSQLKeyWordNULL,
		PXSQLKeyWordFunctionAverage,
		PXSQLKeyWordFunctionMinimum,
		PXSQLKeyWordFunctionMaximum,
		PXSQLKeyWordFunctionSum,
		PXSQLKeyWordFunctionCount,
	}
	PXSQLKeyWord;

	//PXPublic PXActionResult PXAPI PXSQLLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	//PXPublic PXActionResult PXAPI PXSQLSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
