#ifndef PXProcessorINCLUDE
#define PXProcessorINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum PXProcessorBrand_
    {
        ProcessorBrandInvalid,
        ProcessorBrandAMD,
        ProcessorBrandIntel,
        ProcessorBrandWinChip,
        ProcessorBrandCyrix,
        ProcessorBrandTransmeta,
        ProcessorBrandNationalSemiconductor,
        ProcessorBrandNexGen,
        ProcessorBrandRise,
        ProcessorBrandSiS,
        ProcessorBrandUMC,
        ProcessorBrandVIA,
        ProcessorBrandDMAndPVortex,
        ProcessorBrandZhaoxin,
        ProcessorBrandHygon,
        ProcessorBrandMCSTElbrus,
        ProcessorBrandao486,
        ProcessorBrandbhyve,
        ProcessorBrandKVM,
        ProcessorBrandQEMU,
        ProcessorBrandMicrosoftHyperVOrVirtualPC,
        ProcessorBrandMicrosoftx86ToARM,
        ProcessorBrandParallels,
        ProcessorBrandVMware,
        ProcessorBrandXenHVM,
        ProcessorBrandProjectACRN,
        ProcessorBrandQNXHypervisor,
        ProcessorBrandAppleRosetta
    }
    PXProcessorBrand;

    typedef enum PXProcessorModelName_
    {
        ProcessorModelNameInvalid,

        Intel486DX25x33,
        Intel486DX50,
        Intel486SX,
        Intel486DX2,
        Intel486SL,
        Intel486SX2,
        Intel486DX2WB,
        Intel486DX4,
        Intel486DX4WB,
        IntelPentium60x66AStep,
        IntelPentium60x66,
        IntelPentium75x200,
        IntelOverDrivePODP5V83,
        IntelPentiumMMX,
        IntelMobilePentium75x200,
        IntelMobilePentiumMMX,
        IntelPentiumProAStep,
        IntelPentiumPro,
        IntelPentiumIIKlaPXMath,
        IntelPentiumIIDeschutes,
        IntelCeleronMendocino,
        IntelPentiumIIIKatmai,
        IntelPentiumIIICoppermine,
        IntelMobilePentiumIII,
        IntelPentiumIII0x18um,
        IntelPentiumIII0x13um,
        IntelItanium,
        IntelPentiumIV0x18um,
        IntelPentiumIV0x13um,
        IntelPentiumIV0x09um,
        IntelItanium2,

        AMD486DX2,
        AMD486DX2WB,
        AMD486DX4,
        AMD486DX4WB,
        AMDAm5x86WT,
        AMDAm5x86WB,
        AMDK5SSA5,
        AMDK5,
        AMDK6,
        AMDK62,
        AMDK63,
        AMDK63P,
        AMDAthlon0x25um,
        AMDAthlon0x18um,
        AMDDuron,
        AMDAthlonThunderbird,
        AMDAthlonPalamino,
        AMDDuronMorgan,
        AMDAthlonThoroughbred,
        AMDAthlonBarton,
        AMDAthlon64,
        AMDAthlon64FXOperon

    }
    PXProcessorModelName;

    PXPrivate void PXProcessorModelNameGet(const PXProcessorModelName processorModelName, char* const name);
    PXPrivate PXProcessorBrand PXProcessorBrandDetect(const char* name);
    PXPrivate PXProcessorModelName PXProcessorModelNameDetect(const PXProcessorBrand processorBrand, const unsigned char famliy, const unsigned char model);

	typedef struct PXProcessor_
	{       
        char BrandName[64];  // string includes manufacturer, model and clockspeed
        char IdentityString[13];

        PXProcessorBrand BrandID;
        PXProcessorModelName ModelNameID;

        PXInt8U NumberOfProcessors;
        PXInt8U BrandNameSize;

        // eax 
        PXBool SteppingID;
        PXBool Model;
        PXBool Family;
        PXBool ProcessorType; // Intel Only
        PXBool ReservedBitsA;
        PXBool ExtendedModel;
        PXBool ExtendedFamily;
        PXBool ReservedBitsB;

        // EBX
        PXBool BrandIndex;
        PXBool CLflushCacheLineSize;
        PXBool logicalProcessors;
        PXBool initialApicID;


        // ecx
        PXBool  SSE3Instructions;
        PXBool  ReservedBitsAAB;
        PXBool  MonitorMWait;
        PXBool cplQualifiedDebugStore;// Intel Only
        PXBool VirtualmachineExtensions; // Intel Only
        PXBool saferModeExtensions; // Intel Only
        PXBool enhancedIntelSpeedstepExtensions;// Intel Only
        PXBool thermalMonitor;// Intel Only
        PXBool supplementalSSE3;
        PXBool L1ContextID; // Intel Only
        PXBool ReservedBitsAAC;
        PXBool FMAExtensions;// Intel Only
        PXBool  CompareExchange16XSupport;// Intel Only
        PXBool  xTPRUpdateControl;
        PXBool  performanceDebugCapabilityMSR;
        PXBool  ReservedBitsAAF;
        PXBool DirectCacheAccess; // Intel Only
        PXBool SSE4x1Extensions;
        PXBool SSE4x2Extensions;
        PXBool x2APICSupport; // Intel Only
        PXBool MOVBEInstructionSupport; // Intel Only
        PXBool POPCNTInstructionSupport;
        PXBool ReservedBitsAAE;
        PXBool AESInstructionSupport; // Intel Only
        PXBool XSAVEInstructionSupport; // Intel Only
        PXBool OSXSAVEInstructionSupport; // Intel Only
        PXBool AdvancedVectorExtensions;// Intel Only
        PXBool ReservedBitsAAA;

        // EDX
        PXBool  FPU;// -x87 FPU on chip : % d\n", cpuinfo[3] >> 0 & 1;
        PXBool  VME;//Virtual-8086 mode enhancement: %d\n", cpuinfo[3] >> 1 & 1;
        PXBool  DE;//Debugging extensions: %d\n", cpuinfo[3] >> 2 & 1;
        PXBool  PSE;//Page size extensions: %d\n", cpuinfo[3] >> 3 & 1;
        PXBool  TSC;//Time stamp counter: %d\n", cpuinfo[3] >> 4 & 1;
        PXBool  MSR;//RDMSR and WRMSR support: %d\n", cpuinfo[3] >> 5 & 1;
        PXBool  PAE;//Physical address extensions: %d\n", cpuinfo[3] >> 6 & 1;
        PXBool  MCE;//Machine check exception: %d\n", cpuinfo[3] >> 7 & 1;
        PXBool  CX8;//CMPXCHG8B instruction: %d\n", cpuinfo[3] >> 8 & 1;
        PXBool  APIC;//APIC on chip: %d\n", cpuinfo[3] >> 9 & 1;
        PXBool  nAAaAAAAA;//Reserved: %d\n", cpuinfo[3] >> 10 & 1;
        PXBool  SEP;//SYSENTER and SYSEXIT: %d\n", cpuinfo[3] >> 11 & 1;
        PXBool  MTRR;//Memory type range registers: %d\n", cpuinfo[3] >> 12 & 1;
        PXBool  PGE;//PTE global bit: %d\n", cpuinfo[3] >> 13 & 1;
        PXBool  MCA;//Machine check architecture: %d\n", cpuinfo[3] >> 14 & 1;
        PXBool  CMOV;//Conditional move/compare instruction: %d\n", cpuinfo[3] >> 15 & 1;
        PXBool  PAT;//Page Attribute Table: %d\n", cpuinfo[3] >> 16 & 1;
        PXBool  PSEE;//-36 - 36-bit page size extension: %d\n", cpuinfo[3] >> 17 & 1;
        PXBool  PSN;// Processor serial number: %d\n", cpuinfo[3] >> 18 & 1;
        PXBool  CLFSH;// CFLUSH instruction: %d\n", cpuinfo[3] >> 19 & 1;
        PXBool  nAA;// -/ a Reserved : % d\n", cpuinfo[3] >> 20 & 1;
        PXBool  DS;// Debug store: %d\n", cpuinfo[3] >> 21 & 1;
        PXBool  ACPI;// Thermal monitor and clock control: %d\n", cpuinfo[3] >> 22 & 1;
        PXBool  MMX;// MMX technology: %d\n", cpuinfo[3] >> 23 & 1;
        PXBool  FXSR;// FXSAVE/FXRSTOR: %d\n", cpuinfo[3] >> 24 & 1;
        PXBool  SSE;// SSE extensions: %d\n", cpuinfo[3] >> 25 & 1;
        PXBool  SSE2;// SSE2 extensions: %d\n", cpuinfo[3] >> 26 & 1;
        PXBool  SS;// Self snoop: %d\n", cpuinfo[3] >> 27 & 1;
        PXBool  HTT;// Multithreading: %d\n", cpuinfo[3] >> 28 & 1;
        PXBool  TM;// Thermal monitor: %d\n", cpuinfo[3] >> 29 & 1;
        PXBool  nAAAABB;// Reserved: %d\n", cpuinfo[3] >> 30 & 1;
        PXBool  PBE;// Pending break enable: %d\n", cpuinfo[3] >> 31 & 1;
	}
	PXProcessor;

	PXPublic void PXAPI PXProcessorFetchInfo(PXProcessor* const processor);

	PXPublic unsigned int PXAPI PXProcessorFrequencyCurrent();
	PXPublic unsigned int PXAPI PXProcessorTimeReal();

    PXPublic PXActionResult PXAPI PXProcessorTemperature(PXInt32U* const cpuTemp);


    //-----------------------------------------------------
    // Intrinsic function
    //-----------------------------------------------------

    // NOP - Execute no command.
    // Can have a side effect where the pipeline can still process stuff
    // So this command is not useless, it can help to flush out parralel running actions.
    PXPublic void PXAPI PXProcessorNoOperation();

    PXPublic void PXAPI PXProcessorSwapByteOrderI16U(PXInt16U* const value);
    PXPublic void PXAPI PXProcessorSwapByteOrderI32U(PXInt32U* const value);
    PXPublic void PXAPI PXProcessorSwapByteOrderI64U(PXInt64U* const value);

#ifdef __cplusplus
}
#endif

#endif
