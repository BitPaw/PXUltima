#include "PXLDAPClient.h"

#include <OS/Memory/PXMemory.h>

#define PXLDAPPortDefault 389 // LDAP_PORT

#if OSUnix
#elif OSWindows
#include <winldap.h> // Wldap32.dll
#include <winber.h>
#define PXLDAPPortDefault LDAP_PORT
#pragma comment(lib, "Wldap32.lib")
#endif

typedef LDAP* (LDAPAPI* PXldap_openW)( const PWCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXldap_openA)( const PCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXldap_initW)( const PWCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXldap_initA)( const PCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXldap_sslinitW)( PWCHAR HostName, ULONG PortNumber, int secure);
typedef LDAP* (LDAPAPI* PXldap_sslinitA)( PCHAR HostName, ULONG PortNumber, int secure);
typedef ULONG (LDAPAPI* PXldap_connect)(LDAP* ld, struct l_timeval* timeout);
typedef LDAP* (LDAPAPI* PXldap_open)( PCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXldap_init)( PCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXldap_sslinit)( PCHAR HostName, ULONG PortNumber, int secure);
typedef LDAP* (LDAPAPI* PXcldap_openW)( PWCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXcldap_openA)( PCHAR HostName, ULONG PortNumber);
typedef LDAP* (LDAPAPI* PXcldap_open)( PCHAR HostName, ULONG PortNumber);
typedef ULONG (LDAPAPI* PXldap_unbind)(LDAP* ld);
typedef ULONG (LDAPAPI* PXldap_unbind_s)(LDAP* ld); // calls ldap_unbind
typedef ULONG (LDAPAPI* PXldap_get_optionA)(LDAP* ld, int option, void* outvalue);
typedef ULONG (LDAPAPI* PXldap_get_optionW)(LDAP* ld, int option, void* outvalue);
typedef ULONG (LDAPAPI* PXldap_set_optionA)(LDAP* ld, int option, const void* invalue);
typedef ULONG (LDAPAPI* PXldap_set_optionW)(LDAP* ld, int option, const void* invalue);
typedef ULONG (LDAPAPI* PXldap_simple_bindW)(LDAP* ld,  PWCHAR dn,  PWCHAR passwd);
typedef ULONG (LDAPAPI* PXldap_simple_bindA)(LDAP* ld,  PCHAR dn,  PCHAR passwd);
typedef ULONG (LDAPAPI* PXldap_simple_bind_sW)(LDAP* ld,  PWCHAR dn,  PWCHAR passwd);
typedef ULONG (LDAPAPI* PXldap_simple_bind_sA)(LDAP* ld,  PCHAR dn,  PCHAR passwd);
typedef ULONG (LDAPAPI* PXldap_bindW)(LDAP* ld,  PWCHAR dn,  PWCHAR cred, ULONG method);
typedef ULONG (LDAPAPI* PXldap_bindA)(LDAP* ld,  PCHAR dn,  PCHAR cred, ULONG method);
typedef ULONG (LDAPAPI* PXldap_bind_sW)(LDAP* ld,  PWCHAR dn,  PWCHAR cred, ULONG method);
typedef ULONG (LDAPAPI* PXldap_bind_sA)(LDAP* ld,  PCHAR dn,  PCHAR cred, ULONG method);
typedef INT (LDAPAPI* PXldap_sasl_bindA)(LDAP* ExternalHandle,  const PCHAR DistName,  const PCHAR AuthMechanism, const BERVAL* cred, PLDAPControlA* ServerCtrls, PLDAPControlA* ClientCtrls, int* MessageNumber);
typedef INT (LDAPAPI* PXldap_sasl_bindW)(LDAP* ExternalHandle,  const PWCHAR DistName,  const PWCHAR AuthMechanism, const BERVAL* cred, PLDAPControlW* ServerCtrls, PLDAPControlW* ClientCtrls, int* MessageNumber);
typedef INT (LDAPAPI* PXldap_sasl_bind_sA)(LDAP* ExternalHandle,  const PCHAR DistName,  const PCHAR AuthMechanism, const BERVAL* cred, PLDAPControlA* ServerCtrls, PLDAPControlA* ClientCtrls, PBERVAL* ServerData);
typedef INT (LDAPAPI* PXldap_sasl_bind_sW)(LDAP* ExternalHandle,  const PWCHAR DistName,  const PWCHAR AuthMechanism, const BERVAL* cred, PLDAPControlW* ServerCtrls, PLDAPControlW* ClientCtrls, PBERVAL* ServerData);
typedef ULONG (LDAPAPI* PXldap_simple_bind)(LDAP* ld,  const PCHAR dn,  const PCHAR passwd);
typedef ULONG (LDAPAPI* PXldap_simple_bind_s)(LDAP* ld,  const PCHAR dn,  const PCHAR passwd);
typedef ULONG (LDAPAPI* PXldap_bind)(LDAP* ld,  const PCHAR dn,  const PCHAR cred, ULONG method);
typedef ULONG (LDAPAPI* PXldap_bind_s)(LDAP* ld,  const PCHAR dn,  const PCHAR cred, ULONG method);
typedef ULONG (LDAPAPI* PXldap_searchW)(LDAP* ld,  const PWCHAR base, ULONG scope,  const PWCHAR filter,  PWCHAR attrs[], ULONG attrsonly);
typedef ULONG (LDAPAPI* PXldap_searchA)(LDAP* ld,  const PCHAR base, ULONG scope,  const PCHAR filter,  PCHAR attrs[], ULONG attrsonly);
typedef ULONG (LDAPAPI* PXldap_search_sW)(LDAP* ld,  const PWCHAR base, ULONG scope,  const PWCHAR filter,  PWCHAR attrs[], ULONG attrsonly, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_sA)(LDAP* ld,  const PCHAR base, ULONG scope,  const PCHAR filter,  PCHAR attrs[], ULONG attrsonly, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_stW)(LDAP* ld,  const PWCHAR base, ULONG scope,  const PWCHAR filter,  PWCHAR attrs[], ULONG attrsonly, struct l_timeval* timeout, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_stA)(LDAP* ld,  const PCHAR base, ULONG scope,  const PCHAR filter,  PCHAR attrs[], ULONG attrsonly, struct l_timeval* timeout, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_extW)(LDAP* ld,  const PWCHAR base, ULONG scope,  const PWCHAR filter,  PWCHAR attrs[], ULONG attrsonly, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG TimeLimit, ULONG SizeLimit, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_search_extA)(LDAP* ld,  const PCHAR base, ULONG scope,  const PCHAR filter,  PCHAR attrs[], ULONG attrsonly, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG TimeLimit, ULONG SizeLimit, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_search_ext_sW)(LDAP* ld,  const PWCHAR base,  ULONG scope,  const PWCHAR filter,  PWCHAR attrs[], ULONG attrsonly, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, struct l_timeval* timeout, ULONG SizeLimit, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_ext_sA)(LDAP* ld,  const PCHAR base, ULONG scope,  const PCHAR filter,  PCHAR attrs[], ULONG attrsonly, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, struct l_timeval* timeout, ULONG SizeLimit, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search)(LDAP* ld,  PCHAR base, ULONG scope,  PCHAR filter,  PCHAR attrs[], ULONG attrsonly);
typedef ULONG (LDAPAPI* PXldap_search_s)(LDAP* ld,  PCHAR base, ULONG scope,  PCHAR filter,  PCHAR attrs[], ULONG attrsonly, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_st)(LDAP* ld,  PCHAR base, ULONG scope,  PCHAR filter,  PCHAR attrs[], ULONG attrsonly, struct l_timeval* timeout, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_search_ext)(LDAP* ld, PCHAR base, ULONG scope, PCHAR filter, PCHAR attrs[], ULONG attrsonly, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG TimeLimit, ULONG SizeLimit, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_search_ext_s)(LDAP* ld, PCHAR base, ULONG scope, PCHAR filter, PCHAR attrs[], ULONG attrsonly, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, struct l_timeval* timeout, ULONG SizeLimit, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_check_filterW)(LDAP* ld,  PWCHAR SearchFilter);
typedef ULONG (LDAPAPI* PXldap_check_filterA)(LDAP* ld,  PCHAR SearchFilter);
typedef ULONG (LDAPAPI* PXldap_modifyW)(LDAP* ld,  PWCHAR dn, LDAPModW* mods[]);
typedef ULONG (LDAPAPI* PXldap_modifyA)(LDAP* ld,  PCHAR dn, LDAPModA* mods[]);
typedef ULONG (LDAPAPI* PXldap_modify_sW)(LDAP* ld,  PWCHAR dn, LDAPModW* mods[]);
typedef ULONG (LDAPAPI* PXldap_modify_sA)(LDAP* ld,  PCHAR dn, LDAPModA* mods[]);
typedef ULONG (LDAPAPI* PXldap_modify_extW)(LDAP* ld,  const PWCHAR dn, LDAPModW* mods[], PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_modify_extA)(LDAP* ld,  const PCHAR dn, LDAPModA* mods[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_modify_ext_sW)(LDAP* ld,  const PWCHAR dn, LDAPModW* mods[], PLDAPControlW* ServerControls, PLDAPControlW* ClientControls);
typedef ULONG (LDAPAPI* PXldap_modify_ext_sA)(LDAP* ld,  const PCHAR dn, LDAPModA* mods[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_modify)(LDAP* ld,  PCHAR dn, LDAPModA* mods[]);
typedef ULONG (LDAPAPI* PXldap_modify_s)(LDAP* ld,  PCHAR dn, LDAPModA* mods[]);
typedef ULONG (LDAPAPI* PXldap_modify_ext)(LDAP* ld, const PCHAR dn, LDAPModA* mods[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_modify_ext_s)(LDAP* ld, const PCHAR dn, LDAPModA* mods[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_modrdn2W)(LDAP* ExternalHandle,  const PWCHAR DistinguishedName,  const PWCHAR NewDistinguishedName, INT DeleteOldRdn);
typedef ULONG (LDAPAPI* PXldap_modrdn2A)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName, INT DeleteOldRdn);
typedef ULONG (LDAPAPI* PXldap_modrdnW)(LDAP* ExternalHandle,  const PWCHAR DistinguishedName,  const PWCHAR NewDistinguishedName);
typedef ULONG (LDAPAPI* PXldap_modrdnA)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName);
typedef ULONG (LDAPAPI* PXldap_modrdn2_sW)(LDAP* ExternalHandle,  const PWCHAR DistinguishedName,  const PWCHAR NewDistinguishedName, INT DeleteOldRdn);
typedef ULONG (LDAPAPI* PXldap_modrdn2_sA)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName, INT DeleteOldRdn);
typedef ULONG (LDAPAPI* PXldap_modrdn_sW)(LDAP* ExternalHandle,  const PWCHAR DistinguishedName,  const PWCHAR NewDistinguishedName);
typedef ULONG (LDAPAPI* PXldap_modrdn_sA)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName);
typedef ULONG (LDAPAPI* PXldap_modrdn2)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName, INT DeleteOldRdn);
typedef ULONG (LDAPAPI* PXldap_modrdn)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName);
typedef ULONG (LDAPAPI* PXldap_modrdn2_s)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName, INT DeleteOldRdn);
typedef ULONG (LDAPAPI* PXldap_modrdn_s)(LDAP* ExternalHandle,  const PCHAR DistinguishedName,  const PCHAR NewDistinguishedName);
typedef ULONG (LDAPAPI* PXldap_rename_extW)(LDAP* ld,  const PWCHAR dn,  const PWCHAR NewRDN,  const PWCHAR NewParent, INT DeleteOldRdn, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_rename_extA)(LDAP* ld,  const PCHAR dn,  const PCHAR NewRDN,  const PCHAR NewParent, INT DeleteOldRdn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_rename_ext_sW)(LDAP* ld,  const PWCHAR dn,  const PWCHAR NewRDN,  const PWCHAR NewParent, INT DeleteOldRdn, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls);
typedef ULONG (LDAPAPI* PXldap_rename_ext_sA)(LDAP* ld,  const PCHAR dn,  const PCHAR NewRDN,  const PCHAR NewParent, INT DeleteOldRdn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_rename_ext)(LDAP* ld, const PCHAR dn, const PCHAR NewRDN, const PCHAR NewParent, INT DeleteOldRdn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_rename_ext_s)(LDAP* ld, const PCHAR dn, const PCHAR NewRDN, const PCHAR NewParent, INT DeleteOldRdn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_addW)(LDAP* ld,  PWCHAR dn, LDAPModW* attrs[]);
typedef ULONG (LDAPAPI* PXldap_addA)(LDAP* ld,  PCHAR dn, LDAPModA* attrs[]);
typedef ULONG (LDAPAPI* PXldap_add_sW)(LDAP* ld,  PWCHAR dn, LDAPModW* attrs[]);
typedef ULONG (LDAPAPI* PXldap_add_sA)(LDAP* ld,  PCHAR dn, LDAPModA* attrs[]);
typedef ULONG (LDAPAPI* PXldap_add_extW)(LDAP* ld,  const PWCHAR dn, LDAPModW* attrs[], PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_add_extA)(LDAP* ld,  const PCHAR dn, LDAPModA* attrs[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_add_ext_sW)(LDAP* ld,  const PWCHAR dn, LDAPModW* attrs[], PLDAPControlW* ServerControls, PLDAPControlW* ClientControls);
typedef ULONG (LDAPAPI* PXldap_add_ext_sA)(LDAP* ld,  const PCHAR dn, LDAPModA* attrs[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_add)(LDAP* ld,  PCHAR dn, LDAPMod* attrs[]);
typedef ULONG (LDAPAPI* PXldap_add_s)(LDAP* ld,  PCHAR dn, LDAPMod* attrs[]);
typedef ULONG (LDAPAPI* PXldap_add_ext)(LDAP* ld, const PCHAR dn, LDAPModA* attrs[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_add_ext_s)(LDAP* ld, const PCHAR dn, LDAPModA* attrs[], PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_compareW)(LDAP* ld,  const PWCHAR dn,  const PWCHAR attr,  PWCHAR value);
typedef ULONG (LDAPAPI* PXldap_compareA)(LDAP* ld,  const PCHAR dn,  const PCHAR attr,  PCHAR value);
typedef ULONG (LDAPAPI* PXldap_compare_sW)(LDAP* ld,  const PWCHAR dn,  const PWCHAR attr,  PWCHAR value);
typedef ULONG (LDAPAPI* PXldap_compare_sA)(LDAP* ld,  const PCHAR dn,  const PCHAR attr, PCHAR value);
typedef ULONG (LDAPAPI* PXldap_compare)(LDAP* ld,  const PCHAR dn,  const PCHAR attr,  PCHAR value);
typedef ULONG (LDAPAPI* PXldap_compare_s)(LDAP* ld,  const PCHAR dn,  const PCHAR attr,  PCHAR value);
typedef ULONG (LDAPAPI* PXldap_compare_extW)(LDAP* ld,  const PWCHAR dn,  const PWCHAR Attr,  const PWCHAR Value,  struct berval* Data, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_compare_extA)(LDAP* ld,  const PCHAR dn,  const PCHAR Attr,  const PCHAR Value,  struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_compare_ext_sW)(LDAP* ld,  const PWCHAR dn,  const PWCHAR Attr,  const PWCHAR Value,  struct berval* Data, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls);
typedef ULONG (LDAPAPI* PXldap_compare_ext_sA)(LDAP* ld,  const PCHAR dn,  const PCHAR Attr,  const PCHAR Value,  struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_compare_ext)(LDAP* ld, const PCHAR dn, const PCHAR Attr, const PCHAR Value, struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_compare_ext_s)(LDAP* ld, const PCHAR dn, const PCHAR Attr, const PCHAR Value, struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_deleteW)(LDAP* ld,  const PWCHAR dn);
typedef ULONG (LDAPAPI* PXldap_deleteA)(LDAP* ld,  const PCHAR dn);
typedef ULONG (LDAPAPI* PXldap_delete_sW)(LDAP* ld,  const PWCHAR dn);
typedef ULONG (LDAPAPI* PXldap_delete_sA)(LDAP* ld,  const PCHAR dn);
typedef ULONG (LDAPAPI* PXldap_delete_extW)(LDAP* ld,  const PWCHAR dn, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_delete_extA)(LDAP* ld,  const PCHAR dn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_delete_ext_sW)(LDAP* ld,  const PWCHAR dn, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls);
typedef ULONG (LDAPAPI* PXldap_delete_ext_sA)(LDAP* ld,  const PCHAR dn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_delete)(LDAP* ld,  PCHAR dn);
typedef ULONG (LDAPAPI* PXldap_delete_s)(LDAP* ld,  PCHAR dn);
typedef ULONG (LDAPAPI* PXldap_delete_ext)(LDAP* ld, const PCHAR dn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_delete_ext_s)(LDAP* ld, const PCHAR dn, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef ULONG (LDAPAPI* PXldap_abandon)(LDAP* ld, ULONG msgid);
typedef ULONG (LDAPAPI* PXldap_result)(LDAP* ld, ULONG msgid, ULONG all, struct l_timeval* timeout, LDAPMessage** res);
typedef ULONG (LDAPAPI* PXldap_msgfree)(LDAPMessage* res);
typedef ULONG (LDAPAPI* PXldap_result2error)(LDAP* ld, LDAPMessage* res, ULONG freeit);
typedef ULONG (LDAPAPI* PXldap_parse_resultW)(LDAP* Connection, LDAPMessage* ResultMessage, ULONG* ReturnCode OPTIONAL, __deref_out PWCHAR* MatchedDNs OPTIONAL, __deref_out PWCHAR* ErrorMessage OPTIONAL, __deref_out PWCHAR** Referrals OPTIONAL, PLDAPControlW** ServerControls OPTIONAL, BOOLEAN Freeit);
typedef ULONG (LDAPAPI* PXldap_parse_resultA)(LDAP* Connection, LDAPMessage* ResultMessage, ULONG* ReturnCode OPTIONAL, __deref_out PCHAR* MatchedDNs OPTIONAL, __deref_out PCHAR* ErrorMessage OPTIONAL, __deref_out PCHAR** Referrals OPTIONAL, PLDAPControlA** ServerControls OPTIONAL, BOOLEAN Freeit);
typedef ULONG (LDAPAPI* PXldap_parse_extended_resultA)(LDAP* Connection, LDAPMessage* ResultMessage, __deref_out PCHAR* ResultOID, struct berval** ResultData, BOOLEAN Freeit);
typedef ULONG (LDAPAPI* PXldap_parse_extended_resultW)(LDAP* Connection, LDAPMessage* ResultMessage, __deref_out PWCHAR* ResultOID, struct berval** ResultData, BOOLEAN Freeit);
typedef ULONG (LDAPAPI* PXldap_controls_freeA)(LDAPControlA** Controls);
typedef ULONG (LDAPAPI* PXldap_control_freeA)(LDAPControlA* Controls);
typedef ULONG (LDAPAPI* PXldap_controls_freeW)(LDAPControlW** Control);
typedef ULONG (LDAPAPI* PXldap_control_freeW)(LDAPControlW* Control);
typedef ULONG (LDAPAPI* PXldap_free_controlsW)(LDAPControlW** Controls);
typedef ULONG (LDAPAPI* PXldap_free_controlsA)(LDAPControlA** Controls);
typedef ULONG (LDAPAPI* PXldap_parse_result)(LDAP* Connection, LDAPMessage* ResultMessage, ULONG* ReturnCode OPTIONAL, PCHAR* MatchedDNs OPTIONAL, PCHAR* ErrorMessage OPTIONAL, PCHAR** Referrals OPTIONAL, PLDAPControlA** ServerControls OPTIONAL, BOOLEAN Freeit);
typedef ULONG (LDAPAPI* PXldap_controls_free)(LDAPControlA** Controls);
typedef ULONG (LDAPAPI* PXldap_control_free)(LDAPControlA* Control);
typedef ULONG (LDAPAPI* PXldap_free_controls)(LDAPControlA** Controls);
typedef PWCHAR (LDAPAPI* PXldap_err2stringW)(ULONG err);
typedef PCHAR (LDAPAPI* PXldap_err2stringA)(ULONG err);
typedef PCHAR (LDAPAPI* PXldap_err2string)(ULONG err);
typedef void (LDAPAPI* PXldap_perror)(LDAP* ld,  const PCHAR msg);
typedef LDAPMessage* (LDAPAPI* PXldap_first_entry)(LDAP* ld, LDAPMessage* res);
typedef LDAPMessage* (LDAPAPI* PXldap_next_entry)(LDAP* ld, LDAPMessage* entry);
typedef ULONG (LDAPAPI* PXldap_count_entries)(LDAP* ld, LDAPMessage* res);
typedef PWCHAR (LDAPAPI* PXldap_first_attributeW)(LDAP* ld, LDAPMessage* entry, BerElement** ptr);
typedef PCHAR (LDAPAPI* PXldap_first_attributeA)(LDAP* ld, LDAPMessage* entry, BerElement** ptr);
typedef PCHAR (LDAPAPI* PXldap_first_attribute)(LDAP* ld, LDAPMessage* entry, BerElement** ptr);
typedef PWCHAR (LDAPAPI* PXldap_next_attributeW)(LDAP* ld, LDAPMessage* entry, BerElement* ptr);
typedef PCHAR (LDAPAPI* PXldap_next_attributeA)(LDAP* ld, LDAPMessage* entry, BerElement* ptr);
typedef PCHAR (LDAPAPI* PXldap_next_attribute)(LDAP* ld, LDAPMessage* entry, BerElement* ptr);
typedef PWCHAR* (LDAPAPI* PXldap_get_valuesW)(LDAP* ld, LDAPMessage* entry,  const PWCHAR attr);
typedef PCHAR* (LDAPAPI* PXldap_get_valuesA)(LDAP* ld, LDAPMessage* entry,  const PCHAR attr);
typedef PCHAR* (LDAPAPI* PXldap_get_values)(LDAP* ld, LDAPMessage* entry,  const PCHAR attr);
typedef struct berval** (LDAPAPI* PXldap_get_values_lenW)(LDAP* ExternalHandle, LDAPMessage* Message,  const PWCHAR attr);
typedef struct berval** (LDAPAPI* PXldap_get_values_lenA)(LDAP* ExternalHandle, LDAPMessage* Message,  const PCHAR attr);
typedef struct berval** (LDAPAPI* PXldap_get_values_len)(LDAP* ExternalHandle, LDAPMessage* Message,  const PCHAR attr);
typedef ULONG (LDAPAPI* PXldap_count_valuesW)( PWCHAR* vals);
typedef ULONG (LDAPAPI* PXldap_count_valuesA)( PCHAR* vals);
typedef ULONG (LDAPAPI* PXldap_count_values)( PCHAR* vals);
typedef ULONG (LDAPAPI* PXldap_count_values_len)(struct berval** vals);
typedef ULONG (LDAPAPI* PXldap_value_freeW)( PWCHAR* vals);
typedef ULONG (LDAPAPI* PXldap_value_freeA)( PCHAR* vals);
typedef ULONG (LDAPAPI* PXldap_value_free)( PCHAR* vals);
typedef ULONG (LDAPAPI* PXldap_value_free_len)(struct berval** vals);
typedef PWCHAR (LDAPAPI* PXldap_get_dnW)(LDAP* ld, LDAPMessage* entry);
typedef PCHAR (LDAPAPI* PXldap_get_dnA)(LDAP* ld, LDAPMessage* entry);
typedef PCHAR (LDAPAPI* PXldap_get_dn)(LDAP* ld, LDAPMessage* entry);
typedef PWCHAR* (LDAPAPI* PXldap_explode_dnW)( const PWCHAR dn, ULONG notypes);
typedef PCHAR* (LDAPAPI* PXldap_explode_dnA)( const PCHAR dn, ULONG notypes);
typedef PCHAR* (LDAPAPI* PXldap_explode_dn)( const PCHAR dn, ULONG notypes);
typedef PWCHAR (LDAPAPI* PXldap_dn2ufnW)( const PWCHAR dn);
typedef PCHAR (LDAPAPI* PXldap_dn2ufnA)( const PCHAR dn);
typedef PCHAR (LDAPAPI* PXldap_dn2ufn)( const PCHAR dn);
typedef VOID (LDAPAPI* PXldap_memfreeW)( PWCHAR Block);
typedef VOID (LDAPAPI* PXldap_memfreeA)( PCHAR Block);
typedef VOID (LDAPAPI* PXber_bvfree)(struct berval* bv);
typedef VOID (LDAPAPI* PXldap_memfree)( PCHAR Block);
typedef ULONG (LDAPAPI* PXldap_ufn2dnW)( const PWCHAR ufn, PWCHAR* pDn);
typedef ULONG (LDAPAPI* PXldap_ufn2dnA)( const PCHAR ufn, PCHAR* pDn);
typedef ULONG (LDAPAPI* PXldap_ufn2dn)( const PCHAR ufn, PCHAR* pDn);
typedef ULONG (LDAPAPI* PXldap_startup)(PLDAP_VERSION_INFO version, HANDLE* Instance);
typedef ULONG (LDAPAPI* PXldap_cleanup)(HANDLE hInstance);
typedef ULONG (LDAPAPI* PXldap_escape_filter_elementW)(PCHAR sourceFilterElement, ULONG sourceLength, PWCHAR destFilterElement, ULONG destLength);
typedef ULONG (LDAPAPI* PXldap_escape_filter_elementA)(PCHAR sourceFilterElement, ULONG sourceLength, PCHAR destFilterElement, ULONG destLength);
typedef ULONG (LDAPAPI* PXldap_escape_filter_element)(PCHAR sourceFilterElement, ULONG sourceLength, PCHAR destFilterElement, ULONG destLength);
typedef ULONG (LDAPAPI* PXldap_set_dbg_flags)(ULONG NewFlags);
typedef VOID (LDAPAPI* PXldap_set_dbg_routine)(DBGPRINT DebugPrintRoutine);
typedef int (LDAPAPI* PXldapUTF8ToUnicode)(LPCSTR lpSrcStr, int cchSrc, LPWSTR lpDestStr, int cchDest);
typedef int (LDAPAPI* PXldapUnicodeToUTF8)(LPCWSTR lpSrcStr, int cchSrc, LPSTR lpDestStr, int cchDest);
typedef ULONG (LDAPAPI* PXldap_create_sort_controlA)(PLDAP ExternalHandle, PLDAPSortKeyA* SortKeys, UCHAR IsCritical, PLDAPControlA* Control);
typedef ULONG (LDAPAPI* PXldap_create_sort_controlW)(PLDAP ExternalHandle, PLDAPSortKeyW* SortKeys, UCHAR IsCritical, PLDAPControlW* Control);
typedef ULONG (LDAPAPI* PXldap_parse_sort_controlA)(PLDAP ExternalHandle, PLDAPControlA* Control, ULONG* Result, PCHAR* Attribute);
typedef ULONG (LDAPAPI* PXldap_parse_sort_controlW)(PLDAP ExternalHandle, PLDAPControlW* Control, ULONG* Result, PWCHAR* Attribute);
typedef ULONG (LDAPAPI* PXldap_create_sort_control)(PLDAP ExternalHandle, PLDAPSortKeyA* SortKeys, UCHAR IsCritical, PLDAPControlA* Control);
typedef ULONG (LDAPAPI* PXldap_parse_sort_control)(PLDAP ExternalHandle, PLDAPControlA* Control, ULONG* Result, PCHAR* Attribute);
typedef ULONG (LDAPAPI* PXldap_encode_sort_controlW)(PLDAP ExternalHandle, PLDAPSortKeyW* SortKeys, PLDAPControlW Control, BOOLEAN Criticality);
typedef ULONG (LDAPAPI* PXldap_encode_sort_controlA)(PLDAP ExternalHandle, PLDAPSortKeyA* SortKeys, PLDAPControlA Control, BOOLEAN Criticality);
typedef ULONG (LDAPAPI* PXldap_encode_sort_control)(PLDAP ExternalHandle, PLDAPSortKeyA* SortKeys, PLDAPControlA Control, BOOLEAN Criticality);
typedef ULONG (LDAPAPI* PXldap_create_page_controlW)(PLDAP ExternalHandle, ULONG PageSize, struct berval* Cookie, UCHAR IsCritical, PLDAPControlW* Control);
typedef ULONG (LDAPAPI* PXldap_create_page_controlA)(PLDAP ExternalHandle, ULONG PageSize, struct berval* Cookie, UCHAR IsCritical, PLDAPControlA* Control);
typedef ULONG (LDAPAPI* PXldap_parse_page_controlW)(PLDAP ExternalHandle, PLDAPControlW* ServerControls, ULONG* TotalCount, struct berval** Cookie);
typedef ULONG (LDAPAPI* PXldap_parse_page_controlA)(PLDAP ExternalHandle, PLDAPControlA* ServerControls, ULONG* TotalCount, struct berval** Cookie);
typedef ULONG (LDAPAPI* PXldap_create_page_control)(PLDAP ExternalHandle, ULONG PageSize, struct berval* Cookie, UCHAR IsCritical, PLDAPControlA* Control);
typedef ULONG (LDAPAPI* PXldap_parse_page_control)(PLDAP ExternalHandle, PLDAPControlA* ServerControls, ULONG* TotalCount, struct berval** Cookie);
typedef PLDAPSearch (LDAPAPI* PXldap_search_init_pageW)(PLDAP ExternalHandle,  const PWCHAR DistinguishedName, ULONG ScopeOfSearch,  const PWCHAR SearchFilter,  PWCHAR AttributeList[], ULONG AttributesOnly, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG PageTimeLimit, ULONG TotalSizeLimit, PLDAPSortKeyW* SortKeys);
typedef PLDAPSearch (LDAPAPI* PXldap_search_init_pageA)(PLDAP ExternalHandle,  const PCHAR DistinguishedName, ULONG ScopeOfSearch,  const PCHAR SearchFilter,  PCHAR AttributeList[], ULONG AttributesOnly, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG PageTimeLimit, ULONG TotalSizeLimit, PLDAPSortKeyA* SortKeys);
typedef PLDAPSearch (LDAPAPI* PXldap_search_init_page)(PLDAP ExternalHandle, const PCHAR DistinguishedName, ULONG ScopeOfSearch, const PCHAR SearchFilter, PCHAR AttributeList[], ULONG AttributesOnly, PLDAPControl* ServerControls, PLDAPControl* ClientControls, ULONG PageTimeLimit, ULONG TotalSizeLimit, PLDAPSortKey* SortKeys);
typedef ULONG (LDAPAPI* PXldap_get_next_page)(PLDAP ExternalHandle, PLDAPSearch SearchHandle, ULONG PageSize, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_get_next_page_s)(PLDAP ExternalHandle, PLDAPSearch SearchHandle, struct l_timeval* timeout, ULONG PageSize, ULONG* TotalCount, LDAPMessage** Results);
typedef ULONG (LDAPAPI* PXldap_get_paged_count)(PLDAP ExternalHandle, PLDAPSearch SearchBlock, ULONG* TotalCount, PLDAPMessage Results);
typedef ULONG (LDAPAPI* PXldap_search_abandon_page)(PLDAP ExternalHandle, PLDAPSearch SearchBlock);
typedef INT (LDAPAPI* PXldap_create_vlv_controlW)(PLDAP ExternalHandle, PLDAPVLVInfo VlvInfo, UCHAR IsCritical, PLDAPControlW* Control);
typedef INT (LDAPAPI* PXldap_create_vlv_controlA)(PLDAP ExternalHandle, PLDAPVLVInfo VlvInfo, UCHAR IsCritical, PLDAPControlA* Control);
typedef INT (LDAPAPI* PXldap_parse_vlv_controlW)(PLDAP ExternalHandle, PLDAPControlW* Control, PULONG TargetPos, PULONG ListCount, PBERVAL* Context, PINT ErrCode);
typedef INT (LDAPAPI* PXldap_parse_vlv_controlA)(PLDAP ExternalHandle, PLDAPControlA* Control, PULONG TargetPos, PULONG ListCount, PBERVAL* Context, PINT ErrCode);
typedef ULONG (LDAPAPI* PXldap_start_tls_sW)(PLDAP ExternalHandle, PULONG ServerReturnValue, LDAPMessage** result, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls);
typedef ULONG (LDAPAPI* PXldap_start_tls_sA)(PLDAP ExternalHandle, PULONG ServerReturnValue, LDAPMessage** result, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls);
typedef BOOLEAN (LDAPAPI* PXldap_stop_tls_s)(PLDAP ExternalHandle);
typedef LDAPMessage* (LDAPAPI* PXldap_first_reference)(LDAP* ld, LDAPMessage* res);
typedef LDAPMessage* (LDAPAPI* PXldap_next_reference)(LDAP* ld, LDAPMessage* entry);
typedef ULONG (LDAPAPI* PXldap_count_references)(LDAP* ld, LDAPMessage* res);
typedef ULONG (LDAPAPI* PXldap_parse_referenceW)(LDAP* Connection, LDAPMessage* ResultMessage, PWCHAR** Referrals);
typedef ULONG (LDAPAPI* PXldap_parse_referenceA)(LDAP* Connection, LDAPMessage* ResultMessage, PCHAR** Referrals);
typedef ULONG (LDAPAPI* PXldap_parse_reference)(LDAP* Connection, LDAPMessage* ResultMessage, PCHAR** Referrals);
typedef ULONG (LDAPAPI* PXldap_extended_operationW)(LDAP* ld,  const PWCHAR Oid, struct berval* Data, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_extended_operationA)(LDAP* ld,  const PCHAR Oid, struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_extended_operation_sA)(LDAP* ExternalHandle,  PCHAR Oid, struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, PCHAR* ReturnedOid, struct berval** ReturnedData);
typedef ULONG (LDAPAPI* PXldap_extended_operation_sW)(LDAP* ExternalHandle,  PWCHAR Oid, struct berval* Data, PLDAPControlW* ServerControls, PLDAPControlW* ClientControls, PWCHAR* ReturnedOid, struct berval** ReturnedData);
typedef ULONG (LDAPAPI* PXldap_extended_operation)(LDAP* ld, const PCHAR Oid, struct berval* Data, PLDAPControlA* ServerControls, PLDAPControlA* ClientControls, ULONG* MessageNumber);
typedef ULONG (LDAPAPI* PXldap_close_extended_op)(LDAP* ld, ULONG MessageNumber);
typedef ULONG (LDAPAPI* PXldapGetLastError)(VOID);
typedef ULONG (LDAPAPI* PXldapMapErrorToWin32)(ULONG LdapError);
typedef LDAP* (LDAPAPI* PXldap_conn_from_msg)(LDAP* PrimaryConn, LDAPMessage* res);




PXActionResult PXAPI PXLDAPClienInitialize(PXLDAPClient* const pxLDAPClient)
{
	PXClear(PXLDAPClient, pxLDAPClient);

	// Library open
	{
		const PXActionResult pxActionResult = PXLibraryOpenA(&pxLDAPClient->LDAPLibrary, "");

		if(PXActionSuccessful != pxActionResult)
		{
			return PXActionRefusedNotSupported;
		}
	}
	
	// Fetch functions
	{
		const PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
		{
			{ pxLDAPClient->openW, "ldap_openW" },
			{ pxLDAPClient->openA,"ldap_openA" },
			{ pxLDAPClient->initW,"ldap_initW" },
			{ pxLDAPClient->initA,"ldap_initA" },
			{ pxLDAPClient->sslinitW,"ldap_sslinitW" },
			{ pxLDAPClient->sslinitA,"ldap_sslinitA" },
			{ pxLDAPClient->connect,"ldap_connect" },
		{ pxLDAPClient->open,"ldap_open" },
		{ pxLDAPClient->init,"ldap_init" },
		{ pxLDAPClient->sslinit,"ldap_sslinit" },
		{ pxLDAPClient->copenW,"cldap_openW" },
		{ pxLDAPClient->copenA,"cldap_openA" },
		{ pxLDAPClient->copen,"cldap_open" },
		{ pxLDAPClient->unbind,"ldap_unbind" },
		{ pxLDAPClient->unbind_s,"ldap_unbind_s" },
		{ pxLDAPClient->get_optionA,"ldap_getion" },
		{ pxLDAPClient->get_optionW,"ldap_getionW" },
		{ pxLDAPClient->set_optionA,"ldap_setion" },
		{ pxLDAPClient->set_optionW,"ldap_setionW" },
		{ pxLDAPClient->simple_bindW,"ldap_simple_bindW" },
		{ pxLDAPClient->simple_bindA,"ldap_simple_bindA" },
		{ pxLDAPClient->simple_bind_sW,"ldap_simple_bind_sW" },
		{ pxLDAPClient->simple_bind_sA,"ldap_simple_bind_sA" },
		{ pxLDAPClient->bindW,"ldap_bindW" },
		{ pxLDAPClient->bindA,"ldap_bindA" },
		{ pxLDAPClient->bind_sW,"ldap_bind_sW" },
		{ pxLDAPClient->bind_sA,"ldap_bind_sA" },
		{ pxLDAPClient->sasl_bindA,"ldap_sasl_bindA" },
		{ pxLDAPClient->sasl_bindW,"ldap_sasl_bindW" },
		{ pxLDAPClient->sasl_bind_sA,"ldap_sasl_bind_sA" },
		{ pxLDAPClient->sasl_bind_sW,"ldap_sasl_bind_sW" },
		{ pxLDAPClient->simple_bind,"ldap_simple_bind" },
		{ pxLDAPClient->simple_bind_s,"ldap_simple_bind_s" },
		{ pxLDAPClient->bind,"ldap_bind" },
		{ pxLDAPClient->bind_s,"ldap_bind_s" },
		{ pxLDAPClient->searchW,"ldap_searchW" },
		{ pxLDAPClient->searchA,"ldap_searchA" },
		{ pxLDAPClient->search_sW,"ldap_search_sW" },
		{ pxLDAPClient->search_sA,"ldap_search_sA" },
		{ pxLDAPClient->search_stW,"ldap_search_stW" },
		{ pxLDAPClient->search_stA,"ldap_search_stA" },
		{ pxLDAPClient->search_extW,"ldap_search_extW" },
		{ pxLDAPClient->search_extA,"ldap_search_extA" },
		{ pxLDAPClient->search_ext_sW,"ldap_search_ext_sW" },
		{ pxLDAPClient->search_ext_sA,"ldap_search_ext_sA" },
		{ pxLDAPClient->search,"ldap_search" },
		{ pxLDAPClient->search_s,"ldap_search_s" },
		{ pxLDAPClient->search_st,"ldap_search_st" },
		{ pxLDAPClient->search_ext,"ldap_search_ext" },
		{ pxLDAPClient->search_ext_s,"ldap_search_ext_s" },
		{ pxLDAPClient->check_filterW,"ldap_check_filterW" },
		{ pxLDAPClient->check_filterA,"ldap_check_filterA" },
		{ pxLDAPClient->modifyW,"ldap_modifyW" },
		{ pxLDAPClient->modifyA,"ldap_modifyA" },
		{ pxLDAPClient->modify_sW,"ldap_modify_sW" },
		{ pxLDAPClient->modify_sA,"ldap_modify_sA" },
		{ pxLDAPClient->modify_extW,"ldap_modify_extW" },
		{ pxLDAPClient->modify_extA,"ldap_modify_extA" },
		{ pxLDAPClient->modify_ext_sW,"ldap_modify_ext_sW" },
		{ pxLDAPClient->modify_ext_sA,"ldap_modify_ext_sA" },
		{ pxLDAPClient->modify,"ldap_modify" },
		{ pxLDAPClient->modify_s,"ldap_modify_s" },
		{ pxLDAPClient->modify_ext,"ldap_modify_ext" },
		{ pxLDAPClient->modify_ext_s,"ldap_modify_ext_s" },
		{ pxLDAPClient->modrdn2W,"ldap_modrdn2W" },
		{ pxLDAPClient->modrdn2A,"ldap_modrdn2A" },
		{ pxLDAPClient->modrdnW,"ldap_modrdnW" },
		{ pxLDAPClient->modrdnA,"ldap_modrdnA" },
		{ pxLDAPClient->modrdn2_sW,"ldap_modrdn2_sW" },
		{ pxLDAPClient->modrdn2_sA,"ldap_modrdn2_sA" },
		{ pxLDAPClient->modrdn_sW,"ldap_modrdn_sW" },
		{ pxLDAPClient->modrdn_sA,"ldap_modrdn_sA" },
		{ pxLDAPClient->modrdn2,"ldap_modrdn2" },
		{ pxLDAPClient->modrdn,"ldap_modrdn" },
		{ pxLDAPClient->modrdn2_s,"ldap_modrdn2_s" },
		{ pxLDAPClient->modrdn_s,"ldap_modrdn_s" },
		{ pxLDAPClient->rename_extW,"ldap_rename_extW" },
		{ pxLDAPClient->rename_extA,"ldap_rename_extA" },
		{ pxLDAPClient->rename_ext_sW,"ldap_rename_ext_sW" },
		{ pxLDAPClient->rename_ext_sA,"ldap_rename_ext_sA" },
		{ pxLDAPClient->rename_ext,"ldap_rename_ext" },
		{ pxLDAPClient->rename_ext_s,"ldap_rename_ext_s" },
		{ pxLDAPClient->addW,"ldap_addW" },
		{ pxLDAPClient->addA,"ldap_addA" },
		{ pxLDAPClient->add_sW,"ldap_add_sW" },
		{ pxLDAPClient->add_sA,"ldap_add_sA" },
		{ pxLDAPClient->add_extW,"ldap_add_extW" },
		{ pxLDAPClient->add_extA,"ldap_add_extA" },
		{ pxLDAPClient->add_ext_sW,"ldap_add_ext_sW" },
		{ pxLDAPClient->add_ext_sA,"ldap_add_ext_sA" },
		{ pxLDAPClient->add,"ldap_add" },
		{ pxLDAPClient->add_s,"ldap_add_s" },
		{ pxLDAPClient->add_ext,"ldap_add_ext" },
		{ pxLDAPClient->add_ext_s,"ldap_add_ext_s" },
		{ pxLDAPClient->compareW,"ldap_compareW" },
		{ pxLDAPClient->compareA,"ldap_compareA" },
		{ pxLDAPClient->compare_sW,"ldap_compare_sW" },
		{ pxLDAPClient->compare_sA,"ldap_compare_sA" },
		{ pxLDAPClient->compare,"ldap_compare" },
		{ pxLDAPClient->compare_s,"ldap_compare_s" },
		{ pxLDAPClient->compare_extW,"ldap_compare_extW" },
		{ pxLDAPClient->compare_extA,"ldap_compare_extA" },
		{ pxLDAPClient->compare_ext_sW,"ldap_compare_ext_sW" },
		{ pxLDAPClient->compare_ext_sA,"ldap_compare_ext_sA" },
		{ pxLDAPClient->compare_ext,"ldap_compare_ext" },
		{ pxLDAPClient->compare_ext_s,"ldap_compare_ext_s" },
		{ pxLDAPClient->deleteW,"ldap_deleteW" },
		{ pxLDAPClient->deleteA,"ldap_deleteA" },
		{ pxLDAPClient->delete_sW,"ldap_delete_sW" },
		{ pxLDAPClient->delete_sA,"ldap_delete_sA" },
		{ pxLDAPClient->delete_extW,"ldap_delete_extW" },
		{ pxLDAPClient->delete_extA,"ldap_delete_extA" },
		{ pxLDAPClient->delete_ext_sW,"ldap_delete_ext_sW" },
		{ pxLDAPClient->delete_ext_sA,"ldap_delete_ext_sA" },
		{ pxLDAPClient->delete,"ldap_delete" },
		{ pxLDAPClient->delete_s,"ldap_delete_s" },
		{ pxLDAPClient->delete_ext,"ldap_delete_ext" },
		{ pxLDAPClient->delete_ext_s,"ldap_delete_ext_s" },
		{ pxLDAPClient->abandon,"ldap_abandon" },
		{ pxLDAPClient->result,"ldap_result" },
		{ pxLDAPClient->msgfree,"ldap_msgfree" },
		{ pxLDAPClient->result2error,"ldap_result2error" },
		{ pxLDAPClient->parse_resultW,"ldap_parse_resultW" },
		{ pxLDAPClient->parse_resultA,"ldap_parse_resultA" },
		{ pxLDAPClient->parse_extended_resultA,"ldap_parse_extended_resultA" },
		{ pxLDAPClient->parse_extended_resultW,"ldap_parse_extended_resultW" },
		{ pxLDAPClient->controls_freeA,"ldap_controls_freeA" },
		{ pxLDAPClient->control_freeA,"ldap_control_freeA" },
		{ pxLDAPClient->controls_freeW,"ldap_controls_freeW" },
		{ pxLDAPClient->control_freeW,"ldap_control_freeW" },
		{ pxLDAPClient->free_controlsW,"ldap_free_controlsW" },
		{ pxLDAPClient->free_controlsA,"ldap_free_controlsA" },
		{ pxLDAPClient->parse_result,"ldap_parse_result" },
		{ pxLDAPClient->controls_free,"ldap_controls_free" },
		{ pxLDAPClient->control_free,"ldap_control_free" },
		{ pxLDAPClient->free_controls,"ldap_free_controls" },
		{ pxLDAPClient->err2stringW,"ldap_err2stringW" },
		{ pxLDAPClient->err2stringA,"ldap_err2stringA" },
		{ pxLDAPClient->err2string,"ldap_err2string" },
		{ pxLDAPClient->perror,"ldap_perror" },
		{ pxLDAPClient->first_entry,"ldap_first_entry" },
		{ pxLDAPClient->next_entry,"ldap_next_entry" },
		{ pxLDAPClient->count_entries,"ldap_count_entries" },
		{ pxLDAPClient->first_attributeW,"ldap_first_attributeW" },
		{ pxLDAPClient->first_attributeA,"ldap_first_attributeA" },
		{ pxLDAPClient->first_attribute,"ldap_first_attribute" },
		{ pxLDAPClient->next_attributeW,"ldap_next_attributeW" },
		{ pxLDAPClient->next_attributeA,"ldap_next_attributeA" },
		{ pxLDAPClient->next_attribute,"ldap_next_attribute" },
		{ pxLDAPClient->get_valuesW,"ldap_get_valuesW" },
		{ pxLDAPClient->get_valuesA,"ldap_get_valuesA" },
		{ pxLDAPClient->get_values,"ldap_get_values" },
		{ pxLDAPClient->get_values_lenW,"ldap_get_values_lenW" },
		{ pxLDAPClient->get_values_lenA,"ldap_get_values_lenA" },
		{ pxLDAPClient->get_values_len,"ldap_get_values_len" },
		{ pxLDAPClient->count_valuesW,"ldap_count_valuesW" },
		{ pxLDAPClient->count_valuesA,"ldap_count_valuesA" },
		{ pxLDAPClient->count_values,"ldap_count_values" },
		{ pxLDAPClient->count_values_len,"ldap_count_values_len" },
		{ pxLDAPClient->value_freeW,"ldap_value_freeW" },
		{ pxLDAPClient->value_freeA,"ldap_value_freeA" },
		{ pxLDAPClient->value_free,"ldap_value_free" },
		{ pxLDAPClient->value_free_len,"ldap_value_free_len" },
		{ pxLDAPClient->get_dnW,"ldap_get_dnW" },
		{ pxLDAPClient->get_dnA,"ldap_get_dnA" },
		{ pxLDAPClient->get_dn,"ldap_get_dn" },
		{ pxLDAPClient->explode_dnW,"ldap_explode_dnW" },
		{ pxLDAPClient->explode_dnA,"ldap_explode_dnA" },
		{ pxLDAPClient->explode_dn,"ldap_explode_dn" },
		{ pxLDAPClient->dn2ufnW,"ldap_dn2ufnW" },
		{ pxLDAPClient->dn2ufnA,"ldap_dn2ufnA" },
		{ pxLDAPClient->dn2ufn,"ldap_dn2ufn" },
		{ pxLDAPClient->memfreeW,"ldap_memfreeW" },
		{ pxLDAPClient->memfreeA,"ldap_memfreeA" },
		{ pxLDAPClient->ber_bvfree,"ber_bvfree" },
		{ pxLDAPClient->memfree,"ldap_memfree" },
		{ pxLDAPClient->ufn2dnW,"ldap_ufn2dnW" },
		{ pxLDAPClient->ufn2dnA,"ldap_ufn2dnA" },
		{ pxLDAPClient->ufn2dn,"ldap_ufn2dn" },
		{ pxLDAPClient->startup,"ldap_startup" },
		{ pxLDAPClient->cleanup,"ldap_cleanup" },
		{ pxLDAPClient->escape_filter_elementW,"ldap_escape_filter_elementW" },
		{ pxLDAPClient->escape_filter_elementA,"ldap_escape_filter_elementA" },
		{ pxLDAPClient->escape_filter_element,"ldap_escape_filter_element" },
		{ pxLDAPClient->set_dbg_flags,"ldap_set_dbg_flags" },
		{ pxLDAPClient->set_dbg_routine,"ldap_set_dbg_routine" },
		{ pxLDAPClient->LdapUTF8ToUnicode,"LdapUTF8ToUnicode" },
		{ pxLDAPClient->LdapUnicodeToUTF8,"LdapUnicodeToUTF8" },
		{ pxLDAPClient->create_sort_controlA,"ldap_create_sort_controlA" },
		{ pxLDAPClient->create_sort_controlW,"ldap_create_sort_controlW" },
		{ pxLDAPClient->parse_sort_controlA,"ldap_parse_sort_controlA" },
		{ pxLDAPClient->parse_sort_controlW,"ldap_parse_sort_controlW" },
		{ pxLDAPClient->create_sort_control,"ldap_create_sort_control" },
		{ pxLDAPClient->parse_sort_control,"ldap_parse_sort_control" },
		{ pxLDAPClient->encode_sort_controlW,"ldap_encode_sort_controlW" },
		{ pxLDAPClient->encode_sort_controlA,"ldap_encode_sort_controlA" },
		{ pxLDAPClient->encode_sort_control,"ldap_encode_sort_control" },
		{ pxLDAPClient->create_page_controlW,"ldap_create_page_controlW" },
		{ pxLDAPClient->create_page_controlA,"ldap_create_page_controlA" },
		{ pxLDAPClient->parse_page_controlW,"ldap_parse_page_controlW" },
		{ pxLDAPClient->parse_page_controlA,"ldap_parse_page_controlA" },
		{ pxLDAPClient->create_page_control,"ldap_create_page_control" },
		{ pxLDAPClient->parse_page_control,"ldap_parse_page_control" },
		{ pxLDAPClient->search_init_pageW,"ldap_search_init_pageW" },
		{ pxLDAPClient->search_init_pageA,"ldap_search_init_pageA" },
		{ pxLDAPClient->search_init_page,"ldap_search_init_page" },
			{ pxLDAPClient->get_next_page,"ldap_get_next_page" },
			{ pxLDAPClient->get_next_page_s,"ldap_get_next_page_s" },
			{ pxLDAPClient->get_paged_count,"ldap_get_paged_count" },
			{ pxLDAPClient->search_abandon_page,"ldap_search_abandon_page" },
			{ pxLDAPClient->create_vlv_controlW,"ldap_create_vlv_controlW" },
			{ pxLDAPClient->create_vlv_controlA,"ldap_create_vlv_controlA" },
			{ pxLDAPClient->parse_vlv_controlW,"ldap_parse_vlv_controlW" },
			{ pxLDAPClient->parse_vlv_controlA,"ldap_parse_vlv_controlA" },
			{ pxLDAPClient->start_tls_sW,"ldap_start_tls_sW" },
			{ pxLDAPClient->start_tls_sA,"ldap_start_tls_sA" },
			{ pxLDAPClient->stop_tls_s,"ldap_stop_tls_s" },
			{ pxLDAPClient->first_reference,"ldap_first_reference" },
			{ pxLDAPClient->next_reference,"ldap_next_reference" },
			{ pxLDAPClient->count_references,"ldap_count_references" },
			{ pxLDAPClient->parse_referenceW,"ldap_parse_referenceW" },
			{ pxLDAPClient->parse_referenceA,"ldap_parse_referenceA" },
			{ pxLDAPClient->parse_reference,"ldap_parse_reference" },
			{ pxLDAPClient->extended_operationW,"ldap_extended_operationW" },
			{ pxLDAPClient->extended_operationA,"ldap_extended_operationA" },
			{ pxLDAPClient->extended_operation_sA,"ldap_extended_operation_sA" },
			{ pxLDAPClient->extended_operation_sW,"ldap_extended_operation_sW" },
			{ pxLDAPClient->extended_operation,"ldap_extended_operation" },
			{ pxLDAPClient->close_extended_op,"ldap_close_extended_op" },
			{ pxLDAPClient->LdapGetLastError,"LdapGetLastError" },
			{ pxLDAPClient->LdapMapErrorToWin32,"LdapMapErrorToWin32" },
			{ pxLDAPClient->conn_from_msg,"ldap_conn_from_msg" } 
		};

		const PXSize amount = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

		PXLibraryGetSymbolListA(&pxLDAPClient->LDAPLibrary, pxLibraryFuntionEntry, amount);

	}


	return PXActionSuccessful;
}

PXActionResult PXAPI PXLDAPClienRelease(PXLDAPClient* const pxLDAPClient)
{
	PXLDAPClientClose(pxLDAPClient);

	PXLibraryClose(&pxLDAPClient->LDAPLibrary);

	PXClear(PXLDAPClient, pxLDAPClient);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXLDAPClientOpen(PXLDAPClient* const pxLDAPClient, const PXLDAPConnectionInfo* const pxLDAPInfo)
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
				const PXldap_sslinitA pxldap_sslinitA = pxLDAPClient->sslinitA;

				pxLDAPClient->ID = pxldap_sslinitA(pxLDAPInfo->Host.TextA, pxLDAPInfo->Port, pxLDAPInfo->SSLUse); // Windows Vista, Wldap32.dll, winldap.h
			}
			else
			{
				const PXcldap_openA pxcldap_openA = pxLDAPClient->openA;

				pxLDAPClient->ID = pxcldap_openA(pxLDAPInfo->Host.TextA, pxLDAPInfo->Port); // Windows Vista, Wldap32.dll, winldap.h
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
				const PXldap_sslinitW pxldap_sslinitW = pxLDAPClient->sslinitW;

				pxLDAPClient->ID = pxldap_sslinitW(pxLDAPInfo->Host.TextW, pxLDAPInfo->Port, pxLDAPInfo->SSLUse); // Windows Vista, Wldap32.dll, winldap.h
			}
			else
			{
				const PXcldap_openW pxcldap_openW = pxLDAPClient->openW;

				pxLDAPClient->ID = pxcldap_openW(pxLDAPInfo->Host.TextW, pxLDAPInfo->Port); // Windows Vista, Wldap32.dll, winldap.h
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

		// Set the version to 3.0 (default is 2.0).
		const PXldap_set_optionA pxldap_set_option = pxLDAPClient->set_optionA;
		const ULONG optionSetResult = pxldap_set_option(pxLDAPClient->ID, LDAP_OPT_PROTOCOL_VERSION, &version);
		const PXBool optionSetSuccessful = LDAP_SUCCESS == optionSetResult;

		//LdapGetLastError();

		PXActionOnErrorFetchAndReturn(!optionSetSuccessful); // LdapGetLastError()
	}
#endif

	// Connect
	{
		LDAP_TIMEVAL ldaptime = { pxLDAPInfo->ConnectTimeout, pxLDAPInfo->ConnectTimeout };

		const PXldap_connect pxldap_connect = pxLDAPClient->connect;
		const ULONG connectResult = pxldap_connect(pxLDAPClient->ID, &ldaptime);
		const PXBool connectionSuccessful = LDAP_SUCCESS == connectResult;

		PXActionOnErrorFetchAndReturn(!connectionSuccessful); // LdapGetLastError()
	}

	// Binding
	{
		const PXldap_bind_sA pxldap_bind_sA = pxLDAPClient->bind_sA;
		const ULONG bindResult = pxldap_bind_sA
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

PXActionResult PXAPI PXLDAPClientClose(PXLDAPClient* const pxLDAPClient)
{
	if (!pxLDAPClient->ID)
	{
		return PXActionRefusedObjectNotFound;
	}

#if OSUnix
	return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop
	const PXldap_unbind pxldap_unbind = pxLDAPClient->unbind;
	const ULONG result = pxldap_unbind(pxLDAPClient->ID);
	const PXBool successful = LDAP_SUCCESS == result;

	PXActionOnErrorFetchAndReturn(!successful);

	pxLDAPClient->ID = PXNull;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXLDAPClientSearch(PXLDAPClient* const pxLDAPClient, PXLDAPSearchInfo* const pxLDAPSearchInfo)
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
				const PXldap_searchA pxldap_searchA = pxLDAPClient->searchA;

				const ULONG messageID = pxldap_searchA
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
				const PXldap_search_sA pxldap_search_sA = pxLDAPClient->search_sA;

				const ULONG searchResult = pxldap_search_sA
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
			const PXldap_searchW pxldap_searchW = pxLDAPClient->searchW;

			const ULONG messageID = pxldap_searchW
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