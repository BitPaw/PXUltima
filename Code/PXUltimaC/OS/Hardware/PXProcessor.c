#include "PXProcessor.h"

#if OSUnix
#include <time.h>
#include <sys/time.h>
#elif OSWindows
#include <Windows.h>
//#include <comdef.h>
#include <Wbemidl.h>
//#include <intrin.h> // MISSING

#include <intrin.h>

#pragma comment(lib, "Wbemuuid.lib") // CLSID_WbemLocator

#endif

#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Console/PXConsole.h>


// 32-Bit Registers EAX, EBX, ECX, EDX
typedef struct PXCPUInfo_
{
    PXInt32U EAX;
    PXInt32U EBX;
    PXInt32U ECX;
    PXInt32U EDX;
}
PXCPUInfo;


#if OSUnix
void cpuid(PXCPUInfo* pxCPUInfo, int code)
{
    asm volatile("cpuid" : "=a" (pxCPUInfo->EAX), "=b" (pxCPUInfo->EBX), "=c" (pxCPUInfo->ECX), "=d" (pxCPUInfo->EDX) : "a" (code));
}
#define PXIntrinsicCPUID cpuid
#elif OSWindows
#define PXIntrinsicCPUID __cpuid
#endif



void PXAPI PXProcessorModelNameGet(const PXProcessorModelName processorModelName, char* const name)
{
    const char* processorName = 0;

    switch(processorModelName)
    {
        case Intel486DX25x33:
            processorName = "Intel ";
            break;

        case Intel486DX50:
            processorName = "Intel ";
            break;

        case Intel486SX:
            processorName = "Intel ";
            break;

        case Intel486DX2:
            processorName = "Intel ";
            break;

        case Intel486SL:
            processorName = "Intel ";
            break;

        case Intel486SX2:
            processorName = "Intel ";
            break;

        case Intel486DX2WB:
            processorName = "Unkown";
            break;

        case Intel486DX4:
            processorName = "Unkown";
            break;

        case Intel486DX4WB:
            processorName = "Unkown";
            break;

        case IntelPentium60x66AStep:
            processorName = "Unkown";
            break;

        case IntelPentium60x66:
            processorName = "Intel ";
            break;

        case IntelPentium75x200:
            processorName = "Intel ";
            break;

        case IntelOverDrivePODP5V83:
            processorName = "Intel ";
            break;

        case IntelPentiumMMX:
            processorName = "Intel ";
            break;

        case IntelMobilePentium75x200:
            processorName = "Intel ";
            break;

        case IntelMobilePentiumMMX:
            processorName = "Intel Pentium MMX";
            break;

        case IntelPentiumProAStep:
            processorName = "Intel ";
            break;

        case IntelPentiumPro:
            processorName = "Intel ";
            break;

        case IntelPentiumIIKlaPXMath:
            processorName = "Intel Pentium II (KlaPXMath)";
            break;

        case IntelPentiumIIDeschutes:
            processorName = "Intel Pentium II (Deschutes)";
            break;

        case IntelCeleronMendocino:
            processorName = "Intel Celeron Mendocino";
            break;

        case IntelPentiumIIIKatmai:
            processorName = "Intel Pentium III (Katmai)";
            break;

        case IntelPentiumIIICoppermine:
            processorName = "Intel Pentium III (Coppermine)";
            break;

        case IntelMobilePentiumIII:
            processorName = "Intel Pentium III Mobile";
            break;

        case IntelPentiumIII0x18um:
            processorName = "Intel ";
            break;

        case IntelPentiumIII0x13um:
            processorName = "Intel ";
            break;

        case IntelItanium:
            processorName = "Intel ";
            break;

        case IntelPentiumIV0x18um:
            processorName = "Intel ";
            break;

        case IntelPentiumIV0x13um:
            processorName = "Intel ";
            break;

        case IntelPentiumIV0x09um:
            processorName = "Intel Pentium IV 0.90nm";
            break;

        case IntelItanium2:
            processorName = "Intel Itanium 2";
            break;

        case AMD486DX2:
            processorName = "AMD ";
            break;

        case AMD486DX2WB:
            processorName = "AMD ";
            break;

        case AMD486DX4:
            processorName = "AMD ";
            break;

        case AMD486DX4WB:
            processorName = "AMD ";
            break;

        case AMDAm5x86WT:
            processorName = "AMD ";
            break;

        case AMDAm5x86WB:
            processorName = "AMD ";
            break;

        case AMDK5SSA5:
            processorName = "AMD ";
            break;

        case AMDK5:
            processorName = "AMD ";
            break;

        case AMDK6:
            processorName = "AMD ";
            break;

        case AMDK62:
            processorName = "AMD ";
            break;

        case AMDK63:
            processorName = "AMD ";
            break;

        case AMDK63P:
            processorName = "AMD ";
            break;

        case AMDAthlon0x25um:
            processorName = "AMD ";
            break;

        case AMDAthlon0x18um:
            processorName = "AMD ";
            break;

        case AMDDuron:
            processorName = "AMD Duron";
            break;

        case AMDAthlonThunderbird:
            processorName = "AMD Athlon Thunderbird";
            break;

        case AMDAthlonPalamino:
            processorName = "AMD Athlon Palamino";
            break;

        case AMDDuronMorgan:
            processorName = "AMD Duron Morgan";
            break;

        case AMDAthlonThoroughbred:
            processorName = "AMD Athlon Thoroughbred";
            break;

        case AMDAthlonBarton:
            processorName = "AMD Athlon Barton";
            break;

        case AMDAthlon64:
            processorName = "AMD Athlon 64";
            break;

        case AMDAthlon64FXOperon:
            processorName = "AMD Athlon 64 FX Operon";
            break;

        default:
            processorName = "Unkown";
            break;
    }

    PXTextCopyA(processorName, -1, name, -1);
}

