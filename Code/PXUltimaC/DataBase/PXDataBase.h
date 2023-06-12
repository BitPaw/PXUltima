#ifndef PXDataBaseINLCUDE
#define PXDataBaseINLCUDE

#include <OS/System/PXOSVersion.h>

#define PXDataBaseUSE OSDeviceToUse == OSDeviceDestop
#if PXDataBaseUSE

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

	const PXSQLType PXSQLTypeFromID(const PXInt32U sqlTypeID);

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
	PXSQLDataBaseConnection;


	PXPublic void PXDataBaseConnectionConstruct(PXSQLDataBaseConnection* const dataBaseConnection);
	PXPublic void PXDataBaseConnectionDestruct(PXSQLDataBaseConnection* const dataBaseConnection);

	PXPublic PXActionResult PXDataBaseConnectionConnect
	(
		PXSQLDataBaseConnection* const dataBaseConnection,
		const PXText* const source,
		const PXText* const database,
		const PXText* const user,
		const PXText* const password
	);
	PXPublic void PXDataBaseConnectionDisconnect(PXSQLDataBaseConnection* const dataBaseConnection);
	PXPublic void PXDataBaseConnectionCleanup(PXSQLDataBaseConnection* const dataBaseConnection);

	PXPublic void PXDataBaseConnectionScanForDrivers(PXSQLDataBaseConnection* const dataBaseConnection);

	PXPublic PXActionResult PXDataBaseConnectionExecute(PXSQLDataBaseConnection* const dataBaseConnection, const PXText* const PXSQLStatement);

#ifdef __cplusplus
}
#endif

#endif
#endif