#include "LAME.h"

#include <File/DataStream.h>
#include <Text/Text.h>

ActionResult LAMEParse(LAME* lame, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;

	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);

	// Check signature (4 Bytes)
	{
		const char signature[] = { 'L', 'A', 'M', 'E' };
		const size_t signatueSize = sizeof(signature);
		const unsigned char isValid = DataStreamReadAndCompare(&dataStream, signature, signatueSize);

		if(!isValid)
		{
			return 0;
		}
	}

	// Fetch version (5 Bytes)
	{
		char majorVersionText;
		char minorVersionText[2];
		char releaseVersionText = 0;

		DataStreamReadC(&dataStream, &majorVersionText); // 1 character
		const char isDot = DataStreamReadAndCompare(&dataStream, '.', sizeof(unsigned char));
		DataStreamReadP(&dataStream, minorVersionText, 2u); // 2 character
		DataStreamReadC(&dataStream, releaseVersionText); // letter

		int a = 0;
		int b = 0;

		TextToIntA(&majorVersionText, 1u, &a);
		TextToIntA(minorVersionText, 2u, &b);

		lame->MajorVersion = a;
		lame->MinorVersion = b;
		lame->ReleaseVersion = releaseVersionText;
	}

	DataStreamReadCU(&dataStream, &lame->Revision);
	DataStreamReadCU(&dataStream, &lame->VBRType);
	DataStreamReadCU(&dataStream, &lame->LowpassFrequency);
	DataStreamReadIU(&dataStream, &lame->Peak_signal, EndianBig);
	DataStreamReadSU(&dataStream, &lame->Radio_replay_pad, EndianBig);
	DataStreamReadSU(&dataStream, &lame->Radio_replay_set_name, EndianBig);
	DataStreamReadSU(&dataStream, &lame->Radio_replay_originator_code, EndianBig);
	DataStreamReadSU(&dataStream, &lame->Radio_replay_gain, EndianBig);
	DataStreamReadSU(&dataStream, &lame->Audiophile_replay_gain, EndianBig);
	DataStreamReadCU(&dataStream, &lame->Flag_ath_type);
	DataStreamReadCU(&dataStream, &lame->Flag_expn_psy_tune);
	DataStreamReadCU(&dataStream, &lame->Flag_safe_joint);
	DataStreamReadCU(&dataStream, &lame->Flag_no_gap_more);
	DataStreamReadCU(&dataStream, &lame->Flag_no_gap_previous);
	DataStreamReadCU(&dataStream, &lame->Average_bit_rate);
	DataStreamReadCU(&dataStream, &lame->Delay_padding_delay_high);
	DataStreamReadCU(&dataStream, &lame->Delay_padding_delay_low);
	DataStreamReadCU(&dataStream, &lame->Delay_padding_padding_high);
	DataStreamReadCU(&dataStream, &lame->Delay_padding_padding_low);
	DataStreamReadCU(&dataStream, &lame->noise_shaping);
	DataStreamReadCU(&dataStream, &lame->stereo_mode);
	DataStreamReadCU(&dataStream, &lame->non_optimal);

	// Parse: source frequency
	{
		unsigned char sourcefrequencyID = 0;

		DataStreamReadCU(&dataStream, &sourcefrequencyID);

		switch(sourcefrequencyID)
		{
			case 0b00:
				lame->SourceFrequency = LAMESourceFrequencykHz32OrLess;
				break;

			case 0b01:
				lame->SourceFrequency = LAMESourceFrequencykHz44x1;
				break;

			case 0b10:
				lame->SourceFrequency = LAMESourceFrequencykHz48;
				break;

			case 0b11:
				lame->SourceFrequency = LAMESourceFrequencykHz48Over;
				break;

			default:
				lame->SourceFrequency = LAMESourceFrequencyInvalid;
				break;
		}
	}

	DataStreamReadCU(&dataStream, &lame->Unused);
	DataStreamReadSU(&dataStream, &lame->Preset, EndianBig);
	DataStreamReadIU(&dataStream, &lame->MusicLength, EndianBig);

	*dataRead = dataStream.DataCursor;

    return ActionSuccessful;
}
