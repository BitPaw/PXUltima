#include "PXDataBase.h"

#if PXDataBaseUSE

#include <OS/Memory/PXMemory.h>

#include <stdio.h>

const PXSQLType PXSQLTypeFromID(const PXInt32U sqlTypeID)
{
#if OSUnix
    return PXSQLTypeInvalid;

#elif OSWindows
    switch (sqlTypeID)
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
#endif
}

void PXDataBaseConnectionConstruct(PXSQLDataBaseConnection* const dataBaseConnection)
{
    PXMemoryClear(dataBaseConnection, sizeof(PXSQLDataBaseConnection));
}

void PXDataBaseConnectionDestruct(PXSQLDataBaseConnection* const dataBaseConnection)
{
    PXDataBaseConnectionCleanup(dataBaseConnection);
}

PXActionResult PXDataBaseConnectionConnect
(
    PXSQLDataBaseConnection* const dataBaseConnection,
    const PXText* const source,
    const PXText* const database,
    const PXText* const user,
    const PXText* const password
)
{
#if OSUnix
    return PXActionNotImplemented;

#elif OSWindows
    // Allocate environment handle
    {
        const SQLRETURN result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &dataBaseConnection->EnvironmentID);
        const PXBool successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return PXActionFailedMemoryAllocation;
        }
    }

    // Set the ODBC version environment attribute
    {
        const SQLRETURN result = SQLSetEnvAttr(dataBaseConnection->EnvironmentID, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        const PXBool successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return PXActionFailedSettingsInvalid;;
        }
    }

    // Allocate connection handle
    {
        const SQLRETURN result = SQLAllocHandle(SQL_HANDLE_DBC, dataBaseConnection->EnvironmentID, &dataBaseConnection->ConnectionID);
        const PXBool successful = result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return PXActionFailedMemoryAllocation;
        }
    }

    // Set login timeout to 5 seconds
    {
        const SQLRETURN resultConnect = SQLSetConnectAttrW(dataBaseConnection->ConnectionID, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
        const PXBool successful = resultConnect == SQL_SUCCESS || resultConnect == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return PXActionFailedSettingsInvalid;
        }
    }

    printf("+---[ODBC Drivers]-------------------------------------------+--------------+\n");

    PXDataBaseConnectionScanForDrivers(dataBaseConnection);

    printf("+------------------------------------------------------------+--------------+\n");

  

    // Connect
    {
        PXText pxTextSQLConnectionString;
        PXTextConstructNamedBufferA(&pxTextSQLConnectionString, pxTextSQLConnectionStringBuffer, 1024u);

        PXText pxTextSQLConnectionStringResult;
        PXTextConstructNamedBufferA(&pxTextSQLConnectionStringResult, pxTextSQLConnectionStringResultBuffer, 1024u);

        SQLRETURN resultDriverConnect = 0;

        switch (source->Format)
        {
            case TextFormatASCII:
            case TextFormatUTF8:
            {
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

                resultDriverConnect = SQLDriverConnectA
                (
                    dataBaseConnection->ConnectionID,
                    NULL,
                    pxTextSQLConnectionString.TextA,
                    pxTextSQLConnectionString.SizeUsed,
                    pxTextSQLConnectionStringResult.TextA,
                    pxTextSQLConnectionStringResult.SizeAllocated,
                    &writtenSize,
                    SQL_DRIVER_NOPROMPT
                );

                pxTextSQLConnectionStringResult.SizeUsed = writtenSize;

                break;
            }
            case TextFormatUNICODE:
            {
                const wchar_t driver[] = L"MySQL ODBC 8.0 Unicode Driver";

                pxTextSQLConnectionString.SizeUsed = wsprintf
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

                resultDriverConnect = SQLDriverConnectW
                (
                    dataBaseConnection->ConnectionID,
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

        if (!successfulConnected)
        {
            return PXActionFailedConnectionFailure;
        }
    }

    if (dataBaseConnection->OnConnectedEvent)
    {
       // dataBaseConnection->OnConnectedEvent(driver, source, database, user);
    }

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
        PXSQLSMALLINT   cbInfoValueMax = sizeof(bufferLength);
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

void PXDataBaseConnectionDisconnect(PXSQLDataBaseConnection* const dataBaseConnection)
{
#if OSUnix
#elif OSWindows
    if (dataBaseConnection->ConnectionID)
    {
        const SQLRETURN resultDisconnect = SQLDisconnect(dataBaseConnection->ConnectionID);
        const SQLRETURN resultFree = SQLFreeHandle(SQL_HANDLE_DBC, dataBaseConnection->ConnectionID);

        dataBaseConnection->ConnectionID = PXNull;
    }
#endif
}

void PXDataBaseConnectionCleanup(PXSQLDataBaseConnection* const dataBaseConnection)
{
    PXDataBaseConnectionDisconnect(dataBaseConnection);

#if OSUnix
#elif OSWindows
    if (dataBaseConnection->EnvironmentID)
    {
        const SQLRETURN result = SQLFreeHandle(SQL_HANDLE_ENV, dataBaseConnection->EnvironmentID);

        dataBaseConnection->EnvironmentID = PXNull;
    }
#endif
}

void PXDataBaseConnectionScanForDrivers(PXSQLDataBaseConnection* const dataBaseConnection)
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
            dataBaseConnection->EnvironmentID,
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

void PXDataBaseConnectionExecute(PXSQLDataBaseConnection* const dataBaseConnection, const PXText* const pxTextSQLStatement)
{

#if OSUnix
#elif OSWindows
    SQLHSTMT handleStatement = 0;
    PXSize colums = 0;
    PXSize rows = 0;

    // Check connection
    {
        const PXBool connected = dataBaseConnection->ConnectionID != 0;

        if (!connected)
        {
            return PXActionRefusedNotConnected;
        }
    }

    // Alloc statement
    {
        const SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_STMT, dataBaseConnection->ConnectionID, &handleStatement);
        const PXBool sucessfulAlloc = retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO;

        if (!sucessfulAlloc)
        {
            return PXActionFailedMemoryAllocation;
        }
    }

    // Execute command
    {
        SQLRETURN resultExecute = 0;

        switch (pxTextSQLStatement->Format)
        {
            case TextFormatASCII:
            case TextFormatUTF8:
            {
                resultExecute = SQLExecDirectA(handleStatement, pxTextSQLStatement->TextA, SQL_NTS);
                break;
            }
            case TextFormatUNICODE:
            {
                resultExecute = SQLExecDirectW(handleStatement, pxTextSQLStatement->TextW, SQL_NTSL);
            }       

            default:
                return PXActionRefuedTextFormatUnsupported;
        }

        const PXBool successful = resultExecute == SQL_SUCCESS || resultExecute == SQL_SUCCESS_WITH_INFO;

        if (!successful)
        {
            return PXActionFailedCommandExecute;
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

        const PXSQLType type = PXSQLTypeFromID(DataType);

        //SQLColAttribute();

        //isBaseType = DataType == PXSQL_COLUMN_DISTINCT_TYPE;
        isNullable = isNullableID == SQL_NULLABLE; // PXSQL_NO_NULLS

        const char* typeName = 0;

        switch (type)
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
            const PXBool success = SQL_SUCCEEDED(resultData);

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
        const PXBool successful = resultFree == SQL_SUCCESS || resultFree == SQL_SUCCESS_WITH_INFO;

        if (successful)
        {
            return PXActionFailedMemoryRelease;
        }
    }

    return PXActionSuccessful;
#endif
}

#endif