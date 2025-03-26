#include "PXUSD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/Console/PXConsole.h>

const char PXUSDName[] = "USD";

const char PXUSDTextDef[] = "def";
const char PXUSDTextXform[] = "Xform";

const char PXUSDTextdefaultPrim[] = "defaultPrim";
const char PXUSDTextupAxis[] = "upAxis";
const char PXUSDTextkind[] = "kind";
const char PXUSDTextprepend[] = "prepend";
const char PXUSDTextreferences[] = "references";

const char PXUSDTextdouble3[] = "double3";
const char PXUSDTextuniform[] = "uniform";


void PXAPI PXUSDParsePropertysScene(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseDEF(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseFormThingParam(PXCompiler* pxCompiler, PXFile* tokenSteam);


void PXAPI PXUSDParsePropertysScene(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    while(!PXFileIsAtEnd(tokenSteam))
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundClose:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                return;
            }

            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                break;
            }
        }
    }
}

void PXAPI PXUSDParseDEF(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    while(!PXFileIsAtEnd(tokenSteam))
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isxform = PXTextCompareA(PXUSDTextXform, sizeof(PXUSDTextXform)-1, pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

                PXCompilerSymbolEntryForward(pxCompiler);
                
                PXUSDParseFormThing(pxCompiler, tokenSteam);

                return;
            }            
            case PXCompilerSymbolLexerString:
            {
                PXUSDParseFormThing(pxCompiler, tokenSteam);

                break;
            }

            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                break;
            }
        }
    }
}


void PXAPI PXUSDParseFormThingParam(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);

    PXBool done = 0;

    while(!done)
    {
        if(PXFileIsAtEnd(tokenSteam))
        {
            done = 1;
            continue;
        }

        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundClose:
            {
                done = 1;
                continue;
            }
            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                break;
            }
        }
    }

    PXBool isClose = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundClose);

    if(!isClose)
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDParseFormThingContent(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);

    PXBool done = 0;

    while(!done)
    {
        if(PXFileIsAtEnd(tokenSteam))
        {
            done = 1;
            continue;
        }

        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isDef = PXTextCompareA(PXUSDTextDef, sizeof(PXUSDTextDef)-1, pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

                if(isDef)
                {
                    PXCompilerSymbolEntryForward(pxCompiler);
                    PXUSDParseDEF(pxCompiler, tokenSteam);
                }

                break;
            }
            case PXCompilerSymbolLexerBracketCurlyClose:
            {
                done = 1;
                continue;
            }
            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                return;
            }
        }
    }


    PXBool isClose = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);

    if(!isClose)
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXCompilerSymbolEntryPeek(pxCompiler);

    PXBool isName = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    if(!isName)
    {
        return;
    }

#if PXLogEnable
    char nameCache[64];
    PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, nameCache, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDName,
        "Parsing",
        "Element:%s",
        nameCache
    );
#endif

    PXCompilerSymbolEntryForward(pxCompiler);

    PXUSDParseFormThingParam(pxCompiler, tokenSteam);
    PXUSDParseFormThingContent(pxCompiler, tokenSteam);
}


PXActionResult PXAPI PXUSDLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXUSD pxUSD;

    PXFile* const pxFile = pxResourceLoadInfo->FileReference;
    
    PXCompiler pxCompiler;
    PXFile tokenSteam;

    // Lexer
    {   
        PXClear(PXCompiler, &pxCompiler);
        pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
        pxCompiler.ReadInfo.FileCache = &tokenSteam;
        pxCompiler.Flags = PXCompilerKeepAnalyseTypes;
        //pxCompiler.CommentSingleLineSize = 1u;
        //pxCompiler.CommentSingleLine = "#";

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDName,
            "Parsing",
            "Start"
        );
#endif


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDName,
            "Parsing",
            "Step 1, Lexer..."
        );
#endif

        // Lexer - Level I
        PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens
    }


    // Header
    {
        PXBool isHash = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerHash);

        if(!isHash)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXBool isusda = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerGeneric);

        if(!isusda)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXBool isFloatNumber = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerPXF32);

        if(!isFloatNumber)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        pxUSD.Version = pxCompiler.ReadInfo.SymbolEntryCurrent.DataF;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDName,
            "Parsing",
            "Version: %f",
            pxUSD.Version
        );
#endif

        PXCompilerSymbolEntryForward(&pxCompiler);
    }




    while(!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntryPeek(&pxCompiler);
    
        switch(pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundOpen:
            {
                PXUSDParsePropertysScene(&pxCompiler, &tokenSteam);
                break;
            }
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isDef = PXTextCompareA(PXUSDTextDef, sizeof(PXUSDTextDef)-1, pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                if(isDef)
                {
                    PXCompilerSymbolEntryForward(&pxCompiler);
                    PXUSDParseDEF(&pxCompiler, &tokenSteam);
                }

                break;
            }

            default:
                break;
        }
    
    }



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDName,
        "Parsing",
        "DONE!"
    );
#endif


    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}