#include "PXID3.h"

#include <Media/PXText.h>
#include <OS/File/PXFile.h>

const char PXID3HeaderSignature[3] = { 'I','D','3' };

#if PXID3Debug
#include <cstdio>
#endif

PXID3v2xFrameTag PXAPI PXID3v2xFrameTagFromID(const PXInt32U id3v2xFrameTagID)
{
    switch(id3v2xFrameTagID)
    {
        case PXInt32Make('A', 'E', 'N', 'C'): return PXID3v2xFrameTagAudioEncryption;
        case PXInt32Make('A', 'P', 'I', 'C'): return PXID3v2xFrameTagAttachedPicture;
        case PXInt32Make('C', 'O', 'M', 'M'): return PXID3v2xFrameTagComments;
        case PXInt32Make('C', 'O', 'M', 'R'): return PXID3v2xFrameTagCommercialFrame;
        case PXInt32Make('E', 'N', 'C', 'R'): return PXID3v2xFrameTagEncryptionMethodRegistration;
        case PXInt32Make('E', 'Q', 'U', 'A'): return PXID3v2xFrameTagEqualization;
        case PXInt32Make('E', 'T', 'C', 'O'): return PXID3v2xFrameTagEventTimingCodes;
        case PXInt32Make('G', 'E', 'O', 'B'): return PXID3v2xFrameTagGeneralEncapsulatedObject;
        case PXInt32Make('G', 'R', 'I', 'D'): return PXID3v2xFrameTagGroupIdentificationRegistration;
        case PXInt32Make('I', 'P', 'L', 'S'): return PXID3v2xFrameTagInvolvedPeopleList;
        case PXInt32Make('L', 'I', 'N', 'K'): return PXID3v2xFrameTagLinkedInformation;
        case PXInt32Make('M', 'C', 'D', 'I'): return PXID3v2xFrameTagMusicCDIdentifier;
        case PXInt32Make('M', 'L', 'L', 'T'): return PXID3v2xFrameTagMPEGLocationLookupTable;
        case PXInt32Make('O', 'W', 'N', 'E'): return PXID3v2xFrameTagOwnershipFrame;
        case PXInt32Make('P', 'R', 'I', 'V'): return PXID3v2xFrameTagPrivateFrame;
        case PXInt32Make('P', 'C', 'N', 'T'): return PXID3v2xFrameTagPlayCounter;
        case PXInt32Make('P', 'O', 'P', 'M'): return PXID3v2xFrameTagPopularimeter;
        case PXInt32Make('P', 'O', 'S', 'S'): return PXID3v2xFrameTagPositionSynchronisationFrame;
        case PXInt32Make('R', 'B', 'U', 'F'): return PXID3v2xFrameTagRecommendedBufferSize;
        case PXInt32Make('R', 'V', 'A', 'D'): return PXID3v2xFrameTagRelativeVolumeAdjustment;
        case PXInt32Make('R', 'V', 'R', 'B'): return PXID3v2xFrameTagReverb;
        case PXInt32Make('S', 'Y', 'L', 'T'): return PXID3v2xFrameTagSynchronizedLyric;
        case PXInt32Make('S', 'Y', 'T', 'C'): return PXID3v2xFrameTagSynchronizedTempocodes;
        case PXInt32Make('T', 'A', 'L', 'B'): return PXID3v2xFrameTagAlbum;
        case PXInt32Make('T', 'B', 'P', 'M'): return PXID3v2xFrameTagBeatsPerMinute;
        case PXInt32Make('T', 'C', 'O', 'M'): return PXID3v2xFrameTagComposer;
        case PXInt32Make('T', 'C', 'O', 'N'): return PXID3v2xFrameTagContentType;
        case PXInt32Make('T', 'C', 'O', 'P'): return PXID3v2xFrameTagCopyrightMessage;
        case PXInt32Make('T', 'D', 'A', 'T'): return PXID3v2xFrameTagDate;
        case PXInt32Make('T', 'D', 'L', 'Y'): return PXID3v2xFrameTagPlaylistDelay;
        //case PXInt32Make('T', 'D', 'T', 'G'): return TaggingTime;
        case PXInt32Make('T', 'E', 'N', 'C'): return PXID3v2xFrameTagEncodedBy;
        case PXInt32Make('T', 'E', 'X', 'T'): return PXID3v2xFrameTagLyricist;
        case PXInt32Make('T', 'F', 'L', 'T'): return PXID3v2xFrameTagFileType;
        case PXInt32Make('T', 'I', 'M', 'E'): return PXID3v2xFrameTagTime;
        case PXInt32Make('T', 'I', 'T', '1'): return PXID3v2xFrameTagContentGroupDescription;
        case PXInt32Make('T', 'I', 'T', '2'): return PXID3v2xFrameTagTitle;
        case PXInt32Make('T', 'I', 'T', '3'): return PXID3v2xFrameTagSubtitle;
        case PXInt32Make('T', 'K', 'E', 'Y'): return PXID3v2xFrameTagInitialKey;
        case PXInt32Make('T', 'L', 'A', 'N'): return PXID3v2xFrameTagLanguage;
        case PXInt32Make('T', 'L', 'E', 'N'): return PXID3v2xFrameTagLength;
        case PXInt32Make('T', 'M', 'E', 'D'): return PXID3v2xFrameTagMediaType;
        case PXInt32Make('T', 'O', 'A', 'L'): return PXID3v2xFrameTagOriginalAlbum;
        case PXInt32Make('T', 'O', 'F', 'N'): return PXID3v2xFrameTagOriginalFilename;
        case PXInt32Make('T', 'O', 'L', 'Y'): return PXID3v2xFrameTagOriginalLyricist;
        case PXInt32Make('T', 'O', 'P', 'E'): return PXID3v2xFrameTagOriginalArtist;
        case PXInt32Make('T', 'O', 'R', 'Y'): return PXID3v2xFrameTagOriginalReleaseYear;
        case PXInt32Make('T', 'O', 'W', 'N'): return PXID3v2xFrameTagFileOwner;
        case PXInt32Make('T', 'P', 'E', '1'): return PXID3v2xFrameTagLeadPerformer;
        case PXInt32Make('T', 'P', 'E', '2'): return PXID3v2xFrameTagBand;
        case PXInt32Make('T', 'P', 'E', '3'): return PXID3v2xFrameTagConductor;
        case PXInt32Make('T', 'P', 'E', '4'): return PXID3v2xFrameTagInterpretedBy;
        case PXInt32Make('T', 'P', 'O', 'S'): return PXID3v2xFrameTagPartOfASet;
        case PXInt32Make('T', 'P', 'U', 'B'): return PXID3v2xFrameTagPublisher;
        case PXInt32Make('T', 'R', 'C', 'K'): return PXID3v2xFrameTagTrackNumber;
        case PXInt32Make('T', 'R', 'D', 'A'): return PXID3v2xFrameTagRecordingDates;
        case PXInt32Make('T', 'R', 'S', 'N'): return PXID3v2xFrameTagInternetRadioStationName;
        case PXInt32Make('T', 'R', 'S', 'O'): return PXID3v2xFrameTagInternetRadioStationOwner;
        case PXInt32Make('T', 'S', 'I', 'Z'): return PXID3v2xFrameTagSize;
        case PXInt32Make('T', 'S', 'R', 'C'): return PXID3v2xFrameTagInternationalStandardRecordingCode;
        case PXInt32Make('T', 'S', 'S', 'E'): return PXID3v2xFrameTagSoftwareHardwaresettingsUsedForEncoding;
        case PXInt32Make('T', 'Y', 'E', 'R'): return PXID3v2xFrameTagYear;
        case PXInt32Make('T', 'X', 'X', 'X'): return PXID3v2xFrameTagUserDefinedText;
        case PXInt32Make('U', 'F', 'I', 'D'): return PXID3v2xFrameTagUniqueFileIdentifier;
        case PXInt32Make('U', 'S', 'E', 'R'): return PXID3v2xFrameTagTermsOfUse;
        case PXInt32Make('U', 'S', 'L', 'T'): return PXID3v2xFrameTagUnsychronizedLyric;
        case PXInt32Make('W', 'C', 'O', 'M'): return PXID3v2xFrameTagCommercialInformation;
        case PXInt32Make('W', 'C', 'O', 'P'): return PXID3v2xFrameTagCopyrightLegalInformation;
        case PXInt32Make('W', 'O', 'A', 'F'): return PXID3v2xFrameTagOfficialAudioFileWebpage;
        case PXInt32Make('W', 'O', 'A', 'R'): return PXID3v2xFrameTagOfficialArtistPerformerWebpage;
        case PXInt32Make('W', 'O', 'A', 'S'): return PXID3v2xFrameTagOfficialAudioSourceWebpage;
        case PXInt32Make('W', 'O', 'R', 'S'): return PXID3v2xFrameTagInternetRadioStationHomepage;
        case PXInt32Make('W', 'P', 'A', 'Y'): return PXID3v2xFrameTagPayment;
        case PXInt32Make('W', 'P', 'U', 'B'): return PXID3v2xFrameTagPublishersOfficialWebpage;
        case PXInt32Make('W', 'X', 'X', 'X'): return PXID3v2xFrameTagUserDefinedURLLinkFrame;

        default:
            return PXID3v2xFrameTagInvalid;
    }
}

