using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Library
    {
        internal struct PXLibrary
        {
            public UIntPtr ID;
            public UIntPtr Handle;
        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXLibraryOpen(ref PXLibrary pxLibrary, ref PXText filePath); //  gain access to an executable object file. RTLD_LAZY
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXLibraryClose(ref PXLibrary pxLibrary); // close a dlopen object
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern byte PXLibraryGetSymbol(ref PXLibrary pxLibrary, UIntPtr libraryFunction, ref PXText symbolName); // obtain the address of a symbol from a dlopen object

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXLibraryName(ref PXLibrary pxLibrary, ref PXText libraryName);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXLibraryParseSymbols(ref PXText libraryFilePath, UIntPtr pxSymbolDetectedEvent);
   
        private PXLibrary _pxLibrary = new PXLibrary();

        public unsafe ActionResult Open(string filePath)
        {
            if (filePath == null) return ActionResult.RefuedParameterNull;

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
