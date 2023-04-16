#include "TTF.h"

#include <OS/Memory/PXMemory.h>

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
		case PXInt32Make('a', 'c', 'n', 't'): return TTFTableEntryAccentAttachment;
		case PXInt32Make('a', 'n', 'k', 'r'): return TTFTableEntryAnchorPoint;
		case PXInt32Make('a', 'v', 'a', 'r'): return TTFTableEntryAxisVariation;
		case PXInt32Make('b', 'd', 'a', 't'): return TTFTableEntryBitmapData;
		case PXInt32Make('b', 'h', 'e', 'd'): return TTFTableEntryBitmapFontHeader;
		case PXInt32Make('b', 'l', 'o', 'c'): return TTFTableEntryBitmapLocation;
		case PXInt32Make('b', 's', 'l', 'n'): return TTFTableEntryBaseline;
		case PXInt32Make('c', 'm', 'a', 'p'): return TTFTableEntryCharacterCodeMapping;
		case PXInt32Make('c', 'v', 'a', 'r'): return TTFTableEntryCVTVariation;
		case PXInt32Make('c', 'v', 't', ' '): return TTFTableEntryControlValue;
		case PXInt32Make('D', 'S', 'I', 'G'): return TTFTableEntryDigitalSignature;
		case PXInt32Make('E', 'B', 'S', 'C'): return TTFTableEntryEmbeddedBitmapScalingControl;
		case PXInt32Make('f', 'd', 's', 'c'): return TTFTableEntryFontDescriptor;
		case PXInt32Make('f', 'e', 'a', 't'): return TTFTableEntryLayoutFeature;
		case PXInt32Make('f', 'm', 't', 'x'): return TTFTableEntryFontMetrics;
		case PXInt32Make('f', 'o', 'n', 'd'): return TTFTableEntryFontFamilyCompatibility;
		case PXInt32Make('f', 'p', 'g', 'm'): return TTFTableEntryFontProgram;
		case PXInt32Make('f', 'v', 'a', 'r'): return TTFTableEntryFontVariation;
		case PXInt32Make('g', 'a', 's', 'p'): return TTFTableEntryGridFittingAndScanConversionProcedure;
		case PXInt32Make('g', 'l', 'y', 'f'): return TTFTableEntryGlyphOutline;
		case PXInt32Make('g', 'v', 'a', 'r'): return TTFTableEntryGlyphVariation;
		case PXInt32Make('h', 'd', 'm', 'x'): return TTFTableEntryHorizontalDeviceMetrics;
		case PXInt32Make('h', 'e', 'a', 'd'): return TTFTableEntryFontHeader;
		case PXInt32Make('h', 'h', 'e', 'a'): return TTFTableEntryHorizontalHeader;
		case PXInt32Make('h', 'm', 't', 'x'): return TTFTableEntryHorizontalMetrics;
		case PXInt32Make('j', 'u', 's', 't'): return TTFTableEntryJustification;
		case PXInt32Make('k', 'e', 'r', 'n'): return TTFTableEntryKerning;
		case PXInt32Make('k', 'e', 'r', 'x'): return TTFTableEntryExtendedKerning;
		case PXInt32Make('l', 'c', 'a', 'r'): return TTFTableEntryLigatureCaret;
		case PXInt32Make('l', 'o', 'c', 'a'): return TTFTableEntryGlyphLocation;
		case PXInt32Make('l', 't', 'a', 'g'): return TTFTableEntryLanguageTag;
		case PXInt32Make('L', 'T', 'S', 'H'): return TTFTableEntryLinearThreshold;
		case PXInt32Make('m', 'a', 'x', 'p'): return TTFTableEntryMaximumProfile;
		case PXInt32Make('m', 'e', 't', 'a'): return TTFTableEntryMetadata;
		case PXInt32Make('m', 'o', 'r', 't'): return TTFTableEntryMetamorphosisTabledeprecated;
		case PXInt32Make('m', 'o', 'r', 'x'): return TTFTableEntryExtendedMetamorphosis;
		case PXInt32Make('n', 'a', 'm', 'e'): return TTFTableEntryName;
		case PXInt32Make('o', 'p', 'b', 'd'): return TTFTableEntryOpticalBounds;
		case PXInt32Make('O', 'S', '/', '2'): return TTFTableEntryCompatibility;
		case PXInt32Make('p', 'o', 's', 't'): return TTFTableEntryGlyphNameAndPostScriptCompatibility;
		case PXInt32Make('p', 'r', 'e', 'p'): return TTFTableEntryControlValueProgram;
		case PXInt32Make('p', 'r', 'o', 'p'): return TTFTableEntryProperties;
		case PXInt32Make('s', 'b', 'i', 'x'): return TTFTableEntryExtendedBitmaps;
		case PXInt32Make('t', 'r', 'a', 'k'): return TTFTableEntryTracking;
		case PXInt32Make('v', 'h', 'e', 'a'): return TTFTableEntryVerticalHeader;
		case PXInt32Make('v', 'm', 't', 'x'): return TTFTableEntryVerticalMetrics;
		case PXInt32Make('V', 'D', 'M', 'X'): return TTFTableEntryVerticalDeviceMetrics;
		case PXInt32Make('x', 'r', 'e', 'f'): return TTFTableEntryCrossReference;
		case PXInt32Make('Z', 'a', 'p', 'f'): return TTFTableEntryGlyphReference;
		default: return TTFTableEntryUnkown;
	}
}

