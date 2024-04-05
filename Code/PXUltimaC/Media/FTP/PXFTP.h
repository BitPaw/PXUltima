#ifndef PXFTPINCLUDE
#define PXFTPINCLUDE

#include "../PXResource.h"

#define FTPDefaultPort 21

typedef enum PXFTPCommand_
{
	PXFTPCommandInvalid,
	PXFTPCommandUser,
	PXFTPCommandPassword, // PASS
	PXFTPCommandAccount, // ACCT
	PXFTPCommandWorkingDirectoryChange, // CWD
	PXFTPCommandDirectoryGoToParent, // CDUP
	PXFTPCommandSMNT, // SMNT
	PXFTPCommandQuit, // QUIT
	PXFTPCommandREIN, // REIN
	PXFTPCommandPORT,
	PXFTPCommandPASV,
	PXFTPCommandTYPE,
	PXFTPCommandSTRU,
	PXFTPCommandMODE,
	PXFTPCommandRETR,
	PXFTPCommandSTOR,
	PXFTPCommandStoreUnique,
	PXFTPCommandAPPE,
	PXFTPCommandALLO,
	PXFTPCommandREST,
	PXFTPCommandFileRenameFrom, // RNFR
	PXFTPCommandFileRenameTo, // RNTO
	PXFTPCommandAbort, // ABOR
	PXFTPCommandDelete, // DELE
	PXFTPCommandDirectoryRemove,
	PXFTPCommandMKD,
	PXFTPCommandDirectoryPrint,
	PXFTPCommandList,
	PXFTPCommandNLST,
	PXFTPCommandSITE,
	PXFTPCommandSYST,
	PXFTPCommandSTAT,
	PXFTPCommandHELP,
	PXFTPCommandNOOP,
}
PXFTPCommand;

typedef enum PXFTPResult_
{
	PXFTPResultInvalid,

	// 1xx
	// 110 Restart marker reply.
	// 120 Service ready in nnn minutes.
	// 125 Data connection already open; transfer starting.
	// 150 File status okay; about to open data connection.
	// 120 Service ready in nnn minutes.
	// 150 File status okay; about to open data connection.

	//---------------------------------------------------------------------

	// 2xx

	PXFTPResultOK, // 200 Command okay.
	PXFTPResultCommandNotImplemented, // 202 Command not implemented, superfluous at this site.
	PXFTPResultSystemStatus, // 211 System status, or system help reply.
	PXFTPResultDirectoryStatus, // 212 Directory status.
	PXFTPResultFileStatus, // 213 File status.
	PXFTPResultHelp, // 214 Help message.
	PXFTPResultName, // 215 NAME system type.
	PXFTPResultLoginReady, // 220 Service ready for new user.
	PXFTPResultServiceClosed, // 221 Service closing control connection.Logged out if appropriate.
	PXFTPResultServiceReady, // 225 Data connection open; no transfer in progress.
	PXFTPResultPassiveModeNowActive,// 227 Entering Passive Mode(h1, h2, h3, h4, p1, p2).
	PXFTPResultLoginCompleate,// 230 User logged in, proceed.
	PXFTPResultFileActionOK,// 250 Requested file action okay, completed.
	PXFTPResultPathNameCreated,// 257 "PATHNAME" created. 
	//---------------------------------------------------------------------


	// 3xx
	PXFTPResultLoginUserOKButPasswordMissing, // 331 User name okay, need password.
	PXFTPResultLoginAccountNeeded, // 332 Need account for login.
	PXFTPResultActionPending, // 350 Requested file action pending further information.
	//---------------------------------------------------------------------

	// 4xx
	PXFTPResultServiceNotAvailable, // 421 Service not available, closing control connection.
	PXFTPResultConnectionCantBeOpened, // 425 Can’t open data connection.
	PXFTPResultConnectionClosed, // 426 Connection closed; transfer aborted.
	// 450 Requested file action not taken.File unavailable(e.g., file busy).
	PXFTPResultActionAborted, // 451 Requested action aborted : local error in processing.
	// 452 Requested action not taken.Insufficient storage space in system.
	//---------------------------------------------------------------------

	// 5xx
	PXFTPResultCommandInvalid, // 500 Syntax error, command unrecognized.	This may include errors such as command line too long.
	PXFTPResultParameterSyntaxError, // 501 Syntax error in parameters or arguments.

	// 503 Bad sequence of commands.
	// 504 Command not implemented for that parameter.
	// 500 Syntax error, command unrecognized.This may include errors such as command line too long.
	// 501 Syntax error in parameters or arguments.
	// 502 Command not implemented.
	// 503 Bad sequence of commands.
	// 504 Command not implemented for that parameter.
	PXFTPResultUserNotLoggedIn, // 530 Not logged in.
	// 532 Need account for storing files.
	// 550 Requested action not taken.File unavailable(e.g., file not found, no access).
	// 551 Requested action aborted : page type unknown.
	PXFTPResultSpaceTooSmal, //  552 Requested file action aborted.Exceeded storage allocation(for current directory or dataset).
	PXFTPResultFileNameRejected, // 553 Requested action not taken.File name not allowed.
	//---------------------------------------------------------------------

}
PXFTPResult;

// USER <SP> <username> <CRLF>
// 	PASS <SP> <password> <CRLF>
// ACCT <SP> <account - information> <CRLF>
// CWD <SP> <pathname> <CRLF>
// CDUP <CRLF>
// SMNT <SP> <pathname> <CRLF>
// QUIT <CRLF>
// REIN <CRLF>
// PORT <SP> <host - port> <CRLF>
// PASV <CRLF>
// TYPE <SP> <type - code> <CRLF>
// STRU <SP> <structure - code> <CRLF>
// MODE <SP> <mode - code> <CRLF>
// RETR <SP> <pathname> <CRLF>
// STOR <SP> <pathname> <CRLF>
// STOU <CRLF>
// APPE <SP> <pathname> <CRLF>
// ALLO <SP> <decimal - integer>	[<SP> R <SP> <decimal - integer>] <CRLF>
// REST <SP> <marker> <CRLF>
// RNFR <SP> <pathname> <CRLF>
// RNTO <SP> <pathname> <CRLF>
// ABOR <CRLF>
// DELE <SP> <pathname> <CRLF>
// RMD <SP> <pathname> <CRLF>
// MKD <SP> <pathname> <CRLF>
// PWD <CRLF>
// LIST[<SP> <pathname>] <CRLF>
// NLST[<SP> <pathname>] <CRLF>
// SITE <SP> <string> <CRLF>
// SYST <CRLF>
// STAT[<SP> <pathname>] <CRLF>
// HELP[<SP> <string>] <CRLF>
// NOOP <CRLF>

PXPublic PXFTPResult PXAPI PXFTPResultParse(const unsigned short resultID);

PXPublic PXSize PXAPI PXFTPCommandBuild(const PXFTPCommand pxFTPCommand, PXText* const pxText, const char* const parameterA);

#endif