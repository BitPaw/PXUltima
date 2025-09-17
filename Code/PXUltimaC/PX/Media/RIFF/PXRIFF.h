#ifndef PXRIFFIncluded
#define PXRIFFIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXRIFFFormat_
{
    PXRIFFInvalid,
    PXRIFFWebPicture, // WEBP
    PXRIFFWaveformAudio, // WAVE
    PXRIFFMultimediaMovieFile, // RMMP
    PXRIFFPXMIDI, // RMID
    PXRIFFDeviceIndependentBitmap, // RDIB
    PXRIFFPalette,  // PAL
    PXRIFFAudioVideoInterleave, // AVI
    //DLS,
}
PXRIFFFormat;

// Resource Interchange File Format.
typedef struct PXRIFF_
{
    PXI32U ChunkSize;
    PXRIFFFormat Format;
    PXEndian EndianFormat;
    PXBool Valid;
}
PXRIFF;

PXPrivate inline PXEndian PXAPI PXRIFFEndianFromID(const PXI32U value);
PXPrivate inline PXI32U PXAPI PXRIFFEndianToID(const PXEndian value);

PXPrivate inline PXRIFFFormat PXAPI PXRIFFFormatFromID(const PXI32U value);
PXPrivate inline PXI32U PXAPI PXRIFFFormatToID(const PXRIFFFormat value);


PXPublic PXResult PXAPI PXRIFFLoadFromFile(PXRIFF* const riff, PXFile* const PXFile);
PXPublic PXResult PXAPI PXRIFFSaveToFile(const PXRIFF* const riff, PXFile* const PXFile);

#endif
