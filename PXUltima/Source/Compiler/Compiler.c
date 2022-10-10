#include "Compiler.h"

#include <stdio.h>

#include <File/DataStream.h>
#include <Text/Text.h>



void PXCompilerSymbolEntryAdd(DataStream* const dataStream, PXCompilerSymbolLexer symbol, unsigned int coloum, unsigned int line, unsigned int size, char* source)
{
	unsigned int symbolID = symbol;

	DataStreamWriteIU(dataStream, symbolID, EndianLittle);
	DataStreamWriteIU(dataStream, coloum, EndianLittle);
	DataStreamWriteIU(dataStream, line, EndianLittle);
	DataStreamWriteIU(dataStream, size, EndianLittle);
	DataStreamWriteP(dataStream, &source, sizeof(void*));

	char textbuffer[128];

	MemorySet(textbuffer, sizeof(textbuffer), 0);

	switch (symbolID)
	{
		case PXCompilerSymbolLexerWhiteSpaceID:
		{
			TextCopyA("***Whitespace***", 17, textbuffer, 128);
			break;
		}
		case PXCompilerSymbolLexerNewLineID:
		{
			TextCopyA("***New Line***", 15, textbuffer, 128);
			break;
		}
		case PXCompilerSymbolLexerElementID:
		{
			TextCopyA(source, size, textbuffer, 128);
			break;
		}
		default:
			break;
	}

#if 0
	printf
	(
		"[%8x] C:%-2i L:%-2i S:%-2i D:%p -> %s\n",
		id,
		coloum,
		line,
		size,
		source,
		textbuffer
	);
#endif

}

void PXCompilerSymbolEntryExtract(DataStream* const dataStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{
	void* const oldPos = DataStreamCursorPosition(dataStream);
	size_t size = 0;

	unsigned int symbolID = 0;

	size += DataStreamReadIU(dataStream, &symbolID, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Coloum, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Line, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Size, EndianLittle);
	size += DataStreamReadP(dataStream, &compilerSymbolEntry->Source, sizeof(void*));

	compilerSymbolEntry->ID = symbolID;

	MemorySet(oldPos, size, '#');
}

PXCompilerSymbolLexer PXCompilerTryAnalyseType(const char* const text, const size_t textSize)
{
	if (textSize == 1)
	{
		return PXCompilerSymbolLexerSingleCharacter;
	}

	switch (text[0])
	{
		case 'T':
		case 't':
		{
			const PXBool  result = TextCompareIgnoreCaseA(text, textSize, "true", 4);

			if (result)
			{
				return PXCompilerSymbolLexerTrue;
			}

			break;
		}		
		case 'F':
		case 'f':
		{
			const PXBool  result = TextCompareIgnoreCaseA(text, textSize, "false", 5);

			if (result)
			{
				return PXCompilerSymbolLexerFalse;
			}

			break;
		}
		case '+':
		case '-':
		case 0u:
		case 1u:
		case 2u:
		case 3u:
		case 4u:
		case 5u:
		case 6u:
		case 7u:
		case 8u:
		case 9u:
		{
			// Probe for number
			const size_t dotIndex = TextFindFirstA(text, textSize, '.');
			const PXBool probablyFloat = dotIndex != (size_t)-1;
			size_t writtenNumbers = 0;

			if (probablyFloat)
			{
				float number = 0;
				const size_t writtenNumbers = TextToFloatA(text, textSize, &number);

				if (writtenNumbers)
				{
					return PXCompilerSymbolLexerFloat;
				}
			}
			else
			{
				int number = 0;
				const size_t 	writtenNumbers = TextToIntA(text, textSize, &number);

				if (writtenNumbers)
				{
					return PXCompilerSymbolLexerInteger;
				}
			}
		}
	}

	return PXCompilerSymbolLexerGenericElement;
}

void PXCompilerLexicalAnalysis(DataStream* const inputStream, DataStream* const outputStream, const PXCompilerSettings* const compilerSettings)
{
	size_t currentLine = 0;
	size_t currentColoum = 0;
	PXBool isFirstWhiteSpaceInLine = 1u;

	const PXCompilerSymbolLexer newLineSymbol = compilerSettings->IntrepredNewLineAsWhiteSpace ? PXCompilerSymbolLexerWhiteSpace : PXCompilerSymbolLexerNewLine;

	while (!DataStreamIsAtEnd(inputStream))
	{
		// Consume whitespace
		{
			char* const whiteSpaceSource = (char* const)DataStreamCursorPosition(inputStream);
			const size_t whiteSpaceSize = DataStreamSkipEmptySpace(inputStream); 

			currentColoum += whiteSpaceSize;

			if (whiteSpaceSize && (compilerSettings->KeepWhiteSpace || isFirstWhiteSpaceInLine))
			{
				isFirstWhiteSpaceInLine = 0;

				PXCompilerSymbolEntryAdd
				(
					outputStream,
					PXCompilerSymbolLexerWhiteSpace,
					currentColoum,
					currentLine,
					whiteSpaceSize,
					whiteSpaceSource
				);
			}
		}

		// Consume new line
		{
			char* const endofLineSource = (char* const)DataStreamCursorPosition(inputStream);
			const size_t endofLineSize = DataStreamSkipEndOfLineCharacters(inputStream);

			if (endofLineSize)
			{			
				const size_t linesSkipped = TextCountA(endofLineSource, endofLineSize, '\n');

				PXCompilerSymbolEntryAdd
				(
					outputStream,
					newLineSymbol,
					currentColoum,
					currentLine,
					endofLineSize,
					endofLineSource
				);

				isFirstWhiteSpaceInLine = 1u;

				currentColoum = 0; // Reset, next entry will begin in new line
				currentLine += linesSkipped;
			}
		}	

		// Consume block
		{
			char* const blockSpaceSource = (char* const)DataStreamCursorPosition(inputStream);
			const size_t blockSize = DataStreamSkipBlock(inputStream);	// consume block

			currentColoum += blockSize;

			if (blockSize)
			{
				PXCompilerSymbolLexer symbol = PXCompilerSymbolLexerGenericElement;

				if (compilerSettings->TryAnalyseTypes)
				{
					symbol = PXCompilerTryAnalyseType(blockSpaceSource, blockSize);
				}

				PXCompilerSymbolEntryAdd
				(
					outputStream,
					symbol,
					currentColoum,
					currentLine,
					blockSize,
					blockSpaceSource
				);
			}
		}
	}
}