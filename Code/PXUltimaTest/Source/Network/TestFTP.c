#include "TestFTP.h"

#include <Network/Web/PXFTPClient.h>
#include <Text/PXText.h>
#include <Format/FTP/FTP.h>

#include <stdio.h>

void OnSocketDataReceiveFTP(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize)
{
	int returnCode = 0;

	PXSize parsedNumberSize = PXTextToIntA(message, messageSize, &returnCode);

	if (parsedNumberSize)
	{
		const PXFTPResult pxFTPResult = PXFTPResultParse(returnCode);

		char bufferThing[1024];
		PXSize bufferOffset = 0;

		printf("[FTP] Code:%i Server:%s\n", returnCode, (char*)message + parsedNumberSize + 1);

		switch (pxFTPResult)
		{
			case PXFTPResultLoginReady:
			{
				bufferOffset += PXFTPCommandBuildUser("anonymous", bufferThing + bufferOffset, bufferOffset);
				bufferOffset += PXFTPCommandBuildPassword("BitPaw", bufferThing + bufferOffset, bufferOffset);
				//PXFTPCommandBuildCurrentWorkDirectory();

				PXSocketSend(pxSocket, bufferThing, bufferOffset, 0);

				break;
			}
			case PXFTPResultLoginUserOKButPasswordMissing:
			{
				printf("[PX] Login ok\n");

				bufferOffset += PXFTPCommandBuildDirectoryPrint(bufferThing + bufferOffset, bufferOffset);

				PXSocketSend(pxSocket, bufferThing, bufferOffset, 0);

				break;
			}
			case PXFTPResultPathNameCreated:
			{
				bufferOffset += PXFTPCommandBuildSYST(bufferThing, bufferOffset);

				PXSocketSend(pxSocket, bufferThing, bufferOffset, 0);

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

	client.Client.EventListener.MessageReceiveCallback = OnSocketDataReceiveFTP;

	const PXActionResult pxActionResult = PXFTPClientConnectToServer(&client, "127.0.0.1", FTPDefaultPort);

	while (1)
	{

	}
}