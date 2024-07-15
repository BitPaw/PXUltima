#ifndef PXN64INCLUDE
#define PXN64INCLUDE

#include "../PXResource.h"

/*
* N64 only reads up to 0x18
* 
	0x00 - 0x0040 - Internal Header
	0x40 - 0x1000 - Boostrap


	// he first 1024 (0x400) bytes of physical memory are reserved for exception vectors and configuration parameters (the interrupt vector table).


	MIPS:BE:64:64-32addr", "o32

	0x00000000, 0x03EFFFFF, "RDRAM Memory",".rdram")
	0xA3F00000, 0xA3F00027, "RDRAM Registers",".rdreg"
	0xa4040000, 0xa404001f, "SP Registers",".spreg"
	0xa4080000, 0xa4080003, "SP_PC_Reg",".spcreg"
	0xA4100000, 0xA410001F, "DP Command Registers",".dpcreg"
	0xA4200000, 0xa420000F, "DP Span Registers",".dpsreg"
	0xa4300000, 0xa430000F, "MIPS Interface (MI) Registers",".mireg"
	0xa4400000, 0xa4400037, "Video Interface (VI) Registers",".vireg"
	0xa4500000, 0xa4500017, "Audio Interface (AI) Registers",".aireg"
	0xa4600000, 0xa4600034, "Peripheral Interface (PI) Registers",".pireg"
	0xa4700000, 0xa470001F, "RDRAM Interface (RI) Registers",".rireg"
	0xa4800000, 0xa480001b, "Serial Interface (SI) Registers",".sireg"
	0xa5000500, 0xa500054b, "N64 Disk Drive (DD) Registers",".ddreg"
	0x1FC00000, 0x1FC007BF, "PIF Boot ROM",".pifrom"
	0x1FC007C0, 0x1FC007FF, "PIF RAM",".pifram"
	0x80000000, 0x800003FF, "Interrupt Vector Table",".ivt"


				0xA3f00000L), "RDRAM_CONFIG"
				0xA3f00004L), "RDRAM_DEVICE_ID"
				0xA3f00008L), "RDRAM_DELAY"
				0xA3f0000CL), "RDRAM_MODE"
				0xA3f00010L), "RDRAM_REF_INTERVAL"
				0xA3f00014L), "RDRAM_REF_ROW"
				0xA3f00018L), "RDRAM_RAS_INTERVAL"
				0xA3f0001CL), "RDRAM_MIN_INTERVAL"
				0xA3f00020L), "RDRAM_ADDR_SELECT"
				0xA3f00024L), "RDRAM_DEVICE_MANUF"
				0xA4040000L), "SP_MEM_ADDR"
				0xA4040004L), "SP_DRAM_ADDR"
				0xA4040008L), "SP_RD_LEN"
				0xA404000CL), "SP_WR_LEN"
				0xA4040010L), "SP_STATUS"
				0xA4040014L), "SP_DMA_FULL"
				0xA4040018L), "SP_DMA_BUSY"
				0xA404001CL), "SP_SEMAPHORE"
				0xA4080000L), "SP_PC"
				0xA4100000L), "DCP_START"
				0xA4100004L), "DCP_END"
				0xA4100008L), "DCP_CURRENT"
				0xA410000cL), "DCP_STATUS"
				0xA4100010L), "DCP_CLOCK"
				0xA4100014L), "DCP_BUFBUSY"
				0xA4100018L), "DCP_PIPEBUSY"
				0xA410001cL), "DCP_START"
				0xA4300000L), "MI_INIT_MODE"
				0xA4300004L), "MI_VERSION"
				0xA4300008L), "MI_INTR"
				0xA430000CL), "MI_INTR_MASK"
				0xA4400000L), "VI_STATUS"
				0xA4400004L), "VI_ORIGIN"
				0xA4400008L), "VI_WIDTH"
				0xA440000CL), "VI_INTR"
				0xA4400010L), "VI_CURRENT"
				0xA4400014L), "VI_BURST"
				0xA4400018L), "VI_V_SYNC"
				0xA440001CL), "VI_H_SYNC"
				0xA4400020L), "VI_LEAP"
				0xA4400024L), "VI_H_START"
				0xA4400028L), "VI_V_START"
				0xA440002CL), "VI_V_BURST"
				0xA4400030L), "VI_X_SCALE"
				0xA4400034L), "VI_Y_SCALE"
				0xA4500000L), "AI_DRAM_ADDR"
				0xA4500004L), "AI_LEN"
				0xA4500008L), "AI_CONTROL"
				0xA450000CL), "AI_STATUS"
				0xA4500010L), "AI_DACRATE"
				0xA4500014L), "AI_BITRATE"
				0xA4600000L), "PI_DRAM_ADDR"
				0xA4600004L), "PI_CART_ADDR"
				0xA4600008L), "PI_RD_LEN"
				0xA460000CL), "PI_WR_LEN"
				0xA4600010L), "PI_STATUS"
				0xA4600014L), "PI_BSD_DOM1_LAT"
				0xA4600018L), "PI_BSD_DOM1_PWD"
				0xA460001CL), "PI_BSD_DOM1_PGS"
				0xA4600020L), "PI_BSD_DOM1_RLS"
				0xA4600024L), "PI_BSD_DOM2_LAT"
				0xA4600028L), "PI_BSD_DOM2_PWD"
				0xA460002CL), "PI_BSD_DOM2_PGS"
				0xA4600030L), "PI_BSD_DOM2_RLS"
				0xA4700000L), "RI_MODE"
				0xA4700004L), "RI_CONFIG"
				0xA4700008L), "RI_CURRENT_LOAD"
				0xA470000CL), "RI_SELECT"
				0xA4700010L), "RI_REFRESH"
				0xA4700014L), "RI_LATENCY"
				0xA4700018L), "RI_RERROR"
				0xA470001CL), "RI_WERROR"
				0xA4800000L), "SI_DRAM_ADDR"
				0xA4800004L), "SI_PIF_ADDR_RD64B_REG"
				0xA4800010L), "SI_PIF_ADDR_WR64B_REG"
				0xA4800018L), "SI_STATUS"
				0xA5000500L), "ASIC_DATA"
				0xA5000504L), "ASIC_MISC_REG"
				0xA5000508L), "ASIC_STATUS"
				0xA500050CL), "ASIC_CUR_TK"
				0xA5000510L), "ASIC_BM_STATUS"
				0xA5000514L), "ASIC_ERR_SECTOR"
				0xA5000518L), "ASIC_SEQ_STATUS"
				0xA500051CL), "ASIC_CUR_SECTOR"
				0xA5000520L), "ASIC_HARD_RESET"
				0xA5000524L), "ASIC_C1_SO"
				0xA5000528L), "ASIC_HOST_SECBYTE"
				0xA500052CL), "ASIC_C1_S2"
				0xA5000530L), "ASIC_SEC_BYTE"
				0xA5000534L), "ASIC_C1_S4"
				0xA5000538L), "ASIC_C1_S6"
				0xA500053CL), "ASIC_CUR_ADDR"
				0xA5000540L), "ASIC_ID_REG"
				0xA5000544L), "ASIC_TEST_REG"
				0xA5000548L), "ASIC_TEST_PIN_SEL"
				0x80000000L), "TLB_REFILL"
				0x80000080L), "XTLB_REFILL"
				0x80000100L), "CACHE_ERROR"
				0x80000180L), "GEN_EXCEPTION"
				0x80000300L), "NTSC_PAL"
				0x80000304L), "CART_DD"
				0x80000308L), "ROM_BASE"
				0x8000030cL), "RESET"
				0x80000310L), "CIC_ID"
				0x80000314L), "VERSION"
				0x80000318L), "RDRAM_SIZE"
				0x8000031cL), "NMI_BUFFER"


*/


