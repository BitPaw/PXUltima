#ifndef PXSQLINCLUDE
#define PXSQLINCLUDE

#include "../PXResource.h"

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

PXPublic PXActionResult PXAPI PXSQLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSQLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif