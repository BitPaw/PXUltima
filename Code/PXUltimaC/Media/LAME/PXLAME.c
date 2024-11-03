#include "PXLAME.h"

#include <Media/PXText.h>
#include <OS/File/PXFile.h>

const char PXLameSignatureID[] = { 'L', 'A', 'M', 'E' };

PXActionResult PXAPI PXLAMELoadFromFile(PXLAME* const lame, PXFile* const pxFile)
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
        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {&lame->Revision,PXDataTypeInt08U},
            {&lame->VBRType,PXDataTypeInt08U},
            {&lame->LowpassFrequency,PXDataTypeInt08U},
            {&lame->Peak_signal, PXDataTypeInt32UBE},
            {&lame->Radio_replay_pad, PXDataTypeInt16UBE },
            {&lame->Radio_replay_set_name, PXDataTypeInt16UBE},
            {&lame->Radio_replay_originator_code, PXDataTypeInt16UBE},
            {&lame->Radio_replay_gain, PXDataTypeInt16UBE},
            {&lame->Audiophile_replay_gain, PXDataTypeInt16UBE},
            {&lame->Flag_ath_type,PXDataTypeInt08U},
            {&lame->Flag_expn_psy_tune,PXDataTypeInt08U},
            {&lame->Flag_safe_joint,PXDataTypeInt08U},
            {&lame->Flag_no_gap_more,PXDataTypeInt08U},
            {&lame->Flag_no_gap_previous,PXDataTypeInt08U},
            {&lame->Average_bit_rate,PXDataTypeInt08U},
            {&lame->Delay_padding_delay_high,PXDataTypeInt08U},
            {&lame->Delay_padding_delay_low,PXDataTypeInt08U},
            {&lame->Delay_padding_padding_high,PXDataTypeInt08U},
            {&lame->Delay_padding_padding_low,PXDataTypeInt08U},
            {&lame->noise_shaping,PXDataTypeInt08U},
            {&lame->stereo_mode,PXDataTypeInt08U},
            {&lame->non_optimal,PXDataTypeInt08U}
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
        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {&lame->Unused,PXDataTypeInt08U},
            {&lame->Preset,PXDataTypeInt16UBE},
            {&lame->MusicLength,PXDataTypeInt32UBE}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXLAMESaveToFile(const PXLAME* const lame, PXFile* const pxFile)
{
    PXFileWriteB(pxFile, PXLameSignatureID, sizeof(PXLameSignatureID));

    return PXActionRefusedNotImplemented;
}