void TTFConstruct(TTF* const ttf)
{
	MemoryClear(ttf, sizeof(TTF));
}

void TTFDestruct(TTF* const ttf)
{
	// TODO
}

PXActionResult TTFParse(TTF* const ttf, PXDataStream* const dataStream)
{
	TTFOffsetTable offsetTable;

	TTFConstruct(ttf);

	/*

	PXDataStreamRead(&dataStream, &offsetTable.Version.Major, EndianBig);
	PXDataStreamRead(&dataStream, &offsetTable.Version.Minor, EndianBig);
	PXDataStreamRead(&dataStream, &offsetTable.NumberOfTables, EndianBig);
	PXDataStreamRead(&dataStream, &offsetTable.SearchRange, EndianBig);
	PXDataStreamRead(&dataStream, &offsetTable.EntrySelctor, EndianBig);
	PXDataStreamRead(&dataStream, &offsetTable.RangeShift, EndianBig);

	for(PXSize i = 0; i < offsetTable.NumberOfTables; i++)
	{
		TTFTableEntry tableEntry;

		PXDataStreamRead(&dataStream, tableEntry.TypeRaw, 4u);
		PXDataStreamRead(&dataStream, tableEntry.CheckSum, EndianBig);
		PXDataStreamRead(&dataStream, tableEntry.Offset, EndianBig);
		PXDataStreamRead(&dataStream, tableEntry.Length, EndianBig);

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
				PXDataStreamReadI16U(&dataStream, &ttf->Header.Version.Major, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.Version.Minor, EndianLittle);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.Revision.Major, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.Revision.Minor, EndianLittle);
				PXDataStreamReadI32U(&dataStream, &ttf->Header.CheckSumAdjustment, EndianBig);
				PXDataStreamReadI32U(&dataStream, &ttf->Header.MagicNumber, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.Flags, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.UnitsPerEM, EndianBig);
				PXDataStreamReadLLU(&dataStream, &ttf->Header.Created, EndianBig);
				PXDataStreamReadLLU(&dataStream, &ttf->Header.Modified, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.Minimum[0], EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.Minimum[1], EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.Maximum[0], EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.Maximum[1], EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.MacStyle, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Header.LowestRecPpem, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.FontDirectionHint, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.IndexToLocFormat, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Header.GlyphDataFormat, EndianBig);
				break;
			}
			case TTFTableEntryHorizontalHeader:
			{
				PXDataStreamReadI16U(&dataStream, &ttf->HorizontalHeader.Version.Major, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->HorizontalHeader.Version.Minor, EndianLittle);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.Ascender, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.Descender, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.LineGap, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->HorizontalHeader.AdvanceWidthMaximum, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.MinimumLeftSideBearing, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.MinimumRightSideBearing, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.MaximalExtendX, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.CaretSlopeRun, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.CaretSlopeRise, EndianBig);
				PXDataStreamReadB(&dataStream, &ttf->HorizontalHeader.Reserved, 10u);
				PXDataStreamReadS(&dataStream, &ttf->HorizontalHeader.MetricDataFormat, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->HorizontalHeader.NumberOfHorizontalMetrics, EndianBig);
				break;
			}
			case TTFTableEntryMaximumProfile:
			{
				PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.Version.Major, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.Version.Minor, EndianLittle);

				const unsigned char trustedTypeFonts = ttf->MaximumProfile.Version.Major == 1 && ttf->MaximumProfile.Version.Minor == 0;
				const unsigned char openTypeFonts = ttf->MaximumProfile.Version.Major == 0 && ttf->MaximumProfile.Version.Minor == 5;
				const unsigned char validVersion = trustedTypeFonts || openTypeFonts;

				PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.NumberOfGlyphs, EndianBig);

				assert(validVersion);

				if(trustedTypeFonts && !openTypeFonts)
				{
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.PointsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.ContoursMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.ComponentPointsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.ComponentContoursMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.ZonesMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.TwilightPointsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.StorageMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.FunctionDefsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.InstructionDefsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.StackElementsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.SizeOfInstructionsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.ComponentElementsMaximal, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->MaximumProfile.ComponentDepthMaximal, EndianBig);
				}

				break;
			}
			case TTFTableEntryCompatibility:
			{
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.Version, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.xAvgCharWidth, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usWeightClass, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usWidthClass, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.fsType, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptXSize, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptYSize, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptXOffset, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySubscriptYOffset, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptXSize, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptYSize, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptXOffset, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.ySuperscriptYOffset, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.yStrikeoutPosition, EndianBig);
				PXDataStreamReadS(&dataStream, &ttf->Compatibility.sFamilyClass, EndianBig);

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

					PXDataStreamReadI8U(&dataStream, &bFamilyType);
					PXDataStreamReadI8U(&dataStream, &bSerifStyle);
					PXDataStreamReadI8U(&dataStream, &bWeight);
					PXDataStreamReadI8U(&dataStream, &bProportion);
					PXDataStreamReadI8U(&dataStream, &bContrast);
					PXDataStreamReadI8U(&dataStream, &bStrokeVariation);
					PXDataStreamReadI8U(&dataStream, &bArmStyle);
					PXDataStreamReadI8U(&dataStream, &bLetterform);
					PXDataStreamReadI8U(&dataStream, &bMidline);
					PXDataStreamReadI8U(&dataStream, &bXHeight);

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

				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[0], EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[1], EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[2], EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[3], EndianBig);

				PXDataStreamReadB(&dataStream, &ttf->Compatibility.achVendID, 4u);

				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.fsSelection, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.fsFirstCharIndex, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.fsLastCharIndex, EndianBig);

				if(ttf->Compatibility.Version > 0)
				{
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.sTypoAscender, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.sTypoDescender, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.sTypoLineGap, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usWinAscent, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usWinDescent, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.ulCodePageRange1, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.ulCodePageRange2, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.sxHeight, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.sCapHeight, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usDefaultChar, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usBreakChar, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usMaxContext, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usLowerPointSize, EndianBig);
					PXDataStreamReadI16U(&dataStream, &ttf->Compatibility.usUpperPointSize, EndianBig);
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
				PXDataStreamReadI16U(&dataStream, &ttf->PostScript.Version.Major, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->PostScript.Version.Minor, EndianLittle);

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
						PXDataStreamReadI16U(&PXDataStream, &ttf->PostScript.NumberOfGlyphs, EndianBig);
						break;
					}
					case TTFVersion2Dot5:
					{
						PXDataStreamReadI16U(&PXDataStream, &ttf->PostScript.NumberOfGlyphs, EndianBig);
						break;
					}
					case TTFVersion3Dot0:
						break;

					default:
						printf("Illegal Version");
						break;
				}* /

				break;
			}
			case TTFTableEntryKerning: // kern
			{
				PXDataStreamReadI16U(&dataStream, &ttf->Kerning.Version, EndianBig);
				PXDataStreamReadI16U(&dataStream, &ttf->Kerning.NumberOfSubtables, EndianBig);

				/*

				Kerning.KerningSubtableList = new TTFKerningSubtable[Kerning.NumberOfSubtables];

				for(PXSize i = 0; i < Kerning.NumberOfSubtables; i++)
				{
					TTFKerningSubtable& kerningTable = Kerning.KerningSubtableList[i];

					unsigned short coverage = 0;

					PXDataStreamReadI16U(&PXDataStream, &ttf->kerningTable.Version, EndianBig);
					PXDataStreamReadI16U(&PXDataStream, &ttf->kerningTable.Length, EndianBig);
					PXDataStreamReadI16U(&PXDataStream, &ttf->coverage, EndianBig);

					kerningTable.ParseCoverageValue(coverage);

					switch(kerningTable.Version)
					{
						case 0:
						{
							TTFSubtableFormat0& subtableFormat = kerningTable.SubtableFormat0;

							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.NumberOfPairs, EndianBig);
							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.SearchRange, EndianBig);
							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.EntrySelector, EndianBig);
							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.RangeShift, EndianBig);

							subtableFormat.KerningPairList = new TTFKerningPair[subtableFormat.NumberOfPairs];

							for(PXSize i = 0; i < subtableFormat.NumberOfPairs; i++)
							{
								TTFKerningPair& kerningPair = subtableFormat.KerningPairList[i];

								PXDataStreamReadI16U(&PXDataStream, &ttf->kerningPair.Left, EndianBig);
								PXDataStreamReadI16U(&PXDataStream, &ttf->kerningPair.Right, EndianBig);
								PXDataStreamReadI16U(&PXDataStream, &ttf->kerningPair.Value, EndianBig);
							}

							break;
						}
						case 2:
						{
							TTFSubtableFormat2& subtableFormat = kerningTable.SubtableFormat2;

							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.RowWidth, EndianBig);
							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.LeftClassOffset, EndianBig);
							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.RightClassOffset, EndianBig);
							PXDataStreamReadI16U(&PXDataStream, &ttf->subtableFormat.KerningArrayOffset, EndianBig);

							break;
						}
					}
				}* /

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
				PXDataStreamReadI16U(&PXDataStream, &ttf->CharacterMapping.Version, EndianBig);
				PXDataStreamReadI16U(&PXDataStream, &ttf->CharacterMapping.NumberOfTables, EndianBig);

				CharacterMapping.EncodingRecordList = new EncodingRecord[CharacterMapping.NumberOfTables];

				for(PXSize i = 0; i < CharacterMapping.NumberOfTables; i++)
				{
					EncodingRecord& encodingRecord = CharacterMapping.EncodingRecordList[i];

					unsigned short platformID;
					unsigned short encodingID;

					PXDataStreamReadI16U(&PXDataStream, &ttf->platformID, EndianBig);
					PXDataStreamReadI16U(&PXDataStream, &ttf->encodingID, EndianBig);
					PXDataStreamReadI16U(&PXDataStream, &ttf->encodingRecord.SubtableOffset, EndianBig);

					encodingRecord.Platform = ConvertPlatformID(platformID);
					encodingRecord.Encoding = ConvertEncodingID(encodingRecord.Platform, encodingID);
				}
				* /
				break;
			}
			case TTFTableEntryLinearThreshold:
			{
				/*
				PXDataStreamReadI16U(&PXDataStream, &ttf->LinearThreshold.Version, EndianBig);
				PXDataStreamReadI16U(&PXDataStream, &ttf->LinearThreshold.NumberOfGlyphs, EndianBig);

				LinearThreshold.PelsHeightList = Memory::Allocate<Byte__>(LinearThreshold.NumberOfGlyphs);

				PXDataStreamReadI16U(&PXDataStream, &ttf->LinearThreshold.PelsHeightList, LinearThreshold.NumberOfGlyphs);* /

				break;
			}
			case TTFTableEntryDigitalSignature:
			{
				/*
				Byte__* startPointer = chunkData.Data + chunkData.DataCursor;

				PXDataStreamReadI16U(&PXDataStream, &ttf->DigitalSignature.Version, EndianBig);
				PXDataStreamReadI16U(&PXDataStream, &ttf->DigitalSignature.NumberOfSignatures, EndianBig);
				PXDataStreamReadI16U(&PXDataStream, &ttf->DigitalSignature.Flags, EndianBig);

				DigitalSignature.SignatureRecordList = new TTFDigitalSignatureRecord[DigitalSignature.NumberOfSignatures];
				DigitalSignature.SignatureBlockList = new TTFDigitalSignatureBlock[DigitalSignature.NumberOfSignatures];

				for(PXSize i = 0; i < DigitalSignature.NumberOfSignatures; i++)
				{
					TTFDigitalSignatureRecord& signatureRecord = DigitalSignature.SignatureRecordList[i];

					PXDataStreamReadI16U(&PXDataStream, &ttf->signatureRecord.Format, EndianBig);
					PXDataStreamReadI16U(&PXDataStream, &ttf->signatureRecord.Length, EndianBig);
					PXDataStreamReadI16U(&PXDataStream, &ttf->signatureRecord.SignatureBlockOffset, EndianBig);

					{
						PXDataStreamX byteSteam(startPointer + signatureRecord.SignatureBlockOffset, signatureRecord.Length);
						TTFDigitalSignatureBlock& signatureBlock = DigitalSignature.SignatureBlockList[i];

						PXDataStreamReadI16U(&PXDataStream, &ttf->signatureBlock.Reserved1, EndianBig);
						PXDataStreamReadI16U(&PXDataStream, &ttf->signatureBlock.Reserved2, EndianBig);
						PXDataStreamReadI16U(&PXDataStream, &ttf->signatureBlock.SignatureLength, EndianBig);

						signatureBlock.Signature = (char*)malloc(signatureBlock.SignatureLength);

						PXDataStreamReadI16U(&PXDataStream, &ttf->signatureBlock.Signature, signatureBlock.SignatureLength);

						for(PXSize w = 0; w < signatureBlock.SignatureLength - 2; w++)
						{
							if(signatureBlock.Signature[w] == 0)
							{
								signatureBlock.Signature[w] = '#';
							}
						}
					}
				}* /

				break;
			}
		}
	}
	*/

	return PXActionSuccessful;
}