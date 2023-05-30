#include "PXTTF.h"

EncodingID PXPXTTFEncodingFromID(const PlatformID platformID, unsigned char encodingID)
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

PXTTFVersionType PXPXTTFVersionTypeFromID(unsigned short major, unsigned short minor)
{
	switch(minor)
	{
		case 0:
		{
			switch(major)
			{
				case 1: return PXTTFVersion1Dot0;
				case 2: return PXTTFVersion2Dot0;
				case 3: return PXTTFVersion3Dot0;
			}

			break;
		}
		case 5:
		{
			if(major == 2)
			{
				return PXTTFVersion2Dot5;
			}

			break;
		}
	}

	return PXTTFVersionInvalid;
}

void PXPXTTFTableEntryTypeToID(unsigned short* major, unsigned short* minor, const PXTTFVersionType versionType)
{

}

PXTTFTableEntryType PXPXTTFTableEntryTypeFromID(const unsigned int tableEntryType)
{
	switch(tableEntryType)
	{
		case PXInt32Make('a', 'c', 'n', 't'): return PXTTFTableEntryAccentAttachment;
		case PXInt32Make('a', 'n', 'k', 'r'): return PXTTFTableEntryAnchorPoint;
		case PXInt32Make('a', 'v', 'a', 'r'): return PXTTFTableEntryAxisVariation;
		case PXInt32Make('b', 'd', 'a', 't'): return PXTTFTableEntryBitmapData;
		case PXInt32Make('b', 'h', 'e', 'd'): return PXTTFTableEntryBitmapFontHeader;
		case PXInt32Make('b', 'l', 'o', 'c'): return PXTTFTableEntryBitmapLocation;
		case PXInt32Make('b', 's', 'l', 'n'): return PXTTFTableEntryBaseline;
		case PXInt32Make('c', 'm', 'a', 'p'): return PXTTFTableEntryCharacterCodeMapping;
		case PXInt32Make('c', 'v', 'a', 'r'): return PXTTFTableEntryCVTVariation;
		case PXInt32Make('c', 'v', 't', ' '): return PXTTFTableEntryControlValue;
		case PXInt32Make('D', 'S', 'I', 'G'): return PXTTFTableEntryDigitalSignature;
		case PXInt32Make('E', 'B', 'S', 'C'): return PXTTFTableEntryEmbeddedBitmapScalingControl;
		case PXInt32Make('f', 'd', 's', 'c'): return PXTTFTableEntryFontDescriptor;
		case PXInt32Make('f', 'e', 'a', 't'): return PXTTFTableEntryLayoutFeature;
		case PXInt32Make('f', 'm', 't', 'x'): return PXTTFTableEntryFontMetrics;
		case PXInt32Make('f', 'o', 'n', 'd'): return PXTTFTableEntryFontFamilyCompatibility;
		case PXInt32Make('f', 'p', 'g', 'm'): return PXTTFTableEntryFontPXProgram;
		case PXInt32Make('f', 'v', 'a', 'r'): return PXTTFTableEntryFontVariation;
		case PXInt32Make('g', 'a', 's', 'p'): return PXTTFTableEntryGridFittingAndScanConversionProcedure;
		case PXInt32Make('g', 'l', 'y', 'f'): return PXTTFTableEntryGlyphOutline;
		case PXInt32Make('g', 'v', 'a', 'r'): return PXTTFTableEntryGlyphVariation;
		case PXInt32Make('h', 'd', 'm', 'x'): return PXTTFTableEntryHorizontalDeviceMetrics;
		case PXInt32Make('h', 'e', 'a', 'd'): return PXTTFTableEntryFontHeader;
		case PXInt32Make('h', 'h', 'e', 'a'): return PXTTFTableEntryHorizontalHeader;
		case PXInt32Make('h', 'm', 't', 'x'): return PXTTFTableEntryHorizontalMetrics;
		case PXInt32Make('j', 'u', 's', 't'): return PXTTFTableEntryJustification;
		case PXInt32Make('k', 'e', 'r', 'n'): return PXTTFTableEntryKerning;
		case PXInt32Make('k', 'e', 'r', 'x'): return PXTTFTableEntryExtendedKerning;
		case PXInt32Make('l', 'c', 'a', 'r'): return PXTTFTableEntryLigatureCaret;
		case PXInt32Make('l', 'o', 'c', 'a'): return PXTTFTableEntryGlyphLocation;
		case PXInt32Make('l', 't', 'a', 'g'): return PXTTFTableEntryLanguageTag;
		case PXInt32Make('L', 'T', 'S', 'H'): return PXTTFTableEntryLinearThreshold;
		case PXInt32Make('m', 'a', 'x', 'p'): return PXTTFTableEntryMaximumProfile;
		case PXInt32Make('m', 'e', 't', 'a'): return PXTTFTableEntryMetadata;
		case PXInt32Make('m', 'o', 'r', 't'): return PXTTFTableEntryMetamorphosisTabledeprecated;
		case PXInt32Make('m', 'o', 'r', 'x'): return PXTTFTableEntryExtendedMetamorphosis;
		case PXInt32Make('n', 'a', 'm', 'e'): return PXTTFTableEntryName;
		case PXInt32Make('o', 'p', 'b', 'd'): return PXTTFTableEntryOpticalBounds;
		case PXInt32Make('O', 'S', '/', '2'): return PXTTFTableEntryCompatibility;
		case PXInt32Make('p', 'o', 's', 't'): return PXTTFTableEntryGlyphNameAndPostScriptCompatibility;
		case PXInt32Make('p', 'r', 'e', 'p'): return PXTTFTableEntryControlValuePXProgram;
		case PXInt32Make('p', 'r', 'o', 'p'): return PXTTFTableEntryProperties;
		case PXInt32Make('s', 'b', 'i', 'x'): return PXTTFTableEntryExtendedBitmaps;
		case PXInt32Make('t', 'r', 'a', 'k'): return PXTTFTableEntryTracking;
		case PXInt32Make('v', 'h', 'e', 'a'): return PXTTFTableEntryVerticalHeader;
		case PXInt32Make('v', 'm', 't', 'x'): return PXTTFTableEntryVerticalMetrics;
		case PXInt32Make('V', 'D', 'M', 'X'): return PXTTFTableEntryVerticalDeviceMetrics;
		case PXInt32Make('x', 'r', 'e', 'f'): return PXTTFTableEntryCrossReference;
		case PXInt32Make('Z', 'a', 'p', 'f'): return PXTTFTableEntryGlyphReference;
		default: return PXTTFTableEntryUnkown;
	}
}

