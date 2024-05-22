#include "PXCompiler.h"

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>

#define PXCompilerDebug 1
#define PXCompilerSanitise 1

#if PXCompilerDebug
#include <stdio.h>
#endif

const char* PXAPI PXCompilerCompilerSymbolLexerToString(const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
	switch(pxCompilerSymbolLexer)
	{
		case PXCompilerSymbolLexerWhiteSpace: return "WhiteSpace";
		case PXCompilerSymbolLexerNewLine: return "NewLine";
		case PXCompilerSymbolLexerTab: return "Tab";
		case PXCompilerSymbolLexerGeneric: return "Generic";
		case PXCompilerSymbolLexerSingleCharacter: return "SingleCharacter";
		case PXCompilerSymbolLexerBrackedRoundOpen: return "(";
		case PXCompilerSymbolLexerBrackedRoundClose: return ")";
		case PXCompilerSymbolLexerBracketSquareOpen: return "[";
		case PXCompilerSymbolLexerBracketSquareClose: return "]";
		case PXCompilerSymbolLexerBracketCurlyOpen: return "{";
		case PXCompilerSymbolLexerBracketCurlyClose: return "}";
		case PXCompilerSymbolLexerBracketAngleOpen: return "<";
		case PXCompilerSymbolLexerBracketAngleClose: return ">";
		case PXCompilerSymbolLexerBiggerAndEqual: return "<=";
		case PXCompilerSymbolLexerSmalerAndEqual: return ">=";
		case PXCompilerSymbolLexerEqual: return "=";
		case PXCompilerSymbolLexerEqualDouble: return "==";
		case PXCompilerSymbolLexerEqualTrippel: return "===";
		case PXCompilerSymbolLexerCompareThreeWay: return "<=>";
		case PXCompilerSymbolLexerQuestionmark: return "?";
		case PXCompilerSymbolLexerExclamation: return "!";
		case PXCompilerSymbolLexerDot: return ".";
		case PXCompilerSymbolLexerComma: return ",";
		case PXCompilerSymbolLexerColon: return ":";
		case PXCompilerSymbolLexerSemiColon: return ";";
		case PXCompilerSymbolLexerHash: return "#";
		case PXCompilerSymbolLexerPlus: return "+";
		case PXCompilerSymbolLexerMinus: return "-";
		case PXCompilerSymbolLexerSlash: return "/";
		case PXCompilerSymbolLexerAsterisk: return "*";
		case PXCompilerSymbolLexerSlashBack: return "\\";
		case PXCompilerSymbolLexerAmpercant: return "&";
		case PXCompilerSymbolLexerPercent: return "%";
		case PXCompilerSymbolLexerBar: return "|";
		case PXCompilerSymbolLexerDegree: return "°";
		case PXCompilerSymbolLexerExponent: return "^";
		case PXCompilerSymbolLexerTilde: return "~";
		case PXCompilerSymbolLexerApostrophe: return "";
		case PXCompilerSymbolLexerComment: return "Comment";
		case PXCompilerSymbolLexerBool: return "bool";
		case PXCompilerSymbolLexerFloat: return "float";
		case PXCompilerSymbolLexerInteger: return "int";
		case PXCompilerSymbolLexerString: return "string";
		case PXCompilerSymbolLexerStringBegin: return "string beginning";
		case PXCompilerSymbolLexerStringEnd: return "string ending";
		case PXCompilerSymbolLexerEndOfFile: return "EOF";

		default:
			return PXNull;
	}
}

void PXAPI PXCompilerSymbolEntryAdd(PXCompiler* const pxCompiler, const PXCompilerSymbolEntry* const compilerSymbolEntry)
{
	const PXInt8U symbolID = compilerSymbolEntry->ID;

	const PXFileDataElementType pxFileDataElementType[] =
	{
		&symbolID, PXDataTypeInt08U,
		&compilerSymbolEntry->Coloum, PXDataTypeInt32U,
		&compilerSymbolEntry->Line, PXDataTypeInt32U,
		&compilerSymbolEntry->Size, PXDataTypeInt32U,
		&compilerSymbolEntry->Source, PXDataTypeAdress
	};

	const PXSize written = PXFileWriteMultible(pxCompiler->FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));

