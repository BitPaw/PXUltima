#ifndef PXMIDINCLUDE
#define PXMIDINCLUDE

#include "../PXResource.h"

typedef enum PXMIDICommand_
{
	PXMIDICommandUnkown,
	PXMIDICommandNoteON, // 0x8?
	PXMIDICommandNoteOFF, // 0x9?
	PXMIDICommandPolyphonicKeyAfterTouch, // 0xA?
	PXMIDICommandControlChange, // 0xB?
	PXMIDICommandPXProgrammChange, // 0xC?
	PXMIDICommandChannelAfterTouch, // 0xD?
	PXMIDICommandPitchBendChange, // 0xE?
	PXMIDICommandNoteEvent // 0xFF
}
PXMIDICommand;

typedef enum PXMIDICommandExtended_
{
	PXMIDICommandExtendedUnkown,

	PXMIDICommandExtendedCopyright, // 0x01
	PXMIDICommandExtendedInstrumentName, // 0x03
	PXMIDICommandExtendedTempo, // 0x51
	PXMIDICommandExtendedTimeSignature, // 0x58
	PXMIDICommandExtendedKeySignature // 0x59
}
PXMIDICommandExtended;

typedef struct PXMIDINote_
{
	PXInt8U DeltaTime;
	PXInt8U KeyNote;
	PXInt8U Volume;
}
PXMIDINote;

typedef struct PXMIDITrack_
{
	unsigned int ID;
	unsigned int EventDataSize;
	unsigned char* EventData;
}
PXMIDITrack;

typedef struct PXMIDI_
{
	PXMIDITrack* TrackList;
	PXSize TrackListSize;

	PXInt16U TrackListAmount;
	PXInt16U Format;
	PXInt16U MusicSpeed;
}
PXMIDI;

PXPublic PXActionResult PXAPI PXMIDILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMIDISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif