#include "PXMIPS.h"

#include <OS/Console/PXConsole.h>

void PXAPI PXMIPSInitializize(PXMIPSProcessor* const pxMIPSProcessor, PXCodeSegment* const pxCodeSegment)
{
	
}

const char* PXAPI PXMIPSInstructionTypeToString(const PXMIPSInstructionType pxMIPSInstruction)
{
	switch(pxMIPSInstruction)
	{
		case PXMIPSInstructionTypeLoadUpperImmediate: return "LUI";
		case PXMIPSInstructionTypeAddImmediate: return "ADDI";
		case PXMIPSInstructionTypeAddImmediateUnsigned: return "ADDIU";
		case PXMIPSInstructionTypeORImmediate: return "ORI";
		case PXMIPSInstructionTypeStoreWord: return "SW";
		case PXMIPSInstructionTypeStoreWordLeft: return "SWL";
		case PXMIPSInstructionTypeBranchOnNotEqual: return "BNE";
		case PXMIPSInstructionTypeBranchOnNotEqualLikely: return "BNEL";
		case PXMIPSInstructionTypeJumpRegister: return "JR";

		default:
			return PXNull;
	}
}




PXActionResult PXAPI PXMIPSTranslate(PXMIPSProcessor* const pxMIPSProcessor, const PXByte* const data, const PXSize length)
{
	//PXClear(PXMIPSProcessor, pxMIPSProcessor);

	void* programmCounter = data;

	for(size_t i = 0; i < length; i += 4)
	{
		PXMIPSTInstruction pxMIPSTInstruction;
		pxMIPSTInstruction.Adress = (PXInt8U*)programmCounter + i;
		pxMIPSTInstruction.AdressVirtual = (PXSize)pxMIPSProcessor->ROMOffset + i;

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


#if PXLogEnable
		char* instuctionName = PXMIPSInstructionTypeToString(pxMIPSTInstruction.Type);

		PXLogPrint
		(
			PXLoggingAllocation,
			"MIPS",
			"Instruction",
			"%8.8X - %2.2X %2.2X %2.2X %2.2X - %2.2X:%-7s RS:%2.2X RT:%2.2X IMM:%4.4X",
			(int)pxMIPSTInstruction.AdressVirtual,
			pxMIPSTInstruction.Adress[0],
			pxMIPSTInstruction.Adress[1],
			pxMIPSTInstruction.Adress[2],
			pxMIPSTInstruction.Adress[3],
			pxMIPSTInstruction.Type,
			instuctionName,
			pxMIPSTInstruction.RS,
			pxMIPSTInstruction.RT,
			pxMIPSTInstruction.Immediate
		);
#endif



		switch(pxMIPSTInstruction.Type)
		{
			case PXMIPSInstructionTypeSpecial:
			{	
				// MIPS has 'special' opcodes, these are all zero and the "Immediate" value
				PXInt8U subInstruction = pxMIPSTInstruction.Immediate & 0b111111;

				switch(subInstruction)
				{
					case 0b001000:
					{
						PXSize vadress = pxMIPSProcessor->Register[pxMIPSTInstruction.RS];
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
						i = offset - 4; // -4 because of allignment problem after the for loop
					}

					default:
						break;
				}

				break;
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

#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Add",
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
				PXSize stackOffset = pxMIPSTInstruction.RS + pxMIPSTInstruction.Immediate; // base + offset				

				pxMIPSProcessor->Stack[stackOffset] = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];

#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"StoreWord",
					"%8.8X at Base:%8.8X + Offset:%i",
					pxMIPSTInstruction.RT,
					pxMIPSTInstruction.RS,
					pxMIPSTInstruction.Immediate
				);
#endif

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

#if PXLogEnable
					PXLogPrint
					(
						PXLoggingInfo,
						"MIPS",
						"Jump NEQ",
						"%4.4X vs %4.4X - Jump to: %i",
						pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
						pxMIPSProcessor->Register[pxMIPSTInstruction.RT],
						offset
					);
#endif

					i += offset;
				}
				else
				{
#if PXLogEnable
					PXLogPrint
					(
						PXLoggingInfo,
						"MIPS",
						"Jump Register",
						"No jump. Value equal %4.4X",
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
				PXInt64S* target = (PXInt16S*)&pxMIPSProcessor->Register[pxMIPSTInstruction.RT];
					 
#if PXLogEnable
				PXSize valueBefore = *target;
#endif

				*target = (PXInt64S)pxMIPSProcessor->Register[pxMIPSTInstruction.RS] + value;

#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Add Signed",
					"\n"
					"%8.8X = %8.8X + %8.8X Before:%8.8X\n"
					"%8i = %8i + %8i Before:%8i",
					*target,
					(PXInt64S)pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
					value,
					valueBefore,
					*target,
					(PXInt64S)pxMIPSProcessor->Register[pxMIPSTInstruction.RS],
					value,
					valueBefore
				);
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

#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Add Unsigned",
					"\n"
					"%8.8X = %8.8X + %8.8X Before:%8.8X\n"
					"%8i = %8i + %8i Before:%8i",
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
				PXSize valueBefore = 0;
#endif

				pxMIPSProcessor->Register[pxMIPSTInstruction.RT] += pxMIPSProcessor->Register[pxMIPSTInstruction.RS] ^ pxMIPSTInstruction.Immediate;
				
#if PXLogEnable
				PXLogPrint
				(
					PXLoggingInfo,
					"MIPS",
					"Add",
					"\n"
					"%8.8X = %8.8X ^ %8.8X Before:%8.8X\n"
					"%8i = %8i ^ %8i Before:%8i",
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

			default:
				break;
		}



	}

	return PXActionSuccessful;
}