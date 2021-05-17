#include "plateform_opengl_win32.h"

// TODO (Toffa): This is a bit ugly
// I'm not even sure that we need to have gl functions as globalq
// maybe we should wrap it inside a "renderer"object?
#define DECLARE_FUN(type,name) type name;
#define GET_FUN(type, name) name = (type)GetAnyGLFuncAddress(#name);

DECLARE_FUN(PFNGLCREATESHADERPROC, glCreateShader)
DECLARE_FUN(PFNGLSHADERSOURCEPROC, glShaderSource)
DECLARE_FUN(PFNGLCOMPILESHADERPROC, glCompileShader)
DECLARE_FUN(PFNGLGETSHADERIVPROC, glGetShaderiv)
DECLARE_FUN(PFNGLGETSHADERINFOLOGPROC,glGetShaderInfoLog)
DECLARE_FUN(PFNGLATTACHSHADERPROC,glAttachShader)
DECLARE_FUN(PFNGLLINKPROGRAMPROC,glLinkProgram)
DECLARE_FUN(PFNGLGETPROGRAMIVPROC,glGetProgramiv)
DECLARE_FUN(PFNGLGETPROGRAMINFOLOGPROC,glGetProgramInfoLog)
DECLARE_FUN(PFNGLDELETESHADERPROC,glDeleteShader)
DECLARE_FUN(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)
DECLARE_FUN(PFNGLGENBUFFERSPROC, glGenBuffers)
DECLARE_FUN(PFNGLBINDVERTEXARRAYPROC,glBindVertexArray)
DECLARE_FUN(PFNGLBINDBUFFERPROC,glBindBuffer)
DECLARE_FUN(PFNGLBUFFERDATAPROC,glBufferData)
DECLARE_FUN(PFNGLVERTEXATTRIBPOINTERPROC,glVertexAttribPointer)
DECLARE_FUN(PFNGLENABLEVERTEXATTRIBARRAYPROC,glEnableVertexAttribArray)
DECLARE_FUN(PFNGLCREATEPROGRAMPROC, glCreateProgram)
DECLARE_FUN(PFNGLUSEPROGRAMPROC, glUseProgram)

namespace TE { namespace Core { namespace Renderer {

struct renderer {
};

void Init(renderer& Renderer, HWND Window) {
    PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC DC = GetDC(Window);

// TODO (Toffa) : use a more advanced strategy to chose PF
		int PF = ChoosePixelFormat(DC, &pfd); 
		SetPixelFormat(DC, PF, &pfd);

		HGLRC Ctx = wglCreateContext(DC);
		wglMakeCurrent (DC, Ctx);	

GET_FUN(PFNGLCREATESHADERPROC, glCreateShader)
GET_FUN(PFNGLSHADERSOURCEPROC, glShaderSource)
GET_FUN(PFNGLCOMPILESHADERPROC, glCompileShader)
GET_FUN(PFNGLGETSHADERIVPROC, glGetShaderiv)
GET_FUN(PFNGLGETSHADERINFOLOGPROC,glGetShaderInfoLog)
GET_FUN(PFNGLATTACHSHADERPROC,glAttachShader)
GET_FUN(PFNGLLINKPROGRAMPROC,glLinkProgram)
GET_FUN(PFNGLGETPROGRAMIVPROC,glGetProgramiv)
GET_FUN(PFNGLGETPROGRAMINFOLOGPROC,glGetProgramInfoLog)
GET_FUN(PFNGLDELETESHADERPROC,glDeleteShader)
GET_FUN(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)
GET_FUN(PFNGLGENBUFFERSPROC, glGenBuffers)
GET_FUN(PFNGLBINDVERTEXARRAYPROC,glBindVertexArray)
GET_FUN(PFNGLBINDBUFFERPROC,glBindBuffer)
GET_FUN(PFNGLBUFFERDATAPROC,glBufferData)
GET_FUN(PFNGLVERTEXATTRIBPOINTERPROC,glVertexAttribPointer)
GET_FUN(PFNGLENABLEVERTEXATTRIBARRAYPROC,glEnableVertexAttribArray)
GET_FUN(PFNGLCREATEPROGRAMPROC, glCreateProgram)
GET_FUN(PFNGLUSEPROGRAMPROC, glUseProgram)
}

}
}
}