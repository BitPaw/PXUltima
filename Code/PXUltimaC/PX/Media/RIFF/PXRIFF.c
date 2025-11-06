#include "PXRIFF.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>

#define PXRIFFSignature PXI32Make('R', 'I', 'F', 'F')
#define RIFXSignature PXI32Make('R', 'I', 'F', 'X')
//#define PXRIFFSubTypeANI PXI32MakeEndianBig('x', 'x', 'x', 'x')
#define PXRIFFSubTypeAVI PXI32Make('A', 'V', 'I', ' ')
#define PXRIFFSubTypeAVIX PXI32Make('A', 'V', 'I', 'X')
#define PXRIFFSubTypeRDIB PXI32Make('R', 'D', 'I', 'B')
//#define PXRIFFSubTypeBND PXI32MakeEndianBig('x', 'x', 'x', 'x')
//#define PXRIFFSubTypeDXR PXI32MakeEndianBig('x', 'x', 'x', 'x')
#define PXRIFFSubTypePAL PXI32Make('P', 'A', 'L', ' ')
//#define PXRIFFSubTypeRDI PXI32MakeEndianBig('x', 'x', 'x', 'x')
#define PXRIFFSubTypeRMMP PXI32Make('R', 'M', 'M', 'P')
#define PXRIFFSubTypeRMID PXI32Make('R', 'M', 'I', 'D')
#define PXRIFFSubTypeWAVE PXI32Make('W', 'A', 'V', 'E')
#define PXRIFFSubTypeWEBP PXI32Make('W', 'E', 'B', 'P')

PXEndian PXAPI PXRIFFEndianFromID(const PXI32U value)
{
    switch (value)
    {
    case RIFXSignature:
        return PXEndianBig;
    case PXRIFFSignature:
        return PXEndianLittle;
    default:
        return PXEndianInvalid;
    }
}

PXI32U PXAPI PXRIFFEndianToID(const PXEndian value)
{
    switch (value)
    {
    case PXEndianBig:
        return RIFXSignature;
    case PXEndianLittle:
        return PXRIFFSignature;
    default:
        return 0;
    }
}

PXRIFFFormat PXAPI PXRIFFFormatFromID(const PXI32U value)
{
    switch (value)
    {
    case PXRIFFSubTypeAVI:
        return PXRIFFAudioVideoInterleave;
    case PXRIFFSubTypeAVIX:
        return PXRIFFAudioVideoInterleave; // Is this a different version?
    case PXRIFFSubTypeRDIB:
        return PXRIFFDeviceIndependentBitmap;
    case PXRIFFSubTypePAL:
        return PXRIFFPalette;
    case PXRIFFSubTypeRMMP:
        return PXRIFFMultimediaMovieFile;
    case PXRIFFSubTypeRMID:
        return PXRIFFPXMIDI;
    case PXRIFFSubTypeWAVE:
        return PXRIFFWaveformAudio;
    case PXRIFFSubTypeWEBP:
        return PXRIFFWebPicture;
    default:
        return PXRIFFInvalid;
    }
}

PXI32U PXAPI PXRIFFFormatToID(const PXRIFFFormat value)
{
    switch (value)
    {
    case PXRIFFAudioVideoInterleave:
        return PXRIFFSubTypeAVI;
    //case PXRIFFAudioVideoInterleave: return PXRIFFSubTypeAVIX; // Is this a different version?
    case PXRIFFDeviceIndependentBitmap:
        return PXRIFFSubTypeRDIB;
    case PXRIFFPalette:
        return PXRIFFSubTypePAL;
    case PXRIFFMultimediaMovieFile:
        return PXRIFFSubTypeRMMP;
    case PXRIFFPXMIDI:
        return PXRIFFSubTypeRMID;
    case PXRIFFWaveformAudio:
        return PXRIFFSubTypeWAVE;
    case PXRIFFWebPicture:
        return PXRIFFSubTypeWEBP;
    default:
        return 0;
    }
}

PXResult PXAPI PXRIFFLoadFromFile(PXRIFF PXREF riff, PXFile PXREF pxFile)
{
    PXI32UCluster chunkID;
    PXI32UCluster formatID;

    const PXTypeEntry pxDataStreamElementList[] =
    {
        {chunkID.Data, PXTypeDatax4},
        {&riff->ChunkSize, PXTypeInt32ULE},
        {formatID.Data, PXTypeDatax4}
    };

    PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

    riff->EndianFormat = PXRIFFEndianFromID(chunkID.Value);
    riff->Format = PXRIFFFormatFromID(formatID.Value);
    riff->Valid = (riff->EndianFormat != PXEndianInvalid) && (riff->Format != PXRIFFInvalid);

    return PXActionSuccessful;
}

PXResult PXAPI PXRIFFSaveToFile(const PXRIFF PXREF riff, PXFile PXREF pxFile)
{
    const PXI32U riffSignature = PXRIFFEndianToID(riff->EndianFormat);
    const PXI32U riffType = PXRIFFFormatToID(riff->Format);

    const PXTypeEntry pxDataStreamElementList[] =
    {
        {&riffSignature, PXTypeInt32ULE},
        {&riff->ChunkSize, PXTypeInt32ULE},
        {&riffType, PXTypeInt32ULE}
    };

    PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

    return PXActionSuccessful;
}
