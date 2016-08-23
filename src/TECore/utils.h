#if !defined(UTILS_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define UTILS_H

#include <stdlib.h>
#include <string>
#include <memory>

#include "GL/glew.h"
#include "stb_image.h"
#include "TEDebug\debug_memory.h"

#define NON_COPYABLE_CLASS(Class) \
Class(const Class&) = delete;\
Class& operator=(const Class&) = delete;


#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

namespace TE
{
	namespace Core
	{

		std::unique_ptr<unsigned char> LoadImage(const char* Path, int* Width, int* Height, int* Comp, unsigned int Channels);


		//Is there on or more error since last call ?
		static bool CheckOpenGLError(const char* Title)
		{
			int Error;
			if ((Error = glGetError()) != GL_NO_ERROR)
			{
				std::string ErrorString;
				switch (Error)
				{
				case GL_INVALID_ENUM:
					ErrorString = "GL_INVALID_ENUM";
					break;
				case GL_INVALID_VALUE:
					ErrorString = "GL_INVALID_VALUE";
					break;
				case GL_INVALID_OPERATION:
					ErrorString = "GL_INVALID_OPERATION";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					ErrorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;
				case GL_OUT_OF_MEMORY:
					ErrorString = "GL_OUT_OF_MEMORY";
					break;
				default:
					ErrorString = "UNKNOWN";
					break;
				}
				fprintf(stdout, "OpenGL Error(%s): %s\n", ErrorString.c_str(), Title);
			}
			return (Error == GL_NO_ERROR);
		}
	}
}

#endif
