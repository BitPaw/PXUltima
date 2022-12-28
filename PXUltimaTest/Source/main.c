
#include <stdio.h>

#include "TestSystemInfo.h"
#include "TestWindow.h"
#include "TestOpenGL.h"
#include "TestPXCompiler.h"
#include "TestImage.h"
#include "TestYAML.h"
#include "TestSound.h"
#include "TestFont.h"

#include <Graphic/DirectX/DirectX.h>

#include <OS/PXProcess.h>
#include <Format/TIFF/TIFF.h>
#include <Format/Image.h>

int main()
{
	printf("[i] Starting testing...\n");

#if 1
	TestFontAll();
#endif // 



	//Image image;

	//ImageLoadA(&image, "D:/_Data/Git/PXUltima/PXUltimaTest/_TEST_DATA_INPUT_/ImageInput.bmp");

	//ImageLoadA(&image, "A:/_WorkSpace/Download/at3_1m4_03.tif");

	//TestYAMLAll();
	//TestSystemInfoAll();
	//TestPXCompilerAll();
	//TestOpenGLAll();
	//TestWindowAll();
	//TestImageAll();
	// 
	// 
	// 
	//TestSBPAll();


#if 1
	TestSoundAll();
#endif


	printf("[i] Finished testing...\n");

	return 0;
}
