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
		PXSize DataSize;
		PXSize DataCursor;
		PXSize DataCursorBitOffset;
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
	PXPublic void DataStreamFromExternal(DataStream* const dataStream, void* const data, const PXSize dataSize);
	//---------------------------------------------------------------------

	//---<Open>------------------------------------------------------------
	PXPublic ActionResult DataStreamOpenFromPathA(DataStream* const dataStream, const char* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode);
	PXPublic ActionResult DataStreamOpenFromPathW(DataStream* const dataStream, const wchar_t* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode);
	//---------------------------------------------------------------------

	//---<Close>-----------------------------------------------------------
	PXPublic ActionResult DataStreamClose(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Mapping>---------------------------------------------------------
	PXPublic ActionResult DataStreamMapToMemoryA(DataStream* const dataStream, const char* filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult DataStreamMapToMemoryW(DataStream* const dataStream, const wchar_t* filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult DataStreamMapToMemory(DataStream* const dataStream, const PXSize size, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult DataStreamUnmapFromMemory(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Parsing Utility>-----------------------------------------------------
	PXPublic PXSize DataStreamRemainingSize(DataStream* const dataStream);
	PXPublic unsigned char DataStreamIsAtEnd(DataStream* const dataStream);

	PXPublic void* DataStreamCursorPosition(DataStream* const dataStream);
	PXPublic void DataStreamCursorToBeginning(DataStream* const dataStream);
	PXPublic void DataStreamCursorAdvance(DataStream* const dataStream, const PXSize steps);
	PXPublic void DataStreamCursorRewind(DataStream* const dataStream, const PXSize steps);
	PXPublic void DataStreamCursorToEnd(DataStream* const dataStream);	
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic PXSize DataStreamSkipEndOfLineCharacters(DataStream* const  DataStream);
	PXPublic PXSize DataStreamSkipEmptySpace(DataStream* const dataStream);

	PXPublic PXSize DataStreamReadNextLineInto(DataStream* const dataStream, void* exportBuffer, const PXSize exportBufferSize);


	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	PXPublic PXSize DataStreamSkipBlock(DataStream* const dataStream);
	PXPublic PXSize DataStreamSkipToNextBlock(DataStream* const dataStream);
	PXPublic PXSize DataStreamSkipLine(DataStream* const dataStream);

	PXPublic PXSize DataStreamReadC(DataStream* const dataStream, char* const value);
	PXPublic PXSize DataStreamReadCU(DataStream* const dataStream, unsigned char* const value);
	PXPublic PXSize DataStreamReadS(DataStream* const dataStream, short* const value, const Endian endian);
	PXPublic PXSize DataStreamReadSU(DataStream* const dataStream, unsigned short* const value, const Endian endian);
	PXPublic PXSize DataStreamReadI(DataStream* const dataStream, int* const value, const  Endian endian);
	PXPublic PXSize DataStreamReadIU(DataStream* const dataStream, unsigned int* const value, const Endian endian);
	PXPublic PXSize DataStreamReadLL(DataStream* const dataStream, long long* const value, const Endian endian);
	PXPublic PXSize DataStreamReadLLU(DataStream* const dataStream, unsigned long long* const value, const  Endian endian);
	PXPublic PXSize DataStreamReadF(DataStream* const dataStream, float* const value);
	PXPublic PXSize DataStreamReadD(DataStream* const dataStream, double* const value);
	PXPublic PXSize DataStreamReadP(DataStream* const dataStream, void* const value, const PXSize length);
	PXPublic PXSize DataStreamRead(DataStream* const dataStream, const void* const format, const PXSize length, ...);

	PXPublic PXSize DataStreamReadA(DataStream* const dataStream, char* const value, const PXSize length);
	PXPublic PXSize DataStreamReadW(DataStream* const dataStream, wchar_t* const value, const PXSize length);

	PXPublic void DataStreamReadUntil(DataStream* const dataStream, void* value, const PXSize length, const char character);



	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//PXSize ReadSafe(Byte__* value, const PXSize length);


	PXPublic unsigned char DataStreamReadAndCompare(DataStream* const dataStream, const void* value, const PXSize length);
	//unsigned char DataStreamReadAndCompareC(DataStream* DataStream, const char value);
	//unsigned char DataStreamReadAndCompareIU(DataStream* DataStream, const unsigned int value);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic PXSize DataStreamWriteC(DataStream* const dataStream, const char value);
	PXPublic PXSize DataStreamWriteCU(DataStream* const dataStream, const unsigned char value);
	PXPublic PXSize DataStreamWriteS(DataStream* const dataStream, const short value, const Endian endian);
	PXPublic PXSize DataStreamWriteSU(DataStream* const dataStream, const unsigned short value, const Endian endian);
	PXPublic PXSize DataStreamWriteI(DataStream* const dataStream, const int value, const Endian endian);
	PXPublic PXSize DataStreamWriteIU(DataStream* const dataStream, const unsigned int value, const Endian endian);
	PXPublic PXSize DataStreamWriteLL(DataStream* const dataStream, const long long value, const Endian endian);
	PXPublic PXSize DataStreamWriteLLU(DataStream* const dataStream, const unsigned long long value, const Endian endian);

	PXPublic PXSize DataStreamWriteF(DataStream* const dataStream, const float value);
	PXPublic PXSize DataStreamWriteD(DataStream* const dataStream, const double value);

	// Write ASCII string \0 terminated.
	PXPublic PXSize DataStreamWriteA(DataStream* const dataStream, const char* const text, PXSize textSize);

	// Write UNICODE string \0 terminated
	PXPublic PXSize DataStreamWriteW(DataStream* const dataStream, const wchar_t* const text, PXSize textSize);

	PXPublic PXSize DataStreamWriteP(DataStream* const dataStream, const void* value, const PXSize length);
	PXPublic PXSize DataStreamWriteFill(DataStream* const dataStream, const unsigned char value, const PXSize length);
	PXPublic PXSize DataStreamWrite(DataStream* const dataStream, const char* format, ...);

	PXPublic PXSize DataStreamWriteAtP(DataStream* const dataStream, const void* const data, const PXSize dataSize, const PXSize index);
	PXPublic PXSize DataStreamWriteAtCU(DataStream* const dataStream, const unsigned char value, const PXSize index);
	PXPublic PXSize DataStreamWriteAtSU(DataStream* const dataStream, const unsigned short value, const Endian endian, const PXSize index);
	PXPublic PXSize DataStreamWriteAtIU(DataStream* const dataStream, const unsigned int value, const Endian endian, const PXSize index);
	//-------------------------------------------------------------------------



	//---<Extra bit stuff>-----------------------------------------------------
	PXPublic PXSize DataStreamSkipBitsToNextByte(DataStream* const dataStream);
	PXPublic PXSize DataStreamCursorMoveBits(DataStream* const dataStream, const PXSize amountOfBits);
	PXPublic PXSize DataStreamBitsAllign(DataStream* const dataStream);

	PXPublic PXSize DataStreamPeekBits(DataStream* const dataStream, const PXSize amountOfBits);
	PXPublic PXSize DataStreamReadBits(DataStream* const dataStream, const PXSize amountOfBits);

	PXPublic PXSize DataStreamWriteBits(DataStream* const dataStream, const PXSize bitData, const PXSize amountOfBits);
	//-------------------------------------------------------------------------


	PXPublic PXSize DataStreamFilePathGetA(DataStream* const dataStream, char* const filePath, const PXSize filePathMaxSize);
	PXPublic PXSize DataStreamFilePathGetW(DataStream* const dataStream, wchar_t* const filePath, const PXSize filePathMaxSize);

#ifdef __cplusplus
}
#endif

#endif