#include "TTF.h"

#include <File/File.h>
#include <Memory/PXMemory.h>
#include <File/DataStream.h>
#include <Container/ClusterValue.h>

EncodingID ConvertToEncodingID(const PlatformID platformID, unsigned char encodingID)
{
	switch(platformID)
	{
		case PlatformUnicode:
		{
			switch(encodingID)
			{
				case 0: return EncodingUnicode10;
				case 1:	return EncodingUnicode11;
				case 2:	return EncodingISOIEC10646;
				case 3:	return EncodingUnicode20BMPOnly;
				case 4: return EncodingUnicode20FullRepertoire;
				case 5: return EncodingUnicodeVariation;
				case 6: return EncodingUnicodeFullRepertoire;
			}

			break;
		}
		case PlatformMacintosh:
		{
			// ???
			break;
		}
		case PlatformISO:
		{
			switch(encodingID)
			{
				case 0:	return EncodingSevenBitASCII;
				case 1:	return EncodingISO10646;
				case 2:	return EncodingISO8858;
			}

			break;
		}
		case PlatformWindows:
		{
			switch(encodingID)
			{
				case 0: return EncodingSymbol;
				case 1: return EncodingUnicodeBMP;
				case 2: return EncodingShiftJIS;
				case 3: return EncodingPRC;
				case 4: return EncodingBig5;
				case 5: return EncodingWansung;
				case 6: return EncodingJohab;

				case 7: // fall through
				case 8: // fall through
				case 9: // fall through
					return EncodingReserved;

				case 10: return EncodingUnicodeFullRepertoire;
			}

			break;
		}
		case PlatformCustom:
			return EncodingOTFWindowsNTCompatibilityMapping;

		case PlatformInvalid:
			return EncodingInvalid;
	}

	return EncodingInvalid;
}

TTFVersionType ConvertFromVersionType(unsigned short major, unsigned short minor)
{
	switch(minor)
	{
		case 0:
		{
			switch(major)
			{
				case 1: return TTFVersion1Dot0;
				case 2: return TTFVersion2Dot0;
				case 3: return TTFVersion3Dot0;
			}

			break;
		}
		case 5:
		{
			if(major == 2)
			{
				return TTFVersion2Dot5;
			}

			break;
		}
	}

	return TTFVersionInvalid;
}

void ConvertToVersionType(unsigned short* major, unsigned short* minor, const TTFVersionType versionType)
{
}

