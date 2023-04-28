#ifndef MIDInclude
#define MIDInclude

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum MIDICommand_
	{
		MIDICommandUnkown,
		MIDICommandNoteON, // 0x8?
		MIDICommandNoteOFF, // 0x9?
		MIDICommandPolyphonicKeyAfterTouch, // 0xA?
		MIDICommandControlChange, // 0xB?
		MIDICommandProgrammChange, // 0xC?
		MIDICommandChannelAfterTouch, // 0xD?
		MIDICommandPitchBendChange, // 0xE?
		MIDICommandNoteEvent // 0xFF
	}
	MIDICommand;
	
	typedef enum MIDICommandExtended_
	{
		MIDICommandExtendedUnkown,

		MIDICommandExtendedCopyright, // 0x01
		MIDICommandExtendedInstrumentName, // 0x03
		MIDICommandExtendedTempo, // 0x51
		MIDICommandExtendedTimeSignature, // 0x58
		MIDICommandExtendedKeySignature // 0x59
	}
	MIDICommandExtended;

	typedef struct MIDINote_
	{
		unsigned char DeltaTime;
		unsigned char KeyNote;
		unsigned char Volume;
	}
	MIDINote;

	typedef struct MIDITrack_
	{
		unsigned int ID;
		unsigned int EventDataSize;
		unsigned char* EventData;
	}
	MIDITrack;

	typedef struct MID_
	{
		unsigned short Format;
		unsigned short MusicSpeed;
		unsigned int TrackListSize;
		MIDITrack* TrackList;
	}
	MID;

	PXPublic PXActionResult MIDParse(MID* mid, PXFile* const PXFile);
	PXPublic PXActionResult MIDSerialize(MID* mid, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif