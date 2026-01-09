#include "PXDNS.h"


#if OSUnix
#elif OSWindows
#include <windns.h>
#pragma comment(lib, "Dnsapi.lib")
// https://learn.microsoft.com/en-us/windows/win32/api/windns/
#endif

PXResult PXAPI PXDNSFetch(const char* name)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    PDNS_RECORD dnsRECORD;

    const DNS_STATUS dnsStatus = DnsQuery_UTF8
    (
        name,
        DNS_TYPE_NULL,
        DNS_TYPE_NULL,
        NULL,
        &dnsRECORD,
        NULL
    ); // Windows 2000, Dnsapi.dll, windns.h

    return PXResultOK;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}
