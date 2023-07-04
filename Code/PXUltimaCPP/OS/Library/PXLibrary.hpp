#ifndef PXCPPLibraryINCLUDE
#define PXCPPLibraryINCLUDE

#include <OS/Library/PXLibrary.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
    class Library : public PXLibrary
    {
        public:
        PXDLLExport Library();
        PXDLLExport Library(const Text& const filePath);
        PXDLLExport ~Library();

        PXDLLExport bool Open(const Text& const filePath); //  gain access to an executable object file. RTLD_LAZY
        PXDLLExport bool Close(); // close a dlopen object
        PXDLLExport bool GetSymbol(LibraryFunction* libraryFunction, const Text& symbolName); // obtain the address of a symbol from a dlopen object

        PXDLLExport PX::ActionResult Name(Text& const libraryName);

        PXDLLExport bool ParseSymbols();
    };
}
#endif