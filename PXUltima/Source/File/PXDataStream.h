#ifndef PXDataStreamINCLUDE
#define PXDataStreamINCLUDE

#include <Format/Type.h>
#include <Error/ActionResult.h>

#include "Endian.h"
#include "File.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXDataStreamLocation_
	{
		FileLocationInvalid,
		FileLocationExternal,
		FileLocationMappedVirtual, // Used 'VirtalAlloc()' / 'mmap()'
		FileLocationMappedFromDisk, // Used 'FileView()' / 'fmap()'
		FileLocationCachedFromDisk, // When the file is cached into an CPrivate buffer
		FileLocationLinked // Used existing file with 'fopen()'
	}
	PXDataStreamLocation;

	typedef enum DataPersistence_
	{
		PersistenceInvalid,
		PersistencePermanent,
		PersistenceTempoary
	}
	DataPersistence;

	typedef enum PXDataStreamCachingMode_
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
	PXDataStreamCachingMode;

	typedef struct PXDataStreamElementType_
	{	
		PXDataType Type;
		void* Adress;
	}
	PXDataStreamElementType;

	typedef struct PXDataStream_
	{
		//---<Raw Data>--------------------------------------------------------
		void* Data;
		PXSize DataSize;
		PXSize DataCursor;
		PXSize DataCursorBitOffset;
		//---------------------------------------------------------------------

		//---<Extended Settings>-----------------------------------------------
		MemoryProtectionMode MemoryMode;
		PXDataStreamLocation DataLocation; // Where the is stored, used as indicator how to clean up.
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
	PXDataStream;

	//---------------------------------------------------------------------
	PXPublic void PXDataStreamConstruct(PXDataStream* const dataStream);
	PXPublic void PXDataStreamDestruct(PXDataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Convert>---------------------------------------------------------
	PXPrivate PXDataStreamCachingMode ConvertToFileCachingMode(const unsigned int value);
	PXPrivate unsigned int ConvertFromFileCachingMode(const PXDataStreamCachingMode fileCachingMode);
	//---------------------------------------------------------------------

	//---<Set>-------------------------------------------------------------
	PXPublic void PXDataStreamFromExternal(PXDataStream* const dataStream, void* const data, const PXSize dataSize);
	//---------------------------------------------------------------------

	//---<Open>------------------------------------------------------------
	PXPublic ActionResult PXDataStreamOpenFromPathA(PXDataStream* const dataStream, const PXTextASCII filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode);
	PXPublic ActionResult PXDataStreamOpenFromPathW(PXDataStream* const dataStream, const PXTextUNICODE filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode);
	PXPublic ActionResult PXDataStreamOpenFromPathU(PXDataStream* const dataStream, const PXTextUTF8 filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode);
	//---------------------------------------------------------------------

	//---<Close>-----------------------------------------------------------
	PXPublic ActionResult PXDataStreamClose(PXDataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Mapping>---------------------------------------------------------
	PXPublic ActionResult PXDataStreamMapToMemoryA(PXDataStream* const dataStream, const PXTextASCII filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult PXDataStreamMapToMemoryW(PXDataStream* const dataStream, const PXTextUNICODE filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult PXDataStreamMapToMemoryU(PXDataStream* const dataStream, const PXTextUTF8 filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult PXDataStreamMapToMemory(PXDataStream* const dataStream, const PXSize size, const MemoryProtectionMode protectionMode);
	PXPublic ActionResult PXDataStreamUnmapFromMemory(PXDataStream* const dataStream);
	//---------------------------------------------------------------------

	//---<Parsing Utility>-----------------------------------------------------
	PXPublic PXSize PXDataStreamRemainingSize(PXDataStream* const dataStream);
	PXPublic unsigned char PXDataStreamIsAtEnd(PXDataStream* const dataStream);

	PXPublic void* PXDataStreamCursorPosition(PXDataStream* const dataStream);
	PXPublic void PXDataStreamCursorToBeginning(PXDataStream* const dataStream);
	PXPublic PXSize PXDataStreamCursorAdvance(PXDataStream* const dataStream, const PXSize steps);
	PXPublic void PXDataStreamCursorRewind(PXDataStream* const dataStream, const PXSize steps);
	PXPublic void PXDataStreamCursorToEnd(PXDataStream* const dataStream);	
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic PXSize PXDataStreamSkipEndOfLineCharacters(PXDataStream* const  PXDataStream);
	PXPublic PXSize PXDataStreamSkipEmptySpace(PXDataStream* const dataStream);

	PXPublic PXSize PXDataStreamReadNextLineInto(PXDataStream* const dataStream, void* exportBuffer, const PXSize exportBufferSize);


	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	PXPublic PXSize PXDataStreamSkipBlock(PXDataStream* const dataStream);
	PXPublic PXSize PXDataStreamSkipToNextBlock(PXDataStream* const dataStream);
	PXPublic PXSize PXDataStreamSkipLine(PXDataStream* const dataStream);

	PXPublic PXSize PXDataStreamReadTextIU8(PXDataStream* const dataStream, PXInt8U* const number);
	PXPublic PXSize PXDataStreamReadTextI(PXDataStream* const dataStream, int* const number);

	
	PXPublic PXSize PXDataStreamReadI8S(PXDataStream* const dataStream, PXInt8S* const value);
	PXPublic PXSize PXDataStreamReadI8SV(PXDataStream* const dataStream, PXInt8S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI8U(PXDataStream* const dataStream, PXInt8U* const value);
	PXPublic PXSize PXDataStreamReadI8UV(PXDataStream* const dataStream, PXInt8U* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXDataStreamReadI16S(PXDataStream* const dataStream, PXInt16S* const value);
	PXPublic PXSize PXDataStreamReadI16SV(PXDataStream* const dataStream, PXInt16S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI16SE(PXDataStream* const dataStream, PXInt16S* const value, const Endian endian);
	PXPublic PXSize PXDataStreamReadI16SVE(PXDataStream* const dataStream, PXInt16S* const valueList, const PXSize valueListSize, const Endian endian);
	PXPublic PXSize PXDataStreamReadI16U(PXDataStream* const dataStream, PXInt16U* const value);
	PXPublic PXSize PXDataStreamReadI16UV(PXDataStream* const dataStream, PXInt16U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI16UE(PXDataStream* const dataStream, PXInt16U* const value, const Endian endian);
	PXPublic PXSize PXDataStreamReadI16UVE(PXDataStream* const dataStream, PXInt16U* const valueList, const PXSize valueListSize, const Endian endian);

	PXPublic PXSize PXDataStreamReadI32S(PXDataStream* const dataStream, PXInt32S* const value);
	PXPublic PXSize PXDataStreamReadI32SV(PXDataStream* const dataStream, PXInt32S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI32SE(PXDataStream* const dataStream, PXInt32S* const value, const Endian endian);
	PXPublic PXSize PXDataStreamReadI32SVE(PXDataStream* const dataStream, PXInt32S* const valueList, const PXSize valueListSize, const Endian endian);
	PXPublic PXSize PXDataStreamReadI32U(PXDataStream* const dataStream, PXInt32U* const value);
	PXPublic PXSize PXDataStreamReadI32UV(PXDataStream* const dataStream, PXInt32U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI32UE(PXDataStream* const dataStream, PXInt32U* const value, const Endian endian);
	PXPublic PXSize PXDataStreamReadI32UVE(PXDataStream* const dataStream, PXInt32U* const valueList, const PXSize valueListSize, const Endian endian);

	PXPublic PXSize PXDataStreamReadI64S(PXDataStream* const dataStream, PXInt64S* const value);
	PXPublic PXSize PXDataStreamReadI64SV(PXDataStream* const dataStream, PXInt64S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI64SE(PXDataStream* const dataStream, PXInt64S* const value, const Endian endian);
	PXPublic PXSize PXDataStreamReadI64VE(PXDataStream* const dataStream, PXInt64S* const valueList, const PXSize valueListSize, const Endian endian);
	PXPublic PXSize PXDataStreamReadI64U(PXDataStream* const dataStream, PXInt64U* const value);
	PXPublic PXSize PXDataStreamReadI64UV(PXDataStream* const dataStream, PXInt64U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXDataStreamReadI64UE(PXDataStream* const dataStream, PXInt64U* const value, const Endian endian);
	PXPublic PXSize PXDataStreamReadI64UVE(PXDataStream* const dataStream, PXInt64U* const valueList, const PXSize valueListSize, const Endian endian);

	PXPublic PXSize PXDataStreamReadF(PXDataStream* const dataStream, float* const value);
	PXPublic PXSize PXDataStreamReadFV(PXDataStream* const dataStream, float* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXDataStreamReadD(PXDataStream* const dataStream, double* const value);
	PXPublic PXSize PXDataStreamReadDV(PXDataStream* const dataStream, double* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXDataStreamReadMultible(PXDataStream* const dataStream, PXDataStreamElementType* const pxDataStreamElementList, const PXSize pxDataStreamElementListSize);


	PXPublic PXSize PXDataStreamReadB(PXDataStream* const dataStream, void* const value, const PXSize length);

	PXPublic PXSize PXDataStreamReadTextA(PXDataStream* const dataStream, PXTextASCII value, const PXSize length);
	PXPublic PXSize PXDataStreamReadTextW(PXDataStream* const dataStream, PXTextUNICODE value, const PXSize length);
	PXPublic PXSize PXDataStreamReadTextU(PXDataStream* const dataStream, PXTextUNICODE value, const PXSize length);

	PXPublic void PXDataStreamReadUntil(PXDataStream* const dataStream, void* value, const PXSize length, const char character);



	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//PXSize ReadSafe(Byte__* value, const PXSize length);


	PXPublic unsigned char PXDataStreamReadAndCompare(PXDataStream* const dataStream, const void* value, const PXSize length);
	//unsigned char PXDataStreamReadAndCompareC(PXDataStream* PXDataStream, const char value);
	//unsigned char PXDataStreamReadAndCompareIU(PXDataStream* PXDataStream, const unsigned int value);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic PXSize PXDataStreamWriteC(PXDataStream* const dataStream, const char value);
	PXPublic PXSize PXDataStreamWriteCU(PXDataStream* const dataStream, const unsigned char value);
	PXPublic PXSize PXDataStreamWriteS(PXDataStream* const dataStream, const short value, const Endian endian);
	PXPublic PXSize PXDataStreamWriteSU(PXDataStream* const dataStream, const unsigned short value, const Endian endian);
	PXPublic PXSize PXDataStreamWriteI(PXDataStream* const dataStream, const int value, const Endian endian);
	PXPublic PXSize PXDataStreamWriteIU(PXDataStream* const dataStream, const unsigned int value, const Endian endian);
	PXPublic PXSize PXDataStreamWriteLL(PXDataStream* const dataStream, const long long value, const Endian endian);
	PXPublic PXSize PXDataStreamWriteLLU(PXDataStream* const dataStream, const unsigned long long value, const Endian endian);

	PXPublic PXSize PXDataStreamWriteF(PXDataStream* const dataStream, const float value);
	PXPublic PXSize PXDataStreamWriteD(PXDataStream* const dataStream, const double value);

	// Write ASCII string \0 terminated.
	PXPublic PXSize PXDataStreamWriteA(PXDataStream* const dataStream, const char* const text, PXSize textSize);

	// Write UNICODE string \0 terminated
	PXPublic PXSize PXDataStreamWriteW(PXDataStream* const dataStream, const wchar_t* const text, PXSize textSize);

	PXPublic PXSize PXDataStreamWriteP(PXDataStream* const dataStream, const void* value, const PXSize length);
	PXPublic PXSize PXDataStreamWriteFill(PXDataStream* const dataStream, const unsigned char value, const PXSize length);
	PXPublic PXSize PXDataStreamWrite(PXDataStream* const dataStream, const char* format, ...);

	PXPublic PXSize PXDataStreamWriteAtP(PXDataStream* const dataStream, const void* const data, const PXSize dataSize, const PXSize index);
	PXPublic PXSize PXDataStreamWriteAtCU(PXDataStream* const dataStream, const unsigned char value, const PXSize index);
	PXPublic PXSize PXDataStreamWriteAtSU(PXDataStream* const dataStream, const unsigned short value, const Endian endian, const PXSize index);
	PXPublic PXSize PXDataStreamWriteAtIU(PXDataStream* const dataStream, const unsigned int value, const Endian endian, const PXSize index);
	//-------------------------------------------------------------------------



	//---<Extra bit stuff>-----------------------------------------------------
	PXPublic PXSize PXDataStreamSkipBitsToNextByte(PXDataStream* const dataStream);
	PXPublic PXSize PXDataStreamCursorMoveBits(PXDataStream* const dataStream, const PXSize amountOfBits);
	PXPublic PXSize PXDataStreamBitsAllign(PXDataStream* const dataStream);

	PXPublic PXSize PXDataStreamPeekBits(PXDataStream* const dataStream, const PXSize amountOfBits);
	PXPublic PXSize PXDataStreamReadBits(PXDataStream* const dataStream, const PXSize amountOfBits);

	PXPublic PXSize PXDataStreamWriteBits(PXDataStream* const dataStream, const PXSize bitData, const PXSize amountOfBits);
	//-------------------------------------------------------------------------


	PXPublic PXSize PXDataStreamFilePathGetA(PXDataStream* const dataStream, char* const filePath, const PXSize filePathMaxSize);
	PXPublic PXSize PXDataStreamFilePathGetW(PXDataStream* const dataStream, wchar_t* const filePath, const PXSize filePathMaxSize);

#ifdef __cplusplus
}
#endif

#endif