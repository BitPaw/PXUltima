#ifndef PXMP3INCLUDE
#define PXMP3INCLUDE

#include <Media/ID3/PXID3.h>

#define PXMP3Debug 0

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXMPEGGenre_
	{
		MPEGGenreUnknown,

		Blues,
		ClassicRock,
		Country,
		Dance,
		Disco,
		Funk,
		Grunge,
		HipHop,
		Jazz,
		Metal,
		NewAge,
		Oldies,
		Other,
		Pop,
		RAndB,
		Rap,
		Reggae,
		Rock,
		Techno,
		Industrial,
		Alternative,
		Ska,
		DeathMetal,
		Pranks,
		Soundtrack,
		EuroTechno,
		Ambient,
		TripHop,
		Vocal,
		JazzFunk,
		Fusion,
		Trance,
		Classical,
		Instrumental,
		Acid,
		House,
		Game,
		SoundClip,
		Gospel,
		Noise,
		AlternRock,
		Bass,
		Soul,
		Punk,
		Space,
		Meditative,
		InstrumentalPop,
		InstrumentalRock,
		Ethnic,
		Gothic,
		Darkwave,
		TechnoIndustrial,
		Electronic,
		PopFolk,
		Eurodance,
		Dream,
		SouthernRock,
		Comedy,
		Cult,
		Gangsta,
		Top40,
		ChristianRap,
		PopFunk,
		Jungle,
		NativeAmerican,
		Cabaret,
		NewWave,
		Psychadelic,
		Rave,
		Showtunes,
		Trailer,
		LoFi,
		Tribal,
		AcidPunk,
		AcidJazz,
		Polka,
		Retro,
		Musical,
		RockAndRoll,
		HardRock,
		Folk,
		FolkRock,
		NationalFolk,
		Swing,
		FastFusion,
		Bebob,
		Latin,
		Revival,
		Celtic,
		Bluegrass,
		Avantgarde,
		GothicRock,
		ProgressiveRock,
		PsychedelicRock,
		SymphonicRock,
		SlowRock,
		BigBand,
		Chorus,
		EasyListening,
		Acoustic,
		Humour,
		Speech,
		Chanson,
		Opera,
		ChamberMusic,
		Sonata,
		Symphony,
		BootyBrass,
		Primus,
		PornGroove,
		Satire,
		SlowJam,
		Club,
		Tango,
		Samba,
		Folklore,
		Ballad,
		PoweerBallad,
		RhytmicSoul,
		Freestyle,
		Duet,
		PunkRock,
		DrumSolo,
		ACapela,
		EuroHouse,
		DanceHall
	}
	PXMPEGGenre;

	typedef enum PXMP3ChannelMode_
	{
		PXMP3ChannelStereo,
		PXMP3ChannelJointStereo,
		PXMP3ChannelDual,
		PXMP3ChannelSingle
	}
	PXMP3ChannelMode;

	typedef enum PXMP3Emphasis_
	{
		PXMP3EmphasisReserved,
		PXMP3EmphasisNone,
		PXMP3Emphasisms5015,
		PXMP3EmphasisCCITJ17,
	}
	PXMP3Emphasis;

	typedef enum PXMP3Layer_
	{
		PXMP3LayerReserved,
		PXMP3LayerI,
		PXMP3LayerII,
		PXMP3LayerIII
	}
	PXMP3Layer;

	typedef enum PXMP3MPEGVersion_
	{
		MPEGVersionReserved,
		MPEGVersion1,
		MPEGVersion2,
		MPEGVersion25
	}
	PXMP3MPEGVersion;

	typedef struct MPEGAudioTag_
	{
		// char[3] == TAG
		char Title[30];
		char Artist[30];
		char Album[30];
		char Year[4];
		char Comment[30];
		PXMPEGGenre Genre;
	}
	MPEGAudioTag;

	typedef struct PXMP3Header_
	{
		PXMP3MPEGVersion MPEGVersion;
		PXMP3Layer Layer;
		PXBool CRCErrorProtection;

		PXInt32U BitRate;
		PXInt16U SampleRate;
		PXBool UsePadding;
		PXBool IsPrivate; // Unused
		PXMP3ChannelMode ChannelMode;
		PXBool AudioModeIntensity;
		PXBool AudioModeMS;

		PXBool Copyrighted;
		PXBool CopyOfOriginal;
		PXMP3Emphasis Emphasis;

		PXInt32U FrameLength;
	}
	PXMP3Header;

	typedef struct PXMP3_
	{
		PXID3 PXID3Info;
		PXMP3Header Header;
	}
	PXMP3;

	PXPrivate inline PXInt8U PXMPEGGenreToID(const PXMPEGGenre mpegGenre);
	PXPrivate inline PXMPEGGenre PXMPEGGenreFromID(const PXInt8U mpegGenre);

	PXPublic PXActionResult PXMP3LoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXMP3SaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif