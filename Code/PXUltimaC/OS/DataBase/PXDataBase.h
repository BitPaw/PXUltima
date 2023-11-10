#ifndef PXDataBaseINLCUDE
#define PXDataBaseINLCUDE

#include <OS/System/PXOSVersion.h>

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#endif

#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum PXSQLType_
    {
        PXSQLTypeInvalid,
        PXSQLTypeUnkown,

        // C Types
        PXSQLTypeCharacter, // tinyint
        PXSQLTypeInteger,
        PXSQLTypeLongLong, // bigint
        PXSQLTypeFloat,
        PXSQLTypeDouble,
        PXSQLTypeShort, // smallint

        PXSQLTypeStringANSI, // char Array
        PXSQLTypeStringUNICODE, // wchar_t Array


        // PXSQL - Variable Types
        PXSQLTypeNumeric,
        PXSQLTypeDecimal,
        PXSQLTypeReal, // like float

        PXSQLTypeDATETIME,

        PXSQLTypeTYPE_DATE,
        PXSQLTypeTYPE_TIME,
        PXSQLTypeTYPE_TIMESTAMP
    }
    PXSQLType;

    const PXSQLType PXAPI PXSQLTypeFromID(const PXInt32U sqlTypeID);

    typedef void (*ConnectedEvent)
        (
            const wchar_t* driver,
            const wchar_t* source,
            const wchar_t* database,
            const wchar_t* user
            );

    typedef void (*DriverDetectedEvent)(const wchar_t* driverName, const wchar_t* driverAttribte);
    typedef void (*ResultEvent)(const PXSize columSize, const PXSize rowSize);
    typedef void (*ColumInfoEvent)
        (
            const PXSize columIndex,
            const PXSQLType type,
            const unsigned char canBeNull,
            const wchar_t* name,
            const PXSize nameLength
            );
    typedef void (*RowDataEvent)
        (
            const PXSize rowIndex,
            const PXSize columIndex,
            const wchar_t* fieldString
            );


    typedef struct PXSQLDataBaseConnection_
    {
#if OSUnix
#elif PXOSWindowsDestop
        SQLHENV EnvironmentID;
        SQLHDBC ConnectionID;
#endif	

        ConnectedEvent OnConnectedEvent;
        DriverDetectedEvent OnDriverDetectedEvent;
        ResultEvent OnResultEvent;
        ColumInfoEvent OnColumInfoEvent;
        RowDataEvent OnRowDataEvent;
    }
    PXDataBase;


    PXPublic void PXAPI PXDataBaseConstruct(PXDataBase* const dataBaseConnection);
    PXPublic void PXAPI PXDataBaseDestruct(PXDataBase* const dataBaseConnection);

    PXPublic PXActionResult PXAPI PXDataBaseConnect
    (
        PXDataBase* const dataBaseConnection,
        const PXText* const source,
        const PXText* const database,
        const PXText* const user,
        const PXText* const password
    );
    PXPublic void PXAPI PXDataBaseDisconnect(PXDataBase* const dataBaseConnection);
    PXPublic void PXAPI PXDataBaseCleanup(PXDataBase* const dataBaseConnection);

    PXPublic void PXAPI PXDataBaseScanForDrivers(PXDataBase* const dataBaseConnection);

    PXPublic PXActionResult PXAPI PXDataBaseCommandExecute(PXDataBase* const dataBaseConnection, const PXText* const PXSQLStatement);


    PXPublic PXActionResult PXAPI PXDataBaseCommandCancel(PXDataBase* const dataBaseConnection);





#if 0

    SQLRETURN  SQL_API SQLAllocConnect(SQLHENV EnvironmentHandle, _Out_ SQLHDBC* ConnectionHandle);
    SQLRETURN  SQL_API SQLAllocEnv(_Out_ SQLHENV* EnvironmentHandle);
    SQLRETURN  SQL_API SQLAllocHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle, _Out_ SQLHANDLE* OutputHandle);
    SQLRETURN  SQL_API SQLAllocStmt(SQLHDBC ConnectionHandle, _Out_ SQLHSTMT* StatementHandle);
    SQLRETURN  SQL_API SQLBindCol(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, _Inout_updates_opt_(_Inexpressible_(BufferLength)) SQLPOINTER TargetValue, SQLLEN BufferLength, _Inout_opt_ SQLLEN* StrLen_or_Ind);

