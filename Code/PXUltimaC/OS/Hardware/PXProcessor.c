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

#pragma comment(lib, "wbemuuid.lib")

#endif

#include <stdio.h>
#include <stdlib.h>
#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>

void PXProcessorModelNameGet(const PXProcessorModelName processorModelName, char* const name)
{
    const char* processorName = 0;

    switch (processorModelName)
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

PXProcessorBrand PXProcessorBrandDetect(const char* name)
{
    const PXSize bufferSize = 13;

    if (PXTextCompareA("AMDisbetter!", bufferSize, name, bufferSize)) return ProcessorBrandAMD;
    if (PXTextCompareA("AuthenticAMD", bufferSize, name, bufferSize)) return ProcessorBrandAMD;
    if (PXTextCompareA("CentaurHauls", bufferSize, name, bufferSize)) return ProcessorBrandWinChip;
    if (PXTextCompareA("CyrixInstead", bufferSize, name, bufferSize)) return ProcessorBrandCyrix;
    if (PXTextCompareA("GenuineIntel", bufferSize, name, bufferSize)) return ProcessorBrandIntel;
    if (PXTextCompareA("TransmetaCPU", bufferSize, name, bufferSize)) return ProcessorBrandTransmeta;
    if (PXTextCompareA("GenuineTMx86", bufferSize, name, bufferSize)) return ProcessorBrandTransmeta;
    if (PXTextCompareA("Geode by NSC", bufferSize, name, bufferSize)) return ProcessorBrandNationalSemiconductor;
    if (PXTextCompareA("NexGenDriven", bufferSize, name, bufferSize)) return ProcessorBrandNexGen;
    if (PXTextCompareA("RiseRiseRise", bufferSize, name, bufferSize)) return ProcessorBrandRise;
    if (PXTextCompareA("SiS SiS SiS ", bufferSize, name, bufferSize)) return ProcessorBrandSiS;
    if (PXTextCompareA("UMC UMC UMC ", bufferSize, name, bufferSize)) return ProcessorBrandUMC;
    if (PXTextCompareA("VIA VIA VIA ", bufferSize, name, bufferSize)) return ProcessorBrandVIA;
    if (PXTextCompareA("Vortex86 SoC", bufferSize, name, bufferSize)) return ProcessorBrandDMAndPVortex;
    if (PXTextCompareA("  Shanghai  ", bufferSize, name, bufferSize)) return ProcessorBrandZhaoxin;
    if (PXTextCompareA("HygonGenuine", bufferSize, name, bufferSize)) return ProcessorBrandHygon;
    if (PXTextCompareA("E2K MACHINE", bufferSize, name, bufferSize)) return ProcessorBrandMCSTElbrus;
    if (PXTextCompareA("MiSTer AO486", bufferSize, name, bufferSize)) return ProcessorBrandao486;
    if (PXTextCompareA("bhyve bhyve ", bufferSize, name, bufferSize)) return ProcessorBrandbhyve;
    if (PXTextCompareA(" KVMKVMKVM  ", bufferSize, name, bufferSize)) return ProcessorBrandKVM;
    if (PXTextCompareA("TCGTCGTCGTCG", bufferSize, name, bufferSize)) return ProcessorBrandQEMU;
    if (PXTextCompareA("Microsoft Hv", bufferSize, name, bufferSize)) return ProcessorBrandMicrosoftHyperVOrVirtualPC;
    if (PXTextCompareA("MicrosoftXTA", bufferSize, name, bufferSize)) return ProcessorBrandMicrosoftx86ToARM;
    if (PXTextCompareA(" lrpepyh  vr", bufferSize, name, bufferSize)) return ProcessorBrandParallels;
    if (PXTextCompareA("VMwareVMware", bufferSize, name, bufferSize)) return ProcessorBrandVMware;
    if (PXTextCompareA("XenVMMXenVMM", bufferSize, name, bufferSize)) return ProcessorBrandXenHVM;
    if (PXTextCompareA("ACRNACRNACRN", bufferSize, name, bufferSize)) return ProcessorBrandProjectACRN;
    if (PXTextCompareA(" QNXQVMBSQG ", bufferSize, name, bufferSize)) return ProcessorBrandQNXHypervisor;
    if (PXTextCompareA("VirtualApple", bufferSize, name, bufferSize)) return ProcessorBrandAppleRosetta;

    return ProcessorBrandInvalid;
}

PXProcessorModelName PXProcessorModelNameDetect(const PXProcessorBrand processorBrand, const unsigned char famliy, const unsigned char model)
{       
    const PXInt16U id = PXInt16Make(famliy, model);

    switch (processorBrand)
    {
        case ProcessorBrandIntel:
        {
            switch (id)
            {
                case PXInt16Make(4, 0): return Intel486DX25x33;
                case PXInt16Make(4, 1): return Intel486DX50;
                case PXInt16Make(4, 2): return Intel486SX;
                case PXInt16Make(4, 3): return Intel486DX2;
                case PXInt16Make(4, 4): return Intel486SL;
                case PXInt16Make(4, 5): return Intel486SX2;
                case PXInt16Make(4, 7): return Intel486DX2WB;
                case PXInt16Make(4, 8): return Intel486DX4;
                case PXInt16Make(4, 9): return Intel486DX4WB;

                case PXInt16Make(5, 0): return IntelPentium60x66AStep;
                case PXInt16Make(5, 1): return IntelPentium60x66;
                case PXInt16Make(5, 2): return IntelPentium75x200;
                case PXInt16Make(5, 3): return IntelOverDrivePODP5V83;
                case PXInt16Make(5, 4): return IntelPentiumMMX;
                case PXInt16Make(5, 7): return IntelMobilePentium75x200;
                case PXInt16Make(5, 8): return IntelMobilePentiumMMX;

                case PXInt16Make(6, 1): return IntelPentiumProAStep;
                case PXInt16Make(6, 2): return IntelPentiumPro;
                case PXInt16Make(6, 3): return IntelPentiumIIKlaPXMath;
                case PXInt16Make(6, 5): return IntelPentiumIIDeschutes;
                case PXInt16Make(6, 6): return IntelCeleronMendocino;
                case PXInt16Make(6, 7): return IntelPentiumIIIKatmai;
                case PXInt16Make(6, 8): return IntelPentiumIIICoppermine;
                case PXInt16Make(6, 9): return IntelMobilePentiumIII;
                case PXInt16Make(6, 10): return IntelPentiumIII0x18um;
                case PXInt16Make(6, 11): return IntelPentiumIII0x13um;
            }

            if (famliy == 7)
            {
                return IntelItanium;
            }

            break;
        }

        case ProcessorBrandAMD:
        {
            switch (id)
            {
                case PXInt16Make(4, 3): return AMD486DX2;
                case PXInt16Make(4, 7): return AMD486DX2WB;
                case PXInt16Make(4, 8): return AMD486DX4;
                case PXInt16Make(4, 9): return AMD486DX4WB;
                case PXInt16Make(4, 14): return AMDAm5x86WT;
                case PXInt16Make(4, 15): return AMDAm5x86WB;

                case PXInt16Make(5, 0): return AMDK5SSA5;
                case PXInt16Make(5, 1): // fallthough
                case PXInt16Make(5, 2): // fallthough
                case PXInt16Make(5, 3): return AMDK5;

                case PXInt16Make(5, 6):  // fallthough
                case PXInt16Make(5, 7): return AMDK6;

                case PXInt16Make(5, 8): return AMDK62;
                case PXInt16Make(5, 9): return AMDK63;
                case PXInt16Make(5, 13): return AMDK63P;

                case PXInt16Make(6, 0): // fallthough
                case PXInt16Make(6, 1): return AMDAthlon0x25um;
                case PXInt16Make(6, 2): return AMDAthlon0x18um;
                case PXInt16Make(6, 3): return AMDDuron;
                case PXInt16Make(6, 4): return AMDAthlonThunderbird;
                case PXInt16Make(6, 6): return AMDAthlonPalamino;
                case PXInt16Make(6, 7): return AMDDuronMorgan;
                case PXInt16Make(6, 8): return AMDAthlonThoroughbred;
                case PXInt16Make(6, 10): return AMDAthlonBarton;
            }

            if (famliy == 15)
            {
                // Extended family...
            }

            break;
        }
    }   

    return ProcessorModelNameInvalid;
}

void PXProcessorFetchInfo(PXProcessor* const processor)
{
    PXMemoryClear(processor, sizeof(PXProcessor));

#if OSUnix


#elif OSWindows 
    //
    {
        int cpuinfo[4] = { 0,0,0,0 };

        __cpuid(cpuinfo, 0);

        processor->IdentityString[0] = cpuinfo[1] & 0xff;
        processor->IdentityString[1] = cpuinfo[1] >> 8 & 0xff;
        processor->IdentityString[2] = cpuinfo[1] >> 16 & 0xff;
        processor->IdentityString[3] = cpuinfo[1] >> 24 & 0xff;

        processor->IdentityString[4] = cpuinfo[3] & 0xff;
        processor->IdentityString[5] = cpuinfo[3] >> 8 & 0xff;
        processor->IdentityString[6] = cpuinfo[3] >> 16 & 0xff;
        processor->IdentityString[7] = cpuinfo[3] >> 24 & 0xff;

        processor->IdentityString[8] = cpuinfo[2] & 0xff;
        processor->IdentityString[9] = cpuinfo[2] >> 8 & 0xff;
        processor->IdentityString[10] = cpuinfo[2] >> 16 & 0xff;
        processor->IdentityString[11] = cpuinfo[2] >> 24 & 0xff;
        processor->IdentityString[12] = 0;

        processor->BrandID = PXProcessorBrandDetect(processor->IdentityString);
    }


    {
        int cpuinfo[4] = { 0,0,0,0 };

        __cpuid(cpuinfo, 1);

        processor->SteppingID = cpuinfo[0] & 0xf;
        processor->Model = cpuinfo[0] >> 4 & 0xf;
        processor->Family = cpuinfo[0] >> 8 & 0xf;
        processor->ProcessorType = cpuinfo[0] >> 12 & 0x3;
        processor->ReservedBitsA = cpuinfo[0] >> 14 & 0x3;
        processor->ExtendedModel = cpuinfo[0] >> 16 & 0xf;
        processor->ExtendedFamily = cpuinfo[0] >> 20 & 0xff;
        processor->ReservedBitsB = cpuinfo[0] >> 28 & 0xf;

        processor->ModelNameID = PXProcessorModelNameDetect(processor->BrandID, processor->Family, processor->Model);

        processor->BrandIndex = cpuinfo[1] & 0xff;
        processor->CLflushCacheLineSize = cpuinfo[1] >> 8 & 0xff;
        processor->logicalProcessors = cpuinfo[1] >> 16 & 0xff;
        processor->initialApicID = cpuinfo[1] >> 24 & 0xff;

        processor->SSE3Instructions = cpuinfo[2] & 1;
        processor->ReservedBitsAAB = cpuinfo[2] >> 1 & 3;
        processor->MonitorMWait = cpuinfo[2] >> 3 & 1;
        processor->cplQualifiedDebugStore = cpuinfo[2] >> 4 & 1;
        processor->VirtualmachineExtensions = cpuinfo[2] >> 5 & 1;
        processor->saferModeExtensions = cpuinfo[2] >> 6 & 1;
        processor->enhancedIntelSpeedstepExtensions = cpuinfo[2] >> 7 & 1;
        processor->thermalMonitor = cpuinfo[2] >> 8 & 1;
        processor->supplementalSSE3 = cpuinfo[2] >> 9 & 1;
        processor->L1ContextID = cpuinfo[2] >> 10 & 1;
        processor->ReservedBitsAAC = cpuinfo[2] >> 11 & 1;
        processor->FMAExtensions = cpuinfo[2] >> 12 & 1;
        processor->CompareExchange16XSupport = cpuinfo[2] >> 13 & 1;
        processor->xTPRUpdateControl = cpuinfo[2] >> 14 & 1;
        processor->performanceDebugCapabilityMSR = cpuinfo[2] >> 15 & 1;
        processor->ReservedBitsAAF = cpuinfo[2] >> 16 & 3;
        processor->DirectCacheAccess = cpuinfo[2] >> 18 & 1;
        processor->SSE4x1Extensions = cpuinfo[2] >> 19 & 1;
        processor->SSE4x2Extensions = cpuinfo[2] >> 20 & 1;
        processor->x2APICSupport = cpuinfo[2] >> 21 & 1;
        processor->MOVBEInstructionSupport = cpuinfo[2] >> 22 & 1;
        processor->POPCNTInstructionSupport = cpuinfo[2] >> 23 & 1;
        processor->ReservedBitsAAE = cpuinfo[2] >> 24 & 1;
        processor->AESInstructionSupport = cpuinfo[2] >> 25 & 1;
        processor->XSAVEInstructionSupport = cpuinfo[2] >> 26 & 1;
        processor->OSXSAVEInstructionSupport = cpuinfo[2] >> 27 & 1;
        processor->AdvancedVectorExtensions = cpuinfo[2] >> 28 & 1;
        processor->ReservedBitsAAA = cpuinfo[2] >> 29 & 0xf;


        // EDX
        processor->FPU = cpuinfo[3] >> 0 & 1;
        processor->VME = cpuinfo[3] >> 1 & 1;
        processor->DE = cpuinfo[3] >> 2 & 1;
        processor->PSE = cpuinfo[3] >> 3 & 1;
        processor->TSC = cpuinfo[3] >> 4 & 1;
        processor->MSR = cpuinfo[3] >> 5 & 1;
        processor->PAE = cpuinfo[3] >> 6 & 1;
        processor->MCE = cpuinfo[3] >> 7 & 1;
        processor->CX8 = cpuinfo[3] >> 8 & 1;
        processor->APIC = cpuinfo[3] >> 9 & 1;
        processor->nAAaAAAAA = cpuinfo[3] >> 10 & 1;
        processor->SEP = cpuinfo[3] >> 11 & 1;
        processor->MTRR = cpuinfo[3] >> 12 & 1;
        processor->PGE = cpuinfo[3] >> 13 & 1;
        processor->MCA = cpuinfo[3] >> 14 & 1;
        processor->CMOV = cpuinfo[3] >> 15 & 1;
        processor->PAT = cpuinfo[3] >> 16 & 1;
        processor->PSEE = cpuinfo[3] >> 17 & 1;
        processor->PSN = cpuinfo[3] >> 18 & 1;
        processor->CLFSH = cpuinfo[3] >> 19 & 1;
        processor->nAA = cpuinfo[3] >> 20 & 1;
        processor->DS = cpuinfo[3] >> 21 & 1;
        processor->ACPI = cpuinfo[3] >> 22 & 1;
        processor->MMX = cpuinfo[3] >> 23 & 1;
        processor->FXSR = cpuinfo[3] >> 24 & 1;
        processor->SSE = cpuinfo[3] >> 25 & 1;
        processor->SSE2 = cpuinfo[3] >> 26 & 1;
        processor->SS = cpuinfo[3] >> 27 & 1;
        processor->HTT = cpuinfo[3] >> 28 & 1;
        processor->TM = cpuinfo[3] >> 29 & 1;
        processor->nAAAABB = cpuinfo[3] >> 30 & 1;
        processor->PBE = cpuinfo[3] >> 31 & 1; 
    }




    // Detect CPU Name
    {


        int CPUInfo[4] = { 0,0,0,0 };
        unsigned   nExIds;

        // Get the information associated with each extended ID.

        __cpuid(CPUInfo, 0x80000000);

        nExIds = CPUInfo[0];
        // Interpret CPU brand string
        for (PXSize i = 0x80000000; i <= nExIds; ++i)
        {
            __cpuid(CPUInfo, i);

            switch (i)
            {
                case 0x80000002:
                case 0x80000003:
                case 0x80000004:
                {
                    processor->BrandNameSize += PXTextCopyA((char*)CPUInfo, sizeof(CPUInfo), processor->BrandName + processor->BrandNameSize, 64);
                    break;
                }
                default:
                    break;
            }
        }

        if (processor->BrandNameSize < 5)
        {
            PXProcessorModelNameGet(processor->ModelNameID, processor->BrandName);
        }
    }



    
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

        for (WORD i = 0; i < nrOfProcessorGroups; ++i)
        {
            const DWORD activeProcessors = GetActiveProcessorCount(i); // Windows 7, kernel32.dll, winbase.h
        }
#endif
    }

    for (int i = processor->BrandNameSize-1; i > 0; --i)
    {
        const PXBool whiteSpace = processor->BrandName[i] == ' ';

        if (!whiteSpace) 
        {
            break;
        }

        processor->BrandNameSize--;
        processor->BrandName[i] = '\0';
    }
#endif
}

