
#include <stdio.h>

#define PXLDAPTest 0
#define PXKeyBoardVirtualTest 0

#include "OS/TestSystemInfo.h"
#include "Graphic/TestWindow.h"
#include "Graphic/TestOpenGL.h"
#include "Graphic/TestFont.h"
#include "Compiler/TestPXCompiler.h"
#include "Graphic/TestImage.h"
#include "Compiler/TestYAML.h"
#include "Sound/TestSound.h"
#include "OS/TestSocket.h"
#include "Service/TestFTP.h"
#include "Service/TestSBP.h"

#include <Media/PXFont.h>
#include <Media/TTF/PXTTF.h>



#include <OS/Graphic/DirectX/PXDirectX.h>


#if PXLDAPTest
#include <Service/LDAP/PXLDAPClient.h>

#endif // PXLDAPTest


#if PXKeyBoardVirtualTest
#include <OS/Hardware/PXKeyBoard.h>
#endif // PXKeyBoardVirtualTest
#include <OS/Window/PXWindow.h>

/*

void OnFileElementDetected(PXFileElementInfo* pxFileElementInfo)
{
	switch (pxFileElementInfo->Type)
	{
		case PXFileElementInfoTypeInvalid:
		case PXFileElementInfoTypeFile:
		{
			printf("| %-4s | %-20ls | %i |\n", "File", pxFileElementInfo->Name, pxFileElementInfo->Size);		

			break;
		}
		case PXFileElementInfoTypeDictionary:
		{
			printf("| %-4s | %-20ls |\n", "DIR", pxFileElementInfo->Name);
			break;
		}

		default:
			break;
	}


}



void PXTextMatchTest()
{
	const PXBool a = PXTextMatchW(L"MyFile.h", 8, L"*.h", 3);
	printf("");

	const PXBool b = PXTextMatchW(L"MyFile.c", 8, L"*.h", 3);
	printf("");

	const PXBool c = PXTextMatchW(L"MyFile.*", 8, L"*.h", 3);
	printf("");
}*/


int main()
{
	printf("[i] Starting testing...\n");


	// Direct X - Test
	PXWindow pxWindow;
	PXWindowConstruct(&pxWindow);

	PXWindowCreateA(&pxWindow, -1, -1, "Direct X - Test", PXTrue);

	PXAwaitChangeCU(&pxWindow.IsRunning);


	PXDirectX* directX = &pxWindow.GraphicInstance.DirectXInstance;



	const float vertices[] =
	{
		 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, // x, y, z, rhw, color
		 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00,
		  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff,
	};


	PXVertexBuffer pxVertexBuffer;

	pxVertexBuffer.VertexData = vertices;
	pxVertexBuffer.VertexDataSize = sizeof(vertices);// / sizeof(float);
	pxVertexBuffer.Format = PXVertexBufferFormatXYZUXC;

	PXDirectXVertexBufferCreate(directX, &pxVertexBuffer);


	while (1)
	{
		PXDirectXClear(directX, 0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(30, 30, 30), 1.0f, 0);

		PXDirectXSceneBegin(directX);

		PXDirectXStreamSourceSet(directX, 0, &pxVertexBuffer, 0, 20);
		PXDirectXVertexFixedFunctionSet(directX, pxVertexBuffer.Format);
		PXDirectXPrimitiveDraw(directX, PXGraphicRenderModeTriangle, 0, 1);

		PXDirectXSceneEnd(directX);

		PXDirectXPresent(directX, 0, 0, 0, 0);
	}

	PXWindowDestruct(&pxWindow);


	printf("\n");















	PXTTF ttf;

	PXFile pxFile;

	PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
	PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, "C:\\Windows\\Fonts\\arial.ttf");

	pxFileOpenFromPathInfo.FileSize = 0;
	pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
	pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
	pxFileOpenFromPathInfo.AllowMapping = PXTrue;
	pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
	pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

	PXFileOpenFromPath(&pxFile, &pxFileOpenFromPathInfo);

	PXActionResult xx = PXTTFParse(&ttf, &pxFile);

	printf("");

