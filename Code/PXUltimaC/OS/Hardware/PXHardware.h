#ifndef PXHardwareInclude
#define PXHardwareInclude

#include <Media/PXResource.h>

typedef struct PXBIOS_
{
	PXInt16U   BiosCharacteristics[64];
	char   BIOSVersion[64];
	char   BuildNumber[64];
	char   Caption[64];
	char   CodeSet[64];
	char   CurrentLanguage[64];
	char   Description[64];
	PXInt8U    EmbeddedControllerMajorVersion;
	PXInt8U    EmbeddedControllerMinorVersion;
	char   IdentificationCode[64];
	PXInt16U   InstallableLanguages;
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
	PXInt16U   SMBIOSMajorVersion;
	PXInt16U   SMBIOSMinorVersion;
	PXBool  SMBIOSPresent;
	char   SoftwareElementID[64];
	PXInt16U   SoftwareElementState;
	char   Status[64];
	PXInt8U    SystemBiosMajorVersion;
	PXInt8U    SystemBiosMinorVersion;
	PXInt16U   TargetOperatingSystem;
	char   Version[64];
}
PXBIOS;

typedef struct PXPhysicalMemory_
{
	PXInt32U   Attributes;
	char   BankLabel[64];
	PXInt64U   Capacity;
	char   Caption[64];
	PXInt32U   ConfiguredClockSpeed;
	PXInt32U   ConfiguredVoltage;
	char   CreationClassName[64];
	PXInt16U   DataWidth;
	char   Description[64];
	char   DeviceLocator[64];
	PXInt16U   FormFactor;
	PXBool  HotSwappable;
	double InstallDate; // ???
	PXInt16U   InterleaveDataDepth;
	PXInt32U   InterleavePosition;
	char   Manufacturer[64];
	PXInt32U   MaxVoltage;
	PXInt16U   MemoryType;
	PXInt32U   MinVoltage;
	char   Model[64];
	char   Name[64];
	char   OtherIdentifyingInfo[64];
	char   PartNumber[64];
	PXInt32U   PositionInRow;
	PXBool  PoweredOn;
	PXBool  Removable;
	PXBool  Replaceable;
	char   SerialNumber[64];
	char   SKU[64];
	PXInt32U   SMBIOSMemoryType;
	PXInt32U   Speed;
	char   Status[64];
	char   Tag[64];
	PXInt16U   TotalWidth;
	PXInt16U   TypeDetail;
	char   Version[64];
}
PXPhysicalMemory;

typedef struct PXCCCCMemory_
{
	PXInt16U   Access;
	PXInt8U    AdditionalErrorData[10];
	PXInt16U   Availability;
	PXInt64U   BlockSize;
	char   Caption[64];
	PXInt32U   ConfigManagerErrorCode;
	PXBool  ConfigManagerUserConfig;
	PXBool  CorrectableError;
	char   CreationClassName[64];
	char   Description[64];
	char   DeviceID[64];
	PXInt64U   EndingAddress;
	PXInt16U   ErrorAccess;
	PXInt64U   ErrorAddress;
	PXBool  ErrorCleared;
	PXInt8U    ErrorData[10];
	PXInt16U   ErrorDataOrder;
	char   ErrorDescription[64];
	PXInt16U   ErrorGranularity;
	PXInt16U   ErrorInfo;
	char   ErrorMethodology[64];
	PXInt64U   ErrorResolution;
	double ErrorTime; // ??
	PXInt32U   ErrorTransferSize;
	double InstallDate;  // ??
	PXInt32U   LastErrorCode;
	char   Name[64];
	PXInt64U   NumberOfBlocks;
	char   OtherErrorDescription[64];
	char   PNPDeviceID[64];
	PXInt16U   PowerManagementCapabilities[10];
	PXBool  PowerManagementSupported;
	char   Purpose[64];
	PXInt64U   StartingAddress;
	char   Status[64];
	PXInt16U   StatusInfo;
	char   SystemCreationClassName[64];
	PXBool  SystemLevelAddress;
	char   SystemName[64];
}
PXCCCCMemory;

typedef struct PXCacheMemory_
{
	PXInt16U   Access;
	PXInt8U    AdditionalErrorData[10];
	PXInt16U   Associativity;
	PXInt16U   Availability;
	PXInt64U   BlockSize;
	PXInt32U   CacheSpeed;
	PXInt16U   CacheType;
	char   Caption[64];
	PXInt32U   ConfigManagerErrorCode;
	PXBool  ConfigManagerUserConfig;
	PXBool  CorrectableError;
	char   CreationClassName[64];
	PXInt16U   CurrentSRAM[10];
	char   Description[64];
	char   DeviceID[64];
	PXInt64U   EndingAddress;
	PXInt16U   ErrorAccess;
	PXInt64U   ErrorAddress;
	PXBool  ErrorCleared;
	PXInt16U   ErrorCorrectType;
	PXInt8U    ErrorData[10];
	PXInt16U   ErrorDataOrder;
	char   ErrorDescription[64];
	PXInt16U   ErrorInfo;
	char   ErrorMethodology[64];
	PXInt64U   ErrorResolution;
	double ErrorTime; // !
	PXInt32U   ErrorTransferSize;
	PXInt32U   FlushTimer;
	double InstallDate; // !
	PXInt32U   InstalledSize;
	PXInt32U   LastErrorCode;
	PXInt16U   Level;
	PXInt32U   LineSize;
	PXInt16U   Location;
	PXInt32U   MaxCacheSize;
	char   Name[64];
	PXInt64U   NumberOfBlocks;
	char   OtherErrorDescription[64];
	char   PNPDeviceID[64];
	PXInt16U   PowerManagementCapabilities[10];
	PXBool  PowerManagementSupported;
	char   Purpose[64];
	PXInt16U   ReadPolicy;
	PXInt16U   ReplacementPolicy;
	PXInt64U   StartingAddress;
	char   Status[64];
	PXInt16U   StatusInfo;
	PXInt16U   SupportedSRAM[10];
	char   SystemCreationClassName[64];
	PXBool  SystemLevelAddress;
	char   SystemName[64];
	PXInt16U   WritePolicy;
}
PXCacheMemory;

