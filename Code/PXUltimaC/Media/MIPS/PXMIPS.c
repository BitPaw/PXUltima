#include "PXMIPS.h"

#include <OS/Console/PXConsole.h>

void PXAPI PXMIPSInitializize(PXMIPSProcessor* const pxMIPSProcessor, PXCodeSegment* const pxCodeSegment)
{
	
}

const char* PXAPI PXMIPSInstructionTypeToStringShort(const PXMIPSInstructionType pxMIPSInstruction)
{
	switch(pxMIPSInstruction)
	{
		case PXMIPSInstructionTypeJump: return "J";
		case PXMIPSInstructionTypeJumpAndLink: return "JAL";
		case PXMIPSInstructionTypeBranchOnEqual: return "BEQ";
		case PXMIPSInstructionTypeBranchOnNotEqual: return "BNE";
		case PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero: return "BLEZ";
		case PXMIPSInstructionTypeBranchOnGreaterThanZero: return "BGTZ";
		case PXMIPSInstructionTypeAddImmediate: return "ADDI";
		case PXMIPSInstructionTypeAddImmediateUnsigned: return "ADDIU";
		case PXMIPSInstructionTypeSetOnLessThanImmediate: return "SLTI";
		case PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned: return "SLTIU";
		case PXMIPSInstructionTypeAndImmediate: return "ANDI";
		case PXMIPSInstructionTypeORImmediate: return "ORI";
		case PXMIPSInstructionTypeExclusiveOrImmediate: return "XORI";
		case PXMIPSInstructionTypeLoadUpperImmediate: return "LUI";


		case PXMIPSInstructionTypeStoreWord: return "SW";
		case PXMIPSInstructionTypeStoreWordLeft: return "SWL";
		case PXMIPSInstructionTypeBranchOnNotEqualLikely: return "BNEL";
		case PXMIPSInstructionTypeJumpRegister: return "JR";

		default:
			return PXNull;
	}
}

const char* PXAPI PXMIPSInstructionTypeToStringLong(const PXMIPSInstructionType pxMIPSInstruction)
{
	switch(pxMIPSInstruction)
	{
		case PXMIPSInstructionTypeJump: return "Jump";
		case PXMIPSInstructionTypeJumpAndLink: return "Jump And Link";
		case PXMIPSInstructionTypeBranchOnEqual: return "Branch On Equal";
		case PXMIPSInstructionTypeBranchOnNotEqual: return "Branch On Not Equal";
		case PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero: return "Branch On Less Than Or Equal To Zero";
		case PXMIPSInstructionTypeBranchOnGreaterThanZero: return "Branch On Greater Than Zero";
		case PXMIPSInstructionTypeAddImmediate: return "Add Immediate";
		case PXMIPSInstructionTypeAddImmediateUnsigned: return "Add Immediate Unsigned";
		case PXMIPSInstructionTypeSetOnLessThanImmediate: return "Set On Less Than Immediate";
		case PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned: return "Set On Less Than Immediate Unsigned";
		case PXMIPSInstructionTypeAndImmediate: return "And Immediate";
		case PXMIPSInstructionTypeORImmediate: return "OR Immediate";
		case PXMIPSInstructionTypeExclusiveOrImmediate: return "Exclusive Or Immediate";
		case PXMIPSInstructionTypeLoadUpperImmediate: return "Load Upper Immediate";


		case PXMIPSInstructionTypeStoreWord: return "Store Word";
		case PXMIPSInstructionTypeStoreWordLeft: return "Store Word Left";	
		case PXMIPSInstructionTypeBranchOnNotEqualLikely: return "Branch On Not Equal Likely";
		case PXMIPSInstructionTypeJumpRegister: return "Jump Register";

		default:
			return PXNull;
	}
}


void PXMIPSInstructionPrint(const PXMIPSTInstruction* const pxMIPSTInstruction)
{
#if PXLogEnable
	const char* instuctionNameShort = PXMIPSInstructionTypeToStringShort(pxMIPSTInstruction->Type);
	const char* instuctionNameLong = PXMIPSInstructionTypeToStringLong(pxMIPSTInstruction->Type);

	PXLogPrint
	(
		PXLoggingAllocation,
		"MIPS",
		"Instruction",
		"%8.8X   %2.2X %2.2X %2.2X %2.2X   RS:%2.2X RT:%2.2X IMM:%4.4X   %2.2X:%-7s %s",
		(int)pxMIPSTInstruction->AdressVirtual,	
		pxMIPSTInstruction->Adress[0],
		pxMIPSTInstruction->Adress[1],
		pxMIPSTInstruction->Adress[2],
		pxMIPSTInstruction->Adress[3],
		pxMIPSTInstruction->RS,
		pxMIPSTInstruction->RT,
		pxMIPSTInstruction->Immediate,
		pxMIPSTInstruction->Type,
		instuctionNameShort,
		instuctionNameLong

	);
#endif
}

