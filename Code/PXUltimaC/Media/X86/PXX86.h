

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


struct enum PXX86OPCode
{
  PXX86OPCodeInvalid,

  
}
PXX86OPCode;











