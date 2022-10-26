#ifndef DataStreamINCLUDE
#define DataStreamINCLUDE

#include <Format/Type.h>
#include <Error/ActionResult.h>

#include "Endian.h"
#include "File.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum DataStreamLocation_
	{
		FileLocationInvalid,
		FileLocationExternal,
		FileLocationMappedVirtual, // Used 'VirtalAlloc()' / 'mmap()'
		FileLocationMappedFromDisk, // Used 'FileView()' / 'fmap()'
		FileLocationCachedFromDisk, // When the file is cached into an CPrivate buffer
		FileLocationLinked // Used existing file with 'fopen()'
	}
	DataStreamLocation;

	typedef enum DataPersistence_
	{
		PersistenceInvalid,
		PersistencePermanent,
		PersistenceTempoary
	}
	DataPersistence;

	typedef enum DataStreamCachingMode_
	{
		FileCachingDefault,

		FileCachingRandom,  // Access data in a random order.
		FileCachingSequential, // Data sequentially from lower offsets to higher offsets.
		FileCachingTemporary, // File will not be saves to drive.
		FileCachingUseOnce, // Access the specified data once and then not reuse it again.

		// Windows only
		FileCachingWriteThrough,
		FileCachingNoBuffering, // No OS Caching, direct to Harddrive if supprted

		// UNIX only
		FileCachingNeedLater, // Data is not needed right now. "For later"[near future].
		FileCachingDontNeedNow // Data will not be cached. "I dont need it yet"
	}
	DataStreamCachingMode;


	typedef struct DataStream_
	{
		//---<Raw Data>--------------------------------------------------------
		void* Data;
		size_t DataSize;
		size_t DataCursor;
		size_t DataCursorBitOffset;
		//---------------------------------------------------------------------

		//---<Extended Settings>-----------------------------------------------
		MemoryProtectionMode MemoryMode;
		DataStreamLocation DataLocation; // Where the is stored, used as indicator how to clean up.
		//---------------------------------------------------------------------

		//---<Internal location Info>------------------------------------------
		FileHandleType FileHandle; // Only used if file is used directly
		FileMappingID IDMapping; // Only used while mapping a file

#if OSUnix

#elif OSWindows
		FILE* FileHandleCStyle; // Used for writing only, usage of fprintf()
#endif	
		//---------------------------------------------------------------------
	}
	DataStream;

	//---------------------------------------------------------------------
	PXPublic void DataStreamConstruct(DataStream* const dataStream);
	PXPublic void DataStreamDestruct(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Convert>---------------------------------------------------------
	PXPrivate DataStreamCachingMode ConvertToFileCachingMode(const unsigned int value);
	PXPrivate unsigned int ConvertFromFileCachingMode(const DataStreamCachingMode fileCachingMode);
	//---------------------------------------------------------------------

	//---<Set>-------------------------------------------------------------
	PXPublic void DataStreamFromExternal(DataStream* const dataStream, void* const data, const size_t dataSize);
	//---------------------------------------------------------------------

	//---<Open>------------------------------------------------------------
	PXPublic ActionResult DataStreamOpenFromPathA(DataStream* const dataStream, const char* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode);
	PXPublic ActionResult DataStreamOpenFromPathW(DataStream* const dataStream, const wchar_t* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode);
	//---------------------------------------------------------------------

	//---<Close>-----------------------------------------------------------
	PXPublic ActionResult DataStreamClose(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Mapping>---------------------------------------------------------
	PXPublic ActionResult DataStreamMapToMemoryA(DataStream* const dataStream, const char* filePath, const size_t fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult DataStreamMapToMemoryW(DataStream* const dataStream, const wchar_t* filePath, const size_t fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult DataStreamMapToMemory(DataStream* const dataStream, const size_t size, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult DataStreamUnmapFromMemory(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Parsing Utility>-----------------------------------------------------
	PXPublic size_t DataStreamRemainingSize(DataStream* const dataStream);
	PXPublic unsigned char DataStreamIsAtEnd(DataStream* const dataStream);

	PXPublic void* DataStreamCursorPosition(DataStream* const dataStream);
	PXPublic void DataStreamCursorToBeginning(DataStream* const dataStream);
	PXPublic void DataStreamCursorAdvance(DataStream* const dataStream, const size_t steps);
	PXPublic void DataStreamCursorRewind(DataStream* const dataStream, const size_t steps);
	PXPublic void DataStreamCursorToEnd(DataStream* const dataStream);	
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic size_t DataStreamSkipEndOfLineCharacters(DataStream* const  DataStream);
	PXPublic size_t DataStreamSkipEmptySpace(DataStream* const dataStream);

	PXPublic size_t DataStreamReadNextLineInto(DataStream* const dataStream, void* exportBuffer, const size_t exportBufferSize);


	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	PXPublic size_t DataStreamSkipBlock(DataStream* const dataStream);
	PXPublic size_t DataStreamSkipToNextBlock(DataStream* const dataStream);
	PXPublic size_t DataStreamSkipLine(DataStream* const dataStream);

	PXPublic size_t DataStreamReadC(DataStream* const dataStream, char* value);
	PXPublic size_t DataStreamReadCU(DataStream* const dataStream, unsigned char* value);
	PXPublic size_t DataStreamReadS(DataStream* const dataStream, short* value, const Endian endian);
	PXPublic size_t DataStreamReadSU(DataStream* const dataStream, unsigned short* value, const Endian endian);
	PXPublic size_t DataStreamReadI(DataStream* const dataStream, int* value, const  Endian endian);
	PXPublic size_t DataStreamReadIU(DataStream* const dataStream, unsigned int* value, const Endian endian);
	PXPublic size_t DataStreamReadLL(DataStream* const dataStream, long long* value, const Endian endian);
	PXPublic size_t DataStreamReadLLU(DataStream* const dataStream, unsigned long long* value, const  Endian endian);
	PXPublic size_t DataStreamReadF(DataStream* const dataStream, float* const value);
	PXPublic size_t DataStreamReadD(DataStream* const dataStream, double* const value);
	PXPublic size_t DataStreamReadP(DataStream* const dataStream, void* value, const size_t length);
	PXPublic size_t DataStreamRead(DataStream* const dataStream, const void* format, const size_t length, ...);

	PXPublic size_t DataStreamReadTextA(DataStream* const dataStream, char* const value, const size_t length);
	PXPublic size_t DataStreamReadTextW(DataStream* const dataStream, wchar_t* const value, const size_t length);

	PXPublic void DataStreamReadUntil(DataStream* const dataStream, void* value, const size_t length, const char character);



	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//size_t ReadSafe(Byte__* value, const size_t length);


	PXPublic unsigned char DataStreamReadAndCompare(DataStream* const dataStream, const void* value, const size_t length);
	//unsigned char DataStreamReadAndCompareC(DataStream* DataStream, const char value);
	//unsigned char DataStreamReadAndCompareIU(DataStream* DataStream, const unsigned int value);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic size_t DataStreamWriteC(DataStream* const dataStream, const char value);
	PXPublic size_t DataStreamWriteCU(DataStream* const dataStream, const unsigned char value);
	PXPublic size_t DataStreamWriteS(DataStream* const dataStream, const short value, const Endian endian);
	PXPublic size_t DataStreamWriteSU(DataStream* const dataStream, const unsigned short value, const Endian endian);
	PXPublic size_t DataStreamWriteI(DataStream* const dataStream, const int value, const Endian endian);
	PXPublic size_t DataStreamWriteIU(DataStream* const dataStream, const unsigned int value, const Endian endian);
	PXPublic size_t DataStreamWriteLL(DataStream* const dataStream, const long long value, const Endian endian);
	PXPublic size_t DataStreamWriteLU(DataStream* const dataStream, const unsigned long long value, const Endian endian);

	PXPublic size_t DataStreamWriteF(DataStream* const dataStream, const float value);
	PXPublic size_t DataStreamWriteD(DataStream* const dataStream, const double value);

	PXPublic size_t DataStreamWriteP(DataStream* const dataStream, const void* value, const size_t length);
	PXPublic size_t DataStreamWriteFill(DataStream* const dataStream, const unsigned char value, const size_t length);
	PXPublic size_t DataStreamWrite(DataStream* const dataStream, const char* format, ...);

	PXPublic size_t DataStreamWriteAtCU(DataStream* const dataStream, const unsigned char value, const size_t index);
	PXPublic size_t DataStreamWriteAtSU(DataStream* const dataStream, const unsigned short value, const Endian endian, const size_t index);
	PXPublic size_t DataStreamWriteAtIU(DataStream* const dataStream, const unsigned int value, const Endian endian, const size_t index);
	//-------------------------------------------------------------------------



	//---<Extra bit stuff>-----------------------------------------------------
	PXPublic size_t DataStreamSkipBitsToNextByte(DataStream* const dataStream);
	PXPublic size_t DataStreamCursorMoveBits(DataStream* const dataStream, const size_t amountOfBits);
	PXPublic size_t DataStreamBitsAllign(DataStream* const dataStream);

	PXPublic size_t DataStreamPeekBits(DataStream* const dataStream, const size_t amountOfBits);
	PXPublic size_t DataStreamReadBits(DataStream* const dataStream, const size_t amountOfBits);

	PXPublic size_t DataStreamWriteBits(DataStream* const dataStream, const size_t bitData, const size_t amountOfBits);
	//-------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif