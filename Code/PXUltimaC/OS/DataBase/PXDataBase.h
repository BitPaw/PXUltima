#ifndef PXDataBaseINLCUDE
#define PXDataBaseINLCUDE

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

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


typedef void (PXAPI* ConnectedEvent)(const wchar_t* driver, const wchar_t* source, const wchar_t* database, const wchar_t* user);
typedef void (PXAPI* DriverDetectedEvent)(const wchar_t* driverName, const wchar_t* driverAttribte);
typedef void (PXAPI* ResultEvent)(const PXSize columSize, const PXSize rowSize);
typedef void (PXAPI* ColumInfoEvent)(const PXSize columIndex, const PXSQLType type, const unsigned char canBeNull, const wchar_t* name, const PXSize nameLength);
typedef void (PXAPI* RowDataEvent)(const PXSize rowIndex, const PXSize columIndex, const wchar_t* fieldString);


typedef struct PXDataBase_
{
    PXLibrary ODBCLibrary;

    void* AllocConnect;
    void* AllocEnv;
    void* AllocHandle;
    void* AllocStmt;
    void* BindCol;
    void* BindParam;
    void* Cancel;
    void* CancelHandle;
    void* CloseCursor;
    void* ColAttribute;
    void* Columns;
    void* Connect;
    void* CopyDesc;
    void* DataSources;
    void* DescribeCol;
    void* Disconnect;
    void* EndTran;
    void* Error;
    void* ExecDirect;
    void* Execute;
    void* Fetch;
    void* FetchScroll;
    void* FreeConnect;
    void* FreeEnv;
    void* FreeHandle;
    void* FreeStmt;
    void* GetConnectAttr;
    void* GetConnectOption;
    void* GetCursorName;
    void* GetData;
    void* GetDescField;
    void* GetDescRec;
    void* GetDiagField;
    void* GetDiagRec;
    void* GetEnvAttr;
    void* GetFunctions;
    void* GetInfo;
    void* GetStmtAttr;
    void* GetStmtOption;
    void* GetTypeInfo;
    void* NumResultCols;
    void* ParamData;
    void* Prepare;
    void* PutData;
    void* RowCount;
    void* SetConnectAttr;
    void* SetConnectOption;
    void* SetCursorName;
    void* SetDescField;
    void* SetDescRec;
    void* SetEnvAttr;
    void* SetParam;
    void* SetStmtAttr;
    void* SetStmtOption;
    void* SpecialColumns;
    void* Statistics;
    void* Tables;
    void* Transact;

    void* ColAttributeW;
    void* ColAttributesW;
    void* ConnectW;
    void* DescribeColW;
    void* ErrorW;
    void* ExecDirectW;
    void* GetConnectAttrW;
    void* GetCursorNameW;
    void* SetDescFieldW;
    void* GetDescFieldW;
    void* GetDescRecW;
    void* GetDiagFieldW;
    void* GetDiagRecW;
    void* PrepareW;
    void* SetConnectAttrW;
    void* SetCursorNameW;
    void* ColumnsW;
    void* GetConnectOptionW;
    void* GetInfoW;
    void* GetTypeInfoW;
    void* SetConnectOptionW;
    void* SpecialColumnsW;
    void* StatisticsW;
    void* TablesW;
    void* DataSourcesW;
    void* DriverConnectW;
    void* BrowseConnectW;
    void* ColumnPrivilegesW;
    void* GetStmtAttrW;
    void* SetStmtAttrW;
    void* ForeignKeysW;
    void* NativeSqlW;
    void* PrimaryKeysW;
    void* ProcedureColumnsW;
    void* ProceduresW;
    void* TablePrivilegesW;
    void* DriversW;
    void* ColAttributeA;
    void* ColAttributesA;
    void* ConnectA;
    void* DescribeColA;
    void* ErrorA;
    void* ExecDirectA;
    void* GetConnectAttrA;
    void* GetCursorNameA;
    void* GetDescFieldA;
    void* GetDescRecA;
    void* GetDiagFieldA;
    void* GetDiagRecA;
    void* GetStmtAttrA;
    void* GetTypeInfoA;
    void* PrepareA;
    void* SetConnectAttrA;
    void* SetCursorNameA;
    void* ColumnsA;
    void* GetConnectOptionA;
    void* GetInfoA;
    void* GetStmtOptionA;
    void* SetConnectOptionA;
    void* SetStmtOptionA;
    void* SpecialColumnsA;
    void* StatisticsA;
    void* TablesA;
    void* DataSourcesA;
    void* DriverConnectA;
    void* BrowseConnectA;
    void* ColumnPrivilegesA;
    void* DescribeParamA;
    void* ForeignKeysA;
    void* NativeSqlA;
    void* PrimaryKeysA;
    void* ProcedureColumnsA;
    void* ProceduresA;
    void* TablePrivilegesA;
    void* DriversA;

#if OSUnix
#elif PXOSWindowsDestop
    void* EnvironmentID;
    void* ConnectionID;
#endif	

    char* DriverListData;
    char** DriverList; // Its a single line of text only beeing seperated by a \0
    PXSize DriverListAmount;

    ConnectedEvent OnConnectedEvent;
    DriverDetectedEvent OnDriverDetectedEvent;
    ResultEvent OnResultEvent;
    ColumInfoEvent OnColumInfoEvent;
    RowDataEvent OnRowDataEvent;
}
PXDataBase;

PXPrivate PXSQLType PXAPI PXSQLTypeFromID(const PXInt32U sqlTypeID);

PXPublic PXActionResult PXAPI PXDataBaseInitialize(PXDataBase* const pxDataBase);
PXPublic PXActionResult PXAPI PXDataBaseRelease(PXDataBase* const pxDataBase);

PXPublic PXActionResult PXAPI PXDataBaseConnectA
(
    PXDataBase* const pxDataBase,
    const char* const source,
    const char* const database,
    const char* const user,
    const char* const password
);
PXPublic PXActionResult PXAPI PXDataBaseConnect
(
    PXDataBase* const pxDataBase,
    const PXText* const source,
    const PXText* const database,
    const PXText* const user,
    const PXText* const password
);
PXPublic void PXAPI PXDataBaseDisconnect(PXDataBase* const pxDataBase);
PXPublic void PXAPI PXDataBaseCleanup(PXDataBase* const pxDataBase);
PXPublic void PXAPI PXDataBaseScanForDrivers(PXDataBase* const pxDataBase);
PXPublic PXActionResult PXAPI PXDataBaseCommandExecute(PXDataBase* const pxDataBase, const PXText* const PXSQLStatement);
PXPublic PXActionResult PXAPI PXDataBaseCommandCancel(PXDataBase* const pxDataBase);

#endif