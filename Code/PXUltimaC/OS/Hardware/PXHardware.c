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

PXActionResult PXAPI PXHardwareInfoScan(PXHardwareInfo* const pxHardwareInfo)
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

    return PXActionSuccessful;
}