PXProcessorBrand PXAPI PXProcessorBrandDetect(const char* name)
{
    const PXSize bufferSize = 13;

    if(PXTextCompareA("AMDisbetter!", bufferSize, name, bufferSize)) return ProcessorBrandAMD;
    if(PXTextCompareA("AuthenticAMD", bufferSize, name, bufferSize)) return ProcessorBrandAMD;
    if(PXTextCompareA("CentaurHauls", bufferSize, name, bufferSize)) return ProcessorBrandWinChip;
    if(PXTextCompareA("CyrixInstead", bufferSize, name, bufferSize)) return ProcessorBrandCyrix;
    if(PXTextCompareA("GenuineIntel", bufferSize, name, bufferSize)) return ProcessorBrandIntel;
    if(PXTextCompareA("TransmetaCPU", bufferSize, name, bufferSize)) return ProcessorBrandTransmeta;
    if(PXTextCompareA("GenuineTMx86", bufferSize, name, bufferSize)) return ProcessorBrandTransmeta;
    if(PXTextCompareA("Geode by NSC", bufferSize, name, bufferSize)) return ProcessorBrandNationalSemiconductor;
    if(PXTextCompareA("NexGenDriven", bufferSize, name, bufferSize)) return ProcessorBrandNexGen;
    if(PXTextCompareA("RiseRiseRise", bufferSize, name, bufferSize)) return ProcessorBrandRise;
    if(PXTextCompareA("SiS SiS SiS ", bufferSize, name, bufferSize)) return ProcessorBrandSiS;
    if(PXTextCompareA("UMC UMC UMC ", bufferSize, name, bufferSize)) return ProcessorBrandUMC;
    if(PXTextCompareA("VIA VIA VIA ", bufferSize, name, bufferSize)) return ProcessorBrandVIA;
    if(PXTextCompareA("Vortex86 SoC", bufferSize, name, bufferSize)) return ProcessorBrandDMAndPVortex;
    if(PXTextCompareA("  Shanghai  ", bufferSize, name, bufferSize)) return ProcessorBrandZhaoxin;
    if(PXTextCompareA("HygonGenuine", bufferSize, name, bufferSize)) return ProcessorBrandHygon;
    if(PXTextCompareA("E2K MACHINE", bufferSize, name, bufferSize)) return ProcessorBrandMCSTElbrus;
    if(PXTextCompareA("MiSTer AO486", bufferSize, name, bufferSize)) return ProcessorBrandao486;
    if(PXTextCompareA("bhyve bhyve ", bufferSize, name, bufferSize)) return ProcessorBrandbhyve;
    if(PXTextCompareA(" KVMKVMKVM  ", bufferSize, name, bufferSize)) return ProcessorBrandKVM;
    if(PXTextCompareA("TCGTCGTCGTCG", bufferSize, name, bufferSize)) return ProcessorBrandQEMU;
    if(PXTextCompareA("Microsoft Hv", bufferSize, name, bufferSize)) return ProcessorBrandMicrosoftHyperVOrVirtualPC;
    if(PXTextCompareA("MicrosoftXTA", bufferSize, name, bufferSize)) return ProcessorBrandMicrosoftx86ToARM;
    if(PXTextCompareA(" lrpepyh  vr", bufferSize, name, bufferSize)) return ProcessorBrandParallels;
    if(PXTextCompareA("VMwareVMware", bufferSize, name, bufferSize)) return ProcessorBrandVMware;
    if(PXTextCompareA("XenVMMXenVMM", bufferSize, name, bufferSize)) return ProcessorBrandXenHVM;
    if(PXTextCompareA("ACRNACRNACRN", bufferSize, name, bufferSize)) return ProcessorBrandProjectACRN;
    if(PXTextCompareA(" QNXQVMBSQG ", bufferSize, name, bufferSize)) return ProcessorBrandQNXHypervisor;
    if(PXTextCompareA("VirtualApple", bufferSize, name, bufferSize)) return ProcessorBrandAppleRosetta;

    return ProcessorBrandInvalid;
}

