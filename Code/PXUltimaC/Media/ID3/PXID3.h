#ifndef PXID3Include
#define PXID3Include

#include "../PXResource.h"

#define PXID3Debug 0

#define PXID3TitleSize 30
#define PXID3ArtistSize 30
#define PXID3AlbumSize 30
#define PXID3YearSize 4
#define PXID3CommentSize 30

typedef enum PXID3Version_
{
	PXID3VersionInvalid,
	PXID3Versionv1x0,
	PXID3Versionv1x1,
	PXID3Versionv2x0,
	PXID3Versionv2x2,
	PXID3Versionv2x3,
	PXID3Versionv2x4
}
PXID3Version;

typedef enum PXID3v2xFrameTag_
{
	PXID3v2xFrameTagInvalid,
	PXID3v2xFrameTagAudioEncryption, // 4.20    AENC [[#sec4.20 | ]]
	PXID3v2xFrameTagAttachedPicture,//4.15    APIC[#sec4.15 ]
	PXID3v2xFrameTagComments,//4.11    COMM[#sec4.11 Comments]
	PXID3v2xFrameTagCommercialFrame,//4.25    COMR[#sec4.25 Commercial frame]
	PXID3v2xFrameTagEncryptionMethodRegistration,//4.26    ENCR[#sec4.26 Encryption method registration]
	PXID3v2xFrameTagEqualization,//4.13    EQUA[#sec4.13 Equalization]
	PXID3v2xFrameTagEventTimingCodes,//4.6     ETCO[#sec4.6 Event timing codes]
	PXID3v2xFrameTagGeneralEncapsulatedObject,//4.16    GEOB[#sec4.16 General encapsulated object]
	PXID3v2xFrameTagGroupIdentificationRegistration,//4.27    GRID[#sec4.27 Group identification registration]
	PXID3v2xFrameTagInvolvedPeopleList,//4.4     IPLS[#sec4.4 Involved people list]
	PXID3v2xFrameTagLinkedInformation,//4.21    LINK[#sec4.21 Linked information]
	PXID3v2xFrameTagMusicCDIdentifier,//4.5     MCDI[#sec4.5 Music CD identifier]
	PXID3v2xFrameTagMPEGLocationLookupTable,//4.7     MLLT[#sec4.7 MPEG location lookup table]
	PXID3v2xFrameTagOwnershipFrame,//4.24    OWNE[#sec4.24 Ownership frame]
	PXID3v2xFrameTagPrivateFrame,//4.28    PRIV[#sec4.28 Private frame]
	PXID3v2xFrameTagPlayCounter,//4.17    PCNT[#sec4.17 Play counter]
	PXID3v2xFrameTagPopularimeter,//4.18    POPM[#sec4.18 Popularimeter]
	PXID3v2xFrameTagPositionSynchronisationFrame,//4.22    POSS[#sec4.22 Position synchronisation frame]
	PXID3v2xFrameTagRecommendedBufferSize,//4.19    RBUF[#sec4.19 Recommended buffer size]
	PXID3v2xFrameTagRelativeVolumeAdjustment,//4.12    RVAD[#sec4.12 Relative volume adjustment]
	PXID3v2xFrameTagReverb,//4.14    RVRB[#sec4.14 Reverb]
	PXID3v2xFrameTagSynchronizedLyric,//4.10    SYLT[#sec4.10 Synchronized lyric / text]
	PXID3v2xFrameTagSynchronizedTempocodes,//4.8     SYTC[#sec4.8 Synchronized tempo codes]
	PXID3v2xFrameTagAlbum,//4.2.1   TALB[#TALB Album / Movie / Show title]
	PXID3v2xFrameTagBeatsPerMinute,//4.2.1   TBPM[#TBPM BPM(beats per minute)]
	PXID3v2xFrameTagComposer,//4.2.1   TCOM[#TCOM Composer]
	PXID3v2xFrameTagContentType,//4.2.1   TCON[#TCON Content type]
	PXID3v2xFrameTagCopyrightMessage,//4.2.1   TCOP[#TCOP Copyright message]
	PXID3v2xFrameTagDate,//4.2.1   TDAT[#TDAT Date]
	PXID3v2xFrameTagPlaylistDelay,//4.2.1   TDLY[#TDLY Playlist delay]
	PXID3v2xFrameTagTaggingTime, // TDTG
	PXID3v2xFrameTagEncodedBy,//4.2.1   TENC[#TENC Encoded by]
	PXID3v2xFrameTagLyricist,//4.2.1   TEXT[#TEXT Lyricist / Text writer]
	PXID3v2xFrameTagFileType,//4.2.1   TFLT[#TFLT File type]
	PXID3v2xFrameTagTime,//4.2.1   TIME[#TIME Time]
	PXID3v2xFrameTagContentGroupDescription,//4.2.1   TIT1[#TIT1 Content group description]
	PXID3v2xFrameTagTitle,//4.2.1   TIT2[#TIT2 Title / songname / content description]
	PXID3v2xFrameTagSubtitle,//4.2.1   TIT3[#TIT3 Subtitle / Description refinement]
	PXID3v2xFrameTagInitialKey,//4.2.1   TKEY[#TKEY Initial key]
	PXID3v2xFrameTagLanguage,//4.2.1   TLAN[#TLAN Language(s)]
	PXID3v2xFrameTagLength,//4.2.1   TLEN[#TLEN Length]
	PXID3v2xFrameTagMediaType,//4.2.1   TMED[#TMED Media type]
	PXID3v2xFrameTagOriginalAlbum,//4.2.1   TOAL[#TOAL Original album / movie / show title]
	PXID3v2xFrameTagOriginalFilename,//4.2.1   TOFN[#TOFN Original filename]
	PXID3v2xFrameTagOriginalLyricist,//4.2.1   TOLY[#TOLY Original lyricist(s) / text writer(s)]
	PXID3v2xFrameTagOriginalArtist,//4.2.1   TOPE[#TOPE Original artist(s) / performer(s)]
	PXID3v2xFrameTagOriginalReleaseYear,//4.2.1   TORY[#TORY Original release year]
	PXID3v2xFrameTagFileOwner,//4.2.1   TOWN[#TOWN File owner / licensee]
	PXID3v2xFrameTagLeadPerformer,//4.2.1   TPE1[#TPE1 Lead performer(s) / Soloist(s)]
	PXID3v2xFrameTagBand,//4.2.1   TPE2[#TPE2 Band / orchestra / accompaniment]
	PXID3v2xFrameTagConductor,//4.2.1   TPE3[#TPE3 Conductor / performer refinement]
	PXID3v2xFrameTagInterpretedBy,//4.2.1   TPE4[#TPE4 Interpreted, remixed, or otherwise modified by]
	PXID3v2xFrameTagPartOfASet,//4.2.1   TPOS[#TPOS Part of a set]
	PXID3v2xFrameTagPublisher,//4.2.1   TPUB[#TPUB Publisher]
	PXID3v2xFrameTagTrackNumber,//4.2.1   TRCK[#TRCK Track number / Position in set]
	PXID3v2xFrameTagRecordingDates,//4.2.1   TRDA[#TRDA Recording dates]
	PXID3v2xFrameTagInternetRadioStationName,//4.2.1   TRSN[#TRSN Internet radio station name]
	PXID3v2xFrameTagInternetRadioStationOwner,//4.2.1   TRSO[#TRSO Internet radio station owner]
	PXID3v2xFrameTagSize,//4.2.1   TSIZ[#TSIZ Size]
	PXID3v2xFrameTagInternationalStandardRecordingCode,//4.2.1   TSRC[#TSRC ISRC(international standard recording code)]
	PXID3v2xFrameTagSoftwareHardwaresettingsUsedForEncoding,//4.2.1   TSSE[#TSEE Software / Hardware and settings used for encoding]
	PXID3v2xFrameTagYear,//4.2.1   TYER[#TYER Year]
	PXID3v2xFrameTagUserDefinedText,//4.2.2   TXXX[#TXXX User defined text information frame]
	PXID3v2xFrameTagUniqueFileIdentifier,//4.1     UFID[#sec4.1 Unique file identifier]
	PXID3v2xFrameTagTermsOfUse,//4.23    USER[#sec4.23 Terms of use]
	PXID3v2xFrameTagUnsychronizedLyric,//4.9     USLT[#sec4.9 Unsychronized lyric / text transcription]
	PXID3v2xFrameTagCommercialInformation,//4.3.1   WCOM[#WCOM Commercial information]
	PXID3v2xFrameTagCopyrightLegalInformation,//4.3.1   WCOP[#WCOP Copyright / Legal information]
	PXID3v2xFrameTagOfficialAudioFileWebpage,//4.3.1   WOAF[#WOAF Official audio file webpage]
	PXID3v2xFrameTagOfficialArtistPerformerWebpage,//4.3.1   WOAR[#WOAR Official artist / performer webpage]
	PXID3v2xFrameTagOfficialAudioSourceWebpage,//4.3.1   WOAS[#WOAS Official audio source webpage]
	PXID3v2xFrameTagInternetRadioStationHomepage,//4.3.1   WORS[#WORS Official internet radio station homepage]
	PXID3v2xFrameTagPayment,//4.3.1   WPAY[#WPAY Payment]
	PXID3v2xFrameTagPublishersOfficialWebpage,//4.3.1   WPUB[#WPUB Publishers official webpage]
	PXID3v2xFrameTagUserDefinedURLLinkFrame//4.3.2   WXXX[#WXXX User defined URL link frame]
}
PXID3v2xFrameTag;

typedef struct PXID3_
{
	// Version 1.0
	char Title[PXID3TitleSize + 1];
	char Artist[PXID3ArtistSize + 1];
	char Album[PXID3AlbumSize + 1];
	char Year[PXID3YearSize + 1];
	char Comment[PXID3CommentSize + 1];
	PXInt8U Genre;

	// Version 1.1
	PXInt8U TrackID;

	// Version 2.x
	PXInt32U BeatsPerMinute;
	char Composer[PXID3ArtistSize + 1];
}
PXID3;

PXPrivate inline PXID3v2xFrameTag PXAPI PXID3v2xFrameTagFromID(const PXInt32U id3v2xFrameTagID);

PXPublic PXActionResult PXAPI PXID3LoadFromFile(PXID3* const id3, PXFile* const PXFile);
PXPublic PXActionResult PXAPI PXID3SaveToFile(const PXID3* const id3, PXFile* const PXFile);

#endif