#ifndef PXMP3INCLUDE
#define PXMP3INCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>
#include <Media/ID3/PXID3.h>

#define PXMP3Debug 1

#ifdef __cplusplus
extern "C"
{
#endif
	typedef enum MPEGGenre_
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
	MPEGGenre;

	typedef struct MPEGAudioTag
	{
		// char[3] == TAG
		char Title[30];
		char Artist[30];
		char Album[30];
		char Year[4];
		char Comment[30];
		MPEGGenre Genre;
	};

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

	
	static MPEGGenre ConvertMPEGToGenre(const unsigned char mpegGenre);
	static unsigned char ConvertMPEGFromGenre(const MPEGGenre mpegGenre);

	PXPublic PXActionResult PXMP3Parse(PXMP3* const mp3, PXFile* const PXFile);


#ifdef __cplusplus
}
#endif

#endif