#if PXCompilerDebug
	const char* typeName = PXCompilerCompilerSymbolLexerToString(compilerSymbolEntry->ID);

	switch(compilerSymbolEntry->ID)
	{
		case PXCompilerSymbolLexerFloat:
		{
			PXLogPrint
			(
				PXLoggingInfo,
				"Compiler",
				"Entry",
				"L:%-4i C:%-4i S:%-4i (%s) %6.2f",
				compilerSymbolEntry->Line,
				compilerSymbolEntry->Coloum,
				compilerSymbolEntry->Size,
				typeName,
				compilerSymbolEntry->DataF
			);

			break;
		}

		case PXCompilerSymbolLexerInteger:
		{
			PXLogPrint
			(
				PXLoggingInfo,
				"Compiler",
				"Entry",
				"L:%-4i C:%-4i S:%-4i (%s) %i",
				compilerSymbolEntry->Line,
				compilerSymbolEntry->Coloum,
				compilerSymbolEntry->Size,
				typeName,
				compilerSymbolEntry->DataI32U
			);

			break;
		}
		case PXCompilerSymbolLexerComment:
		case PXCompilerSymbolLexerGeneric:
		{
			char buffer[32];
			PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 21);

			PXLogPrint
			(
				PXLoggingInfo,
				"Compiler",
				"Entry",
				"L:%-4i C:%-4i S:%-4i (%s) §6%s",
				compilerSymbolEntry->Line,
				compilerSymbolEntry->Coloum,
				compilerSymbolEntry->Size,
				typeName,
				buffer
			);
			break;
		}
		default:
			PXLogPrint
			(
				PXLoggingInfo,
				"Compiler",
				"Entry",
				"L:%-4i C:%-4i S:%-4i (%s)",
				compilerSymbolEntry->Line,
				compilerSymbolEntry->Coloum,
				compilerSymbolEntry->Size,
				typeName
			);
			break;
	}
#endif

