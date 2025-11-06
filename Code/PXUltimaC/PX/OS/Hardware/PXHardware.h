#pragma once

#ifndef PXHardwareIncluded
#define PXHardwareIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXBIOS_
{
    PXI16U   BiosCharacteristics[64];
    char   BIOSVersion[64];
    char   BuildNumber[64];
    char   Caption[64];
    char   CodeSet[64];
    char   CurrentLanguage[64];
    char   Description[64];
    PXI8U    EmbeddedControllerMajorVersion;
    PXI8U    EmbeddedControllerMinorVersion;
    char   IdentificationCode[64];
    PXI16U   InstallableLanguages;
    double InstallDate; // ???
    char   LanguageEdition[64];
    char   ListOfLanguages[64];
    char   Manufacturer[64];
    char   Name[64];
    char   OtherTargetOS[64];
    PXBool  PrimaryBIOS;
    double ReleaseDate; // ???
    char   SerialNumber[64];
    char   SMBIOSBIOSVersion[64];
    PXI16U   SMBIOSMajorVersion;
    PXI16U   SMBIOSMinorVersion;
    PXBool  SMBIOSPresent;
    char   SoftwareElementID[64];
    PXI16U   SoftwareElementState;
    char   BIOSStatus[64];
    PXI8U    SystemBiosMajorVersion;
    PXI8U    SystemBiosMinorVersion;
    PXI16U   TargetOperatingSystem;
    char   Version[64];
}
PXBIOS;

typedef struct PXPhysicalMemory_
{
    PXI32U   Attributes;
    char   BankLabel[64];
    PXI64U   Capacity;
    char   Caption[64];
    PXI32U   ConfiguredClockSpeed;
    PXI32U   ConfiguredVoltage;
    char   CreationClassName[64];
    PXI16U   DataWidth;
    char   Description[64];
    char   DeviceLocator[64];
    PXI16U   FormFactor;
    PXBool  HotSwappable;
    double InstallDate; // ???
    PXI16U   InterleaveDataDepth;
    PXI32U   InterleavePosition;
    char   Manufacturer[64];
    PXI32U   MaxVoltage;
    PXI16U   MemoryType;
    PXI32U   MinVoltage;
    char   Model[64];
    char   Name[64];
    char   OtherIdentifyingInfo[64];
    char   PartNumber[64];
    PXI32U   PositionInRow;
    PXBool  PoweredOn;
    PXBool  Removable;
    PXBool  Replaceable;
    char   SerialNumber[64];
    char   SKU[64];
    PXI32U   SMBIOSMemoryType;
    PXI32U   Speed;
    char   MemoryStatus[64];
    char   Tag[64];
    PXI16U   TotalWidth;
    PXI16U   TypeDetail;
    char   Version[64];
}
PXPhysicalMemory;

typedef struct PXCCCCMemory_
{
    PXI16U   Access;
    PXI8U    AdditionalErrorData[10];
    PXI16U   Availability;
    PXI64U   BlockSize;
    char   Caption[64];
    PXI32U   ConfigManagerErrorCode;
    PXBool  ConfigManagerUserConfig;
    PXBool  CorrectableError;
    char   CreationClassName[64];
    char   Description[64];
    char   DeviceID[64];
    PXI64U   EndingAddress;
    PXI16U   ErrorAccess;
    PXI64U   ErrorAddress;
    PXBool  ErrorCleared;
    PXI8U    ErrorData[10];
    PXI16U   ErrorDataOrder;
    char   ErrorDescription[64];
    PXI16U   ErrorGranularity;
    PXI16U   ErrorInfo;
    char   ErrorMethodology[64];
    PXI64U   ErrorResolution;
    double ErrorTime; // ??
    PXI32U   ErrorTransferSize;
    double InstallDate;  // ??
    PXI32U   LastErrorCode;
    char   Name[64];
    PXI64U   NumberOfBlocks;
    char   OtherErrorDescription[64];
    char   PNPDeviceID[64];
    PXI16U   PowerManagementCapabilities[10];
    PXBool  PowerManagementSupported;
    char   Purpose[64];
    PXI64U   StartingAddress;
    char   CCStatus[64];
    PXI16U   StatusInfo;
    char   SystemCreationClassName[64];
    PXBool  SystemLevelAddress;
    char   SystemName[64];
}
PXCCCCMemory;

