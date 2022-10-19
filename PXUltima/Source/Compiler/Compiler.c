#include "Compiler.h"

#include <stdio.h>

#include <File/DataStream.h>
#include <Text/Text.h>
#include <Math/Math.h>

void PXCompilerSymbolEntryAdd(DataStream* const dataStream, const PXCompilerSymbolEntry* const compilerSymbolEntry)
{
	DataStreamWriteCU(dataStream, compilerSymbolEntry->ID, EndianLittle);
	DataStreamWriteIU(dataStream, compilerSymbolEntry->Coloum, EndianLittle);
	DataStreamWriteIU(dataStream, compilerSymbolEntry->Line, EndianLittle);
	DataStreamWriteIU(dataStream, compilerSymbolEntry->Size, EndianLittle);
	DataStreamWriteP(dataStream, &compilerSymbolEntry->Source, sizeof(void*));

	size_t idBufferSize = 10;
	char idbuffer[15];
	size_t textBufferSize = 20;
	char textbuffer[25];

	MemorySet(idbuffer, sizeof(idbuffer), 0);
	MemorySet(textbuffer, sizeof(textbuffer), 0);

	switch (compilerSymbolEntry->ID)
	{
		case PXCompilerSymbolLexerWhiteSpace:
		{
			//TextCopyA("***Whitespace***", 17, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerNewLine:
		{
			TextCopyA("\\n", 3, idbuffer, idBufferSize);
			//TextCopyA("***New Line***", 15, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerComment:
			TextCopyA("comment", 8, idbuffer, idBufferSize);
			TextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, textbuffer, textBufferSize);
			break;

		case PXCompilerSymbolLexerBool:
			TextCopyA("bool", 5, idbuffer, idBufferSize);
			sprintf(textbuffer, "%s", compilerSymbolEntry->DataC ? "true" : "false");
			break;

		case PXCompilerSymbolLexerFloat:
			TextCopyA("float", 6, idbuffer, idBufferSize);
			sprintf(textbuffer, "%f", compilerSymbolEntry->DataF);
			break;

		case PXCompilerSymbolLexerInteger:
			TextCopyA("int", 4, idbuffer, idBufferSize);
			sprintf(textbuffer, "%i", compilerSymbolEntry->DataI);
			break;

		case PXCompilerSymbolLexerString:
			TextCopyA("string", 7, idbuffer, idBufferSize);
			TextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, textbuffer, textBufferSize);
			break;

		case PXCompilerSymbolLexerGenericElement:
		{
			TextCopyA("*Generic*", 10, idbuffer, idBufferSize);
			TextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, textbuffer, textBufferSize);
			break;
		}
	}

#if 0
	printf
	(
		"|| C:%-2i L:%-2i S:%-2i | %-10s -> %-20s ||\n",
		compilerSymbolEntry->Coloum,
		compilerSymbolEntry->Line,
		compilerSymbolEntry->Size,
		idbuffer,
		textbuffer
	);	
#endif
}

void PXCompilerSymbolEntryExtract(DataStream* const dataStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{
	void* const oldPos = DataStreamCursorPosition(dataStream);
	size_t size = 0;

	unsigned char symbolID = 0;

	size += DataStreamReadCU(dataStream, &symbolID);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Coloum, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Line, EndianLittle);
	size += DataStreamReadIU(dataStream, &compilerSymbolEntry->Size, EndianLittle);
	size += DataStreamReadP(dataStream, &compilerSymbolEntry->Source, sizeof(void*));

	compilerSymbolEntry->ID = symbolID;

	MemorySet(oldPos, size, '#');
}

PXCompilerSymbolLexer PXCompilerTryAnalyseType(const char* const text, const size_t textSize, PXCompilerSymbolEntry* const compilerSymbolEntry)
{
	switch (text[0])
	{
		case '\'':
		case '\"':
		{
			const PXBool isFull = text[textSize - 1] == '\'' || text[textSize - 1] == '\"';

			if (isFull)
			{
				return PXCompilerSymbolLexerString;
			}

			return PXCompilerSymbolLexerStringBegin;
		}
		case 'T':
		case 't':
		{
			const PXBool  result = TextCompareIgnoreCaseA(text, textSize, "true", 4) && textSize > 4;

			if (result)
			{
				compilerSymbolEntry->DataC = PXYes;

				return PXCompilerSymbolLexerBool;
			}

			break;
		}
		case 'F':
		case 'f':
		{
			const PXBool  result = TextCompareIgnoreCaseA(text, textSize, "false", 5) && textSize > 5;

			if (result)
			{
				compilerSymbolEntry->DataC = PXNo;

				return PXCompilerSymbolLexerBool;
			}

			break;
		}
		case '+':
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			// Probe for number
			const size_t dotIndex = TextFindFirstA(text, textSize, '.');
			const PXBool probablyFloat = dotIndex != (size_t)-1;
			size_t writtenNumbers = 0;			

			if (probablyFloat)
			{
				float value = 0;

				const size_t writtenNumbers = TextToFloatA(text, textSize, &value);
				const PXBool isFloat = textSize == writtenNumbers;

				if (isFloat)
				{
					compilerSymbolEntry->Source = 0;
					compilerSymbolEntry->DataF = value;

					return PXCompilerSymbolLexerFloat;
				}
			}
			else
			{
				unsigned int value = 0;

				const size_t writtenNumbers = TextToIntA(text, textSize, &value);
				const PXBool isInteger = textSize == writtenNumbers;

				if (isInteger)
				{
					compilerSymbolEntry->Source = 0;
					compilerSymbolEntry->DataI = value;

					return PXCompilerSymbolLexerInteger;
				}
			}
		}
	}

	const PXBool isEndOfString = (text[textSize - 1] == '\'') || (text[textSize - 1] == '\"');

	if (isEndOfString)
	{
		return PXCompilerSymbolLexerStringEnd;
	}

	return PXCompilerSymbolLexerGenericElement;
}

