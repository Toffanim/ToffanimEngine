// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
/* ========================================================================
   $File: $
   $Modified: 06-02-2017 07h50m20s $
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
#include <glew/glew.h>
#include "../utils/Utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Shader
{
public:
    Shader(std::string name);
    ~Shader();
    void attach( GLuint shader );
    void attach( int type, const char* filename);
    void link();
    void unuse();
    GLuint getProgram() { return(program_handle); }
    void use();
    GLuint operator[](int idx){ return(handles[idx]);}

	void SetInt(const std::string& UniformName, int Value);
	void SetFloat(const std::string& UniformName, float Value);
	void SetVector3(const std::string& UniformName, const glm::vec3& Value);
	void SetVector4(const std::string& UniformName, const glm::vec4& Value);
	void SetMatrix4(const std::string& UniformName, const glm::mat4& Value);
private :
    std::vector<GLuint> handles;
    GLuint program_handle;
    std::string name;

    char* read_file(const char* name);
    
};

#endif