#if PXCompilerDebug && 0

	PXSize idBufferSize = 10;
	char idbuffer[15];
	PXSize textBufferSize = 20;
	char textbuffer[25];

	PXMemoryClear(idbuffer, sizeof(idbuffer));
	PXMemoryClear(textbuffer, sizeof(textbuffer));

	switch (compilerSymbolEntry->ID)
	{
		case PXCompilerSymbolLexerQuestionmark:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("?", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerExclamation:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("!", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerDot:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA(".", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerComma:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA(",", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerColon:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA(":", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerSemiColon:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA(";", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerPlus:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("+", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerMinus:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("-", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerSlash:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("/", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerAsterisk:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("*", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerSlashBack:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("\\", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerAmpercant:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("&", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerPercent:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("%", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBar:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("|", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerHash:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("#", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerDegree:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("°", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerExponent:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("^", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerTilde:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("~", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerApostrophe:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("\'", 1, textbuffer, textBufferSize);
			break;
		}

		case PXCompilerSymbolLexerBrackedRoundOpen:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("(", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBrackedRoundClose:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA(")", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBracketSquareOpen:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("[", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBracketSquareClose:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("]", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBracketCurlyOpen:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("{", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBracketCurlyClose:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("}", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBracketAngleOpen:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA("<", 1, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerBracketAngleClose:
		{
			PXTextCopyA("*Symbol*", 9, idbuffer, idBufferSize);
			PXTextCopyA(">", 1, textbuffer, textBufferSize);
			break;
		}

		case PXCompilerSymbolLexerWhiteSpace:
		{
			PXTextCopyA("***Whitespace***", 17, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerNewLine:
		{
			PXTextCopyA("\\n", 3, idbuffer, idBufferSize);
			PXTextCopyA("***New Line***", 15, textbuffer, textBufferSize);
			break;
		}
		case PXCompilerSymbolLexerComment:
			PXTextCopyA("comment", 8, idbuffer, idBufferSize);
			PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, textbuffer, textBufferSize);
			break;

		case PXCompilerSymbolLexerBool:
			PXTextCopyA("bool", 5, idbuffer, idBufferSize);
			PXConsoleWriteF(0, textbuffer, "%s", compilerSymbolEntry->DataC ? "true" : "false");
			break;

		case PXCompilerSymbolLexerFloat:
			PXTextCopyA("float", 6, idbuffer, idBufferSize);
			PXConsoleWriteF(0, textbuffer, "%f", compilerSymbolEntry->DataF);
			break;

		case PXCompilerSymbolLexerInteger:
			PXTextCopyA("int", 4, idbuffer, idBufferSize);
			PXConsoleWriteF(0, textbuffer, "%i", compilerSymbolEntry->DataI32U);
			break;

		case PXCompilerSymbolLexerString:
			PXTextCopyA("string", 7, idbuffer, idBufferSize);
			PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, textbuffer, textBufferSize);
			break;

		case PXCompilerSymbolLexerGenericElement:
		{
			//PXTextCopyA("*Generic*", 10, idbuffer, idBufferSize);
			PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, textbuffer, textBufferSize);
			break;
		}
	}


	printf
	(
		"|| C:%-3i L:%-3i S:%-3i | %-10s -> %-20s ||\n",
		compilerSymbolEntry->Coloum,
		compilerSymbolEntry->Line,
		compilerSymbolEntry->Size,
		idbuffer,
		textbuffer
	);
#endif
}

PXSize PXAPI PXCompilerSymbolEntryMergeCurrentWithNext(PXCompiler* const pxCompiler, PXCompilerSymbolEntry* const compilerSymbolEntry)
{
	PXSize offset = 0;
	
	PXCompilerSymbolEntry oldCopy;
	PXCompilerSymbolEntry mergCopy;

	PXCompilerSymbolEntryExtract(pxCompiler, &oldCopy);// Copy old value
	PXSize skippedBytes = PXCompilerSymbolEntryPeek(pxCompiler, &mergCopy);
	PXSize skippedBlocks = (skippedBytes / 21 ) - 1;

	PXCompilerSymbolRewind(pxCompiler, 1); // Go back again

	// Write merged symbol
	{
		const PXInt8U symbolID = compilerSymbolEntry->ID;
		const PXInt32U size = oldCopy.Size + ((mergCopy.Coloum + mergCopy.Size) - (oldCopy.Coloum + oldCopy.Size));
		const PXFileDataElementType pxFileDataElementType[] =
		{
			&oldCopy.ID, PXDataTypeInt08U,
			&oldCopy.Coloum, PXDataTypeInt32U,
			&oldCopy.Line, PXDataTypeInt32U,
			&size, PXDataTypeInt32U,
			&oldCopy.Source, PXDataTypeAdress
		};

		const PXSize written = PXFileWriteMultible(pxCompiler->FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));
	}

	// add magic offset of previous deleted entrys
	PXFileCursorAdvance(pxCompiler->FileCache, skippedBlocks * 21);

	// delete old symbol
	{
		const PXInt8U symbolID = PXCompilerSymbolLexerInvalid;
		const PXInt32U emptyValue = 0;
		const void* emptyAdress = 0;
		const PXFileDataElementType pxFileDataElementType[] =
		{
			&symbolID, PXDataTypeInt08U,
			&mergCopy.Coloum, PXDataTypeInt32U,
			&mergCopy.Line, PXDataTypeInt32U,
			&emptyValue, PXDataTypeInt32U,
			&emptyAdress, PXDataTypeAdress
		};

		const PXSize written = PXFileWriteMultible(pxCompiler->FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));
	}

	PXCompilerSymbolRewind(pxCompiler, skippedBlocks+2); // Go back again, again

	PXCompilerSymbolEntryPeek(pxCompiler);

	return PXActionSuccessful;
}

PXSize PXAPI PXCompilerSymbolRewind(PXCompiler* const pxCompiler, const PXSize amount)
{
	const PXSize totalSize =
		sizeof(PXInt8U) +
		sizeof(PXInt32U) +
		sizeof(PXInt32U) +
		sizeof(PXInt32U) +
		sizeof(void*);

	PXSize reveredSize = pxCompiler->FileCache->DataCursor;
	PXBool isInvalidToken;

	do
	{
		PXFileCursorRewind(pxCompiler->FileCache, totalSize * amount);

		PXCompilerSymbolEntry pxCompilerSymbolEntry;

		PXCompilerSymbolEntryPeek(pxCompiler, &pxCompilerSymbolEntry);

		isInvalidToken = PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry.ID;
	}
	while (isInvalidToken);

	reveredSize = reveredSize - pxCompiler->FileCache->DataCursor;

	return reveredSize;
}

PXSize PXAPI PXCompilerSymbolEntryExtract(PXCompiler* const pxCompiler)
{
	PXCompilerSymbolEntry* pxCompilerSymbolEntry = &pxCompiler->SymbolEntryCurrent;

	PXSize readBytes = 0;

	do
	{
		PXInt8U symbolID = 0;

		const PXFileDataElementType pxFileDataElementType[] =
		{
			&symbolID, PXDataTypeInt08U,
			&pxCompilerSymbolEntry->Coloum, PXDataTypeInt32U,
			&pxCompilerSymbolEntry->Line, PXDataTypeInt32U,
			&pxCompilerSymbolEntry->Size, PXDataTypeInt32U,
			&pxCompilerSymbolEntry->Source, PXDataTypeAdress
		};
		readBytes += PXFileReadMultible(pxCompiler->FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));

		pxCompilerSymbolEntry->ID = (PXCompilerSymbolLexer)symbolID;

		if (readBytes == 0)
		{
			pxCompilerSymbolEntry->ID = PXCompilerSymbolLexerEndOfFile;
			pxCompilerSymbolEntry->Source = PXNull;
			pxCompilerSymbolEntry->Coloum = -1;
			pxCompilerSymbolEntry->Line = -1;
			pxCompilerSymbolEntry->Size = 0;
			return 0;
		}
	}
	while (PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry->ID);

	return readBytes;
}

PXSize PXAPI PXCompilerSymbolEntryForward(PXCompiler* const pxCompiler)
{
	const PXSize totalSize = 
		sizeof(PXInt8U) +
		sizeof(PXInt32U) + 
		sizeof(PXInt32U) +
		sizeof(PXInt32U) + 
		sizeof(void*);	

	PXCompilerSymbolEntry pxCompilerSymbolEntry;

	const PXSize positionBefore = pxCompiler->FileCache->DataCursor;

	PXFileCursorAdvance(pxCompiler->FileCache, totalSize);

	/*
	* 	do
	{
		PXFileCursorAdvance(pxCompiler->FileCache, totalSize);
		PXCompilerSymbolEntryPeek(pxCompiler, &pxCompilerSymbolEntry);
	}
	while (PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry.ID);
	*/
	


	return pxCompiler->FileCache->DataCursor - positionBefore;
}

PXSize PXAPI PXCompilerSymbolEntryPeek(PXCompiler* const pxCompiler)
{
	const PXSize readBytes = PXCompilerSymbolEntryExtract(pxCompiler);

	PXFileCursorRewind(pxCompiler->FileCache, readBytes);

	return readBytes;
}

PXBool PXAPI PXCompilerSymbolEntryPeekEnsure(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
	const PXBool isTarget = PXCompilerSymbolEntryPeekCheck(pxCompiler, pxCompilerSymbolLexer);

	if(!isTarget)
	{
#if PXLogEnable
		const char* expectedTypeText = PXCompilerCompilerSymbolLexerToString(pxCompilerSymbolLexer);
		const char* gotTypeText = PXCompilerCompilerSymbolLexerToString(pxCompiler->SymbolEntryCurrent.ID);

		PXLogPrint
		(
			PXLoggingError,
			"Compiler",
			"Parse",
			"Symbol not as expected!\n"
			"Expected:<%s> but got <%s>\n"
			"Line: %i, Character: %i, Size: %i",
			expectedTypeText,
			gotTypeText,
			pxCompiler->SymbolEntryCurrent.Line,
			pxCompiler->SymbolEntryCurrent.Coloum,
			pxCompiler->SymbolEntryCurrent.Size
		);		
#endif
	}

	return isTarget;
}

PXBool PXAPI PXCompilerSymbolEntryPeekCheck(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
	const PXSize readBytes = PXCompilerSymbolEntryPeek(pxCompiler);
	const PXBool isCorrectType = pxCompilerSymbolLexer == pxCompiler->SymbolEntryCurrent.ID;

	return isCorrectType;
}

PXBool PXAPI PXCompilerSymbolEntryEnsureCheckList(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer* const pxCompilerSymbolLexerList, const PXSize amount)
{
	const PXSize readBytes = PXCompilerSymbolEntryPeek(pxCompiler);

	for(PXSize i = 0; i < amount; ++i)
	{
		PXCompilerSymbolLexer pxCompilerSymbolLexer = pxCompilerSymbolLexerList[i];	

		PXBool succ = pxCompilerSymbolLexer == pxCompiler->SymbolEntryCurrent.ID;

		if(succ)
		{
			return PXTrue;
		}
	}

#if PXLogEnable

	const char* gotTypeText = PXCompilerCompilerSymbolLexerToString(pxCompiler->SymbolEntryCurrent.ID);

	PXText bufferOptions;
	PXTextConstructBufferA(&bufferOptions, 64);

	for(size_t i = 0; i < amount; i++)
	{
		PXCompilerSymbolLexer pxCompilerSymbolLexer = pxCompilerSymbolLexerList[i];

		const char* expectedTypeText = PXCompilerCompilerSymbolLexerToString(pxCompilerSymbolLexer);

		PXTextAppendF(&bufferOptions, "%s, ", expectedTypeText);
	}


	PXLogPrint
	(
		PXLoggingError,
		"Compiler",
		"Parse",
		"Symbol not as expected!\n"
		"Allowed:%s but got <%s>\n"
		"Line: %i, Character: %i, Size: %i",
		bufferOptions.TextA,
		gotTypeText,
		pxCompiler->SymbolEntryCurrent.Line,
		pxCompiler->SymbolEntryCurrent.Coloum,
		pxCompiler->SymbolEntryCurrent.Size
	);
#endif

	return PXFalse;
}

PXCompilerSymbolLexer PXAPI PXCompilerTryAnalyseType(PXFile* const tokenStream, const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry)
{
	if (textSize == 1)
	{
		switch (text[0])
		{
			case '.': return PXCompilerSymbolLexerDot;
			case ',': return PXCompilerSymbolLexerComma;
			case ':': return PXCompilerSymbolLexerColon;
			case ';': return PXCompilerSymbolLexerSemiColon;
			case '?': return PXCompilerSymbolLexerQuestionmark;
			case '!': return PXCompilerSymbolLexerExclamation;
			case '#': return PXCompilerSymbolLexerHash;
			case '+': return PXCompilerSymbolLexerPlus;
			case '-': return PXCompilerSymbolLexerMinus;
			case '/': return PXCompilerSymbolLexerSlash;
			case '*': return PXCompilerSymbolLexerAsterisk;
			case '\\': return PXCompilerSymbolLexerSlashBack;
			case '&': return PXCompilerSymbolLexerAmpercant;
			case '%': return PXCompilerSymbolLexerPercent;
			case '|': return PXCompilerSymbolLexerBar;
			case '°': return PXCompilerSymbolLexerDegree;
			case '^': return PXCompilerSymbolLexerExponent;
			case '~': return PXCompilerSymbolLexerTilde;
			case '\'': return PXCompilerSymbolLexerApostrophe;
		}
	}

	switch (text[0])
	{
		case '\r':
		case '\n':
			return PXCompilerSymbolLexerNewLine;

		case ' ':
		case '\t':
		{
			return PXCompilerSymbolLexerWhiteSpace;
		}

		case '.': 
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerDot; 
		}
		case ',': 
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerComma; 
		}
		case ':': 
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerColon; 
		}
		case ';': 
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerSemiColon; 
		}

		case '?': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerQuestionmark; }
		case '!': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerExclamation; }
		case '*': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerAsterisk; }
		case '&': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerAmpercant; }
		case '%': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerPercent; }
		case '|': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerBar; }
		case '°': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerDegree; }
		case '^': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerExponent; }
		case '~': {compilerSymbolEntry->Size = 1; return PXCompilerSymbolLexerTilde; }

		case '=':
		{
			if (text[1] == '=')
			{
				if (text[2] == '=')
				{
					compilerSymbolEntry->Size = 3;

					return PXCompilerSymbolLexerEqualTrippel;
				}

				compilerSymbolEntry->Size = 2;

				return PXCompilerSymbolLexerEqualDouble;
			}

			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerEqual;
		}

		case '{':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBracketCurlyOpen;
		}
		case '}':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBracketCurlyClose;
		}
		case '(':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBrackedRoundOpen;
		}
		case ')':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBrackedRoundClose;
		}
		case '[':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBracketSquareOpen;
		}
		case ']':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBracketSquareClose;
		}
		case '#':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerHash;
		}
		case '>':
		{
			if (text[1] == '=')
			{
				compilerSymbolEntry->Size = 2;

				return PXCompilerSymbolLexerBiggerAndEqual;
			}

			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBracketAngleClose;
		}
		case '<':
		{
			if (text[1] == '=')
			{
				if (text[2] == '>')
				{
					compilerSymbolEntry->Size = 3;

					return PXCompilerSymbolLexerCompareThreeWay;
				}

				compilerSymbolEntry->Size = 2;

				return PXCompilerSymbolLexerSmalerAndEqual;
			}

			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerBracketAngleOpen;
		}
		case '/':
		{
			compilerSymbolEntry->Size = 1;

			return PXCompilerSymbolLexerSlash;
		}
		case '\'':
		{
			const PXBool isFull = text[textSize - 1] == '\'';

			if (!isFull)
			{
				char* const dataAdress = compilerSymbolEntry->Source + 1u;
				const PXSize symbolPositionQuotationMark = PXTextFindFirstCharacterA(dataAdress, compilerSymbolEntry->Size, '\'');
				const PXBool hasIndex = symbolPositionQuotationMark != (PXSize)-1;

				if (!hasIndex)
				{
					// Error
				}

				compilerSymbolEntry->Size = symbolPositionQuotationMark + 2u;;
			}

			return PXCompilerSymbolLexerString;
		}
		case '\"':
		{
			const PXBool isFull = text[textSize - 1] == '\"';

			if (!isFull)
			{
				char* const dataAdress = compilerSymbolEntry->Source + 1u;
				PXSize symbolPositionIndex = PXTextFindFirstCharacterA(dataAdress, compilerSymbolEntry->Size, '\"');
				PXBool hasIndex = symbolPositionIndex != (PXSize)-1;

				if (!hasIndex) // Prop string has empty space, so we need to mend them together
				{
					char* const dataAdressExtended = &dataAdress[compilerSymbolEntry->Size-1];

					PXBool isEmptySpace = dataAdressExtended[0] == ' ';

					if (isEmptySpace) // Parse into oblivion for next 
					{
						const PXSize spaceLeft = PXFileRemainingSize(tokenStream);

						symbolPositionIndex = PXTextFindFirstCharacterA(dataAdress, spaceLeft, '\"');
						hasIndex = symbolPositionIndex != (PXSize)-1;

						if (hasIndex)
						{
							compilerSymbolEntry->Size = symbolPositionIndex;
							PXFileCursorAdvance(tokenStream, symbolPositionIndex);
						}
					}
				}

				compilerSymbolEntry->Size = symbolPositionIndex + 2u;
			}

			return PXCompilerSymbolLexerString;
		}
		case 'T':
		case 't':
		{
			const PXBool  result = PXTextCompareIgnoreCaseA(text, textSize, "true", 4) && textSize > 4;

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
			const PXBool  result = PXTextCompareIgnoreCaseA(text, textSize, "false", 5) && textSize > 5;

			if (result)
			{
				compilerSymbolEntry->DataC = PXNo;

				return PXCompilerSymbolLexerBool;
			}

			break;
		}
		case '+':
		case '-':
		{
			PXBool isDouble = text[0] == text[1];

			if (isDouble)
			{
				compilerSymbolEntry->Size = 2;

				return PXCompilerSymbolLexerGeneric;
			}
		}
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
			const PXSize dotIndex = PXTextFindFirstCharacterA(text, textSize, '.');
			const PXBool probablyFloat = dotIndex != (PXSize)-1;
			PXSize writtenNumbers = 0;

			PXBool isValidFloatSyntax = PXTrue;

			PXSize floatTextSize = 0;

			if (probablyFloat)
			{
				// Validate is float is valid until the '.'
				for (floatTextSize = 0; (floatTextSize <= dotIndex + 1) && isValidFloatSyntax; ++floatTextSize)
				{
					isValidFloatSyntax = PXTextFloatIsAllowedCharacter(text[floatTextSize]);
				}

				if (isValidFloatSyntax)
				{
					// If this is a float, check after the '.' until we hit non numbers
					for (floatTextSize = dotIndex + 1; (floatTextSize <= textSize) && isValidFloatSyntax; ++floatTextSize)
					{
						isValidFloatSyntax = PXTextFloatIsAllowedCharacter(text[floatTextSize]);
					}

					floatTextSize -= 1;
					isValidFloatSyntax = PXTrue;
				}			
			}

			PXText numberText;
			PXTextConstructFromAdressA(&numberText, text, floatTextSize, textSize);

			if (probablyFloat && isValidFloatSyntax)
			{
				float value = 0;

				const PXSize writtenNumbers = PXTextToFloat(&numberText, &value);
				const PXBool isFloat = floatTextSize == writtenNumbers;

				if (isFloat)
				{
					compilerSymbolEntry->Source = 0;
					compilerSymbolEntry->Size = writtenNumbers;
					compilerSymbolEntry->DataF = value;

					return PXCompilerSymbolLexerFloat;
				}
			}
			else
			{
				int value = 0;

				const PXSize writtenNumbers = PXTextToInt(&numberText, &value);

				if (writtenNumbers > 0)
				{
					compilerSymbolEntry->Source = 0;
					compilerSymbolEntry->Size = writtenNumbers;
					compilerSymbolEntry->DataI32U = value;

					return PXCompilerSymbolLexerInteger;
				}			
			}
		}
	}

	// Before we end, check if we can cleave internal stuff mixed into the block
