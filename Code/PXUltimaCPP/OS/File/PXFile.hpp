#ifndef PXCPPFileINCLUDE
#define PXCPPFileINCLUDE

#include <OS/File/PXFile.h>
#include <Media/PXText.hpp>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class File : public PXFile
	{
		public:
		File();
		~File();

		static bool DoesExist(const Text& filePath);
		static PX::ActionResult Remove(const Text& const filePath);
		static PX::ActionResult Rename(const Text& const oldName, const Text& const newName);
		static PX::ActionResult Copy(const Text& const sourceFilePath, const PXText& const destinationFilePath, const bool overrideIfExists);
		
		PX::ActionResult Name(Text& const fileName);

		//---<Set>-------------------------------------------------------------
		void BufferAllocate(const PXSize dataSize);
		void BufferExternal(void* const data, const PXSize dataSize);
		//---------------------------------------------------------------------

		//---<Open>------------------------------------------------------------
		PX::ActionResult OpenFromPath(const PXFileOpenFromPathInfo& const  OpenFromPathInfo);
		//---------------------------------------------------------------------

		//---<Close>-----------------------------------------------------------
		PX::ActionResult Close();
		//---------------------------------------------------------------------

		//---<Mapping>---------------------------------------------------------
		PX::ActionResult MapToMemory(const PXSize size, const PXMemoryAccessMode protectionMode);
		PX::ActionResult UnmapFromMemory();
		//---------------------------------------------------------------------

		//---<Parsing Utility>-----------------------------------------------------
		PXSize  RemainingSize();
		PXSize  RemainingSizeRelativeFromAddress(const void* const adress);
		bool  IsAtEnd();

		void* CursorPosition();
		void  CursorMoveTo(const PXSize position);
		void  CursorToBeginning();
		PXSize  CursorAdvance(const PXSize steps);
		void  CursorRewind(const PXSize steps);
		void  CursorToEnd();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		PXSize  SkipEndOfLineCharacters();
		PXSize  SkipEmptySpace();
		PXSize  ReadNextLineInto(void* exportBuffer, const PXSize exportBufferSize);


		// Skips current block thats seperated with spaces.
		// Example : '|' is the cursor.
		// Before : |AAAA  BBBB CCC
		// After :  AAAA |BBBB CCC
		PXSize  SkipBlock();
		PXSize  SkipToNextBlock();
		PXSize  SkipLine();

		/*

		PXSize  DataCopy(const pxInputStream, * const pxOutputStream, const PXSize length);

		PXSize  ReadTextIU8(PXInt8U* const number);
		PXSize  ReadTextI(int* const number);

		PXSize  ReadIXXE(void* const valueAdress, const PXSize valueSize, const PXEndian pxEndian);
		PXSize  ReadIXXVE(void* const valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian);

		PXSize  ReadI8S(* const, PXInt8S* const value);
		PXSize  ReadI8SV(* const, PXInt8S* const valueList, const PXSize valueListSize);
		PXSize  ReadI8U(* const, PXInt8U* const value);
		PXSize  ReadI8UV(* const, PXInt8U* const valueList, const PXSize valueListSize);

		PXSize  ReadI16S(* const, PXInt16S* const value);
		PXSize  ReadI16SV(* const, PXInt16S* const valueList, const PXSize valueListSize);
		PXSize  ReadI16SE(* const, PXInt16S* const value, const PXEndian pxEndian);
		PXSize  ReadI16SVE(* const, PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
		PXSize  ReadI16U(* const, PXInt16U* const value);
		PXSize  ReadI16UV(* const, PXInt16U* const valueList, const PXSize valueListSize);
		PXSize  ReadI16UE(* const, PXInt16U* const value, const PXEndian pxEndian);
		PXSize  ReadI16UVE(* const, PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

		PXSize  ReadI32S(* const, PXInt32S* const value);
		PXSize  ReadI32SV(* const, PXInt32S* const valueList, const PXSize valueListSize);
		PXSize  ReadI32SE(* const, PXInt32S* const value, const PXEndian pxEndian);
		PXSize  ReadI32SVE(* const, PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
		PXSize  ReadI32U(* const, PXInt32U* const value);
		PXSize  ReadI32UV(* const, PXInt32U* const valueList, const PXSize valueListSize);
		PXSize  ReadI32UE(* const, PXInt32U* const value, const PXEndian pxEndian);
		PXSize  ReadI32UVE(* const, PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

		PXSize  ReadI64S(* const, PXInt64S* const value);
		PXSize  ReadI64SV(* const, PXInt64S* const valueList, const PXSize valueListSize);
		PXSize  ReadI64SE(* const, PXInt64S* const value, const PXEndian pxEndian);
		PXSize  ReadI64VE(* const, PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
		PXSize  ReadI64U(* const, PXInt64U* const value);
		PXSize  ReadI64UV(* const, PXInt64U* const valueList, const PXSize valueListSize);
		PXSize  ReadI64UE(* const, PXInt64U* const value, const PXEndian pxEndian);
		PXSize  ReadI64UVE(* const, PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

		PXSize  ReadF(* const, float* const value);
		PXSize  ReadFV(* const, float* const valueList, const PXSize valueListSize);

		PXSize  ReadD(* const, double* const value);
		PXSize  ReadDV(* const, double* const valueList, const PXSize valueListSize);

		PXSize  ReadMultible(* const, DataElementType* const  ElementList, const PXSize  ElementListSize);

		PXSize  ReadB(* const, void* const value, const PXSize length);

		// read Text
		PXSize  ReadTextA(* const, PXTextASCII value, const PXSize length);
		PXSize  ReadTextW(* const, PXTextUNICODE value, const PXSize length);
		PXSize  ReadTextU(* const, PXTextUNICODE value, const PXSize length);

		void  ReadUntil(* const, void* value, const PXSize length, const char character);



		// Additional check how many bytes are read.
		// Slower version of Read(), this function can't fail.
		//PXSize ReadSafe(Byte__* value, const PXSize length);

		PXBool  ReadAndCompareI64U(* const, const PXInt64U value);


		PXBool  ReadAndCompare(* const, const void* value, const PXSize length);
		PXBool  ReadAndCompareV(* const, const void** value, const PXSize* valueElementSizeList, const PXSize valueLength);
		//unsigned char  ReadAndCompareC( *  , const char value);
		//unsigned char  ReadAndCompareIU( *  , const unsigned int value);

		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		PXSize  WriteI8S(* const, const PXInt8S value);
		PXSize  WriteI8SV(* const, const PXInt8S* const valueList, const PXSize valueListSize);
		PXSize  WriteI8U(* const, const PXInt8U value);
		PXSize  WriteAtI8U(* const, const PXInt8U value, const PXSize index);
		PXSize  WriteI8UV(* const, const PXInt8U* const valueList, const PXSize valueListSize);

		PXSize  WriteI16S(* const, const PXInt16S const value);
		PXSize  WriteI16SV(* const, const PXInt16S* const valueList, const PXSize valueListSize);
		PXSize  WriteI16SE(* const, const PXInt16S const value, const PXEndian pxEndian);
		PXSize  WriteI16SVE(* const, const PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
		PXSize  WriteI16U(* const, const PXInt16U const value);
		PXSize  WriteAtI16U(* const, const PXInt16U const value, const PXSize index);
		PXSize  WriteI16UV(* const, const PXInt16U* const valueList, const PXSize valueListSize);
		PXSize  WriteI16UE(* const, const PXInt16U const value, const PXEndian pxEndian);
		PXSize  WriteI16UVE(* const, const PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

		PXSize  WriteI32S(* const, const PXInt32S value);
		PXSize  WriteI32SV(* const, const PXInt32S* const valueList, const PXSize valueListSize);
		PXSize  WriteI32SE(* const, const PXInt32S value, const PXEndian pxEndian);
		PXSize  WriteI32SVE(* const, const PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
		PXSize  WriteI32U(* const, const PXInt32U value);
		PXSize  WriteAtI32U(* const, const PXInt32U value, const PXSize index);
		PXSize  WriteI32UV(* const, const PXInt32U* const valueList, const PXSize valueListSize);
		PXSize  WriteI32UE(* const, const PXInt32U value, const PXEndian pxEndian);
		PXSize  WriteAtI32UE(* const, const PXInt32U value, const PXEndian pxEndian, const PXSize index);
		PXSize  WriteI32UVE(* const, const PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

		PXSize  WriteI64S(* const, const PXInt64S const value);
		PXSize  WriteI64SV(* const, const PXInt64S* const valueList, const PXSize valueListSize);
		PXSize  WriteI64SE(* const, const PXInt64S const value, const PXEndian pxEndian);
		PXSize  WriteI64VE(* const, const PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
		PXSize  WriteI64U(* const, const PXInt64U const value);
		PXSize  WriteI64UV(* const, const PXInt64U* const valueList, const PXSize valueListSize);
		PXSize  WriteI64UE(* const, const PXInt64U const value, const PXEndian pxEndian);
		PXSize  WriteI64UVE(* const, const PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

		PXSize  WriteF(* const, const float value);
		PXSize  WriteFV(* const, const float* const valueList, const PXSize valueListSize);

		PXSize  WriteD(* const, const double value);
		PXSize  WriteDV(* const, const double* const valueList, const PXSize valueListSize);

		PXSize  WriteB(* const, const void* const value, const PXSize length);
		PXSize  WriteAtB(* const, const void* const data, const PXSize dataSize, const PXSize index);

		PXSize  WriteMultible(* const, const  DataElementType* const  ElementList, const PXSize  ElementListSize);

		PXSize  WriteFill(* const, const unsigned char value, const PXSize length);

		// Write ASCII string \0 terminated.
		PXSize  WriteA(* const, const PXTextASCII text, PXSize textSize);

		// Write UNICODE string \0 terminated
		PXSize  WriteW(* const, const PXTextUNICODE const text, PXSize textSize);

		PXSize  WriteAF(* const, const PXTextASCII format, ...);
		PXSize  WriteWF(* const, const PXTextUNICODE format, ...);
		//-------------------------------------------------------------------------



		//---<Extra bit stuff>-----------------------------------------------------
		PXSize  SkipBitsToNextByte(* const);
		PXSize  CursorMoveBits(* const, const PXSize amountOfBits);
		PXSize  BitsAllign(* const);

		PXSize  PeekBits(* const, const PXSize amountOfBits);
		PXSize  ReadBits(* const, const PXSize amountOfBits);

		PXSize  WriteBits(* const, const PXSize bitData, const PXSize amountOfBits);
		//-------------------------------------------------------------------------

		PXBool  PathGet(const* const, PXText* const filePath);
		*/

	};
}

#endif