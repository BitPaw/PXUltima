#ifndef SBPProtocolINCLUDE
#define SBPProtocolINCLUDE

#include <Format/Type.h>
#include <Container/ClusterValue.h>
#include <File/File.h>
#include <Text/Text.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum SBPResult_
	{
		SBPResultInvalid,

		SBPResultPackageSendingFailure,

		// Nobody is listening for a result
		SBPResultPackageSendSucessfulButNoResponder,

		SBPResultPackageAnswered,
		SBPResultPackageTimeout,

		SBPResultPackageDetectedRegistered,
		SBPResultPackageDetectedCustom,

		SBPResultInvalidHeader
	}
	SBPResult;

	typedef struct SBPDataPackage_
	{
		ClusterInt SymbolID;
	}
	SBPDataPackage;

	PXSize SBPDataPackageParse(const void* inputData, const PXSize inputDataSize);
	PXSize SBPDataPackageSerialize(void* outputData, const PXSize outputDataSize);



#define ConnectionCreateReasonFile 'F'
#define ConnectionCreateReasonData 'D'


#define SBPIDConnectionCreate MakeInt('C', 'o', 'n', '+')
#define SBPIDConnectionResult MakeInt('C', 'o', 'n', '#')
#define SBPIDConnectionKill MakeInt('C', 'o', 'n', '-')

#define SBPIDText MakeInt('T', 'e', 'x', 't')



#define SBPFilePathSizeMaskTextType 0b1000000000000000 // 0=char* 1=wchar_t*
#define SBPFilePathSizeMaskA        0b0100000000000000
#define SBPFilePathSizeMaskB		0b0010000000000000
#define SBPFilePathSizeMaskC		0b0001000000000000
#define SBPFilePathSizeMask			0b1111000000000000

	// Source

#define SourceInvalid	-1
#define SourceMe	-2 // Request the reciever (Server mostly) to fill in data.
#define SourceServer	-3 // PXServer is the owner of this message

#define SourceLimitMaximum SourceServer

	typedef enum SBPSource_
	{
		SBPSourceInvalid = SourceInvalid,

		// Sending
		SBPSourceMe = SourceMe,
		SBPSourceServer = SourceServer
	}
	SBPSource;

	// Target
#define TargetInvalid		-1
#define TargetServer		-2 // PXClient(You) -> PXServer only
#define TargetAll		-3 // PXClient(You) -> All PXClients
#define TargetSpecific	-4 // PXClient(You) -> specific PXClients
#define TargetYou		-5 // *Something* -> PXClient(You)
#define TargetYouAndOthers	-6 // *Something* -> pecific PXClients + PXClient (You)
#define TargetEveryBody		-7 // *Something* -> All PXClients (You)

#define TargetLimitMaximum TargetEveryBody

#define ResponseID unsigned int

	typedef enum SBPTarget_
	{
		SBPTargetInvalid = TargetInvalid,

		// Sending
		SBPTargetServer = TargetServer,
		SBPTargetAll = TargetAll,
		SBPTargetSpesific = TargetSpecific,

		// Recieving
		SBPTargetYou = TargetYou,
		SBPTargetYouAndOthers = TargetYouAndOthers,
		SBPTargetEverybody = TargetEveryBody
	}
	SBPTarget;


	typedef struct SBPData_
	{
		ClusterInt CommandID;

		union
		{
			unsigned int SourceID;
			SBPSource Source;
		};

		union
		{
			unsigned int TargetID;
			SBPTarget Target;
		};

		unsigned int ID;
		unsigned int DataSize;
		void* Data;
	}
	SBPData;

	PXPublic void SBPDataConstruct(SBPData* const sbpData);
	PXPublic void SBPDataDestruct(SBPData* const sbpData);

	PXPublic void SBPDataSet
	(
		SBPData* const sbpData,
		const unsigned int command,
		const unsigned int source,
		const unsigned int target,
		const unsigned int id,
		const unsigned int dataSize,
		const void* adress
	);

	PXPublic unsigned int SBPDataSize(SBPData* const sbpData);

	PXPublic void SBPDataClear(SBPData* const sbpData);

	PXPublic void SBPDataPrint(SBPData* const sbpData);

	PXPublic PXSize PackageParse(SBPData* data, const void* inputBuffer, const PXSize* inputBufferSize);
	PXPublic PXSize PackageSerialize(const SBPData* data, void* outputBuffer, const PXSize outputBufferSize);
	/*
	CPublic PXSize PackageSerialize
	(
		void* outputBuffer,
		const PXSize outputBufferSize,
		const unsigned int source,
		const unsigned int target,
		const SBPDataPackage* dataPackage,
		const ResponseID responseID
	);*/

	// Recieve custom package, this is only called for unregistered packages
	typedef void (*PackageRecieveEvent)(const SBPData* sbpData);










	//---<Pre-Defined packages>----------------------------------------------

	typedef enum SBPConnectionCreateReason_
	{
		SBPConnectionCreateInvalid,
		SBPConnectionCreateVoiceChat,
		SBPConnectionCreateVideoChat,
		SBPConnectionCreateFileTransfer
	}
	SBPConnectionCreateReason;

	typedef struct SBPDataPackageConnectionCreate_
	{
		SBPConnectionCreateReason Reason;
	}
	SBPDataPackageConnectionCreate;

	PXPublic void SBPDataPackageConnectionCreateFill(const SBPConnectionCreateReason reason);

	PXPublic PXSize SBPDataPackageConnectionCreateParse(const void* inputData, const PXSize inputDataSize);
	PXPublic PXSize SBPDataPackageConnectionCreateSerialize(void* outputData, const PXSize outputDataSize);



