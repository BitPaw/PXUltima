#pragma once

#ifndef PXMonitorIncluded
#define PXMonitorIncluded

#define MonitorNameLength 32

typedef struct PXMonitor_
{
#if OSUnix
#elif OSWindows
    HMONITOR ID;
    HDC DeciveContect;
#endif

    char Name[MonitorNameLength];
    char Driver[MonitorNameLength];

    PXI32U Left;
    PXI32U Top;
    PXI32U Right;
    PXI32U Bottom;

    //PXI16U X;
    //PXI16U Y;
    //PXI16U Width;
    //PXI16U Height;
}
PXMonitor;

#endif