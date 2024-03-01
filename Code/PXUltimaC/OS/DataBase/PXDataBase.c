#include "PXDataBase.h"

#if OSUnix
#elif OSWindows
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#endif

#include <OS/Memory/PXMemory.h>
#include <OS/Console/PXConsole.h>

typedef SQLRETURN(SQL_API* PXSQLAllocConnect)(SQLHENV EnvironmentHandle, __out SQLHDBC* ConnectionHandle);
typedef SQLRETURN(SQL_API* PXSQLAllocEnv)(__out SQLHENV* EnvironmentHandle);
typedef SQLRETURN(SQL_API* PXSQLAllocHandle)(SQLSMALLINT HandleType, SQLHANDLE InputHandle, __out SQLHANDLE* OutputHandle);
typedef SQLRETURN(SQL_API* PXSQLAllocStmt)(SQLHDBC ConnectionHandle, __out SQLHSTMT* StatementHandle);
typedef SQLRETURN(SQL_API* PXSQLBindCol)(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValue, SQLLEN BufferLength, __inout_opt SQLLEN* StrLen_or_Ind);
typedef SQLRETURN(SQL_API* PXSQLBindParam)(SQLHSTMT StatementHandle, SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType, SQLSMALLINT ParameterType, SQLULEN LengthPrecision, SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue, SQLLEN* StrLen_or_Ind);
typedef SQLRETURN(SQL_API* PXSQLCancel)(SQLHSTMT StatementHandle);
typedef SQLRETURN(SQL_API* PXSQLCancelHandle)(SQLSMALLINT HandleType, SQLHANDLE InputHandle);
typedef SQLRETURN(SQL_API* PXSQLCloseCursor)(SQLHSTMT StatementHandle);
typedef SQLRETURN(SQL_API* PXSQLColAttribute)(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttribute, SQLSMALLINT BufferLength, SQLSMALLINT* StringLength, SQLLEN* NumericAttribute); // SQLPOINTER NumericAttribute);
typedef SQLRETURN(SQL_API* PXSQLColumns)(SQLHSTMT StatementHandle, SQLCHAR* CatalogName, SQLSMALLINT NameLength1, SQLCHAR* SchemaName, SQLSMALLINT NameLength2, SQLCHAR* TableName, SQLSMALLINT NameLength3, SQLCHAR* ColumnName, SQLSMALLINT NameLength4);
typedef SQLRETURN(SQL_API* PXSQLConnect)(SQLHDBC ConnectionHandle, SQLCHAR* ServerName, SQLSMALLINT NameLength1, SQLCHAR* UserName, SQLSMALLINT NameLength2, SQLCHAR* Authentication, SQLSMALLINT NameLength3);
typedef SQLRETURN(SQL_API* PXSQLCopyDesc)(SQLHDESC SourceDescHandle, SQLHDESC TargetDescHandle);
typedef SQLRETURN(SQL_API* PXSQLDataSources)(SQLHENV EnvironmentHandle, SQLUSMALLINT Direction, SQLCHAR* ServerName, SQLSMALLINT BufferLength1, SQLSMALLINT* NameLength1Ptr, SQLCHAR* Description, SQLSMALLINT BufferLength2, SQLSMALLINT* NameLength2Ptr);
typedef SQLRETURN(SQL_API* PXSQLDescribeCol)(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLCHAR* ColumnName, SQLSMALLINT BufferLength, SQLSMALLINT* NameLength, SQLSMALLINT* DataType, SQLULEN* ColumnSize, SQLSMALLINT* DecimalDigits, SQLSMALLINT* Nullable);
typedef SQLRETURN(SQL_API* PXSQLDisconnect)(SQLHDBC ConnectionHandle);
typedef SQLRETURN(SQL_API* PXSQLEndTran)(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType);
typedef SQLRETURN(SQL_API* PXSQLError)(SQLHENV EnvironmentHandle, SQLHDBC ConnectionHandle, SQLHSTMT StatementHandle, SQLCHAR* Sqlstate, SQLINTEGER* NativeError, SQLCHAR* MessageText, SQLSMALLINT BufferLength, SQLSMALLINT* TextLength);
typedef SQLRETURN(SQL_API* PXSQLExecDirect)(SQLHSTMT StatementHandle, SQLCHAR* StatementText, SQLINTEGER TextLength);
typedef SQLRETURN(SQL_API* PXSQLExecute)(SQLHSTMT StatementHandle);
typedef SQLRETURN(SQL_API* PXSQLFetch)(SQLHSTMT StatementHandle);
typedef SQLRETURN(SQL_API* PXSQLFetchScroll)(SQLHSTMT StatementHandle, SQLSMALLINT FetchOrientation, SQLLEN FetchOffset);
typedef SQLRETURN(SQL_API* PXSQLFreeConnect)(SQLHDBC ConnectionHandle);
typedef SQLRETURN(SQL_API* PXSQLFreeEnv)(SQLHENV EnvironmentHandle);
typedef SQLRETURN(SQL_API* PXSQLFreeHandle)(SQLSMALLINT HandleType, SQLHANDLE Handle);
typedef SQLRETURN(SQL_API* PXSQLFreeStmt)(SQLHSTMT StatementHandle, SQLUSMALLINT Option);
typedef SQLRETURN(SQL_API* PXSQLGetConnectAttr)(SQLHDBC ConnectionHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER BufferLength, SQLINTEGER* StringLengthPtr);
typedef SQLRETURN(SQL_API* PXSQLGetConnectOption)(SQLHDBC ConnectionHandle, SQLUSMALLINT Option, SQLPOINTER Value);
typedef SQLRETURN(SQL_API* PXSQLGetCursorName)(SQLHSTMT StatementHandle, SQLCHAR* CursorName, SQLSMALLINT BufferLength, SQLSMALLINT* NameLengthPtr);
typedef SQLRETURN(SQL_API* PXSQLGetData)(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValue, SQLLEN BufferLength, SQLLEN* StrLen_or_IndPtr);
typedef SQLRETURN(SQL_API* PXSQLGetDescField)(SQLHDESC DescriptorHandle, SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier, SQLPOINTER Value, SQLINTEGER BufferLength, SQLINTEGER* StringLength);
typedef SQLRETURN(SQL_API* PXSQLGetDescRec)(SQLHDESC DescriptorHandle, SQLSMALLINT RecNumber, SQLCHAR* Name, SQLSMALLINT BufferLength, SQLSMALLINT* StringLengthPtr, SQLSMALLINT* TypePtr, SQLSMALLINT* SubTypePtr, SQLLEN* LengthPtr, SQLSMALLINT* PrecisionPtr, SQLSMALLINT* ScalePtr, SQLSMALLINT* NullablePtr);
typedef SQLRETURN(SQL_API* PXSQLGetDiagField)(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier, SQLPOINTER DiagInfo, SQLSMALLINT BufferLength, SQLSMALLINT* StringLength);
typedef SQLRETURN(SQL_API* PXSQLGetDiagRec)(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLCHAR* Sqlstate, SQLINTEGER* NativeError, SQLCHAR* MessageText, SQLSMALLINT BufferLength, SQLSMALLINT* TextLength);
typedef SQLRETURN(SQL_API* PXSQLGetEnvAttr)(SQLHENV EnvironmentHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER BufferLength, SQLINTEGER* StringLength);
typedef SQLRETURN(SQL_API* PXSQLGetFunctions)(SQLHDBC ConnectionHandle, SQLUSMALLINT FunctionId, SQLUSMALLINT* Supported);
typedef SQLRETURN(SQL_API* PXSQLGetInfo)(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType, SQLPOINTER InfoValue, SQLSMALLINT BufferLength, SQLSMALLINT* StringLengthPtr);
typedef SQLRETURN(SQL_API* PXSQLGetStmtAttr)(SQLHSTMT StatementHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER BufferLength, SQLINTEGER* StringLength);
typedef SQLRETURN(SQL_API* PXSQLGetStmtOption)(SQLHSTMT StatementHandle, SQLUSMALLINT Option, SQLPOINTER Value);
typedef SQLRETURN(SQL_API* PXSQLGetTypeInfo)(SQLHSTMT StatementHandle, SQLSMALLINT DataType);
typedef SQLRETURN(SQL_API* PXSQLNumResultCols)(SQLHSTMT StatementHandle, __out SQLSMALLINT* ColumnCount);
typedef SQLRETURN(SQL_API* PXSQLParamData)(SQLHSTMT StatementHandle, SQLPOINTER* Value);
typedef SQLRETURN(SQL_API* PXSQLPrepare)(SQLHSTMT StatementHandle, SQLCHAR* StatementText, SQLINTEGER TextLength);
typedef SQLRETURN(SQL_API* PXSQLPutData)(SQLHSTMT StatementHandle, SQLPOINTER Data, SQLLEN StrLen_or_Ind);
typedef SQLRETURN(SQL_API* PXSQLRowCount)(__in SQLHSTMT StatementHandle, __out SQLLEN* RowCount);
typedef SQLRETURN(SQL_API* PXSQLSetConnectAttr)(SQLHDBC ConnectionHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER StringLength);
typedef SQLRETURN(SQL_API* PXSQLSetConnectOption)(SQLHDBC ConnectionHandle, SQLUSMALLINT Option, SQLULEN Value);
typedef SQLRETURN(SQL_API* PXSQLSetCursorName)(SQLHSTMT StatementHandle, SQLCHAR* CursorName, SQLSMALLINT NameLength);
typedef SQLRETURN(SQL_API* PXSQLSetDescField)(SQLHDESC DescriptorHandle, SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier, SQLPOINTER Value, SQLINTEGER BufferLength);
typedef SQLRETURN(SQL_API* PXSQLSetDescRec)(SQLHDESC DescriptorHandle, SQLSMALLINT RecNumber, SQLSMALLINT Type, SQLSMALLINT SubType, SQLLEN Length, SQLSMALLINT Precision, SQLSMALLINT Scale, SQLPOINTER Data, __inout_opt SQLLEN* StringLength, __inout_opt SQLLEN* Indicator);
typedef SQLRETURN(SQL_API* PXSQLSetEnvAttr)(SQLHENV EnvironmentHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER StringLength);
typedef SQLRETURN(SQL_API* PXSQLSetParam)(SQLHSTMT StatementHandle, SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType, SQLSMALLINT ParameterType, SQLULEN LengthPrecision, SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue, __inout SQLLEN* StrLen_or_Ind);
typedef SQLRETURN(SQL_API* PXSQLSetStmtAttr)(SQLHSTMT StatementHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER StringLength);
typedef SQLRETURN(SQL_API* PXSQLSetStmtOption)(SQLHSTMT StatementHandle, SQLUSMALLINT Option, SQLULEN Value);
typedef SQLRETURN(SQL_API* PXSQLSpecialColumns)(SQLHSTMT StatementHandle, SQLUSMALLINT IdentifierType, SQLCHAR* CatalogName, SQLSMALLINT NameLength1, SQLCHAR* SchemaName, SQLSMALLINT NameLength2, SQLCHAR* TableName, SQLSMALLINT NameLength3, SQLUSMALLINT Scope, SQLUSMALLINT Nullable);
typedef SQLRETURN(SQL_API* PXSQLStatistics)(SQLHSTMT StatementHandle, SQLCHAR* CatalogName, SQLSMALLINT NameLength1, SQLCHAR* SchemaName, SQLSMALLINT NameLength2, SQLCHAR* TableName, SQLSMALLINT NameLength3, SQLUSMALLINT Unique, SQLUSMALLINT Reserved);
typedef SQLRETURN(SQL_API* PXSQLTables)(SQLHSTMT StatementHandle, SQLCHAR* CatalogName, SQLSMALLINT NameLength1, SQLCHAR* SchemaName, SQLSMALLINT NameLength2, SQLCHAR* TableName, SQLSMALLINT NameLength3, SQLCHAR* TableType, SQLSMALLINT NameLength4);
typedef SQLRETURN(SQL_API* PXSQLTransact)(SQLHENV EnvironmentHandle, SQLHDBC ConnectionHandle, SQLUSMALLINT CompletionType);

