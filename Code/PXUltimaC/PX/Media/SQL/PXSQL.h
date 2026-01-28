#pragma once

#ifndef PXSQLIncluded
#define PXSQLIncluded

#include <PX/Engine/PXResource.h>

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

PXPublic PXResult PXAPI PXSQLLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSQLSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
