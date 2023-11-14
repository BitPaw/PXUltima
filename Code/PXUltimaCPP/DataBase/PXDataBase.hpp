#pragma once

#include <OS/DataBase/PXDataBase.h>
#include "../Media/PXText.hpp"

namespace PX
{
	enum class SQLType
	{
		Invalid,
		Unkown,

		// C Types
		Character, // tinyint
		Integer,
		LongLong, // bigint
		Float,
		Double,
		Short, // smallint

		StringANSI, // char Array
		StringUNICODE, // wchar_t Array


		// SQL - Variable Types
		Numeric,
		Decimal,
		Real, // like float

		DATETIME,

		TYPE_DATE,
		TYPE_TIME,
		TYPE_TIMESTAMP
	};

	class DataBase : public PXDataBase
	{
		public:
		DataBase();
		~DataBase();

		void Connect
		(
			const PX::Text& source,
			const PX::Text& database,
			const PX::Text& user,
			const PX::Text& password
		);
		void Disconnect();
		void Cleanup();

		void ScanForDrivers();

		void Execute(const PX::Text& sqlStatement);
	};
}