#if 1
	PXSize i = compilerSymbolEntry->Size;
	PXSize newSize = 0;

	for (i = 0; i < compilerSymbolEntry->Size; ++i)
	{
		const char symbol = compilerSymbolEntry->Source[i];
		const PXBool isNumber = 
			('A' <= symbol && symbol <= 'Z') || 
			('a' <= symbol && symbol <= 'z') || 
			('0' <= symbol && symbol <= '9') || // We already checked if it begins with a letter. This here prevents problems that "Texture2D" will get splitted
			symbol == '.' || symbol == '_' || symbol == ':' || symbol == '/' || symbol == '\\';

		newSize += isNumber;

		if (!isNumber)
		{
			break;
		}
	}

	compilerSymbolEntry->Size = newSize;
#endif

	return PXCompilerSymbolLexerGeneric;
}

PXActionResult PXAPI PXCompilerLexicalAnalysis(PXCompiler* const pxCompiler, const PXCompilerSettings* const compilerSettings)
{
#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"Compiler",
		"Parsing",
		"----- Start -----"
	);
#endif

	// Valid call
	{
		const PXBool isValidCall = pxCompiler->FileCache && pxCompiler->FileInput && compilerSettings;

		if (!isValidCall)
		{
			return;
		}
	}


	// Do you have a valid input file?

	// Do we have a valid outputfile?

	// Settings invalid?





	PXFileOpenTemporal(pxCompiler->FileCache, pxCompiler->FileInput->DataSize * 16);


	PXSize currentLine = 1;
	PXSize currentColoum = 1;
	PXBool isFirstWhiteSpaceInLine = 1u;