#if (ODBCVER >= 0x0300)
    __declspec(deprecated("ODBC API: SQLBindParam is deprecated. Please use SQLBindParameter instead."))
        SQLRETURN  SQL_API SQLBindParam(SQLHSTMT StatementHandle,            SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,            SQLSMALLINT ParameterType, SQLULEN LengthPrecision,            SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue,            SQLLEN* StrLen_or_Ind);
#endif




    SQLRETURN  SQL_API SQLCancelHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle);

    SQLRETURN  SQL_API SQLCloseCursor(SQLHSTMT StatementHandle);


    SQLRETURN  SQL_API SQLColAttribute(SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
        _Out_writes_bytes_opt_(BufferLength) SQLPOINTER CharacterAttribute, SQLSMALLINT BufferLength,
        _Out_opt_ SQLSMALLINT* StringLength, _Out_opt_ SQLLEN* NumericAttribute);

    SQLRETURN  SQL_API SQLColumns(SQLHSTMT StatementHandle,
        _In_reads_opt_(NameLength1) SQLCHAR* CatalogName, SQLSMALLINT NameLength1,
        _In_reads_opt_(NameLength2) SQLCHAR* SchemaName, SQLSMALLINT NameLength2,
        _In_reads_opt_(NameLength3) SQLCHAR* TableName, SQLSMALLINT NameLength3,
        _In_reads_opt_(NameLength4) SQLCHAR* ColumnName, SQLSMALLINT NameLength4);


    SQLRETURN SQL_API SQLCompleteAsync(SQLSMALLINT    HandleType,
        SQLHANDLE      Handle,
        _Out_ RETCODE* AsyncRetCodePtr);


    SQLRETURN  SQL_API SQLConnect(SQLHDBC ConnectionHandle,
        _In_reads_(NameLength1) SQLCHAR* ServerName, SQLSMALLINT NameLength1,
        _In_reads_(NameLength2) SQLCHAR* UserName, SQLSMALLINT NameLength2,
        _In_reads_(NameLength3) SQLCHAR* Authentication, SQLSMALLINT NameLength3);


    SQLRETURN  SQL_API SQLCopyDesc(SQLHDESC SourceDescHandle, SQLHDESC TargetDescHandle);


    SQLRETURN  SQL_API SQLDataSources(SQLHENV EnvironmentHandle,
        SQLUSMALLINT Direction, _Out_writes_opt_(BufferLength1) SQLCHAR* ServerName,
        SQLSMALLINT BufferLength1, _Out_opt_ SQLSMALLINT* NameLength1Ptr,
        _Out_writes_opt_(BufferLength2) SQLCHAR* Description, SQLSMALLINT BufferLength2,
        _Out_opt_ SQLSMALLINT* NameLength2Ptr);

    SQLRETURN  SQL_API SQLDescribeCol(SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber, _Out_writes_opt_(BufferLength) SQLCHAR* ColumnName,
        SQLSMALLINT BufferLength, _Out_opt_ SQLSMALLINT* NameLength,
        _Out_opt_ SQLSMALLINT* DataType, _Out_opt_ SQLULEN* ColumnSize,
        _Out_opt_ SQLSMALLINT* DecimalDigits, _Out_opt_ SQLSMALLINT* Nullable);

    SQLRETURN  SQL_API SQLDisconnect(SQLHDBC ConnectionHandle);


    SQLRETURN  SQL_API SQLEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType);


    SQLRETURN  SQL_API SQLError(SQLHENV EnvironmentHandle,
        SQLHDBC ConnectionHandle, SQLHSTMT StatementHandle,
        _Out_writes_(6) SQLCHAR* Sqlstate, _Out_opt_ SQLINTEGER* NativeError,
        _Out_writes_opt_(BufferLength) SQLCHAR* MessageText, SQLSMALLINT BufferLength,
        _Out_opt_ SQLSMALLINT* TextLength);

    SQLRETURN  SQL_API SQLExecDirect
    (
        SQLHSTMT StatementHandle,
        _In_reads_opt_(TextLength) SQLCHAR* StatementText,
        SQLINTEGER TextLength
    );

    SQLRETURN  SQL_API SQLExecute(SQLHSTMT StatementHandle);
    SQLRETURN  SQL_API SQLFetch(SQLHSTMT StatementHandle);
    SQLRETURN  SQL_API SQLFetchScroll(SQLHSTMT StatementHandle, SQLSMALLINT FetchOrientation, SQLLEN FetchOffset);
    SQLRETURN  SQL_API SQLFreeConnect(SQLHDBC ConnectionHandle);
    SQLRETURN  SQL_API SQLFreeEnv(SQLHENV EnvironmentHandle);
    SQLRETURN  SQL_API SQLFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle);
    SQLRETURN  SQL_API SQLFreeStmt(SQLHSTMT StatementHandle, SQLUSMALLINT Option);


    SQLRETURN  SQL_API SQLGetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute, _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength, _Out_opt_ SQLINTEGER* StringLengthPtr);


    SQLRETURN  SQL_API SQLGetConnectOption(SQLHDBC ConnectionHandle, SQLUSMALLINT Option, SQLPOINTER Value);

    SQLRETURN  SQL_API SQLGetCursorName
    (
        SQLHSTMT StatementHandle,
        _Out_writes_opt_(BufferLength) SQLCHAR* CursorName,
        SQLSMALLINT BufferLength,
        _Out_opt_
        SQLSMALLINT* NameLengthPtr
    );

    SQLRETURN  SQL_API SQLGetData(SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
        _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER TargetValue, SQLLEN BufferLength,
        _Out_opt_ SQLLEN* StrLen_or_IndPtr);

