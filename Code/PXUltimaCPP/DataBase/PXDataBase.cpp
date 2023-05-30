#include "PXDataBase.hpp"

PX::SQLDataBaseConnection::SQLDataBaseConnection()
{
	PXDataBaseConnectionConstruct(this);
}

PX::SQLDataBaseConnection::~SQLDataBaseConnection()
{
	PXDataBaseConnectionDestruct(this);
}

void PX::SQLDataBaseConnection::Connect(const Text& source, const Text& database, const Text& user, const Text& password)
{
	PXDataBaseConnectionConnect(this, &source, &database, &user, &password);
}

void PX::SQLDataBaseConnection::Disconnect()
{
	PXDataBaseConnectionDisconnect(this);
}

void PX::SQLDataBaseConnection::Cleanup()
{
	PXDataBaseConnectionCleanup(this);
}

void PX::SQLDataBaseConnection::ScanForDrivers()
{
	PXDataBaseConnectionScanForDrivers(this);
}

void PX::SQLDataBaseConnection::Execute(const Text& sqlStatement)
{
	PXDataBaseConnectionExecute(this, &sqlStatement);
}