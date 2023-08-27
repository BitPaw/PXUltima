#include "PXTTF.h"

PXPlatformID PXTTFPlatformFromID(const PXInt16U platformID)
{
	switch (platformID)
	{
		case 0u: return PXPlatformUnicode;
		case 1u: return PXPlatformMacintosh;
		case 2u: return PXPlatformReserved;
		case 3u: return PXPlatformWindows;

		default:
			return PXPlatformInvalid;
	}
}

PXEncodingID PXTTFEncodingFromID(const PXPlatformID platformID, const PXInt16U encodingID)
{
	switch(platformID)
	{
		case PXPlatformUnicode:
		{
			switch(encodingID)
			{
				case 0: return PXEncodingUnicode10;
				case 1:	return PXEncodingUnicode11;
				case 2:	return PXEncodingISOIEC10646;
				case 3:	return PXEncodingUnicode20BMPOnly;
				case 4: return PXEncodingUnicode20FullRepertoire;
				case 5: return PXEncodingUnicodeVariation;
				case 6: return PXEncodingUnicodeFullRepertoire;
			}

			break;
		}
		case PXPlatformMacintosh:
		{
			// ???
			break;
		}
		case PXPlatformISO:
		{
			switch(encodingID)
			{
				case 0:	return PXEncodingSevenBitASCII;
				case 1:	return PXEncodingISO10646;
				case 2:	return PXEncodingISO8858;
			}

			break;
		}
		case PXPlatformWindows:
		{
			switch(encodingID)
			{
				case 0: return PXEncodingSymbol;
				case 1: return PXEncodingUnicodeBMP;
				case 2: return PXEncodingShiftJIS;
				case 3: return PXEncodingPRC;
				case 4: return PXEncodingBig5;
				case 5: return PXEncodingWansung;
				case 6: return PXEncodingJohab;

				case 7: // fall through
				case 8: // fall through
				case 9: // fall through
					return PXEncodingReserved;

				case 10: return PXEncodingUnicodeFullRepertoire;
			}

			break;
		}
		case PXPlatformCustom:
			return PXEncodingOTFWindowsNTCompatibilityMapping;

		case PXPlatformInvalid:
			return PXEncodingInvalid;
	}

	return PXEncodingInvalid;
}


PXTTFVersionType PXTTFVersionTypeFromID(unsigned short major, unsigned short minor)
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

void PXTTFTableEntryTypeToID(unsigned short* major, unsigned short* minor, const PXTTFVersionType versionType)
{

}

