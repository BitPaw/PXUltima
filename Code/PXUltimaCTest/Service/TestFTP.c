#include "TestFTP.h"

#include <Service/FTP/PXFTPClient.h>
#include <Media/PXText.h>
#include <Media/FTP/PXFTP.h>

#include <stdio.h>

void OnSocketDataReceiveFTP(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize)
{
	int returnCode = 0;

	PXText pxText;	
	PXTextConstructFromAdressA(&pxText, message, messageSize);

	PXSize parsedNumberSize = PXTextToInt(&pxText, &returnCode);

	if (parsedNumberSize)
	{
		const PXFTPResult pxFTPResult = PXFTPResultParse(returnCode);

		PXText command;
		PXTextConstructBufferA(&command, 1024);

		printf("[FTP] Code:%i Server:%s\n", returnCode, (char*)message + parsedNumberSize + 1);

		switch (pxFTPResult)
		{
			case PXFTPResultLoginReady:
			{
				PXFTPCommandBuild(PXFTPCommandUser, &command, PXNull);
				PXFTPCommandBuild(PXFTPCommandPassword, &command, "BitPaw");
				//PXFTPCommandBuildCurrentWorkDirectory();

				PXSocketSend(pxSocket, command.TextA, command.SizeUsed, 0);

				break;
			}
			case PXFTPResultLoginUserOKButPasswordMissing:
			{
				printf("[PX] Login ok\n");

				PXFTPCommandBuild(PXFTPCommandDirectoryPrint, &command, PXNull);

				PXSocketSend(pxSocket, command.TextA, command.SizeUsed, 0);

				break;
			}
			case PXFTPResultPathNameCreated:
			{
				PXFTPCommandBuild(PXFTPCommandSYST, &command, PXNull);

				PXSocketSend(pxSocket, command.TextA, command.SizeUsed, 0);

				break;
			}

			default:
				break;
		}		
	}
	else
	{
		printf("%s\n", message);
	}

	
}

void TestFTPAll()
{
	TestFTPClientConnectGeneral();
}

void TestFTPClientConnectGeneral()
{
	PXFTPClient client;

	PXClientConstruct(&client);

	client.Client.EventList.SocketDataReceiveCallBack = OnSocketDataReceiveFTP;

	const PXActionResult pxActionResult = PXFTPClientConnectToServer(&client, "127.0.0.1", FTPDefaultPort);

	while (1)
	{

	}
}