typedef struct PXCacheMemory_
{
    PXI16U   Access;
    PXI8U    AdditionalErrorData[10];
    PXI16U   Associativity;
    PXI16U   Availability;
    PXI64U   BlockSize;
    PXI32U   CacheSpeed;
    PXI16U   CacheType;
    char   Caption[64];
    PXI32U   ConfigManagerErrorCode;
    PXBool  ConfigManagerUserConfig;
    PXBool  CorrectableError;
    char   CreationClassName[64];
    PXI16U   CurrentSRAM[10];
    char   Description[64];
    char   DeviceID[64];
    PXI64U   EndingAddress;
    PXI16U   ErrorAccess;
    PXI64U   ErrorAddress;
    PXBool  ErrorCleared;
    PXI16U   ErrorCorrectType;
    PXI8U    ErrorData[10];
    PXI16U   ErrorDataOrder;
    char   ErrorDescription[64];
    PXI16U   ErrorInfo;
    char   ErrorMethodology[64];
    PXI64U   ErrorResolution;
    double ErrorTime; // !
    PXI32U   ErrorTransferSize;
    PXI32U   FlushTimer;
    double InstallDate; // !
    PXI32U   InstalledSize;
    PXI32U   LastErrorCode;
    PXI16U   Level;
    PXI32U   LineSize;
    PXI16U   Location;
    PXI32U   MaxCacheSize;
    char   Name[64];
    PXI64U   NumberOfBlocks;
    char   OtherErrorDescription[64];
    char   PNPDeviceID[64];
    PXI16U   PowerManagementCapabilities[10];
    PXBool  PowerManagementSupported;
    char   Purpose[64];
    PXI16U   ReadPolicy;
    PXI16U   ReplacementPolicy;
    PXI64U   StartingAddress;
    char   MemoryStatus[64];
    PXI16U   StatusInfo;
    PXI16U   SupportedSRAM[10];
    char   SystemCreationClassName[64];
    PXBool  SystemLevelAddress;
    char   SystemName[64];
    PXI16U   WritePolicy;
}
PXCacheMemory;

typedef struct PXVideoDevice_
{
    PXI16U   AcceleratorCapabilities[10];
    char   AdapterCompatibility[64];
    char   AdapterDACType[64];
    PXI32U   AdapterRAM;
    PXI16U   Availability;
    char   CapabilityDescriptions[64];
    char   Caption[64];
    PXI32U   ColorTableEntries;
    PXI32U   ConfigManagerErrorCode;
    PXBool  ConfigManagerUserConfig;
    char   CreationClassName;
    PXI32U   CurrentBitsPerPixel;
    PXI32U   CurrentHorizontalResolution;
    PXI64U   CurrentNumberOfColors;
    PXI32U   CurrentNumberOfColumns;
    PXI32U   CurrentNumberOfRows;
    PXI32U   CurrentRefreshRate;
    PXI16U   CurrentScanMode;
    PXI32U   CurrentVerticalResolution;
    char   Description[64];
    char   DeviceID[64];
    PXI32U   DeviceSpecificPens;
    PXI32U   DitherType;
    PXF32 DriverDate; // Format wierd !!
    char   DriverVersion[64];
    PXBool  ErrorCleared;
    char   ErrorDescription[64];
    PXI32U   ICMIntent;
    PXI32U   ICMMethod;
    char   InfFilename[64];
    char   InfSection[64];
    double InstallDate; // !
    char   InstalledDisplayDrivers[64];
    PXI32U   LastErrorCode;
    PXI32U   MaxMemorySupported;
    PXI32U   MaxNumberControlled;
    PXI32U   MaxRefreshRate;
    PXI32U   MinRefreshRate;
    PXBool  Monochrome;
    char   Name[64];
    PXI16U   NumberOfColorPlanes;
    PXI32U   NumberOfVideoPages;
    char   PNPDeviceID[64];
    PXI16U   PowerManagementCapabilities[10];
    PXBool  PowerManagementSupported;
    PXI16U   ProtocolSupported;
    PXI32U   ReservedSystemPaletteEntries;
    PXI32U   SpecificationVersion;
    char   VideoStatus[64];
    PXI16U   StatusInfo;
    char   SystemCreationClassName[64];
    char   SystemName[64];
    PXI32U   SystemPaletteEntries;
    double TimeOfLastReset; // !
    PXI16U   VideoArchitecture;
    PXI16U   VideoMemoryType;
    PXI16U   VideoMode;
    char   VideoModeDescription[64];
    char   VideoProcessor[64];
}
PXVideoDevice;

