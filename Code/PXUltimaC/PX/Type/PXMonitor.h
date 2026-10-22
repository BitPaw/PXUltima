#pragma once

#ifndef PXMonitorIncluded
#define PXMonitorIncluded

#define MonitorNameLength 32

#include <PX/Type/PXRectangle.h>

typedef struct PXMonitorHandle_
{
#if OSUnix
    int Data;
#elif OSWindows 
    HMONITOR Data;
#else
    void* Data;
#endif
}
PXMonitorHandle;

typedef struct PXMonitor_
{
    PXMonitorHandle Handle;

#if OSUnix
#elif OSWindows
    HDC DeviceContext;
#endif

    char Name[MonitorNameLength];
    char Driver[MonitorNameLength];

    PXRectangleXYWHI32 Form;
}
PXMonitor;

#endif