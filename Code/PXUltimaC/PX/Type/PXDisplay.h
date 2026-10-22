#pragma once

#ifndef PXDisplayIncluded
#define PXDisplayIncluded

#define PXDisplayScreenMonitorLength 32
#define PXDisplayScreenNameLength 32
#define PXDisplayScreenDeviceLength 128

// Container where windows can be created in
typedef struct PXDisplayScreen_
{
    char GraphicDeviceName[PXDisplayScreenDeviceLength];
    char NameMonitor[PXDisplayScreenMonitorLength];
    char NameID[PXDisplayScreenNameLength];

    int Width;
    int Height;
    int Cells;
    int Planes;
    int WidthMM;
    int HeightMM;


}
PXDisplayScreen;

typedef struct PXDisplay_
{
    int ScreenDefaultID;
    int ScreenListAmount;

    PXSize DisplayScreenListAmount;
    PXDisplayScreen* DisplayScreenList;

    PXBool IsConnected;
    PXBool IsPrimary;
}
PXDisplay;

#endif