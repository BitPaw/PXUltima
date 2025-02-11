#ifndef PXX86INCLUDE
#define PXX86INCLUDE

#include <Media/PXResource.h>
#include <Container/List/PXList.h>

//---------------------------------------------------------
// EFLAG - Register
//---------------------------------------------------------
#define PXX86EFLAGS31 			               	 (1<<31) // ---UNDEFINED---
#define PXX86EFLAGS30 			               	 (1<<30) // ---UNDEFINED---
#define PXX86EFLAGS29 				               (1<<29) // ---UNDEFINED---
#define PXX86EFLAGS28 				               (1<<28) // ---UNDEFINED---
#define PXX86EFLAGS27 				               (1<<27) // ---UNDEFINED---
#define PXX86EFLAGS26 				               (1<<26) // ---UNDEFINED---
#define PXX86EFLAGS25 				               (1<<25) // ---UNDEFINED---
#define PXX86EFLAGS24 				               (1<<24) // ---UNDEFINED---
#define PXX86EFLAGS23 				               (1<<23) // ---UNDEFINED---
#define PXX86EFLAGS22 				               (1<<22) // ---UNDEFINED---
#define PXX86EFLAGSIDFlag 			             (1<<21) // ID Flag (ID)
#define PXX86EFLAGSVirtualInterruptPending   (1<<20) // Virtual Interrupt Pending (VIP)
#define PXX86EFLAGSVirtualInterruptFlag		   (1<<19) // Virtual Interrupt Flag (VIF)
#define PXX86EFLAGSAccessControl 		         (1<<18) // Alignment Check / Access Control (AC)
#define PXX86EFLAGSVirtualMode 			         (1<<17) // Virtual-8086 Mode (VM)
#define PXX86EFLAGSResumeFlag 			         (1<<16) // Resume Flag (RF)
#define PXX86EFLAGS15 				               (1<<15) // ---UNDEFINED---
#define PXX86EFLAGSNestedTask 			         (1<<14) // Nested Task (NT)
#define PXX86EFLAGSIOPrivilegeLevel 		     (1<<13) | (1<<12) // I/O Privilege Level (IOPL)
#define PXX86EFLAGSOverflowFlag 		         (1<<11) // Overflow Flag (OF)
#define PXX86EFLAGSDirectionFlag 		         (1<<10) // Direction Flag (DF)
#define PXX86EFLAGSInterruptEnableFlag  	   (1<<9) // Interrupt Enable Flag (IF)
#define PXX86EFLAGSTrapFlag 			           (1<<8) // Trap Flag (TF)
#define PXX86EFLAGSTrapFlag 			           (1<<7) // Sign Flag (SF)
#define PXX86EFLAGSZeroFlag 			           (1<<6) // Zero Flag (ZF)
#define PXX86EFLAGS05 				               (1<<5) // ---UNDEFINED---
#define PXX86EFLAGSAuxiliaryCarryFlag 		   (1<<4) // Auxiliary Carry Flag (AF)
#define PXX86EFLAGS03 				               (1<<3) // ---UNDEFINED---
#define PXX86EFLAGSParityFlag 			         (1<<2) // Parity Flag (PF)
#define PXX86EFLAGS01 				               (1<<1) // ---UNDEFINED---
#define PXX86EFLAGSCarryFlag 			           (1<<0) // Carry Flag (CF)
//---------------------------------------------------------


typedef enum PXX86OPCode
{
  PXX86OPCodeInvalid,

  
}
PXX86OPCode;

typedef struct PXAssemblyInstruction
{
    char* Name;
    char* Description;
}
PXAssemblyInstruction;


typedef struct PXX86Iterator_ PXX86Iterator;

typedef void (PXAPI* PXX86InstructionFunction)(PXX86Iterator* const pxX86Iterator);

typedef struct PXX86Instruction
{
    PXX86InstructionFunction HandlerFunction;
    char* Name;
    char* Description;
    PXInt32U Type;
}
PXX86Instruction;

typedef struct PXX86Iterator_
{
    PXX86Instruction* InstructionCurrent;
    PXFile* Data;
    void* VirtualAdress;

    PXInt8U OperationCode; // opcode

    PXList Stack;
}
PXX86Iterator;

typedef struct PXX86ModRM
{
    PXInt8U Mod;
    PXInt8U RegisterIndex;
    PXInt8U RM;

    char* Name;
}
PXX86ModRM;



PXPrivate void PXAPI PXX86InstructionSUBImidate(PXX86Iterator* const pxX86Iterator);
PXPrivate void PXAPI PXX86InstructionInvoke(PXX86Iterator* const pxX86Iterator);
PXPrivate void PXAPI PXX86InstructionFunctionMODRMRead(PXX86Iterator* const pxX86Iterator, PXX86ModRM* const pxX86ModRM);
PXPrivate void PXAPI PXX86InstructionFunctionMOVx3(PXX86Iterator* const pxX86Iterator);
PXPrivate void PXAPI PXX86InstructionFunctionREXMOV(PXX86Iterator* const pxX86Iterator);

PXPublic PXActionResult PXAPI PXX86InstructionNext(PXX86Iterator* const pxX86Iterator);

PXPublic PXActionResult PXAPI PXX86InstructionDisassemble(PXX86Iterator* const pxX86Iterator);


typedef struct PXSectionTable_ PXSectionTable;

PXPublic PXActionResult PXAPI PXX86InstructionWalk(PXFile* const pxFile, PXSectionTable* const pxSectionTable);

#endif