typedef enum PXN64CountryCode_
{
	PXN64CountryCodeInvalid,
	PXN64CountryCodeBeta,
	PXN64CountryCodeAsianNTSC,
	PXN64CountryCodeBrazilian,
	PXN64CountryCodeChinese,
	PXN64CountryCodeGerman,
	PXN64CountryCodeNorthAmerica,
	PXN64CountryCodeFrench,
	PXN64CountryCodeGateway64NTSC,
	PXN64CountryCodeDutch,
	PXN64CountryCodeItalian,
	PXN64CountryCodeJapanese,
	PXN64CountryCodeKorean,
	PXN64CountryCodeGateway64PAL,
	PXN64CountryCodeCanadian,
	PXN64CountryCodeEuropeanBasicSpec,
	PXN64CountryCodeSpanish,
	PXN64CountryCodeAustralian,
	PXN64CountryCodeScandinavian,
	PXN64CountryCodeEuropean
}
PXN64CountryCode;

typedef enum PXN64CICType_
{
	PXN64CICTypeInvalid,
	PXN64CICType6101,
	PXN64CICType6102,
	PXN64CICType6103,
	PXN64CICType6105,
	PXN64CICType6106
}
PXN64CICType;

typedef struct PXN64_
{
	PXEndian Endian;
	PXN64CICType CICType;
	void* RAMEntryPointAdress;

	PXInt8U EndiannessID;
	PXInt8U initialPI_BSB_DOM1_LAT_REG;
	PXInt8U initialPI_BSD_DOM1_PGS_REG;
	PXInt8U initialPI_BSD_DOM1_PWD_REG;
	PXInt8U initialPI_BSB_DOM1_PGS_REG;
	PXInt32U ClockRateOverride;
	PXInt32U RAMEntryPointOffset; // ProgramCounter, LoadAddress
	PXInt32U RAMEntryPointLength;
	PXInt32U ReleaseAddress;
	PXInt32U CRC1Checksum;
	PXInt32U CRC2;
	PXInt64U UnknownA;
	char ImageName[21];
	PXInt32U UnknownB;
	char MediaFormatID[4];
	char CartridgeID[3];
	PXInt8U CountryCodeID;
	PXN64CountryCode CountryCode;
	PXInt8U Version;

	void* BootCode;
	PXSize BootCodeSize;
	PXInt32U BootCodeCRC;
}
PXN64;

PXPrivate PXN64CountryCode PXAPI PXN64CountryCodeFromID(PXInt8U pxN64CountryCodeID);
PXPrivate const char* PXN64CountryCodeToString(const PXN64CountryCode pxN64CountryCode);

PXPublic PXActionResult PXAPI PXN64LoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXN64SaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif