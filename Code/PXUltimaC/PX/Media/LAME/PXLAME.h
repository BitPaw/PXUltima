#ifndef PXLAMEIncluded
#define PXLAMEIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXLAMESourceFrequency_
{
    PXLAMESourceFrequencyInvalid,
    PXLAMESourceFrequencykHz32OrLess,
    PXLAMESourceFrequencykHz44x1,
    PXLAMESourceFrequencykHz48,
    PXLAMESourceFrequencykHz48Over,
}
PXLAMESourceFrequency;

typedef struct PXLAME_
{
    PXI8U MajorVersion;
    PXI8U MinorVersion;
    PXI8U ReleaseVersion;
    unsigned char Revision; // At this time these 4 bits are set to 0.
    unsigned char VBRType;
    unsigned char LowpassFrequency;
    unsigned int Peak_signal;           // 9.23 fixed point
    unsigned short Radio_replay_pad;
    unsigned short Radio_replay_set_name;
    unsigned short Radio_replay_originator_code;
    unsigned short Radio_replay_gain;
    unsigned short Audiophile_replay_gain;
    unsigned char Flag_ath_type;
    unsigned char Flag_expn_psy_tune;
    unsigned char Flag_safe_joint;
    unsigned char Flag_no_gap_more;
    unsigned char Flag_no_gap_previous;
    unsigned char Average_bit_rate;
    unsigned char Delay_padding_delay_high;
    unsigned char Delay_padding_delay_low;
    unsigned char Delay_padding_padding_high;
    unsigned char Delay_padding_padding_low;
    unsigned char noise_shaping;
    unsigned char stereo_mode;
    unsigned char non_optimal;
    PXLAMESourceFrequency SourceFrequency;
    unsigned char Unused;                // set to 0
    unsigned short Preset;
    unsigned int MusicLength;
}
PXLAME;

PXPublic PXResult PXAPI PXLAMELoadFromFile(PXLAME* const lame, PXFile* const pxFile);
PXPublic PXResult PXAPI PXLAMESaveToFile(const PXLAME* const lame, PXFile* const pxFile);

#endif
