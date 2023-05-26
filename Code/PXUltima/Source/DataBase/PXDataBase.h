#ifndef PXDataBaseINLCUDE
#define PXDataBaseINLCUDE

#include <OS/System/OSVersion.h>

#define PXDataBaseUSE OSDeviceToUse == OSDeviceDestop
#if PXDataBaseUSE

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <sqlext.h>  
#include <sqltypes.h>
#endif

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum SQLType_
	{
		SQLTypeInvalid,
		SQLTypeUnkown,

		// C Types
		SQLTypeCharacter, // tinyint
		SQLTypeInteger,
		SQLTypeLongLong, // bigint
		SQLTypeFloat,
		SQLTypeDouble,
		SQLTypeShort, // smallint

		SQLTypeStringANSI, // char Array
		SQLTypeStringUNICODE, // wchar_t Array


		// SQL - Variable Types
		SQLTypeNumeric,
		SQLTypeDecimal,
		SQLTypeReal, // like float

		SQLTypeDATETIME,

		SQLTypeTYPE_DATE,
		SQLTypeTYPE_TIME,
		SQLTypeTYPE_TIMESTAMP
	}
	SQLType;

	const SQLType PXSQLTypeFromID(const unsigned int sqlType);


	typedef enum SQLResult_
	{
		SQLResultInvalid,
		SQLResultSuccessful,
		SQLResultConnectionFailed,
		SQLResultDatabaseNotConnected, // Trying to use a command that requires an already established connection.
		SQLResultMemoryAllocFailed,
		SQLResultMemoryFreeFailed,
		SQLResultSettingEnvironmentAttributeFailed,
		SQLResultExecuteFailed
	}
	SQLResult;

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
			const SQLType type,
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


	typedef struct DataBaseConnection_
	{
#if OSUnix
#elif OSWindows
		SQLHENV _handleEnvironment;
		SQLHDBC _handleConnection;
#endif	

		ConnectedEvent OnConnectedEvent;
		DriverDetectedEvent OnDriverDetectedEvent;
		ResultEvent OnResultEvent;
		ColumInfoEvent OnColumInfoEvent;
		RowDataEvent OnRowDataEvent;
	}
	DataBaseConnection;


	PXPublic void PXDataBaseConnectionConstruct(DataBaseConnection* const dataBaseConnection);
	PXPublic void PXDataBaseConnectionDestruct(DataBaseConnection* const dataBaseConnection);

	PXPublic void PXDataBaseConnectionConnect
	(
		DataBaseConnection* const dataBaseConnection,
		const wchar_t* source,
		const wchar_t* database,
		const wchar_t* user,
		const wchar_t* password
	);
	PXPublic void PXDataBaseConnectionDisconnect(DataBaseConnection* const dataBaseConnection);
	PXPublic void PXDataBaseConnectionCleanup(DataBaseConnection* const dataBaseConnection);

	PXPublic void PXDataBaseConnectionScanForDrivers(DataBaseConnection* const dataBaseConnection);

	PXPublic void PXDataBaseConnectionExecute(DataBaseConnection* const dataBaseConnection, const wchar_t* sqlStatement);

#ifdef __cplusplus
}
#endif

#endif
#endif