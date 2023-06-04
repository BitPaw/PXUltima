#ifndef PXMIDINCLUDE
#define PXMIDINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
		unsigned char DeltaTime;
		unsigned char KeyNote;
		unsigned char Volume;
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
		unsigned short Format;
		unsigned short MusicSpeed;
		unsigned int TrackListSize;
		PXMIDITrack* TrackList;
	}
	PXMIDI;

	PXPublic PXActionResult PXMIDIParse(PXMIDI* const mid, PXFile* const PXFile);
	PXPublic PXActionResult PXMIDISerialize(PXMIDI* const mid, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif