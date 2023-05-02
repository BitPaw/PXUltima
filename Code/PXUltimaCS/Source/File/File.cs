using System;
using System.Runtime.InteropServices;

namespace PX
{
    public enum Endian
    {
		Invalid,
		Big,
		Middle,
		Little
    }

    public class File
    {
		[StructLayout(LayoutKind.Sequential, Size = 72)]
		private unsafe struct PXFile
        {
			//---<PosisionData>---
			public void* Data; // [Do not use directly] Data from where to read/write depending on the used method of linking.
			public void* DataCursor; // [Do not use directly] Current position of the data.
			public void* DataCursorBitOffset; // [Do not use directly] Current offset in bits of current byte
			public void* DataSize; // [Do not use directly] The total size of the data block.
			public void* DataAllocated; // [Do not use directly] The size of the data pace in which you can move without triggering an invalid access.
										//--------------------

			public uint AccessMode;
			public uint CachingMode;
			public uint LocationMode;
		}

		private PXFile _pxFile = new PXFile();

		public unsafe ulong DataCursor => (ulong)_pxFile.DataCursor;
		public unsafe ulong DataCursorBitOffset => (ulong)_pxFile.DataCursorBitOffset;
		public unsafe ulong DataSize => (ulong)_pxFile.DataSize;
		public unsafe ulong DataAllocated => (ulong)_pxFile.DataAllocated;
		public uint AccessMode => _pxFile.AccessMode;
		public uint CachingMode => _pxFile.CachingMode;
		public uint LocationMode => _pxFile.LocationMode;


		[DllImport("PXUltima.dll")] private static extern unsafe byte PXFileDoesExistA(char* filePath);
		[DllImport("PXUltima.dll")] private static extern unsafe byte PXFileDoesExistW(char* filePath);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileRemoveA(char* filePath);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileRemoveW(char* filePath);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileRenameA(char* oldName, char* newName);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileRenameW(char* oldName, char* newName);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileCopyA(char* sourceFilePath, char* destinationFilePath);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileCopyW(char* sourceFilePath, char* destinationFilePath);

		//---<Set>-------------------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileBufferAllocate(ref PXFile pXFile, UIntPtr dataSize);
		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileBufferExternal(ref PXFile pXFile, void*  data, UIntPtr dataSize);
		//---------------------------------------------------------------------

		//---<Open>------------------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileOpenFromPathA(ref PXFile pXFile, char* filePath, int pxAccessMode, int pxMemoryCachingMode);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileOpenFromPathW(ref PXFile pXFile, char* filePath, int pxAccessMode, int pxMemoryCachingMode);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileOpenFromPathU(ref PXFile pXFile, char* filePath, int pxAccessMode, int pxMemoryCachingMode);
		//---------------------------------------------------------------------

		//---<Close>-----------------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileClose(ref PXFile pXFile);
		//---------------------------------------------------------------------

		//---<Mapping>---------------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileMapToMemory(ref PXFile pXFile, UIntPtr size,  int protectionMode);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileUnmapFromMemory(ref PXFile pXFile);
		//---------------------------------------------------------------------

		//---<Parsing Utility>-----------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileRemainingSize(ref PXFile pXFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileRemainingSizeRelativeFromAddress(ref PXFile pXFile,  void* adress);
		[DllImport("PXUltima.dll")] private static extern unsafe byte PXFileIsAtEnd(ref PXFile pXFile);