void PXCompilerLexicalAnalysis(DataStream* const inputStream, DataStream* const outputStream, const PXCompilerSettings* const compilerSettings)
{
	size_t currentLine = 0;
	size_t currentColoum = 0;
	PXBool isFirstWhiteSpaceInLine = 1u;

	const PXCompilerSymbolLexer newLineSymbol = compilerSettings->IntrepredNewLineAsWhiteSpace ? PXCompilerSymbolLexerWhiteSpace : PXCompilerSymbolLexerNewLine;

	printf("||=====================================================||\n");

	while (!DataStreamIsAtEnd(inputStream))
	{
		PXCompilerSymbolEntry compilerSymbolEntry;
		compilerSymbolEntry.Source = (const char*)DataStreamCursorPosition(inputStream);	

		// Consume whitespace
		{
			const size_t whiteSpaceSize = DataStreamSkipEmptySpace(inputStream);

			currentColoum += whiteSpaceSize;

			if (whiteSpaceSize)
			{
				isFirstWhiteSpaceInLine = 0;

				compilerSymbolEntry.ID = PXCompilerSymbolLexerWhiteSpace;
				compilerSymbolEntry.Line = currentLine;
				compilerSymbolEntry.Coloum = currentColoum;
				compilerSymbolEntry.Size = whiteSpaceSize;

				if (compilerSettings->KeepWhiteSpace || isFirstWhiteSpaceInLine)
				{
					PXCompilerSymbolEntryAdd(outputStream, &compilerSymbolEntry);
				}	

				continue;
			}
		}

		// Consume new line
		{
			const size_t endofLineSize = DataStreamSkipEndOfLineCharacters(inputStream);

			if (endofLineSize)
			{
				const size_t linesSkipped = TextCountA(compilerSymbolEntry.Source, endofLineSize, '\n');

				compilerSymbolEntry.ID = newLineSymbol;
				compilerSymbolEntry.Line = currentLine;
				compilerSymbolEntry.Coloum = currentColoum;
				compilerSymbolEntry.Size = endofLineSize;

				isFirstWhiteSpaceInLine = 1u;

				currentColoum = 0; // Reset, next entry will begin in new line
				currentLine += linesSkipped;

				PXCompilerSymbolEntryAdd(outputStream, &compilerSymbolEntry);

				continue;
			}
		}

		// Consume block
		{
			compilerSymbolEntry.ID = PXCompilerSymbolLexerGenericElement;
			compilerSymbolEntry.Line = currentLine;
			compilerSymbolEntry.Coloum = currentColoum;
			compilerSymbolEntry.Size = DataStreamSkipBlock(inputStream);	// consume block

			currentColoum += compilerSymbolEntry.Size;

			if (compilerSymbolEntry.Size)
			{
				if (compilerSettings->TryAnalyseTypes)
				{
					compilerSymbolEntry.ID = PXCompilerTryAnalyseType(compilerSymbolEntry.Source, compilerSymbolEntry.Size, &compilerSymbolEntry);		

					switch (compilerSymbolEntry.ID)
					{
						case PXCompilerSymbolLexerGenericElement:
						{
							if (compilerSettings->CommentSingleLineSize)
							{
								const PXBool isComment = TextCompareA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, compilerSettings->CommentSingleLine, compilerSettings->CommentSingleLineSize);

								if (isComment)
								{
									compilerSymbolEntry.ID = PXCompilerSymbolLexerComment;
									compilerSymbolEntry.Size += DataStreamSkipLine(inputStream);
								}
							}

							break;
						}

						case PXCompilerSymbolLexerStringBegin:
						{
							++compilerSymbolEntry.Source;

							const size_t blockSizeCUr = DataStreamRemainingSize(inputStream) + compilerSymbolEntry.Size;
							const size_t symbolPositionApostrophe = TextFindFirstA(compilerSymbolEntry.Source, blockSizeCUr, '\'');
							const size_t symbolPositionQuotationMark = TextFindFirstA(compilerSymbolEntry.Source, blockSizeCUr, '\"');
							const size_t symbolPosition = MathMinimumIU(symbolPositionApostrophe, symbolPositionQuotationMark);
							const PXBool hasIndex = symbolPosition != (size_t)-1;

							if (!hasIndex)
							{
								// Error
							}
							DataStreamCursorAdvance(inputStream, symbolPosition);

							compilerSymbolEntry.Size = symbolPosition;
							compilerSymbolEntry.ID = PXCompilerSymbolLexerString;

							break;
						}
						case PXCompilerSymbolLexerString:
						{
							compilerSymbolEntry.Source += 1u;
							compilerSymbolEntry.Size -= 2u;;

							break;
						}

						default:
							break;
					}
				}

				PXCompilerSymbolEntryAdd(outputStream, &compilerSymbolEntry);

				continue;
			}
		}
	}

	printf("||=====================================================||\n");
}