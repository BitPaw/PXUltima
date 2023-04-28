using System;
using System.Runtime.InteropServices;

namespace PX
{
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
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileMapToMemoryA(ref PXFile pXFile,  char* filePath, UIntPtr fileSize, int protectionMode);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileMapToMemoryW(ref PXFile pXFile, char* filePath, UIntPtr fileSize, int protectionMode);
		[DllImport("PXUltima.dll")] private static extern unsafe int PXFileMapToMemoryU(ref PXFile pXFile, char* filePath, UIntPtr fileSize, int protectionMode);
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
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipEndOfLineCharacters(ref PXFile pXFile);
		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileSkipEmptySpace(ref PXFile pXFile);

		[DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXFileReadNextLineInto(ref PXFile pXFile, void* exportBuffer, UIntPtr exportBufferSize);


		public bool IsAtEnd { get => PXFileIsAtEnd(ref _pxFile) != 0x00; }

		public UIntPtr RemainingSize { get => PXFileRemainingSize(ref _pxFile); }


	


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
		//public PXSize SkipEndOfLineCharacters();
		//public PXSize SkipEmptySpace();

		//public PXSize ReadNextLineInto(void* exportBuffer, PXSize exportBufferSize);
	}
}