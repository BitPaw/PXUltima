#include "SBPProtocol.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <OS/User/PXUser.h>

PXSize PXSBPChannelHeaderParse(PXSBPChannelHeader* const sbpChannelHeader, PXFile* const dataStream)
{
	const PXSize startOffset = dataStream->DataCursor;

	MemoryClear(sbpChannelHeader, sizeof(PXSBPChannelHeader));

	// Check header
	{
		const PXBool validHeader = PXFileReadAndCompare(dataStream, "같", 2u);

		if (!validHeader)
		{
			return 0;
		}
	}

	PXFileReadI8U(dataStream, &sbpChannelHeader->ID);
	//PXFileReadI16U(dataStream, &sbpChannelHeader->Size, PXEndianLittle);

	sbpChannelHeader->Data = PXFileCursorPosition(dataStream);

	return  dataStream->DataCursor - startOffset; // read Bytes
}

PXSize PXSBPChannelHeaderSerialize(PXSBPChannelHeader* const sbpChannelHeader, PXFile* const dataStream)
{
	const PXSize startOffset = dataStream->DataCursor;

	PXFileWriteB(dataStream, "같", 2u);
	PXFileWriteI8U(dataStream, sbpChannelHeader->ID);
	//PXFileWriteI16U(dataStream, sbpChannelHeader->Size, PXEndianLittle);

	return dataStream->DataCursor - startOffset; // written Bytes
}

void SBPPackageHeaderConstruct(SBPPackageHeader* const sbpPackageHeader)
{
	SBPPackageHeaderSet(sbpPackageHeader, PXInt32Make('#', '#', '#', '#'), SourceInvalid, TargetInvalid, 0, PXNull);
}

void SBPPackageHeaderDestruct(SBPPackageHeader* const sbpPackageHeader)
{

}

void SBPPackageHeaderSet
(
	SBPPackageHeader* const sbpPackageHeader,
	const unsigned int command,
	const unsigned int source,
	const unsigned int target,
	const unsigned int dataSize,
	const void* adress
)
{
	sbpPackageHeader->SourceID = command;
	sbpPackageHeader->TargetID = source;
	sbpPackageHeader->CommandID.Value = command;
	sbpPackageHeader->CommandSize = dataSize;
	sbpPackageHeader->Command = adress;

}