PXTTFTableEntryType PXTTFTableEntryTypeFromID(const PXInt32U tableEntryType)
{
	switch(tableEntryType)
	{
		case PXInt32MakeEndianBig('a', 'c', 'n', 't'): return PXTTFTableEntryAccentAttachment;
		case PXInt32MakeEndianBig('a', 'n', 'k', 'r'): return PXTTFTableEntryAnchorPoint;
		case PXInt32MakeEndianBig('a', 'v', 'a', 'r'): return PXTTFTableEntryAxisVariation;
		case PXInt32MakeEndianBig('b', 'd', 'a', 't'): return PXTTFTableEntryBitmapData;
		case PXInt32MakeEndianBig('b', 'h', 'e', 'd'): return PXTTFTableEntryBitmapFontHeader;
		case PXInt32MakeEndianBig('b', 'l', 'o', 'c'): return PXTTFTableEntryBitmapLocation;
		case PXInt32MakeEndianBig('b', 's', 'l', 'n'): return PXTTFTableEntryBaseline;
		case PXInt32MakeEndianBig('c', 'm', 'a', 'p'): return PXTTFTableEntryCharacterCodeMapping;
		case PXInt32MakeEndianBig('c', 'v', 'a', 'r'): return PXTTFTableEntryCVTVariation;
		case PXInt32MakeEndianBig('c', 'v', 't', ' '): return PXTTFTableEntryControlValue;
		case PXInt32MakeEndianBig('D', 'S', 'I', 'G'): return PXTTFTableEntryDigitalSignature;
		case PXInt32MakeEndianBig('E', 'B', 'S', 'C'): return PXTTFTableEntryEmbeddedBitmapScalingControl;
		case PXInt32MakeEndianBig('f', 'd', 's', 'c'): return PXTTFTableEntryFontDescriptor;
		case PXInt32MakeEndianBig('f', 'e', 'a', 't'): return PXTTFTableEntryLayoutFeature;
		case PXInt32MakeEndianBig('f', 'm', 't', 'x'): return PXTTFTableEntryFontMetrics;
		case PXInt32MakeEndianBig('f', 'o', 'n', 'd'): return PXTTFTableEntryFontFamilyCompatibility;
		case PXInt32MakeEndianBig('f', 'p', 'g', 'm'): return PXTTFTableEntryFontPXProgram;
		case PXInt32MakeEndianBig('f', 'v', 'a', 'r'): return PXTTFTableEntryFontVariation;
		case PXInt32MakeEndianBig('g', 'a', 's', 'p'): return PXTTFTableEntryGridFittingAndScanConversionProcedure;
		case PXInt32MakeEndianBig('g', 'l', 'y', 'f'): return PXTTFTableEntryGlyphOutline;
		case PXInt32MakeEndianBig('g', 'v', 'a', 'r'): return PXTTFTableEntryGlyphVariation;
		case PXInt32MakeEndianBig('h', 'd', 'm', 'x'): return PXTTFTableEntryHorizontalDeviceMetrics;
		case PXInt32MakeEndianBig('h', 'e', 'a', 'd'): return PXTTFTableEntryFontHeader;
		case PXInt32MakeEndianBig('h', 'h', 'e', 'a'): return PXTTFTableEntryHorizontalHeader;
		case PXInt32MakeEndianBig('h', 'm', 't', 'x'): return PXTTFTableEntryHorizontalMetrics;
		case PXInt32MakeEndianBig('j', 'u', 's', 't'): return PXTTFTableEntryJustification;
		case PXInt32MakeEndianBig('k', 'e', 'r', 'n'): return PXTTFTableEntryKerning;
		case PXInt32MakeEndianBig('k', 'e', 'r', 'x'): return PXTTFTableEntryExtendedKerning;
		case PXInt32MakeEndianBig('l', 'c', 'a', 'r'): return PXTTFTableEntryLigatureCaret;
		case PXInt32MakeEndianBig('l', 'o', 'c', 'a'): return PXTTFTableEntryGlyphLocation;
		case PXInt32MakeEndianBig('l', 't', 'a', 'g'): return PXTTFTableEntryLanguageTag;
		case PXInt32MakeEndianBig('L', 'T', 'S', 'H'): return PXTTFTableEntryLinearThreshold;
		case PXInt32MakeEndianBig('m', 'a', 'x', 'p'): return PXTTFTableEntryMaximumProfile;
		case PXInt32MakeEndianBig('m', 'e', 't', 'a'): return PXTTFTableEntryMetadata;
		case PXInt32MakeEndianBig('m', 'o', 'r', 't'): return PXTTFTableEntryMetamorphosisTabledeprecated;
		case PXInt32MakeEndianBig('m', 'o', 'r', 'x'): return PXTTFTableEntryExtendedMetamorphosis;
		case PXInt32MakeEndianBig('n', 'a', 'm', 'e'): return PXTTFTableEntryName;
		case PXInt32MakeEndianBig('o', 'p', 'b', 'd'): return PXTTFTableEntryOpticalBounds;
		case PXInt32MakeEndianBig('O', 'S', '/', '2'): return PXTTFTableEntryCompatibility;
		case PXInt32MakeEndianBig('p', 'o', 's', 't'): return PXTTFTableEntryGlyphNameAndPostScriptCompatibility;
		case PXInt32MakeEndianBig('p', 'r', 'e', 'p'): return PXTTFTableEntryControlValuePXProgram;
		case PXInt32MakeEndianBig('p', 'r', 'o', 'p'): return PXTTFTableEntryProperties;
		case PXInt32MakeEndianBig('s', 'b', 'i', 'x'): return PXTTFTableEntryExtendedBitmaps;
		case PXInt32MakeEndianBig('t', 'r', 'a', 'k'): return PXTTFTableEntryTracking;
		case PXInt32MakeEndianBig('v', 'h', 'e', 'a'): return PXTTFTableEntryVerticalHeader;
		case PXInt32MakeEndianBig('v', 'm', 't', 'x'): return PXTTFTableEntryVerticalMetrics;
		case PXInt32MakeEndianBig('V', 'D', 'M', 'X'): return PXTTFTableEntryVerticalDeviceMetrics;
		case PXInt32MakeEndianBig('x', 'r', 'e', 'f'): return PXTTFTableEntryCrossReference;
		case PXInt32MakeEndianBig('Z', 'a', 'p', 'f'): return PXTTFTableEntryGlyphReference;
		default: return PXTTFTableEntryUnkown;
	}
}

