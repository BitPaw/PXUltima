#ifndef PXLDAPINCLUDE
#define PXLDAPINCLUDE

#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Library/PXLibrary.h>

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
	PXLibrary LDAPLibrary;

	void* openW;
	void* openA;
	void* initW;
	void* initA;
	void* sslinitW;
	void* sslinitA;
	void* connect;
	void* open;
	void* init;
	void* sslinit;
	void* copenW;
	void* copenA;
	void* copen;
	void* unbind;
	void* unbind_s;
	void* get_optionA;
	void* get_optionW;
	void* set_optionA;
	void* set_optionW;
	void* simple_bindW;
	void* simple_bindA;
	void* simple_bind_sW;
	void* simple_bind_sA;
	void* bindW;
	void* bindA;
	void* bind_sW;
	void* bind_sA;
	void* sasl_bindA;
	void* sasl_bindW;
	void* sasl_bind_sA;
	void* sasl_bind_sW;
	void* simple_bind;
	void* simple_bind_s;
	void* bind;
	void* bind_s;
	void* searchW;
	void* searchA;
	void* search_sW;
	void* search_sA;
	void* search_stW;
	void* search_stA;
	void* search_extW;
	void* search_extA;
	void* search_ext_sW;
	void* search_ext_sA;
	void* search;
	void* search_s;
	void* search_st;
	void* search_ext;
	void* search_ext_s;
	void* check_filterW;
	void* check_filterA;
	void* modifyW;
	void* modifyA;
	void* modify_sW;
	void* modify_sA;
	void* modify_extW;
	void* modify_extA;
	void* modify_ext_sW;
	void* modify_ext_sA;
	void* modify;
	void* modify_s;
	void* modify_ext;
	void* modify_ext_s;
	void* modrdn2W;
	void* modrdn2A;
	void* modrdnW;
	void* modrdnA;
	void* modrdn2_sW;
	void* modrdn2_sA;
	void* modrdn_sW;
	void* modrdn_sA;
	void* modrdn2;
	void* modrdn;
	void* modrdn2_s;
	void* modrdn_s;
	void* rename_extW;
	void* rename_extA;
	void* rename_ext_sW;
	void* rename_ext_sA;
	void* rename_ext;
	void* rename_ext_s;
	void* addW;
	void* addA;
	void* add_sW;
	void* add_sA;
	void* add_extW;
	void* add_extA;
	void* add_ext_sW;
	void* add_ext_sA;
	void* add;
	void* add_s;
	void* add_ext;
	void* add_ext_s;
	void* compareW;
	void* compareA;
	void* compare_sW;
	void* compare_sA;
	void* compare;
	void* compare_s;
	void* compare_extW;
	void* compare_extA;
	void* compare_ext_sW;
	void* compare_ext_sA;
	void* compare_ext;
	void* compare_ext_s;
	void* deleteW;
	void* deleteA;
	void* delete_sW;
	void* delete_sA;
	void* delete_extW;
	void* delete_extA;
	void* delete_ext_sW;
	void* delete_ext_sA;
	void* deleteC;
	void* delete_s;
	void* delete_ext;
	void* delete_ext_s;
	void* abandon;
	void* result;
	void* msgfree;
	void* result2error;
	void* parse_resultW;
	void* parse_resultA;
	void* parse_extended_resultA;
	void* parse_extended_resultW;
	void* controls_freeA;
	void* control_freeA;
	void* controls_freeW;
	void* control_freeW;
	void* free_controlsW;
	void* free_controlsA;
	void* parse_result;
	void* controls_free;
	void* control_free;
	void* free_controls;
	void* err2stringW;
	void* err2stringA;
	void* err2string;
	void* perror;
	void* first_entry;
	void* next_entry;
	void* count_entries;
	void* first_attributeW;
	void* first_attributeA;
	void* first_attribute;
	void* next_attributeW;
	void* next_attributeA;
	void* next_attribute;
	void* get_valuesW;
	void* get_valuesA;
	void* get_values;
	void* get_values_lenW;
	void* get_values_lenA;
	void* get_values_len;
	void* count_valuesW;
	void* count_valuesA;
	void* count_values;
	void* count_values_len;
	void* value_freeW;
	void* value_freeA;
	void* value_free;
	void* value_free_len;
	void* get_dnW;
	void* get_dnA;
	void* get_dn;
	void* explode_dnW;
	void* explode_dnA;
	void* explode_dn;
	void* dn2ufnW;
	void* dn2ufnA;
	void* dn2ufn;
	void* memfreeW;
	void* memfreeA;
	void* ber_bvfree;
	void* memfree;
	void* ufn2dnW;
	void* ufn2dnA;
	void* ufn2dn;
	void* startup;
	void* cleanup;
	void* escape_filter_elementW;
	void* escape_filter_elementA;
	void* escape_filter_element;
	void* set_dbg_flags;
	void* set_dbg_routine;
	void* LdapUTF8ToUnicode;
	void* LdapUnicodeToUTF8;
	void* create_sort_controlA;
	void* create_sort_controlW;
	void* parse_sort_controlA;
	void* parse_sort_controlW;
	void* create_sort_control;
	void* parse_sort_control;
	void* encode_sort_controlW;
	void* encode_sort_controlA;
	void* encode_sort_control;
	void* create_page_controlW;
	void* create_page_controlA;
	void* parse_page_controlW;
	void* parse_page_controlA;
	void* create_page_control;
	void* parse_page_control;
	void* search_init_pageW;
	void* search_init_pageA;
	void* search_init_page;
	void* get_next_page;
	void* get_next_page_s;
	void* get_paged_count;
	void* search_abandon_page;
	void* create_vlv_controlW;
	void* create_vlv_controlA;
	void* parse_vlv_controlW;
	void* parse_vlv_controlA;
	void* start_tls_sW;
	void* start_tls_sA;
	void* stop_tls_s;
	void* first_reference;
	void* next_reference;
	void* count_references;
	void* parse_referenceW;
	void* parse_referenceA;
	void* parse_reference;
	void* extended_operationW;
	void* extended_operationA;
	void* extended_operation_sA;
	void* extended_operation_sW;
	void* extended_operation;
	void* close_extended_op;
	void* LdapGetLastError;
	void* LdapMapErrorToWin32;
	void* conn_from_msg;

#if OSUnix
	void* ID;
#elif PXOSWindowsDestop
	struct ldap* ID;
	struct ldapmsg* SearchResult;
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

PXPublic PXActionResult PXAPI PXLDAPClienInitialize(PXLDAPClient* const pxLDAPClient);
PXPublic PXActionResult PXAPI PXLDAPClienRelease(PXLDAPClient* const pxLDAPClient);

PXPublic PXActionResult PXAPI PXLDAPClientOpen(PXLDAPClient* const pxLDAPClient, const PXLDAPConnectionInfo* const pxLDAPConnectionInfo);
PXPublic PXActionResult PXAPI PXLDAPClientClose(PXLDAPClient* const pxLDAPClient);

// Used an open connection to fetch data either syncrounous or asyncounous.
// The result is fetched into an adress that need to be cleared before the next call.
PXPublic PXActionResult PXAPI PXLDAPClientSearch(PXLDAPClient* const pxLDAPClient, PXLDAPSearchInfo* const pxLDAPSearchInfo);

#endif