#if (ODBCVER >= 0x0300)
    SQLRETURN  SQL_API SQLGetDescField(SQLHDESC DescriptorHandle,        SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,        _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength,        _Out_opt_ SQLINTEGER* StringLength);

    SQLRETURN  SQL_API SQLGetDescRec(SQLHDESC DescriptorHandle,
        SQLSMALLINT RecNumber, _Out_writes_opt_(BufferLength) SQLCHAR* Name,
        SQLSMALLINT BufferLength, _Out_opt_ SQLSMALLINT* StringLengthPtr,
        _Out_opt_ SQLSMALLINT* TypePtr, _Out_opt_ SQLSMALLINT* SubTypePtr,
        _Out_opt_ SQLLEN* LengthPtr, _Out_opt_ SQLSMALLINT* PrecisionPtr,
        _Out_opt_ SQLSMALLINT* ScalePtr, _Out_opt_ SQLSMALLINT* NullablePtr);

    SQLRETURN  SQL_API SQLGetDiagField(SQLSMALLINT HandleType, SQLHANDLE Handle,
        SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
        _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER DiagInfo, SQLSMALLINT BufferLength,
        _Out_opt_ SQLSMALLINT* StringLength);

    SQLRETURN  SQL_API SQLGetDiagRec
    (
        SQLSMALLINT HandleType,
        SQLHANDLE Handle,
        SQLSMALLINT RecNumber,
        _Out_writes_opt_(6) SQLCHAR* Sqlstate,
        SQLINTEGER* NativeError,
        _Out_writes_opt_(BufferLength) SQLCHAR* MessageText,
        SQLSMALLINT BufferLength,
        _Out_opt_
        SQLSMALLINT* TextLength
    );

    SQLRETURN  SQL_API SQLGetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute, _Out_writes_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength, _Out_opt_ SQLINTEGER* StringLength);


    SQLRETURN  SQL_API SQLGetFunctions(SQLHDBC ConnectionHandle, SQLUSMALLINT FunctionId, SQLUSMALLINT* Supported);


    SQLRETURN  SQL_API SQLGetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType, _Out_writes_bytes_opt_(BufferLength) SQLPOINTER InfoValue, SQLSMALLINT BufferLength, _Out_opt_ SQLSMALLINT* StringLengthPtr);


    SQLRETURN  SQL_API SQLGetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute, _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength, _Out_opt_ SQLINTEGER* StringLength);


    SQLRETURN  SQL_API SQLGetStmtOption(SQLHSTMT StatementHandle, SQLUSMALLINT Option, SQLPOINTER Value);

    SQLRETURN  SQL_API SQLGetTypeInfo(SQLHSTMT StatementHandle, SQLSMALLINT DataType);

    SQLRETURN  SQL_API SQLNumResultCols(SQLHSTMT StatementHandle, _Out_ SQLSMALLINT* ColumnCount);

    SQLRETURN  SQL_API SQLParamData(SQLHSTMT StatementHandle, _Out_opt_ SQLPOINTER* Value);

    SQLRETURN  SQL_API SQLPrepare(SQLHSTMT StatementHandle, _In_reads_(TextLength) SQLCHAR* StatementText, SQLINTEGER TextLength);

    SQLRETURN  SQL_API SQLPutData(SQLHSTMT StatementHandle, _In_reads_(_Inexpressible_(StrLen_or_Ind)) SQLPOINTER Data, SQLLEN StrLen_or_Ind);

    SQLRETURN  SQL_API SQLRowCount(_In_ SQLHSTMT StatementHandle, _Out_ SQLLEN* RowCount);


    SQLRETURN  SQL_API SQLSetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute, _In_reads_bytes_opt_(StringLength) SQLPOINTER Value, SQLINTEGER StringLength);


    SQLRETURN  SQL_API SQLSetConnectOption(SQLHDBC ConnectionHandle, SQLUSMALLINT Option, SQLULEN Value);

    SQLRETURN  SQL_API SQLSetCursorName(SQLHSTMT StatementHandle, _In_reads_(NameLength) SQLCHAR* CursorName, SQLSMALLINT NameLength);


    SQLRETURN  SQL_API SQLSetDescField(SQLHDESC DescriptorHandle, SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier, _In_reads_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength);

    SQLRETURN  SQL_API SQLSetDescRec(SQLHDESC DescriptorHandle,
        SQLSMALLINT RecNumber, SQLSMALLINT Type,
        SQLSMALLINT SubType, SQLLEN Length,
        SQLSMALLINT Precision, SQLSMALLINT Scale,
        _Inout_updates_bytes_opt_(Length) SQLPOINTER Data, _Inout_opt_ SQLLEN* StringLength,
        _Inout_opt_ SQLLEN* Indicator);

    SQLRETURN  SQL_API SQLSetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute, _In_reads_bytes_opt_(StringLength) SQLPOINTER Value, SQLINTEGER StringLength);


    __declspec(deprecated("ODBC API: SQLSetParam is deprecated. Please use SQLBindParameter instead."))
        SQLRETURN  SQL_API SQLSetParam(SQLHSTMT StatementHandle,
            SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
            SQLSMALLINT ParameterType, SQLULEN LengthPrecision,
            SQLSMALLINT ParameterScale, _In_reads_(_Inexpressible_(*StrLen_or_Ind)) SQLPOINTER ParameterValue,
            _Inout_ SQLLEN* StrLen_or_Ind);

