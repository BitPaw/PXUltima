#include "PXMP3.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <Media/XingInfo/PXXingInfo.h>
#include <Media/LAME/PXLAME.h>

#define MPEGGenreIDBlues 0u
#define MPEGGenreIDClassicRock 1u
#define MPEGGenreIDCountry 2u
#define MPEGGenreIDDance 3u
#define MPEGGenreIDDisco 4u
#define MPEGGenreIDFunk 5u
#define MPEGGenreIDGrunge 6u
#define MPEGGenreIDHipHop 7u
#define MPEGGenreIDJazz 8u
#define MPEGGenreIDMetal 9u
#define MPEGGenreIDNewAge 10u
#define MPEGGenreIDOldies 11u
#define MPEGGenreIDOther 12u
#define MPEGGenreIDPop 13u
#define MPEGGenreIDRAndB 14u
#define MPEGGenreIDRap 15u
#define MPEGGenreIDReggae 16u
#define MPEGGenreIDRock 17u
#define MPEGGenreIDTechno 18u
#define MPEGGenreIDIndustrial 19u
#define MPEGGenreIDAlternative 20u
#define MPEGGenreIDSka 21u
#define MPEGGenreIDDeathMetal 22u
#define MPEGGenreIDPranks 23u
#define MPEGGenreIDSoundtrack 24u
#define MPEGGenreIDEuroTechno 25u
#define MPEGGenreIDAmbient 26u
#define MPEGGenreIDTripHop 27u
#define MPEGGenreIDVocal 28u
#define MPEGGenreIDJazzFunk 29u
#define MPEGGenreIDFusion 30u
#define MPEGGenreIDTrance 31u
#define MPEGGenreIDClassical 32u
#define MPEGGenreIDInstrumental 33u
#define MPEGGenreIDAcid 34u
#define MPEGGenreIDHouse 35u
#define MPEGGenreIDGame 36u
#define MPEGGenreIDSoundClip 37u
#define MPEGGenreIDGospel 38u
#define MPEGGenreIDNoise 39u
#define MPEGGenreIDAlternRock 40u
#define MPEGGenreIDBass 41u
#define MPEGGenreIDSoul 42u
#define MPEGGenreIDPunk 43u
#define MPEGGenreIDSpace 44u
#define MPEGGenreIDMeditative 45u
#define MPEGGenreIDInstrumentalPop 46u
#define MPEGGenreIDInstrumentalRock 47u
#define MPEGGenreIDEthnic 48u
#define MPEGGenreIDGothic 49u
#define MPEGGenreIDDarkwave 50u
#define MPEGGenreIDTechnoIndustrial 51u
#define MPEGGenreIDElectronic 52u
#define MPEGGenreIDPopFolk 53u
#define MPEGGenreIDEurodance 54u
#define MPEGGenreIDDream 55u
#define MPEGGenreIDSouthernRock 56u
#define MPEGGenreIDComedy 57u
#define MPEGGenreIDCult 58u
#define MPEGGenreIDGangsta 59u
#define MPEGGenreIDTop40 60u
#define MPEGGenreIDChristianRap 61u
#define MPEGGenreIDPopFunk 62u
#define MPEGGenreIDJungle 63u
#define MPEGGenreIDNativeAmerican 64u
#define MPEGGenreIDCabaret 65u
#define MPEGGenreIDNewWave 66u
#define MPEGGenreIDPsychadelic 67u
#define MPEGGenreIDRave 68u
#define MPEGGenreIDShowtunes 69u
#define MPEGGenreIDTrailer 70u
#define MPEGGenreIDLoFi 71u
#define MPEGGenreIDTribal 72u
#define MPEGGenreIDAcidPunk 73u
#define MPEGGenreIDAcidJazz 74u
#define MPEGGenreIDPolka 75u
#define MPEGGenreIDRetro 76u
#define MPEGGenreIDMusical 77u
#define MPEGGenreIDRockAndRoll 78u
#define MPEGGenreIDHardRock 79u
#define MPEGGenreIDFolk 80u
#define MPEGGenreIDFolkRock 81u
#define MPEGGenreIDNationalFolk 82u
#define MPEGGenreIDSwing 83u
#define MPEGGenreIDFastFusion 84u
#define MPEGGenreIDBebob 85u
#define MPEGGenreIDLatin 86u
#define MPEGGenreIDRevival 87u
#define MPEGGenreIDCeltic 88u
#define MPEGGenreIDBluegrass 89u
#define MPEGGenreIDAvantgarde 90u
#define MPEGGenreIDGothicRock 91u
#define MPEGGenreIDProgressiveRock 92u
#define MPEGGenreIDPsychedelicRock 93u
#define MPEGGenreIDSymphonicRock 94u
#define MPEGGenreIDSlowRock 95u
#define MPEGGenreIDBigBand 96u
#define MPEGGenreIDChorus 97u
#define MPEGGenreIDEasyListening 98u
#define MPEGGenreIDAcoustic 99u
#define MPEGGenreIDHumour 100u
#define MPEGGenreIDSpeech 101u
#define MPEGGenreIDChanson 102u
#define MPEGGenreIDOpera 103u
#define MPEGGenreIDChamberMusic 104u
#define MPEGGenreIDSonata 105u
#define MPEGGenreIDSymphony 106u
#define MPEGGenreIDBootyBrass 107u
#define MPEGGenreIDPrimus 108u
#define MPEGGenreIDPornGroove 109u
#define MPEGGenreIDSatire 110u
#define MPEGGenreIDSlowJam 111u
#define MPEGGenreIDClub 112u
#define MPEGGenreIDTango 113u
#define MPEGGenreIDSamba 114u
#define MPEGGenreIDFolklore 115u
#define MPEGGenreIDBallad 116u
#define MPEGGenreIDPoweerBallad 117u
#define MPEGGenreIDRhytmicSoul 118u
#define MPEGGenreIDFreestyle 119u
#define MPEGGenreIDDuet 120u
#define MPEGGenreIDPunkRock 121u
#define MPEGGenreIDDrumSolo 122u
#define MPEGGenreIDACapela 123u
#define MPEGGenreIDEuroHouse 124u
#define MPEGGenreIDDanceHall 125u

const char PXMP3ChunkTag[4] = { 'L', 'a', 'v', 'c' };

PXMPEGGenre PXAPI PXMPEGGenreFromID(const PXInt8U mpegGenre)
{
	switch(mpegGenre)
	{
		case MPEGGenreIDBlues:
			return Blues;

		case MPEGGenreIDClassicRock:
			return ClassicRock;

		case MPEGGenreIDCountry:
			return Country;

		case MPEGGenreIDDance:
			return Dance;

		case MPEGGenreIDDisco:
			return Disco;

		case MPEGGenreIDFunk:
			return Funk;

		case MPEGGenreIDGrunge:
			return Grunge;

		case MPEGGenreIDHipHop:
			return HipHop;

		case MPEGGenreIDJazz:
			return  Jazz;

		case MPEGGenreIDMetal:
			return  Metal;

		case MPEGGenreIDNewAge:
			return  NewAge;

		case MPEGGenreIDOldies:
			return  Oldies;

		case MPEGGenreIDOther:
			return  Other;

		case MPEGGenreIDPop:
			return  Pop;

		case MPEGGenreIDRAndB:
			return  RAndB;

			/*
		case MPEGGenreIDRap 15:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDReggae 16:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDRock 17:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTechno 18:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDIndustrial 19:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAlternative 20:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSka 21:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDDeathMetal 22:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPranks 23:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSoundtrack 24:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDEuroTechno 25:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAmbient 26:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTripHop 27:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDVocal 28:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDJazzFunk 29:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDFusion 30:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTrance 31:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDClassical 32:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDInstrumental 33:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAcid 34:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDHouse 35:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDGame 36:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSoundClip 37:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDGospel 38:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDNoise 39:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAlternRock 40:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDBass 41:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSoul 42:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPunk 43:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSpace 44:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDMeditative 45:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDInstrumentalPop 46:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDInstrumentalRock 47:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDEthnic 48:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDGothic 49:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDDarkwave 50:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTechnoIndustrial 51:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDElectronic 52:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPopFolk 53:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDEurodance 54:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDDream 55:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSouthernRock 56:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDComedy 57:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDCult 58:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDGangsta 59:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTop40 60:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDChristianRap 61:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPopFunk 62:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDJungle 63:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDNativeAmerican 64:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDCabaret 65:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDNewWave 66:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPsychadelic 67:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDRave 68:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDShowtunes 69:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTrailer 70:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDLoFi 71:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDTribal 72:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAcidPunk 73:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAcidJazz 74:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPolka 75:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDRetro 76:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDMusical 77:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDRockAndRoll 78:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDHardRock 79:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDFolk 80:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDFolkRock 81:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDNationalFolk 82:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSwing 83:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDFastFusion 84:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDBebob 85:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDLatin 86:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDRevival 87:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDCeltic 88:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDBluegrass 89:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAvantgarde 90:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDGothicRock 91:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDProgressiveRock 92:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPsychedelicRock 93:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSymphonicRock 94:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSlowRock 95:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDBigBand 96:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDChorus 97:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDEasyListening 98:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDAcoustic 99:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDHumour 100:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSpeech 101:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDChanson 102:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDOpera 103:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDChamberMusic 104:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSonata 105:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSymphony 106:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDBootyBrass 107:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPrimus 108:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDPornGroove 109:
			return  BF::MPEGGenre : ;

		case MPEGGenreIDSatire 110:
			return  BF::MPEGGenre : ;
			*/

		case MPEGGenreIDSlowJam:
			return  SlowJam;

		case MPEGGenreIDClub:
			return  Club;

		case MPEGGenreIDTango:
			return  Tango;

		case MPEGGenreIDSamba:
			return  Samba;

		case MPEGGenreIDFolklore:
			return  Folklore;

		case MPEGGenreIDBallad:
			return  Ballad;

		case MPEGGenreIDPoweerBallad:
			return  PoweerBallad;

		case MPEGGenreIDRhytmicSoul:
			return  RhytmicSoul;

		case MPEGGenreIDFreestyle:
			return  Freestyle;

		case MPEGGenreIDDuet:
			return  Duet;

		case MPEGGenreIDPunkRock:
			return  PunkRock;

		case MPEGGenreIDDrumSolo:
			return  DrumSolo;

		case MPEGGenreIDACapela:
			return  ACapela;

		case MPEGGenreIDEuroHouse:
			return  EuroHouse;

		case MPEGGenreIDDanceHall:
			return DanceHall;

		default:
			return MPEGGenreUnknown;

	}
}

PXInt8U PXAPI PXMPEGGenreToID(const PXMPEGGenre mpegGenre)
{
	switch(mpegGenre)
	{
		case Blues:
			return MPEGGenreIDBlues;

		case ClassicRock:
			return MPEGGenreIDClassicRock;

		case Country:
			break;
		case Dance:
			break;
		case Disco:
			break;
		case Funk:
			break;
		case Grunge:
			break;
		case HipHop:
			break;
		case Jazz:
			break;
		case Metal:
			break;
		case NewAge:
			break;
		case Oldies:
			break;
		case Other:
			break;
		case Pop:
			break;
		case RAndB:
			break;
		case Rap:
			break;
		case Reggae:
			break;
		case Rock:
			break;
		case Techno:
			break;
		case Industrial:
			break;
		case Alternative:
			break;
		case Ska:
			break;
		case DeathMetal:
			break;
		case Pranks:
			break;
		case Soundtrack:
			break;
		case EuroTechno:
			break;
		case Ambient:
			break;
		case TripHop:
			break;
		case Vocal:
			break;
		case JazzFunk:
			break;
		case Fusion:
			break;
		case Trance:
			break;
		case Classical:
			break;
		case Instrumental:
			break;
		case Acid:
			break;
		case House:
			break;
		case Game:
			break;
		case SoundClip:
			break;
		case Gospel:
			break;
		case Noise:
			break;
		case AlternRock:
			break;
		case Bass:
			break;
		case Soul:
			break;
		case Punk:
			break;
		case Space:
			break;
		case Meditative:
			break;
		case InstrumentalPop:
			break;
		case InstrumentalRock:
			break;
		case Ethnic:
			break;
		case Gothic:
			break;
		case Darkwave:
			break;
		case TechnoIndustrial:
			break;
		case Electronic:
			break;
		case PopFolk:
			break;
		case Eurodance:
			break;
		case Dream:
			break;
		case SouthernRock:
			break;
		case Comedy:
			break;
		case Cult:
			break;
		case Gangsta:
			break;
		case Top40:
			break;
		case ChristianRap:
			break;
		case PopFunk:
			break;
		case Jungle:
			break;
		case NativeAmerican:
			break;
		case Cabaret:
			break;
		case NewWave:
			break;
		case Psychadelic:
			break;
		case Rave:
			break;
		case Showtunes:
			break;
		case Trailer:
			break;
		case LoFi:
			break;
		case Tribal:
			break;
		case AcidPunk:
			break;
		case AcidJazz:
			break;
		case Polka:
			break;
		case Retro:
			break;
		case Musical:
			break;
		case RockAndRoll:
			break;
		case HardRock:
			break;
		case Folk:
			break;
		case FolkRock:
			break;
		case NationalFolk:
			break;
		case Swing:
			break;
		case FastFusion:
			break;
		case Bebob:
			break;
		case Latin:
			break;
		case Revival:
			break;
		case Celtic:
			break;
		case Bluegrass:
			break;
		case Avantgarde:
			break;
		case GothicRock:
			break;
		case ProgressiveRock:
			break;
		case PsychedelicRock:
			break;
		case SymphonicRock:
			break;
		case SlowRock:
			break;
		case BigBand:
			break;
		case Chorus:
			break;
		case EasyListening:
			break;
		case Acoustic:
			break;
		case Humour:
			break;
		case Speech:
			break;
		case Chanson:
			break;
		case Opera:
			break;
		case ChamberMusic:
			break;
		case Sonata:
			break;
		case Symphony:
			break;
		case BootyBrass:
			break;
		case Primus:
			break;
		case PornGroove:
			break;
		case Satire:
			break;
		case SlowJam:
			break;
		case Club:
			break;
		case Tango:
			break;
		case Samba:
			break;
		case Folklore:
			break;
		case Ballad:
			break;
		case PoweerBallad:
			break;
		case RhytmicSoul:
			break;
		case Freestyle:
			break;
		case Duet:
			break;
		case PunkRock:
			break;
		case DrumSolo:
			break;
		case ACapela:
			break;
		case EuroHouse:
			break;
		case DanceHall:
			return -1;
	}

	return -1; // MPEGGenreUnknown
}

