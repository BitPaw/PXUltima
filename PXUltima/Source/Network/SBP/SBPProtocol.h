#ifndef SBPProtocolINCLUDE
#define SBPProtocolINCLUDE

#include <stddef.h>

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

	size_t SBPDataPackageParse(const void* inputData, const size_t inputDataSize);
	size_t SBPDataPackageSerialize(void* outputData, const size_t outputDataSize);



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
#define SourceServer	-3 // Server is the owner of this message

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
#define TargetServer		-2 // Client(You) -> Server only
#define TargetAll		-3 // Client(You) -> All Clients
#define TargetSpecific	-4 // Client(You) -> specific Clients
#define TargetYou		-5 // *Something* -> Client(You)
#define TargetYouAndOthers	-6 // *Something* -> pecific Clients + Client (You)
#define TargetEveryBody		-7 // *Something* -> All Clients (You)

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

	extern void SBPDataConstruct(SBPData* const sbpData);
	extern void SBPDataDestruct(SBPData* const sbpData);

	extern void SBPDataSet
	(
		SBPData* const sbpData,
		const unsigned int command,
		const unsigned int source,
		const unsigned int target,
		const unsigned int id,
		const unsigned int dataSize,
		const void* adress
	);

	extern unsigned int SBPDataSize(SBPData* const sbpData);

	extern void SBPDataClear(SBPData* const sbpData);

	extern void SBPDataPrint(SBPData* const sbpData);

	extern size_t PackageParse(SBPData* data, const void* inputBuffer, const size_t* inputBufferSize);
	extern size_t PackageSerialize(const SBPData* data, void* outputBuffer, const size_t outputBufferSize);
	extern size_t PackageSerialize
	(
		void* outputBuffer,
		const size_t outputBufferSize,
		const unsigned int source,
		const unsigned int target,
		const SBPDataPackage* dataPackage,
		const ResponseID responseID
	);

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

	extern void SBPDataPackageConnectionCreateFill(const SBPConnectionCreateReason reason);

	extern size_t SBPDataPackageConnectionCreateParse(const void* inputData, const size_t inputDataSize);
	extern size_t SBPDataPackageConnectionCreateSerialize(void* outputData, const size_t outputDataSize);



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
			size_t FilePathSourceSize;
			union
			{
				char FilePathSourceA[PathMaxSize];
				wchar_t FilePathSourceW[PathMaxSize];
			};

			TextFormat FilePathTargetFormat;
			size_t FilePathTargetSize;
			union
			{
				char FilePathTargetA[PathMaxSize];
				wchar_t FilePathTargetW[PathMaxSize];
			};

			size_t FileSize; // HAs to be 8 Byte__!			
		}
		SBPDataPackageFile;
	


		extern void SBPDataPackageFileConstruct(SBPDataPackageFile* const sbpDataPackageFile);

		// S:N T:N => Invalid
		// S:N T:Y => Create
		// S:Y T:N => Delete
		// S:Y T:Y => Copy

		extern void SBPDataPackageFileFill(SBPDataPackageFile* const sbpDataPackageFile, const SBPDataPackageFileMode mode, const char* source, const char* target);

		extern size_t SBPDataPackageFileParse(SBPDataPackageFile* const sbpDataPackageFile, const void* inputData, const size_t inputDataSize);
		extern size_t SBPDataPackageFileSerialize(SBPDataPackageFile* const sbpDataPackageFile, void* outputData, const size_t outputDataSize);




#define SBPDataPackageIamID MakeInt('I', '\'', 'a', 'm')


	typedef struct SBPDataPackageIam_
	{
		TextFormat Format;
		size_t NameSize;
		union
		{
			char NameA[PathMaxSize];
			wchar_t NameW[PathMaxSize];
		};		
	}
	SBPDataPackageIam;


	extern void SBPDataPackageIamConstruct(SBPDataPackageIam* const sbpDataPackageIam);

	extern void SBPDataPackageIamFill(SBPDataPackageIam* const sbpDataPackageIam);

	extern size_t SBPDataPackageIamParse(SBPDataPackageIam* const sbpDataPackageIam, const void* inputData, const size_t inputDataSize);
	extern size_t SBPDataPackageIamSerialize(SBPDataPackageIam* const sbpDataPackageIam, void* outputData, const size_t outputDataSize);



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
	
		extern void SBPDataPackageResponseConstruct(SBPDataPackageResponse* const sbpDataPackageResponse);

		extern size_t SBPDataPackageResponseParse(SBPDataPackageResponse* const sbpDataPackageResponse, const void* inputData, const size_t inputDataSize);
		extern size_t SBPDataPackageResponseSerialize(SBPDataPackageResponse* const sbpDataPackageResponse, void* outputData, const size_t outputDataSize);



#define SBPDataPackageTextID MakeInt('T', 'e', 'x', 't')

	typedef struct SBPDataPackageText_
	{
		Text TextData;
	}
	SBPDataPackageText;

	extern void SBPDataPackageTextConstruct(SBPDataPackageText* const sbpDataPackageText);
	extern void SBPDataPackageTextDestruct(SBPDataPackageText* const sbpDataPackageText);

	extern size_t SBPDataPackageTextParse(SBPDataPackageText* const sbpDataPackageText, const void* inputData, const size_t inputDataSize);
	extern size_t SBPDataPackageTextSerialize(SBPDataPackageText* const sbpDataPackageText, void* outputData, const size_t outputDataSize);




#ifdef __cplusplus
}
#endif

#endif