#include "PXLAME.h"

#include <Media/PXText.h>

PXActionResult PXLAMEParse(PXLAME* const lame, PXFile* const dataStream)
{
	// Check signature (4 Bytes)
	{
		const char signature[] = { 'L', 'A', 'M', 'E' };
		const PXSize signatueSize = sizeof(signature);
		const PXBool isValid = PXFileReadAndCompare(&dataStream, signature, signatueSize);

		if(!isValid)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}
	}

	// Fetch version (5 Bytes)
	{
		PXFileReadTextIU8(&dataStream, &lame->MajorVersion); // 1 character
		const char isDot = PXFileReadAndCompare(&dataStream, '.', sizeof(unsigned char));
		PXFileReadTextIU8(&dataStream, lame->MinorVersion); // 2 character
		PXFileReadI8U(&dataStream, &lame->ReleaseVersion); // letter
	}

	{
		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{PXDataTypeInt8U, &lame->Revision},
			{PXDataTypeInt8U, &lame->VBRType },
			{PXDataTypeInt8U, &lame->LowpassFrequency },
			{PXDataTypeBEInt32U, &lame->Peak_signal},
			{PXDataTypeBEInt16U, &lame->Radio_replay_pad },
			{PXDataTypeBEInt16U, &lame->Radio_replay_set_name},
			{PXDataTypeBEInt16U, &lame->Radio_replay_originator_code},
			{PXDataTypeBEInt16U, &lame->Radio_replay_gain},
			{PXDataTypeBEInt16U, &lame->Audiophile_replay_gain},
			{PXDataTypeInt8U, &lame->Flag_ath_type},
			{PXDataTypeInt8U, &lame->Flag_expn_psy_tune},
			{PXDataTypeInt8U, &lame->Flag_safe_joint},
			{PXDataTypeInt8U, &lame->Flag_no_gap_more},
			{PXDataTypeInt8U, &lame->Flag_no_gap_previous},
			{PXDataTypeInt8U, &lame->Average_bit_rate},
			{PXDataTypeInt8U, &lame->Delay_padding_delay_high},
			{PXDataTypeInt8U, &lame->Delay_padding_delay_low},
			{PXDataTypeInt8U, &lame->Delay_padding_padding_high},
			{PXDataTypeInt8U, &lame->Delay_padding_padding_low},
			{PXDataTypeInt8U, &lame->noise_shaping},
			{PXDataTypeInt8U, &lame->stereo_mode},
			{PXDataTypeInt8U, &lame->non_optimal}
		};
		const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

		PXFileReadMultible(dataStream, pxDataStreamElementList, pxDataStreamElementListSize);
	}

	// Parse: source frequency
	{
		unsigned char sourcefrequencyID = 0;

		PXFileReadI8U(&dataStream, &sourcefrequencyID);

		switch(sourcefrequencyID)
		{
			case 0b00:
				lame->SourceFrequency = PXLAMESourceFrequencykHz32OrLess;
				break;

			case 0b01:
				lame->SourceFrequency = PXLAMESourceFrequencykHz44x1;
				break;

			case 0b10:
				lame->SourceFrequency = PXLAMESourceFrequencykHz48;
				break;

			case 0b11:
				lame->SourceFrequency = PXLAMESourceFrequencykHz48Over;
				break;

			default:
				lame->SourceFrequency = PXLAMESourceFrequencyInvalid;
				break;
		}
	}

	{
		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{PXDataTypeInt8U, &lame->Unused},
			{PXDataTypeBEInt16U, &lame->Preset },
			{PXDataTypeBEInt32U, &lame->MusicLength}
		};
		const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

		PXFileReadMultible(dataStream, pxDataStreamElementList, pxDataStreamElementListSize);
	}

    return PXActionSuccessful;
}
