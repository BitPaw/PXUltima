#include "SBPProtocol.h"

#include <Memory/Memory.h>
#include <File/ParsingStream.h>
#include <OS/User.h>

void SBPDataConstruct(SBPData* const sbpData)
{
	sbpData->SourceID = SourceInvalid;
	sbpData->TargetID = TargetInvalid;
	sbpData->CommandID.Value = MakeInt('#', '#', '#', '#');
	sbpData->ID = 0;
	sbpData->DataSize = 0;
	sbpData->Data = 0;
}

void SBPDataSet
(
	SBPData* const sbpData,
	const unsigned int command,
	const unsigned int source,
	const unsigned int target,
	const unsigned int id,
	const unsigned int dataSize,
	const void* adress
)
{
	sbpData->CommandID.Value = command;
	sbpData->SourceID = source;
	sbpData->TargetID = target;
	sbpData->ID = id;
	sbpData->DataSize = dataSize;
	sbpData->Data = (void*)adress;
}

unsigned int SBPDataSize(SBPData* const sbpData)
{
	return 0;
}

void SBPDataClear(SBPData* const sbpData)
{
	sbpData->CommandID.Value = MakeInt('#', '#', '#', '#');
	sbpData->SourceID = -1;
	sbpData->TargetID = -1;
	sbpData->ID = -1;
	sbpData->DataSize = 0;
	sbpData->Data = 0;
}


void SBPDataPrint(SBPData* const sbpData)
{
	const char commandText[5]
	={
		sbpData->CommandID.A,
		sbpData->CommandID.B,
		sbpData->CommandID.C,
		sbpData->CommandID.D,
		'\0'
	};

	char sourceText[10];
	char targetText[10];

	if(sbpData->SourceID >= (unsigned int)SourceLimitMaximum)
	{
		const char* textINN = 0;

		switch(sbpData->TargetID)
		{
			default:
			case SourceInvalid:
				textINN = "Invalid";
				break;

			case SourceMe:
				textINN = "Me";
				break;

			case SourceServer:
				textINN = "Server";
				break;
		}

		sprintf(sourceText, "%s", textINN);
	}
	else
	{
		sprintf(sourceText, "%8X", sbpData->Source);
	}

	if(sbpData->TargetID >= (unsigned int)TargetLimitMaximum)
	{
		const char* textINN = 0;

		switch(sbpData->TargetID)
		{
			default:
			case TargetInvalid:
				textINN = "Invalid";
				break;

			case TargetServer:
				textINN = "Me->Ser";
				break;

			case TargetAll:
				textINN = "Me->All";
				break;

			case TargetSpecific:
				textINN = "Me->Oth";
				break;

			case TargetYou:
				textINN = "Ser->You";
				break;

			case TargetYouAndOthers:
				textINN = "Othe->You";
				break;

			case TargetEveryBody:
				textINN = "ToAll";
				break;
		}

		sprintf(targetText, "%s", textINN);
	}
	else
	{
		sprintf(targetText, "%8X", sbpData->Target);
	}

	printf
	(
		"+---------+----------+----------+----------+----------+\n"
		"| Command |  Source  |  Target  |    ID    |   Data   |\n"
		"| %7s | %8s | %8s | %8X | %6i B |\n"
		"+---------+----------+----------+----------+----------+\n",
		commandText,
		sourceText,
		targetText,
		sbpData->ID,
		sbpData->DataSize
	);

	if(sbpData->DataSize)
	{
		printf("+-------------------[Payload Data]--------------------+\n");
		for(size_t i = 0; i < sbpData->DataSize; ++i)
		{
			const char* text = (char*)sbpData->Data;
			const char byte = text[i];
			const unsigned char printableChar = (byte > 0x20) && (byte < 0x7F);
			const char printChar = printableChar ? byte : '.';

			printf("%c", printChar);

			if(((i + 1) % 64) == 0)
			{
				printf("\n");
			}
		}
		printf("\n");
		printf("+-----------------------------------------------------+\n");
	}
}

