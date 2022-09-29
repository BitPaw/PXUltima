#include "Compiler.h"

#include <stdio.h>

#include <File/DataStream.h>
#include <Text/Text.h>

void PXCompilerSymbolEntryAdd(DataStream* const dataStream, unsigned int id, unsigned int coloum, unsigned int line, unsigned int size, char* source)
{
	DataStreamWriteIU(dataStream, id, EndianLittle);
	DataStreamWriteIU(dataStream, coloum, EndianLittle);
	DataStreamWriteIU(dataStream, line, EndianLittle);
	DataStreamWriteIU(dataStream, size, EndianLittle);
	DataStreamWriteD(dataStream, &source, sizeof(void*));

	char textbuffer[128];

	MemorySet(textbuffer, sizeof(textbuffer), 0);

	switch (id)
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

	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->ID, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Coloum, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Line, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Size, EndianLittle);
	size += DataStreamReadD(dataStream, &compilerSymbolEntry->Source, sizeof(void*));

	MemorySet(oldPos, size, '#');
}

void PXCompilerLexicalAnalysis(DataStream* const inputStream, DataStream* const outputStream, const PXCompilerSettings* const compilerSettings)
{
	size_t currentLine = 0;
	size_t currentColoum = 0;

	while (!DataStreamIsAtEnd(inputStream))
	{
		// Consume whitespace
		{
			char* const whiteSpaceSource = (char* const)DataStreamCursorPosition(inputStream);
			const size_t whiteSpaceSize = DataStreamSkipEmptySpace(inputStream); 

			currentColoum += whiteSpaceSize;

			if (whiteSpaceSize && compilerSettings->WhiteSpaceKeep)
			{
				PXCompilerSymbolEntryAdd
				(
					outputStream,
					PXCompilerSymbolLexerWhiteSpaceID,
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
					PXCompilerSymbolLexerNewLineID,
					currentColoum,
					currentLine,
					endofLineSize,
					endofLineSource
				);

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
				PXCompilerSymbolEntryAdd
				(
					outputStream,
					PXCompilerSymbolLexerElementID,
					currentColoum,
					currentLine,
					blockSize,
					blockSpaceSource
				);		
			}
		}
	}
}