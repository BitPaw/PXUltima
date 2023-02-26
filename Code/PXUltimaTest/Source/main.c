
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

#include <Format/TIFF/TIFF.h>
#include <Format/Image.h>
#include <Text/PXText.h>

#include <OS/Graphic/DirectX/DirectX.h>
#include <OS/Process/PXProcess.h>
#include <OS/Debug/PXDebug.h>

int main()
{
	printf("[i] Starting testing...\n");

#if 0
	char buffer[64];

	for (size_t i = 0; i <= 0xFF; i++)
	{
		TextFromIntToBinary64U(buffer, 64, i);

		printf("[%4i] %s\n", i, buffer);
	}
#endif
	
#if 1
	PXTestDebugAll();
#endif // 1


#if 0
	TestSoundAll();
#endif

#if 0
	TestFontAll();
#endif // 

#if 0 // OpenGL Test
	TestOpenGLAll();
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





	printf("[i] Finished testing...\n");

	return 0;
}