PXProcessorModelName PXAPI PXProcessorModelNameDetect(const PXProcessorBrand processorBrand, const unsigned char famliy, const unsigned char model)
{
    const PXInt16U id = PXInt16Make(famliy, model);

    switch(processorBrand)
    {
        case ProcessorBrandIntel:
        {
            switch(id)
            {
                case PXInt16Make(4, 0):
                    return Intel486DX25x33;
                case PXInt16Make(4, 1):
                    return Intel486DX50;
                case PXInt16Make(4, 2):
                    return Intel486SX;
                case PXInt16Make(4, 3):
                    return Intel486DX2;
                case PXInt16Make(4, 4):
                    return Intel486SL;
                case PXInt16Make(4, 5):
                    return Intel486SX2;
                case PXInt16Make(4, 7):
                    return Intel486DX2WB;
                case PXInt16Make(4, 8):
                    return Intel486DX4;
                case PXInt16Make(4, 9):
                    return Intel486DX4WB;

                case PXInt16Make(5, 0):
                    return IntelPentium60x66AStep;
                case PXInt16Make(5, 1):
                    return IntelPentium60x66;
                case PXInt16Make(5, 2):
                    return IntelPentium75x200;
                case PXInt16Make(5, 3):
                    return IntelOverDrivePODP5V83;
                case PXInt16Make(5, 4):
                    return IntelPentiumMMX;
                case PXInt16Make(5, 7):
                    return IntelMobilePentium75x200;
                case PXInt16Make(5, 8):
                    return IntelMobilePentiumMMX;

                case PXInt16Make(6, 1):
                    return IntelPentiumProAStep;
                case PXInt16Make(6, 2):
                    return IntelPentiumPro;
                case PXInt16Make(6, 3):
                    return IntelPentiumIIKlaPXMath;
                case PXInt16Make(6, 5):
                    return IntelPentiumIIDeschutes;
                case PXInt16Make(6, 6):
                    return IntelCeleronMendocino;
                case PXInt16Make(6, 7):
                    return IntelPentiumIIIKatmai;
                case PXInt16Make(6, 8):
                    return IntelPentiumIIICoppermine;
                case PXInt16Make(6, 9):
                    return IntelMobilePentiumIII;
                case PXInt16Make(6, 10):
                    return IntelPentiumIII0x18um;
                case PXInt16Make(6, 11):
                    return IntelPentiumIII0x13um;
            }

            if(famliy == 7)
            {
                return IntelItanium;
            }

            break;
        }

        case ProcessorBrandAMD:
        {
            switch(id)
            {
                case PXInt16Make(4, 3):
                    return AMD486DX2;
                case PXInt16Make(4, 7):
                    return AMD486DX2WB;
                case PXInt16Make(4, 8):
                    return AMD486DX4;
                case PXInt16Make(4, 9):
                    return AMD486DX4WB;
                case PXInt16Make(4, 14):
                    return AMDAm5x86WT;
                case PXInt16Make(4, 15):
                    return AMDAm5x86WB;

                case PXInt16Make(5, 0):
                    return AMDK5SSA5;
                case PXInt16Make(5, 1): // fallthough
                case PXInt16Make(5, 2): // fallthough
                case PXInt16Make(5, 3):
                    return AMDK5;

                case PXInt16Make(5, 6):  // fallthough
                case PXInt16Make(5, 7):
                    return AMDK6;

                case PXInt16Make(5, 8):
                    return AMDK62;
                case PXInt16Make(5, 9):
                    return AMDK63;
                case PXInt16Make(5, 13):
                    return AMDK63P;

                case PXInt16Make(6, 0): // fallthough
                case PXInt16Make(6, 1):
                    return AMDAthlon0x25um;
                case PXInt16Make(6, 2):
                    return AMDAthlon0x18um;
                case PXInt16Make(6, 3):
                    return AMDDuron;
                case PXInt16Make(6, 4):
                    return AMDAthlonThunderbird;
                case PXInt16Make(6, 6):
                    return AMDAthlonPalamino;
                case PXInt16Make(6, 7):
                    return AMDDuronMorgan;
                case PXInt16Make(6, 8):
                    return AMDAthlonThoroughbred;
                case PXInt16Make(6, 10):
                    return AMDAthlonBarton;
            }

            if(famliy == 15)
            {
                // Extended family...
            }

            break;
        }
    }

    return ProcessorModelNameInvalid;
}

