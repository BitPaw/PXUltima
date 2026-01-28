#include "PXLAME.h"

#include <PX/Media/PXText.h>
#include <PX/OS/File/PXFile.h>

const char PXLameSignatureID[4] = "LAME";

PXResult PXAPI PXLAMELoadFromFile(PXLAME PXREF lame, PXFile PXREF pxFile)
{
    // Check signature (4 Bytes)
    {
        const PXSize signatueSize = sizeof(PXLameSignatureID);
        const PXBool isValid = PXFileReadAndCompare(pxFile, PXLameSignatureID, sizeof(PXLameSignatureID));

        if(!isValid)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    // Fetch version (5 Bytes)
    {
        PXFileReadTextIU8(pxFile, &lame->MajorVersion); // 1 character
        const PXBool isDot = PXFileReadAndCompare(pxFile, '.', sizeof(unsigned char));
        PXFileReadTextIU8(pxFile, &lame->MinorVersion); // 2 character
        PXFileReadI8U(pxFile, &lame->ReleaseVersion); // letter
    }

    {
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&lame->Revision,PXTypeI08U},
            {&lame->VBRType,PXTypeI08U},
            {&lame->LowpassFrequency,PXTypeI08U},
            {&lame->Peak_signal, PXTypeI32UBE},
            {&lame->Radio_replay_pad, PXTypeI16UBE },
            {&lame->Radio_replay_set_name, PXTypeI16UBE},
            {&lame->Radio_replay_originator_code, PXTypeI16UBE},
            {&lame->Radio_replay_gain, PXTypeI16UBE},
            {&lame->Audiophile_replay_gain, PXTypeI16UBE},
            {&lame->Flag_ath_type,PXTypeI08U},
            {&lame->Flag_expn_psy_tune,PXTypeI08U},
            {&lame->Flag_safe_joint,PXTypeI08U},
            {&lame->Flag_no_gap_more,PXTypeI08U},
            {&lame->Flag_no_gap_previous,PXTypeI08U},
            {&lame->Average_bit_rate,PXTypeI08U},
            {&lame->Delay_padding_delay_high,PXTypeI08U},
            {&lame->Delay_padding_delay_low,PXTypeI08U},
            {&lame->Delay_padding_padding_high,PXTypeI08U},
            {&lame->Delay_padding_padding_low,PXTypeI08U},
            {&lame->noise_shaping,PXTypeI08U},
            {&lame->stereo_mode,PXTypeI08U},
            {&lame->non_optimal,PXTypeI08U}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    // Parse: source frequency
    {
        unsigned char sourcefrequencyID = 0;

        PXFileReadI8U(pxFile, &sourcefrequencyID);

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
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&lame->Unused,PXTypeI08U},
            {&lame->Preset,PXTypeI16UBE},
            {&lame->MusicLength,PXTypeI32UBE}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    return PXResultOK;
}

PXResult PXAPI PXLAMESaveToFile(const PXLAME PXREF lame, PXFile PXREF pxFile)
{
    PXFileWriteB(pxFile, PXLameSignatureID, sizeof(PXLameSignatureID));

    return PXActionRefusedNotImplemented;
}
