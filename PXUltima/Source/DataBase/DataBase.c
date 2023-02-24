#include "DataBase.h"

#include <OS/Memory/PXMemory.h>

#include <stdio.h>

const SQLType SQLTypeFromID(const unsigned int sqlType)
{
#if OSUnix
    return SQLTypeInvalid;

#elif OSWindows
    switch (sqlType)
    {
        // Mysql?
    case -9: return SQLTypeStringUNICODE;

        // is this thenCorrect?
    case SQL_VARCHAR:return SQLTypeStringANSI;

    case SQL_UNKNOWN_TYPE: return SQLTypeUnkown;
    case SQL_CHAR:return SQLTypeCharacter;
    case SQL_NUMERIC:return SQLTypeNumeric;
    case SQL_DECIMAL:return SQLTypeDecimal;
    case SQL_INTEGER:return SQLTypeInteger;
    case SQL_SMALLINT:return SQLTypeShort;
    case SQL_FLOAT:return SQLTypeFloat;
    case SQL_REAL:return SQLTypeReal;
    case SQL_DOUBLE:return SQLTypeDouble;
#if (ODBCVER >= 0x0300)
    case  SQL_DATETIME:return SQLTypeDATETIME;
#endif


        /* One-parameter shortcuts for date/time data types */
#if (ODBCVER >= 0x0300)
    case SQL_TYPE_DATE:return SQLTypeTYPE_DATE;
    case SQL_TYPE_TIME:return SQLTypeTYPE_TIME;
    case SQL_TYPE_TIMESTAMP:return SQLTypeTYPE_TIMESTAMP;
#endif

    default:
        return SQLTypeInvalid;
    }
#endif
}

void DataBaseConnectionConstruct(DataBaseConnection* const dataBaseConnection)
{
    MemoryClear(dataBaseConnection, sizeof(DataBaseConnection));
}

void DataBaseConnectionDestruct(DataBaseConnection* const dataBaseConnection)
{
    DataBaseConnectionCleanup(dataBaseConnection);
}