TTFTableEntryType ConvertTableEntryType(const unsigned int tableEntryType)
{
	switch(tableEntryType)
	{
		case MakeInt('a', 'c', 'n', 't'): return TTFTableEntryAccentAttachment;
		case MakeInt('a', 'n', 'k', 'r'): return TTFTableEntryAnchorPoint;
		case MakeInt('a', 'v', 'a', 'r'): return TTFTableEntryAxisVariation;
		case MakeInt('b', 'd', 'a', 't'): return TTFTableEntryBitmapData;
		case MakeInt('b', 'h', 'e', 'd'): return TTFTableEntryBitmapFontHeader;
		case MakeInt('b', 'l', 'o', 'c'): return TTFTableEntryBitmapLocation;
		case MakeInt('b', 's', 'l', 'n'): return TTFTableEntryBaseline;
		case MakeInt('c', 'm', 'a', 'p'): return TTFTableEntryCharacterCodeMapping;
		case MakeInt('c', 'v', 'a', 'r'): return TTFTableEntryCVTVariation;
		case MakeInt('c', 'v', 't', ' '): return TTFTableEntryControlValue;
		case MakeInt('D', 'S', 'I', 'G'): return TTFTableEntryDigitalSignature;
		case MakeInt('E', 'B', 'S', 'C'): return TTFTableEntryEmbeddedBitmapScalingControl;
		case MakeInt('f', 'd', 's', 'c'): return TTFTableEntryFontDescriptor;
		case MakeInt('f', 'e', 'a', 't'): return TTFTableEntryLayoutFeature;
		case MakeInt('f', 'm', 't', 'x'): return TTFTableEntryFontMetrics;
		case MakeInt('f', 'o', 'n', 'd'): return TTFTableEntryFontFamilyCompatibility;
		case MakeInt('f', 'p', 'g', 'm'): return TTFTableEntryFontProgram;
		case MakeInt('f', 'v', 'a', 'r'): return TTFTableEntryFontVariation;
		case MakeInt('g', 'a', 's', 'p'): return TTFTableEntryGridFittingAndScanConversionProcedure;
		case MakeInt('g', 'l', 'y', 'f'): return TTFTableEntryGlyphOutline;
		case MakeInt('g', 'v', 'a', 'r'): return TTFTableEntryGlyphVariation;
		case MakeInt('h', 'd', 'm', 'x'): return TTFTableEntryHorizontalDeviceMetrics;
		case MakeInt('h', 'e', 'a', 'd'): return TTFTableEntryFontHeader;
		case MakeInt('h', 'h', 'e', 'a'): return TTFTableEntryHorizontalHeader;
		case MakeInt('h', 'm', 't', 'x'): return TTFTableEntryHorizontalMetrics;
		case MakeInt('j', 'u', 's', 't'): return TTFTableEntryJustification;
		case MakeInt('k', 'e', 'r', 'n'): return TTFTableEntryKerning;
		case MakeInt('k', 'e', 'r', 'x'): return TTFTableEntryExtendedKerning;
		case MakeInt('l', 'c', 'a', 'r'): return TTFTableEntryLigatureCaret;
		case MakeInt('l', 'o', 'c', 'a'): return TTFTableEntryGlyphLocation;
		case MakeInt('l', 't', 'a', 'g'): return TTFTableEntryLanguageTag;
		case MakeInt('L', 'T', 'S', 'H'): return TTFTableEntryLinearThreshold;
		case MakeInt('m', 'a', 'x', 'p'): return TTFTableEntryMaximumProfile;
		case MakeInt('m', 'e', 't', 'a'): return TTFTableEntryMetadata;
		case MakeInt('m', 'o', 'r', 't'): return TTFTableEntryMetamorphosisTabledeprecated;
		case MakeInt('m', 'o', 'r', 'x'): return TTFTableEntryExtendedMetamorphosis;
		case MakeInt('n', 'a', 'm', 'e'): return TTFTableEntryName;
		case MakeInt('o', 'p', 'b', 'd'): return TTFTableEntryOpticalBounds;
		case MakeInt('O', 'S', '/', '2'): return TTFTableEntryCompatibility;
		case MakeInt('p', 'o', 's', 't'): return TTFTableEntryGlyphNameAndPostScriptCompatibility;
		case MakeInt('p', 'r', 'e', 'p'): return TTFTableEntryControlValueProgram;
		case MakeInt('p', 'r', 'o', 'p'): return TTFTableEntryProperties;
		case MakeInt('s', 'b', 'i', 'x'): return TTFTableEntryExtendedBitmaps;
		case MakeInt('t', 'r', 'a', 'k'): return TTFTableEntryTracking;
		case MakeInt('v', 'h', 'e', 'a'): return TTFTableEntryVerticalHeader;
		case MakeInt('v', 'm', 't', 'x'): return TTFTableEntryVerticalMetrics;
		case MakeInt('V', 'D', 'M', 'X'): return TTFTableEntryVerticalDeviceMetrics;
		case MakeInt('x', 'r', 'e', 'f'): return TTFTableEntryCrossReference;
		case MakeInt('Z', 'a', 'p', 'f'): return TTFTableEntryGlyphReference;
		default: return TTFTableEntryUnkown;
	}
}

void TTFConstruct(TTF* const ttf)
{
	MemorySet(ttf, sizeof(TTF), 0);
}

void TTFDestruct(TTF* const ttf)
{
	// TODO
}