PXActionResult PXAPI PXID3LoadFromFile(PXID3* const id3, PXFile* const PXFile)
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
                return PXActionRefusedVersionInvalid;
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
                            case PXID3v2xFrameTagAudioEncryption:
                                break;
                            case PXID3v2xFrameTagAttachedPicture:
                                break;
                            case PXID3v2xFrameTagComments:
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
                            case PXID3v2xFrameTagCommercialFrame:
                                break;
                            case PXID3v2xFrameTagEncryptionMethodRegistration:
                                break;
                            case PXID3v2xFrameTagEqualization:
                                break;
                            case PXID3v2xFrameTagEventTimingCodes:
                                break;
                            case PXID3v2xFrameTagGeneralEncapsulatedObject:
                                break;
                            case PXID3v2xFrameTagGroupIdentificationRegistration:
                                break;
                            case PXID3v2xFrameTagInvolvedPeopleList:
                                break;
                            case PXID3v2xFrameTagLinkedInformation:
                                break;
                            case PXID3v2xFrameTagMusicCDIdentifier:
                                break;
                            case PXID3v2xFrameTagMPEGLocationLookupTable:
                                break;
                            case PXID3v2xFrameTagOwnershipFrame:
                                break;
                            case PXID3v2xFrameTagPrivateFrame:
                            {
                                break;
                            }
                            case PXID3v2xFrameTagPlayCounter:
                                break;
                            case PXID3v2xFrameTagPopularimeter:
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
                            case PXID3v2xFrameTagPositionSynchronisationFrame:
                                break;
                            case PXID3v2xFrameTagRecommendedBufferSize:
                                break;
                            case PXID3v2xFrameTagRelativeVolumeAdjustment:
                                break;
                            case PXID3v2xFrameTagReverb:
                                break;
                            case PXID3v2xFrameTagSynchronizedLyric:
                                break;
                            case PXID3v2xFrameTagSynchronizedTempocodes:
                                break;
                            case PXID3v2xFrameTagAlbum:
                            {
                               // PXFile.CursorAdvance(3u);
                                //PXFile.Read(Album, frameSize - 3);

                                break;
                            }
                            case PXID3v2xFrameTagBeatsPerMinute:
                            {
                                // Is text, convert to int
                                id3->BeatsPerMinute = 0;
                                break;
                            }
                            case PXID3v2xFrameTagComposer:
                            {
                               // PXFile.CursorAdvance(3u);
                               // PXFile.Read(Composer, frameSize - 3);
                                break;
                            }
                            case PXID3v2xFrameTagContentType:
                            {
                                // GenreString -> enum
                                break;
                            }
                            case PXID3v2xFrameTagCopyrightMessage:
                                break;
                            case PXID3v2xFrameTagDate:
                                break;
                            case PXID3v2xFrameTagPlaylistDelay:
                                break;
                            case PXID3v2xFrameTagEncodedBy:
                                break;
                            case PXID3v2xFrameTagLyricist:
                                break;
                            case PXID3v2xFrameTagFileType:
                                break;
                            case PXID3v2xFrameTagTime:
                                break;
                            case PXID3v2xFrameTagContentGroupDescription:
                                break;
                            case PXID3v2xFrameTagTitle:
                            {
                               // PXFile.CursorAdvance(3u);
                                //PXFile.Read(Title, frameSize - 3);
                                break;
                            }
                            case PXID3v2xFrameTagSubtitle:
                            {
                                break;
                            }
                            case PXID3v2xFrameTagInitialKey:
                                break;
                            case PXID3v2xFrameTagLanguage:
                                break;
                            case PXID3v2xFrameTagLength:
                                break;
                            case PXID3v2xFrameTagMediaType:
                                break;
                            case PXID3v2xFrameTagOriginalAlbum:
                                break;
                            case PXID3v2xFrameTagOriginalFilename:
                                break;
                            case PXID3v2xFrameTagOriginalLyricist:
                                break;
                            case PXID3v2xFrameTagOriginalArtist:
                                break;
                            case PXID3v2xFrameTagOriginalReleaseYear:
                                break;
                            case PXID3v2xFrameTagFileOwner:
                                break;
                            case PXID3v2xFrameTagLeadPerformer:
                            {
                                //PXFile.CursorAdvance(3u);
                               // PXFile.Read(Artist, frameSize - 3);
                                break;
                            }
                            case PXID3v2xFrameTagBand:
                            {

                                break;
                            }
                            case PXID3v2xFrameTagConductor:
                            {
                                //PXFile.CursorAdvance(3u);
                                //PXFile.Read(Album, frameSize - 3);
                                break;
                            }
                            case PXID3v2xFrameTagInterpretedBy:
                                break;
                            case PXID3v2xFrameTagPartOfASet:
                                break;
                            case PXID3v2xFrameTagPublisher:
                                break;
                            case PXID3v2xFrameTagTrackNumber:
                                break;
                            case PXID3v2xFrameTagRecordingDates:
                                break;
                            case PXID3v2xFrameTagInternetRadioStationName:
                                break;
                            case PXID3v2xFrameTagInternetRadioStationOwner:
                                break;
                            case PXID3v2xFrameTagSize:
                                break;
                            case PXID3v2xFrameTagInternationalStandardRecordingCode:
                                break;
                            case PXID3v2xFrameTagSoftwareHardwaresettingsUsedForEncoding:
                                break;
                            case PXID3v2xFrameTagYear:
                                break;
                            case PXID3v2xFrameTagUserDefinedText:
                                break;
                            case PXID3v2xFrameTagUniqueFileIdentifier:
                                break;
                            case PXID3v2xFrameTagTermsOfUse:
                                break;
                            case PXID3v2xFrameTagUnsychronizedLyric:
                                break;
                            case PXID3v2xFrameTagCommercialInformation:
                                break;
                            case PXID3v2xFrameTagCopyrightLegalInformation:
                                break;
                            case PXID3v2xFrameTagOfficialAudioFileWebpage:
                                break;
                            case PXID3v2xFrameTagOfficialArtistPerformerWebpage:
                                break;
                            case PXID3v2xFrameTagOfficialAudioSourceWebpage:
                                break;
                            case PXID3v2xFrameTagInternetRadioStationHomepage:
                                break;
                            case PXID3v2xFrameTagPayment:
                                break;
                            case PXID3v2xFrameTagPublishersOfficialWebpage:
                                break;
                            case PXID3v2xFrameTagUserDefinedURLLinkFrame:
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

PXActionResult PXAPI PXID3SaveToFile(const PXID3* const id3, PXFile* const pxFile)
{
    //-----------------------------------------------------
    // ID3 - Header write 
    //-----------------------------------------------------
    {
        PXFileWriteB(pxFile, PXID3HeaderSignature, sizeof(PXID3HeaderSignature));
    }

    return PXActionRefusedNotImplemented;
}