/*

size_t SBPDataPackageParse(SBPData* data, const void* inputBuffer, const size_t* inputBufferSize)
{
	ParsingStream dataStream(inputBuffer, inputBufferSize);

	size_t bufferSize = 0;

	data.Clear();

	// long engigh

	if(inputBufferSize < 22u)
	{
		return 0;
	}

	// Check header
	{
		const bool validHeader = dataStream.ReadAndCompare("°°", 2u);

		if(!validHeader)
		{
			return 0;
		}
	}

	dataStream.Read(data.CommandID.Data, 4u);
	dataStream.Read(data.SourceID, EndianLittle);
	dataStream.Read(data.TargetID, EndianLittle);
	dataStream.Read(data.ID, EndianLittle);
	dataStream.Read(data.DataSize, EndianLittle);

	data.Data = dataStream.CursorCurrentAdress();

	return dataStream.DataCursor;
}

size_t SBPDataPackageSerialize(const SBPData& data, void* outputBuffer, const size_t outputBufferSize)
{
	ParsingStreamX stream(outputBuffer, outputBufferSize);

	stream.Write("°°", 2u);
	stream.Write(data.CommandID.Data, 4u);
	stream.Write(data.SourceID, EndianLittle);
	stream.Write(data.TargetID, EndianLittle);
	stream.Write(data.ID, EndianLittle);
	stream.Write(data.DataSize, EndianLittle);
	stream.Write(data.Data, data.DataSize);

	return stream.DataCursor;
}

size_t BF::SBPData::PackageSerialize
(
	void* outputBuffer,
	const size_t outputBufferSize,
	const unsigned int source,
	const unsigned int target,
	const SBPDataPackage* dataPackage,
	const ResponseID responseID
)
{
	SBPData data;

	data.SourceID = source;
	data.TargetID = target;
	data.ID = responseID;
	data.CommandID.Value = dataPackage->SymbolID.Value;


	const size_t bufferSize = 512u;
	char buffer[bufferSize]{ 0 };

	data.DataSize = dataPackage->Serialize(buffer, bufferSize);
	data.Data = buffer;

	const size_t result = PackageSerialize(data, outputBuffer, outputBufferSize);

	return result;
}











void SBPDataPackageFileConstruct(SBPDataPackageFile* const sbpDataPackageFile)
{
	SymbolID.Value = SBPDataPackageFileID;

	Mode = SBPDataPackageFileMode::Invalid;

	FilePathSourceFormat = TextFormatInvalid;
	FilePathSourceSize = 0;
	FilePathSourceW[0] = L'0';

	FilePathTargetFormat = TextFormatInvalid;
	FilePathTargetSize = 0;
	FilePathTargetW[0] = L'0';

	FileSize = 0;
}

void SBPDataPackageFileFill(SBPDataPackageFile* const sbpDataPackageFile, const SBPDataPackageFileMode mode, const char* source, const char* target)
{
	Mode = mode;

	FilePathSourceFormat = TextFormatASCII;
	FilePathSourceSize = TextCopyA(source, PathMaxSize, FilePathSourceA, PathMaxSize);

	FilePathTargetFormat = TextFormatASCII;
	FilePathTargetSize = TextCopyA(target, PathMaxSize, FilePathTargetA, PathMaxSize);

	FileSize = 0;
}

size_t SBPDataPackageFileParse(SBPDataPackageFile* const sbpDataPackageFile, const void* inputData, const size_t inputDataSize)
{
	ParsingStreamX byteStream(inputData, inputDataSize);

	{
		unsigned char modeID = 0;

		byteStream.Read(modeID);

		switch (modeID)
		{
		default:
			return 0;

		case SBPDataPackageFileModeIDCreate:
			Mode = SBPDataPackageFileMode::Create;
			break;

		case SBPDataPackageFileModeIDDelete:
			Mode = SBPDataPackageFileMode::Delete;
			break;

		case SBPDataPackageFileModeIDUpdate:
			Mode = SBPDataPackageFileMode::Update;
			break;

		case SBPDataPackageFileModeIDMove:
			Mode = SBPDataPackageFileMode::Move;
			break;

		case SBPDataPackageFileModeIDCopy:
			Mode = SBPDataPackageFileMode::Copy;
			break;

		case SBPDataPackageFileModeIDRename:
			Mode = SBPDataPackageFileMode::Rename;
			break;
		}
	}

	const unsigned int bitMask = 0b00001111111111111111111111111111;

	// FilePath
	{
		unsigned int FilePathSourceInfoBlock = 0;

		byteStream.Read(FilePathSourceInfoBlock, EndianLittle);

		unsigned char filePathSourceFormatID = (~bitMask & FilePathSourceInfoBlock) >> 28u;

		FilePathSourceSize = (bitMask & FilePathSourceInfoBlock);
		FilePathSourceFormat = (TextFormat)filePathSourceFormatID;

		byteStream.Read(FilePathSourceA, FilePathSourceSize);
	}

	// FilePath
	{
		unsigned int FilePathSourceInfoBlock = 0;

		byteStream.Read(FilePathSourceInfoBlock, EndianLittle);

		unsigned char filePathSourceFormatID = (~bitMask & FilePathSourceInfoBlock) >> 28u;

		FilePathTargetSize = (bitMask & FilePathSourceInfoBlock);
		FilePathTargetFormat = (TextFormat)filePathSourceFormatID;

		byteStream.Read(FilePathTargetW, FilePathSourceSize);
	}

	byteStream.Read(FileSize, EndianLittle);

	return byteStream.DataCursor;
}

size_t SBPDataPackageFileSerialize(SBPDataPackageFile* const sbpDataPackageFile, void* outputData, const size_t outputDataSize) const
{
	ParsingStreamX byteStream(outputData, outputDataSize);

	{
		unsigned char modeID = 0;

		switch (Mode)
		{
		default:
		case BF::SBPDataPackageFileMode::Invalid:
			return 0;

		case BF::SBPDataPackageFileMode::Create:
			modeID = SBPDataPackageFileModeIDCreate;
			break;

		case BF::SBPDataPackageFileMode::Delete:
			modeID = SBPDataPackageFileModeIDDelete;
			break;

		case BF::SBPDataPackageFileMode::Update:
			modeID = SBPDataPackageFileModeIDUpdate;
			break;

		case BF::SBPDataPackageFileMode::Move:
			modeID = SBPDataPackageFileModeIDMove;
			break;

		case BF::SBPDataPackageFileMode::Copy:
			modeID = SBPDataPackageFileModeIDCopy;
			break;

		case BF::SBPDataPackageFileMode::Rename:
			modeID = SBPDataPackageFileModeIDRename;
			break;
		}

		byteStream.Write(modeID);
	}



	const unsigned int bitMask = 0b00001111111111111111111111111111;

	// FilePath
	{
		const unsigned char filePathSourceFormatID = (TextFormat)FilePathSourceFormat;
		unsigned int FilePathSourceInfoBlock = FilePathSourceSize | (filePathSourceFormatID << 28u);

		byteStream.Write(FilePathSourceInfoBlock, EndianLittle);

		byteStream.Write(FilePathSourceA, FilePathSourceSize);
	}

	// FilePath
	{
		const unsigned char filePathSourceFormatID = (TextFormat)FilePathTargetFormat;
		unsigned int FilePathSourceInfoBlock = FilePathTargetSize | (filePathSourceFormatID << 28u);

		byteStream.Write(FilePathSourceInfoBlock, EndianLittle);

		byteStream.Write(FilePathTargetA, FilePathTargetSize);
	}

	byteStream.Write(FileSize, EndianLittle);

	return byteStream.DataCursor;
}

void SBPDataPackageIamConstruct(SBPDataPackageIam* const sbpDataPackageIam)
{
	Format = TextFormatInvalid;
	NameSize = 0;
	NameW[0] = L'\0';

	SymbolID.Value = SBPDataPackageIamID;
}

void SBPDataPackageIamFill(SBPDataPackageIam* const sbpDataPackageIam)
{
	Format = TextFormatUNICODE;

	UserNameGetW(NameW, PathMaxSize, &NameSize);
}

size_t SBPDataPackageIamParse(SBPDataPackageIam* const sbpDataPackageIam, const void* inputData, const size_t inputDataSize)
{
	ParsingStreamX parsingStream(inputData, inputDataSize);

	// Add name
	{
		unsigned char formatType = 0;
		unsigned short size = 0;

		parsingStream.Read(formatType);
		parsingStream.Read(size, EndianLittle);

		NameSize = size;
		Format = (TextFormat)formatType;

		const Byte__* nameStart = parsingStream.CursorCurrentAdress();

		switch (Format)
		{
		case TextFormatASCII:
			MemoryCopy(nameStart, size, NameA, PathMaxSize);
			break;

		case TextFormatUNICODE:
			MemoryCopy(nameStart, size, NameW, PathMaxSize);
			break;

		default:
			break;
		}
	}

	return parsingStream.DataCursor;
}

size_t SBPDataPackageIamSerialize(SBPDataPackageIam* cosnt sbpDataPackageIam, void* outputData, const size_t outputDataSize) const
{
	ParsingStreamX parsingStream(outputData, outputDataSize);

	// Add name
	{
		const unsigned char formatType = (TextFormat)Format;
		unsigned short size = NameSize;

		parsingStream.Write(formatType);
		parsingStream.Write(size, EndianLittle);
		parsingStream.Write(NameW, size);
	}

	return parsingStream.DataCursor;
}

void SBPDataPackageResponseConstruct(SBPDataPackageResponse* const sbpDataPackageResponse)
{
	Type = SBPDataPackageResponseType::Invalid;
	SymbolID.Value = SBPDataPackageResponseID;
}

size_t SBPDataPackageResponseParse(SBPDataPackageResponse* const sbpDataPackageResponse, const void* inputData, const size_t inputDataSize)
{
	ParsingStreamX parsingStream(inputData, inputDataSize);

	{
		unsigned char typeID = 0;

		parsingStream.Read(typeID);

		switch (typeID)
		{
		case SBPDataPackageResponseTypeOKID:
			Type = BF::SBPDataPackageResponseType::OK;
			break;

		case SBPDataPackageResponseTypeNoPermissionID:
			Type = BF::SBPDataPackageResponseType::NoPermission;
			break;

		case SBPDataPackageResponseTypeDeniedID:
			Type = BF::SBPDataPackageResponseType::Denied;
			break;

		default:
			Type = BF::SBPDataPackageResponseType::Invalid;
			break;
		}
	}

	return parsingStream.DataCursor;
}

size_t SBPDataPackageResponseSerialize(SBPDataPackageResponse* const sbpDataPackageResponse, void* outputData, const size_t outputDataSize)
{
	ParsingStreamX parsingStream(outputData, outputDataSize);

	unsigned char typeID = 0;

	switch (Type)
	{
	default:
	case BF::SBPDataPackageResponseType::Invalid:
		return 0;

	case BF::SBPDataPackageResponseType::OK:
		typeID = SBPDataPackageResponseTypeOKID;
		break;

	case BF::SBPDataPackageResponseType::NoPermission:
		typeID = SBPDataPackageResponseTypeNoPermissionID;
		break;

	case BF::SBPDataPackageResponseType::Denied:
		typeID = SBPDataPackageResponseTypeDeniedID;
		break;
	}

	parsingStream.Write(typeID);

	return parsingStream.DataCursor;
}

void SBPDataPackageTextConstruct(SBPDataPackageText* const sbpDataPackageText)
{
	sbpDataPackageText->SymbolID.Value = SBPDataPackageTextID;
}

void SBPDataPackageTextDestruct(SBPDataPackageText* const sbpDataPackageText)
{
}

size_t SBPDataPackageTextParse(SBPDataPackageText* const sbpDataPackageText, const void* inputData, const size_t inputDataSize)
{
	ParsingStreamX parsingStream(inputData, inputDataSize);

	{
		unsigned char textModeID = 0;

		parsingStream.Read(textModeID);

		TextData.Format = (TextFormat)textModeID;
	}

	parsingStream.Read(TextData.SizeInBytes, EndianLittle);
	parsingStream.Read(TextData.TextData, TextData.SizeInBytes);

	return parsingStream.DataCursor;
}

size_t SBPDataPackageTextSerialize(SBPDataPackageText* const sbpDataPackageText, void* outputData, const size_t outputDataSize)
{
	ParsingStreamX parsingStream(outputData, outputDataSize);

	const unsigned char textModeID = (TextFormat)TextData.Format;

	parsingStream.Write(textModeID);
	parsingStream.Write(TextData.SizeInBytes, EndianLittle);
	parsingStream.Write(TextData.TextData, TextData.SizeInBytes);

	return parsingStream.DataCursor;
}

void SBPDataPackageConnectionCreateFill(const SBPConnectionCreateReason reason)
{

}

size_t SBPDataPackageConnectionCreateParse(const void* inputData, const size_t inputDataSize)
{
	return size_t();
}

size_t SBPDataPackageConnectionCreateSerialize(void* outputData, const size_t outputDataSize)
{
	return size_t();
}
*/