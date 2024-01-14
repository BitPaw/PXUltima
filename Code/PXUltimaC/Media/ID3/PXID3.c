#include "PXID3.h"

#include <Media/PXText.h>

const char PXID3HeaderSignature[3] = { 'I','D','3' };

#if PXID3Debug
#include <cstdio>
#endif

PXID3v2xFrameTag PXID3v2xFrameTagFromID(const PXInt32U id3v2xFrameTagID)
{
    switch(id3v2xFrameTagID)
    {
        case PXInt32Make('A', 'E', 'N', 'C'): return AudioEncryption;
        case PXInt32Make('A', 'P', 'I', 'C'): return AttachedPicture;
        case PXInt32Make('C', 'O', 'M', 'M'): return Comments;
        case PXInt32Make('C', 'O', 'M', 'R'): return CommercialFrame;
        case PXInt32Make('E', 'N', 'C', 'R'): return EncryptionMethodRegistration;
        case PXInt32Make('E', 'Q', 'U', 'A'): return Equalization;
        case PXInt32Make('E', 'T', 'C', 'O'): return EventTimingCodes;
        case PXInt32Make('G', 'E', 'O', 'B'): return GeneralEncapsulatedObject;
        case PXInt32Make('G', 'R', 'I', 'D'): return GroupIdentificationRegistration;
        case PXInt32Make('I', 'P', 'L', 'S'): return InvolvedPeopleList;
        case PXInt32Make('L', 'I', 'N', 'K'): return LinkedInformation;
        case PXInt32Make('M', 'C', 'D', 'I'): return MusicCDIdentifier;
        case PXInt32Make('M', 'L', 'L', 'T'): return MPEGLocationLookupTable;
        case PXInt32Make('O', 'W', 'N', 'E'): return OwnershipFrame;
        case PXInt32Make('P', 'R', 'I', 'V'): return PrivateFrame;
        case PXInt32Make('P', 'C', 'N', 'T'): return PlayCounter;
        case PXInt32Make('P', 'O', 'P', 'M'): return Popularimeter;
        case PXInt32Make('P', 'O', 'S', 'S'): return PositionSynchronisationFrame;
        case PXInt32Make('R', 'B', 'U', 'F'): return RecommendedBufferSize;
        case PXInt32Make('R', 'V', 'A', 'D'): return RelativeVolumeAdjustment;
        case PXInt32Make('R', 'V', 'R', 'B'): return Reverb;
        case PXInt32Make('S', 'Y', 'L', 'T'): return SynchronizedLyric;
        case PXInt32Make('S', 'Y', 'T', 'C'): return SynchronizedTempocodes;
        case PXInt32Make('T', 'A', 'L', 'B'): return Album;
        case PXInt32Make('T', 'B', 'P', 'M'): return BeatsPerMinute;
        case PXInt32Make('T', 'C', 'O', 'M'): return Composer;
        case PXInt32Make('T', 'C', 'O', 'N'): return ContentType;
        case PXInt32Make('T', 'C', 'O', 'P'): return CopyrightMessage;
        case PXInt32Make('T', 'D', 'A', 'T'): return Date;
        case PXInt32Make('T', 'D', 'L', 'Y'): return PlaylistDelay;
        //case PXInt32Make('T', 'D', 'T', 'G'): return TaggingTime;
        case PXInt32Make('T', 'E', 'N', 'C'): return EncodedBy;
        case PXInt32Make('T', 'E', 'X', 'T'): return Lyricist;
        case PXInt32Make('T', 'F', 'L', 'T'): return FileType;
        case PXInt32Make('T', 'I', 'M', 'E'): return Time;
        case PXInt32Make('T', 'I', 'T', '1'): return ContentGroupDescription;
        case PXInt32Make('T', 'I', 'T', '2'): return Title;
        case PXInt32Make('T', 'I', 'T', '3'): return Subtitle;
        case PXInt32Make('T', 'K', 'E', 'Y'): return InitialKey;
        case PXInt32Make('T', 'L', 'A', 'N'): return Language;
        case PXInt32Make('T', 'L', 'E', 'N'): return Length;
        case PXInt32Make('T', 'M', 'E', 'D'): return MediaType;
        case PXInt32Make('T', 'O', 'A', 'L'): return OriginalAlbum;
        case PXInt32Make('T', 'O', 'F', 'N'): return OriginalFilename;
        case PXInt32Make('T', 'O', 'L', 'Y'): return OriginalLyricist;
        case PXInt32Make('T', 'O', 'P', 'E'): return OriginalArtist;
        case PXInt32Make('T', 'O', 'R', 'Y'): return OriginalReleaseYear;
        case PXInt32Make('T', 'O', 'W', 'N'): return FileOwner;
        case PXInt32Make('T', 'P', 'E', '1'): return LeadPerformer;
        case PXInt32Make('T', 'P', 'E', '2'): return Band;
        case PXInt32Make('T', 'P', 'E', '3'): return Conductor;
        case PXInt32Make('T', 'P', 'E', '4'): return InterpretedBy;
        case PXInt32Make('T', 'P', 'O', 'S'): return PartOfASet;
        case PXInt32Make('T', 'P', 'U', 'B'): return Publisher;
        case PXInt32Make('T', 'R', 'C', 'K'): return TrackNumber;
        case PXInt32Make('T', 'R', 'D', 'A'): return RecordingDates;
        case PXInt32Make('T', 'R', 'S', 'N'): return InternetRadioStationName;
        case PXInt32Make('T', 'R', 'S', 'O'): return InternetRadioStationOwner;
        case PXInt32Make('T', 'S', 'I', 'Z'): return Size;
        case PXInt32Make('T', 'S', 'R', 'C'): return InternationalStandardRecordingCode;
        case PXInt32Make('T', 'S', 'S', 'E'): return SoftwareHardwaresettingsUsedForEncoding;
        case PXInt32Make('T', 'Y', 'E', 'R'): return Year;
        case PXInt32Make('T', 'X', 'X', 'X'): return UserDefinedText;
        case PXInt32Make('U', 'F', 'I', 'D'): return UniqueFileIdentifier;
        case PXInt32Make('U', 'S', 'E', 'R'): return TermsOfUse;
        case PXInt32Make('U', 'S', 'L', 'T'): return UnsychronizedLyric;
        case PXInt32Make('W', 'C', 'O', 'M'): return CommercialInformation;
        case PXInt32Make('W', 'C', 'O', 'P'): return CopyrightLegalInformation;
        case PXInt32Make('W', 'O', 'A', 'F'): return OfficialAudioFileWebpage;
        case PXInt32Make('W', 'O', 'A', 'R'): return OfficialArtistPerformerWebpage;
        case PXInt32Make('W', 'O', 'A', 'S'): return OfficialAudioSourceWebpage;
        case PXInt32Make('W', 'O', 'R', 'S'): return InternetRadioStationHomepage;
        case PXInt32Make('W', 'P', 'A', 'Y'): return Payment;
        case PXInt32Make('W', 'P', 'U', 'B'): return PublishersOfficialWebpage;
        case PXInt32Make('W', 'X', 'X', 'X'): return UserDefinedURLLinkFrame;

        default:
            return PXID3v2xFrameTagInvalid;
    }
}

