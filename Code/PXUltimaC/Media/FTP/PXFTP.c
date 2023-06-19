#include "PXFTP.h"

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

PXSize PXFTPCommandBuild(const PXFTPCommand pxFTPCommand, PXText* const pxText, const char* const parameterA)
{
	const char syntaxDouble[] = "%s %s\r\n"; 
	const char syntaxSingle[] = "%s \r\n";

	switch (pxFTPCommand)
	{
		case PXFTPCommandUser:
			return PXTextPrint(pxText, syntaxDouble, "USER", parameterA);

		case PXFTPCommandPassword: // PASS
			return PXTextPrint(pxText, syntaxDouble, "PASS", parameterA);

		case PXFTPCommandAccount: // ACCT
			return PXTextPrint(pxText, syntaxDouble, "ACCT", parameterA);
			
		case PXFTPCommandWorkingDirectoryChange: // CWD
			return PXTextPrint(pxText, syntaxDouble, "CWD", parameterA);

		case PXFTPCommandDirectoryGoToParent: // CDUP
			return PXTextPrint(pxText, syntaxSingle, "CDUP");

		case PXFTPCommandSMNT: // SMNT
			return PXTextPrint(pxText, syntaxDouble, "SMNT", parameterA);

		case PXFTPCommandQuit: // QUIT
			return PXTextPrint(pxText, syntaxSingle, "QUIT");

		case PXFTPCommandREIN: // REIN
			return PXTextPrint(pxText, syntaxSingle, "REIN");

		case PXFTPCommandPORT:
			return PXTextPrint(pxText, syntaxDouble, "PORT", parameterA);

		case PXFTPCommandPASV:
			return PXTextPrint(pxText, syntaxSingle, "PASV");

		case PXFTPCommandTYPE:
			return PXTextPrint(pxText, syntaxDouble, "TYPE", parameterA);

		case PXFTPCommandSTRU:
			return PXTextPrint(pxText, syntaxDouble, "STRU", parameterA);

		case PXFTPCommandMODE:
			return PXTextPrint(pxText, syntaxDouble, "MODE", parameterA);

		case PXFTPCommandRETR:
			return PXTextPrint(pxText, syntaxDouble, "RETR", parameterA);

		case PXFTPCommandSTOR:
			return PXTextPrint(pxText, syntaxDouble, "STOR", parameterA);

		case PXFTPCommandStoreUnique:
			return PXTextPrint(pxText, syntaxSingle, "STOU");

		case PXFTPCommandAPPE:
			return PXTextPrint(pxText, syntaxDouble, "APPE", parameterA);

		case PXFTPCommandALLO:
			return PXTextPrint(pxText, syntaxDouble, "ALLO", parameterA);

		case PXFTPCommandREST:
			return PXTextPrint(pxText, syntaxDouble, "REST", parameterA);

		case PXFTPCommandFileRenameFrom: // RNFR
			return PXTextPrint(pxText, syntaxDouble, "RNFR", parameterA);

		case PXFTPCommandFileRenameTo: // RNTO
			return PXTextPrint(pxText, syntaxDouble, "RNTO", parameterA);

		case PXFTPCommandAbort: // ABOR
			return PXTextPrint(pxText, syntaxSingle, "ABOR");

		case PXFTPCommandDelete: // DELE
			return PXTextPrint(pxText, syntaxDouble, "DELE", parameterA);

		case PXFTPCommandDirectoryRemove:
			return PXTextPrint(pxText, syntaxDouble, "RMD", parameterA);

		case PXFTPCommandMKD:
			return PXTextPrint(pxText, syntaxDouble, "MKD", parameterA);

		case PXFTPCommandDirectoryPrint:
			return PXTextPrint(pxText, syntaxSingle, "PWD");

		case PXFTPCommandList:

			//return PXFTPCommandBuildBare("NLST", pxText);

			return PXTextPrint(pxText, syntaxDouble, "LIST", parameterA);

		case PXFTPCommandNLST:
			return PXTextPrint(pxText, syntaxDouble, "NLST", parameterA);

		case PXFTPCommandSITE:
			return PXTextPrint(pxText, syntaxDouble, "SITE", parameterA);

		case PXFTPCommandSYST:
			return PXTextPrint(pxText, syntaxSingle, "SYST");

		case PXFTPCommandSTAT:
			return PXTextPrint(pxText, syntaxDouble, "STAT", parameterA);

		case PXFTPCommandHELP:
			return PXTextPrint(pxText, syntaxDouble, "HELP", parameterA);

		case PXFTPCommandNOOP:
			return PXTextPrint(pxText, syntaxSingle, "NOOP");

		case PXFTPCommandInvalid:
		default:
			return 0;
	}
}