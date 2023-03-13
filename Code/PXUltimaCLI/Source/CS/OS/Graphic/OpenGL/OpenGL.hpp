#pragma once

#include <File/PXDataStream.h>
#include <CS/OS/Error/ActionResult.hpp>
#include <OS/Graphic/OpenGL/OpenGL.h>

namespace PX
{
	public enum class OpenGLVersion
	{
		Invalid,
		None,
		GLV1x0x0,
		GLV1x1x0, // 1995              Texture objects, Vertex Arrays
		GLV1x2x0, // 1998 March    16. 3D textures, BGRA and packed pixel formats, introduction of the imaging subset useful to image - processing applications
		GLV1x2x1, // 1998 October  14. A concept of ARB extensions
		GLV1x3x0, // 2001 August   14. Multitexturing, multisampling, texture compression
		GLV1x4x0, // 2002 July     24. Depth textures, GLSlang
		GLV1x5x0, // 2003 July     29. Vertex Buffer Object(VBO), Occlusion Queries
		GLV2x0x0, // 2004 September 7. GLSL 1.10, MRT, Non Power of Two textures, Point Sprites, Two - sided stencil
		GLV2x1x0, // 2006 July      2. GLSL 1.20, Pixel Buffer Object(PBO), sRGB Textures
		GLV3x0x0, // 2008 August   11. GLSL 1.30, Texture Arrays, Conditional rendering, Frame Buffer Object(FBO)
		GLV3x1x0, // 2009 March    24. GLSL 1.40, Instancing, Texture Buffer Object, Uniform Buffer Object, Primitive restart
		GLV3x2x0, // 2009 August    3. GLSL 1.50, Geometry Shader, Multi - sampled textures
		GLV3x3x0, // 2010 March    11. GLSL 3.30, Backports as much function as possible from the OpenGL 4.0 specification
		GLV4x0x0, // 2010 March    11. GLSL 4.00, Tessellation on GPU, shaders with 64 - bit precision
		GLV4x1x0, // 2010 July     26. GLSL 4.10, Developer - friendly debug outputs, compatibility with OpenGL ES 2.0
		GLV4x2x0, // 2011 August    8. GLSL 4.20, Shaders with atomic counters, draw transform feedback instanced, shader packing, performance improvements
		GLV4x3x0, // 2012 August    6. GLSL 4.30, Compute shaders leveraging GPU parallelism, shader storage buffer objects, high - quality ETC2 / EAC texture compression, increased memory security, a multi - application robustness extension, compatibility with OpenGL ES 3.0
		GLV4x4x0, // 2013 July     22. GLSL 4.40, Buffer Placement Control, Efficient Asynchronous Queries, Shader Variable Layout, Efficient Multiple Object Binding, Streamlined Porting of Direct3D applications, Bindless Texture Extension, Sparse Texture Extension
		GLV4x5x0,	// 2014 August   11. GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness, OpenGL ES 3.1 API and shader compatibility, DX11 emulation features
		GLV4x6x0	// 2017 July     31. GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering
	};

	public ref class OpenGL
	{
		private:
		OpenGLContext* Context;

		public:
		OpenGL();
		~OpenGL();

		property OpenGLVersion Version { OpenGLVersion get() { return (OpenGLVersion)Context->Version; }}
		property System::String^ Vendor { System::String^ get() { return gcnew System::String(Context->Vendor); }}
		property System::String^ Renderer { System::String^ get() { return  gcnew System::String(Context->Renderer); }}
		property System::String^ VersionText { System::String^ get() { return gcnew System::String(Context->VersionText); }}
		property System::String^ GLSLVersionText { System::String^ get() { return gcnew System::String(Context->GLSLVersionText); }}

		bool ContextCreateForWindow();
		void ContextCreateWindowless(const PXSize width, const PXSize height);
	};
}