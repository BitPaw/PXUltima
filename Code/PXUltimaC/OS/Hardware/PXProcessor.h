#ifndef PXProcessorINCLUDE
#define PXProcessorINCLUDE

#include <Media/PXResource.h>

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












#define PXProcessorFeatureRDRAND                            ((1 << 30) << 32)
#define PXProcessorFeatureF16C                              ((1 << 29) << 32)
#define PXProcessorFeatureAVX                               ((1 << 28) << 32)
#define PXProcessorFeatureOSXSAVE                           ((1 << 27) << 32)
#define PXProcessorFeatureXSAVE                             ((1 << 26) << 32)
#define PXProcessorFeatureAES                               ((1 << 25) << 32)
#define PXProcessorFeatureTSCDeadline                       ((1 << 24) << 32)
#define PXProcessorFeaturePOPCNT                            ((1 << 23) << 32)
#define PXProcessorFeatureMOVBE                             ((1 << 22) << 32)
#define PXProcessorFeaturex2APIC                            ((1 << 21) << 32)
#define PXProcessorFeatureSSE4x2                            ((1 << 20) << 32)
#define PXProcessorFeatureSSE4x1                            ((1 << 19) << 32)
#define PXProcessorFeatureDirectCacheAccess                 ((1 << 18) << 32)
#define PXProcessorFeatureProcessContextIdentifiers         ((1 << 17) << 32)
#define PXProcessorFeatureUnused                            ((1 << 16) << 32)
#define PXProcessorFeaturePerfDebugCapabilityMSR            ((1 << 15) << 32)
#define PXProcessorFeaturexTPRUpdateControl                 ((1 << 14) << 32)
#define PXProcessorFeatureCMPXCHG16B                        ((1 << 13) << 32)
#define PXProcessorFeatureFusedMultiplyAdd                  ((1 << 12) << 32)
#define PXProcessorFeatureSDBG                              ((1 << 11) << 32)
#define PXProcessorFeatureL1ContextID                       ((1 << 10) << 32)
#define PXProcessorFeatureSSSE3Extensions                   ((1 << 9) << 32)
#define PXProcessorFeatureThermalMonitor2                   ((1 << 8) << 32)
#define PXProcessorFeatureEnhancedIntelSpeedStepTechnology  ((1 << 7) << 32)
#define PXProcessorFeatureSaferModeExtensions               ((1 << 6) << 32)
#define PXProcessorFeatureVirtualMachineExtensions          ((1 << 5) << 32)
#define PXProcessorFeatureCPLQualifiedDebugStore            ((1 << 4) << 32)
#define PXProcessorFeatureMONITORAndMWAIT                   ((1 << 3) << 32)
#define PXProcessorFeature64BitDSArea                       ((1 << 2) << 32)
#define PXProcessorFeatureCarrylessMultiplication           ((1 << 1) << 32)
#define PXProcessorFeatureSSE3                              ((1 << 0) << 32)


#define PXProcessorFeature3DNowB                            (1 << 31)
#define PXProcessorFeature3DNowA                            (1 << 30)
#define PXProcessorFeatureThermMonitor                      (1 << 29)
#define PXProcessorFeatureMultithreading                    (1 << 28)
#define PXProcessorFeatureSelfSnoop                         (1 << 27)
#define PXProcessorFeatureSSE2Extensions                    (1 << 26)
#define PXProcessorFeatureSSEExtensions                     (1 << 25)
#define PXProcessorFeatureFXSAVEFXRSTOR                     (1 << 24)
#define PXProcessorFeatureMMXTechnology                     (1 << 23)
#define PXProcessorFeatureThermalMonitorandClockCtrl        (1 << 22)
#define PXProcessorFeatureDebugStore                        (1 << 21)
//#define PXProcessorFeaturexxxxxx                          (1 << 20)
#define PXProcessorFeatureCLFLUSHinstruction                (1 << 19)
#define PXProcessorFeatureProcessorSerialNumber             (1 << 18)
#define PXProcessorFeaturePageSizeExtension                 (1 << 17)
#define PXProcessorFeaturePageAttributeTable                (1 << 16)
#define PXProcessorFeatureConditionalMoveCompareInstruction (1 << 15)
#define PXProcessorFeatureMachineCheckArchitecture          (1 << 14)
#define PXProcessorFeaturePTEGlobalBit                      (1 << 13)
#define PXProcessorFeatureMemoryTypeRangeRegisters          (1 << 12)
#define PXProcessorFeatureSYSENTERandSYSEXIT                (1 << 11)
//#define PXProcessorFeatureXXXXXX                          (1 << 10)
#define PXProcessorFeatureAPIConChip                        (1 << 9)
#define PXProcessorFeatureCMPXCHG8BInst                     (1 << 8)
#define PXProcessorFeatureMachineCheckException             (1 << 7)
#define PXProcessorFeaturePhysicalAddressExtensions         (1 << 6)
#define PXProcessorFeatureRDMSRandWRMSRSupport              (1 << 5)
#define PXProcessorFeatureTimeStampCounter                  (1 << 4)
#define PXProcessorFeaturePageSizeExtensions                (1 << 3)
#define PXProcessorFeatureDebuggingExtensions               (1 << 2)
#define PXProcessorFeatureVirtual8086ModeEnhancement        (1 << 1)
#define PXProcessorFeatureFPUx87FPUonChip                   (1 << 0)


typedef struct PXProcessor_
{
    char BrandName[64];  // string includes manufacturer, model and clockspeed
    char IdentityString[13];

    PXProcessorBrand BrandID;
    PXProcessorModelName ModelNameID;

    PXInt8U NumberOfProcessors;
    PXInt8U BrandNameSize;

    PXBool IsInvariantTSCAvailable;

    PXInt8U L2AssociativityField;
    PXInt8U CacheSizein1Kunits;
    PXInt16U CacheLineSizeInBytes;

    PXInt64U FeatureList;


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
    PXInt8U BrandIndex;
    PXInt8U CLflushCacheLineSize;
    PXInt8U logicalProcessors;
    PXInt8U initialApicID;
}
PXProcessor;


PXPrivate void PXAPI PXProcessorModelNameGet(const PXProcessorModelName processorModelName, char* const name);
PXPrivate PXProcessorBrand PXAPI PXProcessorBrandDetect(const char* name);
PXPrivate PXProcessorModelName PXAPI PXProcessorModelNameDetect(const PXProcessorBrand processorBrand, const unsigned char famliy, const unsigned char model);

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
PXPublic void PXAPI PXProcessorRandomNumber();

PXPublic void PXAPI PXProcessorSwapByteOrderI16U(PXInt16U* const value);
PXPublic void PXAPI PXProcessorSwapByteOrderI32U(PXInt32U* const value);
PXPublic void PXAPI PXProcessorSwapByteOrderI64U(PXInt64U* const value);

#endif