#if 0

	vswprintf_s;

	PXText filePath;
	PXTextMakeFixedA(&filePath, "_TEST_DATA_INPUT_/A.fnt");

	PXSpriteFont pxSpriteFont;
	PXActionResult result = PXFontLoad(&pxSpriteFont, &filePath);

	printf("OK\n");

#endif



#if PXLDAPTest

	printf("Begin");

	PXLDAPClient pxLDAP;
	PXLDAPClientConstruct(&pxLDAP);

	PXLDAPConnectionInfo pxLDAPConnectionInfo;
	pxLDAPConnectionInfo.ConnectionOriented = PXTrue;
	PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.Host, Host, "ipa.demo1.freeipa.org");
	PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.ConnectionDomain, ConnectionDomain, "uid=admin,cn=users,cn=accounts,dc=demo1,dc=freeipa,dc=org");
	PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.AuthenticationCredentials, AuthenticationCredentials, "Secret123");
	pxLDAPConnectionInfo.Port = PXLDAPPortDefault;
	pxLDAPConnectionInfo.SSLUse = 0;
	pxLDAPConnectionInfo.ConnectTimeout = 2000;
	pxLDAPConnectionInfo.AuthenticationMethod = PXLDAPAuthenticationMethodInvalid;

	PXActionResult openResult = PXLDAPClientOpen(&pxLDAP, &pxLDAPConnectionInfo);
	

	PXLDAPSearchInfo pxLDAPSearchInfo;
	PXTextMakeFixedNamedA(&pxLDAPSearchInfo.EntryName, EntryName, "dc=demo1,dc=freeipa,dc=org");
	PXTextMakeFixedNamedA(&pxLDAPSearchInfo.Filter, Filter, "(objectclass=person)");
	pxLDAPSearchInfo.Async = PXFalse;
	pxLDAPSearchInfo.OnlyTypesRequired = PXFalse;
	pxLDAPSearchInfo.AttributeList = PXNull;
	pxLDAPSearchInfo.Scope = PXLDAPScopeSubTree;

	const PXActionResult searchResult = PXLDAPClientSearch(&pxLDAP, &pxLDAPSearchInfo);
	const PXBool successful = PXActionSuccessful == searchResult;

	if (successful)
	{
		

		for (LDAPMessage* msg = ldap_first_entry(pxLDAP.ID, pxLDAP.SearchResult); msg; msg = ldap_next_entry(pxLDAP.ID, msg))
		{
			ULONG ReturnCode;                            // returned by server
			PSTR MatchedDNs;         // free with ldap_memfree
			PSTR ErrorMessage;       // free with ldap_memfree
			PZPSTR Referrals;        // free with ldap_value_freeA
			PLDAPControlA* ServerControls;               // free with ldap_free_controlsA
			const ULONG result = ldap_parse_resultA(pxLDAP.ID, msg, &ReturnCode, &MatchedDNs,&ErrorMessage,&Referrals, &ServerControls, PXFalse);
			const PXBool success = LDAP_SUCCESS == result;

			if (success)
			{
				BerElement* berElement = 0;
				
				printf("[%p] ID:%i Type:%i\n", msg, msg->lm_msgid, msg->lm_msgtype);

				for (char* attribute = ldap_first_attributeA(pxLDAP.ID, msg, &berElement); attribute; attribute = ldap_next_attributeA(pxLDAP.ID, msg, berElement))
				{

					struct berval** calxx = ldap_get_values_lenA(pxLDAP.ID, msg, attribute);
					const PXBool valueFetchSuccess = PXNull != calxx;

					if (valueFetchSuccess)
					{
						printf("\t- %-25s : %-25s\n", attribute, (*calxx)->bv_val);
					
					}
					else
					{
						printf("\t- %-25s : %-25s\n", attribute, 0);
					}				
				}

				printf("\n\n");
			}
			else
			{
				printf("[%p] Error\n", msg);
			}

			
		}
	}


	PXLDAPClientDestruct(&pxLDAP);

	printf("END");

