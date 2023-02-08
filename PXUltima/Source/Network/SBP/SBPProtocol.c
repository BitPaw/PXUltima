#include "SBPProtocol.h"

#include <Memory/PXMemory.h>
#include <File/PXDataStream.h>
#include <OS/User.h>

void SBPDataConstruct(SBPDataPackage* const sbpData)
{
	sbpData->SourceID = SourceInvalid;
	sbpData->TargetID = TargetInvalid;
	sbpData->CommandID.Value = MakeInt('#', '#', '#', '#');
	sbpData->ID = 0;
	sbpData->DataSizeCurrent = 0;
	sbpData->Data = 0;
}

void SBPDataDestruct(SBPDataPackage* const sbpData)
{
	
}

void SBPDataSet
(
	SBPDataPackage* const sbpData,
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
	sbpData->DataSizeTotal = dataSize;
	sbpData->Data = (void*)adress;
}

unsigned int SBPDataSize(SBPDataPackage* const sbpData)
{
	return 0;
}

PXBool PXSBPPackageIsConsumable(const SBPDataPackage* const sbpDataPackage)
{
	return sbpDataPackage->DataSizeCurrent == sbpDataPackage->DataSizeTotal;
}

void SBPDataClear(SBPDataPackage* const sbpData)
{
	sbpData->CommandID.Value = MakeInt('#', '#', '#', '#');
	sbpData->SourceID = -1;
	sbpData->TargetID = -1;
	sbpData->ID = -1;
	sbpData->DataSizeTotal = 0;
	sbpData->Data = 0;
}