PXActionResult PXAPI PXMIPSTranslate(PXMIPSProcessor* const pxMIPSProcessor, const PXByte* const data, const PXSize length)
{
	//PXClear(PXMIPSProcessor, pxMIPSProcessor);
	DWORD protectIDOld = 0;
	DWORD protectIDNew = PAGE_READONLY;
	const PXBool succ = VirtualProtect(data, 0x0007dac0, protectIDNew, &protectIDOld);

	void* programmCounter = data;

	PXBool jumped = 0;

	pxMIPSProcessor->ProgramCounter = 0;


#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"MIPS",
		"Execute",
		"--- Start ---"
	);
#endif


#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"MIPS",
		"Execute",
		"Begin at 0x%p, Size %i",
		data,
		length
	);
#endif

	for(;;)
	{
		PXMIPSTInstruction pxMIPSTInstruction;
		pxMIPSTInstruction.Adress = (PXInt8U*)programmCounter + pxMIPSProcessor->ProgramCounter;
		pxMIPSTInstruction.AdressVirtual = (PXSize)pxMIPSProcessor->ROMOffset + pxMIPSProcessor->ProgramCounter;

		// instruction is big endian, to use our bit extraction, we need to swap it

		PXInt32U instructionVal = PXInt32Make(pxMIPSTInstruction.Adress[3], pxMIPSTInstruction.Adress[2], pxMIPSTInstruction.Adress[1], pxMIPSTInstruction.Adress[0]);

		if(instructionVal == 0)
		{
			break;
		}


		pxMIPSTInstruction.Type = (instructionVal & 0b11111100000000000000000000000000) >> 26;
		pxMIPSTInstruction.RS = (instructionVal & 0b00000011111000000000000000000000) >> 19;
		pxMIPSTInstruction.RT = (instructionVal & 0b00000000000111110000000000000000) >> 16;
		pxMIPSTInstruction.Immediate = (instructionVal & 0b00000000000000001111111111111111) >> 0;


		// Check for special upcodes
		// MIPS has 'special' opcodes, the "Immediate" value is defined as the opcode
		// As only 6 bits are used, I will use the two unused bits to make those unique.
		// With this i can still use an 8-bit enum 
		switch(pxMIPSTInstruction.Type)
		{
			case PXMIPSInstructionTypeOPCodeSpecial:
			{		
				pxMIPSTInstruction.Type = (pxMIPSTInstruction.Immediate & 0b111111) | PXMIPSOPCodeSpecial;
				break;
			}
			case PXMIPSInstructionTypeOPCodeREGIMM:
			{
				pxMIPSTInstruction.Type = pxMIPSTInstruction.RT | PXMIPSOPCodeREGIMM;
				break;
			}
			default:
				// Do nothing
				break;
		}

#if PXLogEnable
		if(jumped)
		{
			PXMIPSInstructionPrint(&pxMIPSTInstruction);
		}
#endif

		switch(pxMIPSTInstruction.Type)
		{
			case PXMIPSInstructionTypeJumpRegister:
			{
				PXSize vadress = pxMIPSProcessor->Register[pxMIPSTInstruction.RS] - 0x10000;
				PXSize offset = vadress - (PXSize)pxMIPSProcessor->ROMOffset;

#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Jump Register",
					"Jump to %8.8X (offset:%i)",
					vadress,
					offset
				);
#endif
				pxMIPSProcessor->ProgramCounter = offset;

				jumped = 1;

				continue; // Skip the +4 offset
			}
			case PXMIPSInstructionTypeJumpAndLink:
			{
				/*
				The 26-bit target is shifted left two bits and combined with the high-order four bits
				of the address of the delay slot to calculate the address. The program
				unconditionally jumps to this calculated address with a delay of one instruction.
				The address of the instruction after the delay slot is placed in the link register, r31.
				*/
				const PXSize vAdress = pxMIPSProcessor->ProgramCounter + (PXSize)pxMIPSProcessor->ROMOffset;

				PXSize functionAdress = instructionVal & 0b00000011111111111111111111111111;
				functionAdress <<= 2; // Shift by 2
				functionAdress |= vAdress & 0b11110000000000000000000000000000; // Highest 4 bits

				PXSize offset = functionAdress - (PXSize)pxMIPSProcessor->ROMOffset;


				pxMIPSProcessor->Register[PXMIPSProcessorLinkAddress] = vAdress + 4; // Next instruction


#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Jump And Link",
					"Jump to %8.8X (offset:%i)",
					functionAdress,
					offset
				);
#endif


				// Jump
				pxMIPSProcessor->ProgramCounter = offset;

				continue; // Skip the +4 offset
			}
			case PXMIPSInstructionTypeLoadUpperImmediate:
			{
				PXBool rsValid = 0 == pxMIPSTInstruction.RS;

				/*
				The 16-bit immediate is shifted left 16 bits and combined to 16 bits of zeros. The
				result is placed into general purpose register rt.
				In 64-bit mode, the loaded word	is sign-extended to 64 bits.
				*/

				PXInt64U value = (PXInt16U)pxMIPSTInstruction.Immediate;

				value <<= 16;


#if PXLogEnable
				PXSize valueBefore = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];
