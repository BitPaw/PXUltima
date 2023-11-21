//#include <OS/File/PXFile.hpp>
//#include <OS/Graphic/OpenGL/PXOpenGL.hpp>
#include <OS/Async/PXProcess.h>
#include <OS/Library/PXLibrary.h>

//#include <string>
#include <stdio.h>

#include <Container/BTree/PXBTree.hpp>

void OnPXSymbolDetected(const PXSymbol* const pxSymbol)
{
	if (20000 >= pxSymbol->TypeIndex)
	{
		return;
	}

	printf("| %5i | %5i | 0x%p | 0x%p |%5zi | %-35s |\n", pxSymbol->Index, pxSymbol->TypeIndex, pxSymbol->ModBase, pxSymbol->Address, pxSymbol->Size, pxSymbol->Name.TextA);
}


void OnPXProcessDetected(PXProcess* const pxProcess)
{
	printf("| %5i | %ls |\n", pxProcess->ProcessID, pxProcess->ExecutableFilePath.TextW);
}

int main()
{









	PXProcess pxProcess;



	PXActionResult result = PXProcessListAll(OnPXProcessDetected);

	while (1)
	{

	}


	PXText pxText;
	PXTextMakeFixedA(&pxText, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\[Export]\\PXUltimaC\\32B-Windows-Debug\\PXUltimaC.dll");
	//PXTextMakeFixedA(&pxText, "c:\\windows\\system32\\user32.dll");

	PXLibraryParseSymbols(&pxText, OnPXSymbolDetected);



	while (1)
	{

	}


	PX::BTree<int, int> btree(2);
	btree.Insert(1, 5);




	PXActionResult pxActionResult = PXProcessOpenViaID(&pxProcess, 18368);

	const void* x = (void*)0x005AF85C;

	char testText[] = "Ding";

	PXActionResult resss = PXProcessMemoryWrite(&pxProcess, x, testText, sizeof(testText));

	while (1)
	{

	}	
	/*

	PX::File file;
	//file.OpenFromPath();

	const char* buffer = "Hello";

	PX::Text xx = PX::Text(buffer, sizeof(buffer));
	bool result = PX::File::DoesExist(xx);


	PX::OpenGL opengl;
	opengl.CreateForWindow();

	*/
	return 0;
}