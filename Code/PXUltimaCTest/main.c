#include <OS/Console/PXConsole.h>

#define PXLDAPTest 0
#define PXKeyBoardVirtualTest 0

int _fltused = 0;

#if 1

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "Comctl32.lib")
#endif


#if 0
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
#endif

#include "Network/PXNetworkTest.h"
#include "Window/PXWindowTest.h"

#include <OS/Console/PXConsole.h>
#include <Media/FastFile/PXFastFile.h>
#include <Media/C/PXC.h>
#include <Media/PXDocument.h>

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

#include "Compiler/PXTestCompiler.h"
#include "Debug/PXTestDebug.h"
#include "Media/PXMediaTest.h"
#include "Window/PXWindowTest.h"

#include <OS/Hardware/PXHardware.h>
#include <OS/Hardware/PXVideo.h>
#include <OS/Hardware/PXProcessor.h>
#include <Math/PXMath.h>

int main()
{
	PXConsoleWrite(0, "[i] Starting testing...\n");





	for(size_t i = 0; i < 128; i++)
	{
		float x = 0xAABBCCDD;// *0.0025f;

		PXConsoleWriteF(0, "[%i] %8.8X\n", i, x);

		x = PXMathSinusF(x);

		PXConsoleWriteF(0, "[%i] %6.4f\n", i, x);
	}






	PXProcessor pxProcessor;

	PXProcessorFetchInfo(&pxProcessor);

	PXVideoCaptureDeviceList();





	
	PXResourceLoadInfo pxResourceLoadInfo;
	pxResourceLoadInfo.Manager = 0;
	pxResourceLoadInfo.Target = 0;
	pxResourceLoadInfo.FileReference = PXNull;
	pxResourceLoadInfo.Type = 0;


	PXResourceLoadA(&pxResourceLoadInfo, "H:\\S.n64");


#if 0
	PXHardwareInfo pxHardwareInfo;
	PXHardwareInfoScan(&pxHardwareInfo, PXHardwareInfoAll);

	while(1)
	{
		PXInt32S temperature = 0;



		//PXProcessorTemperature(&temperature);


	}
#endif

	PXTestCompilerAll();

	PXTestWindowAll();
	PXMediaTestAll();
	
	PXTestDebugAll();






#if 0

	PXResourceLoadInfo pxResourceLoadInfo;
	pxResourceLoadInfo.Target = 0;
	pxResourceLoadInfo.FileReference = 0;
	pxResourceLoadInfo.Type = PXGraphicResourceTypeInvalid;


	PXResourceLoadA(&pxResourceLoadInfo, "N:\\NAS\\Games\\Steam\\steamapps\\common\\Call of Duty Black Ops II\\zone\\all\\afghanistan.ff");

#endif // 1



#if 0
	PXTestWindowAll();
#endif // 0

#if 0
	PXTestNetworkAll();
#endif // 0






#if 0
	{
		PXBinaryWindows binaryWindows;

		const PXActionResult result = PXResourceLoadA(&binaryWindows, "C:\\Data\\WorkSpace\\[GIT]\\BitFireEngine\\[Export]\\GameCleaved\\32B-Windows-Release\\GameCleaved2K.exe");

		printf("\n");
	}
#endif // 1




#if 0
	{
		PXImage pxImage;
		PXClear(PXImage, &pxImage);

		const PXActionResult pxLoadResult = PXResourceLoadA(&pxImage, "_TEST_DATA_INPUT_\\ImageInput.bmp");
		const PXActionResult pxSaveResult = PXResourceSaveA(&pxImage, "_TEST_DATA_INPUT_\\ImageInput_COPY.bmp", PXFileFormatBitMap);

		printf("\n");
	}
#endif // 1


#if 0 // XML -> Document -> Image
	{
		PXKnowlegeGraph pxKnowlegeGraph;
		PXDocument pxDocument;
		PXImage pxImage;

		const PXActionResult pxLoadResult = PXResourceLoadA(&pxDocument, "_TEST_DATA_INPUT_\\books.xml");

		const PXActionResult pxGraphResult = PXKnowlegeGraphLoadAndBuild(&pxKnowlegeGraph, &pxDocument, &pxImage);

		const PXActionResult pxSaveResult = PXResourceSaveA(&pxImage, "_TEST_DATA_INPUT_\\books.bmp", PXFileFormatBitMap);

		printf("\n");

		return 0;
	}
#endif // 1 // XML -> Document -> Image





#if 0
	PXFile pxFile;

	PXResourceLoadA(&pxFile, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\PXUltimaC\\Media\\PXImage.h");
#endif // 0




#if 0
	PXBinaryWindows pxBinaryWindows;

	PXResourceLoadA(&pxBinaryWindows, "C:\\Windows\\System32\\user32.dll");
#endif // 0


	
#if 0
	while (1)
	{
		PXInt32U temp;

		PXProcessorTemperature(&temp);

		PXonsoleGoToXY(0,0);
		printf("CPU temp : %i\n", temp);

	}
#endif


#if 0
	TestSoundAll();
#endif // 0

	


#if 0
	
	PXImage pxImage;

	PXResourceLoadA(&pxImage, "N:\\IMG_0147.CR3");
#endif


















	//PXBinaryLinux pxELF;
	//PXResourceLoadA(&pxELF, "N:\\NAS\\Games\\PC\\Re-Volt_Linux\\rvgl.64.elf");

#if 0
	
	PXBinaryWindows pxEXE;
	//PXResourceLoadA(&pxEXE, "C:/Data/WorkSpace/[GIT]/PXUltima/Code/[Export]/PXUltimaCTest/32B-Windows-Debug/PXUltimaCTest.exe");
	PXResourceLoadA(&pxEXE, "C:/Data/WorkSpace/[GIT]/PXUltima/Code/[Export]/PXUltimaCTest/64B-Windows-Debug/PXUltima.dll");
#endif


#if 0
	// X86
	{
		PXInt32U coolNumber = 0xAABBCCDD;

		//PXEndianSwapI32U(&coolNumber);

		PXBool result = coolNumber == 0xDDCCBBAA;

		printf("Hello, \n");

	}
#endif




	



#if 0
	// Direct X - Test
	PXWindow pxWindow;
	PXWindowConstruct(&pxWindow);

	PXWindowCreateA(&pxWindow, -1, -1, "Direct X - Test", PXTrue);

	PXAwaitChangeCU(&pxWindow.IsRunning);


	const float vertices[] =
	{
#if 1
		-0.5f, -0.5f, 1, 0xffff0000, // x, y, z, rhw, color
		0.0f,  0.5f, 1, 0xff00ffff,
		 0.5f, -0.5f, 1, 0xff00ff00
	

#else 
		50.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, // x, y, z, rhw, color
		250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00,
		50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff,
#endif
};

	PXGraphicSelect(&pxWindow.GraphicInstance);


	PXModel pxModel;
	PXObjectClear(PXModel, &pxModel);
	pxModel.VertexBuffer.VertexData = vertices;
	pxModel.VertexBuffer.VertexDataSize = sizeof(vertices);// / sizeof(float);
	pxModel.VertexBuffer.VertexDataRowSize = sizeof(vertices) / 3;
	pxModel.VertexBuffer.Format = PXVertexBufferFormatXYZC; // PXVertexBufferFormatXYZC  PXVertexBufferFormatXYZHWC

	PXGraphicModelRegister(&pxWindow.GraphicInstance, &pxModel);

	while (1)
	{
		const PXColorRGBAF pxColorRGBAF = { 0.3f,0.3f,0.3f,1.0f };

		PXGraphicClear(&pxWindow.GraphicInstance, &pxColorRGBAF);

		PXGraphicSceneBegin(&pxWindow.GraphicInstance);

		PXGraphicModelDraw(&pxWindow.GraphicInstance, &pxModel);

		PXGraphicSceneEnd(&pxWindow.GraphicInstance);
		PXGraphicSceneDeploy(&pxWindow.GraphicInstance);
	}

	PXWindowDestruct(&pxWindow);


	printf("\n");



#endif




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


#if 0
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





	PXConsoleWrite(0, "[i] Finished testing...\n");

	return 0;
}
