#pragma once

#include <OS/File/PXFile.h>

namespace PX
{
	class DataStream : public PXFile
	{
		public:
		//---------------------------------------------------------------------
		PXDLLExport DataStream();
		PXDLLExport ~DataStream();
		//---------------------------------------------------------------------

		//---<Set>-------------------------------------------------------------
		void FromExternal(void* const data, const PXSize dataSize);
		//---------------------------------------------------------------------

		//---<Open>------------------------------------------------------------
		PXActionResult OpenFromPath(const PXTextASCII filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode);
		ActionResult OpenFromPath(const PXTextUNICODE filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode);
		//ActionResult OpenFromPath(const PXTextUTF8 filePath, const PXMemoryAccessMode fileOpenMode, const PXFileCachingMode dataStreamCachingMode);
		//---------------------------------------------------------------------

		//---<Close>-----------------------------------------------------------
		ActionResult Close();
		//---------------------------------------------------------------------

		//---<Mapping>---------------------------------------------------------
		PXDLLExport ActionResult MapToMemory(const PXTextASCII filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
		PXDLLExport ActionResult MapToMemory(const PXTextUNICODE filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
		//ActionResult MapToMemory(const PXTextUTF8 filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
		PXDLLExport ActionResult MapToMemory(const PXSize size, const PXMemoryAccessMode protectionMode);
		PXDLLExport ActionResult UnmapFromMemory();
		//---------------------------------------------------------------------

		//---<Parsing Utility>-----------------------------------------------------
		PXSize RemainingSize();
		bool IsAtEnd();

		void* CursorPosition();
		void CursorToBeginning();
		void CursorAdvance(const PXSize steps);
		void CursorRewind(const PXSize steps);
		void CursorToEnd();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		PXSize SkipEndOfLineCharacters();
		PXSize SkipEmptySpace();

		PXSize ReadNextLineInto(void* exportBuffer, const PXSize exportBufferSize);


		// Skips current block thats seperated with spaces.
		// Example : '|' is the cursor.
		// Before : |AAAA  BBBB CCC
		// After :  AAAA |BBBB CCC
		PXSize SkipBlock();
		PXSize SkipToNextBlock();
		PXSize SkipLine();

		PXSize ReadTextIU8(PXInt8U* const number);
		PXSize ReadTextI(int* const number);

		PXSize Read(char* const value);
		PXSize Read(unsigned char* const value);
		PXSize Read(short* const value, const PXEndian endian);
		PXSize Read(unsigned short* const value, const PXEndian endian);
		PXSize Read(int* const value, const  PXEndian endian);
		PXSize Read(unsigned int* const value, const PXEndian endian);
		PXSize Read(long long* const value, const PXEndian endian);
		PXSize Read(unsigned long long* const value, const  PXEndian endian);
		PXSize Read(float* const value);
		PXSize Read(double* const value);
		PXSize Read(void* const value, const PXSize length);
		PXSize Read(const void* const format, const PXSize length, ...);

		PXSize Read(char* const value, const PXSize length);
		PXSize Read(wchar_t* const value, const PXSize length);

		void ReadUntil(void* value, const PXSize length, const char character);



		// Additional check how many bytes are read.
		// Slower version of Read(), this function can't fail.
		//PXSize ReadSafe(Byte__* value, const PXSize length);


		unsigned char ReadAndCompare(const void* value, const PXSize length);
		//unsigned char ReadAndCompareC(* , const char value);
		//unsigned char ReadAndCompareIU(* , const unsigned int value);

		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		PXSize Write(const char value);
		PXSize Write(const unsigned char value);
		PXSize Write(const short value, const PXEndian endian);
		PXSize Write(const unsigned short value, const PXEndian endian);
		PXSize Write(const int value, const PXEndian endian);
		PXSize Write(const unsigned int value, const PXEndian endian);
		PXSize Write(const long long value, const PXEndian endian);
		PXSize Write(const unsigned long long value, const PXEndian endian);

		PXSize Write(const float value);
		PXSize Write(const double value);

		// Write ASCII string \0 terminated.
		PXSize Write(const char* const text, PXSize textSize);

		// Write UNICODE string \0 terminated
		PXSize Write(const wchar_t* const text, PXSize textSize);

		PXSize Write(const void* value, const PXSize length);
		PXSize WriteFill(const unsigned char value, const PXSize length);
		PXSize Write(const char* format, ...);

		PXSize WriteAt(const void* const data, const PXSize dataSize, const PXSize index);
		PXSize WriteAt(const unsigned char value, const PXSize index);
		PXSize WriteAt(const unsigned short value, const PXEndian endian, const PXSize index);
		PXSize WriteAt(const unsigned int value, const PXEndian endian, const PXSize index);
		//-------------------------------------------------------------------------



		//---<Extra bit stuff>-----------------------------------------------------
		PXSize SkipBitsToNextByte();
		PXSize CursorMoveBits(const PXSize amountOfBits);
		PXSize BitsAllign();

		PXSize PeekBits(const PXSize amountOfBits);
		PXSize ReadBits(const PXSize amountOfBits);

		PXSize WriteBits(const PXSize bitData, const PXSize amountOfBits);
		//-------------------------------------------------------------------------


		PXSize FilePathGetA(char* const filePath, const PXSize filePathMaxSize);
		PXSize FilePathGetW(wchar_t* const filePath, const PXSize filePathMaxSize);
	};
}