#if PXCompilerSanitise && 0
	PXMemorySet(pxCompiler->FileCache, '#', pxCompiler->FileCache->DataSize);
#endif

	const PXCompilerSymbolLexer newLineSymbol = compilerSettings->IntrepredNewLineAsWhiteSpace ? PXCompilerSymbolLexerWhiteSpace : PXCompilerSymbolLexerNewLine;

	while (!PXFileIsAtEnd(pxCompiler->FileInput))
	{
		PXCompilerSymbolEntry compilerSymbolEntry;
		PXClear(PXCompilerSymbolEntry, &compilerSymbolEntry);

		compilerSymbolEntry.Size = PXFileRemainingSize(pxCompiler->FileInput);
		compilerSymbolEntry.Source = (char*)PXFileCursorPosition(pxCompiler->FileInput);

		//-----------------------------------------------------------------------------
		// Consume whitespace
		//-----------------------------------------------------------------------------
		{
			const PXSize whiteSpaceSize = PXFileSkipEmptySpace(pxCompiler->FileInput); // Skip 'space' and TABs

			currentColoum += whiteSpaceSize;

			if (whiteSpaceSize)
			{
				isFirstWhiteSpaceInLine = 0;

				compilerSymbolEntry.ID = PXCompilerSymbolLexerWhiteSpace;
				compilerSymbolEntry.Line = currentLine;
				compilerSymbolEntry.Coloum = currentColoum;
				compilerSymbolEntry.Size = whiteSpaceSize;

				if (compilerSettings->KeepWhiteSpace && isFirstWhiteSpaceInLine)
				{
					PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
				}

				continue;
			}
		}

		//-----------------------------------------------------------------------------
		// Consume new line
		//-----------------------------------------------------------------------------
		{
			const PXSize endofLineSize = PXFileSkipEndOfLineCharacters(pxCompiler->FileInput);

			if (endofLineSize)
			{
				const PXSize linesSkipped = PXTextCountA(compilerSymbolEntry.Source, endofLineSize, '\n');

				compilerSymbolEntry.ID = newLineSymbol;
				compilerSymbolEntry.Line = currentLine;
				compilerSymbolEntry.Coloum = currentColoum;
				compilerSymbolEntry.Size = endofLineSize;

				isFirstWhiteSpaceInLine = 1u;

				currentColoum = 1; // Reset, next entry will begin in new line
				currentLine += linesSkipped;

				if (compilerSettings->KeepNewLines)
				{
					PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
				}	

				continue;
			}
		}
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		// Extract comments, Single line and multible lines
		//-----------------------------------------------------------------------------
		{
			PXBool isLineComment = PXTextCompareA(compilerSymbolEntry.Source, compilerSettings->CommentSingleLineSize, compilerSettings->CommentSingleLine, compilerSettings->CommentSingleLineSize);

			if (isLineComment)
			{
				compilerSymbolEntry.ID = PXCompilerSymbolLexerComment; // This is a comment 
				compilerSymbolEntry.Line = currentLine;
				compilerSymbolEntry.Coloum = currentColoum;
				compilerSymbolEntry.Source += compilerSettings->CommentSingleLineSize;
				compilerSymbolEntry.Size = PXFileSkipLine(pxCompiler->FileInput) - compilerSettings->CommentSingleLineSize;

				// if we have multtible line comments in a row, we want to merge them.
				PXBool merged = PXFalse;
				
				do
				{
					const char* isChainedComment = &compilerSymbolEntry.Source[compilerSymbolEntry.Size];
					isLineComment = PXTextCompareA(isChainedComment, compilerSettings->CommentSingleLineSize, compilerSettings->CommentSingleLine, compilerSettings->CommentSingleLineSize);
				
					if(isLineComment)
					{
						compilerSymbolEntry.Size += PXFileSkipLine(pxCompiler->FileInput);
					}				
				} 
				while(isLineComment);
			
				if (compilerSettings->CommentsKeep)
				{
					PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);

					continue;
				}
			}

			const PXBool isStartOfMultibleLineComment = PXTextCompareA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, compilerSettings->CommentMultibleLineBegin, compilerSettings->CommentMultibleLineBeginSize);

			if (isStartOfMultibleLineComment)
			{
				const PXSize index = PXTextFindFirstStringA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, compilerSettings->CommentMultibleLineEnd, compilerSettings->CommentMultibleLineEndSize);
				const PXBool isCompleate = index != -1;

				if (!isCompleate)
				{
					// Error, we have a multiline comment without an end.
					// printf("Errr\n");
					return PXActionInvalid;
				}

				compilerSymbolEntry.ID = PXCompilerSymbolLexerComment;
				compilerSymbolEntry.Source += compilerSettings->CommentMultibleLineEndSize;
				compilerSymbolEntry.Size -= compilerSettings->CommentMultibleLineEndSize;
				compilerSymbolEntry.Size += index;

				if (compilerSettings->CommentsKeep)
				{
					PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
				}

				continue;
			}
		}
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		// Consume block
		//-----------------------------------------------------------------------------
		{
			PXSize fullBlockSize = PXFileSkipBlock(pxCompiler->FileInput);	// consume block
			char* blockStart = compilerSymbolEntry.Source;

			while (fullBlockSize > 0)
			{
				compilerSymbolEntry.Source = blockStart;
				compilerSymbolEntry.Line = currentLine;
				compilerSymbolEntry.Coloum = currentColoum;
				compilerSymbolEntry.Size = fullBlockSize;
				compilerSymbolEntry.ID = PXCompilerTryAnalyseType(pxCompiler->FileInput, compilerSymbolEntry.Source, compilerSymbolEntry.Size, &compilerSymbolEntry);

				if (compilerSymbolEntry.ID == PXCompilerSymbolLexerWhiteSpace || compilerSymbolEntry.ID == PXCompilerSymbolLexerNewLine)
				{
					break;
				}

				if (compilerSymbolEntry.ID == PXCompilerSymbolLexerString)
				{
					++compilerSymbolEntry.Source;
					compilerSymbolEntry.Size -= 2;
				}

				PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);

				if (compilerSymbolEntry.ID == PXCompilerSymbolLexerString)
				{
					--compilerSymbolEntry.Source;
					compilerSymbolEntry.Size += 2;
				}

				if (fullBlockSize < compilerSymbolEntry.Size) // Block got expanded manually, considering this as done
				{
					break;
				}

				// Change data for next itteration
				blockStart += compilerSymbolEntry.Size; // Move data point forward
				fullBlockSize -= compilerSymbolEntry.Size; // Reduce blocksize by scanned data
				currentColoum += compilerSymbolEntry.Size; // Forward the coloum of the data position			
			}
		}
	}

	// Mark end of output Stream
	pxCompiler->FileCache->DataSize = pxCompiler->FileCache->DataCursor;
	PXFileCursorToBeginning(pxCompiler->FileCache);

