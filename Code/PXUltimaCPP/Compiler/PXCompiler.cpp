#include "PXCompiler.hpp"

PX::Compiler::Compiler()
{
}

PX::Compiler::~Compiler()
{
}

void PX::Compiler::LexicalAnalysis(const CompilerSettings& compilerSettings)
{
	PXCompilerLexicalAnalysis(this, &compilerSettings);
}

void PX::Compiler::SymbolEntryAdd(CompilerSymbolEntry& compilerSymbolEntry)
{
	PXCompilerSymbolEntryAdd(this, &compilerSymbolEntry);
}

void PX::Compiler::SymbolEntryExtract()
{
	PXCompilerSymbolEntryExtract(this);
}

void PX::Compiler::SymbolEntryPeek()
{
	PXCompilerSymbolEntryPeek(this);
}

PXSize PXAPI PX::Compiler::SymbolRewind(const PXSize amount)
{
	return PXCompilerSymbolRewind(this, amount);
}

PXSize PXAPI PX::Compiler::SymbolEntryForward()
{
	return PXCompilerSymbolEntryForward(this);
}

PXSize PXAPI PX::Compiler::SymbolEntryMergeCurrentWithNext()
{
	return PXCompilerSymbolEntryMergeCurrentWithNext(this, nullptr);
}