typedef SQLRETURN(SQL_API* PXSQLColAttributeW)(SQLHSTMT hstmt, SQLUSMALLINT iCol, SQLUSMALLINT iField, SQLPOINTER pCharAttr, SQLSMALLINT cbDescMax, SQLSMALLINT* pcbCharAttr, SQLLEN* pNumAttr);
typedef SQLRETURN(SQL_API* PXSQLColAttributesW)(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLUSMALLINT fDescType, SQLPOINTER rgbDesc, SQLSMALLINT cbDescMax, SQLSMALLINT* pcbDesc, SQLLEN* pfDesc);
typedef SQLRETURN(SQL_API* PXSQLConnectW)(SQLHDBC hdbc, SQLWCHAR* szDSN, SQLSMALLINT cchDSN, SQLWCHAR* szUID, SQLSMALLINT cchUID, SQLWCHAR* szAuthStr, SQLSMALLINT cchAuthStr);
typedef SQLRETURN(SQL_API* PXSQLDescribeColW)(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLWCHAR* szColName, SQLSMALLINT cchColNameMax, SQLSMALLINT* pcchColName, SQLSMALLINT* pfSqlType, SQLULEN* pcbColDef, SQLSMALLINT* pibScale, SQLSMALLINT* pfNullable);
typedef SQLRETURN(SQL_API* PXSQLErrorW)(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt, SQLWCHAR* wszSqlState, SQLINTEGER* pfNativeError, SQLWCHAR* wszErrorMsg, SQLSMALLINT cchErrorMsgMax, SQLSMALLINT* pcchErrorMsg);
typedef SQLRETURN(SQL_API* PXSQLExecDirectW)(SQLHSTMT hstmt, SQLWCHAR* szSqlStr, SQLINTEGER TextLength);
typedef SQLRETURN(SQL_API* PXSQLGetConnectAttrW)(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValueMax, SQLINTEGER* pcbValue);
typedef SQLRETURN(SQL_API* PXSQLGetCursorNameW)(SQLHSTMT hstmt, SQLWCHAR* szCursor, SQLSMALLINT cchCursorMax, SQLSMALLINT* pcchCursor);
typedef SQLRETURN(SQL_API* PXSQLSetDescFieldW)(SQLHDESC DescriptorHandle, SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier, SQLPOINTER Value, SQLINTEGER BufferLength);
typedef SQLRETURN(SQL_API* PXSQLGetDescFieldW)(SQLHDESC hdesc, SQLSMALLINT iRecord, SQLSMALLINT iField, SQLPOINTER rgbValue, SQLINTEGER cbBufferLength, SQLINTEGER* StringLength);
typedef SQLRETURN(SQL_API* PXSQLGetDescRecW)(SQLHDESC hdesc, SQLSMALLINT iRecord, SQLWCHAR* szName, SQLSMALLINT cchNameMax, SQLSMALLINT* pcchName, SQLSMALLINT* pfType, SQLSMALLINT* pfSubType, SQLLEN* pLength, SQLSMALLINT* pPrecision, SQLSMALLINT* pScale, SQLSMALLINT* pNullable);
typedef SQLRETURN(SQL_API* PXSQLGetDiagFieldW)(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLSMALLINT fDiagField, SQLPOINTER rgbDiagInfo, SQLSMALLINT cbBufferLength, SQLSMALLINT* pcbStringLength);
typedef SQLRETURN(SQL_API* PXSQLGetDiagRecW)(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLWCHAR* szSqlState, SQLINTEGER* pfNativeError, SQLWCHAR* szErrorMsg, SQLSMALLINT cchErrorMsgMax, SQLSMALLINT* pcchErrorMsg);
typedef SQLRETURN(SQL_API* PXSQLPrepareW)(SQLHSTMT hstmt, SQLWCHAR* szSqlStr, SQLINTEGER cchSqlStr);
typedef SQLRETURN(SQL_API* PXSQLSetConnectAttrW)(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValue);
typedef SQLRETURN(SQL_API* PXSQLSetCursorNameW)(SQLHSTMT hstmt, SQLWCHAR* szCursor, SQLSMALLINT cchCursor);
typedef SQLRETURN(SQL_API* PXSQLColumnsW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName, SQLWCHAR* szColumnName, SQLSMALLINT cchColumnName);
typedef SQLRETURN(SQL_API* PXSQLGetConnectOptionW)(SQLHDBC hdbc, SQLUSMALLINT fOption, SQLPOINTER pvParam);
typedef SQLRETURN(SQL_API* PXSQLGetInfoW)(SQLHDBC hdbc, SQLUSMALLINT fInfoType, SQLPOINTER rgbInfoValue, SQLSMALLINT cbInfoValueMax, SQLSMALLINT* pcbInfoValue);
typedef SQLRETURN(SQL_API* PXSQLGetTypeInfoW)(SQLHSTMT StatementHandle, SQLSMALLINT DataType);
typedef SQLRETURN(SQL_API* PXSQLSetConnectOptionW)(SQLHDBC hdbc, SQLUSMALLINT fOption, SQLULEN vParam);
typedef SQLRETURN(SQL_API* PXSQLSpecialColumnsW)(SQLHSTMT hstmt, SQLUSMALLINT fColType, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName, SQLUSMALLINT fScope, SQLUSMALLINT fNullable);
typedef SQLRETURN(SQL_API* PXSQLStatisticsW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName, SQLUSMALLINT fUnique, SQLUSMALLINT fAccuracy);
typedef SQLRETURN(SQL_API* PXSQLTablesW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName, SQLWCHAR* szTableType, SQLSMALLINT cchTableType);
typedef SQLRETURN(SQL_API* PXSQLDataSourcesW)(SQLHENV henv, SQLUSMALLINT fDirection, SQLWCHAR* szDSN, SQLSMALLINT cchDSNMax, SQLSMALLINT* pcchDSN, SQLWCHAR* wszDescription, SQLSMALLINT cchDescriptionMax, SQLSMALLINT* pcchDescription);
typedef SQLRETURN(SQL_API* PXSQLDriverConnectW)(SQLHDBC hdbc, SQLHWND hwnd, SQLWCHAR* szConnStrIn, SQLSMALLINT cchConnStrIn, SQLWCHAR* szConnStrOut, SQLSMALLINT cchConnStrOutMax, SQLSMALLINT* pcchConnStrOut, SQLUSMALLINT fDriverCompletion);
typedef SQLRETURN(SQL_API* PXSQLBrowseConnectW)(SQLHDBC hdbc, SQLWCHAR* szConnStrIn, SQLSMALLINT cchConnStrIn, SQLWCHAR* szConnStrOut, SQLSMALLINT cchConnStrOutMax, SQLSMALLINT* pcchConnStrOut);
typedef SQLRETURN(SQL_API* PXSQLColumnPrivilegesW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName, SQLWCHAR* szColumnName, SQLSMALLINT cchColumnName);
typedef SQLRETURN(SQL_API* PXSQLGetStmtAttrW)(SQLHSTMT hstmt, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValueMax, SQLINTEGER* pcbValue);
typedef SQLRETURN(SQL_API* PXSQLSetStmtAttrW)(SQLHSTMT hstmt, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValueMax);
typedef SQLRETURN(SQL_API* PXSQLForeignKeysW)(SQLHSTMT hstmt, SQLWCHAR* szPkCatalogName, SQLSMALLINT cchPkCatalogName, SQLWCHAR* szPkSchemaName, SQLSMALLINT cchPkSchemaName, SQLWCHAR* szPkTableName, SQLSMALLINT cchPkTableName, SQLWCHAR* szFkCatalogName, SQLSMALLINT cchFkCatalogName, SQLWCHAR* szFkSchemaName, SQLSMALLINT cchFkSchemaName, SQLWCHAR* szFkTableName, SQLSMALLINT cchFkTableName);
typedef SQLRETURN(SQL_API* PXSQLNativeSqlW)(SQLHDBC hdbc, SQLWCHAR* szSqlStrIn, SQLINTEGER   cchSqlStrIn, SQLWCHAR* szSqlStr, SQLINTEGER   cchSqlStrMax, SQLINTEGER* pcchSqlStr);
typedef SQLRETURN(SQL_API* PXSQLPrimaryKeysW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName);
typedef SQLRETURN(SQL_API* PXSQLProcedureColumnsW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szProcName, SQLSMALLINT cchProcName, SQLWCHAR* szColumnName, SQLSMALLINT cchColumnName);
typedef SQLRETURN(SQL_API* PXSQLProceduresW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szProcName, SQLSMALLINT cchProcName);
typedef SQLRETURN(SQL_API* PXSQLTablePrivilegesW)(SQLHSTMT hstmt, SQLWCHAR* szCatalogName, SQLSMALLINT cchCatalogName, SQLWCHAR* szSchemaName, SQLSMALLINT cchSchemaName, SQLWCHAR* szTableName, SQLSMALLINT cchTableName);
typedef SQLRETURN(SQL_API* PXSQLDriversW)(SQLHENV henv, SQLUSMALLINT fDirection, SQLWCHAR* szDriverDesc, SQLSMALLINT cchDriverDescMax, SQLSMALLINT* pcchDriverDesc, SQLWCHAR* szDriverAttributes, SQLSMALLINT cchDrvrAttrMax, SQLSMALLINT* pcchDrvrAttr);
typedef SQLRETURN(SQL_API* PXSQLColAttributeA)(SQLHSTMT hstmt, SQLSMALLINT iCol, SQLSMALLINT iField, SQLPOINTER pCharAttr, SQLSMALLINT cbCharAttrMax, SQLSMALLINT* pcbCharAttr, SQLLEN* pNumAttr);
typedef SQLRETURN(SQL_API* PXSQLColAttributesA)(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLUSMALLINT fDescType, SQLPOINTER rgbDesc, SQLSMALLINT cbDescMax, SQLSMALLINT* pcbDesc, SQLLEN* pfDesc);
typedef SQLRETURN(SQL_API* PXSQLConnectA)(SQLHDBC hdbc, SQLCHAR* szDSN, SQLSMALLINT cbDSN, SQLCHAR* szUID, SQLSMALLINT cbUID, SQLCHAR* szAuthStr, SQLSMALLINT cbAuthStr);
typedef SQLRETURN(SQL_API* PXSQLDescribeColA)(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLCHAR* szColName, SQLSMALLINT cbColNameMax, SQLSMALLINT* pcbColName, SQLSMALLINT* pfSqlType, SQLULEN* pcbColDef, SQLSMALLINT* pibScale, SQLSMALLINT* pfNullable);
typedef SQLRETURN(SQL_API* PXSQLErrorA)(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt, SQLCHAR* szSqlState, SQLINTEGER* pfNativeError, SQLCHAR* szErrorMsg, SQLSMALLINT cbErrorMsgMax, SQLSMALLINT* pcbErrorMsg);
typedef SQLRETURN(SQL_API* PXSQLExecDirectA)(SQLHSTMT hstmt, SQLCHAR* szSqlStr, SQLINTEGER cbSqlStr);
typedef SQLRETURN(SQL_API* PXSQLGetConnectAttrA)(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValueMax, SQLINTEGER* pcbValue);
typedef SQLRETURN(SQL_API* PXSQLGetCursorNameA)(SQLHSTMT hstmt, SQLCHAR* szCursor, SQLSMALLINT cbCursorMax, SQLSMALLINT* pcbCursor);
typedef SQLRETURN(SQL_API* PXSQLGetDescFieldA)(SQLHDESC hdesc, SQLSMALLINT iRecord, SQLSMALLINT iField, SQLPOINTER rgbValue, SQLINTEGER cbBufferLength, SQLINTEGER* StringLength);
typedef SQLRETURN(SQL_API* PXSQLGetDescRecA)(SQLHDESC hdesc, SQLSMALLINT iRecord, SQLCHAR* szName, SQLSMALLINT cbNameMax, SQLSMALLINT* pcbName, SQLSMALLINT* pfType, SQLSMALLINT* pfSubType, SQLLEN* pLength, SQLSMALLINT* pPrecision, SQLSMALLINT* pScale, SQLSMALLINT* pNullable);
typedef SQLRETURN(SQL_API* PXSQLGetDiagFieldA)(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLSMALLINT fDiagField, SQLPOINTER rgbDiagInfo, SQLSMALLINT cbDiagInfoMax, SQLSMALLINT* pcbDiagInfo);
typedef SQLRETURN(SQL_API* PXSQLGetDiagRecA)(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLCHAR* szSqlState, SQLINTEGER* pfNativeError, SQLCHAR* szErrorMsg, SQLSMALLINT cbErrorMsgMax, SQLSMALLINT* pcbErrorMsg);
typedef SQLRETURN(SQL_API* PXSQLGetStmtAttrA)(SQLHSTMT hstmt, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValueMax, SQLINTEGER* pcbValue);
typedef SQLRETURN(SQL_API* PXSQLGetTypeInfoA)(SQLHSTMT StatementHandle, SQLSMALLINT DataType);
typedef SQLRETURN(SQL_API* PXSQLPrepareA)(SQLHSTMT hstmt, SQLCHAR* szSqlStr, SQLINTEGER cbSqlStr);
typedef SQLRETURN(SQL_API* PXSQLSetConnectAttrA)(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValue);
typedef SQLRETURN(SQL_API* PXSQLSetCursorNameA)(SQLHSTMT hstmt, SQLCHAR* szCursor, SQLSMALLINT cbCursor);
typedef SQLRETURN(SQL_API* PXSQLColumnsA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName, SQLCHAR* szColumnName, SQLSMALLINT cbColumnName);
typedef SQLRETURN(SQL_API* PXSQLGetConnectOptionA)(SQLHDBC hdbc, SQLUSMALLINT fOption, SQLPOINTER pvParam);
typedef SQLRETURN(SQL_API* PXSQLGetInfoA)(SQLHDBC hdbc, SQLUSMALLINT fInfoType, SQLPOINTER rgbInfoValue, SQLSMALLINT cbInfoValueMax, SQLSMALLINT* pcbInfoValue);
typedef SQLRETURN(SQL_API* PXSQLGetStmtOptionA)(SQLHSTMT hstmt, SQLUSMALLINT fOption, SQLPOINTER pvParam);
typedef SQLRETURN(SQL_API* PXSQLSetConnectOptionA)(SQLHDBC hdbc, SQLUSMALLINT fOption, SQLULEN vParam);
typedef SQLRETURN(SQL_API* PXSQLSetStmtOptionA)(SQLHSTMT hstmt, SQLUSMALLINT fOption, SQLULEN vParam);
typedef SQLRETURN(SQL_API* PXSQLSpecialColumnsA)(SQLHSTMT hstmt, SQLUSMALLINT fColType, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName, SQLUSMALLINT fScope, SQLUSMALLINT fNullable);
typedef SQLRETURN(SQL_API* PXSQLStatisticsA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName, SQLUSMALLINT fUnique, SQLUSMALLINT fAccuracy);
typedef SQLRETURN(SQL_API* PXSQLTablesA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName, SQLCHAR* szTableType, SQLSMALLINT cbTableType);
typedef SQLRETURN(SQL_API* PXSQLDataSourcesA)(SQLHENV henv, SQLUSMALLINT fDirection, SQLCHAR* szDSN, SQLSMALLINT cbDSNMax, SQLSMALLINT* pcbDSN, SQLCHAR* szDescription, SQLSMALLINT cbDescriptionMax, SQLSMALLINT* pcbDescription);
typedef SQLRETURN(SQL_API* PXSQLDriverConnectA)(SQLHDBC hdbc, SQLHWND hwnd, SQLCHAR* szConnStrIn, SQLSMALLINT cbConnStrIn, SQLCHAR* szConnStrOut, SQLSMALLINT cbConnStrOutMax, SQLSMALLINT* pcbConnStrOut, SQLUSMALLINT fDriverCompletion);
typedef SQLRETURN(SQL_API* PXSQLBrowseConnectA)(SQLHDBC hdbc, SQLCHAR* szConnStrIn, SQLSMALLINT cbConnStrIn, SQLCHAR* szConnStrOut, SQLSMALLINT cbConnStrOutMax, SQLSMALLINT* pcbConnStrOut);
typedef SQLRETURN(SQL_API* PXSQLColumnPrivilegesA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName, SQLCHAR* szColumnName, SQLSMALLINT cbColumnName);
typedef SQLRETURN(SQL_API* PXSQLDescribeParamA)(SQLHSTMT hstmt, SQLUSMALLINT ipar, SQLSMALLINT* pfSqlType, SQLUINTEGER* pcbParamDef, SQLSMALLINT* pibScale, SQLSMALLINT* pfNullable);
typedef SQLRETURN(SQL_API* PXSQLForeignKeysA)(SQLHSTMT hstmt, SQLCHAR* szPkCatalogName, SQLSMALLINT cbPkCatalogName, SQLCHAR* szPkSchemaName, SQLSMALLINT cbPkSchemaName, SQLCHAR* szPkTableName, SQLSMALLINT cbPkTableName, SQLCHAR* szFkCatalogName, SQLSMALLINT cbFkCatalogName, SQLCHAR* szFkSchemaName, SQLSMALLINT cbFkSchemaName, SQLCHAR* szFkTableName, SQLSMALLINT cbFkTableName);
typedef SQLRETURN(SQL_API* PXSQLNativeSqlA)(SQLHDBC hdbc, SQLCHAR* szSqlStrIn, SQLINTEGER cbSqlStrIn, SQLCHAR* szSqlStr, SQLINTEGER cbSqlStrMax, SQLINTEGER* pcbSqlStr);
typedef SQLRETURN(SQL_API* PXSQLPrimaryKeysA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName);
typedef SQLRETURN(SQL_API* PXSQLProcedureColumnsA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szProcName, SQLSMALLINT cbProcName, SQLCHAR* szColumnName, SQLSMALLINT cbColumnName);
typedef SQLRETURN(SQL_API* PXSQLProceduresA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szProcName, SQLSMALLINT cbProcName);
typedef SQLRETURN(SQL_API* PXSQLTablePrivilegesA)(SQLHSTMT hstmt, SQLCHAR* szCatalogName, SQLSMALLINT cbCatalogName, SQLCHAR* szSchemaName, SQLSMALLINT cbSchemaName, SQLCHAR* szTableName, SQLSMALLINT cbTableName);
typedef SQLRETURN(SQL_API* PXSQLDriversA)(SQLHENV henv, SQLUSMALLINT fDirection, SQLCHAR* szDriverDesc, SQLSMALLINT cbDriverDescMax, SQLSMALLINT* pcbDriverDesc, SQLCHAR* szDriverAttributes, SQLSMALLINT cbDrvrAttrMax, SQLSMALLINT* pcbDrvrAttr);


