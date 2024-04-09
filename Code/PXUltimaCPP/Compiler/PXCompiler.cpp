#include "PXCompiler.hpp"

PX::Compiler::Compiler()
{
}

PX::Compiler::~Compiler()
{
}

void PX::Compiler::LexicalAnalysis(const CompilerSettings& compilerSettings)
{
}

void PX::Compiler::SymbolEntryAdd(CompilerSymbolEntry& compilerSymbolEntry)
{
}

void PX::Compiler::SymbolEntryExtract()
{
}

void PX::Compiler::SymbolEntryPeek()
{
}

PXSize PXAPI PX::Compiler::SymbolRewind(const PXSize amount)
{
	return PXCompilerSymbolRewind(, amount);
}

PXSize PXAPI PX::Compiler::SymbolEntryForward()
{
	return 0;
}

PXSize PXAPI PX::Compiler::SymbolEntryMergeCurrentWithNext()
{
	return 0;
}