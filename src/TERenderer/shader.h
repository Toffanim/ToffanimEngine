#if !defined(SHADER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */

#define SHADER_H

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include "TECore\utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace TE
{
	namespace Renderer
	{
		class shader
		{
		public:

			enum type
			{
				VERTEX = GL_VERTEX_SHADER,
				FRAGMENT = GL_FRAGMENT_SHADER,
			};

			shader(std::string Name);
			~shader();
			void Attach(GLuint Shader);
			void Attach(type Type, const char* Filename);
			void Link();
			void Unuse() const;
			GLuint GetProgram() const { return(_ProgramHandle); }
			void Use() const;
			GLuint operator[](int idx){ return(_Handles[idx]); }

			void SetInt(const std::string& UniformName, int Value) const;
			void SetFloat(const std::string& UniformName, float Value) const;
			void SetVector3(const std::string& UniformName, const glm::vec3& Value) const;
			void SetVector4(const std::string& UniformName, const glm::vec4& Value) const;
			void SetMatrix4(const std::string& UniformName, const glm::mat4& Value) const;
		private:
			std::vector<GLuint> _Handles;
			GLuint _ProgramHandle;
			std::string _Name;

			const char* ReadFile(const char* Name) const;

		};
	}
}
#endif