void PXTTFConstruct(PXTTF* const ttf)
{
	PXMemoryClear(ttf, sizeof(PXTTF));
}

void PXTTFDestruct(PXTTF* const ttf)
{
	// TODO
}

PXActionResult PXTTFLoadFromFile(PXFont* const pxFont, PXFile* const pxFile)
{
	PXTTF* ttf = PXNull;

	PXTTFOffsetTable offsetTable;

	PXTTFConstruct(ttf);

	// Ferch header
	{
		const PXInt16U* const dataList[] =
		{
			&offsetTable.Version.Major,
			&offsetTable.Version.Minor,
			&offsetTable.NumberOfTables,
			&offsetTable.SearchRange,
			&offsetTable.EntrySelctor,
			&offsetTable.RangeShift
		};
		const PXInt8U dataListSize = sizeof(dataList) / sizeof(PXInt16U*);

		PXFileReadI16UVE(pxFile, dataList, dataListSize, PXEndianBig);
	}

	for (PXSize i = 0; i < offsetTable.NumberOfTables; ++i)
	{
		PXTTFTableEntry tableEntry;
		PXSize sourcePosition = 0;

		// Parse entry
		{
			const PXInt32U* const dataList[] =
			{
				&tableEntry.TypeID,
				&tableEntry.CheckSum,
				&tableEntry.Offset,
				&tableEntry.Length
			};
			const PXInt8U dataListSize = sizeof(dataList) / sizeof(PXInt32U*);

			PXFileReadI32UVE(pxFile, dataList, dataListSize, PXEndianBig);

			tableEntry.Type = PXTTFTableEntryTypeFromID(tableEntry.TypeID);
		}

		printf
		(
			"[TTF] Chunk:[%c%c%c%c], Known:%c, Offset:%6i, Length:%6i\n",
			tableEntry.TypeRaw[3],
			tableEntry.TypeRaw[2],
			tableEntry.TypeRaw[1],
			tableEntry.TypeRaw[0],
			tableEntry.Type == PXTTFTableEntryUnkown ? '-' : 'x',
			tableEntry.Offset,
			tableEntry.Length
		);

		sourcePosition = pxFile->DataCursor;
		PXFileCursorMoveTo(pxFile, tableEntry.Offset);

		switch (tableEntry.Type)
		{
			//---<Essential>---------------------------------------------------		
			case PXTTFTableEntryFontHeader:
			{
#if 0
				PXFileReadI16U(pxFile, &ttf->Header.Version.Major, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Header.Version.Minor, PXEndianLittle);
				PXFileReadI16U(pxFile, &ttf->Header.Revision.Major, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Header.Revision.Minor, PXEndianLittle);
				PXFileReadI32U(pxFile, &ttf->Header.CheckSumAdjustment, PXEndianBig);
				PXFileReadI32U(pxFile, &ttf->Header.MagicNumber, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Header.Flags, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Header.UnitsPerEM, PXEndianBig);
				PXFileReadI64U(pxFile, &ttf->Header.Created, PXEndianBig);
				PXFileReadI64U(pxFile, &ttf->Header.Modified, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.Minimum[0], PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.Minimum[1], PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.Maximum[0], PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.Maximum[1], PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Header.MacStyle, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Header.LowestRecPpem, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.FontDirectionHint, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.IndexToLocFormat, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Header.GlyphDataFormat, PXEndianBig);
#endif
				break;
			}
			case PXTTFTableEntryHorizontalHeader:
			{
#if 0
				PXFileReadI16U(pxFile, &ttf->HorizontalHeader.Version.Major, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->HorizontalHeader.Version.Minor, PXEndianLittle);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.Ascender, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.Descender, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.LineGap, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->HorizontalHeader.AdvanceWidthMaximum, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.MinimumLeftSideBearing, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.MinimumRightSideBearing, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.MaximalExtendX, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.CaretSlopeRun, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.CaretSlopeRise, PXEndianBig);
				PXFileReadB(pxFile, &ttf->HorizontalHeader.Reserved, 10u);
				PXFileReadI16S(pxFile, &ttf->HorizontalHeader.MetricDataFormat, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->HorizontalHeader.NumberOfHorizontalMetrics, PXEndianBig);
#endif
				break;
			}
			case PXTTFTableEntryMaximumProfile:
			{
#if 0
				PXFileReadI16U(pxFile, &ttf->MaximumProfile.Version.Major, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->MaximumProfile.Version.Minor, PXEndianLittle);

				const unsigned char trustedTypeFonts = ttf->MaximumProfile.Version.Major == 1 && ttf->MaximumProfile.Version.Minor == 0;
				const unsigned char openTypeFonts = ttf->MaximumProfile.Version.Major == 0 && ttf->MaximumProfile.Version.Minor == 5;
				const unsigned char validVersion = trustedTypeFonts || openTypeFonts;

				PXFileReadI16U(pxFile, &ttf->MaximumProfile.NumberOfGlyphs, PXEndianBig);

				//assert(validVersion);

				if (trustedTypeFonts && !openTypeFonts)
				{
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.PointsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.ContoursMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.ComponentPointsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.ComponentContoursMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.ZonesMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.TwilightPointsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.StorageMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.FunctionDefsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.InstructionDefsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.StackElementsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.SizeOfInstructionsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.ComponentElementsMaximal, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->MaximumProfile.ComponentDepthMaximal, PXEndianBig);
				}
#endif

				break;
			}
			case PXTTFTableEntryCompatibility:
			{
#if 0
				PXFileReadI16U(pxFile, &ttf->Compatibility.Version, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.xAvgCharWidth, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.usWeightClass, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.usWidthClass, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.fsType, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySubscriptXSize, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySubscriptYSize, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySubscriptXOffset, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySubscriptYOffset, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySuperscriptXSize, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySuperscriptYSize, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySuperscriptXOffset, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.ySuperscriptYOffset, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.yStrikeoutPosition, PXEndianBig);
				PXFileReadI16S(pxFile, &ttf->Compatibility.sFamilyClass, PXEndianBig);

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

					PXFileReadI8U(pxFile, &bFamilyType);
					PXFileReadI8U(pxFile, &bSerifStyle);
					PXFileReadI8U(pxFile, &bWeight);
					PXFileReadI8U(pxFile, &bProportion);
					PXFileReadI8U(pxFile, &bContrast);
					PXFileReadI8U(pxFile, &bStrokeVariation);
					PXFileReadI8U(pxFile, &bArmStyle);
					PXFileReadI8U(pxFile, &bLetterform);
					PXFileReadI8U(pxFile, &bMidline);
					PXFileReadI8U(pxFile, &bXHeight);

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

				PXFileReadI16U(pxFile, &ttf->Compatibility.ulUnicodeRange[0], PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.ulUnicodeRange[1], PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.ulUnicodeRange[2], PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.ulUnicodeRange[3], PXEndianBig);

				PXFileReadB(pxFile, &ttf->Compatibility.achVendID, 4u);

				PXFileReadI16U(pxFile, &ttf->Compatibility.fsSelection, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.fsFirstCharIndex, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Compatibility.fsLastCharIndex, PXEndianBig);

				if (ttf->Compatibility.Version > 0)
				{
					PXFileReadI16U(pxFile, &ttf->Compatibility.sTypoAscender, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.sTypoDescender, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.sTypoLineGap, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usWinAscent, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usWinDescent, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.ulCodePageRange1, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.ulCodePageRange2, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.sxHeight, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.sCapHeight, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usDefaultChar, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usBreakChar, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usMaxContext, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usLowerPointSize, PXEndianBig);
					PXFileReadI16U(pxFile, &ttf->Compatibility.usUpperPointSize, PXEndianBig);
				}
#endif 
				break;
			}

			case PXTTFTableEntryControlValue: // cvt
			{
				// FWORD[ n ] 	List of n values referenceable by instructions. n is the number of FWORD items that fit in the size of the table.
				break;
			}
			case PXTTFTableEntryGlyphNameAndPostScriptCompatibility: // post
			{
#if 0
				PXFileReadI16U(pxFile, &ttf->PostScript.Version.Major, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->PostScript.Version.Minor, PXEndianLittle);
#endif

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
				}*/

				break;
			}
			case PXTTFTableEntryKerning: // kern
			{
#if 0
				PXFileReadI16U(pxFile, &ttf->Kerning.Version, PXEndianBig);
				PXFileReadI16U(pxFile, &ttf->Kerning.NumberOfSubtables, PXEndianBig);
#endif

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
				}*/

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
				PXTableEntryGlyphOutlineEntry tableEntryGlyphOutlineEntry;

				PXMemoryClear(&tableEntryGlyphOutlineEntry, sizeof(PXTableEntryGlyphOutlineEntry));

				PXFileReadI16SE(pxFile, &tableEntryGlyphOutlineEntry.ContourListSize, PXEndianBig);
				PXFileReadI16UE(pxFile, &tableEntryGlyphOutlineEntry.Minimum[0], PXEndianBig);
				PXFileReadI16UE(pxFile, &tableEntryGlyphOutlineEntry.Minimum[1], PXEndianBig);
				PXFileReadI16UE(pxFile, &tableEntryGlyphOutlineEntry.Maximum[0], PXEndianBig);
				PXFileReadI16UE(pxFile, &tableEntryGlyphOutlineEntry.Maximum[1], PXEndianBig);

				const PXBool isSimpleGlyph = tableEntryGlyphOutlineEntry.ContourListSize >= 0;

				if (isSimpleGlyph)
				{
					PXFileReadI16UVE(pxFile, tableEntryGlyphOutlineEntry.ContourList, tableEntryGlyphOutlineEntry.ContourListSize, PXEndianBig);

					PXFileReadI16SE(pxFile, &tableEntryGlyphOutlineEntry.InstructionListSize, PXEndianBig);

					PXFileReadB(pxFile, &tableEntryGlyphOutlineEntry.InstructionList, PXEndianBig);


					PXFileReadI8U(pxFile, &tableEntryGlyphOutlineEntry.FlagList);

					// xCoordinates
					//PXFileReadB(pxFile, &tableEntryGlyphOutlineEntry.InstructionList, PXEndianBig);

					// yCoordinates
					//PXFileReadB(pxFile, &tableEntryGlyphOutlineEntry.InstructionList, PXEndianBig);
				}
				else // compound glyph
				{
					PXFileReadI16SE(pxFile, &tableEntryGlyphOutlineEntry.ComponentFlagList, PXEndianBig);
					PXFileReadI16SE(pxFile, &tableEntryGlyphOutlineEntry.GlyphIndex, PXEndianBig);
				}


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
				PXFileReadI16U(pxFile, &ttf->CharacterMapping.Version, PXEndianBig); // Expect 0
				PXFileReadI16U(pxFile, &ttf->CharacterMapping.NumberOfTables, PXEndianBig);

				ttf->CharacterMapping.EncodingRecordListSize = sizeof(PXEncodingRecord) * ttf->CharacterMapping.NumberOfTables;
				ttf->CharacterMapping.EncodingRecordList = PXMemoryAllocateTypeCleared(PXEncodingRecord, ttf->CharacterMapping.NumberOfTables);

				for (PXSize i = 0; i < ttf->CharacterMapping.NumberOfTables; i++)
				{
					PXEncodingRecord* const encodingRecord = &ttf->CharacterMapping.EncodingRecordList[i];

					PXInt16U platformID = 0;
					PXInt16U encodingID = 0;

					PXFileReadI16U(pxFile, &platformID, PXEndianBig);
					PXFileReadI16U(pxFile, &encodingID, PXEndianBig);
					PXFileReadI32U(pxFile, &encodingRecord->SubtableOffset, PXEndianBig);

					encodingRecord->Platform = PXTTFPlatformFromID(platformID);
					encodingRecord->Encoding = PXTTFEncodingFromID(encodingRecord->Platform, encodingID);

					// Proceed?
				}

				break;
			}
			case PXTTFTableEntryLinearThreshold:
			{
				/*
				PXFileReadI16U(&PXFile, &ttf->LinearThreshold.Version, PXEndianBig);
				PXFileReadI16U(&PXFile, &ttf->LinearThreshold.NumberOfGlyphs, PXEndianBig);

				LinearThreshold.PelsHeightList = Memory::Allocate<Byte__>(LinearThreshold.NumberOfGlyphs);

				PXFileReadI16U(&PXFile, &ttf->LinearThreshold.PelsHeightList, LinearThreshold.NumberOfGlyphs);*/

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
				}*/

				break;
			}
		}

		PXFileCursorMoveTo(pxFile, sourcePosition);
	}

	return PXActionSuccessful;
}

PXActionResult PXTTFSaveToFile(PXFont* const pxFont, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}