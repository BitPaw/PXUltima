#include "PXFTP.h"

#include <Media/PXText.h>

const char PXFTPUser[4] = { 'U', 'S', 'E', 'R' };
const char PXFTPPassword[4] = { 'P', 'A', 'S', 'S' };
const char PXFTPAccount[4] = { 'A', 'C', 'C', 'T' };
const char PXFTPCWD[4] = { 'C', 'W', 'D', '\0' };
const char PXFTPCDUP[4] = { 'C', 'D', 'U', 'P' };
const char PXFTPSMNT[4] = { 'S', 'M', 'N', 'T' };
const char PXFTPQuit[4] = { 'Q', 'U', 'I', 'T' };
const char PXFTPREIN[4] = { 'R', 'E', 'I', 'N' };
const char PXFTPPORT[4] = { 'P', 'O', 'R', 'T' };
const char PXFTPPASV[4] = { 'P', 'A', 'S', 'V' };
const char PXFTPTYPE[4] = { 'T', 'Y', 'P', 'E' };
const char PXFTPSTRU[4] = { 'S', 'T', 'R', 'U' };
const char PXFTPMODE[4] = { 'M', 'O', 'D', 'E' };
const char PXFTPRETR[4] = { 'R', 'E', 'T', 'R' };
const char PXFTPSTOR[4] = { 'S', 'T', 'O', 'R' };
const char PXFTPSTOU[4] = { 'S', 'T', 'O', 'U' };
const char PXFTPAPPE[4] = { 'A', 'P', 'P', 'E' };
const char PXFTPALLO[4] = { 'A', 'L', 'L', 'O' };
const char PXFTPREST[4] = { 'R', 'E', 'S', 'T' };
const char PXFTPRNFR[4] = { 'R', 'N', 'F', 'R' };
const char PXFTPRNTO[4] = { 'R', 'N', 'T', 'O' };
const char PXFTPABOR[4] = { 'A', 'B', 'O', 'R' };
const char PXFTPDelete[4] = { 'D', 'E', 'L', 'E' };
const char PXFTPRMD[4] = { 'R', 'M', 'D', '\0' };
const char PXFTPMKD[4] = { 'M', 'K', 'D', '\0' };
const char PXFTPPWD[4] = { 'P', 'W', 'D', '\0' };
const char PXFTPList[4] = { 'L', 'I', 'S', 'T' };
const char PXFTPNList[4] = { 'N', 'L', 'S', 'T' };
const char PXFTPSite[4] = { 'S', 'I', 'T', 'E' };
const char PXFTPSyst[4] = { 'S', 'Y', 'S', 'T' };
const char PXFTPStat[4] = { 'S', 'T', 'A', 'T' };
const char PXFTPHelp[4] = { 'H', 'E', 'L', 'P' };
const char PXFTPNoOperation[4] = { 'N', 'O', 'O', 'P' };

PXFTPResult PXAPI PXFTPResultParse(const unsigned short resultID)
{
    switch(resultID)
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

        case 200:
            return PXFTPResultOK;
        case 202:
            return PXFTPResultCommandNotImplemented;
        case 211:
            return PXFTPResultSystemStatus;
        case 212:
            return PXFTPResultDirectoryStatus;
        case 213:
            return PXFTPResultFileStatus;
        case 214:
            return PXFTPResultHelp;
        case 215:
            return PXFTPResultName;
        case 220:
            return PXFTPResultLoginReady;
        case 221:
            return PXFTPResultServiceClosed;
        case 225:
            return PXFTPResultServiceReady;
        case 227:
            return PXFTPResultPassiveModeNowActive;
        case 230:
            return PXFTPResultLoginCompleate;
        case 250:
            return PXFTPResultFileActionOK;
        case 257:
            return PXFTPResultPathNameCreated;


            //---------------------------------------------------------------------

            // 226 Closing data connection.Requested file action successful(for example, file                 transfer or file abort).
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
            // 226 Closing data connection.Requested file action successful(for example, file                                         transfer or file abort).
            // 227 Entering Passive Mode(h1, h2, h3, h4, p1, p2).
            // 230 User logged in, proceed.
            // 250 Requested file action okay, completed.
            // 257 "PATHNAME" created.
            //---------------------------------------------------------------------


            // 3xx
        case 331:
            return PXFTPResultLoginUserOKButPasswordMissing; // 331 User name okay, need password.
        case 332:
            return PXFTPResultLoginAccountNeeded; // 332 Need account for login.
        case 350:
            return PXFTPResultActionPending; // 350 Requested file action pending further information.
            //---------------------------------------------------------------------

            // 4xx
        case 421:
            return PXFTPResultServiceNotAvailable; // 421 Service not available, closing control connection.
        case 425:
            return PXFTPResultConnectionCantBeOpened; // 425 Can’t open data connection.
        case 426:
            return PXFTPResultConnectionClosed; // 426 Connection closed; transfer aborted.
            // 450 Requested file action not taken.File unavailable(e.g., file busy).
        case 451:
            return PXFTPResultActionAborted; // 451 Requested action aborted : local error in processing.
            // 452 Requested action not taken.Insufficient storage space in system.
            //---------------------------------------------------------------------

            // 5xx
        case 500:
            return PXFTPResultCommandInvalid; // 500 Syntax error, command unrecognized.    This may include errors such as command line too long.
        case 501:
            return PXFTPResultParameterSyntaxError; // 501 Syntax error in parameters or arguments.

            // 503 Bad sequence of commands.
            // 504 Command not implemented for that parameter.
            // 500 Syntax error, command unrecognized.This may include errors such as command line too long.
            // 501 Syntax error in parameters or arguments.
            // 502 Command not implemented.
            // 503 Bad sequence of commands.
            // 504 Command not implemented for that parameter.
        case 530:
            return PXFTPResultUserNotLoggedIn; // 530 Not logged in.
            // 532 Need account for storing files.
            // 550 Requested action not taken.File unavailable(e.g., file not found, no access).
            // 551 Requested action aborted : page type unknown.
        case 552:
            return PXFTPResultSpaceTooSmal; //  552 Requested file action aborted.Exceeded storage allocation(for current directory or dataset).
        case 553:
            return PXFTPResultFileNameRejected; // 553 Requested action not taken.File name not allowed.

        default:
            return PXFTPResultInvalid;
    }
}

