#include "PXLDAPClient.hpp"

PX::LDAPClient::LDAPClient()
{
	PXLDAPClientConstruct(this);
}

PX::LDAPClient::~LDAPClient()
{
	PXLDAPClientDestruct(this);
}

PX::ActionResult PX::LDAPClient::Open(PXLDAPConnectionInfo& connectionInfo)
{
	return (PX::ActionResult)PXLDAPClientOpen(this, &connectionInfo);
}

PX::ActionResult PX::LDAPClient::Close()
{
	return (PX::ActionResult)PXLDAPClientClose(this);
}

PX::ActionResult PX::LDAPClient::Search(PXLDAPSearchInfo& searchInfo)
{
	return (PX::ActionResult)PXLDAPClientSearch(this, &searchInfo);
}