void PXPXTTFConstruct(PXTTF* const ttf)
{
	PXMemoryClear(ttf, sizeof(PXTTF));
}

void PXPXTTFDestruct(PXTTF* const ttf)
{
	// TODO
}

PXActionResult PXPXTTFParse(PXTTF* const ttf, PXFile* const dataStream)
{
	PXTTFOffsetTable offsetTable;

	PXPXTTFConstruct(ttf);

	/*

	PXFileRead(&dataStream, &offsetTable.Version.Major, PXEndianBig);
	PXFileRead(&dataStream, &offsetTable.Version.Minor, PXEndianBig);
	PXFileRead(&dataStream, &offsetTable.NumberOfTables, PXEndianBig);
	PXFileRead(&dataStream, &offsetTable.SearchRange, PXEndianBig);
	PXFileRead(&dataStream, &offsetTable.EntrySelctor, PXEndianBig);
	PXFileRead(&dataStream, &offsetTable.RangeShift, PXEndianBig);

	for(PXSize i = 0; i < offsetTable.NumberOfTables; i++)
	{
		PXTTFTableEntry tableEntry;

		PXFileRead(&dataStream, tableEntry.TypeRaw, 4u);
		PXFileRead(&dataStream, tableEntry.CheckSum, PXEndianBig);
		PXFileRead(&dataStream, tableEntry.Offset, PXEndianBig);
		PXFileRead(&dataStream, tableEntry.Length, PXEndianBig);

		const unsigned int typeID = MakeInt(tableEntry.TypeRaw[0], tableEntry.TypeRaw[1], tableEntry.TypeRaw[2], tableEntry.TypeRaw[3]);

		tableEntry.Type = ConvertTableEntryType(typeID);

		printf
		(
			"[PXTTF][i] Chunk:[%c%c%c%c], Known:%c, Offset:%6i, Length:%6i\n",
			tableEntry.TypeRaw[0],
			tableEntry.TypeRaw[1],
			tableEntry.TypeRaw[2],
			tableEntry.TypeRaw[3],
			tableEntry.Type == PXTTFTableEntryUnkown ? '-' : 'x',
			tableEntry.Offset,
			tableEntry.Length
		);

		switch(tableEntry.Type)
		{
			//---<Essential>---------------------------------------------------		
			case PXTTFTableEntryFontHeader:
			{
				PXFileReadI16U(&dataStream, &ttf->Header.Version.Major, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Header.Version.Minor, PXEndianLittle);
				PXFileReadI16U(&dataStream, &ttf->Header.Revision.Major, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Header.Revision.Minor, PXEndianLittle);
				PXFileReadI32U(&dataStream, &ttf->Header.CheckSumAdjustment, PXEndianBig);
				PXFileReadI32U(&dataStream, &ttf->Header.MagicNumber, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Header.Flags, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Header.UnitsPerEM, PXEndianBig);
				PXFileReadLLU(&dataStream, &ttf->Header.Created, PXEndianBig);
				PXFileReadLLU(&dataStream, &ttf->Header.Modified, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.Minimum[0], PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.Minimum[1], PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.Maximum[0], PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.Maximum[1], PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Header.MacStyle, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Header.LowestRecPpem, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.FontDirectionHint, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.IndexToLocFormat, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Header.GlyphDataFormat, PXEndianBig);
				break;
			}
			case PXTTFTableEntryHorizontalHeader:
			{
				PXFileReadI16U(&dataStream, &ttf->HorizontalHeader.Version.Major, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->HorizontalHeader.Version.Minor, PXEndianLittle);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.Ascender, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.Descender, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.LineGap, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->HorizontalHeader.AdvanceWidthMaximum, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.MinimumLeftSideBearing, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.MinimumRightSideBearing, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.MaximalExtendX, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.CaretSlopeRun, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.CaretSlopeRise, PXEndianBig);
				PXFileReadB(&dataStream, &ttf->HorizontalHeader.Reserved, 10u);
				PXFileReadS(&dataStream, &ttf->HorizontalHeader.MetricDataFormat, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->HorizontalHeader.NumberOfHorizontalMetrics, PXEndianBig);
				break;
			}
			case PXTTFTableEntryMaximumProfile:
			{
				PXFileReadI16U(&dataStream, &ttf->MaximumProfile.Version.Major, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->MaximumProfile.Version.Minor, PXEndianLittle);

				const unsigned char trustedTypeFonts = ttf->MaximumProfile.Version.Major == 1 && ttf->MaximumProfile.Version.Minor == 0;
				const unsigned char openTypeFonts = ttf->MaximumProfile.Version.Major == 0 && ttf->MaximumProfile.Version.Minor == 5;
				const unsigned char validVersion = trustedTypeFonts || openTypeFonts;

				PXFileReadI16U(&dataStream, &ttf->MaximumProfile.NumberOfGlyphs, PXEndianBig);

				assert(validVersion);

				if(trustedTypeFonts && !openTypeFonts)
				{
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.PointsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.ContoursMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.ComponentPointsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.ComponentContoursMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.ZonesMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.TwilightPointsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.StorageMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.FunctionDefsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.InstructionDefsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.StackElementsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.SizeOfInstructionsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.ComponentElementsMaximal, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->MaximumProfile.ComponentDepthMaximal, PXEndianBig);
				}

				break;
			}
			case PXTTFTableEntryCompatibility:
			{
				PXFileReadI16U(&dataStream, &ttf->Compatibility.Version, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.xAvgCharWidth, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.usWeightClass, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.usWidthClass, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.fsType, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySubscriptXSize, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySubscriptYSize, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySubscriptXOffset, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySubscriptYOffset, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySuperscriptXSize, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySuperscriptYSize, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySuperscriptXOffset, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.ySuperscriptYOffset, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.yStrikeoutPosition, PXEndianBig);
				PXFileReadS(&dataStream, &ttf->Compatibility.sFamilyClass, PXEndianBig);

				// Parse PANROSE
				{
					PXTTFPanose* panrose = &ttf->Compatibility.Panose;
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

					PXFileReadI8U(&dataStream, &bFamilyType);
					PXFileReadI8U(&dataStream, &bSerifStyle);
					PXFileReadI8U(&dataStream, &bWeight);
					PXFileReadI8U(&dataStream, &bProportion);
					PXFileReadI8U(&dataStream, &bContrast);
					PXFileReadI8U(&dataStream, &bStrokeVariation);
					PXFileReadI8U(&dataStream, &bArmStyle);
					PXFileReadI8U(&dataStream, &bLetterform);
					PXFileReadI8U(&dataStream, &bMidline);
					PXFileReadI8U(&dataStream, &bXHeight);

					//panrose.FamilyType = ConvertPXTTFFamilyType(bFamilyType);
					//panrose.SerifStyle = ConvertPXTTFSerifStyle(bSerifStyle);
					//panrose.Weight = ConvertPXTTFWeight(bWeight);
					//panrose.Proportion = ConvertPXTTFProportion(bProportion);
					//panrose.Contrast = ConvertPXTTFContrast(bContrast);
					//panrose.StrokeVariation = ConvertPXTTFStrokeVariatoon(bStrokeVariation);
					//panrose.ArmStyle = ConvertPXTTFArmStyle(bArmStyle);
					//panrose.Letterform = ConvertPXTTFLetterform(bLetterform);
					//panrose.Midline = ConvertPXTTFMidline(bMidline);
					//panrose.HeightX = ConvertPXTTFHeightX(bXHeight);
				}

				PXFileReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[0], PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[1], PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[2], PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.ulUnicodeRange[3], PXEndianBig);

				PXFileReadB(&dataStream, &ttf->Compatibility.achVendID, 4u);

				PXFileReadI16U(&dataStream, &ttf->Compatibility.fsSelection, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.fsFirstCharIndex, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Compatibility.fsLastCharIndex, PXEndianBig);

				if(ttf->Compatibility.Version > 0)
				{
					PXFileReadI16U(&dataStream, &ttf->Compatibility.sTypoAscender, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.sTypoDescender, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.sTypoLineGap, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usWinAscent, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usWinDescent, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.ulCodePageRange1, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.ulCodePageRange2, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.sxHeight, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.sCapHeight, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usDefaultChar, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usBreakChar, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usMaxContext, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usLowerPointSize, PXEndianBig);
					PXFileReadI16U(&dataStream, &ttf->Compatibility.usUpperPointSize, PXEndianBig);
				}

				break;
			}

			case PXTTFTableEntryControlValue: // cvt
			{
				// FWORD[ n ] 	List of n values referenceable by instructions. n is the number of FWORD items that fit in the size of the table.
				break;
			}
			case PXTTFTableEntryGlyphNameAndPostScriptCompatibility: // post
			{
				PXFileReadI16U(&dataStream, &ttf->PostScript.Version.Major, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->PostScript.Version.Minor, PXEndianLittle);

				//PostScript.Version.Check();

				/*

				switch(PostScript.Version.Type)
				{
					case PXTTFVersionInvalid:
						break;

					case PXTTFVersion1Dot0:
						break;

					case PXTTFVersion2Dot0:
					{
						PXFileReadI16U(&PXFile, &ttf->PostScript.NumberOfGlyphs, PXEndianBig);
						break;
					}
					case PXTTFVersion2Dot5:
					{
						PXFileReadI16U(&PXFile, &ttf->PostScript.NumberOfGlyphs, PXEndianBig);
						break;
					}
					case PXTTFVersion3Dot0:
						break;

					default:
						printf("Illegal Version");
						break;
				}* /

				break;
			}
			case PXTTFTableEntryKerning: // kern
			{
				PXFileReadI16U(&dataStream, &ttf->Kerning.Version, PXEndianBig);
				PXFileReadI16U(&dataStream, &ttf->Kerning.NumberOfSubtables, PXEndianBig);

				/*

				Kerning.KerningSubtableList = new PXTTFKerningSubtable[Kerning.NumberOfSubtables];

				for(PXSize i = 0; i < Kerning.NumberOfSubtables; i++)
				{
					PXTTFKerningSubtable& kerningTable = Kerning.KerningSubtableList[i];

					unsigned short coverage = 0;

					PXFileReadI16U(&PXFile, &ttf->kerningTable.Version, PXEndianBig);
					PXFileReadI16U(&PXFile, &ttf->kerningTable.Length, PXEndianBig);
					PXFileReadI16U(&PXFile, &ttf->coverage, PXEndianBig);

					kerningTable.ParseCoverageValue(coverage);

					switch(kerningTable.Version)
					{
						case 0:
						{
							PXTTFSubtableFormat0& subtableFormat = kerningTable.SubtableFormat0;

							PXFileReadI16U(&PXFile, &ttf->subtableFormat.NumberOfPairs, PXEndianBig);
							PXFileReadI16U(&PXFile, &ttf->subtableFormat.SearchRange, PXEndianBig);
							PXFileReadI16U(&PXFile, &ttf->subtableFormat.EntrySelector, PXEndianBig);
							PXFileReadI16U(&PXFile, &ttf->subtableFormat.RangeShift, PXEndianBig);

							subtableFormat.KerningPairList = new PXTTFKerningPair[subtableFormat.NumberOfPairs];

							for(PXSize i = 0; i < subtableFormat.NumberOfPairs; i++)
							{
								PXTTFKerningPair& kerningPair = subtableFormat.KerningPairList[i];

								PXFileReadI16U(&PXFile, &ttf->kerningPair.Left, PXEndianBig);
								PXFileReadI16U(&PXFile, &ttf->kerningPair.Right, PXEndianBig);
								PXFileReadI16U(&PXFile, &ttf->kerningPair.Value, PXEndianBig);
							}

							break;
						}
						case 2:
						{
							PXTTFSubtableFormat2& subtableFormat = kerningTable.SubtableFormat2;

							PXFileReadI16U(&PXFile, &ttf->subtableFormat.RowWidth, PXEndianBig);
							PXFileReadI16U(&PXFile, &ttf->subtableFormat.LeftClassOffset, PXEndianBig);
							PXFileReadI16U(&PXFile, &ttf->subtableFormat.RightClassOffset, PXEndianBig);
							PXFileReadI16U(&PXFile, &ttf->subtableFormat.KerningArrayOffset, PXEndianBig);

							break;
						}
					}
				}* /

				break;
			}
			case PXTTFTableEntryControlValuePXProgram: // prep
			{
				// The name 'prep' is anachronistic (the table used to be known as the Pre PXProgram table.)
				// unsigned char[N]
				break;
			}
			case PXTTFTableEntryGlyphOutline: // glyf
			{
				break;
			}
			case PXTTFTableEntryFontPXProgram: // fpgm
			{
				break;
			}
			//-----------------------------------------------------------------

			// Windows 
			case PXTTFTableEntryCharacterCodeMapping:
			{
				/*
				PXFileReadI16U(&PXFile, &ttf->CharacterMapping.Version, PXEndianBig);
				PXFileReadI16U(&PXFile, &ttf->CharacterMapping.NumberOfTables, PXEndianBig);

				CharacterMapping.EncodingRecordList = new EncodingRecord[CharacterMapping.NumberOfTables];

				for(PXSize i = 0; i < CharacterMapping.NumberOfTables; i++)
				{
					EncodingRecord& encodingRecord = CharacterMapping.EncodingRecordList[i];

					unsigned short platformID;
					unsigned short encodingID;

					PXFileReadI16U(&PXFile, &ttf->platformID, PXEndianBig);
					PXFileReadI16U(&PXFile, &ttf->encodingID, PXEndianBig);
					PXFileReadI16U(&PXFile, &ttf->encodingRecord.SubtableOffset, PXEndianBig);

					encodingRecord.Platform = ConvertPlatformID(platformID);
					encodingRecord.Encoding = ConvertEncodingID(encodingRecord.Platform, encodingID);
				}
				* /
				break;
			}
			case PXTTFTableEntryLinearThreshold:
			{
				/*
				PXFileReadI16U(&PXFile, &ttf->LinearThreshold.Version, PXEndianBig);
				PXFileReadI16U(&PXFile, &ttf->LinearThreshold.NumberOfGlyphs, PXEndianBig);

				LinearThreshold.PelsHeightList = Memory::Allocate<Byte__>(LinearThreshold.NumberOfGlyphs);

				PXFileReadI16U(&PXFile, &ttf->LinearThreshold.PelsHeightList, LinearThreshold.NumberOfGlyphs);* /

				break;
			}
			case PXTTFTableEntryDigitalSignature:
			{
				/*
				Byte__* startPointer = chunkData.Data + chunkData.DataCursor;

				PXFileReadI16U(&PXFile, &ttf->DigitalSignature.Version, PXEndianBig);
				PXFileReadI16U(&PXFile, &ttf->DigitalSignature.NumberOfSignatures, PXEndianBig);
				PXFileReadI16U(&PXFile, &ttf->DigitalSignature.Flags, PXEndianBig);

				DigitalSignature.SignatureRecordList = new PXTTFDigitalSignatureRecord[DigitalSignature.NumberOfSignatures];
				DigitalSignature.SignatureBlockList = new PXTTFDigitalSignatureBlock[DigitalSignature.NumberOfSignatures];

				for(PXSize i = 0; i < DigitalSignature.NumberOfSignatures; i++)
				{
					PXTTFDigitalSignatureRecord& signatureRecord = DigitalSignature.SignatureRecordList[i];

					PXFileReadI16U(&PXFile, &ttf->signatureRecord.Format, PXEndianBig);
					PXFileReadI16U(&PXFile, &ttf->signatureRecord.Length, PXEndianBig);
					PXFileReadI16U(&PXFile, &ttf->signatureRecord.SignatureBlockOffset, PXEndianBig);

					{
						PXFileX byteSteam(startPointer + signatureRecord.SignatureBlockOffset, signatureRecord.Length);
						PXTTFDigitalSignatureBlock& signatureBlock = DigitalSignature.SignatureBlockList[i];

						PXFileReadI16U(&PXFile, &ttf->signatureBlock.Reserved1, PXEndianBig);
						PXFileReadI16U(&PXFile, &ttf->signatureBlock.Reserved2, PXEndianBig);
						PXFileReadI16U(&PXFile, &ttf->signatureBlock.SignatureLength, PXEndianBig);

						signatureBlock.Signature = (char*)malloc(signatureBlock.SignatureLength);

						PXFileReadI16U(&PXFile, &ttf->signatureBlock.Signature, signatureBlock.SignatureLength);

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