void PXAPI PXProcessorFetchInfo(PXProcessor* const processor)
{
    PXInt32U maxInfoValueID = 0; // Maximum Input Value for Basic CPUID Information.
    PXInt32U maxExtendedFunctionID = 0; // Maximum Input Value for Extended Function CPUID Information.

    PXClear(PXProcessor, processor);

    typedef enum PXCPUIDCommand_
    {
        PXCPUIDCommandBasicInformation = 0x00,
        PXCPUIDCommandVersionInformation = 0x01,
        PXCPUIDCommandCacheAndTLBInformation = 0x02,
        PXCPUIDCommandSerialNumber = 0x03,

        PXCPUIDCommandExtendedFunctionInformation = 0x80000000u,
        PXCPUIDCommandExtendedProcessorSignature = 0x80000001u,
        PXCPUIDCommandProcessorBrandStringA = 0x80000002u,
        PXCPUIDCommandProcessorBrandStringB = 0x80000003u,
        PXCPUIDCommandProcessorBrandStringC = 0x80000004u,
        PXCPUIDCommandReserved = 0x80000005u,
        PXCPUIDCommandCacheLineInfo = 0x80000006u,
        PXCPUIDCommandTSC = 0x80000007u,
        PXCPUIDCommandPhysicalAddress = 0x80000008u
    }
    PXCPUIDCommand;

    const PXInt32U cpuIDCommandList[] =
    {
        PXCPUIDCommandBasicInformation,
        PXCPUIDCommandVersionInformation,
        PXCPUIDCommandCacheAndTLBInformation,
        PXCPUIDCommandSerialNumber,
        PXCPUIDCommandExtendedFunctionInformation,
        PXCPUIDCommandExtendedProcessorSignature,
        PXCPUIDCommandProcessorBrandStringA,
        PXCPUIDCommandProcessorBrandStringB,
        PXCPUIDCommandProcessorBrandStringC,
        PXCPUIDCommandCacheLineInfo,
        PXCPUIDCommandTSC,
        PXCPUIDCommandPhysicalAddress
    };
    const PXSize amount = sizeof(cpuIDCommandList) / sizeof(PXInt32U);

    for(PXSize i = 0; i < amount; ++i)
    {
        const PXCPUIDCommand command = cpuIDCommandList[i];

        PXCPUInfo pxCPUInfo;
         
        PXIntrinsicCPUID(&pxCPUInfo, command);

        switch(command)
        {
            case PXCPUIDCommandBasicInformation:
            {
                maxInfoValueID = pxCPUInfo.EAX;

                processor->IdentityString[0] = 0xFF & (pxCPUInfo.EBX >> 0);
                processor->IdentityString[1] = 0xFF & (pxCPUInfo.EBX >> 8);
                processor->IdentityString[2] = 0xFF & (pxCPUInfo.EBX >> 16);
                processor->IdentityString[3] = 0xFF & (pxCPUInfo.EBX >> 24);

                processor->IdentityString[4] = 0xFF & (pxCPUInfo.EDX >> 0);
                processor->IdentityString[5] = 0xFF & (pxCPUInfo.EDX >> 8);
                processor->IdentityString[6] = 0xFF & (pxCPUInfo.EDX >> 16);
                processor->IdentityString[7] = 0xFF & (pxCPUInfo.EDX >> 24);

                processor->IdentityString[8] = 0xFF & (pxCPUInfo.ECX >> 0);
                processor->IdentityString[9] = 0xFF & (pxCPUInfo.ECX >> 8);
                processor->IdentityString[10] = 0xFF & (pxCPUInfo.ECX >> 16);
                processor->IdentityString[11] = 0xFF & (pxCPUInfo.ECX >> 24);
                processor->IdentityString[12] = 0;

                processor->BrandID = PXProcessorBrandDetect(processor->IdentityString);

                break;
            }
            case PXCPUIDCommandVersionInformation:
            {
                //------------------------
                // EAX
                //------------------------
                processor->SteppingID = 0x0F & (pxCPUInfo.EAX >> 0); // 0b----------------------------1111
                processor->Model = 0x0F & (pxCPUInfo.EAX >> 4); // 0b------------------------11110000
                processor->Family = 0x0F & (pxCPUInfo.EAX >> 8); // 0b--------------------111100000000
                processor->ProcessorType = 0x03 & (pxCPUInfo.EAX >> 12);
                processor->ReservedBitsA = 0x03 & (pxCPUInfo.EAX >> 14);
                processor->ExtendedModel = 0x0F & (pxCPUInfo.EAX >> 16);
                processor->ExtendedFamily = 0xFF & (pxCPUInfo.EAX >> 20);
                processor->ReservedBitsB = 0x0F & (pxCPUInfo.EAX >> 28);

                processor->ModelNameID = PXProcessorModelNameDetect(processor->BrandID, processor->Family, processor->Model);
                //------------------------

                //------------------------
                // EBX
                //------------------------
                processor->BrandIndex = 0xFF & (pxCPUInfo.EBX >> 0);
                processor->CLflushCacheLineSize = 0xFF & (pxCPUInfo.EBX >> 8);
                processor->logicalProcessors = 0xFF & (pxCPUInfo.EBX >> 16);
                processor->initialApicID = 0xFF & (pxCPUInfo.EBX >> 24);
                //------------------------

                //------------------------
                // ECX
                //------------------------
                processor->FeatureList = pxCPUInfo.ECX;
                processor->FeatureList <<= 8;
                //------------------------

                //------------------------
                // EDX
                //------------------------
                processor->FeatureList |= pxCPUInfo.EDX;
                //------------------------

                break;
            }
            case PXCPUIDCommandExtendedFunctionInformation:
            {
                maxExtendedFunctionID = pxCPUInfo.EAX;

                break;
            }
            case PXCPUIDCommandProcessorBrandStringA:
            case PXCPUIDCommandProcessorBrandStringB:
            case PXCPUIDCommandProcessorBrandStringC:
            {
                char* text = (char*)&pxCPUInfo;

                processor->BrandNameSize += PXTextCopyA
                (
                    text,
                    sizeof(PXCPUInfo),
                    processor->BrandName + processor->BrandNameSize,
                    64 - processor->BrandNameSize
                );
                break;
            }
            case PXCPUIDCommandCacheLineInfo:
            {
                // EAX - Reserved
                // EBX - Reserved
                // EDX - Reserved

                processor->L2AssociativityField = 0b00000000000000000000000011111111 & (pxCPUInfo.ECX >> 0);
                //processor->Reserved           = 0b00000000000000000000111100000000 & (pxCPUInfo.ECX >> 8);
                processor->CacheSizein1Kunits = 0b00000000000000001111000000000000 & (pxCPUInfo.ECX >> 12);
                processor->CacheLineSizeInBytes = 0b11111111111111110000000000000000 & (pxCPUInfo.ECX >> 16);

                break;
            }
            case PXCPUIDCommandTSC:
            {
                // EAX - Reserved
                // EBX - Reserved
                // ECX - Reserved

                processor->IsInvariantTSCAvailable = 0b00000000000000000000000100000000 & (pxCPUInfo.EDX >> 8);

                break;
            }
            case PXCPUIDCommandPhysicalAddress:
            {
                break;
            }
            default:
                break;
        }
    }


    if(processor->BrandNameSize < 5)
    {
        PXProcessorModelNameGet(processor->ModelNameID, processor->BrandName);
    }

    // Fix processor name
    {
        char processorCore16[] = "16-Core";
        char processorNameEx[] = "Processor";

        PXTextReplaceA(processor->BrandName, processor->BrandNameSize, processorNameEx, sizeof(processorNameEx), ' ');
        PXTextReplaceA(processor->BrandName, processor->BrandNameSize, processorCore16, sizeof(processorCore16), ' ');

        processor->BrandNameSize = PXTextTrimA(processor->BrandName, processor->BrandNameSize);
    }


#if OSWindows
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

#if 0
        printf("Hardware information: \n");
        printf("  OEM ID: %u\n", sysInfo.dwOemId);
        printf("  Number of processors: %u\n", sysInfo.dwNumberOfProcessors);
        printf("  Page size: %u\n", sysInfo.dwPageSize);
        printf("  Processor type: %u\n", sysInfo.dwProcessorType);
        printf("  Minimum application address: %lx\n", sysInfo.lpMinimumApplicationAddress);
        printf("  Maximum application address: %lx\n", sysInfo.lpMaximumApplicationAddress);
        printf("  Active processor mask: %u\n", sysInfo.dwActiveProcessorMask);
#endif

        processor->NumberOfProcessors = sysInfo.dwNumberOfProcessors;

#if WindowsAtleast7 && PXOSWindowsDestop

        const WORD nrOfProcessorGroups = GetActiveProcessorGroupCount(); // Windows 7, kernel32.dll, winbase.h

        for(WORD i = 0; i < nrOfProcessorGroups; ++i)
        {
            const DWORD activeProcessors = GetActiveProcessorCount(i); // Windows 7, kernel32.dll, winbase.h
        }
#endif
    }
