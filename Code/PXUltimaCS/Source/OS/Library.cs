using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Library
    {
        internal unsafe struct PXLibrary
        {
            public void* ID;
            public void* Handle;
        }

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXLibraryOpen(ref PXLibrary pxLibrary, ref PXText filePath); //  gain access to an executable object file. RTLD_LAZY
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXLibraryClose(ref PXLibrary pxLibrary); // close a dlopen object
        [DllImport("PXUltima.dll")] private static extern byte PXLibraryGetSymbol(ref PXLibrary pxLibrary, UIntPtr libraryFunction, ref PXText symbolName); // obtain the address of a symbol from a dlopen object

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXLibraryName(ref PXLibrary pxLibrary, ref PXText libraryName);

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXLibraryParseSymbols(ref PXText libraryFilePath, UIntPtr pxSymbolDetectedEvent);
   
        private PXLibrary _pxLibrary = new PXLibrary();

        public unsafe ActionResult Open(string filePath)
        {
            fixed (char* messageAdress = filePath.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(messageAdress, filePath.Length);

                return PXLibraryOpen(ref _pxLibrary, ref pXText);
            }      
        }

        public ActionResult Close() 
        {
            return PXLibraryClose(ref _pxLibrary);
        }
    }
}
