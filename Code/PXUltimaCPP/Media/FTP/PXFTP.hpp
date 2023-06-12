#pragma once

#include <Media/FTP/PXFTP.h>

namespace PX
{

	class FTP
	{
		public:

		/*
		 PXSize CommandBuildBare(const char* tag, char* const dataBuffer, const PXSize dataBufferSize);
		 PXSize CommandBuild(const char* tag, const char* middle, char* const dataBuffer, const PXSize dataBufferSize);


		// USER <SP> <username> <CRLF>
		 PXSize CommandBuildUser(const char* username, char* const dataBuffer, const PXSize dataBufferSize);

		// 	PASS <SP> <password> <CRLF>
		 PXSize CommandBuildPassword(const char* password, char* const dataBuffer, const PXSize dataBufferSize);

		// ACCT <SP> <account - information> <CRLF>
		// PXSize CommandBuildAccountInfo(const char* username, char* const dataBuffer, const PXSize dataBufferSize);

		// CWD <SP> <pathname> <CRLF>
		 PXSize CommandBuiltWorkDirectoryChange(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// CDUP <CRLF>
		 PXSize CommandBuildCDUP(char* const dataBuffer, const PXSize dataBufferSize);

		// SMNT <SP> <pathname> <CRLF>
		 PXSize CommandBuildSMNT(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// QUIT <CRLF>
		 PXSize CommandBuildQUIT(char* const dataBuffer, const PXSize dataBufferSize);

		// REIN <CRLF>
		 PXSize CommandBuildREIN(char* const dataBuffer, const PXSize dataBufferSize);

		// PORT <SP> <host - port> <CRLF>
		 PXSize CommandBuildPORT(const char* port, char* const dataBuffer, const PXSize dataBufferSize);

		// PASV <CRLF>
		 PXSize CommandBuildPASV(char* const dataBuffer, const PXSize dataBufferSize);

		// 	TYPE <SP> <type - code> <CRLF>
		 PXSize CommandBuildTYPE(const char* code, char* const dataBuffer, const PXSize dataBufferSize);

		// STRU <SP> <structure - code> <CRLF>
		 PXSize CommandBuildSTRU(const char* code, char* const dataBuffer, const PXSize dataBufferSize);

		// MODE <SP> <mode - code> <CRLF>
		 PXSize CommandBuildMODE(const char* mode, char* const dataBuffer, const PXSize dataBufferSize);

		// RETR <SP> <pathname> <CRLF>
		 PXSize CommandBuildRETR(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// STOR <SP> <pathname> <CRLF>		
		 PXSize CommandBuildSTOR(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// STOU <CRLF>
		 PXSize CommandBuildSTOU(char* const dataBuffer, const PXSize dataBufferSize);

		// APPE <SP> <pathname> <CRLF>
		 PXSize CommandBuildAPPE(const char* marker, char* const dataBuffer, const PXSize dataBufferSize);

		// ALLO <SP> <decimal - integer>	[<SP> R <SP> <decimal - integer>] <CRLF>
		 PXSize CommandBuildALLO(const char* text, char* const dataBuffer, const PXSize dataBufferSize);

		// REST <SP> <marker> <CRLF>
		 PXSize CommandBuildREST(const char* marker, char* const dataBuffer, const PXSize dataBufferSize);

		// RNFR <SP> <pathname> <CRLF>
		 PXSize CommandBuildRNFR(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// RNTO <SP> <pathname> <CRLF>
		 PXSize CommandBuildRNTO(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// ABOR <CRLF>
		 PXSize CommandBuildAbort(char* const dataBuffer, const PXSize dataBufferSize);

		// DELE <SP> <pathname> <CRLF>
		 PXSize CommandBuildDELE(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// RMD <SP> <pathname> <CRLF>
		 PXSize CommandBuildDirectoryRemove(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// MKD <SP> <pathname> <CRLF>
		 PXSize CommandBuildMKD(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// PWD <CRLF>
		 PXSize CommandBuildDirectoryPrint(char* const dataBuffer, const PXSize dataBufferSize);

		// LIST[<SP> <pathname>] <CRLF>
		 PXSize CommandBuildDirectoryListAllFromCurrnet(char* const dataBuffer, const PXSize dataBufferSize);
		 PXSize CommandBuildLIST(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// NLST[<SP> <pathname>] <CRLF>
		 PXSize CommandBuildNLST(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// SITE <SP> <string> <CRLF>
		 PXSize CommandBuildSITE(const char* text, char* const dataBuffer, const PXSize dataBufferSize);

		// SYST <CRLF>
		 PXSize CommandBuildSYST(char* const dataBuffer, const PXSize dataBufferSize);

		// STAT[<SP> <pathname>] <CRLF>
		 PXSize CommandBuildStat(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize);

		// HELP[<SP> <string>] <CRLF>
		 PXSize CommandBuildHelp(const char* helpString, char* const dataBuffer, const PXSize dataBufferSize);

		// NOOP <CRLF>
		 PXSize CommandBuildNoOperation(char* const dataBuffer, const PXSize dataBufferSize);
		 */
	};
}