PXActionResult PXAPI PXDataBaseInitialize(PXDataBase* const pxDataBase)
{
	PXClear(PXDataBase, pxDataBase);

	// Load library, ODBC
	{
		const PXActionResult pxActionResult = PXLibraryOpenA(&pxDataBase->ODBCLibrary, "ODBC32.DLL");

		if(PXActionSuccessful != pxActionResult)
		{
			return PXActionRefusedNotSupported;
		}
	}

	// Fetch functions
	{
		const PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
		{
			{ &pxDataBase->AllocConnect,"SQLAllocConnect"},
			{ &pxDataBase->AllocEnv,"SQLAllocEnv"},
			{ &pxDataBase->AllocHandle,"SQLAllocHandle"},
			{ &pxDataBase->AllocStmt,"SQLAllocStmt"},
			{ &pxDataBase->BindCol,"SQLBindCol"},
			{ &pxDataBase->BindParam,"SQLBindParam"},
			{ &pxDataBase->Cancel,"SQLCancel"},
			{ &pxDataBase->CancelHandle,"SQLCancelHandle"},
			{ &pxDataBase->CloseCursor,"SQLCloseCursor"},
		 { &pxDataBase->ColAttribute,"SQLColAttribute"},
		 { &pxDataBase->Columns,"SQLColumns"},
		 { &pxDataBase->Connect,"SQLConnect"},
		 { &pxDataBase->CopyDesc,"SQLCopyDesc"},
		 { &pxDataBase->DataSources,"SQLDataSources"},
		 { &pxDataBase->DescribeCol,"SQLDescribeCol"},
		 { &pxDataBase->Disconnect,"SQLDisconnect"},
		 { &pxDataBase->EndTran,"SQLEndTran"},
		 { &pxDataBase->Error,"SQLError"},
		 { &pxDataBase->ExecDirect,"SQLExecDirect"},
		 { &pxDataBase->Execute,"SQLExecute"},
		 { &pxDataBase->Fetch,"SQLFetch"},
		 { &pxDataBase->FetchScroll,"SQLFetchScroll"},
		 { &pxDataBase->FreeConnect,"SQLFreeConnect"},
		 { &pxDataBase->FreeEnv,"SQLFreeEnv"},
		 { &pxDataBase->FreeHandle,"SQLFreeHandle"},
		 { &pxDataBase->FreeStmt,"SQLFreeStmt"},
		 { &pxDataBase->GetConnectAttr,"SQLGetConnectAttr"},
		 { &pxDataBase->GetConnectOption,"SQLGetConnectOption"},
		 { &pxDataBase->GetCursorName,"SQLGetCursorName"},
		 { &pxDataBase->GetData,"SQLGetData"},
		 { &pxDataBase->GetDescField,"SQLGetDescField"},
		 { &pxDataBase->GetDescRec,"SQLGetDescRec"},
		 { &pxDataBase->GetDiagField,"SQLGetDiagField"},
		 { &pxDataBase->GetDiagRec,"SQLGetDiagRec"},
		 { &pxDataBase->GetEnvAttr,"SQLGetEnvAttr"},
		 { &pxDataBase->GetFunctions,"SQLGetFunctions"},
		 { &pxDataBase->GetInfo,"SQLGetInfo"},
		 { &pxDataBase->GetStmtAttr,"SQLGetStmtAttr"},
		 { &pxDataBase->GetStmtOption,"SQLGetStmtOption"},
		 { &pxDataBase->GetTypeInfo,"SQLGetTypeInfo"},
		 { &pxDataBase->NumResultCols,"SQLNumResultCols"},
		 { &pxDataBase->ParamData,"SQLParamData"},
		 { &pxDataBase->Prepare,"SQLPrepare"},
		 { &pxDataBase->PutData,"SQLPutData"},
		 { &pxDataBase->RowCount,"SQLRowCount"},
		 { &pxDataBase->SetConnectAttr,"SQLSetConnectAttr"},
		 { &pxDataBase->SetConnectOption,"SQLSetConnectOption"},
		 { &pxDataBase->SetCursorName,"SQLSetCursorName"},
		 { &pxDataBase->SetDescField,"SQLSetDescField"},
		 { &pxDataBase->SetDescRec,"SQLSetDescRec"},
		 { &pxDataBase->SetEnvAttr,"SQLSetEnvAttr"},
		 { &pxDataBase->SetParam,"SQLSetParam"},
		 { &pxDataBase->SetStmtAttr,"SQLSetStmtAttr"},
		 { &pxDataBase->SetStmtOption,"SQLSetStmtOption"},
		 { &pxDataBase->SpecialColumns,"SQLSpecialColumns"},
		 { &pxDataBase->Statistics,"SQLStatistics"},
		 { &pxDataBase->Tables,"SQLTables"},
		 { &pxDataBase->Transact,"SQLTransact"},

		 {&pxDataBase->ColAttributeW,"SQLColAttributeW"},
		 {&pxDataBase->ColAttributesW,"SQLColAttributesW"},
		 {&pxDataBase->ConnectW,"SQLConnectW"},
		 {&pxDataBase->DescribeColW,"SQLDescribeColW"},
		 {&pxDataBase->ErrorW,"SQLErrorW"},
		 {&pxDataBase->ExecDirectW,"SQLExecDirectW"},
		 {&pxDataBase->GetConnectAttrW,"SQLGetConnectAttrW"},
		 {&pxDataBase->GetCursorNameW,"SQLGetCursorNameW"},
		 {&pxDataBase->SetDescFieldW,"SQLSetDescFieldW"},
		 {&pxDataBase->GetDescFieldW,"SQLGetDescFieldW"},
		 {&pxDataBase->GetDescRecW,"SQLGetDescRecW"},
		 {&pxDataBase->GetDiagFieldW,"SQLGetDiagFieldW"},
		 {&pxDataBase->GetDiagRecW,"SQLGetDiagRecW"},
		 {&pxDataBase->PrepareW,"SQLPrepareW"},
		 {&pxDataBase->SetConnectAttrW,"SQLSetConnectAttrW"},
		 {&pxDataBase->SetCursorNameW,"SQLSetCursorNameW"},
		 {&pxDataBase->ColumnsW,"SQLColumnsW"},
		 {&pxDataBase->GetConnectOptionW,"SQLGetConnectOptionW"},
		 {&pxDataBase->GetInfoW,"SQLGetInfoW"},
		 {&pxDataBase->GetTypeInfoW,"SQLGetTypeInfoW"},
		 {&pxDataBase->SetConnectOptionW,"SQLSetConnectOptionW"},
		 {&pxDataBase->SpecialColumnsW,"SQLSpecialColumnsW"},
		 {&pxDataBase->StatisticsW,"SQLStatisticsW"},
		 {&pxDataBase->TablesW,"SQLTablesW"},
		 {&pxDataBase->DataSourcesW,"SQLDataSourcesW"},
		 {&pxDataBase->DriverConnectW,"SQLDriverConnectW"},
		 {&pxDataBase->BrowseConnectW,"SQLBrowseConnectW"},
		 {&pxDataBase->ColumnPrivilegesW,"SQLColumnPrivilegesW"},
		 {&pxDataBase->GetStmtAttrW,"SQLGetStmtAttrW"},
		 {&pxDataBase->SetStmtAttrW,"SQLSetStmtAttrW"},
		 {&pxDataBase->ForeignKeysW,"SQLForeignKeysW"},
		 {&pxDataBase->NativeSqlW,"SQLNativeSqlW"},
		 {&pxDataBase->PrimaryKeysW,"SQLPrimaryKeysW"},
		 {&pxDataBase->ProcedureColumnsW,"SQLProcedureColumnsW"},
		 {&pxDataBase->ProceduresW,"SQLProceduresW"},
		 {&pxDataBase->TablePrivilegesW,"SQLTablePrivilegesW"},
		 {&pxDataBase->DriversW,"SQLDriversW"},
		 {&pxDataBase->ColAttributeA,"SQLColAttributeA"},
		 {&pxDataBase->ColAttributesA,"SQLColAttributesA"},
		 {&pxDataBase->ConnectA,"SQLConnectA"},
		 {&pxDataBase->DescribeColA,"SQLDescribeColA"},
		 {&pxDataBase->ErrorA,"SQLErrorA"},
		 {&pxDataBase->ExecDirectA,"SQLExecDirectA"},
		 {&pxDataBase->GetConnectAttrA,"SQLGetConnectAttrA"},
		 {&pxDataBase->GetCursorNameA,"SQLGetCursorNameA"},
		 {&pxDataBase->GetDescFieldA,"SQLGetDescFieldA"},
		 {&pxDataBase->GetDescRecA,"SQLGetDescRecA"},
		 {&pxDataBase->GetDiagFieldA,"SQLGetDiagFieldA"},
		 {&pxDataBase->GetDiagRecA,"SQLGetDiagRecA"},
		 {&pxDataBase->GetStmtAttrA,"SQLGetStmtAttrA"},
		 {&pxDataBase->GetTypeInfoA,"SQLGetTypeInfoA"},
		 {&pxDataBase->PrepareA,"SQLPrepareA"},
		 {&pxDataBase->SetConnectAttrA,"SQLSetConnectAttrA"},
		 {&pxDataBase->SetCursorNameA,"SQLSetCursorNameA"},
		 {&pxDataBase->ColumnsA,"SQLColumnsA"},
		 {&pxDataBase->GetConnectOptionA,"SQLGetConnectOptionA"},
		 {&pxDataBase->GetInfoA,"SQLGetInfoA"},
		 {&pxDataBase->GetStmtOptionA,"SQLGetStmtOptionA"},
		 {&pxDataBase->SetConnectOptionA,"SQLSetConnectOptionA"},
		 {&pxDataBase->SetStmtOptionA,"SQLSetStmtOptionA"},
		 {&pxDataBase->SpecialColumnsA,"SQLSpecialColumnsA"},
		 {&pxDataBase->StatisticsA,"SQLStatisticsA"},
		 {&pxDataBase->TablesA,"SQLTablesA"},
		 {&pxDataBase->DataSourcesA,"SQLDataSourcesA"},
		 {&pxDataBase->DriverConnectA,"SQLDriverConnectA"},
		 {&pxDataBase->BrowseConnectA,"SQLBrowseConnectA"},
		 {&pxDataBase->ColumnPrivilegesA,"SQLColumnPrivilegesA"},
		 {&pxDataBase->DescribeParamA,"SQLDescribeParamA"},
		 {&pxDataBase->ForeignKeysA,"SQLForeignKeysA"},
		 {&pxDataBase->NativeSqlA,"SQLNativeSqlA"},
		 {&pxDataBase->PrimaryKeysA,"SQLPrimaryKeysA"},
		 {&pxDataBase->ProcedureColumnsA,"SQLProcedureColumnsA"},
		 {&pxDataBase->ProceduresA,"SQLProceduresA"},
		 {&pxDataBase->TablePrivilegesA,"SQLTablePrivilegesA"},
		 {&pxDataBase->DriversA,"SQLDriversA"},
		};

		const PXSize amount = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

		PXLibraryGetSymbolListA(&pxDataBase->ODBCLibrary, pxLibraryFuntionEntry, amount);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDataBaseRelease(PXDataBase* const pxDataBase)
{
	PXDataBaseCleanup(pxDataBase);

	PXLibraryClose(&pxDataBase->ODBCLibrary);

	PXClear(PXDataBase, pxDataBase);

	return PXActionSuccessful;
}

PXSQLType PXAPI PXSQLTypeFromID(const PXInt32U sqlTypeID)
{
#if OSUnix
	return PXSQLTypeInvalid;

#elif PXOSWindowsDestop
	switch(sqlTypeID)
	{
		// Mysql?
		case -9: return PXSQLTypeStringUNICODE;

			// is this thenCorrect?
		case SQL_VARCHAR:return PXSQLTypeStringANSI;

		case SQL_UNKNOWN_TYPE: return PXSQLTypeUnkown;
		case SQL_CHAR:return PXSQLTypeCharacter;
		case SQL_NUMERIC:return PXSQLTypeNumeric;
		case SQL_DECIMAL:return PXSQLTypeDecimal;
		case SQL_INTEGER:return PXSQLTypeInteger;
		case SQL_SMALLINT:return PXSQLTypeShort;
		case SQL_FLOAT:return PXSQLTypeFloat;
		case SQL_REAL:return PXSQLTypeReal;
		case SQL_DOUBLE:return PXSQLTypeDouble;
#if (ODBCVER >= 0x0300)
		case SQL_DATETIME:return PXSQLTypeDATETIME;
#endif


			/* One-parameter shortcuts for date/time data types */
#if (ODBCVER >= 0x0300)
		case SQL_TYPE_DATE:return PXSQLTypeTYPE_DATE;
		case SQL_TYPE_TIME:return PXSQLTypeTYPE_TIME;
		case SQL_TYPE_TIMESTAMP:return PXSQLTypeTYPE_TIMESTAMP;
#endif

		default:
			return PXSQLTypeInvalid;
	}
#else 
	return PXSQLTypeInvalid;
#endif
}

PXActionResult PXAPI PXDataBaseConnectA
(
	PXDataBase* const pxDataBase,
	const char* const source,
	const char* const database,
	const char* const user,
	const char* const password
)
{
	PXText pxTextSource;
	PXText pxTextDatabase;
	PXText pxTextUser;
	PXText pxTextPassword;

	PXTextConstructFromAdressA(&pxTextSource, source, PXTextLengthUnkown, PXTextLengthUnkown);
	PXTextConstructFromAdressA(&pxTextDatabase, database, PXTextLengthUnkown, PXTextLengthUnkown);
	PXTextConstructFromAdressA(&pxTextUser, user, PXTextLengthUnkown, PXTextLengthUnkown);
	PXTextConstructFromAdressA(&pxTextPassword, password, PXTextLengthUnkown, PXTextLengthUnkown);

	const PXActionResult pxActionResult = PXDataBaseConnect(pxDataBase, &pxTextSource, &pxTextDatabase, &pxTextUser, &pxTextPassword);


	return pxActionResult;
}

PXActionResult PXAPI PXDataBaseConnect
(
	PXDataBase* const pxDataBase,
	const PXText* const source,
	const PXText* const database,
	const PXText* const user,
	const PXText* const password
)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const PXSQLAllocHandle pxSQLAllocHandle = pxDataBase->AllocHandle;
	const PXSQLSetEnvAttr pxSQLSetEnvAttr = pxDataBase->SetEnvAttr;
	const PXSQLSetConnectAttrW pxSQLSetConnectAttrW = pxDataBase->SetConnectAttrW;


	// Allocate environment handle
	{

		const SQLRETURN result = pxSQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &pxDataBase->EnvironmentID);
		const PXBool successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

		if(!successful)
		{
			return PXActionFailedMemoryAllocation;
		}
	}

	// Set the ODBC version environment attribute
	{
		const SQLRETURN result = pxSQLSetEnvAttr(pxDataBase->EnvironmentID, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		const PXBool successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

		if(!successful)
		{
			return PXActionFailedSettingsInvalid;;
		}
	}

	// Allocate connection handle
	{
		const SQLRETURN result = pxSQLAllocHandle(SQL_HANDLE_DBC, pxDataBase->EnvironmentID, &pxDataBase->ConnectionID);
		const PXBool successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

		if(!successful)
		{
			return PXActionFailedMemoryAllocation;
		}
	}

	// Set login timeout to 5 seconds
	{
		const SQLRETURN resultConnect = pxSQLSetConnectAttrW(pxDataBase->ConnectionID, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
		const PXBool successful = resultConnect == SQL_SUCCESS || resultConnect == SQL_SUCCESS_WITH_INFO;

		if(!successful)
		{
			return PXActionFailedSettingsInvalid;
		}
	}

	PXDataBaseScanForDrivers(pxDataBase);

	// Connect
	{
		PXText pxTextSQLConnectionString;
		PXTextConstructNamedBufferA(&pxTextSQLConnectionString, pxTextSQLConnectionStringBuffer, 1024u);

		PXText pxTextSQLConnectionStringResult;
		PXTextConstructNamedBufferA(&pxTextSQLConnectionStringResult, pxTextSQLConnectionStringResultBuffer, 1024u);

		SQLRETURN resultDriverConnect = 0;

		switch(source->Format)
		{
			case TextFormatASCII:
			case TextFormatUTF8:
			{
				const PXSQLDriverConnectA pxSQLDriverConnectA = pxDataBase->DriverConnectA;

				const char driver[] = "MySQL ODBC 8.0 Unicode Driver";

				PXTextPrint
				(
					&pxTextSQLConnectionString,
					"Driver={%ls};Server=%ls;Database=%ls;UID=%ls;PWD=%ls;",
					driver,
					source->TextA,
					database->TextA,
					user->TextA,
					password->TextA
				);

				SQLSMALLINT writtenSize = 0;

				resultDriverConnect = pxSQLDriverConnectA
				(
					pxDataBase->ConnectionID,
					NULL,
					(SQLCHAR*)pxTextSQLConnectionString.TextA,
					pxTextSQLConnectionString.SizeUsed,
					(SQLCHAR*)pxTextSQLConnectionStringResult.TextA,
					pxTextSQLConnectionStringResult.SizeAllocated,
					&writtenSize,
					SQL_DRIVER_NOPROMPT
				);

				pxTextSQLConnectionStringResult.SizeUsed = writtenSize;

				break;
			}
			case TextFormatUNICODE:
			{
				const PXSQLDriverConnectW pxSQLDriverConnectW = pxDataBase->DriverConnectW;

				const wchar_t driver[] = L"MySQL ODBC 8.0 Unicode Driver";

				pxTextSQLConnectionString.SizeUsed = wsprintfW
				(
					pxTextSQLConnectionString.TextW,
					L"Driver={%ls};Server=%ls;Database=%ls;UID=%ls;PWD=%ls;",
					driver,
					source->TextW,
					database->TextW,
					user->TextW,
					password->TextW
				);

				SQLSMALLINT writtenSize = 0;

				resultDriverConnect = pxSQLDriverConnectW
				(
					pxDataBase->ConnectionID,
					NULL,
					pxTextSQLConnectionString.TextW,
					pxTextSQLConnectionString.SizeUsed,
					pxTextSQLConnectionStringResult.TextW,
					pxTextSQLConnectionStringResult.SizeAllocated,
					&writtenSize,
					SQL_DRIVER_NOPROMPT
				);

				pxTextSQLConnectionStringResult.SizeUsed = writtenSize;

				break;
			}

			default:
				return PXActionRefuedTextFormatUnsupported;
		}

		const PXBool successfulConnected = resultDriverConnect == SQL_SUCCESS || resultDriverConnect == SQL_SUCCESS_WITH_INFO;

		if(!successfulConnected)
		{
			return PXActionFailedConnectionFailure;
		}
	}

#if 0
	if(dataBaseConnection->OnConnectedEvent)
	{
		// dataBaseConnection->OnConnectedEvent(driver, source, database, user);
	}
#endif

	return PXActionSuccessful;

	/*
	PXSQLWCHAR* serverNameS = (wchar_t*)serverName;
	const PXSQLSMALLINT serverNameLength = PXSQL_NTSL;
	PXSQLWCHAR* userName = NULL;
	const PXSQLSMALLINT userNameLength = 0;
	PXSQLWCHAR* password = NULL;
	const PXSQLSMALLINT passwordLength = 0;

	retcode = PXSQLConnectW
	(
	connectionHandle,
	serverNameS,
	serverNameLength,
	userName,
	userNameLength,
	password,
	passwordLength
	);* /

	if (successfulConnected)
	{
	PXSQLUSMALLINT infoType = PXSQL_PROCEDURES;
	wchar_t bufferLength[1024];
	PXSQLSMALLINT cbInfoValueMax = sizeof(bufferLength);
	PXSQLSMALLINT pcbInfoValue = 0;

	PXSQLRETURN infoResult = PXSQLGetInfoW
	(
	 _handleConnection,
	 infoType,
	 bufferLength,
	 cbInfoValueMax,
	 &pcbInfoValue
	);
	}*/
#endif
}

void PXAPI PXDataBaseDisconnect(PXDataBase* const pxDataBase)
{
#if OSUnix
#elif PXOSWindowsDestop
	if(pxDataBase->ConnectionID)
	{
		const PXSQLDisconnect pxSQLDisconnect = pxDataBase->Disconnect;
		const PXSQLFreeHandle pxSQLFreeHandle = pxDataBase->FreeHandle;
		const SQLRETURN resultDisconnect = pxSQLDisconnect(pxDataBase->ConnectionID);
		const SQLRETURN resultFree = pxSQLFreeHandle(SQL_HANDLE_DBC, pxDataBase->ConnectionID);

		pxDataBase->ConnectionID = PXNull;
	}
#endif
}

void PXAPI PXDataBaseCleanup(PXDataBase* const pxDataBase)
{
	PXDataBaseDisconnect(pxDataBase);

#if OSUnix
#elif PXOSWindowsDestop
	if(pxDataBase->EnvironmentID)
	{
		const PXSQLFreeHandle pxSQLFreeHandle = pxDataBase->FreeHandle;
		const SQLRETURN result = pxSQLFreeHandle(SQL_HANDLE_ENV, pxDataBase->EnvironmentID);

		pxDataBase->EnvironmentID = PXNull;
	}
#endif
}

void PXAPI PXDataBaseScanForDrivers(PXDataBase* const pxDataBase)
{
#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"DataBase",
		"ODBC-Driver",
		"Scanning..."
	);
#endif

#if OSUnix
#elif PXOSWindowsDestop
	SQLUSMALLINT direction = SQL_FETCH_FIRST;
	unsigned char finished = 0;

	while(!finished)
	{
		wchar_t driverDescription[1024];
		const SQLSMALLINT driverDescriptionSize = sizeof(driverDescription) / sizeof(wchar_t);
		SQLSMALLINT driverDescriptionSizeWritten = 0;

		wchar_t driverAttributes[1024];
		const SQLSMALLINT driverAttributesSize = sizeof(driverAttributes) / sizeof(wchar_t);
		SQLSMALLINT driverAttributesSizeWritten = 0;

		const PXSQLDriversW pxSQLDriversW = pxDataBase->DriversW;
		const SQLRETURN resultDriver = pxSQLDriversW
		(
			pxDataBase->EnvironmentID,
			direction,
			driverDescription,
			driverDescriptionSize,
			&driverDescriptionSizeWritten,
			driverAttributes,
			driverAttributesSize,
			&driverAttributesSizeWritten
		);

		switch(resultDriver)
		{
			case SQL_SUCCESS_WITH_INFO:
			case SQL_SUCCESS: // Do nothing and go next
				direction = SQL_FETCH_NEXT; // [!] Important [!] - Mark to go next.

				if(pxDataBase->OnDriverDetectedEvent)
				{
					pxDataBase->OnDriverDetectedEvent(driverDescription, driverAttributes);
				}

				printf("| %-57ls | %-13ls |\n", driverDescription, driverAttributes);
				break;

			default:
			case SQL_ERROR: // Unkown error
			case SQL_INVALID_HANDLE: // Environment handle was invalid
			case SQL_NO_DATA: // read error : No entry
				finished = 1;
				break;
		}
	}
#endif

#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"DataBase",
		"ODBC-Driver",
		"Scanned"
	);
#endif
}