void SBPDataPrint(SBPDataPackage* const sbpData)
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
		sbpData->DataSizeCurrent
	);

	if(sbpData->DataSizeCurrent)
	{
		printf("+-------------------[Payload Data]--------------------+\n");
		for(PXSize i = 0; i < sbpData->DataSizeCurrent; ++i)
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

PXSize PXSBPPackageParse(SBPDataPackage* data, const void* inputBuffer, const PXSize inputBufferSize)
{
	PXDataStream dataStream;
	PXSize bufferSize = 0;

	PXDataStreamFromExternal(&dataStream, inputBuffer, inputBufferSize);
	SBPDataClear(data);

	// long enough

	if (inputBufferSize < 22u)
	{
		return 0;
	}

	// Check header
	{
		const PXBool validHeader = PXDataStreamReadAndCompare(&dataStream, "°°", 2u);

		if (!validHeader)
		{
			return 0;
		}
	}

	PXDataStreamReadB(&dataStream, data->CommandID.Data, 4u);
	PXDataStreamReadI32U(&dataStream, &data->SourceID, EndianLittle);
	PXDataStreamReadI32U(&dataStream, &data->TargetID, EndianLittle);
	PXDataStreamReadI32U(&dataStream, &data->ID, EndianLittle);
	PXDataStreamReadI32U(&dataStream, &data->DataSizeTotal, EndianLittle);

	data->DataSizeCurrent = inputBufferSize;
	data->Data = PXDataStreamCursorPosition(&dataStream);

	return dataStream.DataCursor;
}

PXSize PXSBPPackageSerialize(const SBPDataPackage* data, void* outputBuffer, const PXSize outputBufferSize)
{
	return 0;
}


/*

PXSize SBPDataPackageSerialize(const SBPData& data, void* outputBuffer, const PXSize outputBufferSize)
{
	PXDataStreamX stream(outputBuffer, outputBufferSize);

	stream.Write("°°", 2u);
	stream.Write(data.CommandID.Data, 4u);
	stream.Write(data.SourceID, EndianLittle);
	stream.Write(data.TargetID, EndianLittle);
	stream.Write(data.ID, EndianLittle);
	stream.Write(data.DataSize, EndianLittle);
	stream.Write(data.Data, data.DataSize);

	return stream.DataCursor;
}

PXSize BF::SBPData::PackageSerialize
(
	void* outputBuffer,
	const PXSize outputBufferSize,
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


	const PXSize bufferSize = 512u;
	char buffer[bufferSize]{ 0 };

	data.DataSize = dataPackage->Serialize(buffer, bufferSize);
	data.Data = buffer;

	const PXSize result = PackageSerialize(data, outputBuffer, outputBufferSize);

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

PXSize SBPDataPackageFileParse(SBPDataPackageFile* const sbpDataPackageFile, const void* inputData, const PXSize inputDataSize)
{
	PXDataStreamX byteStream(inputData, inputDataSize);

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

PXSize SBPDataPackageFileSerialize(SBPDataPackageFile* const sbpDataPackageFile, void* outputData, const PXSize outputDataSize) const
{
	PXDataStreamX byteStream(outputData, outputDataSize);

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





void SBPDataPackageResponseConstruct(SBPDataPackageResponse* const sbpDataPackageResponse)
{
	Type = SBPDataPackageResponseType::Invalid;
	SymbolID.Value = SBPDataPackageResponseID;
}

PXSize SBPDataPackageResponseParse(SBPDataPackageResponse* const sbpDataPackageResponse, const void* inputData, const PXSize inputDataSize)
{
	PXDataStreamX PXDataStream(inputData, inputDataSize);

	{
		unsigned char typeID = 0;

		PXDataStream.Read(typeID);

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

	return PXDataStream.DataCursor;
}

PXSize SBPDataPackageResponseSerialize(SBPDataPackageResponse* const sbpDataPackageResponse, void* outputData, const PXSize outputDataSize)
{
	PXDataStreamX PXDataStream(outputData, outputDataSize);

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

	PXDataStream.Write(typeID);

	return PXDataStream.DataCursor;
}

void SBPDataPackageTextConstruct(SBPDataPackageText* const sbpDataPackageText)
{
	sbpDataPackageText->SymbolID.Value = SBPDataPackageTextID;
}

void SBPDataPackageTextDestruct(SBPDataPackageText* const sbpDataPackageText)
{
}

PXSize SBPDataPackageTextParse(SBPDataPackageText* const sbpDataPackageText, const void* inputData, const PXSize inputDataSize)
{
	PXDataStreamX PXDataStream(inputData, inputDataSize);

	{
		unsigned char textModeID = 0;

		PXDataStream.Read(textModeID);

		TextData.Format = (TextFormat)textModeID;
	}

	PXDataStream.Read(TextData.SizeInBytes, EndianLittle);
	PXDataStream.Read(TextData.TextData, TextData.SizeInBytes);

	return PXDataStream.DataCursor;
}

PXSize SBPDataPackageTextSerialize(SBPDataPackageText* const sbpDataPackageText, void* outputData, const PXSize outputDataSize)
{
	PXDataStreamX PXDataStream(outputData, outputDataSize);

	const unsigned char textModeID = (TextFormat)TextData.Format;

	PXDataStream.Write(textModeID);
	PXDataStream.Write(TextData.SizeInBytes, EndianLittle);
	PXDataStream.Write(TextData.TextData, TextData.SizeInBytes);

	return PXDataStream.DataCursor;
}

void SBPDataPackageConnectionCreateFill(const SBPConnectionCreateReason reason)
{

}

PXSize SBPDataPackageConnectionCreateParse(const void* inputData, const PXSize inputDataSize)
{
	return PXSize();
}

PXSize SBPDataPackageConnectionCreateSerialize(void* outputData, const PXSize outputDataSize)
{
	return PXSize();
}
*/

PXSize SBPDataPackageIamParse(SBPDataPackage* const sbpDataPackage, SBPDataPackageIam* const sbpDataPackageIam)
{
	PXDataStream dataStream;

	PXDataStreamFromExternal(&dataStream, sbpDataPackage->Data, sbpDataPackage->DataSizeCurrent);

	// Add name
	{
		unsigned char formatType = 0;
		unsigned short size = 0;

		PXDataStreamReadI8U(&dataStream, formatType);
		PXDataStreamReadI16U(&dataStream, size, EndianLittle);

		sbpDataPackageIam->Name.Format = formatType;
		sbpDataPackageIam->Name.SizeInBytes = size;
		sbpDataPackageIam->Name.TextData = PXDataStreamCursorPosition(&dataStream);

		PXDataStreamCursorAdvance(&dataStream, size);
	}

	return dataStream.DataCursor;
}

PXSize SBPDataPackageIamSerialize(SBPDataPackage* const sbpDataPackage, SBPDataPackageIam* const sbpDataPackageIam)
{
	PXDataStream dataStream;

	if (!sbpDataPackage->Data)
	{
		return 0;
	}

	PXDataStreamFromExternal(&dataStream, sbpDataPackage->Data, sbpDataPackage->DataSizeCurrent);

	// Add name	
	wchar_t nameBuffer[128];
	const PXSize nameBufferSize = UserNameGetW(nameBuffer, 128);

	{
		const unsigned char formatType = TextFormatUNICODE;

		PXDataStreamWriteI8U(&dataStream, formatType);
		PXDataStreamWriteI16U(&dataStream, nameBufferSize, EndianLittle);
		PXDataStreamWriteB(&dataStream, nameBuffer, nameBufferSize);
	}

	sbpDataPackage->CommandID.Value = SBPDataPackageIamID;
	sbpDataPackage->Source = SourceMe;
	sbpDataPackage->Target = TargetServer;
	sbpDataPackage->ID = -1;
	sbpDataPackage->DataSizeCurrent = dataStream.DataCursor;
	sbpDataPackage->DataSizeTotal = dataStream.DataCursor;

	return dataStream.DataCursor;
}