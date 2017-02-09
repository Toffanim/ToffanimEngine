// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TERenderer\shader.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m25s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
  ======================================================================== */
#include "Shader.h"
using namespace std;

namespace TE
{
	namespace Renderer
	{
		//Shader encapsulation
		shader::shader(string Name) :_Name(Name)
		{
		}

		shader::~shader()
		{
			for (int i = 0; i < _Handles.size(); i++)
			{
				glDetachShader(_ProgramHandle, _Handles[i]);
				glDeleteShader(_Handles[i]);
			}
			glDeleteProgram(_ProgramHandle);
			glUseProgram(0);
		}

		void shader::Bind() const
		{
			glUseProgram(_ProgramHandle);
		}

		void shader::Unuse() const
		{
			glUseProgram(0);
		}

		void shader::Attach(GLuint Shader)
		{
			_Handles.push_back(Shader);
		}

		void shader::Attach(type Type, const char* Filename)
		{
			auto Mem = ReadFile(Filename);
			auto Handle = glCreateShader(Type);
			glShaderSource(Handle, 1, (const GLchar**)(&Mem), 0);
			glCompileShader(Handle);

			GLint compileSuccess = 0;

			glGetShaderiv(Handle, GL_COMPILE_STATUS, &compileSuccess);
			if (!compileSuccess)
			{
				int LogLength;
				glGetShaderiv(Handle, GL_INFO_LOG_LENGTH, &LogLength);
				char * Log = new char[LogLength];
				glGetShaderInfoLog(Handle, LogLength, &LogLength, Log);
				printf("Shader %s\n%s\ncompileSuccess=%d\n", Filename, Log, compileSuccess);
				delete[] Log;
				return;
			}
			_Handles.push_back(Handle);
			delete[] Mem;
		}

		void shader::Link()
		{
			_ProgramHandle = glCreateProgram();
			for (int i = 0; i < _Handles.size(); i++)
			{
				glAttachShader(_ProgramHandle, _Handles[i]);
			}
			glLinkProgram(_ProgramHandle);

			GLint LinkSuccess;
			glGetProgramiv(_ProgramHandle, GL_LINK_STATUS, &LinkSuccess);
			if (!LinkSuccess)
			{
				int LogLength;
				glGetProgramiv(_ProgramHandle, GL_INFO_LOG_LENGTH, &LogLength);
				char* Log = new char[LogLength];
				glGetProgramInfoLog(_ProgramHandle, LogLength, &LogLength, Log);
				printf("Shader Linker:\n%s\nlinkSuccess=%d\n", Log, LinkSuccess);
				delete[] Log;
				return;
			}
			printf("%s linked successful\n", _Name.c_str());

		}

		void shader::SetInt(const std::string& UniformName, int Value) const
		{
			glUniform1i(glGetUniformLocation(_ProgramHandle, UniformName.c_str()), Value);
			Core::CheckOpenGLError("Error trying to set int");
		}
		void shader::SetFloat(const std::string& UniformName, float Value) const
		{
			glUniform1f(glGetUniformLocation(_ProgramHandle, UniformName.c_str()), Value);
			Core::CheckOpenGLError("Error trying to set float");
		}
		void shader::SetVector2(const std::string& UniformName, const glm::vec2& Value) const
		{
			glUniform2fv(glGetUniformLocation(_ProgramHandle, UniformName.c_str()), 1, &Value[0]);
			Core::CheckOpenGLError("Error trying to set vector3");
		}
		void shader::SetVector3(const std::string& UniformName, const glm::vec3& Value) const
		{
			glUniform3fv(glGetUniformLocation(_ProgramHandle, UniformName.c_str()), 1, &Value[0]);
			Core::CheckOpenGLError("Error trying to set vector3");
		}
		void shader::SetVector4(const std::string& UniformName, const glm::vec4& Value) const
		{
			glUniform4fv(glGetUniformLocation(_ProgramHandle, UniformName.c_str()), 1, &Value[0]);
			Core::CheckOpenGLError("Error trying to set vector4");
		}
		void shader::SetMatrix4(const std::string& UniformName, const glm::mat4& Value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(_ProgramHandle, UniformName.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
			Core::CheckOpenGLError("Error trying to set matrix 4*4");
		}

		const char* shader::ReadFile(const char* name) const
		{
			FILE * fp = fopen(name, "rb");

			if (fp == 0)
			{
				printf("File %s NOT FOUND\n", name);
				while (1);;
			}
			fseek(fp, 0L, SEEK_END);
			int fsize = ftell(fp);
			fseek(fp, 0L, SEEK_SET);
			char* mem = (char*)malloc(fsize + 1);
			for (int i = 0; i < fsize + 1; i++)mem[i] = 0;
			fread(mem, 1, fsize, fp);
			fclose(fp);
			return mem;
		}

	}
}
