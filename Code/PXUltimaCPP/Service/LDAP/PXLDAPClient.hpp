#pragma once

#include <Service/LDAP/PXLDAPClient.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class LDAPClient : public PXLDAPClient
	{
		public:
		LDAPClient();
		~LDAPClient();

		PX::ActionResult Open(PXLDAPConnectionInfo& connectionInfo);
		PX::ActionResult Close();

		PX::ActionResult Search(PXLDAPSearchInfo& searchInfo);
	};
}