#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"Compiler",
		"Parsing",
		"----- Done -----"
	);
#endif

	return PXActionSuccessful;
}

PXBool PXAPI PXCompilerParseStringUntilNewLine(PXCompiler* const pxCompiler, PXText* const pxText)
{
	PXCompilerSymbolEntryExtract(pxCompiler); // Expect a name.

	const PXBool isText = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

	if (!isText)
	{
		return PXNo;
	}

	PXInt16U line = pxCompiler->SymbolEntryCurrent.Line;
	PXInt16U coloumStart = pxCompiler->SymbolEntryCurrent.Coloum;
	PXSize dataBlockSize = pxCompiler->SymbolEntryCurrent.Size;
	char* dataBlockPoint = pxCompiler->SymbolEntryCurrent.Source;

	while (1u)
	{
		PXCompilerSymbolEntryPeek(pxCompiler);

		const PXBool isDone = (pxCompiler->SymbolEntryCurrent.Line != line) || (pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerNewLine);

		if (isDone)
		{
			break;
		}

		PXCompilerSymbolEntryExtract(pxCompiler);

		dataBlockSize = (pxCompiler->SymbolEntryCurrent.Coloum - coloumStart) + pxCompiler->SymbolEntryCurrent.Size;
	}

	pxText->SizeUsed = PXTextCopyA(dataBlockPoint, dataBlockSize, pxText->TextA, pxText->SizeAllocated);

	return PXYes;
}

