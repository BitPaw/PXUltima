#pragma once

#ifndef PXMP4MIncluded
#define PXMP4MIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXM4AChunkID_
{
    M4AChunkInvalid,
    M4AChunkFTYP,
    M4AChunkMDAT,
    M4AChunkMOOV,
    M4AChunkPNOT,
    M4AChunkUDTA,
    M4AChunkUUID,
    M4AChunkMOOF,
    M4AChunkFREE,
    M4AChunkSKIP,
    M4AChunkJP2,
    M4AChunkWIDE,
    M4AChunkLOAD,
    M4AChunkCTAB,
    M4AChunkIMAP,
    M4AChunkMATT,
    M4AChunkKMAT,
    M4AChunkCLIP,
    M4AChunkCRGN,
    M4AChunkSYNC,
    M4AChunkCHAP,
    M4AChunkTMCD,
    M4AChunkSCPT,
    M4AChunkSSRC,
    M4AChunkPICT
}
PXM4AChunkID;

typedef struct PXM4AChunk_
{
    PXM4AChunkID TypePrimary;
    char TypeSub[4];
    unsigned int BlockSize;
}
PXM4AChunk;

typedef struct PXMP4_
{
    unsigned int __dummy__;
}
PXMP4;

PXPrivate PXM4AChunkID PXAPI PXMP4ChunkFromID(const PXI32U chunkID);

PXPublic PXResult PXAPI PXMP4LoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMP4SaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
