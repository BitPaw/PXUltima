
#include <stdio.h>

#include "ImageTest.h"


int main()
{
	printf("[i] Starting testing...\n");

	TestSaveImageSmal();
	TestSaveImage();
	//ImageWriteText();

	printf("[i] Finished testing...\n");

	return 0;
}