PXActionResult PXID3LoadFromFile(PXID3* const id3, PXFile* const PXFile)
{
    PXID3Version version = PXID3VersionInvalid;

    //Check header
    {
        // Einen PXID3v2-Block erkennt man am Header, dessen erste fünf Bytes aus der Zeichenkette „PXID3“ und der PXID3v2-Version (z. B. $04 00 für PXID3v2.4) bestehen
        const PXBool isValidHeader = PXFileReadAndCompare(PXFile, PXID3HeaderSignature, sizeof(PXID3HeaderSignature));

        if(!isValidHeader)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    // Check Version
    {
        char versionTag[2];

        PXFileReadB(PXFile, versionTag, 2);

        switch(versionTag[0])
        {
            case 0x00:
                version = PXID3Versionv2x0;
                break;

            case 0x02:
                version = PXID3Versionv2x2;
                break;

            case 0x03:
                version = PXID3Versionv2x3;
                break;

            case 0x04:
                version = PXID3Versionv2x4;
                break;

            default: // Version is probably 1.x
            {
                const PXByte* trackIDAdress = (PXByte*)PXFileCursorPosition(PXFile) + PXID3CommentSize - 2;
                const unsigned short trackIDSymbol = (((unsigned short)trackIDAdress[0]) << 8u) + trackIDAdress[1];
                const PXBool isVersion1x0 = trackIDSymbol == 0x0000;

                if(isVersion1x0)
                {
                    version = PXID3Versionv1x0;
                }
                else
                {
                    version = PXID3Versionv1x1;
                }

                //PXFile.CursorRewind(2u); // Go the steps back from the version, as 1.x does not have this field.

                break;
            }
        }

        // Cancel if version is invalid
        {
            const PXBool isValidVersion = version != PXID3VersionInvalid;

            if(!isValidVersion)
            {
                return PXActionRefusedInvalidVersion;
            }
        }
    }

    // Parse
    {
        const PXBool useOldHeader = PXID3Versionv1x0 == version || PXID3Versionv1x1 == version;

        if(useOldHeader)
        {
            const PXBool hasTrackID = PXID3Versionv1x1 == version;

            const PXFileDataElementType pxFileDataElementType[] =
            {
                {id3->Title, PXID3TitleSize},
                {id3->Artist, PXID3ArtistSize},
                {id3->Album, PXID3AlbumSize},
                {id3->Year, PXID3YearSize},
                {id3->Comment, PXID3CommentSize},
                {&id3->Genre, PXDataTypeInt08U}
            };

            PXFileWriteMultible(PXFile, pxFileDataElementType, sizeof(pxFileDataElementType));

            if(hasTrackID)
            {
                id3->TrackID = id3->Comment[PXID3CommentSize - 1];
                id3->Comment[PXID3CommentSize - 1] = 0;
            }  
        }
        else
        {
            unsigned int sizeOfDataSegment = 0;
            unsigned char a = 0;
            unsigned char b = 0;
            unsigned char c = 0;

            // Read flags
            {
                unsigned char flags = 0;

                PXFileReadI8U(PXFile, &flags);

                a = (flags & 0b10000000) >> 7;
                b = (flags & 0b01000000) >> 6;
                c = (flags & 0b00100000) >> 5;
            }

            // read Size
            {
                PXInt32UCluster sizeCluster;

                PXFileReadB(PXFile, sizeCluster.Data, 4u);

                // Size format: x000 x000 x000 x000 => 28 Bit int
                // The first bit of each byte not only unused but shall be merged!
                // Some banana thought this was a good idea, as to
                // provide a limitation of how big the value can be.

                const unsigned int as = (unsigned int)sizeCluster.A << (16 - 3);
                const unsigned int bs = (unsigned int)sizeCluster.B << (12 - 2);
                const unsigned int cs = (unsigned int)sizeCluster.C << (8 - 1);
                const unsigned int ds = (unsigned int)sizeCluster.D << 0;

                sizeOfDataSegment = as + bs + cs + ds;
            }

#if PXID3Debug
            printf("[PXID3][Version 2.x] Size:%i\n", sizeOfDataSegment);
#endif

            switch(version)
            {
                case PXID3Versionv2x0:
                    break;
                case PXID3Versionv2x2:
                    break;
                case PXID3Versionv2x3:
                case PXID3Versionv2x4:
                {
                    const PXSize posCurrent = PXFile->DataSize;
                    const PXSize posExpectedMax = PXFile->DataCursor + sizeOfDataSegment;

                   // PXFile->DataSize = (posCurrent < posExpectedMax) ? posCurrent : posExpectedMax;

                    while(PXFile->DataCursor < sizeOfDataSegment) // until the offset is reached
                    {
                        // Read 4 byte indexes
                        PXInt32UCluster indentifier;
                        PXInt32U frameSize = 0;
                        PXInt16U frameFlags = 0;

                        PXFileReadB(PXFile, indentifier.Data, 4u);
                        PXFileReadI32UE(PXFile, &frameSize, PXEndianBig);
                        PXFileReadI16UE(PXFile, &frameFlags, PXEndianBig);

                        const PXID3v2xFrameTag frameTag = PXID3v2xFrameTagFromID(indentifier.Value);
                        const PXBool unkownTag = frameTag == PXID3v2xFrameTagInvalid;

                        const PXSize expectedOffset = PXFile->DataCursor + frameSize;

                        if(unkownTag)
                        {
                            const PXBool emptyDataDertected = (frameSize == 0);

                            if(emptyDataDertected)
                            {
                                PXFileCursorToEnd(PXFile);
                                break; // Cancel while loop, as there is nothing else to parse.
                            }
                            else
                            {
                                PXFileCursorMoveTo(PXFile, expectedOffset);
                            }
                        }

#if PXID3Debug
                        printf
                        (
                            "[Chunk %c%c%c%c] %i Bytes\n",
                            indentifier.A,
                            indentifier.B,
                            indentifier.C,
                            indentifier.D,
                            frameSize
                        );
#endif

                        switch(frameTag)
                        {
                            case AudioEncryption:
                                break;
                            case AttachedPicture:
                                break;
                            case Comments:
                            {
                                unsigned char textEncoding = 0;
                                char language[3];

                                /*
                                PXFile.Read(textEncoding);
                                PXFile.Read(language, 3u);

                                PXSize insertOffset = 0;

                                while(PXFile.DataCursor < posAfterChunk) // There can be multible comments, but only one language.
                                {
                                    char stringEncoding[2];

                                    PXFile.Read(stringEncoding, 2u);

                                    PXFile.ReadUntil(Comment + insertOffset, frameSize - 3, L'\0');
                                }
                                */
                                break;
                            }
                            case CommercialFrame:
                                break;
                            case EncryptionMethodRegistration:
                                break;
                            case Equalization:
                                break;
                            case EventTimingCodes:
                                break;
                            case GeneralEncapsulatedObject:
                                break;
                            case GroupIdentificationRegistration:
                                break;
                            case InvolvedPeopleList:
                                break;
                            case LinkedInformation:
                                break;
                            case MusicCDIdentifier:
                                break;
                            case MPEGLocationLookupTable:
                                break;
                            case OwnershipFrame:
                                break;
                            case PrivateFrame:
                            {
                                break;
                            }
                            case PlayCounter:
                                break;
                            case Popularimeter:
                            {
                                const char* emailToUser = (char*)PXFileCursorPosition(PXFile);
                                const PXSize maximalToRead = PXFileRemainingSize(PXFile) - 6u;
                                const PXSize emailToUserSize = PXTextLengthA((char*)emailToUser, maximalToRead);
                                unsigned char rating = 0;
                                // counter?

                                // Copy

                                PXFileCursorAdvance(PXFile, emailToUserSize + 1);
                                PXFileReadI8U(PXFile, &rating);

                                break;
                            }
                            case PositionSynchronisationFrame:
                                break;
                            case RecommendedBufferSize:
                                break;
                            case RelativeVolumeAdjustment:
                                break;
                            case Reverb:
                                break;
                            case SynchronizedLyric:
                                break;
                            case SynchronizedTempocodes:
                                break;
                            case Album:
                            {
                               // PXFile.CursorAdvance(3u);
                                //PXFile.Read(Album, frameSize - 3);

                                break;
                            }
                            case BeatsPerMinute:
                            {
                                // Is text, convert to int
                                id3->BeatsPerMinute = 0;
                                break;
                            }
                            case Composer:
                            {
                               // PXFile.CursorAdvance(3u);
                               // PXFile.Read(Composer, frameSize - 3);
                                break;
                            }
                            case ContentType:
                            {
                                // GenreString -> enum
                                break;
                            }
                            case CopyrightMessage:
                                break;
                            case Date:
                                break;
                            case PlaylistDelay:
                                break;
                            case EncodedBy:
                                break;
                            case Lyricist:
                                break;
                            case FileType:
                                break;
                            case Time:
                                break;
                            case ContentGroupDescription:
                                break;
                            case Title:
                            {
                               // PXFile.CursorAdvance(3u);
                                //PXFile.Read(Title, frameSize - 3);
                                break;
                            }
                            case Subtitle:
                            {
                                break;
                            }
                            case InitialKey:
                                break;
                            case Language:
                                break;
                            case Length:
                                break;
                            case MediaType:
                                break;
                            case OriginalAlbum:
                                break;
                            case OriginalFilename:
                                break;
                            case OriginalLyricist:
                                break;
                            case OriginalArtist:
                                break;
                            case OriginalReleaseYear:
                                break;
                            case FileOwner:
                                break;
                            case LeadPerformer:
                            {
                                //PXFile.CursorAdvance(3u);
                               // PXFile.Read(Artist, frameSize - 3);
                                break;
                            }
                            case Band:
                            {

                                break;
                            }
                            case Conductor:
                            {
                                //PXFile.CursorAdvance(3u);
                                //PXFile.Read(Album, frameSize - 3);
                                break;
                            }
                            case InterpretedBy:
                                break;
                            case PartOfASet:
                                break;
                            case Publisher:
                                break;
                            case TrackNumber:
                                break;
                            case RecordingDates:
                                break;
                            case InternetRadioStationName:
                                break;
                            case InternetRadioStationOwner:
                                break;
                            case Size:
                                break;
                            case InternationalStandardRecordingCode:
                                break;
                            case SoftwareHardwaresettingsUsedForEncoding:
                                break;
                            case Year:
                                break;
                            case UserDefinedText:
                                break;
                            case UniqueFileIdentifier:
                                break;
                            case TermsOfUse:
                                break;
                            case UnsychronizedLyric:
                                break;
                            case CommercialInformation:
                                break;
                            case CopyrightLegalInformation:
                                break;
                            case OfficialAudioFileWebpage:
                                break;
                            case OfficialArtistPerformerWebpage:
                                break;
                            case OfficialAudioSourceWebpage:
                                break;
                            case InternetRadioStationHomepage:
                                break;
                            case Payment:
                                break;
                            case PublishersOfficialWebpage:
                                break;
                            case UserDefinedURLLinkFrame:
                                break;
                        }

                        const PXBool hasUnhandleChunkDetected = PXFile->DataCursor < expectedOffset;

                        if(hasUnhandleChunkDetected)
                        {
                            // TODO: Handle error

                            //printf("Unhandle chunk detected!\n");

                            PXFileCursorMoveTo(PXFile, expectedOffset);
                        }
                    }

                    break;
                }
            }
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXID3SaveToFile(const PXID3* const id3, PXFile* const pxFile)
{
    //-----------------------------------------------------
    // ID3 - Header write 
    //-----------------------------------------------------
    {
        PXFileWriteB(pxFile, PXID3HeaderSignature, sizeof(PXID3HeaderSignature));
    }

    return PXActionRefusedNotImplemented;
}