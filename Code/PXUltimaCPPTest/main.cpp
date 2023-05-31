#include <OS/File/PXFile.hpp>
#include <OS/Graphic/OpenGL/PXOpenGL.hpp>

#include <string>

int main()
{

	PX::File file;
	//file.OpenFromPath();

	const char* buffer = "Hello";

	PX::Text xx = PX::Text(buffer, sizeof(buffer));
	bool result = PX::File::DoesExist(xx);


	PX::OpenGL opengl;
	opengl.CreateForWindow();


	return 0;
}