#endif

    for(int i = processor->BrandNameSize - 1; i > 0; --i)
    {
        const PXBool whiteSpace = processor->BrandName[i] == ' ';

        if(!whiteSpace)
        {
            break;
        }

        processor->BrandNameSize--;
        processor->BrandName[i] = '\0';
    }
}

unsigned int PXAPI PXProcessorFrequencyCurrent()
{
#if OSWindows
    FILETIME a, b, c, d;
    const HANDLE process = GetCurrentProcess(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXBool sucessful = GetProcessTimes(process, &a, &b, &c, &d); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

    if(!sucessful)
    {
        return -1;
    }

    const PXSize deltaTimeIU = d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32);
    const PXSize deltaTimeD = deltaTimeIU * 0.0000001;

    //  Returns total user time.
    //  Can be tweaked to include kernel times as well.
    return deltaTimeIU;

#elif OSUnix
    return (double)clock() / CLOCKS_PER_SEC;
#endif
}

unsigned int PXAPI PXProcessorTimeReal()
{
#if OSUnix
    struct timeval time;
    if(gettimeofday(&time, NULL))
    {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * 0.000001;
#elif OSWindows
    LARGE_INTEGER time, freq;
    if(!QueryPerformanceFrequency(&freq)) // Windows 2000 (+UWP), Kernel32.dll, profileapi.h
    {
        //  Handle error
        return 0;
    }
    if(!QueryPerformanceCounter(&time)) // Windows 2000 (+UWP), Kernel32.dll, profileapi.h
    {
        //  Handle error
        return 0;
    }

    const double x = ((double)time.QuadPart / (double)freq.QuadPart) * 1000000;

    return x;
#endif
}

PXActionResult PXAPI PXProcessorTemperature(PXProcessorTemperatureInfo* const pxProcessorTemperatureInfo)
{
    const PXSize oldAmount = pxProcessorTemperatureInfo->ListAmount;

    pxProcessorTemperatureInfo->ListAmount = 0;

#if OSUnix
    for(;;)
    {
        char thermalZoneFilePath[40];

        // thermal_zone is a folder and might not exist
        // in a VM this is missing
        PXTextPrintA(thermalZoneFilePath, 40, "/sys/class/thermal/thermal_zone%i/temp", pxProcessorTemperatureInfo);

        // Spamming open and close should not be the best 
        // solution but seems to be forced by the kernel, 
        // as this file is not updated otherwise.
        FILE* const fileHandle = fopen(thermalZoneFilePath, "r");

        if(NULL == fileHandle)
        {
            break;
        }

        // require memory?
        const PXBool requireResize = oldAmount < pxProcessorTemperatureInfo->ListAmount;

        if(requireResize)
        {
            pxProcessorTemperatureInfo->ListData = PXMemoryHeapReallocT(PXInt32S, pxProcessorTemperatureInfo->ListData, pxProcessorTemperatureInfo->ListAmount + 1);
        }

        fscanf(fileHandle, "%d", &pxProcessorTemperatureInfo->ListData[pxProcessorTemperatureInfo->ListAmount]);
        fclose(fileHandle);

        // Note: The temperature is usually reported in millidegrees Celsius, so divide by 1000 to get degrees Celsius
        // This is not the case on a Ryzen processor. Temp was 16°C
        // Works on Intel Core i5-2350
        pxProcessorTemperatureInfo->ListData[pxProcessorTemperatureInfo->ListAmount] /= 1000;

        ++pxProcessorTemperatureInfo->ListAmount;
    }

#elif OSWindows

    return 0;

    {
        const HRESULT initializeResultID = CoInitialize(NULL);
        const PXActionResult initializeResult = PXWindowsHandleErrorFromID(initializeResultID);

        PXActionReturnOnError(initializeResult);
    }



    /*
    Access over the driver, need admin permissions. Seems overkill

#define MSR_TEMPERATURE_TARGET 0x1A2  // Example MSR address for temperature


    HANDLE hDriver;
    DWORD bytesReturned;
    DWORD msrValue;

    // Open a handle to the driver
    hDriver = CreateFile("\\\\.\\DriverName", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hDriver == INVALID_HANDLE_VALUE) {
        printf("Failed to open driver handle.\n");
        return 1;
    }

    // Read the MSR value
    if (!DeviceIoControl(hDriver, IOCTL_READ_MSR, &MSR_TEMPERATURE_TARGET, sizeof(MSR_TEMPERATURE_TARGET), &msrValue, sizeof(msrValue), &bytesReturned, NULL)) {
        printf("Failed to read MSR.\n");
        CloseHandle(hDriver);
        return 1;
    }

    // Calculate the temperature
    int temperature = (msrValue >> 16) & 0xFF;  // Example calculation
    printf("CPU Temperature: %d°C\n", temperature);

    // Close the handle
    CloseHandle(hDriver);
}
*/



    {
        /*
        const HRESULT initializeResultID = CoInitializeSecurity
        (
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE,
            NULL
        );
        const PXActionResult initializeResult = PXWindowsHandleErrorFromID(initializeResultID);

        PXActionReturnOnError(initializeResult);
        */
    }


    IWbemLocator* wmiNameSpace = PXNull;
    IWbemServices* sevices = PXNull;
    IEnumWbemClassObject* objectList = PXNull;
    IWbemClassObject* classObject = PXNull;

    {
        const HRESULT instaceResultID = CoCreateInstance
        (
            &CLSID_WbemLocator, // CLSID_WbemAdministrativeLocator , CLSID_WbemLocator
            NULL,
            CLSCTX_INPROC_SERVER,
            &IID_IWbemLocator,
            (LPVOID*)&wmiNameSpace
        );
        const PXActionResult instaceResult = PXWindowsHandleErrorFromID(instaceResultID);

        PXActionReturnOnError(instaceResult);
    }

    // Connect
    {
        BSTR ns = SysAllocString(L"ROOT\\CimV2"); // WIM, cimv2, default CimV2

        const HRESULT connectResultID = wmiNameSpace->lpVtbl->ConnectServer
        (
            wmiNameSpace,
            ns,
            NULL,
            NULL,
            NULL,
            WBEM_FLAG_CONNECT_USE_MAX_WAIT,
            NULL,
            NULL,
            &sevices
        );
        const PXActionResult connectResult = PXWindowsHandleErrorFromID(connectResultID);

        //locator->lpVtbl->Release(locator);
        SysFreeString(ns);

        PXActionReturnOnError(connectResult);
    }

    {
        /*
        const HRESULT hres = CoSetProxyBlanket
        (
            sevices,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE
        );
        */
    }

#if 0
    {
        // Get me all the classes

        IEnumWbemClassObject* classObj = PXNull;

        const HRESULT huas = sevices->lpVtbl->CreateClassEnum
        (
            sevices,
            PXNull,
            WBEM_FLAG_FORWARD_ONLY,
            PXNull,
            &classObj
        );



        for(;;)
        {
            IWbemClassObject* clasIJA = PXNull;
            ULONG xxx = 0;

            const HRESULT iahjs = classObj->lpVtbl->Next(classObj, 0, 1, &clasIJA, &xxx);


            VARIANT vvv;
            SAFEARRAY saveAFGT;

            const HRESULT jfni = clasIJA->lpVtbl->GetNames(clasIJA, 0, 0, &vvv, &saveAFGT);

            //printf("");
            PXConsoleWriteF(0, "");
        }


        //printf("");
    }


    // Querry
    {
        // Win32_TemperatureProbe does not seem to work on my machine

        // BSTR query = SysAllocString(L"SELECT * FROM Win32_TemperatureProbe");
        //  wchar_t strQuery[] = L"SELECT * FROM Win32_TemperatureProbe"; // Win32_BaseBoard Win32_MemoryDevice

        wchar_t strQuery[] = L"SELECT * FROM Win32_TemperatureProbe"; // Win32_BaseBoard Win32_MemoryDevice


        //BSTR query = SysAllocString(L"SELECT * FROM MSAcpi_ThermalZoneTemperature");
        //BSTR query = SysAllocString(L"select * from cim_temperaturesensor");
        //BSTR query = SysAllocString(L"SELECT * FROM CIM_TemperatureSensor");

        // BSTR wql = SysAllocString(L"WQL");
        wchar_t strQueryLanguage[] = L"WQL"; // WQL

        const HRESULT querryResultID = sevices->lpVtbl->ExecQuery
        (
            sevices,
            strQueryLanguage,
            strQuery,
            WBEM_FLAG_FORWARD_ONLY, // WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
            NULL,
            &objectList
        );
        const PXActionResult querryResult = PXWindowsHandleErrorFromID(querryResultID);

        IErrorInfo* errInfo;

        HRESULT wxx = GetErrorInfo(0, &errInfo);

        BSTR buffer = PXNull;
        BSTR desc = PXNull;
        BSTR vvvvv = PXNull;
        BSTR ccc = PXNull;
        BSTR guiod = PXNull;

        errInfo->lpVtbl->GetSource(errInfo, &guiod);
        errInfo->lpVtbl->GetSource(errInfo, &buffer);
        errInfo->lpVtbl->GetDescription(errInfo, &desc);
        errInfo->lpVtbl->GetHelpFile(errInfo, &vvvvv);
        errInfo->lpVtbl->GetHelpContext(errInfo, &ccc);

        // SysFreeString(wql);
        //SysFreeString(query);
        //sevices->lpVtbl->Release(sevices);

        PXActionReturnOnError(querryResult);
    }


    // Get interface
    {
        /*
        COAUTHIDENTITY* userAcct = NULL;

        const HRESULT SSSS = CoSetProxyBlanket
        (
            objectList,
            RPC_C_AUTHN_DEFAULT,
            RPC_C_AUTHZ_DEFAULT,
            COLE_DEFAULT_PRINCIPAL,
            RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            userAcct,
            EOAC_NONE
        );
        */

        for(;;)
        {
            ULONG returned = 0;
            const HRESULT nextResultID = objectList->lpVtbl->Next
            (
                objectList,
                WBEM_INFINITE,
                1,
                &classObject,
                &returned
            );
            const PXActionResult nextResult = PXWindowsHandleErrorFromID(nextResultID);


            PXActionReturnOnError(nextResult);

            if(S_FALSE == nextResultID)
            {
                break;;
            }




            while(1)
            {
                VARIANT var_val;
                const HRESULT SSSS = classObject->lpVtbl->Get(classObject, L"Name", 0, &var_val, NULL, NULL);


                //  classObject->lpVtbl->Next
                // (
              //  classObject,
                    //
                                    //  );


                  //  printf("");
            }



          //  printf("");
        }






        // objectList->lpVtbl->Release(objectList);






    }
#endif


    // Get value
    {
        /*
        BSTR temp = SysAllocString(L"CurrentTemperature");
        VARIANT v;
        VariantInit(&v);
        const HRESULT getResultID = classObject->lpVtbl->Get(classObject, temp, 0, &v, NULL, NULL);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        classObject->lpVtbl->Release(classObject);
        SysFreeString(temp);
        if(PXActionSuccessful == getResult)
        {
            cpuTemperature = V_I4(&v);
        }
        VariantClear(&v);
        */
    }

    CoUninitialize();
#else
// ToDo: Not supported?
#endif

    //-----------------------------------------------------
    // Calc Average
    //-----------------------------------------------------
    pxProcessorTemperatureInfo->Average = 0;

    for(PXSize i = 0; i < pxProcessorTemperatureInfo->ListAmount; ++i)
    {
        pxProcessorTemperatureInfo->Average += pxProcessorTemperatureInfo->ListData[i];
    }

    if(pxProcessorTemperatureInfo->ListAmount)
    {
        pxProcessorTemperatureInfo->Average /= pxProcessorTemperatureInfo->ListAmount;
    } 
    //-----------------------------------------------------

    return PXActionSuccessful;
}

void PXAPI PXProcessorNoOperation()
{
#if OSUnix
#elif OSWindows && PXDefaultLibraryEnable
    __nop();
#endif
}

void PXAPI PXProcessorRandomNumber()
{
#if OSUnix
#elif OSWindows && PXDefaultLibraryEnable
    _rdrand32_step(0);
#endif
}

void PXAPI PXProcessorSwapByteOrderI16U(PXInt16U* const value)
{
#if OSUnix
#elif OSWindows && PXDefaultLibraryEnable
    * value = _byteswap_ushort(*value);
#endif
}

void PXAPI PXProcessorSwapByteOrderI32U(PXInt32U* const value)
{
#if OSUnix 
#elif OSWindows && PXDefaultLibraryEnable
    * value = _byteswap_ulong(*value);
#endif
}

void PXAPI PXProcessorSwapByteOrderI64U(PXInt64U* const value)
{
#if OSUnix
#elif OSWindows && PXDefaultLibraryEnable
    * value = _byteswap_uint64(*value);
#endif
}