#endif 


#if PXKeyBoardVirtualTest
	PXKeyBoardVirtualInput inputList[4];

	PXKeyBoardVirtualInputSet(&inputList[0], KeyWorld1, PXKeyPressStateDown);
	PXKeyBoardVirtualInputSet(&inputList[1], KeyD, PXKeyPressStateDown);

	PXKeyBoardVirtualInputSet(&inputList[2], KeyD, PXKeyPressStateUp);
	PXKeyBoardVirtualInputSet(&inputList[3], KeyWorld1, PXKeyPressStateUp);
	
	PXKeyBoardVirtualInsertAction(&inputList, 4);
#endif // 0










#if 0
	PXTextMatchTest();

	//PXActionResult res = PXDirectoryFilesInFolderW(L"B:/Daten/Bilder/*", OnFileElementDetected, 0x02, 0);
	//res = PXDirectoryFilesInFolderW(L"B:/Daten/Bilder/*png", OnFileElementDetected, 0x01, 0);


	printf("");



	char buffer[64];

	for (size_t i = 0; i <= 0xFF; i++)
	{
		TextFromIntToBinary64U(buffer, 64, i);

		printf("[%4i] %s\n", i, buffer);
	}
#endif
	

#if 0 // TEST FILE DIALOG

	PXText pxText;
	PXTextMakeFixedA(&pxText, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code");

	PXDialogFileOpen(&pxText);
	PXDialogFileSave(&pxText);

	PXColorRGBI8 pxColorRGBI8;

	PXDialogColorSelect(&pxColorRGBI8);
	PXDialogFontSelect();
	PXDialogPrint();

#endif // 0 // TEST FILE DIALOG


#define KeyBoardTest 0
#if KeyBoardTest

	PXWindow pxWindow;
	PXText pxText;
	PXTextMakeFixedA(&pxText, "Window input Test");

	PXWindowConstruct(&pxWindow);
	PXWindowCreate(&pxWindow, 600, 400, &pxText, PXTrue);

	while (1)
	{
		printf("\033[H\033[J");
		printf("\033[%d;%dH", 0, 0);
		PXKeyBoardInputPrint(&pxWindow.KeyBoardCurrentInput);
		PXThreadSleep(0, 100);
	}

#endif // 0



#if 0
	while (1)
	{
		const PXInt32U temperature = PXProcessorTemperature();

		printf("CPU Temp : %i\n", temperature);

		PXThreadSleep(0, 100);
	}
#endif // 1



#if 0
	PXImage image;

	//ImageLoadTest(&image, "C:/Users/BitPaw/Videos/SquareBlue.bmp");
	ImageLoadTest(&image, "C:/Users/BitPaw/Videos/SquareBlue.png");

	printf("EE\n");
	//TestImageAll();
#endif // 0


#if 0
	PXTestDebugAll();
#endif // 1


#if 0
	TestSoundAll();
#endif

#if 0
	TestFontAll();
#endif // 

#if 0 // PXOpenGL Test
	TestPXOpenGLAll();
#endif


#if 0
	TestSocket();
#endif // 1


#if 1
	TestSBPAll();
#endif // 1


#if 0
	TestFTPAll();
#endif // 0



	//Image image;

	//ImageLoadA(&image, "D:/_Data/Git/PXUltima/PXUltimaTest/_TEST_DATA_INPUT_/ImageInput.bmp");

	//ImageLoadA(&image, "A:/_WorkSpace/Download/at3_1m4_03.tif");

	//TestYAMLAll();
	//TestSystemInfoAll();
	//TestPXCompilerAll();
	//
	//TestWindowAll();
	//TestImageAll();
	// 
	// 
	// 
	//TestSBPAll();





	printf("[i] Finished testing...\n");

	return 0;
}
