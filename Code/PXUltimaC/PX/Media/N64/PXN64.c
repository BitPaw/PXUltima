#include "PXN64.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Media/CRC32/PXCRC32.h>
#include <PX/Media/MIPS/PXMIPS.h>
#include <PX/OS/PXOS.h>

PXN64CountryCode PXAPI PXN64CountryCodeFromID(PXI8U pxN64CountryCodeID)
{
    switch(pxN64CountryCodeID)
    {
        case '7': return PXN64CountryCodeBeta;
        case 'A': return PXN64CountryCodeAsianNTSC;
        case 'B': return PXN64CountryCodeBrazilian;
        case 'C': return PXN64CountryCodeChinese;
        case 'D': return PXN64CountryCodeGerman;
        case 'E': return PXN64CountryCodeNorthAmerica;
        case 'F': return PXN64CountryCodeFrench;
        case 'G': return PXN64CountryCodeGateway64NTSC;
        case 'H': return PXN64CountryCodeDutch;
        case 'I': return PXN64CountryCodeItalian;
        case 'J': return PXN64CountryCodeJapanese;
        case 'K': return PXN64CountryCodeKorean;
        case 'L': return PXN64CountryCodeGateway64PAL;
        case 'N': return PXN64CountryCodeCanadian;
        case 'P': return PXN64CountryCodeEuropeanBasicSpec;
        case 'S': return PXN64CountryCodeSpanish;
        case 'U': return PXN64CountryCodeAustralian;
        case 'W': return PXN64CountryCodeScandinavian;
        case 'X': return PXN64CountryCodeEuropean;
        case 'Y': return PXN64CountryCodeEuropean;

        default:
            return PXN64CountryCodeInvalid;
    }
}

const char* PXN64CountryCodeToString(const PXN64CountryCode pxN64CountryCode)
{
    switch(pxN64CountryCode)
    {
        case PXN64CountryCodeBeta: return "Beta";
        case PXN64CountryCodeAsianNTSC: return "AsianNTSC";
        case PXN64CountryCodeBrazilian: return "Brazilian";
        case PXN64CountryCodeChinese: return "Chinese";
        case PXN64CountryCodeGerman: return "German";
        case PXN64CountryCodeNorthAmerica: return "NorthAmerica";
        case PXN64CountryCodeFrench: return "French";
        case PXN64CountryCodeGateway64NTSC: return "Gateway64NTSC";
        case PXN64CountryCodeDutch: return "Dutch";
        case PXN64CountryCodeItalian: return "Italian";
        case PXN64CountryCodeJapanese: return "Japanese";
        case PXN64CountryCodeKorean: return "Korean";
        case PXN64CountryCodeGateway64PAL: return "Gateway64PAL";
        case PXN64CountryCodeCanadian: return "Canadian";
        case PXN64CountryCodeEuropeanBasicSpec: return "EuropeanBasicSpec";
        case PXN64CountryCodeSpanish:return "Spanish";
        case PXN64CountryCodeAustralian: return "Australian";
        case PXN64CountryCodeScandinavian: return "Scandinavian";
        case PXN64CountryCodeEuropean: return "European";

        default:
            return PXNull;
    }
}

