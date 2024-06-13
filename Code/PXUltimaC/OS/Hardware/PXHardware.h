#ifndef PXHardwareInclude
#define PXHardwareInclude

#include <Media/PXResource.h>

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

	// 	datetime InstallDate;
}
PXSensorTemperature;

typedef struct PXProcessor
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
	//datetime InstallDate;
}
PXProcessor;

// 
typedef struct PXHardwareInfo_
{
	PXSize ProcessorListSize;
	PXProcessor ProcessorList[4];


#if OSWindows
	struct IWbemLocator* locator;
	struct IWbemServices* service;
	struct IEnumWbemClassObject* enumerator;
	struct IWbemClassObject* obj;
#endif

}
PXHardwareInfo;


PXPublic PXActionResult PXAPI PXHardwareInfoScan(PXHardwareInfo* const pxHardwareInfo);

#endif