		[DllImport("PXUltima.dll")] private static extern unsafe void* PXFileCursorPosition(ref PXFile pXFile);
		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileCursorMoveTo(ref PXFile pXFile, UIntPtr position);
		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileCursorToBeginning(ref PXFile pXFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileCursorAdvance(ref PXFile pXFile, UIntPtr steps);
		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileCursorRewind(ref PXFile pXFile, UIntPtr steps);
		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileCursorToEnd(ref PXFile pXFile);
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipEndOfLineCharacters(ref PXFile pxFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipEmptySpace(ref PXFile pxFile);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadNextLineInto(ref PXFile pxFile, void* exportBuffer, UIntPtr exportBufferSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipBlock(ref PXFile pxFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipToNextBlock(ref PXFile pxFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipLine(ref PXFile pxFile);


		//[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileDataCopy(ref PXFile pxInputStream, PXFile* const pxOutputStream, const UIntPtr length);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadTextIU8(ref PXFile pxFile, ref byte number);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadTextI(ref PXFile pxFile, ref int number);


		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI8S(ref PXFile pxFile, ref byte value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI8SV(ref PXFile pxFile, ref byte[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI8U(ref PXFile pxFile, ref byte value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI8UV(ref PXFile pxFile, ref byte[]  valueList,  UIntPtr valueListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16S(ref PXFile pxFile, ref short  value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16SV(ref PXFile pxFile, ref short[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16SE(ref PXFile pxFile, ref short  value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16SVE(ref PXFile pxFile, ref short[] valueList,  UIntPtr valueListSize,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16U(ref PXFile pxFile, ref ushort  value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16UV(ref PXFile pxFile, ref ushort[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16UE(ref PXFile pxFile, ref ushort  value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI16UVE(ref PXFile pxFile, ref ushort[] valueList,  UIntPtr valueListSize,  Endian pxEndian);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32S(ref PXFile pxFile, ref int value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32SV(ref PXFile pxFile, ref int[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32SE(ref PXFile pxFile, ref int value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32SVE(ref PXFile pxFile, ref int[] valueList,  UIntPtr valueListSize,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32U(ref PXFile pxFile, ref uint value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32UV(ref PXFile pxFile, ref uint[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32UE(ref PXFile pxFile, ref uint value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI32UVE(ref PXFile pxFile, ref uint[] valueList,  UIntPtr valueListSize,  Endian pxEndian);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64S(ref PXFile pxFile, ref Int64 value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64SV(ref PXFile pxFile, ref Int64 valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64SE(ref PXFile pxFile, ref Int64 value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64VE(ref PXFile pxFile, ref Int64 valueList,  UIntPtr valueListSize,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64U(ref PXFile pxFile, ref UInt64 value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64UV(ref PXFile pxFile, ref UInt64 valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64UE(ref PXFile pxFile, ref UInt64 value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadI64UVE(ref PXFile pxFile, ref UInt64  valueList,  UIntPtr valueListSize,  Endian pxEndian);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadF(ref PXFile pxFile, ref float[] value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadFV(ref PXFile pxFile, ref float[] valueList, UIntPtr valueListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadD(ref PXFile pxFile, ref double[] value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadDV(ref PXFile pxFile, ref  double[] valueList, UIntPtr valueListSize);

		//[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadMultible(ref PXFile pxFile, PXFileDataElementType* pxFileElementList, UIntPtr pxFileElementListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadB(ref PXFile pxFile, void* value, UIntPtr length);

		// read Text
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadTextA(ref PXFile pxFile, char* value, UIntPtr length);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadTextW(ref PXFile pxFile, char* value, UIntPtr length);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadTextU(ref PXFile pxFile, char* value, UIntPtr length);

		[DllImport("PXUltima.dll")] private static extern unsafe void PXFileReadUntil(ref PXFile pxFile, void* value, UIntPtr length, byte character);



		// Additional check how many bytes are read.
		// Slower version of Read(), this function can't fail.
		//UIntPtr ReadSafe(Byte__* value,  UIntPtr length);

		[DllImport("PXUltima.dll")] private static extern unsafe byte PXFileReadAndCompareI64U(ref PXFile pxFile, UInt64 value);


		[DllImport("PXUltima.dll")] private static extern unsafe byte PXFileReadAndCompare(ref PXFile pxFile,  void* value,  UIntPtr length);
		[DllImport("PXUltima.dll")] private static extern unsafe byte PXFileReadAndCompareV(ref PXFile pxFile,  void** value,  UIntPtr* valueElementSizeList,  UIntPtr valueLength);
		//unsigned char PXFileReadAndCompareC(PXFile* PXFile,  char value);
		//unsigned char PXFileReadAndCompareIU(PXFile* PXFile,  unsigned int value);

		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI8S(ref PXFile pxFile, byte value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI8SV(ref PXFile pxFile, ref byte[] valueList, UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI8U(ref PXFile pxFile, byte value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteAtI8U(ref PXFile pxFile, byte value,  UIntPtr index);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI8UV(ref PXFile pxFile, ref byte[] valueList,  UIntPtr valueListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16S(ref PXFile pxFile, short value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16SV(ref PXFile pxFile, short[]  valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16SE(ref PXFile pxFile, short value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16SVE(ref PXFile pxFile, short[] valueList,  UIntPtr valueListSize,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16U(ref PXFile pxFile, ushort value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteAtI16U(ref PXFile pxFile, ushort value,  UIntPtr index);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16UV(ref PXFile pxFile, ushort[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16UE(ref PXFile pxFile, ushort value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI16UVE(ref PXFile pxFile,  ushort[] valueList,  UIntPtr valueListSize,  Endian pxEndian);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32S(ref PXFile pxFile,  int value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32SV(ref PXFile pxFile, int[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32SE(ref PXFile pxFile, int value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32SVE(ref PXFile pxFile, int[] valueList,  UIntPtr valueListSize,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32U(ref PXFile pxFile, uint value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteAtI32U(ref PXFile pxFile, uint value,  UIntPtr index);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32UV(ref PXFile pxFile, uint[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32UE(ref PXFile pxFile, uint value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteAtI32UE(ref PXFile pxFile, uint value,  Endian pxEndian,  UIntPtr index);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI32UVE(ref PXFile pxFile, uint[] valueList,  UIntPtr valueListSize,  Endian pxEndian);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64S(ref PXFile pxFile, Int64 value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64SV(ref PXFile pxFile, ref Int64[]  valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64SE(ref PXFile pxFile, Int64 value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64VE(ref PXFile pxFile, ref Int64[] valueList,  UIntPtr valueListSize,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64U(ref PXFile pxFile, UInt64 value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64UV(ref PXFile pxFile, ref UInt64[] valueList,  UIntPtr valueListSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64UE(ref PXFile pxFile, UInt64 value,  Endian pxEndian);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteI64UVE(ref PXFile pxFile, ref UInt64[] valueList,  UIntPtr valueListSize,  Endian pxEndian);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteF(ref PXFile pxFile,  float value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteFV(ref PXFile pxFile,  float*  valueList,  UIntPtr valueListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteD(ref PXFile pxFile,  double value);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteDV(ref PXFile pxFile,  double*  valueList,  UIntPtr valueListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteB(ref PXFile pxFile,  void*  value,  UIntPtr length);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteAtB(ref PXFile pxFile,  void*  data,  UIntPtr dataSize,  UIntPtr index);

		//[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteMultible(ref PXFile pxFile,  PXFileDataElementType*  PXFileElementList,  UIntPtr PXFileElementListSize);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteFill(ref PXFile pxFile, byte value,  UIntPtr length);

		// Write ASCII string \0 terminated.
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteA(ref PXFile pxFile,  char* text, UIntPtr textSize);

	// Write UNICODE string \0 terminated
	[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteW(ref PXFile pxFile, char* text, UIntPtr textSize);

	//[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteAF(ref PXFile pxFile, char* format, ...);
		//[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteWF(ref PXFile pxFile, char* format, ...);
		//-------------------------------------------------------------------------



		//---<Extra bit stuff>-----------------------------------------------------
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipBitsToNextByte(ref PXFile pxFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileCursorMoveBits(ref PXFile pxFile,  UIntPtr amountOfBits);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileBitsAllign(ref PXFile pxFile);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFilePeekBits(ref PXFile pxFile,  UIntPtr amountOfBits);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadBits(ref PXFile pxFile,  UIntPtr amountOfBits);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileWriteBits(ref PXFile pxFile,  UIntPtr bitData,  UIntPtr amountOfBits);
		//-------------------------------------------------------------------------


		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileFilePathGetA(ref PXFile pxFile, char*  filePath,  UIntPtr filePathMaxSize);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileFilePathGetW(ref PXFile pxFile, char*  filePath,  UIntPtr filePathMaxSize);




		public bool IsAtEnd { get => PXFileIsAtEnd(ref _pxFile) != 0x00; }

		public UIntPtr RemainingSize { get => PXFileRemainingSize(ref _pxFile); }


        public unsafe string FilePath 
		{
			get
			{
				string filePath = null;

				char[] buffser = new char[256];
				int length = 0;

				fixed(char* bufferAdress = buffser)
				{
                    length = (int)PXFileFilePathGetW(ref _pxFile, bufferAdress, (UIntPtr)256);

					if (length > 0)
					{
						filePath = new string(buffser, 0, length);
                    }
                }		


				return filePath;
			}
		}

        public static bool DoesExist(string filePath)
        {
			return false;
        }

		public ActionResult Remove(string filePath)
        {
			return ActionResult.Invalid;
        }

		public ActionResult Rename(string oldName, string newName)
		{
			return ActionResult.Invalid;
		}

		public ActionResult Copy(string sourceFilePath, string destinationFilePath)
		{
			return ActionResult.Invalid;
		}

		//---<Set>-------------------------------------------------------------
		public void BufferAllocate(UIntPtr size)
        {
			PXFileBufferAllocate(ref _pxFile, size);
        }
		//---------------------------------------------------------------------

		//---<Open>------------------------------------------------------------
		public unsafe ActionResult OpenFromPath(string filePath, int pxAccessMode, int pxMemoryCachingMode)
		{
			ActionResult actionResult = ActionResult.Invalid;

			fixed (char* adress = filePath.ToCharArray())
			{
				actionResult = (ActionResult)PXFileOpenFromPathW(ref _pxFile, adress, pxAccessMode, pxMemoryCachingMode);
			}

			return actionResult;
		}
		//---------------------------------------------------------------------

		//---<Close>-----------------------------------------------------------
		public ActionResult Close()
        {
			return (ActionResult)PXFileClose(ref _pxFile);
        }
		//---------------------------------------------------------------------

		//---<Parsing Utility>-----------------------------------------------------
		public void CursorMoveTo(ulong position)
		{
			PXFileCursorMoveTo(ref _pxFile, (UIntPtr)position);
		}
		public void CursorToBeginning()
		{
			PXFileCursorToBeginning(ref _pxFile);
		}
		public ulong CursorAdvance(ulong steps)
        {
			return (ulong)PXFileCursorAdvance(ref _pxFile, (UIntPtr)steps);
		}
		public void CursorRewind(ulong steps)
        {
			PXFileCursorRewind(ref _pxFile, (UIntPtr)steps);
		}
		public void CursorToEnd()
        {
			PXFileCursorToEnd(ref _pxFile);
		}
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		public ulong SkipEndOfLineCharacters()
        {
			return (ulong)PXFileSkipEndOfLineCharacters(ref _pxFile);
        }

		public ulong SkipEmptySpace()
        {
			return (ulong)PXFileSkipEmptySpace(ref _pxFile);
        }

		public unsafe ulong ReadNextLineInto( void* exportBuffer,  ulong exportBufferSize)
        {
			return (ulong)PXFileReadNextLineInto(ref _pxFile, exportBuffer, (UIntPtr)exportBufferSize);
        }

		public ulong SkipBlock()
		{
			return (ulong)PXFileSkipBlock(ref _pxFile);
		}

		public ulong SkipToNextBlock()
		{
			return (ulong)PXFileSkipToNextBlock(ref _pxFile);
		}

		public ulong SkipLine()
		{
			return (ulong)PXFileSkipLine(ref _pxFile);
		}


		//public ulong DataCopy(PXFile*  pxInputStream, PXFile*  pxOutputStream,  ulong length);

		public ulong ReadText( ref byte  number)
        {
			return (ulong)PXFileReadTextIU8(ref _pxFile, ref number);
        }

		public ulong ReadText( ref int  number)
        {
			return (ulong)PXFileReadTextI(ref _pxFile, ref number);
		}

		public ulong Read(ref byte  value)
        {
			return (ulong)PXFileReadI8U(ref _pxFile, ref value);
		}

		public ulong Read(ref byte[]  valueList,  ulong valueListSize)
        {
			return (ulong)PXFileReadI8UV(ref _pxFile, ref valueList, (UIntPtr)valueListSize);
        }

		public ulong Read( ref short  value)
		{
			return 0;
		}
		public ulong Read(ref short[] valueList,  ulong valueListSize)
        {
            return 0;
        }
        public ulong Read(ref short  value,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref short[] valueList,  ulong valueListSize,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref ushort  value)
        {
            return 0;
        }
        public ulong Read(ref ushort[] valueList,  ulong valueListSize)
        {
            return 0;
        }
        public ulong Read(ref ushort  value,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref ushort[] valueList,  ulong valueListSize,  Endian pxEndian)
        {
            return 0;
        }

        public ulong Read(ref int  value)
        {
            return 0;
        }
        public ulong Read(ref int[] valueList,  ulong valueListSize)
        {
            return 0;
        }
        public ulong Read(ref int  value,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref int[] valueList,  ulong valueListSize,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref uint  value)
        {
            return 0;
        }
        public ulong Read(ref uint[] valueList,  ulong valueListSize)
        {
            return 0;
        }
        public ulong Read(ref uint  value,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref uint[] valueList,  ulong valueListSize,  Endian pxEndian)
        {
            return 0;
        }

        public ulong Read(ref long  value)
        {
            return 0;
        }
        public ulong Read(ref long[] valueList,  ulong valueListSize)
        {
            return 0;
        }
        public ulong Read(ref long  value,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref long[] valueList,  ulong valueListSize,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref ulong  value)
        {
            return 0;
        }
        public ulong Read(ref ulong[] valueList,  ulong valueListSize)
        {
            return 0;
        }
        public ulong Read(ref ulong  value,  Endian pxEndian)
        {
            return 0;
        }
        public ulong Read(ref ulong[]  valueList,  ulong valueListSize,  Endian pxEndian)
        {
            return 0;
        }


        public ulong Read(ref float  value)
        {
            return 0;
        }
        public ulong Read(ref float[]  valueList,  ulong valueListSize)
        {
            return 0;
        }

        public ulong Read(ref double  value)
        {
            return 0;
        }
        public ulong Read(ref double[] valueList,  ulong valueListSize)
        {
            return 0;
        }

        //public ulong ReadMultible( DataElementType*  pxFileElementList,  ulong pxFileElementListSize);

        // read Text
        public ulong ReadText(string value,  ulong length)
        {
            return 0;
        }

        public ulong ReadUntil(ref byte[] value,  ulong length,  char character)
        {
            return 0;
        }

        public bool ReadAndCompareI64U(UIntPtr value)
        {
            return false;
        }
        /*
         * 
         * public bool ReadAndCompare(void* value, UIntPtr length)
        {
            return 0;
        }
        public bool ReadAndCompareV(void** value, UIntPtr* valueElementSizeList, UIntPtr valueLength)
        {
            return 0;
        }*/
        //-------------------------------------------------------------------------

        //-------------------------------------------------------------------------
        public ulong Write(byte value)
        {
			return (ulong)PXFileWriteI8U(ref _pxFile, value);
        }

		public ulong Write(byte[] valueList, UIntPtr valueListSize)
        {
			return (ulong)PXFileWriteI8UV(ref _pxFile, ref valueList, valueListSize);
		}

		public ulong Write(short value)
        {
            return 0;
        }
        public ulong Write(short[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }
        public ulong Write(short value, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(short[] valueList, UIntPtr valueListSize, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(ushort value)
		{
			return (ushort)PXFileWriteI16U(ref _pxFile, value);
		}
		public ulong Write(ushort value, UIntPtr index)
        {
			return (ushort)PXFileWriteI16U(ref _pxFile, value);
		}
		public ulong Write(ushort[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }
        public ulong Write(ushort value, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(ushort[] valueList, UIntPtr valueListSize, Endian pxEndian)
        {
            return 0;
        }

        public ulong Write(int value)
        {
            return 0;
        }
        public ulong Write(int[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }
        public ulong Write(int value, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(int[] valueList, UIntPtr valueListSize, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(uint value)
        {
            return 0;
        }
        public ulong Write(int value, UIntPtr index)
        {
            return 0;
        }
        public ulong Write(uint[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }
        public ulong Write(uint value, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(uint value, Endian pxEndian, UIntPtr index)
        {
            return 0;
        }
        public ulong Write(uint[] valueList, UIntPtr valueListSize, Endian pxEndian)
        {
            return 0;
        }

        public ulong Write(Int64 value)
        {
            return 0;
        }
        public ulong Write(Int64[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }
        public ulong Write(Int64 value, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(Int64[] valueList, UIntPtr valueListSize, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(UInt64 value)
        {
            return 0;
        }
        public ulong Write(UInt64[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }
        public ulong Write(UInt64 value, Endian pxEndian)
        {
            return 0;
        }
        public ulong Write(UInt64[] valueList, UIntPtr valueListSize, Endian pxEndian)
        {
            return 0;
        }

        public ulong Write( float value)
        {
            return 0;
        }
        public ulong Write( float[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }

        public ulong Write( double value)
        {
            return 0;
        }
        public ulong Write( double[] valueList, UIntPtr valueListSize)
        {
            return 0;
        }

        public ulong Write( ref byte[] value, UIntPtr length)
        {
            return 0;
        }
        public ulong WriteAt(ref byte[] data, UIntPtr dataSize, UIntPtr index)
        {
            return 0;
        }

        //public ulong WriteMultible( DataElementType* ElementList, UIntPtr ElementListSize)
   
        public ulong WriteFill( byte value, UIntPtr length)
        {
            return 0;
        }

        // Write ASCII string \0 terminated.
        public ulong Write(string text)
        {
            return 0;
        }

        //public UIntPtr WriteAF( PXTextASCII format, ...);
        //public UIntPtr WriteWF( PXTextUNICODE format, ...);
        //-------------------------------------------------------------------------



        //---<Extra bit stuff>-----------------------------------------------------
        public ulong SkipBitsToNextByte()
		{
            return 0;// (ulong)PXFileSkipToNextByte(ref _pxFile);
		}
		public ulong CursorMoveBits(ulong amountOfBits)
		{
			return (ulong)PXFileCursorMoveBits(ref _pxFile, (UIntPtr)amountOfBits);
		}
		public ulong BitsAllign()
		{
			return (ulong)PXFileBitsAllign(ref _pxFile);
		}

		public ulong PeekBits(ulong amountOfBits)
		{
			return (ulong)PXFilePeekBits(ref _pxFile, (UIntPtr)amountOfBits);
		}
		public ulong ReadBits(ulong amountOfBits)
		{
            return (ulong)PXFileReadBits(ref _pxFile, (UIntPtr)amountOfBits);
		}

		public ulong WriteBits(ulong bitData, ulong amountOfBits)
		{
            return (ulong)PXFileWriteBits(ref _pxFile, (UIntPtr)bitData, (UIntPtr)amountOfBits);
        }
		//-------------------------------------------------------------------------	
	}
}