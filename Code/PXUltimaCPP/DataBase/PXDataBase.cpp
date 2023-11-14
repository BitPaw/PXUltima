#include "PXDataBase.hpp"

PX::DataBase::DataBase()
{
	PXDataBaseConstruct(this);
}

PX::DataBase::~DataBase()
{
	PXDataBaseDestruct(this);
}

void PX::DataBase::Connect(const Text& source, const Text& database, const Text& user, const Text& password)
{
	PXDataBaseConnect(this, &source, &database, &user, &password);
}

void PX::DataBase::Disconnect()
{
	PXDataBaseDisconnect(this);
}

void PX::DataBase::Cleanup()
{
	PXDataBaseCleanup(this);
}

void PX::DataBase::ScanForDrivers()
{
	PXDataBaseScanForDrivers(this);
}

void PX::DataBase::Execute(const Text& sqlStatement)
{
	PXDataBaseCommandExecute(this, &sqlStatement);
}