#endif

				pxMIPSProcessor->Register[pxMIPSTInstruction.RT] |= value;

#if PXLogEnable && 1
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Load",
					"\n"
					"%16.8X = %16.8X + (%8.4X << 16) Before:%8.8X\n"
					"%16i = %16i + (%8i << 16) Before:%4i",
					pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
					pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
					pxMIPSTInstruction.Immediate,
					valueBefore,
					pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
					pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
					pxMIPSTInstruction.Immediate,
					valueBefore
				);
#endif

				break;
			}
			case PXMIPSInstructionTypeStoreWord:
			{
				/*
				The 16-bit offset is sign-extended and added to the contents of general purpose
				register base to form a virtual address. The contents of general purpose register rt
				are stored in the memory location specified by the address. If either of the low-
				order two bits of the address are not zero, an address error exception occurs.
				*/
				const PXInt64U base = pxMIPSProcessor->Register[pxMIPSTInstruction.RS];
				const PXInt64S offset = (PXInt16S)pxMIPSTInstruction.Immediate;
				const PXSize virtualAdress = base + offset;
			
				PXSize realAdress = virtualAdress - (PXSize)pxMIPSProcessor->ROMOffset;

#if PXLogEnable && 1
				if(jumped)
				{
					PXLogPrint
					(
						PXLoggingInfo,
						"MIPS",
						"StoreWord",
						"Value:%8.8X -> %8.8X - Base:%8.8X + Offset:%i",
						pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
						virtualAdress,
						base,
						offset
					);
				}
#endif

				pxMIPSProcessor->Register[pxMIPSTInstruction.RS] += 0x02;


			//	pxMIPSProcessor->Stack[realAdress] = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];

				void* adress = (PXInt8U*)programmCounter + realAdress;

				*((PXInt32U*)adress) = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];

				break;
			}
			case PXMIPSInstructionTypeBranchOnNotEqual:
			{
				/*
				A branch address is calculated from the sum of the address of the instruction in the
				delay slot and the 16-bit offset, shifted two bits left and sign-extended. The
				contents of general purpose register rs and the contents of general purpose register
				rt are compared. If the two registers are not equal, then the program branches to
				the branch address, with a delay of one instruction.
				*/

				PXInt64S offset = ((PXInt16S)pxMIPSTInstruction.Immediate) << 2;
				PXBool notEqualRSRT = pxMIPSProcessor->Register[pxMIPSTInstruction.RS] != pxMIPSProcessor->Register[pxMIPSTInstruction.RT];

				if(notEqualRSRT)
				{	
					// delay of oone instruction?

#if PXLogEnable && 1
					if(jumped)
					{
						PXLogPrint
						(
							PXLoggingInfo,
							"MIPS",
							"Jump NEQ",
							"%8.8X vs %4.4X - Jump to: %i",
							pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
							pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
							offset
						);
					}
#endif

					pxMIPSProcessor->ProgramCounter += offset;

					//continue;
				}
				else
				{
#if PXLogEnable
					PXLogPrint
					(
						PXLoggingInfo,
						"MIPS",
						"Jump NEQ",
						"No jump. Value equal %8.8X",
						pxMIPSProcessor->Register[pxMIPSTInstruction.RS]
					);
#endif
				}

				break;
			}
			case PXMIPSInstructionTypeAddImmediate:
			{
				/*
				The 16-bit immediate is sign-extended and added to the contents of general
				purpose register rs to store the result in general purpose register rt. In 64-bit
				mode, the operand must be sign-extended, 32-bit values.
				An integer overflow exception occurs if carries out of bits 30 and 31 differ (2’s
				complement overflow). The contents of destination register rt is not modified
				when an integer overflow exception occurs
				*/

				const PXInt64S value = (PXInt16S)pxMIPSTInstruction.Immediate;
			
					 
#if PXLogEnable
				const PXInt64S rtBefore = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];
				const PXInt64S rsBefore = pxMIPSProcessor->Register[pxMIPSTInstruction.RS];