PXSize PXAPI PXFTPCommandBuild(const PXFTPCommand pxFTPCommand, PXText* const pxText, const char* parameterA)
{
    const char printFormat[] = "%.4s %s\r\n";
    const char* commandText = ""; // Empty string, to later not print anything. NULL would be printed as "(null)"

    switch(pxFTPCommand)
    {
        case PXFTPCommandUser:
        {
            commandText = PXFTPUser;

            if(parameterA == PXNull)
            {
                parameterA = "anonymous";
            }

            break;
        }
        case PXFTPCommandPassword:
            commandText = PXFTPPassword;
            break;
        case PXFTPCommandAccount:
            commandText = PXFTPAccount;
            break;
        case PXFTPCommandWorkingDirectoryChange:
            commandText = PXFTPCWD;
            break;
        case PXFTPCommandDirectoryGoToParent:
            commandText = PXFTPCDUP;
            break;
        case PXFTPCommandSMNT:
            commandText = PXFTPSMNT;
            break;
        case PXFTPCommandQuit:
            commandText = PXFTPQuit;
            break;
        case PXFTPCommandREIN:
            commandText = PXFTPREIN;
            break;
        case PXFTPCommandPORT:
            commandText = PXFTPPORT;
            break;
        case PXFTPCommandPASV:
            commandText = PXFTPPASV;
            break;
        case PXFTPCommandTYPE:
            commandText = PXFTPTYPE;
            break;
        case PXFTPCommandSTRU:
            commandText = PXFTPSTRU;
            break;
        case PXFTPCommandMODE:
            commandText = PXFTPMODE;
            break;
        case PXFTPCommandRETR:
            commandText = PXFTPRETR;
            break;
        case PXFTPCommandSTOR:
            commandText = PXFTPSTOR;
            break;
        case PXFTPCommandStoreUnique:
            commandText = PXFTPSTOU;
            break;
        case PXFTPCommandAPPE:
            commandText = PXFTPAPPE;
            break;
        case PXFTPCommandALLO:
            commandText = PXFTPALLO;
            break;
        case PXFTPCommandREST:
            commandText = PXFTPREST;
            break;
        case PXFTPCommandFileRenameFrom:
            commandText = PXFTPRNFR;
            break;
        case PXFTPCommandFileRenameTo:
            commandText = PXFTPRNTO;
            break;
        case PXFTPCommandAbort:
            commandText = PXFTPABOR;
            break;
        case PXFTPCommandDelete:
            commandText = PXFTPDelete;
            break;
        case PXFTPCommandDirectoryRemove:
            commandText = PXFTPRMD;
            break;
        case PXFTPCommandMKD:
            commandText = PXFTPMKD;
            break;
        case PXFTPCommandDirectoryPrint:
            commandText = PXFTPPWD;
            break;
        case PXFTPCommandList:
            commandText = PXFTPList;
            break;
        case PXFTPCommandNLST:
            commandText = PXFTPNList;
            break;
        case PXFTPCommandSITE:
            commandText = PXFTPSite;
            break;
        case PXFTPCommandSYST:
            commandText = PXFTPSyst;
            break;
        case PXFTPCommandSTAT:
            commandText = PXFTPStat;
            break;
        case PXFTPCommandHELP:
            commandText = PXFTPHelp;
            break;
        case PXFTPCommandNOOP:
            commandText = PXFTPNoOperation;
            break;
        default:
            return PXActionRefusedArgumentInvalid;

    }

    return PXTextPrint(pxText, printFormat, commandText, parameterA);
}