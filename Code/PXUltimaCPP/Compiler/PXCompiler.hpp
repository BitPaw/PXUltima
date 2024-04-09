#pragma once

#include <Compiler/PXCompiler.h>

class CompilerSymbolEntry : public PXCompilerSymbolEntry
{

};

class CompilerSettings : public PXCompilerSettings
{

};

namespace PX
{
	class Compiler : public PXCompiler
	{
		public:
		Compiler();
		~Compiler();

		void PXAPI LexicalAnalysis(const CompilerSettings& compilerSettings);

		void PXAPI SymbolEntryAdd(CompilerSymbolEntry& compilerSymbolEntry);
		void PXAPI SymbolEntryExtract();
		void PXAPI SymbolEntryPeek();
		PXSize PXAPI SymbolRewind(const PXSize amount);
		PXSize PXAPI SymbolEntryForward();
		PXSize PXAPI SymbolEntryMergeCurrentWithNext();
	};
}