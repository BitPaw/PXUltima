#include "PXMP4.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

#define M4ADebugLog 0

#define M4AChunkIDFTYP PXI32Make('f','t','y','p')
#define M4AChunkIDMDAT PXI32Make('m','d','a','t')
#define M4AChunkIDMOOV PXI32Make('m','o','o','v')
#define M4AChunkIDPNOT PXI32Make('p','n','o','t')
#define M4AChunkIDUDTA PXI32Make('u','d','t','a')
#define M4AChunkIDUUID PXI32Make('u','u','i','d')
#define M4AChunkIDMOOF PXI32Make('m','o','o','f')
#define M4AChunkIDFREE PXI32Make('f','r','e','e')
#define M4AChunkIDSKIP PXI32Make('s','k','i','p')
#define M4AChunkIDJP2 PXI32Make('j','P','2',' ')
#define M4AChunkIDWIDE PXI32Make('w','i','d','e')
#define M4AChunkIDLOAD PXI32Make('l','o','a','d')
#define M4AChunkIDCTAB PXI32Make('c','t','a','b')
#define M4AChunkIDIMAP PXI32Make('i','m','a','p')
#define M4AChunkIDMATT PXI32Make('m','a','t','t')
#define M4AChunkIDKMAT PXI32Make('k','m','a','t')
#define M4AChunkIDCLIP PXI32Make('c','l','i','p')
#define M4AChunkIDCRGN PXI32Make('c','r','g','n')
#define M4AChunkIDSYNC PXI32Make('s','y','n','c')
#define M4AChunkIDCHAP PXI32Make('c','h','a','p')
#define M4AChunkIDTMCD PXI32Make('t','m','c','d')
#define M4AChunkIDSCPT PXI32Make('s','c','p','t')
#define M4AChunkIDSSRC PXI32Make('s','s','r','c')
#define M4AChunkIDPICT PXI32Make('P','I','C','T')

PXM4AChunkID PXAPI PXMP4ChunkFromID(const PXI32U chunkID)
{
    switch(chunkID)
    {
    case  M4AChunkIDFTYP:
        return M4AChunkFTYP;
    case  M4AChunkIDMDAT:
        return M4AChunkMDAT;
    case  M4AChunkIDMOOV:
        return M4AChunkMOOV;
    case  M4AChunkIDPNOT:
        return M4AChunkPNOT;
    case  M4AChunkIDUDTA:
        return M4AChunkUDTA;
    case  M4AChunkIDUUID:
        return M4AChunkUUID;
    case  M4AChunkIDMOOF:
        return M4AChunkMOOF;
    case  M4AChunkIDFREE:
        return M4AChunkFREE;
    case  M4AChunkIDSKIP:
        return M4AChunkSKIP;
    case  M4AChunkIDJP2:
        return M4AChunkJP2;
    case  M4AChunkIDWIDE:
        return M4AChunkWIDE;
    case  M4AChunkIDLOAD:
        return M4AChunkLOAD;
    case  M4AChunkIDCTAB:
        return M4AChunkCTAB;
    case  M4AChunkIDIMAP:
        return M4AChunkIMAP;
    case  M4AChunkIDMATT:
        return M4AChunkMATT;
    case  M4AChunkIDKMAT:
        return M4AChunkKMAT;
    case  M4AChunkIDCLIP:
        return M4AChunkCLIP;
    case  M4AChunkIDCRGN:
        return M4AChunkCRGN;
    case  M4AChunkIDSYNC:
        return M4AChunkSYNC;
    case  M4AChunkIDCHAP:
        return M4AChunkCHAP;
    case  M4AChunkIDTMCD:
        return M4AChunkTMCD;
    case  M4AChunkIDSCPT:
        return M4AChunkSCPT;
    case  M4AChunkIDSSRC:
        return M4AChunkSSRC;
    case  M4AChunkIDPICT:
        return M4AChunkPICT;

    default:
        return M4AChunkInvalid;
    }
}

PXResult PXAPI PXMP4LoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

    PXMP4 pxMP4;

    PXClear(PXMP4, &pxMP4);

    while(!PXFileIsAtEnd(pxFile))
    {
        PXM4AChunk chunk;

        unsigned int chunkSize = 0;
        PXI32UCluster typePrimaryID;

        PXFileReadI32UE(&pxFile, &chunkSize, PXEndianBig);
        PXFileReadB(&pxFile, typePrimaryID.Data, 4u);

        const PXSize positionPrediction = PXFileDataPosition(pxFile) + chunkSize - 8;
        const PXM4AChunkID typePrimary = PXMP4ChunkFromID(typePrimaryID.Value);

#if PXLogEnable && M4ADebugLog
        PXLogPrint
        (
            PXLoggingInfo,
            "M4A",
            "Parse-Chunk"
            "(%c%c%c%c) %i Bytes\n",
            typePrimaryID.A,
            typePrimaryID.B,
            typePrimaryID.C,
            typePrimaryID.D,
            chunkSize
        );
#endif

        switch(typePrimary)
        {
        case M4AChunkFTYP:
        {
            unsigned int sizeB = 0;
            char isoSignature[8]; // isom3gp4

            PXFileReadB(pxFile, chunk.TypeSub, 4u);
            PXFileReadI32UE(pxFile, &sizeB, PXEndianBig);
            PXFileReadB(pxFile, isoSignature, 8u);

            break;
        }
        case M4AChunkMDAT:
            break;
        case M4AChunkMOOV:
            break;
        case M4AChunkPNOT:
            break;
        case M4AChunkUDTA:
            break;
        case M4AChunkUUID:
            break;
        case M4AChunkMOOF:
            break;
        case M4AChunkFREE:
            break;
        case M4AChunkSKIP:
            break;
        case M4AChunkJP2:
            break;
        case M4AChunkWIDE:
            break;
        case M4AChunkLOAD:
            break;
        case M4AChunkCTAB:
            break;
        case M4AChunkIMAP:
            break;
        case M4AChunkMATT:
            break;
        case M4AChunkKMAT:
            break;
        case M4AChunkCLIP:
            break;
        case M4AChunkCRGN:
            break;
        case M4AChunkSYNC:
            break;
        case M4AChunkCHAP:
            break;
        case M4AChunkTMCD:
            break;
        case M4AChunkSCPT:
            break;
        case M4AChunkSSRC:
            break;
        case M4AChunkPICT:
            break;

        case M4AChunkInvalid:
        default:
        {
            // Unhandled chunk
            break;
        }
        }

        if(PXFileDataPosition(pxFile) < positionPrediction)
        {
            const unsigned int offset = positionPrediction - PXFileDataPosition(pxFile);

#if PXLogEnable && M4ADebugLog
            printf("[MP4] Illegal allignment detected! Moving %i Bytes\n", offset);
#endif
            PXFileCursorMoveTo(pxFile, positionPrediction);
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXMP4SaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