unsigned int PXProcessorFrequencyCurrent()
{
#if OSWindows
    FILETIME a, b, c, d;
    const HANDLE process = GetCurrentProcess(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h 
    const PXBool sucessful = GetProcessTimes(process, &a, & b, & c, & d); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h 

    if (!sucessful)
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

unsigned int PXProcessorTimeReal()
{
#if OSUnix
    struct timeval time;
    if (gettimeofday(&time, NULL))
    {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * 0.000001;
#elif OSWindows
    LARGE_INTEGER time, freq;
    if (!QueryPerformanceFrequency(&freq)) // Windows 2000 (+UWP), Kernel32.dll, profileapi.h
    {
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)) // Windows 2000 (+UWP), Kernel32.dll, profileapi.h
    {
        //  Handle error
        return 0;
    }

    const double x = ((double)time.QuadPart / (double)freq.QuadPart) * 1000000;

    return x;
#endif
}

PXActionResult PXAPI PXProcessorTemperature(PXInt32U* const cpuTemp)
{
    PXInt32U cpuTemperature = -1;

    *cpuTemp = cpuTemperature;

#if OSUnix

#elif OSWindows  

    {
        const HRESULT initializeResultID = CoInitialize(NULL);
        const PXActionResult initializeResult = PXWindowsHandleErrorFromID(initializeResultID);

        PXActionReturnOnError(initializeResult);
    }

    {
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
    }


    IWbemLocator* locator = PXNull;
    IWbemServices* sevices = PXNull;
    IEnumWbemClassObject* objectList = PXNull;
    IWbemClassObject* classObject = PXNull;

    {
        const HRESULT instaceResultID = CoCreateInstance
        (
            &CLSID_WbemLocator, // CLSID_WbemAdministrativeLocator
            NULL, 
            CLSCTX_INPROC_SERVER, 
            &IID_IWbemLocator,
            (LPVOID*)&locator
        );
        const PXActionResult instaceResult = PXWindowsHandleErrorFromID(instaceResultID);

        PXActionReturnOnError(instaceResult);
    }   
    
    // Connect
    {
        BSTR ns = SysAllocString(L"ROOT\\CimV2"); // WIM, cimv2, default

        const HRESULT connectResultID = locator->lpVtbl->ConnectServer(locator, ns, NULL, NULL, NULL, 0, NULL, NULL, &sevices);
        const PXActionResult connectResult = PXWindowsHandleErrorFromID(connectResultID);

        //locator->lpVtbl->Release(locator);
        SysFreeString(ns);

        PXActionReturnOnError(connectResult);
    }
           
    {
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
    }

  
    // Querry
    {
        BSTR query = SysAllocString(L"SELECT * FROM Win32_TemperatureProbe");
        //BSTR query = SysAllocString(L"SELECT * FROM MSAcpi_ThermalZoneTemperature");
        //BSTR query = SysAllocString(L"select * from cim_temperaturesensor");
        //BSTR query = SysAllocString(L"SELECT * FROM CIM_TemperatureSensor");

        BSTR wql = SysAllocString(L"WQL");
  
        const HRESULT querryResultID = sevices->lpVtbl->ExecQuery(sevices, wql, query, WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &objectList);
        const PXActionResult querryResult = PXWindowsHandleErrorFromID(querryResultID);
        
        SysFreeString(wql);
        SysFreeString(query);
        //sevices->lpVtbl->Release(sevices);

        PXActionReturnOnError(querryResult);
    }


    // Get interface    
    {
        COAUTHIDENTITY* userAcct = NULL;

        const HRESULT SSSS = CoSetProxyBlanket(
            objectList,
            RPC_C_AUTHN_DEFAULT,
            RPC_C_AUTHZ_DEFAULT,
            COLE_DEFAULT_PRINCIPAL,
            RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            userAcct,
            EOAC_NONE
        );
     
        ULONG returned = 0;
        const HRESULT nextResultID = objectList->lpVtbl->Next(objectList, WBEM_INFINITE, 1, &classObject, &returned);
        const PXActionResult nextResult = PXWindowsHandleErrorFromID(nextResultID);

       // objectList->lpVtbl->Release(objectList);

        PXActionReturnOnError(nextResult);

        if (S_FALSE == nextResultID)
        {
            return PXActionInvalid;
        }
    }


    // Get value
    {    
        BSTR temp = SysAllocString(L"CurrentTemperature");
        VARIANT v;
        VariantInit(&v);
        const HRESULT getResultID = classObject->lpVtbl->Get(classObject, temp, 0, &v, NULL, NULL);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        classObject->lpVtbl->Release(classObject);
        SysFreeString(temp);
        if (PXActionSuccessful == getResult)
        {
            cpuTemperature = V_I4(&v);
        }
        VariantClear(&v);
    }
    
    CoUninitialize();        
#else 
#endif

    *cpuTemp = cpuTemperature;

    return PXActionSuccessful;
}

void PXProcessorSwapByteOrderI16U(PXInt16U* const value)
{
#if OSUnix
#elif OSWindows
    *value = _byteswap_ushort(*value);
#endif
}

void PXProcessorSwapByteOrderI32U(PXInt32U* const value)
{
#if OSUnix
#elif OSWindows
    *value = _byteswap_ulong(*value);
#endif
}

void PXProcessorSwapByteOrderI64U(PXInt64U* const value)
{
#if OSUnix
#elif OSWindows
    *value = _byteswap_uint64(*value);
#endif
}