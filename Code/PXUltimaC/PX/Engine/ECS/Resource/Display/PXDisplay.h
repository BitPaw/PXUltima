


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
#if OSUnix
    Display* DisplayHandle;
    Window WindowRootHandle;
    GC GraphicContent;
#elif OSWindows
    int WindowRootHandle;
    void* DisplayHandle;
    int GraphicContent;
#endif

    char* Data;
    char* Name;

    int ProtocolVersion;
    int ProtocolRevision;

    char* ServerVendor;
    int VendorRelease;

    int ScreenDefaultID;
    int ScreenListAmount;

    PXSize DisplayScreenListAmount;
    PXDisplayScreen* DisplayScreenList;

    PXBool IsConnected;
    PXBool IsPrimary;
}
PXDisplay;