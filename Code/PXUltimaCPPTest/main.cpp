#include <OS/File/PXFile.hpp>

#include <string>

int main()
{

	PX::File file;
	//file.OpenFromPath();

	const char* buffer = "Hello";

	PX::Text xx = PX::Text(buffer, sizeof(buffer));
	bool result = PX::File::DoesExist(xx);

	return 0;
}