void DataBaseConnectionConnect
(
    DataBaseConnection* const dataBaseConnection,
    const wchar_t* source,
    const wchar_t* database,
    const wchar_t* user,
    const wchar_t* password
)
{

#if OSUnix
    return SQLResultInvalid;

#elif OSWindows
    // Allocate environment handle
    {
        const SQLRETURN result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &dataBaseConnection->_handleEnvironment);
        const unsigned char successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return SQLResultMemoryAllocFailed;
        }
    }

    // Set the ODBC version environment attribute
    {
        const SQLRETURN result = SQLSetEnvAttr(dataBaseConnection->_handleEnvironment, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        const unsigned char successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return SQLResultSettingEnvironmentAttributeFailed;
        }
    }

    // Allocate connection handle
    {
        const SQLRETURN result = SQLAllocHandle(SQL_HANDLE_DBC, dataBaseConnection->_handleEnvironment, &dataBaseConnection->_handleConnection);
        const unsigned char successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return SQLResultMemoryAllocFailed;
        }
    }

    // Set login timeout to 5 seconds
    {
        const SQLRETURN resultConnect = SQLSetConnectAttrW(dataBaseConnection->_handleConnection, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
        const unsigned char successful = resultConnect == SQL_SUCCESS || resultConnect == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return SQLResultInvalid;
        }
    }

    printf("+---[ODBC Drivers]-------------------------------------------+--------------+\n");

    DataBaseConnectionScanForDrivers(dataBaseConnection);

    printf("+------------------------------------------------------------+--------------+\n");

    const wchar_t* driver = L"MySQL ODBC 8.0 Unicode Driver";

    // Connect
    {
        SQLWCHAR connectionString[2048];
        SQLWCHAR connectionStringResult[2048];
        const SQLSMALLINT connectionStringResultSize = sizeof(connectionStringResult) / sizeof(SQLWCHAR);
        SQLSMALLINT connectionStringResultWrittenSize = 0;
        const PXSize connectionStringSize = wsprintf
        (
            connectionString,
            L"Driver={%ls};Server=%ls;Database=%ls;UID=%ls;PWD=%ls;",
            driver,
            source,
            database,
            user,
            password
        );

        const SQLRETURN resultDriverConnect = SQLDriverConnectW
        (
            dataBaseConnection->_handleConnection,
            NULL,
            connectionString,
            connectionStringSize,
            connectionStringResult,
            connectionStringResultSize,
            &connectionStringResultWrittenSize,
            SQL_DRIVER_NOPROMPT
        );

        const unsigned char successfulConnected = resultDriverConnect == SQL_SUCCESS || resultDriverConnect == SQL_SUCCESS_WITH_INFO;

        if (!successfulConnected)
        {
            return SQLResultConnectionFailed;
        }
    }

    if (dataBaseConnection->OnConnectedEvent)
    {
        dataBaseConnection->OnConnectedEvent(driver, source, database, user);
    }

    return SQLResultSuccessful;

    /*
    SQLWCHAR* serverNameS = (wchar_t*)serverName;
    const SQLSMALLINT serverNameLength = SQL_NTSL;
    SQLWCHAR* userName = NULL;
    const SQLSMALLINT userNameLength = 0;
    SQLWCHAR* password = NULL;
    const SQLSMALLINT passwordLength = 0;

    retcode = SQLConnectW
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
        SQLUSMALLINT infoType = SQL_PROCEDURES;
        wchar_t bufferLength[1024];
        SQLSMALLINT   cbInfoValueMax = sizeof(bufferLength);
        SQLSMALLINT pcbInfoValue = 0;

        SQLRETURN infoResult = SQLGetInfoW
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

void DataBaseConnectionDisconnect(DataBaseConnection* const dataBaseConnection)
{

#if OSUnix
#elif OSWindows
    if (dataBaseConnection->_handleConnection != 0)
    {
        const SQLRETURN resultDisconnect = SQLDisconnect(dataBaseConnection->_handleConnection);
        const SQLRETURN resultFree = SQLFreeHandle(SQL_HANDLE_DBC, dataBaseConnection->_handleConnection);

        dataBaseConnection->_handleConnection = 0;
    }
#endif
}

void DataBaseConnectionCleanup(DataBaseConnection* const dataBaseConnection)
{
    DataBaseConnectionDisconnect(dataBaseConnection);

#if OSUnix
#elif OSWindows
    if (dataBaseConnection->_handleEnvironment != 0)
    {
        const SQLRETURN result = SQLFreeHandle(SQL_HANDLE_ENV, dataBaseConnection->_handleEnvironment);
    }
#endif
}

void DataBaseConnectionScanForDrivers(DataBaseConnection* const dataBaseConnection)
{

#if OSUnix
#elif OSWindows
    SQLUSMALLINT direction = SQL_FETCH_FIRST;
    unsigned char finished = 0;

    while (!finished)
    {
        wchar_t driverDescription[1024];
        const SQLSMALLINT driverDescriptionSize = sizeof(driverDescription) / sizeof(wchar_t);
        SQLSMALLINT driverDescriptionSizeWritten = 0;

        wchar_t driverAttributes[1024];
        const SQLSMALLINT driverAttributesSize = sizeof(driverAttributes) / sizeof(wchar_t);
        SQLSMALLINT driverAttributesSizeWritten = 0;

        const SQLRETURN resultDriver = SQLDriversW
        (
            dataBaseConnection->_handleEnvironment,
            direction,
            driverDescription,
            driverDescriptionSize,
            &driverDescriptionSizeWritten,
            driverAttributes,
            driverAttributesSize,
            &driverAttributesSizeWritten
        );

        switch (resultDriver)
        {
        case SQL_SUCCESS_WITH_INFO:
        case SQL_SUCCESS: // Do nothing and go next
            direction = SQL_FETCH_NEXT; // [!] Important [!] - Mark to go next.

            if (dataBaseConnection->OnDriverDetectedEvent)
            {
                dataBaseConnection->OnDriverDetectedEvent(driverDescription, driverAttributes);
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
}

void DataBaseConnectionExecute(DataBaseConnection* const dataBaseConnection, const wchar_t* sqlStatement)
{

#if OSUnix
#elif OSWindows
    SQLHSTMT handleStatement = 0;
    PXSize colums = 0;
    PXSize rows = 0;

    // Check connection
    {
        const unsigned char connected = dataBaseConnection->_handleConnection != 0;

        if (!connected)
        {
            return SQLResultDatabaseNotConnected;
        }
    }

    // Alloc statement
    {
        const SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_STMT, dataBaseConnection->_handleConnection, &handleStatement);
        const unsigned char sucessfulAlloc = retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO;

        if (!sucessfulAlloc)
        {
            return SQLResultMemoryAllocFailed;
        }
    }

    // Execute command
    {
        const SQLRETURN resultExecute = SQLExecDirectW(handleStatement, (SQLWCHAR*)sqlStatement, SQL_NTSL);
        const unsigned char successful = resultExecute == SQL_SUCCESS || resultExecute == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return SQLResultExecuteFailed;
        }
    }

    // Get table size
    {
        SQLSMALLINT nCols = 0;
        SQLLEN nRows = 0;

        SQLNumResultCols(handleStatement, &nCols);
        SQLRowCount(handleStatement, &nRows);

        colums = nCols;
        rows = nRows;
    }

    if (dataBaseConnection->OnResultEvent)
    {
        dataBaseConnection->OnResultEvent(colums, rows);
    }

    printf("[Result] Rows:%zi Colums:%zi\n", rows, colums);

    printf("+----------------------+----------------------+----------------------+----------------------+\n");

    printf("|");

    for (PXSize columIndex = 1; columIndex <= colums; ++columIndex)
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

        const SQLRETURN resultDescriped = SQLDescribeColW
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

        const SQLType type = SQLTypeFromID(DataType);

        //SQLColAttribute();

        //isBaseType = DataType == SQL_COLUMN_DISTINCT_TYPE;
        isNullable = isNullableID == SQL_NULLABLE; // SQL_NO_NULLS

        const char* typeName = 0;

        switch (type)
        {
        default:
        case SQLTypeInvalid: typeName = "Invalid"; break;
        case SQLTypeUnkown: typeName = "Unkown"; break;
        case SQLTypeCharacter: typeName = "Character"; break;
        case SQLTypeNumeric:typeName = "Numeric"; break;
        case SQLTypeDecimal:typeName = "Decimal"; break;
        case SQLTypeInteger:typeName = "Integer"; break;
        case SQLTypeShort:typeName = "Short"; break;
        case SQLTypeFloat:typeName = "Invalid"; break;
        case SQLTypeReal:typeName = "Real"; break;
        case SQLTypeDouble:typeName = "Double"; break;
        case SQLTypeDATETIME:typeName = "DATETIME"; break;
        case SQLTypeStringANSI:typeName = "StringANSI"; break;
        case SQLTypeStringUNICODE:typeName = "StringUNICODE"; break;
        case SQLTypeTYPE_DATE:typeName = "TYPE_DATE"; break;
        case SQLTypeTYPE_TIME:typeName = "TYPE_TIME"; break;
        case SQLTypeTYPE_TIMESTAMP:typeName = "TYPE_TIMESTAMP"; break;
        }

        if (dataBaseConnection->OnColumInfoEvent)
        {
            dataBaseConnection->OnColumInfoEvent(columIndex, type, isNullable, columnName, columnNameSizeWritten);
        }

        printf(" %-20ls |", columnName);
    }

    printf("\n");
    printf("+----------------------+----------------------+----------------------+----------------------+\n");

    SQLRETURN resultFetch = 0;
    for (PXSize rowIndex = 0; SQL_SUCCEEDED(resultFetch = SQLFetch(handleStatement)); ++rowIndex)
    {
        printf("|");

        for (PXSize columIndex = 1; columIndex <= colums; ++columIndex)
        {
            wchar_t fieldBuffer[1048];
            const PXSize fieldBufferSize = sizeof(fieldBuffer) / sizeof(wchar_t);
            SQLLEN readLength = 0;

            const SQLRETURN resultData = SQLGetData(handleStatement, columIndex, SQL_UNICODE_CHAR, fieldBuffer, fieldBufferSize, &readLength);
            const unsigned char success = SQL_SUCCEEDED(resultData);

            if (success)
            {
                if (dataBaseConnection->OnRowDataEvent)
                {
                    dataBaseConnection->OnRowDataEvent(rowIndex, columIndex, fieldBuffer);
                }

                printf(" %-20ls |", fieldBuffer);
            }
        }

        printf("\n");
    }

    printf("+----------------------+----------------------+----------------------+----------------------+\n");

    // Free memory
    {
        const SQLRETURN resultFree = SQLFreeHandle(SQL_HANDLE_STMT, handleStatement);
        const unsigned char successful = resultFree == SQL_SUCCESS || resultFree == SQL_SUCCESS_WITH_INFO;

        if (successful)
        {
            return SQLResultMemoryFreeFailed;
        }
    }

    return SQLResultSuccessful;
#endif
}
