
#include <stdio.h>

#include "TestSystemInfo.h"
#include "Graphic/TestWindow.h"
#include "Graphic/TestOpenGL.h"
#include "Graphic/TestFont.h"
#include "Compiler/TestPXCompiler.h"
#include "Graphic/TestImage.h"
#include "Compiler/TestYAML.h"
#include "Sound/TestSound.h"
#include "Network/TestSocket.h"
#include "Network/TestFTP.h"
#include "Network/TestSBP.h"

#include <Media/TIFF/PXTIFF.h>
#include <Media/PXImage.h>
#include <Media/PXText.h>

#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Process/PXProcess.h>
#include <OS/Debug/PXDebug.h>
#include <OS/File/PXDirectory.h>
#include <OS/Dialog/PXDialog.h>

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
}


int main()
{
	printf("[i] Starting testing...\n");

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



#if 1
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