PXResult PXAPI PXN64LoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXFile* pxN64Data = PXFileCreate();

    PXN64 n64;
    PXClear(PXN64, &n64);

    // Because some formats are byte swapped (middle endian) we need to check for that
    // We will expect a "0x80 37 12 40", if it is not, we need to swap
    {
        const char bufferExpectedByteSwapped[4] = { 0x37, 0x80,0x40 , 0x12 };
        const char bufferExpectedEndianLittle[4] = { 0x40, 0x12,0x37,0x80 };
        const char bufferExpectedEndianBig[4] = {0x80, 0x37, 0x12, 0x40};
        char buffer[4];

        PXFileReadB(pxResourceLoadInfo->FileCurrent, buffer, 4u);

        if(PXTextCompareA(bufferExpectedByteSwapped, 4, buffer, 4, 0))
        {
            n64.Endian = PXEndianMiddle;
        }
        else if(PXTextCompareA(bufferExpectedEndianLittle, 4, buffer, 4, 0))
        {
            n64.Endian = PXEndianLittle;
        }
        else if(PXTextCompareA(bufferExpectedEndianBig, 4, buffer, 4, 0))
        {
            n64.Endian = PXEndianBig;
        }
        else
        {
            return PXActionRefusedInvalidHeaderSignature;
        }    
    }


    // if byteswapped, so so
    {
        if(PXEndianMiddle == n64.Endian)
        {
            PXFileCursorToBeginning(pxResourceLoadInfo->FileCurrent);

            PXFileOpenInfo pxFileOpenInfo;
            PXClear(PXFileOpenInfo, &pxFileOpenInfo);
            pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
            pxFileOpenInfo.FilePath.SizeUsed = PXFileAllocatedSize(pxResourceLoadInfo->FileCurrent);

            PXFileOpen(pxN64Data, &pxFileOpenInfo);
            PXFileByteSwap(pxN64Data, pxResourceLoadInfo->FileCurrent);

            pxFile = pxN64Data;

            PXFileEndianessSet(pxFile, PXEndianBig);
        }
    }

    // Read header
    {
        // 0x0000000-ram:b07fffff 

        PXTypeEntry pxFileDataElementType[] =
        {
            {&n64.EndiannessID, PXTypeNibble}, // 0x00
            {PXNull, PXTypeNibble}, // 0x00
            {&n64.initialPI_BSB_DOM1_LAT_REG, PXTypeNibble}, // 0x01
            {&n64.initialPI_BSD_DOM1_PGS_REG, PXTypeNibble}, // 0x01
            {&n64.initialPI_BSD_DOM1_PWD_REG, PXTypeI08U}, // 0x02
            {&n64.initialPI_BSB_DOM1_PGS_REG, PXTypeI08U}, // 0x03
            {&n64.ClockRateOverride, PXTypeI32UBE}, // 0x04
            {&n64.RAMEntryPointOffset, PXTypeI32ULE}, // 0x08
            {&n64.ReleaseAddress, PXTypeI32UBE}, // 0x0C
            {&n64.CRC1Checksum, PXTypeI32UBE}, // 0x10
            {&n64.CRC2, PXTypeI32UBE}, // 0x14
            {&n64.UnknownA, PXTypeI64U}, // 0x18
            {n64.ImageName, PXTypeText(20)}, // 0x20
            {&n64.UnknownB, PXTypeI32UBE}, // 0x34
            {n64.MediaFormatID, PXTypeDatax4}, // 0x38
            {n64.CartridgeID, PXTypeDatax2}, // 0x3C
            {&n64.CountryCodeID, PXTypeI08U}, // 0x3E
            {&n64.Version, PXTypeI08U} // 0x3F
        };
        const PXSize pxFileDataElementTypeSize = sizeof(pxFileDataElementType);

        const PXSize readBytes = PXFileReadMultible(pxFile, pxFileDataElementType, pxFileDataElementTypeSize);
        const PXBool readByteEnough = 64 == PXFileDataPosition(pxFile);

        if(!readByteEnough)
        {
            return PXResultInvalid;
        }
        void* aaw = n64.RAMEntryPointOffset; // Expect 80 00 04 00

       // n64.RAMEntryPointAdress = (char*)pxFile->Buffer.Data + 0x1000;
       // n64.RAMEntryPointLength = pxFile->Buffer.SizeAllowedToUse - 0x1000;

        // The lower most nybble of the ClockRate is not read.
        n64.ClockRateOverride &= 0xFFFFFFF0;

        n64.CountryCode = PXN64CountryCodeFromID(n64.CountryCodeID);

        // 4096 - 1052672
        //  0x1000 to 0x101000

        n64.BootCode = PXFileDataAtCursor(pxFile); // 0x40
        n64.BootCodeSize = 4032; // 0xFC0

      //  void* a = (char*)pxFile->Buffer.Data + 4096;
        PXSize x = 1052672;

       // n64.BootCodeCRC = PXCRC32Generate(a, x);
        //         n64.BootCodeCRC = PXCRC32Generate(n64.BootCode, n64.BootCodeSize);
        

        switch(n64.BootCodeCRC)
        {
            case 0x0B050EE0: // 184880864
            {
                
                break;
            }
            case 0xACC8580A: // 2898810890
            {
            
                break;
            }
            default:
            {
                // ??? CRC invalid?
                break;
            }
        }    


        switch(n64.CICType)
        {
            case PXN64CICType6101:
            case PXN64CICType6102:
            case PXN64CICType6105:
            {
                // Do nothing
                break;
            }
            case PXN64CICType6103:
            {
                //n64.RAMEntryPointOffset -= 0x100000; // 1048576
                break;
            }
            case PXN64CICType6106:
            {
                //n64.RAMEntryPointOffset -= 0x200000; // 2097152
                break;
            }
            default:
                break;
        }


        // Special behaviour for the string. its not null terminated but with spaces?
        for(size_t i = 19; n64.ImageName[i] == ' '; --i)
        {
            n64.ImageName[i] = '\0';
        }


        // Caluclate CRC value from bootloader
    





        /*
        
                             // .ram 
                             // RAM content
                             // ram:80000400-ram:807ff3ff
         */



        PXCodeSegment pxCodeSegmentList[] =
        {
            {0x00000000, 0x00000040, PXNull, PXNull, "N64-Header",""},       
            {0x00000000, 0x03EFFFFF, PXNull, PXNull, ".rdram",    "RDRAM Memory"},
            {0xA3F00000, 0xA3F00027, PXNull, PXNull, ".rdreg",    "RDRAM Registers"},
            {0xA4000040, 0xA4001040, n64.BootCode, n64.BootCodeSize, ".boot",    "ROM bootloader"}, // 0x00000040, 0x00001000
            {0xa4040000, 0xa404001f, PXNull, PXNull, ".spreg",    "SP Registers"},
            {0xa4080000, 0xa4080003, PXNull, PXNull, ".spcreg",    "SP_PC_Reg"},
            {0xA4100000, 0xA410001F, PXNull, PXNull, ".dpcreg",    "DP Command Registers"},
            {0xA4200000, 0xa420000F, PXNull, PXNull, ".dpsreg",    "DP Span Registers"},
            {0xa4300000, 0xa430000F, PXNull, PXNull, ".mireg",    "MIPS Interface (MI) Registers"},
            {0xa4400000, 0xa4400037, PXNull, PXNull, ".vireg",    "Video Interface (VI) Registers"},
            {0xa4500000, 0xa4500017, PXNull, PXNull, ".aireg",    "Audio Interface (AI) Registers"},
            {0xa4600000, 0xa4600034, PXNull, PXNull, ".pireg",    "Peripheral Interface (PI) Registers"},
            {0xa4700000, 0xa470001F, PXNull, PXNull, ".rireg",    "RDRAM Interface (RI) Registers"},
            {0xa4800000, 0xa480001b, PXNull, PXNull, ".sireg",    "Serial Interface (SI) Registers"},
            {0xa5000500, 0xa500054b, PXNull, PXNull, ".ddreg",    "N64 Disk Drive (DD) Registers"},
            {0x1FC00000, 0x1FC007BF, PXNull, PXNull, ".pifrom",    "PIF Boot ROM"},
            {0x1FC007C0, 0x1FC007FF, PXNull, PXNull, ".pifram",    "PIF RAM"},
            {0x80000000, 0x800003FF, PXNull, PXNull, ".ivt",    "Interrupt Vector Table"},

           // {0xB0000000,  0x0, pxFile->Buffer.CursorOffsetByte,      pxFile->Buffer.SizeAllowedToUse,         ".rom", "ROM image", 111}, // Read only
            
           // {0x80000000 + n64.RAMEntryPointOffset, 0x0, n64.RAMEntryPointAdress, n64.RAMEntryPointLength, ".ram", "RAM content", 111}, // RWX
        };







#if PXLogEnable
        PXText sizeText;
        PXTextConstructBufferA(&sizeText, 64);
        //PXTextFormatSize(&sizeText, pxN64Data.DataUsed);

        const char* countryCodeName = PXN64CountryCodeToString(n64.CountryCode);

        PXLogPrint
        (
            PXLoggingInfo,
            "N64",
            "Load",
            "Data\n"
            "%10s : %s\n"
            "%10s : %s\n"
            "%10s : 0x%8.8x\n"
            "%10s : %s",
            "Name", n64.ImageName,
            "Size", sizeText.A,
            "EntyPoint", (0x80000000 + n64.RAMEntryPointOffset),
            "Country", countryCodeName
        );
#endif



        // 0x00000000, 0x03EFFFFF, "RDRAM Memory", ".rdram")

        PXMIPSProcessor pxMIPSProcessor;
        PXClear(PXMIPSProcessor, &pxMIPSProcessor);
        pxMIPSProcessor.ROMOffsetVirtual = (void*)((PXSize)0x80000000 + (PXSize)0x400);
        pxMIPSProcessor.RAMAdressVirtual = 0x00000000;
        pxMIPSProcessor.RAMSize = 0x03EFFFFF;
        pxMIPSProcessor.RAMAdress = PXMemoryVirtualAllocate(pxMIPSProcessor.RAMSize, PXNull, PXAccessModeReadAndWrite);

        pxMIPSProcessor.CoProcessor[0].Enabled = PXTrue;
        pxMIPSProcessor.CoProcessor[1].Enabled = PXTrue;

        PXMIPSTranslate(&pxMIPSProcessor, n64.RAMEntryPointAdress, n64.RAMEntryPointLength);




        n64.ImageName[19] = '\0';
    }




    return PXResultOK;
}

PXResult PXAPI PXN64SaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}