PXActionResult PXAPI PXDataBaseCommandExecute(PXDataBase* const pxDataBase, const PXText* const pxTextSQLStatement)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	SQLHSTMT handleStatement = 0;
	PXSize colums = 0;
	PXSize rows = 0;

	// Check connection
	{
		const PXBool connected = pxDataBase->ConnectionID != 0;

		if(!connected)
		{
			return PXActionRefusedNotConnected;
		}
	}

	// Alloc statement
	{
		const PXSQLAllocHandle pxSQLAllocHandle = pxDataBase->AllocHandle;
		const SQLRETURN retcode = pxSQLAllocHandle(SQL_HANDLE_STMT, pxDataBase->ConnectionID, &handleStatement);
		const PXBool sucessfulAlloc = retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO;

		if(!sucessfulAlloc)
		{
			return PXActionFailedMemoryAllocation;
		}
	}

	// Execute command
	{
		SQLRETURN resultExecute = 0;

		switch(pxTextSQLStatement->Format)
		{
			case TextFormatASCII:
			case TextFormatUTF8:
			{
				const PXSQLExecDirectA pxSQLExecDirectA = pxDataBase->ExecDirectA;

				resultExecute = pxSQLExecDirectA(handleStatement, (SQLCHAR*)pxTextSQLStatement->TextA, SQL_NTS);
				break;
			}
			case TextFormatUNICODE:
			{
				const PXSQLExecDirectW pxSQLExecDirectW = pxDataBase->ExecDirectW;

				resultExecute = pxSQLExecDirectW(handleStatement, pxTextSQLStatement->TextW, SQL_NTSL);
				break;
			}

			default:
				return PXActionRefuedTextFormatUnsupported;
		}

		const PXBool successful = resultExecute == SQL_SUCCESS || resultExecute == SQL_SUCCESS_WITH_INFO;

		if(!successful)
		{
			return PXActionFailedCommandExecute;
		}
	}

	// Get table size
	{
		const PXSQLNumResultCols pxSQLNumResultCols = pxDataBase->NumResultCols;
		const PXSQLRowCount pxSQLRowCount = pxDataBase->RowCount;

		SQLSMALLINT nCols = 0;
		SQLLEN nRows = 0;

		pxSQLNumResultCols(handleStatement, &nCols);
		pxSQLRowCount(handleStatement, &nRows);

		colums = nCols;
		rows = nRows;
	}

	if(pxDataBase->OnResultEvent)
	{
		pxDataBase->OnResultEvent(colums, rows);
	}

	printf("[Result] Rows:%zi Colums:%zi\n", rows, colums);

	printf("+----------------------+----------------------+----------------------+----------------------+\n");

	printf("|");

	for(PXSize columIndex = 1; columIndex <= colums; ++columIndex)
	{
		wchar_t columnName[1024];
		const SQLSMALLINT columnNameSize = sizeof(columnName) / sizeof(wchar_t);
		SQLSMALLINT columnNameSizeWritten;

		SQLSMALLINT DataType;
		SQLULEN ColumnSize;
		SQLSMALLINT decimalDigits = 0;
		SQLSMALLINT isNullableID = 0;
		unsigned char isNullable = 0;
		unsigned char isBaseType = 0;

		const PXSQLDescribeColW pxSQLDescribeColW = pxDataBase->DescribeColW;
		const SQLRETURN resultDescriped = pxSQLDescribeColW
		(
			handleStatement,
			columIndex,
			columnName,
			columnNameSize,
			&columnNameSizeWritten,
			&DataType,
			&ColumnSize,
			&decimalDigits,
			&isNullableID
		);

		const PXSQLType type = PXSQLTypeFromID(DataType);

		//SQLColAttribute();

		//isBaseType = DataType == PXSQL_COLUMN_DISTINCT_TYPE;
		isNullable = isNullableID == SQL_NULLABLE; // PXSQL_NO_NULLS

		const char* typeName = 0;

		switch(type)
		{
			default:
			case PXSQLTypeInvalid: typeName = "Invalid"; break;
			case PXSQLTypeUnkown: typeName = "Unkown"; break;
			case PXSQLTypeCharacter: typeName = "Character"; break;
			case PXSQLTypeNumeric:typeName = "Numeric"; break;
			case PXSQLTypeDecimal:typeName = "Decimal"; break;
			case PXSQLTypeInteger:typeName = "Integer"; break;
			case PXSQLTypeShort:typeName = "Short"; break;
			case PXSQLTypeFloat:typeName = "Invalid"; break;
			case PXSQLTypeReal:typeName = "Real"; break;
			case PXSQLTypeDouble:typeName = "Double"; break;
			case PXSQLTypeDATETIME:typeName = "DATETIME"; break;
			case PXSQLTypeStringANSI:typeName = "StringANSI"; break;
			case PXSQLTypeStringUNICODE:typeName = "StringUNICODE"; break;
			case PXSQLTypeTYPE_DATE:typeName = "TYPE_DATE"; break;
			case PXSQLTypeTYPE_TIME:typeName = "TYPE_TIME"; break;
			case PXSQLTypeTYPE_TIMESTAMP:typeName = "TYPE_TIMESTAMP"; break;
		}

		if(pxDataBase->OnColumInfoEvent)
		{
			pxDataBase->OnColumInfoEvent(columIndex, type, isNullable, columnName, columnNameSizeWritten);
		}

		printf(" %-20ls |", columnName);
	}

	printf("\n");
	printf("+----------------------+----------------------+----------------------+----------------------+\n");

	SQLRETURN resultFetch = 0;
	const PXSQLFetch pxSQLFetch = pxDataBase->Fetch;

	for(PXSize rowIndex = 0; SQL_SUCCEEDED(resultFetch = pxSQLFetch(handleStatement)); ++rowIndex)
	{
		printf("|");

		for(PXSize columIndex = 1; columIndex <= colums; ++columIndex)
		{
			wchar_t fieldBuffer[1048];
			const PXSize fieldBufferSize = sizeof(fieldBuffer) / sizeof(wchar_t);
			SQLLEN readLength = 0;

			const PXSQLGetData pxSQLGetData = pxDataBase->GetData;
			const SQLRETURN resultData = pxSQLGetData(handleStatement, columIndex, SQL_UNICODE_CHAR, fieldBuffer, fieldBufferSize, &readLength);
			const PXBool success = SQL_SUCCEEDED(resultData);

			if(success)
			{
				if(pxDataBase->OnRowDataEvent)
				{
					pxDataBase->OnRowDataEvent(rowIndex, columIndex, fieldBuffer);
				}

				printf(" %-20ls |", fieldBuffer);
			}
		}

		printf("\n");
	}

	printf("+----------------------+----------------------+----------------------+----------------------+\n");

	// Free memory
	{
		const PXSQLFreeHandle pxSQLFreeHandle = pxDataBase->FreeHandle;
		const SQLRETURN resultFree = pxSQLFreeHandle(SQL_HANDLE_STMT, handleStatement);
		const PXBool successful = resultFree == SQL_SUCCESS || resultFree == SQL_SUCCESS_WITH_INFO;

		if(successful)
		{
			return PXActionFailedMemoryRelease;
		}
	}

	return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXDataBaseCommandCancel(PXDataBase* const dataBaseConnection)
{
	return PXActionRefusedNotImplemented; // SQLRETURN SQL_API SQLCancel(SQLHSTMT StatementHandle);
}