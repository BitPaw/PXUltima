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
		//---<Raw Data>--------
		void* Data;
		size_t DataSize;
		size_t DataCursor;
		size_t DataBitOffset;
		//---------------------

		// Into, location needed to write
		MemoryProtectionMode MemoryMode;

		DataStreamLocation _fileLocation; // Where the is stored, used as indicator how to clean up.

		FileHandleType FileHandle; // Only used if file is used directly

		FileMappingID IDMapping; // Only used while mapping a file

#if defined(OSUnix)

#elif defined(OSWindows)
		FILE* FileHandleCStyle; // Used for writing only, usage of fprintf()
#endif	

	}
	DataStream;

	//---------------------------------------------------------------------
	CPublic void DataStreamConstruct(DataStream* const dataStream);
	CPublic void DataStreamDestruct(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Convert>---------------------------------------------------------
	CPrivate DataStreamCachingMode ConvertToFileCachingMode(const unsigned int value);
	CPrivate unsigned int ConvertFromFileCachingMode(const DataStreamCachingMode fileCachingMode);
	//---------------------------------------------------------------------

	//---<Set>-------------------------------------------------------------
	CPublic void DataStreamFromExternal(DataStream* const dataStream, void* const data, const size_t dataSize);
	//---------------------------------------------------------------------

	//---<Open>------------------------------------------------------------
	CPublic ActionResult DataStreamOpenFromPathA(DataStream* const dataStream, const char* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode);
	CPublic ActionResult DataStreamOpenFromPathW(DataStream* const dataStream, const wchar_t* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode);
	//---------------------------------------------------------------------

	//---<Close>-----------------------------------------------------------
	CPublic ActionResult DataStreamClose(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Mapping>---------------------------------------------------------
	CPublic ActionResult DataStreamMapToMemoryA(DataStream* const dataStream, const char* filePath, const size_t fileSize, const MemoryProtectionMode protectionMode);
	CPublic ActionResult DataStreamMapToMemoryW(DataStream* const dataStream, const wchar_t* filePath, const size_t fileSize, const MemoryProtectionMode protectionMode);
	CPublic ActionResult DataStreamMapToMemory(DataStream* const dataStream, const size_t size, const MemoryProtectionMode protectionMode);
	CPublic ActionResult DataStreamUnmapFromMemory(DataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Parsing Utility>-----------------------------------------------------
	CPublic size_t DataStreamRemainingSize(DataStream* const dataStream);
	CPublic unsigned char DataStreamIsAtEnd(DataStream* const dataStream);

	CPublic void* DataStreamCursorPosition(DataStream* const dataStream);
	CPublic void DataStreamCursorToBeginning(DataStream* const dataStream);
	CPublic void DataStreamCursorAdvance(DataStream* const dataStream, const size_t steps);
	CPublic void DataStreamCursorRewind(DataStream* const dataStream, const size_t steps);
	CPublic void DataStreamCursorToEnd(DataStream* const dataStream);	
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	CPublic size_t DataStreamSkipEndOfLineCharacters(DataStream* const  DataStream);
	CPublic size_t DataStreamSkipEmptySpace(DataStream* const dataStream);

	CPublic size_t DataStreamReadNextLineInto(DataStream* const dataStream, void* exportBuffer, const size_t exportBufferSize);


	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	CPublic size_t DataStreamSkipBlock(DataStream* const dataStream);
	CPublic size_t DataStreamSkipToNextBlock(DataStream* const dataStream);
	CPublic size_t DataStreamSkipLine(DataStream* const dataStream);

	CPublic size_t DataStreamReadC(DataStream* const dataStream, char* value);
	CPublic size_t DataStreamReadCU(DataStream* const dataStream, unsigned char* value);
	CPublic size_t DataStreamReadS(DataStream* const dataStream, short* value, const Endian endian);
	CPublic size_t DataStreamReadSU(DataStream* const dataStream, unsigned short* value, const Endian endian);
	CPublic size_t DataStreamReadI(DataStream* const dataStream, int* value, const  Endian endian);
	CPublic size_t DataStreamReadIU(DataStream* const dataStream, unsigned int* value, const Endian endian);
	CPublic size_t DataStreamReadLL(DataStream* const dataStream, long long* value, const Endian endian);
	CPublic size_t DataStreamReadLLU(DataStream* const dataStream, unsigned long long* value, const  Endian endian);
	CPublic size_t DataStreamReadD(DataStream* const dataStream, void* value, const size_t length);
	CPublic size_t DataStreamRead(DataStream* const dataStream, const void* format, const size_t length, ...);


	CPublic void DataStreamReadUntil(DataStream* const dataStream, void* value, const size_t length, const char character);



	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//size_t ReadSafe(Byte__* value, const size_t length);


	unsigned char DataStreamReadAndCompare(DataStream* const dataStream, const void* value, const size_t length);
	//unsigned char DataStreamReadAndCompareC(DataStream* DataStream, const char value);
	//unsigned char DataStreamReadAndCompareIU(DataStream* DataStream, const unsigned int value);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	CPublic size_t DataStreamWriteC(DataStream* const dataStream, const char value);
	CPublic size_t DataStreamWriteCU(DataStream* const dataStream, const unsigned char value);
	CPublic size_t DataStreamWriteS(DataStream* const dataStream, const short value, const Endian endian);
	CPublic size_t DataStreamWriteSU(DataStream* const dataStream, const unsigned short value, const Endian endian);
	CPublic size_t DataStreamWriteI(DataStream* const  DataStream, const int value, const Endian endian);
	CPublic size_t DataStreamWriteIU(DataStream* const dataStream, const unsigned int value, const Endian endian);
	CPublic size_t DataStreamWriteLL(DataStream* const dataStream, const long long value, const Endian endian);
	CPublic size_t DataStreamWriteLU(DataStream* const dataStream, const unsigned long long value, const Endian endian);

	CPublic size_t DataStreamWriteF(DataStream* const dataStream, const float value);
	//CPublic size_t DataStreamWrite(DataStream* const dataStream, const double value);

	CPublic size_t DataStreamWriteD(DataStream* const dataStream, const void* value, const size_t length);
	CPublic size_t DataStreamWriteFill(DataStream* const dataStream, const unsigned char value, const size_t length);
	CPublic size_t DataStreamWrite(DataStream* const dataStream, const char* format, ...);

	CPublic size_t DataStreamWriteAtCU(DataStream* const dataStream, const unsigned char value, const size_t index);
	CPublic size_t DataStreamWriteAtIU(DataStream* const dataStream, const unsigned int value, const Endian endian, const size_t index);
	//-------------------------------------------------------------------------

#endif

#ifdef __cplusplus
}
#endif