typedef struct PXMainBoard_
{
    char Caption[64];
    char CreationClassName[64];
    char Description[64];
    char DeviceID[64];
    char ErrorDescription[64];
    char Name[64];
    char PNPDeviceID[64];
    char PrimaryBusType[64];
    char RevisionNumber[64];
    char SecondaryBusType[64];
    char BoardStatus[64];
    char SystemCreationClassName[64];
    char SystemName[64];

    PXI16U   Availability;
    PXI16U   PowerManagementCapabilities[5];
    PXI16U   StatusInfo;

    PXI32U   ConfigManagerErrorCode;
    PXI32U   LastErrorCode;

    double InstallDate; // !

    PXBool  ConfigManagerUserConfig;
    PXBool  ErrorCleared;
    PXBool  PowerManagementSupported;
}
PXMainBoard;

typedef struct PXSensorTemperature_
{
    char Caption[64];
    char CreationClassName[64];
    char Description[64];
    char DeviceID[64];
    char ErrorDescription[64];
    char Name[64];
    char PNPDeviceID[64];
    char SensorStatus[64];
    char SystemCreationClassName[64];
    char SystemName[64];

    PXI32S   Accuracy;
    PXI32U   ConfigManagerErrorCode;
    PXI32S   CurrentReading;
    PXI32U   LastErrorCode;
    PXI32S   LowerThresholdCritical;
    PXI32S   LowerThresholdFatal;
    PXI32S   LowerThresholdNonCritical;
    PXI32S   MaxReadable;
    PXI32S   MinReadable;
    PXI32S   NominalReading;
    PXI32S   NormalMax;
    PXI32S   NormalMin;
    PXI32U   Resolution;
    PXI32S   Tolerance;
    PXI32S   UpperThresholdCritical;
    PXI32S   UpperThresholdFatal;
    PXI32S   UpperThresholdNonCritical;

    PXI16U   Availability;
    PXI16U   PowerManagementCapabilities[5];
    PXI16U   StatusInfo;

    PXBool ConfigManagerUserConfig;
    PXBool ErrorCleared;
    PXBool IsLinear;
    PXBool PowerManagementSupported;

    double InstallDate; // !
}
PXSensorTemperature;

typedef struct PXProcessorWS
{
    // Strings
    char AssetTag[64];
    char Caption[64];
    char CreationClassName[64];
    char Description[64];
    char DeviceID[64];
    char ErrorDescription[64];
    char Manufacturer[64];
    char Name[64];
    char OtherFamilyDescription[64];
    char PartNumber[64];
    char PNPDeviceID[64];
    char ProcessorId[64];
    char Role[64];
    char SerialNumber[64];
    char SocketDesignation[64];
    char ProcessorStatus[64];
    char Stepping[64];
    char SystemCreationClassName[64];
    char SystemName[64];
    char UniqueId[64];
    char Version[64];

    // 32x
    PXI32U   Characteristics;
    PXI32U   ConfigManagerErrorCode;
    PXI32U   CurrentClockSpeed;
    PXI32U   ExtClock;
    PXI32U   L2CacheSize;
    PXI32U   L2CacheSpeed;
    PXI32U   L3CacheSize;
    PXI32U   L3CacheSpeed;
    PXI32U   LastErrorCode;
    PXI32U   MaxClockSpeed;
    PXI32U   NumberOfCores;
    PXI32U   NumberOfEnabledCore;
    PXI32U   NumberOfLogicalProcessors;
    PXI32U   ThreadCount;
    PXI32U   VoltageCaps;


    // 16x
    PXI16U   AddressWidth;
    PXI16U   Architecture;
    PXI16U   Availability;
    PXI16U   CpuStatus;
    PXI16U   CurrentVoltage;
    PXI16U   DataWidth;
    PXI16U   Family;
    PXI16U   Level;
    PXI16U   LoadPercentage;
    PXI16U   PowerManagementCapabilities[5];
    PXI16U   ProcessorType;
    PXI16U   Revision;
    PXI16U   StatusInfo;
    PXI16U   UpgradeMethod;

    // bool
    PXBool  ConfigManagerUserConfig;
    PXBool  ErrorCleared;
    PXBool  PowerManagementSupported;
    PXBool  SecondLevelAddressTranslationExtensions;
    PXBool  VirtualizationFirmwareEnabled;
    PXBool  VMMonitorModeExtensions;


    // Date?
    double InstallDate; // !
}
PXProcessorWS;