ActionResult TTFParse(TTF* ttf, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;
	TTFOffsetTable offsetTable;

	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);
	

	DataStreamRead(&dataStream, &offsetTable.Version.Major, EndianBig);
	DataStreamRead(&dataStream, &offsetTable.Version.Minor, EndianBig);
	DataStreamRead(&dataStream, &offsetTable.NumberOfTables, EndianBig);
	DataStreamRead(&dataStream, &offsetTable.SearchRange, EndianBig);
	DataStreamRead(&dataStream, &offsetTable.EntrySelctor, EndianBig);
	DataStreamRead(&dataStream, &offsetTable.RangeShift, EndianBig);

	for(size_t i = 0; i < offsetTable.NumberOfTables; i++)
	{
		TTFTableEntry tableEntry;

		DataStreamRead(&dataStream, tableEntry.TypeRaw, 4u);
		DataStreamRead(&dataStream, tableEntry.CheckSum, EndianBig);
		DataStreamRead(&dataStream, tableEntry.Offset, EndianBig);
		DataStreamRead(&dataStream, tableEntry.Length, EndianBig);

		const unsigned int typeID = MakeInt(tableEntry.TypeRaw[0], tableEntry.TypeRaw[1], tableEntry.TypeRaw[2], tableEntry.TypeRaw[3]);

		tableEntry.Type = ConvertTableEntryType(typeID);

		printf
		(
			"[TTF][i] Chunk:[%c%c%c%c], Known:%c, Offset:%6i, Length:%6i\n",
			tableEntry.TypeRaw[0],
			tableEntry.TypeRaw[1],
			tableEntry.TypeRaw[2],
			tableEntry.TypeRaw[3],
			tableEntry.Type == TTFTableEntryUnkown ? '-' : 'x',
			tableEntry.Offset,
			tableEntry.Length
		);

		switch(tableEntry.Type)
		{
			//---<Essential>---------------------------------------------------		
			case TTFTableEntryFontHeader:
			{
				DataStreamReadSU(&dataStream, &ttf->Header.Version.Major, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Header.Version.Minor, EndianLittle);
				DataStreamReadSU(&dataStream, &ttf->Header.Revision.Major, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Header.Revision.Minor, EndianLittle);
				DataStreamReadIU(&dataStream, &ttf->Header.CheckSumAdjustment, EndianBig);
				DataStreamReadIU(&dataStream, &ttf->Header.MagicNumber, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Header.Flags, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Header.UnitsPerEM, EndianBig);
				DataStreamReadLLU(&dataStream, &ttf->Header.Created, EndianBig);
				DataStreamReadLLU(&dataStream, &ttf->Header.Modified, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.Minimum[0], EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.Minimum[1], EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.Maximum[0], EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.Maximum[1], EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Header.MacStyle, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Header.LowestRecPpem, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.FontDirectionHint, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.IndexToLocFormat, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Header.GlyphDataFormat, EndianBig);
				break;
			}
			case TTFTableEntryHorizontalHeader:
			{
				DataStreamReadSU(&dataStream, &ttf->HorizontalHeader.Version.Major, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->HorizontalHeader.Version.Minor, EndianLittle);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.Ascender, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.Descender, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.LineGap, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->HorizontalHeader.AdvanceWidthMaximum, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.MinimumLeftSideBearing, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.MinimumRightSideBearing, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.MaximalExtendX, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.CaretSlopeRun, EndianBig);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.CaretSlopeRise, EndianBig);
				DataStreamReadP(&dataStream, &ttf->HorizontalHeader.Reserved, 10u);
				DataStreamReadS(&dataStream, &ttf->HorizontalHeader.MetricDataFormat, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->HorizontalHeader.NumberOfHorizontalMetrics, EndianBig);
				break;
			}
			case TTFTableEntryMaximumProfile:
			{
				DataStreamReadSU(&dataStream, &ttf->MaximumProfile.Version.Major, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->MaximumProfile.Version.Minor, EndianLittle);

				const unsigned char trustedTypeFonts = ttf->MaximumProfile.Version.Major == 1 && ttf->MaximumProfile.Version.Minor == 0;
				const unsigned char openTypeFonts = ttf->MaximumProfile.Version.Major == 0 && ttf->MaximumProfile.Version.Minor == 5;
				const unsigned char validVersion = trustedTypeFonts || openTypeFonts;

				DataStreamReadSU(&dataStream, &ttf->MaximumProfile.NumberOfGlyphs, EndianBig);

				assert(validVersion);

				if(trustedTypeFonts && !openTypeFonts)
				{
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.PointsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.ContoursMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.ComponentPointsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.ComponentContoursMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.ZonesMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.TwilightPointsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.StorageMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.FunctionDefsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.InstructionDefsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.StackElementsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.SizeOfInstructionsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.ComponentElementsMaximal, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->MaximumProfile.ComponentDepthMaximal, EndianBig);
				}

				break;
			}
			case TTFTableEntryCompatibility:
			{
				DataStreamReadSU(&dataStream, &ttf->Compatibility.Version, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.xAvgCharWidth, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.usWeightClass, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.usWidthClass, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.fsType, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptXSize, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptYSize, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptXOffset, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptYOffset, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptXSize, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptYSize, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptXOffset, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptYOffset, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.yStrikeoutPosition, EndianBig);
				DataStreamReadS(&dataStream, &ttf->Compatibility.sFamilyClass, EndianBig);

				// Parse PANROSE
				{
					TTFPanose* panrose = &ttf->Compatibility.Panose;
					unsigned char bFamilyType;
					unsigned char bSerifStyle;
					unsigned char bWeight;
					unsigned char bProportion;
					unsigned char bContrast;
					unsigned char bStrokeVariation;
					unsigned char bArmStyle;
					unsigned char bLetterform;
					unsigned char bMidline;
					unsigned char bXHeight;

					DataStreamReadCU(&dataStream, &bFamilyType);
					DataStreamReadCU(&dataStream, &bSerifStyle);
					DataStreamReadCU(&dataStream, &bWeight);
					DataStreamReadCU(&dataStream, &bProportion);
					DataStreamReadCU(&dataStream, &bContrast);
					DataStreamReadCU(&dataStream, &bStrokeVariation);
					DataStreamReadCU(&dataStream, &bArmStyle);
					DataStreamReadCU(&dataStream, &bLetterform);
					DataStreamReadCU(&dataStream, &bMidline);
					DataStreamReadCU(&dataStream, &bXHeight);

					//panrose.FamilyType = ConvertTTFFamilyType(bFamilyType);
					//panrose.SerifStyle = ConvertTTFSerifStyle(bSerifStyle);
					//panrose.Weight = ConvertTTFWeight(bWeight);
					//panrose.Proportion = ConvertTTFProportion(bProportion);
					//panrose.Contrast = ConvertTTFContrast(bContrast);
					//panrose.StrokeVariation = ConvertTTFStrokeVariatoon(bStrokeVariation);
					//panrose.ArmStyle = ConvertTTFArmStyle(bArmStyle);
					//panrose.Letterform = ConvertTTFLetterform(bLetterform);
					//panrose.Midline = ConvertTTFMidline(bMidline);
					//panrose.HeightX = ConvertTTFHeightX(bXHeight);
				}

				DataStreamReadSU(&dataStream, &ttf->Compatibility.ulUnicodeRange[0], EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.ulUnicodeRange[1], EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.ulUnicodeRange[2], EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.ulUnicodeRange[3], EndianBig);

				DataStreamReadP(&dataStream, &ttf->Compatibility.achVendID, 4u);

				DataStreamReadSU(&dataStream, &ttf->Compatibility.fsSelection, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.fsFirstCharIndex, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Compatibility.fsLastCharIndex, EndianBig);

				if(ttf->Compatibility.Version > 0)
				{
					DataStreamReadSU(&dataStream, &ttf->Compatibility.sTypoAscender, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.sTypoDescender, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.sTypoLineGap, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usWinAscent, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usWinDescent, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.ulCodePageRange1, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.ulCodePageRange2, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.sxHeight, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.sCapHeight, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usDefaultChar, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usBreakChar, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usMaxContext, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usLowerPointSize, EndianBig);
					DataStreamReadSU(&dataStream, &ttf->Compatibility.usUpperPointSize, EndianBig);
				}

				break;
			}

			case TTFTableEntryControlValue: // cvt
			{
				// FWORD[ n ] 	List of n values referenceable by instructions. n is the number of FWORD items that fit in the size of the table.
				break;
			}
			case TTFTableEntryGlyphNameAndPostScriptCompatibility: // post
			{
				DataStreamReadSU(&dataStream, &ttf->PostScript.Version.Major, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->PostScript.Version.Minor, EndianLittle);

				//PostScript.Version.Check();

				/*

				switch(PostScript.Version.Type)
				{
					case TTFVersionInvalid:
						break;

					case TTFVersion1Dot0:
						break;

					case TTFVersion2Dot0:
					{
						DataStreamReadSU(&DataStream, &ttf->PostScript.NumberOfGlyphs, EndianBig);
						break;
					}
					case TTFVersion2Dot5:
					{
						DataStreamReadSU(&DataStream, &ttf->PostScript.NumberOfGlyphs, EndianBig);
						break;
					}
					case TTFVersion3Dot0:
						break;

					default:
						printf("Illegal Version");
						break;
				}*/

				break;
			}
			case TTFTableEntryKerning: // kern
			{
				DataStreamReadSU(&dataStream, &ttf->Kerning.Version, EndianBig);
				DataStreamReadSU(&dataStream, &ttf->Kerning.NumberOfSubtables, EndianBig);

				/*

				Kerning.KerningSubtableList = new TTFKerningSubtable[Kerning.NumberOfSubtables];

				for(size_t i = 0; i < Kerning.NumberOfSubtables; i++)
				{
					TTFKerningSubtable& kerningTable = Kerning.KerningSubtableList[i];

					unsigned short coverage = 0;

					DataStreamReadSU(&DataStream, &ttf->kerningTable.Version, EndianBig);
					DataStreamReadSU(&DataStream, &ttf->kerningTable.Length, EndianBig);
					DataStreamReadSU(&DataStream, &ttf->coverage, EndianBig);

					kerningTable.ParseCoverageValue(coverage);

					switch(kerningTable.Version)
					{
						case 0:
						{
							TTFSubtableFormat0& subtableFormat = kerningTable.SubtableFormat0;

							DataStreamReadSU(&DataStream, &ttf->subtableFormat.NumberOfPairs, EndianBig);
							DataStreamReadSU(&DataStream, &ttf->subtableFormat.SearchRange, EndianBig);
							DataStreamReadSU(&DataStream, &ttf->subtableFormat.EntrySelector, EndianBig);
							DataStreamReadSU(&DataStream, &ttf->subtableFormat.RangeShift, EndianBig);

							subtableFormat.KerningPairList = new TTFKerningPair[subtableFormat.NumberOfPairs];

							for(size_t i = 0; i < subtableFormat.NumberOfPairs; i++)
							{
								TTFKerningPair& kerningPair = subtableFormat.KerningPairList[i];

								DataStreamReadSU(&DataStream, &ttf->kerningPair.Left, EndianBig);
								DataStreamReadSU(&DataStream, &ttf->kerningPair.Right, EndianBig);
								DataStreamReadSU(&DataStream, &ttf->kerningPair.Value, EndianBig);
							}

							break;
						}
						case 2:
						{
							TTFSubtableFormat2& subtableFormat = kerningTable.SubtableFormat2;

							DataStreamReadSU(&DataStream, &ttf->subtableFormat.RowWidth, EndianBig);
							DataStreamReadSU(&DataStream, &ttf->subtableFormat.LeftClassOffset, EndianBig);
							DataStreamReadSU(&DataStream, &ttf->subtableFormat.RightClassOffset, EndianBig);
							DataStreamReadSU(&DataStream, &ttf->subtableFormat.KerningArrayOffset, EndianBig);

							break;
						}
					}
				}*/

				break;
			}
			case TTFTableEntryControlValueProgram: // prep
			{
				// The name 'prep' is anachronistic (the table used to be known as the Pre Program table.)
				// unsigned char[N]
				break;
			}
			case TTFTableEntryGlyphOutline: // glyf
			{
				break;
			}
			case TTFTableEntryFontProgram: // fpgm
			{
				break;
			}
			//-----------------------------------------------------------------

			// Windows 
			case TTFTableEntryCharacterCodeMapping:
			{
				/*
				DataStreamReadSU(&DataStream, &ttf->CharacterMapping.Version, EndianBig);
				DataStreamReadSU(&DataStream, &ttf->CharacterMapping.NumberOfTables, EndianBig);

				CharacterMapping.EncodingRecordList = new EncodingRecord[CharacterMapping.NumberOfTables];

				for(size_t i = 0; i < CharacterMapping.NumberOfTables; i++)
				{
					EncodingRecord& encodingRecord = CharacterMapping.EncodingRecordList[i];

					unsigned short platformID;
					unsigned short encodingID;

					DataStreamReadSU(&DataStream, &ttf->platformID, EndianBig);
					DataStreamReadSU(&DataStream, &ttf->encodingID, EndianBig);
					DataStreamReadSU(&DataStream, &ttf->encodingRecord.SubtableOffset, EndianBig);

					encodingRecord.Platform = ConvertPlatformID(platformID);
					encodingRecord.Encoding = ConvertEncodingID(encodingRecord.Platform, encodingID);
				}
				*/
				break;
			}
			case TTFTableEntryLinearThreshold:
			{
				/*
				DataStreamReadSU(&DataStream, &ttf->LinearThreshold.Version, EndianBig);
				DataStreamReadSU(&DataStream, &ttf->LinearThreshold.NumberOfGlyphs, EndianBig);

				LinearThreshold.PelsHeightList = Memory::Allocate<Byte__>(LinearThreshold.NumberOfGlyphs);

				DataStreamReadSU(&DataStream, &ttf->LinearThreshold.PelsHeightList, LinearThreshold.NumberOfGlyphs);*/

				break;
			}
			case TTFTableEntryDigitalSignature:
			{
				/*
				Byte__* startPointer = chunkData.Data + chunkData.DataCursor;

				DataStreamReadSU(&DataStream, &ttf->DigitalSignature.Version, EndianBig);
				DataStreamReadSU(&DataStream, &ttf->DigitalSignature.NumberOfSignatures, EndianBig);
				DataStreamReadSU(&DataStream, &ttf->DigitalSignature.Flags, EndianBig);

				DigitalSignature.SignatureRecordList = new TTFDigitalSignatureRecord[DigitalSignature.NumberOfSignatures];
				DigitalSignature.SignatureBlockList = new TTFDigitalSignatureBlock[DigitalSignature.NumberOfSignatures];

				for(size_t i = 0; i < DigitalSignature.NumberOfSignatures; i++)
				{
					TTFDigitalSignatureRecord& signatureRecord = DigitalSignature.SignatureRecordList[i];

					DataStreamReadSU(&DataStream, &ttf->signatureRecord.Format, EndianBig);
					DataStreamReadSU(&DataStream, &ttf->signatureRecord.Length, EndianBig);
					DataStreamReadSU(&DataStream, &ttf->signatureRecord.SignatureBlockOffset, EndianBig);

					{
						DataStreamX byteSteam(startPointer + signatureRecord.SignatureBlockOffset, signatureRecord.Length);
						TTFDigitalSignatureBlock& signatureBlock = DigitalSignature.SignatureBlockList[i];

						DataStreamReadSU(&DataStream, &ttf->signatureBlock.Reserved1, EndianBig);
						DataStreamReadSU(&DataStream, &ttf->signatureBlock.Reserved2, EndianBig);
						DataStreamReadSU(&DataStream, &ttf->signatureBlock.SignatureLength, EndianBig);

						signatureBlock.Signature = (char*)malloc(signatureBlock.SignatureLength);

						DataStreamReadSU(&DataStream, &ttf->signatureBlock.Signature, signatureBlock.SignatureLength);

						for(size_t w = 0; w < signatureBlock.SignatureLength - 2; w++)
						{
							if(signatureBlock.Signature[w] == 0)
							{
								signatureBlock.Signature[w] = '#';
							}
						}
					}
				}*/

				break;
			}
		}
	}

	return ActionSuccessful;
}