PXBool PXAPI PXCompilerParseStringUntilNewLineA(PXCompiler* const pxCompiler, char* const text, const PXSize textMaxSize, PXSize* const textSize)
{
	PXText pxText;
	PXTextConstructFromAdressA(&pxText, text, 0, textMaxSize);
	const PXBool result = PXCompilerParseStringUntilNewLine(pxCompiler, &pxText);

	*textSize = pxText.SizeUsed;

	return result;
}

PXBool PXAPI PXCompilerParseFloatSingle(PXCompiler* const pxCompiler, float* const values)
{
	PXCompilerSymbolEntryPeek(pxCompiler);

	const PXBool isFloat = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerFloat;
	const PXBool isInt = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerInteger;
	const PXBool isValid = isFloat || isInt;

	if (isFloat)
	{
		*values = pxCompiler->SymbolEntryCurrent.DataF;

		PXCompilerSymbolEntryExtract(pxCompiler);
	}

	if (isInt)
	{
		*values = pxCompiler->SymbolEntryCurrent.DataI32S;

		PXCompilerSymbolEntryExtract(pxCompiler);
	}

	return isValid;
}

PXBool PXAPI PXCompilerParseFloatList(PXCompiler* const pxCompiler, float* const values, const PXSize valuesMaxSize, PXSize* const valuesSize)
{
	for (PXSize i = 0; i < valuesMaxSize; ++i)
	{
		const PXBool isValid = PXCompilerParseFloatSingle(pxCompiler, &values[*valuesSize]);

		if (!isValid)
		{
			// we are at the line end

			return PXNo; // finished
		}

		++(*valuesSize);
	}

	return PXYes;
}