#if (ODBCVER >= 0x0300)
    SQLRETURN  SQL_API SQLSetStmtAttr(SQLHSTMT StatementHandle,
        SQLINTEGER Attribute, _In_reads_(_Inexpressible_(StringLength)) SQLPOINTER Value,
        SQLINTEGER StringLength);
    __declspec(deprecated("ODBC API: SQLSetStmtOption is deprecated. Please use SQLSetStmtAttr instead."))
#endif

        SQLRETURN  SQL_API SQLSetStmtOption(SQLHSTMT StatementHandle, SQLUSMALLINT Option, SQLULEN Value);

    SQLRETURN  SQL_API SQLSpecialColumns(SQLHSTMT StatementHandle,
        SQLUSMALLINT IdentifierType,
        _In_reads_opt_(NameLength1) SQLCHAR* CatalogName, SQLSMALLINT NameLength1,
        _In_reads_opt_(NameLength2) SQLCHAR* SchemaName, SQLSMALLINT NameLength2,
        _In_reads_opt_(NameLength3) SQLCHAR* TableName, SQLSMALLINT NameLength3,
        SQLUSMALLINT Scope, SQLUSMALLINT Nullable);

    SQLRETURN  SQL_API SQLStatistics(SQLHSTMT StatementHandle,
        _In_reads_opt_(NameLength1) SQLCHAR* CatalogName, SQLSMALLINT NameLength1,
        _In_reads_opt_(NameLength2) SQLCHAR* SchemaName, SQLSMALLINT NameLength2,
        _In_reads_opt_(NameLength3) SQLCHAR* TableName, SQLSMALLINT NameLength3,
        SQLUSMALLINT Unique, SQLUSMALLINT Reserved);

    SQLRETURN  SQL_API SQLTables(SQLHSTMT StatementHandle,
        _In_reads_opt_(NameLength1) SQLCHAR* CatalogName, SQLSMALLINT NameLength1,
        _In_reads_opt_(NameLength2) SQLCHAR* SchemaName, SQLSMALLINT NameLength2,
        _In_reads_opt_(NameLength3) SQLCHAR* TableName, SQLSMALLINT NameLength3,
        _In_reads_opt_(NameLength4) SQLCHAR* TableType, SQLSMALLINT NameLength4);

    SQLRETURN  SQL_API SQLTransact(SQLHENV EnvironmentHandle, SQLHDBC ConnectionHandle, SQLUSMALLINT CompletionType);
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif