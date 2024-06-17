#include "PXHardware.h"

#include <Windows.h>
#include <WbemIdl.h>
#include <WbemCli.h>

typedef struct PXWindowsWMIEntry_
{
    enum VARENUM VarriantType;
    char* FieldName; 
    void* DataAdress;
}
PXWindowsWMIEntry;

void PXAPI PXWindowsWMIExtractValue(PXHardwareInfo* const pxHardwareInfo, PXWindowsWMIEntry* ValFetchList, int amount)
{
    for(size_t i = 0; i < amount; i++)
    {
        PXWindowsWMIEntry* const valFetch = &ValFetchList[i];

        wchar_t nameW[64];

        MultiByteToWideChar(CP_ACP, MB_COMPOSITE, valFetch->FieldName, -1, nameW, 64);

        VARIANT varriant;

        const HRESULT hr = pxHardwareInfo->obj->lpVtbl->Get(pxHardwareInfo->obj, nameW, 0, &varriant, PXNull, PXNull);
        const PXBool success = SUCCEEDED(hr);
        const PXBool isExpectedType = V_VT(&varriant) == valFetch->VarriantType;

        const PXBool isString = valFetch->VarriantType == VT_BSTR;

        switch(varriant.vt)
        {
            case VT_BSTR:
            {
                BSTR stri = varriant.bstrVal; // wchar_t

                int x = WideCharToMultiByte(CP_UTF8, 0, stri, -1, valFetch->DataAdress, 128, NULL, NULL);



                break;
            }
            case VT_I4:
            {
                int* val = (int*)valFetch->DataAdress;

                *val = varriant.intVal;

                break;
            }

            default:
                break;
        }

        VariantClear(&varriant);
    }
}

PXBool PXAPI PXWindowsWMIPathOpen(PXHardwareInfo* const pxHardwareInfo, const char* pathName)
{
    wchar_t filePath[32];

    MultiByteToWideChar(CP_ACP, MB_COMPOSITE, pathName, -1, filePath, 32);

    HRESULT tes = pxHardwareInfo->locator->lpVtbl->ConnectServer
    (
        pxHardwareInfo->locator,
        filePath,
        PXNull,
        PXNull,
        PXNull,
        0,
        PXNull,
        PXNull,
        &pxHardwareInfo->service
    );

    if(pxHardwareInfo->service)
    {
        tes = CoSetProxyBlanket
        (
            pxHardwareInfo->service,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            PXNull,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            PXNull,
            EOAC_NONE
        );
    }
}


PXBool PXAPI PXWindowsWMIClassOpen(PXHardwareInfo* const pxHardwareInfo, const char* className)
{
    if(!pxHardwareInfo->service)
        return PXFalse;

    wchar_t wql[] = L"WQL";
    wchar_t classNameW[128];
    wchar_t query[128];

    MultiByteToWideChar(CP_ACP, MB_COMPOSITE, className, -1, classNameW, 128);

    wsprintfW(query, L"SELECT * FROM %s", classNameW);

    const HRESULT xx = pxHardwareInfo->service->lpVtbl->ExecQuery
    (
        pxHardwareInfo->service,
        wql,
        query,
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        PXNull,
        &pxHardwareInfo->enumerator
    );

    return PXTrue;
}

