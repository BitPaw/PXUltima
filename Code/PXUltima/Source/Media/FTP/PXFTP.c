#include "PXFTP.h"

#include <stdio.h>

PXFTPResult PXFTPResultParse(const unsigned short resultID)
{
	switch (resultID)
	{
			// 1xx
			// 110 Restart marker reply.
			// 120 Service ready in nnn minutes.
			// 125 Data connection already open; transfer starting.
			// 150 File status okay; about to open data connection.
			// 120 Service ready in nnn minutes.
			// 150 File status okay; about to open data connection.

			//---------------------------------------------------------------------

			// 2xx

		case 200: return PXFTPResultOK;
		case 202: return PXFTPResultCommandNotImplemented;
		case 211: return PXFTPResultSystemStatus; 
		case 212: return PXFTPResultDirectoryStatus; 
		case 213: return PXFTPResultFileStatus; 
		case 214: return PXFTPResultHelp; 
		case 215: return PXFTPResultName; 
		case 220: return PXFTPResultLoginReady; 
		case 221: return PXFTPResultServiceClosed; 
		case 225: return PXFTPResultServiceReady; 
		case 227: return PXFTPResultPassiveModeNowActive;
		case 230: return PXFTPResultLoginCompleate;
		case 250: return PXFTPResultFileActionOK;
		case 257: return PXFTPResultPathNameCreated;


			//---------------------------------------------------------------------

			// 226 Closing data connection.Requested file action successful(for example, file				 transfer or file abort).
			// 227 Entering Passive Mode(h1, h2, h3, h4, p1, p2).

			// 230 User logged in, proceed.

			// 250 Requested file action okay, completed.
			// 257 "PATHNAME" created.
			// 200 Command okay.
			// 202 Command not implemented, superfluous at this site.
			// 211 System status, or system help reply.
			// 212 Directory status.
			// 213 File status.
			// 214 Help message.
			// 215 NAME system type.
			// 220 Service ready for new user.
			// 221 Service closing control connection.Logged out if appropriate.
			// 225 Data connection open; no transfer in progress.
			// 226 Closing data connection.Requested file action successful(for example, file										 transfer or file abort).
			// 227 Entering Passive Mode(h1, h2, h3, h4, p1, p2).
			// 230 User logged in, proceed.
			// 250 Requested file action okay, completed.
			// 257 "PATHNAME" created.
			//---------------------------------------------------------------------


			// 3xx
		case 331: return PXFTPResultLoginUserOKButPasswordMissing; // 331 User name okay, need password.
		case 332: return PXFTPResultLoginAccountNeeded; // 332 Need account for login.
		case 350: return PXFTPResultActionPending; // 350 Requested file action pending further information.
			//---------------------------------------------------------------------

			// 4xx
		case 421: return PXFTPResultServiceNotAvailable; // 421 Service not available, closing control connection.
		case 425: return PXFTPResultConnectionCantBeOpened; // 425 Can’t open data connection.
		case 426: return PXFTPResultConnectionClosed; // 426 Connection closed; transfer aborted.
			// 450 Requested file action not taken.File unavailable(e.g., file busy).
		case 451: return PXFTPResultActionAborted; // 451 Requested action aborted : local error in processing.
			// 452 Requested action not taken.Insufficient storage space in system.
			//---------------------------------------------------------------------

			// 5xx
		case 500: return PXFTPResultCommandInvalid; // 500 Syntax error, command unrecognized.	This may include errors such as command line too long.
		case 501: return PXFTPResultParameterSyntaxError; // 501 Syntax error in parameters or arguments.

			// 503 Bad sequence of commands.
			// 504 Command not implemented for that parameter.
			// 500 Syntax error, command unrecognized.This may include errors such as command line too long.
			// 501 Syntax error in parameters or arguments.
			// 502 Command not implemented.
			// 503 Bad sequence of commands.
			// 504 Command not implemented for that parameter.
		case 530: return PXFTPResultUserNotLoggedIn; // 530 Not logged in.
			// 532 Need account for storing files.
			// 550 Requested action not taken.File unavailable(e.g., file not found, no access).
			// 551 Requested action aborted : page type unknown.
		case 552: return PXFTPResultSpaceTooSmal; //  552 Requested file action aborted.Exceeded storage allocation(for current directory or dataset).
		case 553: return PXFTPResultFileNameRejected; // 553 Requested action not taken.File name not allowed.

		default:
			return PXFTPResultInvalid;
	}
}

PXSize PXFTPCommandBuildBare(const char* tag, char* const dataBuffer, const PXSize dataBufferSize)
{
	return sprintf(dataBuffer, "%s \r\n", tag);
}

PXSize PXFTPCommandBuild(const char* tag, const char* middle, char* const dataBuffer, const PXSize dataBufferSize)
{
	return sprintf(dataBuffer, "%s %s\r\n", tag, middle);
}

PXSize PXFTPCommandBuildUser(const char* username, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("USER", username, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildPassword(const char* password, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("PASS", password, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuiltWorkDirectoryChange(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("CWD", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildCDUP(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("CDUP", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildSMNT(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("SMNT", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildQUIT(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("QUIT", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildREIN(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("REIN", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildPORT(const char* port, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("PORT", port, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildPASV(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("PASV", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildTYPE(const char* code, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("TYPE", code, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildSTRU(const char* code, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("STRU", code, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildMODE(const char* mode, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("MODE", mode, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildRETR(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("RETR", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildSTOR(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("STOR", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildSTOU(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("STOU", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildAPPE(const char* marker, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("APPE", marker, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildALLO(const char* text, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("ALLO", text, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildREST(const char* marker, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("REST", marker, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildRNFR(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("RNFR", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildRNTO(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("RNTO", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildAbort(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("ABOR", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildDELE(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("DELE", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildDirectoryRemove(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("RMD", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildMKD(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("MKD", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildDirectoryPrint(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("PWD", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildDirectoryListAllFromCurrnet(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("NLST", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildLIST(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("LIST", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildNLST(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("NLST", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildSITE(const char* text, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("SITE", text, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildSYST(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("SYST", dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildStat(const char* pathname, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("STAT", pathname, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildHelp(const char* helpString, char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuild("HELP", helpString, dataBuffer, dataBufferSize);
}

PXSize PXFTPCommandBuildNoOperation(char* const dataBuffer, const PXSize dataBufferSize)
{
	return PXFTPCommandBuildBare("NOOP", dataBuffer, dataBufferSize);
}