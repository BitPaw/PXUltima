#include "TestFont.h"

#include <Media/PXFont.h>
#include <stdio.h>

void TestFontAll()
{
	TestFontParse();
}

void TestFontParse()
{
	PXFont font;

	PXText pxText;

	PXTextMakeFixedA(&pxText, "_TEST_DATA_INPUT_/A.fnt");

	const PXActionResult fontLoadResult = PXFontLoad(&font, &pxText);

	printf("");
}