#define SBPDataPackageFileID MakeInt('F', 'i', 'l', 'e')


#define SBPDataPackageFileModeIDCreate 0x01
#define SBPDataPackageFileModeIDDelete 0x02
#define SBPDataPackageFileModeIDUpdate 0x03
#define SBPDataPackageFileModeIDMove 0x04
#define SBPDataPackageFileModeIDCopy 0x05
#define SBPDataPackageFileModeIDRename 0x06

		typedef enum SBPDataPackageFileMode
		{
			Invalid,
			Create, // Create a file and upload data to it
			Delete, // Remove a file
			Update, // override data from the file
			Move, // Move the file form one place to another
			Copy, // Copy a file
			Rename // rename the file, no not change data
		}
		SBPDataPackageFileMode;

		typedef struct SBPDataPackageFile_
		{
			SBPDataPackageFileMode Mode;

			TextFormat FilePathSourceFormat;
			PXSize FilePathSourceSize;
			union
			{
				char FilePathSourceA[PathMaxSize];
				wchar_t FilePathSourceW[PathMaxSize];
			};

			TextFormat FilePathTargetFormat;
			PXSize FilePathTargetSize;
			union
			{
				char FilePathTargetA[PathMaxSize];
				wchar_t FilePathTargetW[PathMaxSize];
			};

			PXSize FileSize; // HAs to be 8 Byte__!
		}
		SBPDataPackageFile;



		PXPublic void SBPDataPackageFileConstruct(SBPDataPackageFile* const sbpDataPackageFile);

		// S:N T:N => Invalid
		// S:N T:Y => Create
		// S:Y T:N => Delete
		// S:Y T:Y => Copy

		PXPublic void SBPDataPackageFileFill(SBPDataPackageFile* const sbpDataPackageFile, const SBPDataPackageFileMode mode, const char* source, const char* target);

		PXPublic PXSize SBPDataPackageFileParse(SBPDataPackageFile* const sbpDataPackageFile, const void* inputData, const PXSize inputDataSize);
		PXPublic PXSize SBPDataPackageFileSerialize(SBPDataPackageFile* const sbpDataPackageFile, void* outputData, const PXSize outputDataSize);




#define SBPDataPackageIamID MakeInt('I', '\'', 'a', 'm')


	typedef struct SBPDataPackageIam_
	{
		TextFormat Format;
		PXSize NameSize;
		union
		{
			char NameA[PathMaxSize];
			wchar_t NameW[PathMaxSize];
		};
	}
	SBPDataPackageIam;


	PXPublic void SBPDataPackageIamConstruct(SBPDataPackageIam* const sbpDataPackageIam);

	PXPublic void SBPDataPackageIamFill(SBPDataPackageIam* const sbpDataPackageIam);

	PXPublic PXSize SBPDataPackageIamParse(SBPDataPackageIam* const sbpDataPackageIam, const void* inputData, const PXSize inputDataSize);
	PXPublic PXSize SBPDataPackageIamSerialize(SBPDataPackageIam* const sbpDataPackageIam, void* outputData, const PXSize outputDataSize);



#define SBPDataPackageResponseID MakeInt('R', 'e', 's', 'p')

#define SBPDataPackageResponseTypeOKID 0x01
#define SBPDataPackageResponseTypeNoPermissionID 0x02
#define SBPDataPackageResponseTypeDeniedID 0x03

		typedef enum SBPDataPackageResponseType_
		{
			SBPResponseInvalid,
			SBPResponseOK,
			SBPResponseNoPermission,
			SBPResponseDenied
		}
		SBPDataPackageResponseType;

		typedef struct SBPDataPackageResponse_
		{
			SBPDataPackageResponseType Type;
		}
		SBPDataPackageResponse;

		PXPublic void SBPDataPackageResponseConstruct(SBPDataPackageResponse* const sbpDataPackageResponse);

		PXPublic PXSize SBPDataPackageResponseParse(SBPDataPackageResponse* const sbpDataPackageResponse, const void* inputData, const PXSize inputDataSize);
		PXPublic PXSize SBPDataPackageResponseSerialize(SBPDataPackageResponse* const sbpDataPackageResponse, void* outputData, const PXSize outputDataSize);



#define SBPDataPackageTextID MakeInt('T', 'e', 'x', 't')

	typedef struct SBPDataPackageText_
	{
		Text TextData;
	}
	SBPDataPackageText;

	PXPublic void SBPDataPackageTextConstruct(SBPDataPackageText* const sbpDataPackageText);
	PXPublic void SBPDataPackageTextDestruct(SBPDataPackageText* const sbpDataPackageText);

	PXPublic PXSize SBPDataPackageTextParse(SBPDataPackageText* const sbpDataPackageText, const void* inputData, const PXSize inputDataSize);
	PXPublic PXSize SBPDataPackageTextSerialize(SBPDataPackageText* const sbpDataPackageText, void* outputData, const PXSize outputDataSize);




#ifdef __cplusplus
}
#endif

#endif