#endif

				const PXInt64S resukt =
					(PXInt64U)pxMIPSProcessor->Register[pxMIPSTInstruction.RS] +
					(PXInt16S)pxMIPSTInstruction.Immediate;

				pxMIPSProcessor->Register[pxMIPSTInstruction.RS] = resukt;
				pxMIPSProcessor->Register[pxMIPSTInstruction.RT] = resukt;

#if PXLogEnable && 1
				if(jumped)
				{
					PXLogPrint
					(
						PXLoggingInfo,
						"MIPS",
						"Add Signed",
						"\n"
						"%16.8X = %16.8X + %16.8X Before:%16.8X\n"
						"%16i = %16i + %16i Before:%16i",
						resukt,
						rsBefore,
						value,
						rtBefore,
						resukt,
						rsBefore,
						value,
						rtBefore
					);
				}
#endif

				break;
			}
			case PXMIPSInstructionTypeAddImmediateUnsigned:
			{
				/*
				The 16-bit immediate is sign-extended and added to the contents of general
				purpose register rs to store the result in general purpose register rt. No integer
				overflow exception occurs under any circumstance. In 64-bit mode, the operand
				must be sign-extended, 32-bit values.
				The only difference between this instruction and the ADDI instruction is that
				ADDIU instruction never causes an integer overflow exception.
				*/

				const PXInt64U value = (PXInt16U)pxMIPSTInstruction.Immediate;

#if PXLogEnable
				PXSize valueBefore = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];
#endif

				PXSize result = pxMIPSProcessor->Register[pxMIPSTInstruction.RT] + pxMIPSProcessor->Register[pxMIPSTInstruction.RS] + value; // Calc and store into rs
				//*target += pxMIPSProcessor->Register[pxMIPSTInstruction.RS]; // copy rs to rt


				pxMIPSProcessor->Register[pxMIPSTInstruction.RT] = result;
				pxMIPSProcessor->Register[pxMIPSTInstruction.RS] = result;

#if PXLogEnable && 1
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Add Unsigned",
					"\n"
					"%16.8X = %16.8X + %16.8X Before:%16.8X\n"
					"%16i = %16i + %16i Before:%16i",
					result,
					(PXInt64S)pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
					value,
					valueBefore,
					result,
					(PXInt64S)pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
					value,
					valueBefore
				);
#endif

				break;
			}
			case PXMIPSInstructionTypeORImmediate:
			{
				/*
				A logical OR operation applied between 16-bit zero-extended immediate and the
				contents of general purpose register rs is executed in bit units. The result is stored
				in general purpose register rt
				*/

#if PXLogEnable
				const PXSize rtBefore = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];
				const PXSize rsBefore = pxMIPSProcessor->Register[pxMIPSTInstruction.RS];
#endif

				pxMIPSProcessor->Register[pxMIPSTInstruction.RS] = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];
				pxMIPSProcessor->Register[pxMIPSTInstruction.RS] |= (PXInt64U)pxMIPSTInstruction.Immediate;
				pxMIPSProcessor->Register[pxMIPSTInstruction.RT] = pxMIPSProcessor->Register[pxMIPSTInstruction.RS];

#if PXLogEnable && 1
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"OR",
					"\n"
					"%16.8X = %16.8X | %16.8X Before:%16.8X\n"
					"%16i = %16i | %16i Before:%16i",
					pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
					rsBefore,
					(PXInt64U)pxMIPSTInstruction.Immediate,
					rtBefore,
					pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
					rsBefore,
					(PXInt64U)pxMIPSTInstruction.Immediate,
					rtBefore
				);
#endif
				
				break;
			}

			default:
			{
#if PXLogEnable
				PXLogPrint
				(
					PXLoggingFailure,
					"MIPS",
					"Instruction",
					"Illegal or not implemented!"
				);
#endif
				DebugBreak();
			}
		}

		// Next command
		// MIPS always has 4-Byte commands
		pxMIPSProcessor->ProgramCounter += 4;
	}

#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"MIPS",
		"Execute",
		"--- Done ---"
	);
#endif


	return PXActionSuccessful;
}