#include "PXLAME.h"

#include <PX/Media/PXText.h>
#include <PX/OS/File/PXFile.h>

const char PXLameSignatureID[4] = "LAME";

PXResult PXAPI  PXLAMELoadFromFile(PXLAME* const lame, PXFile* const pxFile)
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
            {&lame->Revision,PXTypeInt08U},
            {&lame->VBRType,PXTypeInt08U},
            {&lame->LowpassFrequency,PXTypeInt08U},
            {&lame->Peak_signal, PXTypeInt32UBE},
            {&lame->Radio_replay_pad, PXTypeInt16UBE },
            {&lame->Radio_replay_set_name, PXTypeInt16UBE},
            {&lame->Radio_replay_originator_code, PXTypeInt16UBE},
            {&lame->Radio_replay_gain, PXTypeInt16UBE},
            {&lame->Audiophile_replay_gain, PXTypeInt16UBE},
            {&lame->Flag_ath_type,PXTypeInt08U},
            {&lame->Flag_expn_psy_tune,PXTypeInt08U},
            {&lame->Flag_safe_joint,PXTypeInt08U},
            {&lame->Flag_no_gap_more,PXTypeInt08U},
            {&lame->Flag_no_gap_previous,PXTypeInt08U},
            {&lame->Average_bit_rate,PXTypeInt08U},
            {&lame->Delay_padding_delay_high,PXTypeInt08U},
            {&lame->Delay_padding_delay_low,PXTypeInt08U},
            {&lame->Delay_padding_padding_high,PXTypeInt08U},
            {&lame->Delay_padding_padding_low,PXTypeInt08U},
            {&lame->noise_shaping,PXTypeInt08U},
            {&lame->stereo_mode,PXTypeInt08U},
            {&lame->non_optimal,PXTypeInt08U}
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
            {&lame->Unused,PXTypeInt08U},
            {&lame->Preset,PXTypeInt16UBE},
            {&lame->MusicLength,PXTypeInt32UBE}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXLAMESaveToFile(const PXLAME* const lame, PXFile* const pxFile)
{
    PXFileWriteB(pxFile, PXLameSignatureID, sizeof(PXLameSignatureID));

    return PXActionRefusedNotImplemented;
}
