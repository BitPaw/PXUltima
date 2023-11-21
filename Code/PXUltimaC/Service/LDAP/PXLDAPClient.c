#include "PXLDAPClient.h"

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif OSWindows
#pragma comment(lib, "Wldap32.lib")
#endif

void PXLDAPClientConstruct(PXLDAPClient* const pxLDAPClient)
{
	PXClear(PXLDAPClient, pxLDAPClient);
}

void PXLDAPClientDestruct(PXLDAPClient* const pxLDAPClient)
{
	PXLDAPClientClose(pxLDAPClient);
}

PXActionResult PXLDAPClientOpen(PXLDAPClient* const pxLDAPClient, const PXLDAPConnectionInfo* const pxLDAPInfo)
{
	switch (pxLDAPInfo->Host.Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix
			return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop

			if (pxLDAPInfo->ConnectionOriented)
			{
				pxLDAPClient->ID = ldap_sslinitA(pxLDAPInfo->Host.TextA, pxLDAPInfo->Port, pxLDAPInfo->SSLUse); // Windows Vista, Wldap32.dll, winldap.h
			}
			else
			{
				pxLDAPClient->ID = cldap_openA(pxLDAPInfo->Host.TextA, pxLDAPInfo->Port); // Windows Vista, Wldap32.dll, winldap.h
			}

			const PXBool successful = PXNull != pxLDAPClient->ID;

			PXActionOnErrorFetchAndReturn(!successful); // LdapGetLastError()

			break;
#else
			return PXActionNotSupportedByOperatingSystem;
#endif		
		}
		case TextFormatUNICODE:
		{
#if OSUnix
			return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop

			if (pxLDAPInfo->ConnectionOriented)
			{
				pxLDAPClient->ID = ldap_sslinitW(pxLDAPInfo->Host.TextW, pxLDAPInfo->Port, pxLDAPInfo->SSLUse); // Windows Vista, Wldap32.dll, winldap.h
			}
			else
			{
				pxLDAPClient->ID = cldap_openW(pxLDAPInfo->Host.TextW, pxLDAPInfo->Port); // Windows Vista, Wldap32.dll, winldap.h
			}

			const PXBool successful = PXNull != pxLDAPClient->ID;

			PXActionOnErrorFetchAndReturn(!successful); // LdapGetLastError()

			break;
#else
			return PXActionNotSupportedByOperatingSystem;
#endif			
		}

		default:
			return PXActionRefusedFormatNotSupported;
	}

#if OSUnix
#elif PXOSWindowsDestop

#if 1
	// Set options
	{		
		int version = 2;

		//  Set the version to 3.0 (default is 2.0).
		const ULONG optionSetResult = ldap_set_option(pxLDAPClient->ID, LDAP_OPT_PROTOCOL_VERSION, &version);
		const PXBool optionSetSuccessful = LDAP_SUCCESS == optionSetResult;

		//LdapGetLastError();

		PXActionOnErrorFetchAndReturn(!optionSetSuccessful); // LdapGetLastError()
	}
#endif

	// Connect
	{
		LDAP_TIMEVAL ldaptime = { pxLDAPInfo->ConnectTimeout, pxLDAPInfo->ConnectTimeout };

		const ULONG connectResult = ldap_connect(pxLDAPClient->ID, &ldaptime);
		const PXBool connectionSuccessful = LDAP_SUCCESS == connectResult;

		PXActionOnErrorFetchAndReturn(!connectionSuccessful); // LdapGetLastError()
	}

	// Binding
	{
		const ULONG bindResult = ldap_bind_sA
		(
			pxLDAPClient->ID,
			pxLDAPInfo->ConnectionDomain.TextA,
			pxLDAPInfo->AuthenticationCredentials.TextA,
			LDAP_AUTH_SIMPLE
		);
		const PXBool bindSuccessful = LDAP_SUCCESS == bindResult;

		PXActionOnErrorFetchAndReturn(!bindSuccessful); // LdapGetLastError()
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXLDAPClientClose(PXLDAPClient* const pxLDAPClient)
{
	if (!pxLDAPClient->ID)
	{
		return PXActionRefusedObjectNotFound;
	}

#if OSUnix
	return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop
	const ULONG result = ldap_unbind(pxLDAPClient->ID);
	const PXBool successful = LDAP_SUCCESS == result;

	PXActionOnErrorFetchAndReturn(!successful);

	pxLDAPClient->ID = PXNull;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXLDAPClientSearch(PXLDAPClient* const pxLDAPClient, PXLDAPSearchInfo* const pxLDAPSearchInfo)
{
	if (!pxLDAPClient->ID)
	{
		return PXActionRefusedObjectNotFound;
	}

#if OSUnix
	return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop

	ULONG scope = 0;

	switch (pxLDAPSearchInfo->Scope)
	{
		case PXLDAPScopeBase:
		{
			scope = LDAP_SCOPE_BASE;
			break;
		}
		case PXLDAPScopeOneLevel:
		{
			scope = LDAP_SCOPE_ONELEVEL;
			break;
		}
		case PXLDAPScopeSubTree:
		{
			scope = LDAP_SCOPE_SUBTREE;
			break;
		}
		case PXLDAPScopeInvalid:
		default:
			scope = -1;
			break;
	}
#else
	return PXActionNotSupportedByOperatingSystem;
#endif

	switch (pxLDAPSearchInfo->EntryName.Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix
			return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop

			if (pxLDAPSearchInfo->Async)
			{
				const ULONG messageID = ldap_searchA
				(
					pxLDAPClient->ID,
					pxLDAPSearchInfo->EntryName.TextA,
					scope,
					pxLDAPSearchInfo->Filter.TextA,
					pxLDAPSearchInfo->AttributeList,
					pxLDAPSearchInfo->OnlyTypesRequired
				);				

				const PXBool successful = LDAP_SUCCESS == messageID;

				PXActionOnErrorFetchAndReturn(!successful);
			}
			else
			{
				const ULONG searchResult = ldap_search_sA
				(
					pxLDAPClient->ID,
					pxLDAPSearchInfo->EntryName.TextA,
					scope,
					pxLDAPSearchInfo->Filter.TextA,
					pxLDAPSearchInfo->AttributeList,
					pxLDAPSearchInfo->OnlyTypesRequired,
					&pxLDAPClient->SearchResult
				);

				const PXBool successful = LDAP_SUCCESS == searchResult;

				PXActionOnErrorFetchAndReturn(!successful);
			}

			return PXActionSuccessful;
#else
			return PXActionNotSupportedByOperatingSystem;
#endif
		}
		case TextFormatUNICODE:
		{
#if OSUnix
			return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop
			const ULONG messageID = ldap_searchW
			(
				pxLDAPClient->ID,
				pxLDAPSearchInfo->EntryName.TextW,
				scope,
				pxLDAPSearchInfo->Filter.TextW,
				pxLDAPSearchInfo->AttributeList,
				pxLDAPSearchInfo->OnlyTypesRequired
			);
			const PXBool successful = LDAP_SUCCESS == messageID;

			PXActionOnErrorFetchAndReturn(!successful);		

			return PXActionSuccessful;
#else
			return PXActionNotSupportedByOperatingSystem;
#endif
		}

		default:
			return PXActionRefusedFormatNotSupported;
	}

	return PXActionSuccessful;
}