PXActionResult PXAPI PXHardwareInfoScan(PXHardwareInfo* const pxHardwareInfo, const PXInt32U fetchFlags)
{
    PXClear(PXHardwareInfo, pxHardwareInfo);

    IWbemLocator* locator = PXNull;

    // Init
    {
        HRESULT res = CoInitializeSecurity
        (
            PXNull,
            -1,
            PXNull,
            PXNull,
            RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
            PXNull,
            EOAC_NONE, 
            PXNull
        );
        res &= CoInitializeEx(PXNull, COINIT_MULTITHREADED);
        res &= CoCreateInstance(&CLSID_WbemLocator, PXNull, CLSCTX_INPROC_SERVER, &IID_IWbemLocator, (LPVOID*)&pxHardwareInfo->locator);
      

        locator = pxHardwareInfo->locator;

        if(!locator)
        {
            return;
        }

        
    }

    PXWindowsWMIPathOpen(pxHardwareInfo, "root\\wmi");

    // TODO: does not work!

    // termal
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "MSAcpi_ThermalZoneTemperature");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

       // pxHardwareInfo->enumerator->lpVtbl->Reset(pxHardwareInfo->enumerator);

        for(PXSize processorID = 0; pxHardwareInfo->enumerator; ++processorID)
        {
            PXSensorTemperature xxxSensorTemperature;
            PXClear(PXSensorTemperature, &xxxSensorTemperature);

            PXSensorTemperature* const pxSensorTemperature = &xxxSensorTemperature;

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);          
            PXActionResult xx = PXWindowsHandleErrorFromID(EJIE);

            if(!u_return)
            {
                break;
            }

            PXWindowsWMIEntry valFetchList[] =
            {
  {VT_I4   ,"Accuracy", &pxSensorTemperature->Accuracy},
  {VT_UI2   ,"Availability", &pxSensorTemperature->Availability},
  {VT_BSTR   ,"Caption", pxSensorTemperature->Caption},
  {VT_UI4   ,"ConfigManagerErrorCode", &pxSensorTemperature->ConfigManagerErrorCode},
  {VT_BOOL  ,"ConfigManagerUserConfig", &pxSensorTemperature->ConfigManagerUserConfig},
  {VT_BSTR   ,"CreationClassName", pxSensorTemperature->CreationClassName},
  {VT_I4   ,"CurrentReading", &pxSensorTemperature->CurrentReading},
  {VT_BSTR   ,"Description", pxSensorTemperature->Description},
  {VT_BSTR   ,"DeviceID", pxSensorTemperature->DeviceID},
  {VT_BOOL  ,"ErrorCleared", &pxSensorTemperature->ErrorCleared},
  {VT_BSTR   ,"ErrorDescription", pxSensorTemperature->ErrorDescription},
  //{datetime ,"InstallDate", pxSensorTemperature->InstallDate},
  {VT_BOOL  ,"IsLinear", &pxSensorTemperature->IsLinear},
  {VT_I4   ,"LastErrorCode", &pxSensorTemperature->LastErrorCode},
  {VT_I4   ,"LowerThresholdCritical", &pxSensorTemperature->LowerThresholdCritical},
  {VT_I4   ,"LowerThresholdFatal", &pxSensorTemperature->LowerThresholdFatal},
  {VT_I4   ,"LowerThresholdNonCritical", &pxSensorTemperature->LowerThresholdNonCritical},
  {VT_I4   ,"MaxReadable", &pxSensorTemperature->MaxReadable},
  {VT_I4   ,"MinReadable", &pxSensorTemperature->MinReadable},
  {VT_BSTR   ,"Name", pxSensorTemperature->Name},
  {VT_I4   ,"NominalReading", &pxSensorTemperature->NominalReading},
  {VT_I4   ,"NormalMax", &pxSensorTemperature->NormalMax},
  {VT_I4   ,"NormalMin", &pxSensorTemperature->NormalMin},
  {VT_BSTR   ,"PNPDeviceID", pxSensorTemperature->PNPDeviceID},
  {VT_UI2   ,"PowerManagementCapabilities[]", &pxSensorTemperature->PowerManagementCapabilities},
  {VT_BOOL  ,"PowerManagementSupported", &pxSensorTemperature->PowerManagementSupported},
  {VT_UI4   ,"Resolution", &pxSensorTemperature->Resolution},
  {VT_BSTR   ,"Status", pxSensorTemperature->Status},
  {VT_UI2   ,"StatusInfo", &pxSensorTemperature->StatusInfo},
  {VT_BSTR   ,"SystemCreationClassName", pxSensorTemperature->SystemCreationClassName},
  {VT_BSTR   ,"SystemName", pxSensorTemperature->SystemName},
  {VT_I4   ,"Tolerance", &pxSensorTemperature->Tolerance},
  {VT_I4   ,"UpperThresholdCritical", &pxSensorTemperature->UpperThresholdCritical},
  {VT_I4   ,"UpperThresholdFatal", &pxSensorTemperature->UpperThresholdFatal},
  {VT_I4   ,"UpperThresholdNonCritical", &pxSensorTemperature->UpperThresholdNonCritical}                
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }


    PXWindowsWMIPathOpen(pxHardwareInfo, "ROOT\\CIMV2");


    // Processor
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_Processor");

        if(!success) 
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize processorID = 0 ; pxHardwareInfo->enumerator ; ++processorID)
        {
            PXProcessor* const pxProcessor = &pxHardwareInfo->ProcessorList[processorID];

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);
        
            if(!u_return) 
            {
                break;
            }
       
            ++(pxHardwareInfo->ProcessorListSize);
        

            PXWindowsWMIEntry valFetchList[] =
            {
  {VT_UI2,   "AddressWidth", &pxProcessor->AddressWidth},
  {VT_UI2,   "Architecture", &pxProcessor->Architecture},
  {VT_BSTR,    "AssetTag", pxProcessor->AssetTag},
  {VT_UI2,   "Availability", &pxProcessor->Availability},
  {VT_BSTR,    "Caption", pxProcessor->Caption},
  {VT_UI4,   "Characteristics", &pxProcessor->Characteristics},
  {VT_UI4,   "ConfigManagerErrorCode", &pxProcessor->ConfigManagerErrorCode},
  {VT_BOOL,  "ConfigManagerUserConfig", &pxProcessor->ConfigManagerUserConfig},
  {VT_UI2,   "CpuStatus", &pxProcessor->CpuStatus},
  {VT_BSTR,    "CreationClassName", pxProcessor->CreationClassName},
  {VT_UI4,   "CurrentClockSpeed", &pxProcessor->CurrentClockSpeed},
  {VT_UI2,   "CurrentVoltage", &pxProcessor->CurrentVoltage},
  {VT_UI2,   "DataWidth", &pxProcessor->DataWidth},
  {VT_BSTR,    "Description", pxProcessor->Description},
  {VT_BSTR,    "DeviceID", pxProcessor->DeviceID},
  {VT_BOOL,  "ErrorCleared", &pxProcessor->ErrorCleared},
  {VT_BSTR, "ErrorDescription", pxProcessor->ErrorDescription},
  {VT_UI4,   "ExtClock", &pxProcessor->ExtClock},
  {VT_UI2,   "Family", &pxProcessor->Family},
  //{VT_DATE, "InstallDate", &pxProcessor->InstallDate},
  {VT_UI4,   "L2CacheSize", &pxProcessor->L2CacheSize},
  {VT_UI4,   "L2CacheSpeed", &pxProcessor->L2CacheSpeed},
  {VT_UI4,   "L3CacheSize", &pxProcessor->L3CacheSize},
  {VT_UI4,   "L3CacheSpeed", &pxProcessor->L3CacheSpeed},
  {VT_UI4,   "LastErrorCode", &pxProcessor->LastErrorCode},
  {VT_UI2,   "Level", &pxProcessor->Level},
  {VT_UI2,   "LoadPercentage", &pxProcessor->LoadPercentage},
  {VT_BSTR, "Manufacturer", pxProcessor->Manufacturer},
  {VT_UI4,   "MaxClockSpeed", &pxProcessor->MaxClockSpeed},
  {VT_BSTR, "Name", pxProcessor->Name},
  {VT_UI4,   "NumberOfCores", &pxProcessor->NumberOfCores},
  {VT_UI4,   "NumberOfEnabledCore", &pxProcessor->NumberOfEnabledCore},
  {VT_UI4,   "NumberOfLogicalProcessors", &pxProcessor->NumberOfLogicalProcessors},
  {VT_BSTR, "OtherFamilyDescription", pxProcessor->OtherFamilyDescription},
  {VT_BSTR, "PartNumber", pxProcessor->PartNumber},
  {VT_BSTR, "PNPDeviceID", pxProcessor->PNPDeviceID},
  {VT_UI2,   "PowerManagementCapabilities[]", &pxProcessor->PowerManagementCapabilities},
  {VT_BOOL,  "PowerManagementSupported", &pxProcessor->PowerManagementSupported},
  {VT_BSTR, "ProcessorId", pxProcessor->ProcessorId},
  {VT_UI2,   "ProcessorType", &pxProcessor->ProcessorType},
  {VT_UI2,   "Revision", &pxProcessor->Revision},
  {VT_BSTR, "Role", pxProcessor->Role},
  {VT_BOOL,  "SecondLevelAddressTranslationExtensions", &pxProcessor->SecondLevelAddressTranslationExtensions},
  {VT_BSTR, "SerialNumber", pxProcessor->SerialNumber},
  {VT_BSTR, "SocketDesignation", pxProcessor->SocketDesignation},
  {VT_BSTR, "Status", pxProcessor->Status},
  {VT_UI2,   "StatusInfo", &pxProcessor->StatusInfo},
  {VT_BSTR, "Stepping", pxProcessor->Stepping},
  {VT_BSTR, "SystemCreationClassName", pxProcessor->SystemCreationClassName},
  {VT_BSTR, "SystemName", pxProcessor->SystemName},
  {VT_UI4,   "ThreadCount", &pxProcessor->ThreadCount},
  {VT_BSTR, "UniqueId", pxProcessor->UniqueId},
  {VT_UI2,   "UpgradeMethod", &pxProcessor->UpgradeMethod},
  {VT_BSTR, "Version", pxProcessor->Version},
  {VT_BOOL,  "VirtualizationFirmwareEnabled", &pxProcessor->VirtualizationFirmwareEnabled},
  {VT_BOOL,  "VMMonitorModeExtensions", &pxProcessor->VMMonitorModeExtensions},
  {VT_UI4,   "VoltageCaps", &pxProcessor->VoltageCaps}


                /*
                {"Name", VT_BSTR, pxProcessor->Name },
                {"Manufacturer", VT_BSTR, pxProcessor->Manufacturer },
                {"NumberOfCores", VT_I4, &pxProcessor->NumberOfCores },
                {"NumberOfLogicalProcessors", VT_I4, &pxProcessor->NumberOfLogicalProcessors },
                {"MaxClockSpeed", VT_I4, &pxProcessor->MaxClockSpeed },
                */
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }


    // Mainboard
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_MotherboardDevice");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize processorID = 0; pxHardwareInfo->enumerator; ++processorID)
        {
            PXMainBoard* const pxMainBoard = &pxHardwareInfo->MainBoard;

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);

            if(!u_return)
            {
                break;
            }

            PXWindowsWMIEntry valFetchList[] =
            {               
                {VT_UI2,  "Availability", &pxMainBoard->Availability},
                {VT_BSTR, "Caption", pxMainBoard->Caption},
                {VT_UI4,  "ConfigManagerErrorCode", &pxMainBoard->ConfigManagerErrorCode},
                {VT_BOOL, "ConfigManagerUserConfig", &pxMainBoard->ConfigManagerUserConfig},
                {VT_BSTR, "CreationClassName", pxMainBoard->CreationClassName},
                {VT_BSTR, "Description", pxMainBoard->Description},
                {VT_BSTR, "DeviceID", pxMainBoard->DeviceID},
                {VT_BOOL, "ErrorCleared", &pxMainBoard->ErrorCleared},
                {VT_BSTR, "ErrorDescription", pxMainBoard->ErrorDescription},
                {VT_DATE, "InstallDate", &pxMainBoard->InstallDate},
                {VT_UI4,  "LastErrorCode", &pxMainBoard->LastErrorCode},
                {VT_BSTR, "Name", pxMainBoard->Name},
                {VT_BSTR, "PNPDeviceID", pxMainBoard->PNPDeviceID},
                {VT_UI2,  "PowerManagementCapabilities[]", &pxMainBoard->PowerManagementCapabilities},
                {VT_BOOL, "PowerManagementSupported", &pxMainBoard->PowerManagementSupported},
                {VT_BSTR, "PrimaryBusType", pxMainBoard->PrimaryBusType},
                {VT_BSTR, "RevisionNumber", pxMainBoard->RevisionNumber},
                {VT_BSTR, "SecondaryBusType", pxMainBoard->SecondaryBusType},
                {VT_BSTR, "Status", pxMainBoard->Status},
                {VT_UI2,  "StatusInfo", &pxMainBoard->StatusInfo},
                {VT_BSTR, "SystemCreationClassName", pxMainBoard->SystemCreationClassName},
                {VT_BSTR, "SystemName", pxMainBoard->SystemName}
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }


    // Video
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_VideoController");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize videoDeviceID = 0; pxHardwareInfo->enumerator; ++videoDeviceID)
        {
            PXVideoDevice* const pxVideoDevice = &pxHardwareInfo->VideoDeviceList[videoDeviceID];

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);

            if(!u_return)
            {
                break;
            }

            ++(pxHardwareInfo->VideoDeviceSize);

            PXWindowsWMIEntry valFetchList[] =
            {
                {VT_UI2,  "AcceleratorCapabilities[]", &pxVideoDevice->AcceleratorCapabilities},
                {VT_BSTR, "AdapterCompatibility", pxVideoDevice->AdapterCompatibility},
                {VT_BSTR, "AdapterDACType", pxVideoDevice->AdapterDACType},
                {VT_UI4,  "AdapterRAM", &pxVideoDevice->AdapterRAM},
                {VT_UI2,  "Availability", &pxVideoDevice->Availability},
                {VT_BSTR, "CapabilityDescriptions[]", pxVideoDevice->CapabilityDescriptions},
                {VT_BSTR, "Caption", pxVideoDevice->Caption},
                {VT_UI4,  "ColorTableEntries", &pxVideoDevice->ColorTableEntries},
                {VT_UI4,  "ConfigManagerErrorCode", &pxVideoDevice->ConfigManagerErrorCode},
                {VT_BOOL, "ConfigManagerUserConfig", &pxVideoDevice->ConfigManagerUserConfig},
                {VT_BSTR, "CreationClassName", pxVideoDevice->CreationClassName},
                {VT_UI4,  "CurrentBitsPerPixel",& pxVideoDevice->CurrentBitsPerPixel},
                {VT_UI4,  "CurrentHorizontalResolution", &pxVideoDevice->CurrentHorizontalResolution},
                {VT_UI8,  "CurrentNumberOfColors", &pxVideoDevice->CurrentNumberOfColors},
                {VT_UI4,  "CurrentNumberOfColumns", &pxVideoDevice->CurrentNumberOfColumns},
                {VT_UI4,  "CurrentNumberOfRows", &pxVideoDevice->CurrentNumberOfRows},
                {VT_UI4,  "CurrentRefreshRate", &pxVideoDevice->CurrentRefreshRate},
                {VT_UI2,  "CurrentScanMode", &pxVideoDevice->CurrentScanMode},
                {VT_UI4,  "CurrentVerticalResolution", &pxVideoDevice->CurrentVerticalResolution},
                {VT_BSTR, "Description", pxVideoDevice->Description},
                {VT_BSTR, "DeviceID", pxVideoDevice->DeviceID},
                {VT_UI4,  "DeviceSpecificPens", &pxVideoDevice->DeviceSpecificPens},
                {VT_UI4,  "DitherType", &pxVideoDevice->DitherType},
                {VT_DATE, "DriverDate", &pxVideoDevice->DriverDate},
                {VT_BSTR, "DriverVersion", pxVideoDevice->DriverVersion},
                {VT_BOOL, "ErrorCleared", &pxVideoDevice->ErrorCleared},
                {VT_BSTR, "ErrorDescription", pxVideoDevice->ErrorDescription},
                {VT_UI4,  "ICMIntent", &pxVideoDevice->ICMIntent},
                {VT_UI4,  "ICMMethod", &pxVideoDevice->ICMMethod},
                {VT_BSTR, "InfFilename", pxVideoDevice->InfFilename},
                {VT_BSTR, "InfSection", pxVideoDevice->InfSection},
                {VT_DATE, "InstallDate", pxVideoDevice->SystemCreationClassName},
                {VT_BSTR, "InstalledDisplayDrivers", pxVideoDevice->InstalledDisplayDrivers},
                {VT_UI4,  "LastErrorCode", &pxVideoDevice->LastErrorCode},
                {VT_UI4,  "MaxMemorySupported", &pxVideoDevice->MaxMemorySupported},
                {VT_UI4,  "MaxNumberControlled", &pxVideoDevice->MaxNumberControlled},
                {VT_UI4,  "MaxRefreshRate", &pxVideoDevice->MaxRefreshRate},
                {VT_UI4,  "MinRefreshRate", &pxVideoDevice->MinRefreshRate},
                {VT_BOOL, "Monochrome", &pxVideoDevice->Monochrome},
                {VT_BSTR, "Name", pxVideoDevice->Name},
                {VT_UI2,  "NumberOfColorPlanes", &pxVideoDevice->NumberOfColorPlanes},
                {VT_UI4,  "NumberOfVideoPages", &pxVideoDevice->NumberOfVideoPages},
                {VT_BSTR, "PNPDeviceID", pxVideoDevice->PNPDeviceID},
                {VT_UI2,  "PowerManagementCapabilities[]", &pxVideoDevice->PowerManagementCapabilities},
                {VT_BOOL, "PowerManagementSupported", pxVideoDevice->PowerManagementSupported},
                {VT_UI2,  "ProtocolSupported", &pxVideoDevice->ProtocolSupported},
                {VT_UI4,  "ReservedSystemPaletteEntries", &pxVideoDevice->ReservedSystemPaletteEntries},
                {VT_UI4,  "SpecificationVersion", &pxVideoDevice->SpecificationVersion},
                {VT_BSTR, "Status", pxVideoDevice->Status},
                {VT_UI2,  "StatusInfo", &pxVideoDevice->StatusInfo},
                {VT_BSTR, "SystemCreationClassName", pxVideoDevice->SystemCreationClassName},
                {VT_BSTR, "SystemName", pxVideoDevice->SystemName},
                {VT_UI4,  "SystemPaletteEntries", &pxVideoDevice->SystemPaletteEntries},
                {VT_DATE, "TimeOfLastReset", pxVideoDevice->SystemCreationClassName},
                {VT_UI2,  "VideoArchitecture", &pxVideoDevice->VideoArchitecture},
                {VT_UI2,  "VideoMemoryType", &pxVideoDevice->VideoMemoryType},
                {VT_UI2,  "VideoMode", &pxVideoDevice->VideoMode},
                {VT_BSTR, "VideoModeDescription", pxVideoDevice->VideoModeDescription},
                {VT_BSTR, "VideoProcessor", pxVideoDevice->VideoProcessor}
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }


    // Cache-Memory
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_CacheMemory");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize pxCacheMemoryID = 0; pxHardwareInfo->enumerator; ++pxCacheMemoryID)
        {
            PXCacheMemory* const pxCacheMemory = &pxHardwareInfo->CacheMemoryList[pxCacheMemoryID];

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);

            if(!u_return)
            {
                break;
            }

            ++(pxHardwareInfo->CacheMemorySize);

            PXWindowsWMIEntry valFetchList[] =
            {      
                {VT_UI2,  "Access", &pxCacheMemory->Access},
                {VT_UI1,  "AdditionalErrorData[]", &pxCacheMemory->AdditionalErrorData},
                {VT_UI2,  "Associativity", &pxCacheMemory->Associativity},
                {VT_UI2,  "Availability", &pxCacheMemory->Availability},
                {VT_UI8,  "BlockSize", &pxCacheMemory->BlockSize},
                {VT_UI4,  "CacheSpeed", &pxCacheMemory->CacheSpeed},
                {VT_UI2,  "CacheType", &pxCacheMemory->CacheType},
                {VT_BSTR, "Caption", &pxCacheMemory->Caption},
                {VT_UI4,  "ConfigManagerErrorCode", &pxCacheMemory->ConfigManagerErrorCode},
                {VT_BOOL, "ConfigManagerUserConfig", &pxCacheMemory->ConfigManagerUserConfig},
                {VT_BOOL, "CorrectableError", &pxCacheMemory->CorrectableError},
                {VT_BSTR, "CreationClassName", &pxCacheMemory->CreationClassName},
                {VT_UI2,  "CurrentSRAM[]", &pxCacheMemory->CurrentSRAM},
                {VT_BSTR, "Description", &pxCacheMemory->Description},
                {VT_BSTR, "DeviceID", &pxCacheMemory->DeviceID},
                {VT_UI8,  "EndingAddress", &pxCacheMemory->EndingAddress},
                {VT_UI2,  "ErrorAccess", &pxCacheMemory->ErrorAccess},
                {VT_UI8,  "ErrorAddress", &pxCacheMemory->ErrorAddress},
                {VT_BOOL, "ErrorCleared", &pxCacheMemory->ErrorCleared},
                {VT_UI2,  "ErrorCorrectType", &pxCacheMemory->ErrorCorrectType},
                {VT_UI1,  "ErrorData[]", &pxCacheMemory->ErrorData},
                {VT_UI2,  "ErrorDataOrder", &pxCacheMemory->ErrorDataOrder},
                {VT_BSTR, "ErrorDescription", &pxCacheMemory->ErrorDescription},
                {VT_UI2,  "ErrorInfo", &pxCacheMemory->ErrorInfo},
                {VT_BSTR, "ErrorMethodology", &pxCacheMemory->ErrorMethodology},
                {VT_UI8,  "ErrorResolution", &pxCacheMemory->ErrorResolution},
                {VT_DATE, "ErrorTime", &pxCacheMemory->ErrorTime},
                {VT_UI4,  "ErrorTransferSize", &pxCacheMemory->ErrorTransferSize},
                {VT_UI4,  "FlushTimer", &pxCacheMemory->FlushTimer},
                {VT_DATE, "InstallDate", &pxCacheMemory->InstallDate},
                {VT_UI4,  "InstalledSize", &pxCacheMemory->InstalledSize},
                {VT_UI4,  "LastErrorCode", &pxCacheMemory->LastErrorCode},
                {VT_UI2,  "Level", &pxCacheMemory->Level},
                {VT_UI4,  "LineSize", &pxCacheMemory->LineSize},
                {VT_UI2,  "Location", &pxCacheMemory->Location},
                {VT_UI4,  "MaxCacheSize", &pxCacheMemory->MaxCacheSize},
                {VT_BSTR, "Name", &pxCacheMemory->Name},
                {VT_UI8,  "NumberOfBlocks", &pxCacheMemory->NumberOfBlocks},
                {VT_BSTR, "OtherErrorDescription", &pxCacheMemory->OtherErrorDescription},
                {VT_BSTR, "PNPDeviceID", &pxCacheMemory->PNPDeviceID},
                {VT_UI2,  "PowerManagementCapabilities[]", &pxCacheMemory->PowerManagementCapabilities},
                {VT_BOOL, "PowerManagementSupported", &pxCacheMemory->PowerManagementSupported},
                {VT_BSTR, "Purpose", &pxCacheMemory->Purpose},
                {VT_UI2,  "ReadPolicy", &pxCacheMemory->ReadPolicy},
                {VT_UI2,  "ReplacementPolicy", &pxCacheMemory->ReplacementPolicy},
                {VT_UI8,  "StartingAddress", &pxCacheMemory->StartingAddress},
                {VT_BSTR, "Status", &pxCacheMemory->Status},
                {VT_UI2,  "StatusInfo", &pxCacheMemory->StatusInfo},
                {VT_UI2,  "SupportedSRAM[]", &pxCacheMemory->SupportedSRAM},
                {VT_BSTR, "SystemCreationClassName", &pxCacheMemory->SystemCreationClassName},
                {VT_BOOL, "SystemLevelAddress", &pxCacheMemory->SystemLevelAddress},
                {VT_BSTR, "SystemName", &pxCacheMemory->SystemName},
                {VT_UI2,  "WritePolicy", &pxCacheMemory->WritePolicy}
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }


    // RAM Memory
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_MemoryDevice");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize pxRAMMemoryID = 0; pxHardwareInfo->enumerator; ++pxRAMMemoryID)
        {
            PXCCCCMemory* const pxRAMMemory = &pxHardwareInfo->CCCCMemoryList[pxRAMMemoryID];

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);

            if(!u_return)
            {
                break;
            }

            ++(pxHardwareInfo->CCCCMemorySize);

            PXWindowsWMIEntry valFetchList[] =
            {        
                {VT_UI2,  "Access", &pxRAMMemory->Access},
                {VT_UI1,  "AdditionalErrorData[]", &pxRAMMemory->AdditionalErrorData},
                {VT_UI2,  "Availability", &pxRAMMemory->Availability},
                {VT_UI8,  "BlockSize", &pxRAMMemory->BlockSize},
                {VT_BSTR, "Caption", pxRAMMemory->Caption},
                {VT_UI4,  "ConfigManagerErrorCode", &pxRAMMemory->ConfigManagerErrorCode},
                {VT_BOOL, "ConfigManagerUserConfig", &pxRAMMemory->ConfigManagerUserConfig},
                {VT_BOOL, "CorrectableError", &pxRAMMemory->CorrectableError},
                {VT_BSTR, "CreationClassName", pxRAMMemory->CreationClassName},
                {VT_BSTR, "Description", pxRAMMemory->Description},
                {VT_BSTR, "DeviceID", pxRAMMemory->DeviceID},
                {VT_UI8,  "EndingAddress", &pxRAMMemory->EndingAddress},
                {VT_UI2,  "ErrorAccess", &pxRAMMemory->ErrorAccess},
                {VT_UI8,  "ErrorAddress", &pxRAMMemory->ErrorAddress},
                {VT_BOOL, "ErrorCleared", &pxRAMMemory->ErrorCleared},
                {VT_UI1,  "ErrorData[]", &pxRAMMemory->ErrorData},
                {VT_UI2,  "ErrorDataOrder", &pxRAMMemory->ErrorDataOrder},
                {VT_BSTR, "ErrorDescription", pxRAMMemory->ErrorDescription},
                {VT_UI2,  "ErrorGranularity", &pxRAMMemory->ErrorGranularity},
                {VT_UI2,  "ErrorInfo", &pxRAMMemory->ErrorInfo},
                {VT_BSTR, "ErrorMethodology", pxRAMMemory->ErrorMethodology},
                {VT_UI8,  "ErrorResolution", &pxRAMMemory->ErrorResolution},
                {VT_DATE, "ErrorTime", &pxRAMMemory->ErrorTime},
                {VT_UI4,  "ErrorTransferSize", &pxRAMMemory->ErrorTransferSize},
                {VT_DATE, "InstallDate", &pxRAMMemory->InstallDate},
                {VT_UI4,  "LastErrorCode", &pxRAMMemory->LastErrorCode},
                {VT_BSTR, "Name", pxRAMMemory->Name},
                {VT_UI8,  "NumberOfBlocks", &pxRAMMemory->NumberOfBlocks},
                {VT_BSTR, "OtherErrorDescription", pxRAMMemory->OtherErrorDescription},
                {VT_BSTR, "PNPDeviceID", pxRAMMemory->PNPDeviceID},
                {VT_UI2,  "PowerManagementCapabilities[]", &pxRAMMemory->PowerManagementCapabilities},
                {VT_BOOL, "PowerManagementSupported", &pxRAMMemory->PowerManagementSupported},
                {VT_BSTR, "Purpose", pxRAMMemory->Purpose},
                {VT_UI8,  "StartingAddress", &pxRAMMemory->StartingAddress},
                {VT_BSTR, "Status", pxRAMMemory->Status},
                {VT_UI2,  "StatusInfo", &pxRAMMemory->StatusInfo},
                {VT_BSTR, "SystemCreationClassName", pxRAMMemory->SystemCreationClassName},
                {VT_BOOL, "SystemLevelAddress", &pxRAMMemory->SystemLevelAddress},
                {VT_BSTR, "SystemName", pxRAMMemory->SystemName}
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }



    // RAM Memory
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_PhysicalMemory");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize pxRAMMemoryID = 0; pxHardwareInfo->enumerator; ++pxRAMMemoryID)
        {
            PXPhysicalMemory* const pxPhysicalMemory = &pxHardwareInfo->PhysicalMemoryList[pxRAMMemoryID];

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);

            if(!u_return)
            {
                break;
            }

            ++(pxHardwareInfo->PhysicalMemoryListSize);

            PXWindowsWMIEntry valFetchList[] =
            {
                {VT_UI4,  "Attributes", &pxPhysicalMemory->Attributes},
                {VT_BSTR, "BankLabel", pxPhysicalMemory->BankLabel},
                {VT_UI8,  "Capacity", &pxPhysicalMemory->Capacity},
                {VT_BSTR, "Caption", pxPhysicalMemory->Caption},
                {VT_UI4,  "ConfiguredClockSpeed", &pxPhysicalMemory->ConfiguredClockSpeed},
                {VT_UI4,  "ConfiguredVoltage", &pxPhysicalMemory->ConfiguredVoltage},
                {VT_BSTR, "CreationClassName", pxPhysicalMemory->CreationClassName},
                {VT_UI2,  "DataWidth", &pxPhysicalMemory->DataWidth},
                {VT_BSTR, "Description", pxPhysicalMemory->Description},
                {VT_BSTR, "DeviceLocator", pxPhysicalMemory->DeviceLocator},
                {VT_UI2,  "FormFactor", &pxPhysicalMemory->FormFactor},
                {VT_BOOL, "HotSwappable", &pxPhysicalMemory->HotSwappable},
                {VT_DATE, "InstallDate", &pxPhysicalMemory->InstallDate},
                {VT_UI2,  "InterleaveDataDepth", &pxPhysicalMemory->InterleaveDataDepth},
                {VT_UI4,  "InterleavePosition", &pxPhysicalMemory->InterleavePosition},
                {VT_BSTR, "Manufacturer", pxPhysicalMemory->Manufacturer},
                {VT_UI4,  "MaxVoltage", &pxPhysicalMemory->MaxVoltage},
                {VT_UI2,  "MemoryType", &pxPhysicalMemory->MemoryType},
                {VT_UI4,  "MinVoltage", &pxPhysicalMemory->MinVoltage},
                {VT_BSTR, "Model", pxPhysicalMemory->Model},
                {VT_BSTR, "Name", pxPhysicalMemory->Name},
                {VT_BSTR, "OtherIdentifyingInfo", pxPhysicalMemory->OtherIdentifyingInfo},
                {VT_BSTR, "PartNumber", pxPhysicalMemory->PartNumber},
                {VT_UI4,  "PositionInRow", &pxPhysicalMemory->PositionInRow},
                {VT_BOOL, "PoweredOn", &pxPhysicalMemory->PoweredOn},
                {VT_BOOL, "Removable", &pxPhysicalMemory->Removable},
                {VT_BOOL, "Replaceable", &pxPhysicalMemory->Replaceable},
                {VT_BSTR, "SerialNumber", pxPhysicalMemory->SerialNumber},
                {VT_BSTR, "SKU", pxPhysicalMemory->SKU},
                {VT_UI4,  "SMBIOSMemoryType", &pxPhysicalMemory->SMBIOSMemoryType},
                {VT_UI4,  "Speed", &pxPhysicalMemory->Speed},
                {VT_BSTR, "Status", pxPhysicalMemory->Status},
                {VT_BSTR, "Tag", pxPhysicalMemory->Tag},
                {VT_UI2,  "TotalWidth", &pxPhysicalMemory->TotalWidth},
                {VT_UI2,  "TypeDetail", &pxPhysicalMemory->TypeDetail},
                {VT_BSTR, "Version", pxPhysicalMemory->Version},
                
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }




    // sdfsdfsfdsdf
    {
        const PXBool success = PXWindowsWMIClassOpen(pxHardwareInfo, "Win32_BIOS");

        if(!success)
        {
            return PXActionInvalid;
        }

        ULONG u_return = 0;

        for(PXSize pxRAMMemoryID = 0; pxHardwareInfo->enumerator; ++pxRAMMemoryID)
        {
            PXBIOS* const pxBIOS = &pxHardwareInfo->BIOS;

            HRESULT EJIE = pxHardwareInfo->enumerator->lpVtbl->Next(pxHardwareInfo->enumerator, WBEM_INFINITE, 1, &pxHardwareInfo->obj, &u_return);

            if(!u_return)
            {
                break;
            }

            ++(pxHardwareInfo->PhysicalMemoryListSize);

            PXWindowsWMIEntry valFetchList[] =
            {                
                {VT_UI2,  "BiosCharacteristics[]", &pxBIOS->BiosCharacteristics},
                {VT_BSTR, "BIOSVersion[]", pxBIOS->BIOSVersion},
                {VT_BSTR, "BuildNumber", pxBIOS->BuildNumber},
                {VT_BSTR, "Caption", pxBIOS->Caption},
                {VT_BSTR, "CodeSet", pxBIOS->CodeSet},
                {VT_BSTR, "CurrentLanguage", pxBIOS->CurrentLanguage},
                {VT_BSTR, "Description", pxBIOS->Description},
                {VT_UI1,  "EmbeddedControllerMajorVersion", &pxBIOS->EmbeddedControllerMajorVersion},
                {VT_UI1,  "EmbeddedControllerMinorVersion", &pxBIOS->EmbeddedControllerMinorVersion},
                {VT_BSTR, "IdentificationCode", pxBIOS->IdentificationCode},
                {VT_UI2,  "InstallableLanguages", &pxBIOS->InstallableLanguages},
                {VT_DATE, "InstallDate", &pxBIOS->InstallDate},
                {VT_BSTR, "LanguageEdition", pxBIOS->LanguageEdition},
                {VT_BSTR, "ListOfLanguages[]", pxBIOS->ListOfLanguages},
                {VT_BSTR, "Manufacturer", pxBIOS->Manufacturer},
                {VT_BSTR, "Name", pxBIOS->Name},
                {VT_BSTR, "OtherTargetOS", pxBIOS->OtherTargetOS},
                {VT_BOOL, "PrimaryBIOS", &pxBIOS->PrimaryBIOS},
                {VT_DATE, "ReleaseDate", &pxBIOS->ReleaseDate},
                {VT_BSTR, "SerialNumber", pxBIOS->SerialNumber},
                {VT_BSTR, "SMBIOSBIOSVersion", pxBIOS->SMBIOSBIOSVersion},
                {VT_UI2,  "SMBIOSMajorVersion", &pxBIOS->SMBIOSMajorVersion},
                {VT_UI2,  "SMBIOSMinorVersion", &pxBIOS->SMBIOSMinorVersion},
                {VT_BOOL, "SMBIOSPresent", &pxBIOS->SMBIOSPresent},
                {VT_BSTR, "SoftwareElementID", pxBIOS->SoftwareElementID},
                {VT_UI2,  "SoftwareElementState", &pxBIOS->SoftwareElementState},
                {VT_BSTR, "Status", pxBIOS->Status},
                {VT_UI1,  "SystemBiosMajorVersion", &pxBIOS->SystemBiosMajorVersion},
                {VT_UI1,  "SystemBiosMinorVersion", &pxBIOS->SystemBiosMinorVersion},
                {VT_UI2,  "TargetOperatingSystem", &pxBIOS->TargetOperatingSystem},
                {VT_BSTR, "Version", pxBIOS->Version},
            };
            const PXSize amount = sizeof(valFetchList) / sizeof(PXWindowsWMIEntry);

            PXWindowsWMIExtractValue(pxHardwareInfo, valFetchList, amount);

            pxHardwareInfo->obj->lpVtbl->Release(pxHardwareInfo->obj);
            pxHardwareInfo->obj = PXNull;
        }
    }

    return PXActionSuccessful;
}