#define PXBatteryPresentMASK        0b00000011
#define PXBatteryPresentINVALID     0b00000000
#define PXBatteryPresentONLINE      0b00000001
#define PXBatteryPresentOFFLINE     0b00000010
#define PXBatteryPresentUNKNOWN     0b00000011

#define PXBatteryCapacityMASK       0b00011100
#define PXBatteryCapacityNORMAL     0b00000000
#define PXBatteryCapacityHIGH       0b00000100 // High—the battery capacity is at more than 66 percent
#define PXBatteryCapacityLOW        0b00001000 // Low—the battery capacity is at less than 33 percent
#define PXBatteryCapacityCRITICAL   0b00001100 // Critical—the battery capacity is at less than five percent
#define PXBatteryCapacityCHARGING   0b00010000 // Charging
#define PXBatteryCapacityNOBATTERY  0b00010100 // No system battery
#define PXBatteryCapacityUNKOWN     0b00011000 // Unknown status—unable to read the battery flag information

#define PXBatteryPowerSavingEnable 0b00100000


#define PXBatteryModeCharging
#define PXBatteryModeDischarge
#define PXBatteryType
#define PXBatteryTypeLIthiumIon

typedef struct PXBattery_
{
    PXI8U StatusFlag;
    PXI8U LifePercent; // 0-100, 255=Unkown
    PXI32U LifeTime;
    PXI32U FullLifeTime;

    // Info
    char Name[16];
    char MaterialTechnology[16];
    char CAPACITY_LEVEL[16];
    char MODEL_NAME[16];
    char MANUFACTURER[16];
    PXBool PRESENT;
    PXI8U SERIAL_NUMBER;

    // Status
    char ChargeStatus[16];
    PXI8U CAPACITY;
    PXI32U CYCLE_COUNT;

    // A _ Ampere
    PXI32U CURRENT_NOW;

    // V - Voltage
    PXI32U VOLTAGE_MIN_DESIGN;
    PXI32U VOLTAGE_NOW;

    // C - Capacity
    PXI32U ChargeCurrent; // µAh
    PXI32U ChargeMaximumPractical; // µAh
    PXI32U ChargeMaximalTheroetical; // µAh
}
PXBattery;



PXPublic PXResult PXAPI PXHardwareBatteryFetch(PXBattery PXREF pxBattery);




//
typedef struct PXHardwareInfo_
{
    PXSize ProcessorListSize;
    PXProcessorWS ProcessorList[4];

    PXMainBoard MainBoard;

    PXSize CCCCMemorySize;
    PXCCCCMemory CCCCMemoryList[8];

    PXSize PhysicalMemoryListSize;
    PXPhysicalMemory PhysicalMemoryList[4];

    PXSize CacheMemorySize;
    PXCacheMemory CacheMemoryList[4]; // L1, L2, L3 Cache

    PXSize VideoDeviceSize;
    PXVideoDevice VideoDeviceList[4];

    PXBIOS BIOS;

#if OSWindows
    struct IWbemLocator* locator;
    struct IWbemServices* service;
    struct IEnumWbemClassObject* enumerator;
    struct IWbemClassObject* obj;
#endif

}
PXHardwareInfo;


#define PXHardwareInfoProcesor (1 << 0)
#define PXHardwareInfoHardDrive (1 << 0)
#define PXHardwareInfoDisplay (1 << 0)
#define PXHardwareInfoAll PXHardwareInfoProcesor | PXHardwareInfoHardDrive | PXHardwareInfoDisplay

PXPublic PXResult PXAPI PXHardwareInfoScan(PXHardwareInfo PXREF pxHardwareInfo, const PXI32U fetchFlags);

PXPublic PXResult PXAPI PXHardwareBattery(PXBattery PXREF pxBattery);

#endif
