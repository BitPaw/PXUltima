#ifndef PXLDAPINCLUDE
#define PXLDAPINCLUDE

#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>

#if OSUnix
#define PXLDAPPortDefault 389
#elif OSWindows
#include <winldap.h> // Wldap32.dll
#define PXLDAPPortDefault LDAP_PORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXLDAPScope_
	{
		PXLDAPScopeInvalid,
		PXLDAPScopeBase,
		PXLDAPScopeOneLevel,
		PXLDAPScopeSubTree,
	}
	PXLDAPScope;

	typedef enum PXLDAPAuthenticationMethod_
	{
		PXLDAPAuthenticationMethodInvalid
	}
	PXLDAPAuthenticationMethod;

	typedef struct PXLDAPClient_
	{
#if OSUnix
		void* ID;
#elif PXOSWindowsDestop
		LDAP* ID;

		LDAPMessage* SearchResult;
#else 
		void* ID;
#endif
	}
	PXLDAPClient;

	typedef struct PXLDAPConnectionInfo_
	{
		// 1=TCP, 0=UDP
		PXBool ConnectionOriented;

		// Init	
		PXText Host;
		PXInt16U Port;
		PXBool SSLUse;

		// connect
		PXInt32U ConnectTimeout;

		// Bind
		PXText ConnectionDomain;
		PXText AuthenticationCredentials;
		PXLDAPAuthenticationMethod AuthenticationMethod;

	}
	PXLDAPConnectionInfo;


	typedef struct PXLDAPSearchInfo_
	{
		PXText EntryName;
		PXText Filter;
		void* AttributeList;
		PXLDAPScope Scope;
		PXBool OnlyTypesRequired;

		PXBool Async;
	}
	PXLDAPSearchInfo;

	PXPublic void PXLDAPClientConstruct(PXLDAPClient* const pxLDAPClient);
	PXPublic void PXLDAPClientDestruct(PXLDAPClient* const pxLDAPClient);

	PXPublic PXActionResult PXLDAPClientOpen(PXLDAPClient* const pxLDAPClient, const PXLDAPConnectionInfo* const pxLDAPConnectionInfo);
	PXPublic PXActionResult PXLDAPClientClose(PXLDAPClient* const pxLDAPClient);

	// Used an open connection to fetch data either syncrounous or asyncounous.
	// The result is fetched into an adress that need to be cleared before the next call.
	PXPublic PXActionResult PXLDAPClientSearch
	(
		PXLDAPClient* const pxLDAPClient,
		PXLDAPSearchInfo* const pxLDAPSearchInfo
	);

#ifdef __cplusplus
}
#endif

#endif