typedef struct PXVideoDevice_
{
	PXInt16U   AcceleratorCapabilities[10];
	char   AdapterCompatibility[64];
	char   AdapterDACType[64];
	PXInt32U   AdapterRAM;
	PXInt16U   Availability;
	char   CapabilityDescriptions[64];
	char   Caption[64];
	PXInt32U   ColorTableEntries;
	PXInt32U   ConfigManagerErrorCode;
	PXBool  ConfigManagerUserConfig;
	char   CreationClassName;
	PXInt32U   CurrentBitsPerPixel;
	PXInt32U   CurrentHorizontalResolution;
	PXInt64U   CurrentNumberOfColors;
	PXInt32U   CurrentNumberOfColumns;
	PXInt32U   CurrentNumberOfRows;
	PXInt32U   CurrentRefreshRate;
	PXInt16U   CurrentScanMode;
	PXInt32U   CurrentVerticalResolution;
	char   Description[64];
	char   DeviceID[64];
	PXInt32U   DeviceSpecificPens;
	PXInt32U   DitherType;
	float DriverDate; // Format wierd !!
	char   DriverVersion[64];
	PXBool  ErrorCleared;
	char   ErrorDescription[64];
	PXInt32U   ICMIntent;
	PXInt32U   ICMMethod;
	char   InfFilename[64];
	char   InfSection[64];
	double InstallDate; // !
	char   InstalledDisplayDrivers[64];
	PXInt32U   LastErrorCode;
	PXInt32U   MaxMemorySupported;
	PXInt32U   MaxNumberControlled;
	PXInt32U   MaxRefreshRate;
	PXInt32U   MinRefreshRate;
	PXBool  Monochrome;
	char   Name[64];
	PXInt16U   NumberOfColorPlanes;
	PXInt32U   NumberOfVideoPages;
	char   PNPDeviceID[64];
	PXInt16U   PowerManagementCapabilities[10];
	PXBool  PowerManagementSupported;
	PXInt16U   ProtocolSupported;
	PXInt32U   ReservedSystemPaletteEntries;
	PXInt32U   SpecificationVersion;
	char   Status[64];
	PXInt16U   StatusInfo;
	char   SystemCreationClassName[64];
	char   SystemName[64];
	PXInt32U   SystemPaletteEntries;
	double TimeOfLastReset; // !
	PXInt16U   VideoArchitecture;
	PXInt16U   VideoMemoryType;
	PXInt16U   VideoMode;
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
	char Status[64];
	char SystemCreationClassName[64];
	char SystemName[64];

	PXInt16U   Availability;
	PXInt16U   PowerManagementCapabilities[5];
	PXInt16U   StatusInfo;

	PXInt32U   ConfigManagerErrorCode;
	PXInt32U   LastErrorCode;

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
	char Status[64];
	char SystemCreationClassName[64];
	char SystemName[64];

	PXInt32S   Accuracy;
	PXInt32U   ConfigManagerErrorCode;
	PXInt32S   CurrentReading;
	PXInt32U   LastErrorCode;
	PXInt32S   LowerThresholdCritical;
	PXInt32S   LowerThresholdFatal;
	PXInt32S   LowerThresholdNonCritical;
	PXInt32S   MaxReadable;
	PXInt32S   MinReadable;
	PXInt32S   NominalReading;
	PXInt32S   NormalMax;
	PXInt32S   NormalMin;
	PXInt32U   Resolution;
	PXInt32S   Tolerance;
	PXInt32S   UpperThresholdCritical;
	PXInt32S   UpperThresholdFatal;
	PXInt32S   UpperThresholdNonCritical;

	PXInt16U   Availability;
	PXInt16U   PowerManagementCapabilities[5];
	PXInt16U   StatusInfo;

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
	char Status[64];
	char Stepping[64];
	char SystemCreationClassName[64];
	char SystemName[64];
	char UniqueId[64];
	char Version[64];

	// 32x
	PXInt32U   Characteristics;
	PXInt32U   ConfigManagerErrorCode;
	PXInt32U   CurrentClockSpeed;
	PXInt32U   ExtClock;
	PXInt32U   L2CacheSize;
	PXInt32U   L2CacheSpeed;
	PXInt32U   L3CacheSize;
	PXInt32U   L3CacheSpeed;
	PXInt32U   LastErrorCode;
	PXInt32U   MaxClockSpeed;
	PXInt32U   NumberOfCores;
	PXInt32U   NumberOfEnabledCore;
	PXInt32U   NumberOfLogicalProcessors;
	PXInt32U   ThreadCount;
	PXInt32U   VoltageCaps;


	// 16x
	PXInt16U   AddressWidth;
	PXInt16U   Architecture;
	PXInt16U   Availability;
	PXInt16U   CpuStatus;
	PXInt16U   CurrentVoltage;
	PXInt16U   DataWidth;
	PXInt16U   Family;
	PXInt16U   Level;
	PXInt16U   LoadPercentage;
	PXInt16U   PowerManagementCapabilities[5];
	PXInt16U   ProcessorType;
	PXInt16U   Revision;
	PXInt16U   StatusInfo;
	PXInt16U   UpgradeMethod;

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

PXPublic PXActionResult PXAPI PXHardwareInfoScan(PXHardwareInfo* const pxHardwareInfo, const PXInt32U fetchFlags);

#endif