void SBPPackageHeaderPrint(SBPPackageHeader* const sbpData)
{
	const char commandText[5] =
	{
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
		"| Command |  Source  |  Target  |    --    |   Data   |\n"
		"| %7s | %8s | %8s | -------- | %6i B |\n"
		"+---------+----------+----------+----------+----------+\n",
		commandText,
		sourceText,
		targetText,
		sbpData->CommandSize
	);

	if(sbpData->CommandSize)
	{
		printf("+-------------------[Payload Data]--------------------+\n");
		for(PXSize i = 0; i < sbpData->CommandSize; ++i)
		{
			const char* text = (char*)sbpData->Command;
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

PXSize PXSBPPackageParse(SBPPackageHeader* const sbpPackageHeader, PXFile* const dataStream)
{
	const PXSize startOffset = dataStream->DataCursor;

	SBPPackageHeaderConstruct(sbpPackageHeader);

	// long enough

	//if (inputBufferSize < 22u)
	//{
	//	return 0;
	//}


	// Check header
	{
		const PXBool validHeader = PXFileReadAndCompare(&dataStream, "같", 2u);

		if (!validHeader)
		{
			return 0;
		}
	}

	//PXFileReadI32U(&dataStream, &sbpPackageHeader->SourceID, PXEndianLittle);
	PXFileReadI32UE(&dataStream, &sbpPackageHeader->TargetID, PXEndianLittle);
	PXFileReadB(&dataStream, sbpPackageHeader->CommandID.Data, 4u);

	// Fetch Size
	{
		char packageSize = 0;

		PXFileReadI8U(&dataStream, &packageSize);

		switch (packageSize)
		{
			case 0: // Ping package. No Data
			{
				break;
			}
			case 1: // 8-Bit package
			{
				PXInt8U size = 0;

				PXFileReadI8U(&dataStream, &size);

				sbpPackageHeader->CommandSize = size;

				break;
			}
			case 2: // 16-Bit Package
			{
				PXInt16U size = 0;

				PXFileReadI16U(&dataStream, &size);

				sbpPackageHeader->CommandSize = size;

				break;
			}
			case 3: // 32-Bit Package
			{
				PXInt32U size = 0;

				PXFileReadI32U(&dataStream, &size);

				sbpPackageHeader->CommandSize = size;

				break;
			}
			case 4: // 64-Bit Package
			{
				PXInt64U size = 0;

				PXFileReadI64U(&dataStream, &size);

				sbpPackageHeader->CommandSize = size;

				break;
			}

			default:
				break;
		}

		sbpPackageHeader->Command = PXFileCursorPosition(&dataStream);
	}

	return dataStream->DataCursor - startOffset; // read Bytes
}

PXSize PXSBPPackageSerialize(const SBPPackageHeader* data, PXFile* const dataStream)
{
	const PXSize startOffset = dataStream->DataCursor;

	// is buffer big enough?

	//if (inputBufferSize < 22u)
	//{
	//	return 0;
	//}

	PXFileWriteB(dataStream, "같", 2u);
	PXFileWriteI32UE(dataStream, &data->SourceID, PXEndianLittle);
	PXFileWriteI32UE(dataStream, &data->TargetID, PXEndianLittle);

	PXFileWriteI8U(dataStream, 3u);
	PXFileWriteI32UE(dataStream, &data->CommandSize, PXEndianLittle);
	PXFileWriteB(dataStream, data->Command, data->CommandSize);

	return dataStream->DataCursor - startOffset; // written Bytes
}


/*

PXSize SBPPackageHeaderPackageSerialize(const SBPPackageHeader& data, void* outputBuffer, const PXSize outputBufferSize)
{
	PXFileX stream(outputBuffer, outputBufferSize);

	stream.Write("같", 2u);
	stream.Write(data.CommandID.Data, 4u);
	stream.Write(data.SourceID, PXEndianLittle);
	stream.Write(data.TargetID, PXEndianLittle);
	stream.Write(data.ID, PXEndianLittle);
	stream.Write(data.DataSize, PXEndianLittle);
	stream.Write(data.Data, data.DataSize);

	return stream.DataCursor;
}

PXSize BF::SBPPackageHeader::PackageSerialize
(
	void* outputBuffer,
	const PXSize outputBufferSize,
	const unsigned int source,
	const unsigned int target,
	const SBPPackageHeaderPackage* dataPackage,
	const ResponseID responseID
)
{
	SBPPackageHeader data;

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











void SBPPackageHeaderPackageFileConstruct(SBPPackageHeaderPackageFile* const sbpDataPackageFile)
{
	SymbolID.Value = SBPPackageHeaderPackageFileID;

	Mode = SBPPackageHeaderPackageFileMode::Invalid;

	FilePathSourceFormat = TextFormatInvalid;
	FilePathSourceSize = 0;
	FilePathSourceW[0] = L'0';

	FilePathTargetFormat = TextFormatInvalid;
	FilePathTargetSize = 0;
	FilePathTargetW[0] = L'0';

	FileSize = 0;
}

void SBPPackageHeaderPackageFileFill(SBPPackageHeaderPackageFile* const sbpDataPackageFile, const SBPPackageHeaderPackageFileMode mode, const char* source, const char* target)
{
	Mode = mode;

	FilePathSourceFormat = TextFormatASCII;
	FilePathSourceSize = PXTextCopyA(source, PathMaxSize, FilePathSourceA, PathMaxSize);

	FilePathTargetFormat = TextFormatASCII;
	FilePathTargetSize = PXTextCopyA(target, PathMaxSize, FilePathTargetA, PathMaxSize);

	FileSize = 0;
}

PXSize SBPPackageHeaderPackageFileParse(SBPPackageHeaderPackageFile* const sbpDataPackageFile, const void* inputData, const PXSize inputDataSize)
{
	PXFileX byteStream(inputData, inputDataSize);

	{
		unsigned char modeID = 0;

		byteStream.Read(modeID);

		switch (modeID)
		{
		default:
			return 0;

		case SBPPackageHeaderPackageFileModeIDCreate:
			Mode = SBPPackageHeaderPackageFileMode::Create;
			break;

		case SBPPackageHeaderPackageFileModeIDDelete:
			Mode = SBPPackageHeaderPackageFileMode::Delete;
			break;

		case SBPPackageHeaderPackageFileModeIDUpdate:
			Mode = SBPPackageHeaderPackageFileMode::Update;
			break;

		case SBPPackageHeaderPackageFileModeIDMove:
			Mode = SBPPackageHeaderPackageFileMode::Move;
			break;

		case SBPPackageHeaderPackageFileModeIDCopy:
			Mode = SBPPackageHeaderPackageFileMode::Copy;
			break;

		case SBPPackageHeaderPackageFileModeIDRename:
			Mode = SBPPackageHeaderPackageFileMode::Rename;
			break;
		}
	}

	const unsigned int bitMask = 0b00001111111111111111111111111111;

	// FilePath
	{
		unsigned int FilePathSourceInfoBlock = 0;

		byteStream.Read(FilePathSourceInfoBlock, PXEndianLittle);

		unsigned char filePathSourceFormatID = (~bitMask & FilePathSourceInfoBlock) >> 28u;

		FilePathSourceSize = (bitMask & FilePathSourceInfoBlock);
		FilePathSourceFormat = (TextFormat)filePathSourceFormatID;

		byteStream.Read(FilePathSourceA, FilePathSourceSize);
	}

	// FilePath
	{
		unsigned int FilePathSourceInfoBlock = 0;

		byteStream.Read(FilePathSourceInfoBlock, PXEndianLittle);

		unsigned char filePathSourceFormatID = (~bitMask & FilePathSourceInfoBlock) >> 28u;

		FilePathTargetSize = (bitMask & FilePathSourceInfoBlock);
		FilePathTargetFormat = (TextFormat)filePathSourceFormatID;

		byteStream.Read(FilePathTargetW, FilePathSourceSize);
	}

	byteStream.Read(FileSize, PXEndianLittle);

	return byteStream.DataCursor;
}

PXSize SBPPackageHeaderPackageFileSerialize(SBPPackageHeaderPackageFile* const sbpDataPackageFile, void* outputData, const PXSize outputDataSize) const
{
	PXFileX byteStream(outputData, outputDataSize);

	{
		unsigned char modeID = 0;

		switch (Mode)
		{
		default:
		case BF::SBPPackageHeaderPackageFileMode::Invalid:
			return 0;

		case BF::SBPPackageHeaderPackageFileMode::Create:
			modeID = SBPPackageHeaderPackageFileModeIDCreate;
			break;

		case BF::SBPPackageHeaderPackageFileMode::Delete:
			modeID = SBPPackageHeaderPackageFileModeIDDelete;
			break;

		case BF::SBPPackageHeaderPackageFileMode::Update:
			modeID = SBPPackageHeaderPackageFileModeIDUpdate;
			break;

		case BF::SBPPackageHeaderPackageFileMode::Move:
			modeID = SBPPackageHeaderPackageFileModeIDMove;
			break;

		case BF::SBPPackageHeaderPackageFileMode::Copy:
			modeID = SBPPackageHeaderPackageFileModeIDCopy;
			break;

		case BF::SBPPackageHeaderPackageFileMode::Rename:
			modeID = SBPPackageHeaderPackageFileModeIDRename;
			break;
		}

		byteStream.Write(modeID);
	}



	const unsigned int bitMask = 0b00001111111111111111111111111111;

	// FilePath
	{
		const unsigned char filePathSourceFormatID = (TextFormat)FilePathSourceFormat;
		unsigned int FilePathSourceInfoBlock = FilePathSourceSize | (filePathSourceFormatID << 28u);

		byteStream.Write(FilePathSourceInfoBlock, PXEndianLittle);

		byteStream.Write(FilePathSourceA, FilePathSourceSize);
	}

	// FilePath
	{
		const unsigned char filePathSourceFormatID = (TextFormat)FilePathTargetFormat;
		unsigned int FilePathSourceInfoBlock = FilePathTargetSize | (filePathSourceFormatID << 28u);

		byteStream.Write(FilePathSourceInfoBlock, PXEndianLittle);

		byteStream.Write(FilePathTargetA, FilePathTargetSize);
	}

	byteStream.Write(FileSize, PXEndianLittle);

	return byteStream.DataCursor;
}

void SBPPackageHeaderPackageIamConstruct(SBPPackageHeaderPackageIam* const sbpDataPackageIam)
{
	Format = TextFormatInvalid;
	NameSize = 0;
	NameW[0] = L'\0';

	SymbolID.Value = SBPPackageHeaderPackageIamID;
}

void SBPPackageHeaderPackageIamFill(SBPPackageHeaderPackageIam* const sbpDataPackageIam)
{
	Format = TextFormatUNICODE;

	UserNameGetW(NameW, PathMaxSize, &NameSize);
}





void SBPPackageHeaderPackageResponseConstruct(SBPPackageHeaderPackageResponse* const sbpDataPackageResponse)
{
	Type = SBPPackageHeaderPackageResponseType::Invalid;
	SymbolID.Value = SBPPackageHeaderPackageResponseID;
}

PXSize SBPPackageHeaderPackageResponseParse(SBPPackageHeaderPackageResponse* const sbpDataPackageResponse, const void* inputData, const PXSize inputDataSize)
{
	PXFileX PXFile(inputData, inputDataSize);

	{
		unsigned char typeID = 0;

		PXFile.Read(typeID);

		switch (typeID)
		{
		case SBPPackageHeaderPackageResponseTypeOKID:
			Type = BF::SBPPackageHeaderPackageResponseType::OK;
			break;

		case SBPPackageHeaderPackageResponseTypeNoPermissionID:
			Type = BF::SBPPackageHeaderPackageResponseType::NoPermission;
			break;

		case SBPPackageHeaderPackageResponseTypeDeniedID:
			Type = BF::SBPPackageHeaderPackageResponseType::Denied;
			break;

		default:
			Type = BF::SBPPackageHeaderPackageResponseType::Invalid;
			break;
		}
	}

	return PXFile.DataCursor;
}

PXSize SBPPackageHeaderPackageResponseSerialize(SBPPackageHeaderPackageResponse* const sbpDataPackageResponse, void* outputData, const PXSize outputDataSize)
{
	PXFileX PXFile(outputData, outputDataSize);

	unsigned char typeID = 0;

	switch (Type)
	{
	default:
	case BF::SBPPackageHeaderPackageResponseType::Invalid:
		return 0;

	case BF::SBPPackageHeaderPackageResponseType::OK:
		typeID = SBPPackageHeaderPackageResponseTypeOKID;
		break;

	case BF::SBPPackageHeaderPackageResponseType::NoPermission:
		typeID = SBPPackageHeaderPackageResponseTypeNoPermissionID;
		break;

	case BF::SBPPackageHeaderPackageResponseType::Denied:
		typeID = SBPPackageHeaderPackageResponseTypeDeniedID;
		break;
	}

	PXFile.Write(typeID);

	return PXFile.DataCursor;
}

void SBPPackageHeaderPackageTextConstruct(SBPPackageHeaderPackageText* const sbpDataPackageText)
{
	sbpDataPackageText->SymbolID.Value = SBPPackageHeaderPackageTextID;
}

void SBPPackageHeaderPackageTextDestruct(SBPPackageHeaderPackageText* const sbpDataPackageText)
{
}

PXSize SBPPackageHeaderPackageTextParse(SBPPackageHeaderPackageText* const sbpDataPackageText, const void* inputData, const PXSize inputDataSize)
{
	PXFileX PXFile(inputData, inputDataSize);

	{
		unsigned char textModeID = 0;

		PXFile.Read(textModeID);

		TextData.Format = (TextFormat)textModeID;
	}

	PXFile.Read(TextData.SizeInBytes, PXEndianLittle);
	PXFile.Read(TextData.TextData, TextData.SizeInBytes);

	return PXFile.DataCursor;
}

PXSize SBPPackageHeaderPackageTextSerialize(SBPPackageHeaderPackageText* const sbpDataPackageText, void* outputData, const PXSize outputDataSize)
{
	PXFileX PXFile(outputData, outputDataSize);

	const unsigned char textModeID = (TextFormat)TextData.Format;

	PXFile.Write(textModeID);
	PXFile.Write(TextData.SizeInBytes, PXEndianLittle);
	PXFile.Write(TextData.TextData, TextData.SizeInBytes);

	return PXFile.DataCursor;
}

void SBPPackageHeaderPackageConnectionCreateFill(const SBPConnectionCreateReason reason)
{

}

PXSize SBPPackageHeaderPackageConnectionCreateParse(const void* inputData, const PXSize inputDataSize)
{
	return PXSize();
}

PXSize SBPPackageHeaderPackageConnectionCreateSerialize(void* outputData, const PXSize outputDataSize)
{
	return PXSize();
}
*/

PXSize SBPPackageHeaderPackageFileSerialize(SBPPackageHeaderPackageFile* const sbpDataPackageFile, void* outputData, const PXSize outputDataSize)
{
	return 0;
}

PXSize SBPPackageHeaderPackageIamParse(SBPPackageHeader* const sbpDataPackage, SBPPackageHeaderPackageIam* const sbpDataPackageIam)
{
	PXFile dataStream;

	PXFileBufferExternal(&dataStream, sbpDataPackage->Command, sbpDataPackage->CommandSize);

	// Add name
	{
		unsigned char formatType = 0;
		unsigned short size = 0;

		PXFileReadI8U(&dataStream, formatType);
		PXFileReadI16UE(&dataStream, size, PXEndianLittle);

		sbpDataPackageIam->Name.Format = formatType;
		sbpDataPackageIam->Name.SizeUsed = size;
		sbpDataPackageIam->Name.TextA = PXFileCursorPosition(&dataStream);

		PXFileCursorAdvance(&dataStream, size);
	}

	return dataStream.DataCursor;
}

PXSize SBPPackageHeaderPackageIamSerialize(SBPPackageHeader* const sbpDataPackage, SBPPackageHeaderPackageIam* const sbpDataPackageIam)
{
	PXFile dataStream;

	if (!sbpDataPackage->CommandSize)
	{
		return 0;
	}

	PXFileBufferExternal(&dataStream, sbpDataPackage->Command, sbpDataPackage->CommandSize);

	MemoryClear(sbpDataPackageIam, sizeof(SBPPackageHeaderPackageIam));

	// Add name
	PXByte nameBuffer[128];
	const PXSize nameBufferSize = PXUserNameGetU(nameBuffer, 128);

	{
		const unsigned char formatType = TextFormatUTF8;

		PXFileWriteI8U(&dataStream, formatType);
		PXFileWriteI16UE(&dataStream, nameBufferSize, PXEndianLittle);
		PXFileWriteB(&dataStream, nameBuffer, nameBufferSize);
	}

	SBPPackageHeaderSet(sbpDataPackage, SBPPackageHeaderPackageIamID, SourceMe, TargetServer, dataStream.DataCursor, PXNull);

	return dataStream.DataCursor;
}