PXActionResult PXAPI PXMP3LoadFromFile(PXSound* const pxSound, PXFile* const pxFile)
{
	PXMP3 pxMP3;
	
	PXMP3* mp3 = &pxMP3;

	PXClear(PXMP3, mp3);

	{

		const PXActionResult actionResult = PXID3LoadFromFile(&mp3->PXID3Info, pxFile);

		PXActionReturnOnError(actionResult);
	}

	while (!PXFileIsAtEnd(pxFile))
	{
		PXXingInfo xingInfo;

		PXClear(PXXingInfo, &xingInfo);

		PXSize cursorPositionPredict = 0;

		// Parse mp3
		{
			const PXByte* const mp3HeaderDataBlock = (const PXByte* const)PXFileCursorPosition(pxFile);

			// Parse Byte 1/4
			{
				const PXBool validSignature = (mp3HeaderDataBlock[0] == 0xFF) && ((mp3HeaderDataBlock[1] & 0xF0) == 0xF0);

				if (!validSignature)
				{
					return PXActionRefusedInvalidHeaderSignature;
				}
			}

			// Parse Byte 2/4
			{
				const PXByte byteB = mp3HeaderDataBlock[1];
				const PXByte mpegAudioVersionID = (byteB & 0b00011000) >> 3;
				const PXByte layerID = (byteB & 0b00000110) >> 1;

				mp3->Header.CRCErrorProtection = (byteB & 0b00000001);

				switch (mpegAudioVersionID)
				{
					default:
					case 0b00:
						mp3->Header.MPEGVersion = MPEGVersion25;
						break;

					case 0b01:
						mp3->Header.MPEGVersion = MPEGVersionReserved;
						break;

					case 0b10:
						mp3->Header.MPEGVersion = MPEGVersion2;
						break;

					case 0b11:
						mp3->Header.MPEGVersion = MPEGVersion1;
						break;
				}

				switch (layerID)
				{
					default:
					case 0b00:
						mp3->Header.Layer = PXMP3LayerReserved;
						break;

					case 0b01:
						mp3->Header.Layer = PXMP3LayerIII;
						break;

					case 0b10:
						mp3->Header.Layer = PXMP3LayerII;
						break;

					case 0b11:
						mp3->Header.Layer = PXMP3LayerI;
						break;
				}
			}

			// Parse Byte 3/3
			{
				const PXByte byteC = mp3HeaderDataBlock[2];
				const PXBool isMPEGVersion1 = mp3->Header.MPEGVersion == MPEGVersion1;
				const PXBool isMPEGVersion2 = mp3->Header.MPEGVersion == MPEGVersion2;
				const PXBool isMPEGVersion25 = mp3->Header.MPEGVersion == MPEGVersion25;
				const PXBool isLayer1 = mp3->Header.Layer == PXMP3LayerI;
				const PXBool isLayer2 = mp3->Header.Layer == PXMP3LayerII;
				const PXBool isLayer3 = mp3->Header.Layer == PXMP3LayerIII;

				// Bit lookup
				{
					const PXByte bitRateTag = (byteC & 0b11110000) >> 4;

					const PXInt16U bad = 0x00;
					const PXInt16U bitRateKBpsLookup[16][5] =
					{
						{bad, 	bad, 	bad ,	bad ,	bad},
						{  32, 	32, 	32 ,	32 ,	8},
						{  64, 	48, 	40 ,	48 ,	16},
						{  96, 	56, 	48 ,	56 ,	24},
						{ 128, 	64, 	56 ,	64 ,	32},
						{ 160, 	80, 	64 ,	80 ,	48},
						{ 192, 	96, 	80 ,	96 ,	48},
						{ 224, 	112, 	96 ,	112 ,	56},
						{ 256, 	128, 	112 ,	128 ,	64 },
						{ 288, 	160, 	128 ,	144 ,	80 },
						{ 320, 	192, 	160 ,	160 ,	96 },
						{ 352, 	224, 	192 ,	176 ,	112 },
						{ 384, 	256, 	224 ,	192 ,	128 },
						{ 416, 	320, 	256 ,	224 ,	144 },
						{ 448, 	384, 	320 ,	256 ,	160 },
						{ bad, 	bad, 	bad ,	bad ,	bad }
					};

					const PXInt8U yPos = bitRateTag;
					const PXInt8U xPos =
						0 * (isMPEGVersion1 && isLayer1) +
						1 * (isMPEGVersion1 && isLayer2) +
						2 * (isMPEGVersion1 && isLayer3) +
						3 * (isMPEGVersion2 && isLayer1) +
						4 * ((isMPEGVersion2 || isMPEGVersion25) && (isLayer2 || isLayer3));

					const PXInt16U bitRateKBps = bitRateKBpsLookup[yPos][xPos];

					mp3->Header.BitRate = (PXInt32U)bitRateKBps * 1000u;
				}

				//For Layer II there are some combinations of bitrate and mode which are not allowed.

				{
					const unsigned char sampleRateID = (byteC & 0b00001100) >> 2;

					const unsigned short reserverdValue = -1;
					const unsigned short sampleRateLookup[4][3] =
					{
						{	44100, 22050, 11025 },
						{	48000, 24000, 12000 },
						{	32000, 16000, 8000 },
						{	reserverdValue,	reserverdValue,	reserverdValue }
					};

					unsigned char yPos = sampleRateID;
					unsigned char xPos =
						0 * isMPEGVersion1 +
						1 * isMPEGVersion2 +
						2 * isMPEGVersion25;


					const unsigned short sampleRateKBps = sampleRateLookup[yPos][xPos];

					mp3->Header.SampleRate = sampleRateKBps;
				}

				mp3->Header.UsePadding = (byteC & 0b00000010) >> 1;
				mp3->Header.IsPrivate = (byteC & 0b00000001);
			}

			// Parse Byte 4/4
			{
				const PXByte byteD = mp3HeaderDataBlock[3];

				const PXByte channalMode = (byteD & 0b11000000) >> 6;
				mp3->Header.AudioModeIntensity = (byteD & 0b00100000) >> 5;
				mp3->Header.AudioModeMS = (byteD & 0b00010000) >> 4;
				mp3->Header.Copyrighted = (byteD & 0b00001000) >> 3;
				mp3->Header.CopyOfOriginal = (byteD & 0b00000100) >> 2;
				const PXByte emphasis = (byteD & 0b00000011);

				switch (channalMode)
				{
					default:
					case 0b00:
						mp3->Header.ChannelMode = PXMP3ChannelStereo;
						break;

					case 0b01:
						mp3->Header.ChannelMode = PXMP3ChannelJointStereo;
						break;

					case 0b10:
						mp3->Header.ChannelMode = PXMP3ChannelDual;
						break;

					case 0b11:
						mp3->Header.ChannelMode = PXMP3ChannelSingle;
						break;
				}

				switch (emphasis)
				{
					default:
					case 0b00:
						mp3->Header.Emphasis = PXMP3EmphasisNone;
						break;

					case 0b01:
						mp3->Header.Emphasis = PXMP3Emphasisms5015;
						break;

					case 0b10:
						mp3->Header.Emphasis = PXMP3EmphasisReserved;
						break;

					case 0b11:
						mp3->Header.Emphasis = PXMP3EmphasisCCITJ17;
						break;
				}
			}

			const float bitratePerSample = mp3->Header.BitRate / ((float)mp3->Header.SampleRate + mp3->Header.UsePadding);

			switch (mp3->Header.Layer)
			{
				case PXMP3LayerI:
				{
					mp3->Header.FrameLength = ((12 * bitratePerSample) * 4) - 4;
					break;
				}
				case PXMP3LayerII:
				case PXMP3LayerIII:
				{
					mp3->Header.FrameLength = (144 * bitratePerSample) - 4 + mp3->Header.UsePadding;
					break;
				}
				case PXMP3LayerReserved:
				default:
				{
					mp3->Header.FrameLength = 0;
					break;
				}
			}

			PXFileCursorAdvance(pxFile, 4u);

			// Header parsing finished..








			//cursorPositionPredict = pxFile.DataCursor + mp3Header.FrameLength;

			PXFileCursorAdvance(pxFile, 32u);

#if PXMP3Debug
			printf
			(
				"[PXMP3][Frame] Bitrate : %6i | SampleRate : %5i | FrameLength : %5i |\n",
				mp3->Header.BitRate,
				mp3->Header.SampleRate,
				mp3->Header.FrameLength
			);
#endif
		}

		// info header
		{
			const PXActionResult actionResult = PXXingInfoParse(&xingInfo, pxFile);

			PXActionReturnOnError(actionResult);

#if PXMP3Debug
			printf
			(
				"[PXMP3][Info] Number of Frames : %6i | %6i Bytes |\n",
				xingInfo.NumberOfFrames,
				xingInfo.SizeInBytes
			);
#endif
		}

		// LACA??
		{
			const unsigned char isTag = PXFileReadAndCompare(pxFile, PXMP3ChunkTag, sizeof(PXMP3ChunkTag));

			if (isTag)
			{
#if PXMP3Debug
				printf
				(
					"[PXMP3][???] Lavc chunk... what is that? |\n"
				);
#endif

				PXFileCursorAdvance(pxFile, 257u);

				continue; // After this header there is a PXMP3 header next, so parse it.
			}
		}


		{
			PXLAME lame;

			const PXActionResult actionResult = PXLAMELoadFromFile(&lame, pxFile);

			PXActionReturnOnError(actionResult);

#if PXMP3Debug
			printf
			(
				"[PXMP3][PXLAME] Chunk |\n"
			);
#endif

		}


		{
			const PXBool tagDetected = PXFileReadAndCompare(pxFile, "TAG", 3u);

			if (tagDetected)
			{
				const PXSize offset = PXFileRemainingSize(pxFile);

				// I currently dont know what this is.
				// But it comes at the end of the file.. so i am finished?

				PXFileCursorAdvance(pxFile, offset);
			}
		}

		// Check if reader is still alligned
		{
			const PXBool isAlligned = cursorPositionPredict == pxFile->DataCursor;

			if (!isAlligned)
			{
				int offset = cursorPositionPredict - pxFile->DataCursor;

#if PXMP3Debug
				printf("[PXMP3] detected failed allignment! Off by : %i Bytes\n", offset);
#endif

				pxFile->DataCursor = cursorPositionPredict;
				//pxFile.CursorAdvance(mp3Header.FrameLength);
			}
		}
	}

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMP3SaveToFile(PXSound* const pxSound, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}